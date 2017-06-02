/******************************************************************
 *
 * Copyright 2015 Intel Corporation All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include "caleinterface.h"
#include "bluez.h"
#include "central.h"
#include "peripheral.h"
#include "client.h"
#include "server.h"
#include "utils.h"

#include "cagattservice.h"
#include "oic_malloc.h"
#include "oic_string.h"
#include "logger.h"

#include <string.h>
#include <strings.h>  // For strcasecmp().
#include <assert.h>


#define MICROSECS_PER_SEC 1000000

// Logging tag.
static char const TAG[] = "BLE_INTERFACE";

/*
    The IoTivity adapter interface currently doesn't provide a means to
    pass context down to the transport layer so rely on a file scope
    context instead.
*/
static CALEContext g_context = {
    .lock = NULL
};

// -----------------------------------------------------------------------
// Functions internal to this BLE adapter implementation.
// -----------------------------------------------------------------------
static bool CALESetUpBlueZObjects(CALEContext * context);

static bool CALECheckStarted()
{
    oc_mutex_lock(g_context.lock);

    bool const started =
        (g_context.event_loop != NULL
         && g_main_loop_is_running(g_context.event_loop));

    oc_mutex_unlock(g_context.lock);

    /**
     * @todo Fix potential TOCTOU race condition.  A LE transport
     *       adapter could have been started or stopped between the
     *       mutex unlock and boolean check.
     */
    return started;
}

static void CALEDumpDBusSignalParameters(char const * sender_name,
                                         char const * object_path,
                                         char const * interface_name,
                                         char const * signal_name,
                                         GVariant   * parameters)
{
    (void)sender_name;
    (void)object_path;
    (void)interface_name;
    (void)signal_name;
    (void)parameters;
#ifdef TB_LOG
    gchar * const param_dump =
        g_variant_print(parameters, TRUE);

    OIC_LOG_V(DEBUG,
              TAG,
              "%s()\n"
              "\tsender_name: %s\n"
              "\tobject_path: %s\n"
              "\tinterface_name: %s\n"
              "\tsignal_name: %s\n"
              "\tparameters: %s\n",
              __func__,
              sender_name,
              object_path,
              interface_name,
              signal_name,
              param_dump);

    g_free(param_dump);
#endif  // TB_LOG
}

static void CALEOnInterfaceProxyPropertiesChanged(
    GDBusObjectManagerClient * manager,
    GDBusObjectProxy         * object_proxy,
    GDBusProxy               * interface_proxy,
    GVariant                 * changed_properties,
    gchar const * const      * invalidated_properties,
    gpointer                   user_data)
{
    (void)manager;
    (void)object_proxy;
    (void)invalidated_properties;

    char const * const interface_name =
        g_dbus_proxy_get_interface_name(interface_proxy);

    bool const is_adapter_interface =
        (strcmp(BLUEZ_ADAPTER_INTERFACE, interface_name) == 0);

    if (!is_adapter_interface)
    {
        /*
          Only specific org.bluez.Adapter1 property changes are
          currently supported.
        */
        return;
    }

    OIC_LOG_V(DEBUG,
              TAG,
              "%s properties Changed on %s:\n",
              interface_name,
              g_dbus_proxy_get_object_path(interface_proxy));

    CALEContext * const context = user_data;

    GVariantIter iter;
    gchar const * key   = NULL;
    GVariant    * value = NULL;

    /**
     * @todo Since we're only looking up one value here,
     *       i.e. "Powered", can't we just use
     *       g_variant_lookup_value() instead of this while() loop?
     */
    g_variant_iter_init(&iter, changed_properties);
    while (g_variant_iter_next(&iter, "{&sv}", &key, &value))
    {
        if (strcmp(key, "Powered") == 0)
        {
            /*
              Report a change in the availability of the bluetooth
              adapter.
            */
            gboolean const powered = g_variant_get_boolean(value);
            CAAdapterState_t const status =
                (powered ? CA_ADAPTER_ENABLED : CA_ADAPTER_DISABLED);

            /**
             * @todo Should we acquire the context lock here to
             *       prevent the @c CALEDeviceStateChangedCallback
             *       from being potentially yanked out from under us
             *       if the CA adapters are stopped/terminated as
             *       we're about to invoke this callback?
             *
             * @todo Unfortunately the CA LE interface defined in
             *       caleinterface.h assumes that only one BLE adapter
             *       will exist on a given host.  However, this
             *       implementation can handle multiple BLE adapters.
             *       The CA LE interface should be updated so that it
             *       can handle multiple BLE adapters.
             */
            context->on_device_state_changed(status);
        }

#ifdef TB_LOG
        gchar * const s = g_variant_print(value, TRUE);
        OIC_LOG_V(DEBUG, TAG, "  %s -> %s", key, s);
        g_free(s);
#endif  // TB_LOG

        g_variant_unref(value);
    }
}

