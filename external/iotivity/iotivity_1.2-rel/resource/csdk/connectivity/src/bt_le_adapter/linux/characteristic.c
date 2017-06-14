/*****************************************************************
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

#include "characteristic.h"
#include "service.h"
#include "gatt_dbus.h"
#include "utils.h"
#include "bluez.h"
#include "server.h"

#include "logger.h"
#include "cagattservice.h"
#include "caremotehandler.h"

#include <assert.h>


// Logging tag.
static char const TAG[] = "BLE_CHARACTERISTIC";

// ---------------------------------------------------------------------
//                      GATT Request Handling
// ---------------------------------------------------------------------
/**
 * Handle @c org.bluez.GattCharacterstic1.WriteValue() method call.
 *
 * This handler is triggered when the
 * @c org.bluez.GattCharacterstic1.WriteValue() method is called by a
 * client on the BlueZ-based OIC GATT request characteristic.  In
 * particular, IoTivity request data is sent by the GATT client to the
 * GATT server through this call.  The server will retrieve the data
 * that was sent from the @a value argument.  Reassembly of any
 * request data fragments will begin here.
 *
 * @param[in] object     @c org.bluez.GattCharacteristic1 skeleton
 *                       object associated with this method call.
 * @param[in] invocation D-Bus method invocation related object used
 *                       when sending results/errors asynchronously.
 * @param[in] value      The @c GVariant containing the byte array
 *                       (ay) with th request data inside.
 * @param[in] user_data  Pointer to request
 *                       @c CAGattCharacteristic object.
 *
 * @return @c TRUE to indicate that
 *         @c org.bluez.Characteristic.WriteValue() method is
 *         implemented.
 */
static gboolean CAGattCharacteristicHandleWriteValue(
    GattCharacteristic1 * object,
    GDBusMethodInvocation * invocation,
    GVariant * value,
    gpointer user_data)
{
    /*
      This method is only trigged in a GATT server when receiving
      data sent by a GATT client, i.e. the client wrote a value to
      the server, and the server is handling that write request.
    */

    // GLib maps an octet to a guchar, which is of size 1.
    gsize len = 0;
    gconstpointer const data =
        g_variant_get_fixed_array(value, &len, 1);

    CAGattCharacteristic * const c = user_data;

    if (CAGattRecv(&c->recv_info, data, (uint32_t) len))
    {
        gatt_characteristic1_complete_write_value(object, invocation);
    }
    else
    {
        g_dbus_method_invocation_return_dbus_error(
            invocation,
            "org.bluez.Error.Failed",
            "Error when handling GATT request data fragment");
    }

    return TRUE;
}

// ---------------------------------------------------------------------
//                      GATT Response Handling
// ---------------------------------------------------------------------
/**
 * Handle @c org.bluez.GattCharacterstic1.StartNotify() method call.
 *
 * This handler is triggered when the
 * @c org.bluez.GattCharacterstic1.StartNotify() method is called by a
 * client on the BlueZ-based OIC GATT response characteristic.  It
 * sets the @c org.bluez.GattCharacterstic1.Notifying property to
 * @c TRUE, and enables responses from the server.
 *
 * @note This handler is not available in the OIC GATT request
 *       characteristic implementation.
 *
 * @param[in] object     @c org.bluez.GattCharacteristic1 skeleton
 *                       object associated with this method call.
 * @param[in] invocation D-Bus method invocation related object used
 *                       when sending results/errors asynchronously.
 * @param[in] user_data  Pointer to the response
 *                       @c CAGattCharacteristic object.
 *
 * @return @c TRUE to indicate that
 *         @c org.bluez.Characteristic.StartNotify() method is
 *         implemented.
 */
static gboolean CAGattCharacteristicHandleStartNotify(
    GattCharacteristic1 * object,
    GDBusMethodInvocation * invocation,
    gpointer user_data)
{
    (void) user_data;

    /**
     * Only allow the client to start notifications once.
     *
     * @todo Does BlueZ already prevent redundant calls to
     *       @c org.bluez.GattCharacteristic1.StartNotify()?
     */
    if (gatt_characteristic1_get_notifying(object))
    {
        g_dbus_method_invocation_return_dbus_error(
            invocation,
            "org.bluez.Error.Failed",
            "Notifications are already enabled.");

        return TRUE;
    }

    /**
     * @todo Do we need to explicitly emit the @c GObject @c notify or
     *       @c org.freedesktop.Dbus.Properties.PropertiesChanged
     *       signal here?
     */
    gatt_characteristic1_set_notifying(object, TRUE);
    gatt_characteristic1_complete_start_notify(object, invocation);

    return TRUE;
}