static void CALEHandleInterfaceAdded(GList ** proxy_list,
                                     char const * interface,
                                     GVariant * parameters)
{
    /**
     * @note The @a parameters are of the form "(oa{sv})".
     */
    GDBusProxy * const proxy =
        CAGetBlueZInterfaceProxy(parameters,
                                 interface,
                                 g_context.object_manager);

    if (proxy == NULL)
    {
        return;
    }

    oc_mutex_lock(g_context.lock);

    /*
      Add the object information to the list.

      Note that we prepend instead of append in this case since it
      is more efficient to do so for linked lists like the one used
      here.
    */
    *proxy_list = g_list_prepend(*proxy_list, proxy);

    oc_mutex_unlock(g_context.lock);

    /**
     * Let the thread that may be blocked waiting for Devices to be
     * discovered know that at least one was found.
     *
     * @todo It doesn't feel good putting this @c org.bluez.Device1
     *       specific code here since this function is meant to be
     *       BlueZ interface neutral.  Look into ways of moving this
     *       out of here.
     */
    if (strcmp(interface, BLUEZ_DEVICE_INTERFACE) == 0)
    {
        oc_cond_signal(g_context.condition);
    }
}

static void CALEOnInterfacesAdded(GDBusConnection * connection,
                                  char const * sender_name,
                                  char const * object_path,
                                  char const * interface_name,
                                  char const * signal_name,
                                  GVariant   * parameters,
                                  gpointer     user_data)
{
    (void)connection;
    (void)user_data;
    CALEDumpDBusSignalParameters(sender_name,
                                 object_path,
                                 interface_name,
                                 signal_name,
                                 parameters);

    // The signal should always be InterfacesAdded.
    assert(strcmp(signal_name, "InterfacesAdded") == 0);

    // Handle addition of a new org.bluez.Adapter1 interface.
    CALEHandleInterfaceAdded(&g_context.adapters,
                             BLUEZ_ADAPTER_INTERFACE,
                             parameters);

    // Handle addition of a new org.bluez.Device1 interface.
    CALEHandleInterfaceAdded(&g_context.devices,
                             BLUEZ_DEVICE_INTERFACE,
                             parameters);
}

static void CALEOnInterfacesRemoved(GDBusConnection * connection,
                                    char const * sender_name,
                                    char const * object_path,
                                    char const * interface_name,
                                    char const * signal_name,
                                    GVariant   * parameters,
                                    gpointer     user_data)
{
    (void)connection;
    (void)user_data;
    CALEDumpDBusSignalParameters(sender_name,
                                 object_path,
                                 interface_name,
                                 signal_name,
                                 parameters);

    // The signal should always be InterfacesRemoved.
    assert(strcmp(signal_name, "InterfacesRemoved") == 0);

    /*
      The object path is first tuple element, and the interface names
      the second.  Check if "org.bluez.Adapter1" exists in the
      interface array.  If it does, remove the corresponding
      information from the adapter_infos list.
    */
    GVariant * const interfaces =
        g_variant_get_child_value(parameters, 1);

    GVariantIter * iter = NULL;
    g_variant_get(interfaces, "as", &iter);

    /**
     * Iterate over the array and remove all BlueZ interface proxies
     * with a matching D-Bus object path from the corresponding list.
     *
     * @todo Determine whether we should optimize this nested loop.
     *       It may not be worthwhile to do so since the lists being
     *       iterated over should be very short.
     */
    for (GVariant * child = g_variant_iter_next_value(iter);
         child != NULL;
         child = g_variant_iter_next_value(iter))
    {
        char const * interface = NULL;
        g_variant_get(child, "&s", &interface);

        GList ** list = NULL;

        if (strcmp(interface, BLUEZ_ADAPTER_INTERFACE) == 0)
        {
            list = &g_context.adapters;
        }
        else if (strcmp(interface, BLUEZ_DEVICE_INTERFACE) == 0)
        {
            list = &g_context.devices;
        }
        else
        {
            continue;
        }

        // The object path is the first tuple element.
        gchar const * path = NULL;
        g_variant_get_child(parameters, 0, "&o", &path);

        oc_mutex_lock(g_context.lock);

        for (GList * l = *list; l != NULL; l = g_list_next(l))
        {
            GDBusProxy * const proxy = G_DBUS_PROXY(l->data);

            if (strcmp(path,
                       g_dbus_proxy_get_object_path(proxy)) == 0)
            {
                /**
                 * @todo If a BlueZ Device was removed, update the
                 *       characteristic map, accordingly.
                 */

                // Found a match!
                g_object_unref(proxy);

                *list = g_list_delete_link(*list, l);

                break;
            }
        }

        oc_mutex_unlock(g_context.lock);

        g_variant_unref(child);
    }

    if (iter != NULL)
    {
        g_variant_iter_free(iter);
    }
}

static void CALESubscribeToSignals(CALEContext * context,
                                   GDBusConnection * connection,
                                   GDBusObjectManager * object_manager)
{
    static char const om_interface[] =
        "org.freedesktop.DBus.ObjectManager";

    /*
      Subscribe to D-Bus signals that will allow us to detect changes
      in BlueZ adapter and device properties.
     */
    guint const interfaces_added_sub_id =
        g_dbus_connection_signal_subscribe(
            connection,
            NULL,  // sender
            om_interface,
            "InterfacesAdded",
            NULL,  // object path
            NULL,  // arg0
            G_DBUS_SIGNAL_FLAGS_NONE,
            CALEOnInterfacesAdded,
            NULL,  // user_data
            NULL);

    guint const interfaces_removed_sub_id =
        g_dbus_connection_signal_subscribe(
            connection,
            NULL,  // sender
            om_interface,
            "InterfacesRemoved",
            NULL,  // object path
            NULL,  // arg0
            G_DBUS_SIGNAL_FLAGS_NONE,
            CALEOnInterfacesRemoved,
            NULL,  // user_data
            NULL);

    g_signal_connect(object_manager,
                     "interface-proxy-properties-changed",
                     G_CALLBACK(CALEOnInterfaceProxyPropertiesChanged),
                     context);

    oc_mutex_lock(context->lock);

    context->interfaces_added_sub_id   = interfaces_added_sub_id;
    context->interfaces_removed_sub_id = interfaces_removed_sub_id;

    oc_mutex_unlock(context->lock);
}

static bool CALESetUpDBus(CALEContext * context)
{
    assert(context != NULL);

    bool success = false;

    GError * error = NULL;

    /*
      Set up connection to the D-Bus system bus, where the BlueZ
      daemon is found.
    */
    GDBusConnection * const connection =
        g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);

    if (connection == NULL)
    {
        OIC_LOG_V(ERROR,
                  TAG,
                  "Connection to D-Bus system bus failed: %s.",
                  error->message);

        g_error_free(error);

        return success;
    }

    // Create a proxy to the BlueZ D-Bus ObjectManager.
    static char const object_manager_path[] = "/";

    GDBusObjectManager * const object_manager =
        g_dbus_object_manager_client_new_sync(
            connection,
            G_DBUS_OBJECT_MANAGER_CLIENT_FLAGS_NONE,
            BLUEZ_NAME,
            object_manager_path,
            NULL,   // get_proxy_type_func
            NULL,   // get_proxy_type_user_data
            NULL,   // get_proxy_type_destroy_notify
            NULL,   // cancellable
            &error);

    if (object_manager == NULL)
    {
        OIC_LOG_V(ERROR,
                  TAG,
                  "Unable to create D-Bus ObjectManager client: %s",
                  error->message);

        g_error_free(error);

        g_object_unref(connection);

        return success;
    }

    CALESubscribeToSignals(context, connection, object_manager);

    oc_mutex_lock(context->lock);
    context->connection     = connection;
    context->object_manager = object_manager;
    oc_mutex_unlock(context->lock);

    success = CALESetUpBlueZObjects(context);

    return success;
}