/**
 * Handle @c org.bluez.GattCharacterstic1.StopNotify() method call.
 *
 * This handler is triggered when the
 * @c org.bluez.GattCharacterstic1.StopNotify() method is called by a
 * client on the BlueZ-based OIC GATT response characteristic.  It
 * sets the @c org.bluez.GattCharacterstic1.Notifying property to
 * @c FALSE, and disables responses from the server.
 *
 * @param[in] object     @c org.bluez.GattCharacteristic1 skeleton
 *                       object associated with this method call.
 * @param[in] invocation D-Bus method invocation related object used
 *                       when sending results/errors asynchronously.
 * @param[in] user_data  Pointer to the response
 *                       @c CAGattCharacteristic object.
 *
 * @return @c TRUE to indicate that
 *         @c org.bluez.Characteristic.StopNotify() method is
 *         implemented.
 */
static gboolean CAGattCharacteristicHandleStopNotify(
    GattCharacteristic1 * object,
    GDBusMethodInvocation * invocation,
    gpointer user_data)
{
    (void) user_data;

    /**
     * @todo Does BlueZ already prevent redundant calls to
     *       @c org.bluez.GattCharacteristic1.StopNotify()?
     */
    if (!gatt_characteristic1_get_notifying(object))
    {
        g_dbus_method_invocation_return_dbus_error(
            invocation,
            "org.bluez.Error.Failed",
            "Notifications were not previously enabled.");

        return TRUE;
    }

    /**
     * @todo Do we need to explicitly emit the @c GObject @c notify or
     *       @c org.freedesktop.Dbus.Properties.PropertiesChanged
     *       signal here?
     */
    gatt_characteristic1_set_notifying(object, FALSE);
    gatt_characteristic1_complete_stop_notify(object, invocation);

    return TRUE;
}

// ---------------------------------------------------------------------
//                   GATT Characteristic Lifecyle
// ---------------------------------------------------------------------

/**
 * Initialize GATT characteristic fields.
 *
 * This function initializes the @c CAGattCharacteristic object fields.
 *
 * @param[out] c                   GATT characteristic information to
 *                                 be initialized.
 * @param[in]  context             Object containing the D-Bus
 *                                 connection to the bus on which the
 *                                 characteristic will be exported, as
 *                                 well as the list of connected
 *                                 devices.
 * @param[in]  s                   Information about GATT service
 *                                 to which the characteristic
 *                                 belongs.
 * @param[in]  characteristic_path @c GattCharacteristic1 object
 *                                 path.
 * @param[in]  uuid                GATT characteristic UUID.
 * @param[in]  flag                GATT characteristic property flag,
 *                                 i.e. @c "write-without-response"
 *                                 for the request characteristic, and
 *                                 @c "notify" for the response
 *                                 characteristic.
 *
 * @note This function does not allocate the @a characteristic object
 *       itself.  The caller is responsible for allocating that
 *       memory.
 *
 * @todo Too many parameters.  Perhaps pass in a pointer to a struct
 *       instead.
 */
static bool CAGattCharacteristicInitialize(
    CAGattCharacteristic * c,
    CALEContext * context,
    CAGattService * s,
    char const * characteristic_path,
    char const * uuid,
    char const * flag)
{
    // Path of the form /org/iotivity/gatt/hci0/service0/char0.
    c->object_path =
        g_strdup_printf("%s/%s", s->object_path, characteristic_path);

    assert(g_variant_is_object_path(c->object_path));

    c->context = context;
    c->service = s;

    c->characteristic = gatt_characteristic1_skeleton_new();

    gatt_characteristic1_set_uuid(c->characteristic, uuid);
    gatt_characteristic1_set_service(c->characteristic, s->object_path);
    gatt_characteristic1_set_notifying(c->characteristic, FALSE);

    char const * const flags[] = { flag, NULL };
    gatt_characteristic1_set_flags(c->characteristic, flags);

    CAGattRecvInfoInitialize(&c->recv_info);

    // Export the characteristic interface on the bus.
    GError * error = NULL;
    if (!g_dbus_interface_skeleton_export(
            G_DBUS_INTERFACE_SKELETON(c->characteristic),
            context->connection,
            c->object_path,
            &error))
    {
        CAGattCharacteristicDestroy(c);

        OIC_LOG_V(ERROR,
                  TAG,
                  "Unable to export D-Bus GATT characteristic "
                  "interface: %s",
                  error->message);

        g_error_free(error);

        return false;
    }

    return true;
}

// ------------------------------------------------------------