static void CALETearDownDBus(CALEContext * context)
{
    assert(context != NULL);

    /*
      Minimize the time we hold the global lock by only clearing the
      global state, and pushing resource finalization outside the global
      lock.
    */
    oc_mutex_lock(context->lock);

    GDBusConnection * const connection = context->connection;
    context->connection = NULL;

    GDBusObjectManager * const object_manager = context->object_manager;
    context->object_manager = NULL;

    GList * const objects = context->objects;
    context->objects = NULL;

    GList * const adapters = context->adapters;
    context->adapters = NULL;

    GList * const devices = context->devices;
    context->devices = NULL;

    guint const interfaces_added   = context->interfaces_added_sub_id;
    guint const interfaces_removed = context->interfaces_removed_sub_id;

    context->interfaces_added_sub_id   = 0;
    context->interfaces_removed_sub_id = 0;

    oc_mutex_unlock(context->lock);

    // Destroy the device proxies list.
    g_list_free_full(devices, g_object_unref);

    // Destroy the adapter proxies list.
    g_list_free_full(adapters, g_object_unref);

    // Destroy the list of objects obtained from the ObjectManager.
    g_list_free_full(objects, g_object_unref);

    // Destroy the ObjectManager proxy.
    if (object_manager != NULL)
    {
        g_object_unref(object_manager);
    }

    // Tear down the D-Bus connection to the system bus.
    if (connection != NULL)
    {
        g_dbus_connection_signal_unsubscribe(connection,
                                             interfaces_added);
        g_dbus_connection_signal_unsubscribe(connection,
                                             interfaces_removed);
        g_object_unref(connection);
    }
}

static bool CALEDeviceFilter(GDBusProxy * device)
{
    bool accepted = false;

    /*
      Filter out any devices that don't support the OIC Transport
      Profile service.
    */
    GVariant * const prop =
        g_dbus_proxy_get_cached_property(device, "UUIDs");

    if (prop == NULL)
    {
        // No remote services available on the device.
        return accepted;
    }

    gsize length = 0;
    char const ** const UUIDs = g_variant_get_strv(prop, &length);

    /*
      It would have been nice to use g_strv_contains() here, but we
      would need to run it twice: once for the uppercase form of the
      UUID and once for for the lowercase form.  Just run the loop
      manually, and use strcasecmp() instead.
    */
    char const * const * const end = UUIDs + length;
    for (char const * const * u = UUIDs; u != end; ++u)
    {
        if (strcasecmp(*u, CA_GATT_SERVICE_UUID) == 0)
        {
            accepted = true;
            break;
        }
    }

    g_free(UUIDs);
    g_variant_unref(prop);

    return accepted;
}

static bool CALESetUpBlueZObjects(CALEContext * context)
{
    bool success = false;

    // Get the list of BlueZ D-Bus objects.
    GList * const objects =
        g_dbus_object_manager_get_objects(context->object_manager);

    if (objects == NULL) {
        OIC_LOG(ERROR,
                TAG,
                "Unable to get objects from ObjectManager.");

        return success;
    }

    oc_mutex_lock(context->lock);
    context->objects = objects;
    oc_mutex_unlock(context->lock);

    /*
      Create a proxies to the org.bluez.Adapter1 D-Bus objects that
      will later be used to obtain local bluetooth adapter properties,
      as well as by the BLE central code to discover peripherals.
    */
    GList * adapters = NULL;
    success = CAGetBlueZManagedObjectProxies(&adapters,
                                             BLUEZ_ADAPTER_INTERFACE,
                                             context,
                                             NULL);

    // An empty adapters list is NULL.
    if (success && adapters != NULL)
    {
        oc_mutex_lock(context->lock);
        context->adapters = adapters;
        oc_mutex_unlock(context->lock);
    }

    /*
      Create a proxies to the org.bluez.Device1 D-Bus objects that
      will later be used to establish connections.
    */
    GList * devices = NULL;
    success = CAGetBlueZManagedObjectProxies(&devices,
                                             BLUEZ_DEVICE_INTERFACE,
                                             context,
                                             CALEDeviceFilter);

    // An empty device list is NULL.
    if (success && devices != NULL)
    {
        oc_mutex_lock(context->lock);
        context->devices = devices;
        oc_mutex_unlock(context->lock);
    }

    return success;
}

/**
 * Inform thread waiting for the event loop to start that the loop has
 * started.  This is done in the context of the event loop itself so
 * that we can be certain that the event loop is indeed running.
 */
static gboolean CALEEventLoopStarted(gpointer user_data)
{
    sem_t * const sem = user_data;

    (void) sem_post(sem);

    return G_SOURCE_REMOVE;
}

static void CALEStartEventLoop(void * data)
{
    CALEContext * const context = data;

    assert(context != NULL);

    // Create the event loop.
    GMainContext * const loop_context = g_main_context_new();
    GMainLoop * const event_loop = g_main_loop_new(loop_context, FALSE);

    g_main_context_push_thread_default(loop_context);

    /*
      We have to do the BlueZ object manager client initialization and
      signal subscription here so that the corresponding asynchronous
      signal handling occurs in the same thread as the one running the
      GLib event loop.
    */
    if (CALESetUpDBus(&g_context))
    {
        oc_mutex_lock(context->lock);

        assert(context->event_loop == NULL);
        context->event_loop = event_loop;

        oc_mutex_unlock(context->lock);

        /*
          Add an idle handler that notifies a thread waiting for the
          GLib event loop to run that the event loop is actually
          running.  We do this in the context of the event loop itself
          to avoid race conditions.
        */
        GSource * const source = g_idle_source_new();
        g_source_set_priority(source, G_PRIORITY_HIGH_IDLE);
        g_source_set_callback(source,
                              CALEEventLoopStarted,
                              &context->le_started,  // data
                              NULL);                 // notify
        (void) g_source_attach(source, loop_context);
        g_source_unref(source);

        g_main_loop_run(event_loop);  // Blocks until loop is quit.

        CALETearDownDBus(&g_context);
    }

    /*
      Clean up in the same thread to avoid having to explicitly bump
      the ref count to retain ownership.
    */
    g_main_context_unref(loop_context);
    g_main_loop_unref(event_loop);
}

static void CALEStopEventLoop(CALEContext * context)
{
    oc_mutex_lock(context->lock);

    GMainLoop * const event_loop = context->event_loop;
    context->event_loop = NULL;

    oc_mutex_unlock(context->lock);

    if (event_loop != NULL)
    {
        g_main_loop_quit(event_loop);

        GMainContext * const loop_context =
            g_main_loop_get_context(event_loop);

        if (loop_context != NULL)
        {
            g_main_context_wakeup(loop_context);
        }
    }
}

/**
 * Wait for @a list to be non-empty.
 *
 * @param[in] list    List that should not be empty.
 * @param[in] retries Number of times to retry if the @a timeout is
 *                    reached.
 * @param[in] timeout Timeout in microseconds to wait between retries.
 */
static bool CALEWaitForNonEmptyList(GList * const * list,
                                    int retries,
                                    uint64_t timeout)
{
    bool success = false;

    oc_mutex_lock(g_context.lock);

    for (int i = 0; *list == NULL && i < retries; ++i)
    {
        if (oc_cond_wait_for(g_context.condition,
                             g_context.lock,
                             timeout) == OC_WAIT_SUCCESS)
        {
            /*
              Condition variable was signaled before the timeout was
              reached.
            */
            success = true;
        }
    }

    oc_mutex_unlock(g_context.lock);

    return success;
}

// -----------------------------------------------------------------------

CAResult_t CAInitializeLEAdapter()
{
#if !GLIB_CHECK_VERSION(2,36,0)
    /*
      Initialize the GLib type system.

      As of GLib 2.36, it is no longer necessary to explicitly call
      g_type_init().
    */
    g_type_init();
#endif

    return CA_STATUS_OK;
}

CAResult_t CAStartLEAdapter()
{
    /*
      This function is called by the connectivity abstraction when
      CASelectNetwork(CA_ADAPTER_GATT_BTLE) is called by the user.
    */

    OIC_LOG(DEBUG, TAG, __func__);

    CAResult_t result = CA_STATUS_FAILED;

    // Only start if we were previously stopped.
    if (CALECheckStarted())
    {
        return result;
    }

    /**
     * Spawn a thread to run the GLib event loop that will drive D-Bus
     * signal handling.
     *
     * @note Ideally this should be done in the @c CAInitializeLE()
     *       function so that we can detect local bluetooth adapter
     *       changes right away, without having to first start this LE
     *       adapter/transport via @cCASelectNetwork().  However, a
     *       limitation in the CA termination code that destroys the
     *       thread pool before the transport adapters prevents us
     *       from doing that without potentially triggering a
     *       @c pthread_join() call that blocks indefinitely due to
     *       this event loop not being stopped.  See the comments in
     *       the @c CAGetLEInterfaceInformation() function below for
     *       further details.
     */
    result = ca_thread_pool_add_task(g_context.client_thread_pool,
                                     CALEStartEventLoop,
                                     &g_context, NULL);

    /*
      Wait for the GLib event loop to actually run before returning.

      This addresses corner cases where operations are incorrectly
      permitted to run in parallel before the event loop is run. For
      example, the LE transport could have been stopped in a thread
      parallel to the one starting the event loop start.  In that case
      the GLib event loop may never exit since the stop operation that
      causes the event loop to exit occurred before event loop
      started.  That ultimately causes the CA layer termination to
      block indefinitely on a pthread_join().  The solution is to only
      return from the LE transport start operation once we know the
      event loop is up and running.
    */
    struct timespec abs_timeout;
    if (result == CA_STATUS_OK
        && clock_gettime(CLOCK_REALTIME, &abs_timeout) == 0)
    {
        static time_t const relative_timeout = 2;  // seconds
        abs_timeout.tv_sec += relative_timeout;

        int const wait_result =
            sem_timedwait(&g_context.le_started, &abs_timeout);

        if (wait_result == 0)
        {
            result = CA_STATUS_OK;
        }
    }

    return result;
}