bool CAGattRequestCharacteristicInitialize(struct CAGattService * s,
                                           CALEContext * context)
{
    CAGattCharacteristic * const c = &s->request_characteristic;

    if (!CAGattCharacteristicInitialize(c,
                                        context,
                                        s,
                                        CA_GATT_REQUEST_CHRC_PATH,
                                        CA_GATT_REQUEST_CHRC_UUID,
                                        "write-without-response"))
    {
        return false;
    }

    if (!CAGattRequestDescriptorInitialize(s, context->connection))
    {
        CAGattCharacteristicDestroy(c);
        return false;
    }

    /*
      The descriptor object path is not fixed at compile-time.
      Retrieve the object path that was set at run-time.
    */
    char const * const descriptor_paths[] = {
        c->descriptor.object_path,
        NULL
    };

    gatt_characteristic1_set_descriptors(c->characteristic,
                                         descriptor_paths);

    char * const peer = CAGattServiceMakePeerAddress(s);

    if (peer == NULL)
    {
        CAGattCharacteristicDestroy(c);
        return false;
    }

    oc_mutex_lock(context->lock);
    c->recv_info.on_packet_received = context->on_server_received_data;
    oc_mutex_unlock(context->lock);

    c->recv_info.peer    = peer;
    c->recv_info.context = context;

    // The request characteristic only handles writes.
    g_signal_connect(c->characteristic,
                     "handle-write-value",
                     G_CALLBACK(CAGattCharacteristicHandleWriteValue),
                     c);

    return true;
}

// ------------------------------------------------------------

bool CAGattResponseCharacteristicInitialize(struct CAGattService * s,
                                            CALEContext * context)
{
    CAGattCharacteristic * const c = &s->response_characteristic;

    if (!CAGattCharacteristicInitialize(c,
                                        context,
                                        s,
                                        CA_GATT_RESPONSE_CHRC_PATH,
                                        CA_GATT_RESPONSE_CHRC_UUID,
                                        "notify"))
    {
        return false;
    }

    c->service = s;

    if (!CAGattResponseDescriptorInitialize(s, context->connection))
    {
        CAGattCharacteristicDestroy(c);
        return false;
    }

    /*
      The descriptor object path is not fixed at compile-time.
      Retrieve the object path that was set at run-time.

      Note that we don't explicitly add the Client Characteristic
      Configuration Descriptor for the response characteristic since
      that is done by BlueZ when the "notify" property is set.
      Furthermore, a client requests notifications by calling the
      org.bluez.GattCharacteristic1.StartNotify() method.
      Consequently, there is no need for the client to explicitly
      enable notifications by writing to the client characteristic
      configuration descriptor.
    */
    char const * const descriptor_paths[] = {
        c->descriptor.object_path,
        NULL
    };

    gatt_characteristic1_set_descriptors(c->characteristic,
                                         descriptor_paths);

    // The response characteristic only handles notifications.
    g_signal_connect(
        c->characteristic,
        "handle-start-notify",
        G_CALLBACK(CAGattCharacteristicHandleStartNotify),
        c);

    g_signal_connect(
        c->characteristic,
        "handle-stop-notify",
        G_CALLBACK(CAGattCharacteristicHandleStopNotify),
        c);

    return true;
}

void CAGattCharacteristicDestroy(CAGattCharacteristic * c)
{
    assert(c != NULL);  // As designed, c is always non-NULL.

    CAGattRecvInfoDestroy(&c->recv_info);

    CAGattDescriptorDestroy(&c->descriptor);

    g_clear_object(&c->characteristic);

    g_free(c->object_path);
    c->object_path = NULL;

    c->service = NULL;
    c->context = NULL;
}

// ---------------------------------------------------------------------
//                  GATT Characteristic Properties
// ---------------------------------------------------------------------


GVariant * CAGattCharacteristicGetProperties(
    GattCharacteristic1 * characteristic)
{
    /**
     * Create a variant containing the @c GattCharacteristic1
     * properties, of the form @c a{sa{sv}}.
     *
     * @note We don't care about the "Value" property here since it is
     *       automatically made available by BlueZ on the client
     *       side.
     *
     * @todo Should we care about "Notifying" property here?
     */

    /*
      Populate the property table, and create the variant to be
      embedded in the results of the
      org.freedesktop.Dbus.ObjectManager.GetManagedObjects() method
      call.
    */
    CADBusSkeletonProperty const properties[] = {
        { "UUID",
          g_variant_new_string(
              gatt_characteristic1_get_uuid(characteristic)) },
        { "Service",
          g_variant_new_object_path(
              gatt_characteristic1_get_service(characteristic)) },
        { "Flags",
          g_variant_new_strv(
              gatt_characteristic1_get_flags(characteristic),
              -1) },
        { "Descriptors",
          g_variant_new_objv(
              gatt_characteristic1_get_descriptors(characteristic),
              -1) }
    };

    return
        CAMakePropertyDictionary(
            BLUEZ_GATT_CHARACTERISTIC_INTERFACE,
            properties,
            sizeof(properties) / sizeof(properties[0]));
}