CAResult_t CAStopLEAdapter()
{
    /*
      This function is called by the connectivity abstraction when
      CAUnselectNetwork(CA_ADAPTER_GATT_BTLE) is called by the user.
    */

    OIC_LOG(DEBUG, TAG, "Stop Linux BLE adapter.");

    // Only stop if we were previously started.
    if (!CALECheckStarted())
    {
        return CA_STATUS_FAILED;
    }

    CALEStopEventLoop(&g_context);

    return CA_STATUS_OK;
}


CAResult_t CAGetLEAdapterState()
{
    CAResult_t result = CA_ADAPTER_NOT_ENABLED;

    oc_mutex_lock(g_context.lock);

    for (GList * l = g_context.adapters; l != NULL; l = l->next)
    {
        GDBusProxy * const adapter = G_DBUS_PROXY(l->data);
        GVariant * const prop =
            g_dbus_proxy_get_cached_property(adapter, "Powered");

        if (prop == NULL)
        {
            // This should never happen!
            result = CA_STATUS_FAILED;
            break;
        }

        gboolean const powered = g_variant_get_boolean(prop);
        g_variant_unref(prop);

        if (powered)
        {
            result = CA_STATUS_OK;
            break;

            /*
              No need to continue iterating since we have at least
              one enabled Bluetooth adapter.
            */
        }
    }

    oc_mutex_unlock(g_context.lock);

    return result;
}

CAResult_t CAInitializeLENetworkMonitor()
{
    /**
     * @note "Network monitor" operations are started in the
     *       @c CAStartLEAdapter() function rather than this function
     *       due to glib/D-Bus signal handling threads related
     *       issues.
     *
     * @see @c CAStartLEAdapter() for further details.
     */

    g_context.lock      = oc_mutex_new();
    g_context.condition = oc_cond_new();

    static int const PSHARED        = 0;  // shared between threads
    static unsigned int const VALUE = 0;  // force sem_wait() to block

    if (sem_init(&g_context.le_started, PSHARED, VALUE) != 0)
    {
        return CA_STATUS_FAILED;
    }

    /*
      The CA LE interface doesn't expose a CAInitializeLEGattServer()
      function so perform initialization here.
     */
    CAPeripheralInitialize();

    return CA_STATUS_OK;
}

void CATerminateLENetworkMonitor()
{
    /**
     * @note "Network monitor" operations are stopped in @c CALEStop()
     *       since they are started in @c CAStartLEAdapter() rather
     *       than @c CAInitializeLENetworkMonitor().
     *
     * @see @c CAStartLEAdapter() for further details.
     */

    /*
      Since the CA LE interface doesn't expose a
      CAInitializeLEGattServer() function, finalize the LE server
      (peripheral) here rather than in CATerminateLEGattServer() to
      ensure finalization is correctly paired with initialization.
     */
    CAPeripheralFinalize();

    (void) sem_destroy(&g_context.le_started);

    oc_mutex_lock(g_context.lock);

    g_context.on_device_state_changed = NULL;
    g_context.on_server_received_data = NULL;
    g_context.on_client_received_data = NULL;
    g_context.client_thread_pool      = NULL;
    g_context.server_thread_pool      = NULL;
    g_context.on_client_error         = NULL;
    g_context.on_server_error         = NULL;

    oc_cond_free(g_context.condition);
    g_context.condition = NULL;

    oc_mutex_unlock(g_context.lock);

    oc_mutex_free(g_context.lock);
    g_context.lock = NULL;
}

CAResult_t CASetLEAdapterStateChangedCb(
    CALEDeviceStateChangedCallback callback)
{
    oc_mutex_lock(g_context.lock);
    g_context.on_device_state_changed = callback;
    oc_mutex_unlock(g_context.lock);

    return CA_STATUS_OK;
}

CAResult_t CASetLENWConnectionStateChangedCb(CALEConnectionStateChangedCallback callback)
{
    (void)callback;
    return CA_NOT_SUPPORTED;
}

CAResult_t CAGetLEAddress(char **local_address)
{
    OIC_LOG(DEBUG, TAG, "Get Linux BLE local device information.");

    if (local_address == NULL)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    /**
     * @bug Attempting to get LE interface information before this
     *      connectivity abstraction adapter has started (e.g. via
     *      @c CASelectNetwork()) could result in an inaccurate list
     *      of LE interfaces.  For example, detection of hot-plugged
     *      bluetooth adapters will only work after the LE IoTivity
     *      network has been selected.  If the LE IoTivity network
     *      hasn't been selected, the hot-plugged bluetooth adapter
     *      will not be reflected in the @c CALocalConnectivity_t list
     *      returned by this function.
     *      @par
     *      This issue caused by the fact that the event loop that
     *      handles such events can only be run after this LE
     *      adapter/transport is started.  The event loop cannot be
     *      started earlier, e.g. during @c CAInitialize(), due to a
     *      limitation in the CA transport termination code that
     *      requires thread pools to be destroyed before transport
     *      termination.  If the event loop was added as a task to the
     *      thread pool during @c CAInitialize(), it's possible that
     *      the thread running that event loop could be blocked
     *      waiting for events during a later call to
     *      @c CATerminate() if @c CASelectNetwork() was not called
     *      beforehand.  The thread pool destruction that occurs
     *      during @c CATerminate() waits for threads in the pool to
     *      exit.  However, in this case the event loop thread is
     *      still blocked waiting for events since it may not have
     *      been stopped since the transport itself was not stopped,
     *      meaning termination will also be blocked.
     *      @par
     *      Other than refactoring to the termination code to allow
     *      thread pools to be started during @c CAInitialize(), the
     *      only other choice we have to prevent the hang at
     *      termination is to move the event loop creation and
     *      termination to the @c CAAdapterStart() and
     *      @c CAAdapterStop() implementations, respectively, which is
     *      why we have this bug.
     */
    if (!CALECheckStarted())
      return CA_ADAPTER_NOT_ENABLED;

    *local_address = NULL;

    oc_mutex_lock(g_context.lock);

    for (GList * l = g_context.adapters; l != NULL; l = l->next)
    {
        GDBusProxy * const adapter = G_DBUS_PROXY(l->data);

        /*
          The local bluetooth adapter MAC address is stored in the
          org.bluez.Adapter1.Address property.
        */
        GVariant * const prop =
            g_dbus_proxy_get_cached_property(adapter, "Address");

        /*
          Unless the org.bluez.Adapter1.Address property no longer
          exists, prop should not be NULL!  We have bigger problems if
          this assert() is ever tripped since that would mean the
          org.bluez.Adapter1 D-Bus interface changed.
        */
        assert(prop != NULL);

        gchar const * const address = g_variant_get_string(prop, NULL);

        *local_address = OICStrdup(address);

        /*
          No longer need the property variant.  The address has been
          copied.
        */
        g_variant_unref(prop);

        /**
         * @todo Unfortunately the CA LE interface defined in
         *       caleinterface.h assumes that only one BLE adapter
         *       will exist on a given host.  However, this
         *       implementation can handle multiple BLE adapters.  The
         *       CA LE interface should be updated so that it can
         *       handle multiple BLE adapters.  For now we'll just
         *       return the address for the first BLE adapter in the
         *       list.
         */
        break;
    }

    oc_mutex_unlock(g_context.lock);

    return *local_address != NULL ? CA_STATUS_OK : CA_STATUS_FAILED;
}

CAResult_t CAStartLEGattServer()
{
    return CAPeripheralStart(&g_context);
}

CAResult_t CAStopLEGattServer()
{
    return CAPeripheralStop();
}

CAResult_t CAInitializeLEGattServer()
{
    return CA_STATUS_OK;
}

void CATerminateLEGattServer()
{
    /*
      See CATerminateLENetworkMonitor() to understand why the LE
      peripheral is not finalized here.
     */
}

void CASetLEReqRespServerCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_context.lock);
    g_context.on_server_received_data = callback;
    oc_mutex_unlock(g_context.lock);
}

CAResult_t CAUpdateCharacteristicsToGattClient(char const * address,
                                               uint8_t const * value,
                                               uint32_t valueLen)
{
    return CAGattServerSendResponseNotification(address,
                                                value,
                                                valueLen);
}

CAResult_t CAUpdateCharacteristicsToAllGattClients(uint8_t const * value,
                                                   uint32_t valueLen)
{
    return CAGattServerSendResponseNotificationToAll(value, valueLen);
}

CAResult_t CAStartLEGattClient()
{
    CAResult_t result = CACentralStart(&g_context);

    if (result != CA_STATUS_OK)
    {
        return result;
    }

    oc_mutex_lock(g_context.lock);
    bool found_peripherals = (g_context.devices != NULL);
    oc_mutex_unlock(g_context.lock);

    if (!found_peripherals)
    {
        // Wait for LE peripherals to be discovered.

        // Number of times to wait for discovery to complete.
        static int const retries = 5;

        static uint64_t const timeout =
            2 * MICROSECS_PER_SEC;  // Microseconds

        if (!CALEWaitForNonEmptyList(&g_context.devices,
                                     retries,
                                     timeout))
        {
            return result;
        }
    }

    /*
      Stop discovery so that we can connect to LE peripherals.
      Otherwise, the bluetooth subsystem will claim the adapter is
      busy.
    */

    result = CACentralStopDiscovery(&g_context);

    if (result != CA_STATUS_OK)
    {
        return result;
    }

    bool const connected = CACentralConnectToAll(&g_context);

    if (!connected)
    {
        return result;
    }

    /**
     * @todo Verify notifications have been enabled on all response
     *       characteristics.
     */

    return CAGattClientInitialize(&g_context);
}

void CAStopLEGattClient()
{
    CAGattClientDestroy();
    (void) CACentralStop(&g_context);
}

CAResult_t CAInitializeLEGattClient()
{
    return CA_STATUS_OK;
}

void CATerminateLEGattClient()
{
}

CAResult_t CAUpdateCharacteristicsToGattServer(
    char const * remoteAddress,
    uint8_t const * data,
    uint32_t dataLen,
    CALETransferType_t type,
    int32_t position)
{
    (void) position;

    if (type != LE_UNICAST)
    {
        return CA_STATUS_INVALID_PARAM;
    }

    /*
      We can assume that we're already connected to the BLE device
      with the given remote address - we wouldn't have a remote
      address otherwise - so there is no need to start scanning for
      BLE devices.
    */

    return CAGattClientSendData(remoteAddress,
                                data,
                                dataLen,
                                &g_context);
}

CAResult_t CAUpdateCharacteristicsToAllGattServers(uint8_t const * data,
                                                   uint32_t length)
{
    /*
      Now send the request through all BLE connections through the
      corresponding OIC GATT request characterstics.
    */
    return CAGattClientSendDataToAll(data, length, &g_context);

    /**
     * @todo Should we restart discovery after the send?
     */
}

void CASetLEReqRespClientCallback(CABLEDataReceivedCallback callback)
{
    oc_mutex_lock(g_context.lock);
    g_context.on_client_received_data = callback;
    oc_mutex_unlock(g_context.lock);
}

void CASetLEServerThreadPoolHandle(ca_thread_pool_t handle)
{
    oc_mutex_lock(g_context.lock);
    g_context.server_thread_pool = handle;
    oc_mutex_unlock(g_context.lock);
}

void CASetLEClientThreadPoolHandle(ca_thread_pool_t handle)
{
    oc_mutex_lock(g_context.lock);
    g_context.client_thread_pool = handle;
    oc_mutex_unlock(g_context.lock);
}

CAResult_t CAUnSetLEAdapterStateChangedCb()
{
    oc_mutex_lock(g_context.lock);
    g_context.on_device_state_changed = NULL;
    oc_mutex_unlock(g_context.lock);

    return CA_STATUS_OK;
}

void CASetBLEClientErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    oc_mutex_lock(g_context.lock);
    g_context.on_client_error = callback;
    oc_mutex_unlock(g_context.lock);
}

void CASetBLEServerErrorHandleCallback(CABLEErrorHandleCallback callback)
{
    oc_mutex_lock(g_context.lock);
    g_context.on_server_error = callback;
    oc_mutex_unlock(g_context.lock);
}
