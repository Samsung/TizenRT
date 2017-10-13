#ifndef _LWM2MCLIENT_H_
#define _LWM2MCLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define LWM2M_MAX_URI_LEN 16
#define LWM2M_MAX_STR_LEN 100

/*
 * Client object
 */
typedef struct {
    void *client; /* Client handle. */
    int error;    /* Error code:
                   *   LWM2M_CLIENT_ERROR or LWM2M_CLIENT_QUIT when \ref lwm2m_clients_service
                   *   failed, otherwise 0.
                   */
} client_handle_t;

/*
 * Device object
 */
typedef struct {
    char manufacturer[LWM2M_MAX_STR_LEN];     /*PRV_MANUFACTURER*/
    char model_number[LWM2M_MAX_STR_LEN];     /*PRV_MODEL_NUMBER*/
    char serial_number[LWM2M_MAX_STR_LEN];    /*PRV_SERIAL_NUMBER*/
    char firmware_version[LWM2M_MAX_STR_LEN]; /*PRV_FIRMWARE_VERSION*/
    int power_source_1;                       /*PRV_POWER_SOURCE_1*/
    int power_source_2;                       /*PRV_POWER_SOURCE_2*/
    int power_voltage_1;                      /*PRV_POWER_VOLTAGE_1*/
    int power_voltage_2;                      /*PRV_POWER_VOLTAGE_2*/
    int power_current_1;                      /*PRV_POWER_CURRENT_1*/
    int power_current_2;                      /*PRV_POWER_CURRENT_2*/
    int battery_level;                        /*PRV_BATTERY_LEVEL*/
    int memory_free;                          /*PRV_MEMORY_FREE*/
    char time_zone[LWM2M_MAX_STR_LEN];        /*PRV_TIME_ZONE*/
    char utc_offset[LWM2M_MAX_STR_LEN];       /*PRV_UTC_OFFSET*/
    char binding_mode[LWM2M_MAX_STR_LEN];     /*PRV_BINDING_MODE*/
    char device_type[LWM2M_MAX_STR_LEN];      /*PRV_DEVICE_TYPE*/
    char hardware_version[LWM2M_MAX_STR_LEN]; /*PRV_HARDWARE_VERSION*/
    char software_version[LWM2M_MAX_STR_LEN]; /*PRV_SOFTWARE_VERSION*/
    int battery_status;                       /*PRV_BATTERY_STATUS*/
    int memory_total;                         /*PRV_MEMORY_TOTAL*/
} object_device_t;

/*
 * Firmware update object
 */
typedef struct {
    bool supported;                      /*SUPPORTED*/
    char pkg_name[LWM2M_MAX_STR_LEN];    /*PKG_NAME*/
    char pkg_version[LWM2M_MAX_STR_LEN]; /*PKG_VERSION*/
}object_firmware_t;

/*
 * Location object
 */
 typedef struct {
    char  latitude[LWM2M_MAX_STR_LEN];      /*Latitude */
    char  longitude[LWM2M_MAX_STR_LEN];     /*Longitude*/
    char  altidude[LWM2M_MAX_STR_LEN];      /*Altitude*/
    char  uncertainty[LWM2M_MAX_STR_LEN];   /*Uncertainty*/
} object_location_t;

/*
 * Connectivity monitoring object
 */
typedef struct {
    int  network_bearer;                     /*VALUE_NETWORK_BEARER_GSM*/
    int  avl_network_bearer;                 /*VALUE_AVL_NETWORK_BEARER_1*/
    int  radio_signal_strength;              /*VALUE_RADIO_SIGNAL_STRENGTH*/
    int  link_quality;                       /*VALUE_LINK_QUALITY*/
    char ip_addr[LWM2M_MAX_STR_LEN];         /*VALUE_IP_ADDRESS_1*/
    char ip_addr2[LWM2M_MAX_STR_LEN];        /*VALUE_IP_ADDRESS_2*/
    char router_ip_addr[LWM2M_MAX_STR_LEN];  /*VALUE_ROUTER_IP_ADDRESS_1*/
    char router_ip_addr2[LWM2M_MAX_STR_LEN]; /*VALUE_ROUTER_IP_ADDRESS_2*/
    int  link_utilization;                   /*VALUE_LINK_UTILIZATION*/
    char apn[LWM2M_MAX_STR_LEN];             /*VALUE_APN_1*/
    int  cell_id;                            /*VALUE_CELL_ID*/
    int  smnc;                               /*VALUE_SMNC*/
    int  smcc;                               /*VALUE_SMCC*/
} object_conn_monitoring_t;

enum lwm2m_sec_mode_type {
    LWM2M_SEC_MODE_PSK = 0,
    LWM2M_SEC_MODE_CERT = 2,
};

/*
 * LWM2M security object
 */
 typedef struct {
    char serverUri[LWM2M_MAX_STR_LEN];   /*serverUri*/
    enum lwm2m_sec_mode_type securityMode;                /*securityMode: determine wich security mode is used*/
    char *clientCertificateOrPskId;      /*pskId : DEVICE ID or client certificate*/
    char token[LWM2M_MAX_STR_LEN];                         /*token : DEVICE TOKEN */
    char *privateKey;                    /*privateKey : private key*/
    char *serverCertificate;             /*serverCertificate: store the LWM2M server certificate*/
    char client_name[LWM2M_MAX_STR_LEN]; /*name : DEVICE ID*/
    int lifetime;                        /*lifetime*/
    int  batterylevelchanging;           /*battery*/
    int serverId;                        /*serverId*/
    bool verifyCert;                     /*verify server certificate*/
    int localPort;                       /*local source port - random if set to 0*/
} object_security_server_t;

/*
 * Object container
 */
typedef struct {
    object_security_server_t* server;
    object_device_t* device;
    object_firmware_t* firmware;
    object_location_t* location;
    object_conn_monitoring_t* monitoring;
}object_container_t;

enum lwm2m_object_type {
    LWM2M_OBJ_SECURITY = 0,
    LWM2M_OBJ_SERVER,
    LWM2M_OBJ_ACL,
    LWM2M_OBJ_DEVICE,
    LWM2M_OBJ_CONN_MON,
    LWM2M_OBJ_FIRMWARE,
    LWM2M_OBJ_LOCATION,
    LWM2M_OBJ_CONN_STAT,
    LWM2M_OBJ_COUNT
};

typedef void (*lwm2m_exe_callback)(void*, void*);

enum lwm2m_execute_callback_type {
    LWM2M_EXE_FACTORY_RESET = 0,
    LWM2M_EXE_DEVICE_REBOOT,
    LWM2M_EXE_FIRMWARE_UPDATE,
    LWM2M_NOTIFY_RESOURCE_CHANGED,
    LWM2M_EXE_COUNT
};

typedef struct {
    char uri[LWM2M_MAX_URI_LEN];
    uint8_t *buffer;
    int length;
} lwm2m_resource_t;

#define LWM2M_CLIENT_OK     ( 0)
#define LWM2M_CLIENT_ERROR  (-1)
#define LWM2M_CLIENT_QUIT   (-2)

/*
 * Object: Device
 */
#define LWM2M_URI_DEVICE                 "/3/0"
#define LWM2M_URI_DEVICE_MANUFACTURER    LWM2M_URI_DEVICE"/0"
#define LWM2M_URI_DEVICE_MODEL_NUM       LWM2M_URI_DEVICE"/1"
#define LWM2M_URI_DEVICE_SERIAL_NUM      LWM2M_URI_DEVICE"/2"
#define LWM2M_URI_DEVICE_FW_VERSION      LWM2M_URI_DEVICE"/3"
#define LWM2M_URI_DEVICE_REBOOT          LWM2M_URI_DEVICE"/4"
#define LWM2M_URI_DEVICE_FACTORY_RESET   LWM2M_URI_DEVICE"/5"
#define LWM2M_URI_DEVICE_POWER_SOURCES   LWM2M_URI_DEVICE"/6"
#define LWM2M_URI_DEVICE_POWER_VOLTAGE   LWM2M_URI_DEVICE"/7"
#define LWM2M_URI_DEVICE_POWER_CURRENT   LWM2M_URI_DEVICE"/8"
#define LWM2M_URI_DEVICE_BATT_LEVEL      LWM2M_URI_DEVICE"/9"
#define LWM2M_URI_DEVICE_MEMORY_FREE     LWM2M_URI_DEVICE"/10"
#define LWM2M_URI_DEVICE_ERROR_CODE      LWM2M_URI_DEVICE"/11"
#define LWM2M_URI_DEVICE_RESET_ERR_CODE  LWM2M_URI_DEVICE"/12"
#define LWM2M_URI_DEVICE_CURRENT_TIME    LWM2M_URI_DEVICE"/13"
#define LWM2M_URI_DEVICE_UTC_OFFSET      LWM2M_URI_DEVICE"/14"
#define LWM2M_URI_DEVICE_TIMEZONE        LWM2M_URI_DEVICE"/15"
#define LWM2M_URI_DEVICE_SUPP_BIND_MODES LWM2M_URI_DEVICE"/16"
#define LWM2M_URI_DEVICE_DEVICE_TYPE     LWM2M_URI_DEVICE"/17"
#define LWM2M_URI_DEVICE_HW_VERSION      LWM2M_URI_DEVICE"/18"
#define LWM2M_URI_DEVICE_SW_VERSION      LWM2M_URI_DEVICE"/19"
#define LWM2M_URI_DEVICE_BATT_STATUS     LWM2M_URI_DEVICE"/20"
#define LWM2M_URI_DEVICE_MEMORY_TOTAL    LWM2M_URI_DEVICE"/21"

/*
 * Object: Connectivity Monitoring
 */
#define LWM2M_URI_CONNMON                "/4/0"
#define LWM2M_URI_CONNMON_BEARER         LWM2M_URI_CONNMON"/0"
#define LWM2M_URI_CONNMON_AVAIL_BEARERS  LWM2M_URI_CONNMON"/1"
#define LWM2M_URI_CONNMON_SIGNAL         LWM2M_URI_CONNMON"/2"
#define LWM2M_URI_CONNMON_LINK_QUALITY   LWM2M_URI_CONNMON"/3"
#define LWM2M_URI_CONNMON_IP_ADDR        LWM2M_URI_CONNMON"/4"
#define LWM2M_URI_CONNMON_ROUTER_IP_ADDR LWM2M_URI_CONNMON"/5"
#define LWM2M_URI_CONNMON_LINK_UTIL      LWM2M_URI_CONNMON"/6"
#define LWM2M_URI_CONNMON_APN            LWM2M_URI_CONNMON"/7"
#define LWM2M_URI_CONNMON_CELL_ID        LWM2M_URI_CONNMON"/8"
#define LWM2M_URI_CONNMON_SMNC           LWM2M_URI_CONNMON"/9"
#define LWM2M_URI_CONNMON_SMCC           LWM2M_URI_CONNMON"/10"

/*
 * Object: Firmware
 */
#define LWM2M_URI_FIRMWARE               "/5/0"
#define LWM2M_URI_FIRMWARE_PACKAGE       LWM2M_URI_FIRMWARE"/0"
#define LWM2M_URI_FIRMWARE_PACKAGE_URI   LWM2M_URI_FIRMWARE"/1"
#define LWM2M_URI_FIRMWARE_UPDATE        LWM2M_URI_FIRMWARE"/2"
#define LWM2M_URI_FIRMWARE_STATE         LWM2M_URI_FIRMWARE"/3"
#define LWM2M_URI_FIRMWARE_UPD_SUPP_OBJ  LWM2M_URI_FIRMWARE"/4"
#define LWM2M_URI_FIRMWARE_UPDATE_RES    LWM2M_URI_FIRMWARE"/5"
#define LWM2M_URI_FIRMWARE_PKG_NAME      LWM2M_URI_FIRMWARE"/6"
#define LWM2M_URI_FIRMWARE_PKG_URI       LWM2M_URI_FIRMWARE"/7"

/*
 * Object: Firmware
 * Resource: State
 */
#define LWM2M_FIRMWARE_STATE_IDLE        "0"
#define LWM2M_FIRMWARE_STATE_DOWNLOADING "1"
#define LWM2M_FIRMWARE_STATE_DOWNLOADED  "2"
#define LWM2M_FIRMWARE_STATE_UPDATING    "3"

/*
 * Object: Firmware
 * Resource: Update Result
 */
#define LWM2M_FIRMWARE_UPD_RES_DEFAULT   "0"
#define LWM2M_FIRMWARE_UPD_RES_SUCCESS   "1"
#define LWM2M_FIRMWARE_UPD_RES_SPACE_ERR "2"
#define LWM2M_FIRMWARE_UPD_RES_OOM       "3"
#define LWM2M_FIRMWARE_UPD_RES_CONNE_ERR "4"
#define LWM2M_FIRMWARE_UPD_RES_CRC_ERR   "5"
#define LWM2M_FIRMWARE_UPD_RES_PKG_ERR   "6"
#define LWM2M_FIRMWARE_UPD_RES_URI_ERR   "7"

/*
 * Object: Location
 */
#define LWM2M_URI_LOCATION               "/6/0"
#define LWM2M_URI_LOCATION_LATITUDE      LWM2M_URI_LOCATION"/0"
#define LWM2M_URI_LOCATION_LONGITUDE     LWM2M_URI_LOCATION"/1"
#define LWM2M_URI_LOCATION_ALTITUDE      LWM2M_URI_LOCATION"/2"
#define LWM2M_URI_LOCATION_UNCERTAINTY   LWM2M_URI_LOCATION"/3"
#define LWM2M_URI_LOCATION_VELOCITY      LWM2M_URI_LOCATION"/4"
#define LWM2M_URI_LOCATION_TIMESTAMP     LWM2M_URI_LOCATION"/5"

/*!
 *  \brief Start the LWM2M connection to the server
 *
 *  This function initializes the LWM2M client with default
 *  values passed as parameter, then initiates the connection
 *  to the server. It returns a connection-specific handle
 *  that can be used by other functions in subsequent calls.
 *
 *  \param[in] init_val container object defining the default values
 *  to set to the LWM2M standard resources. These values can
 *  be later changed dynamically by calling \ref lwm2m_write_resource
 *
 * \param[in] root_ca If not null, use it as trusted root CA for verifying the
 *                    server's certificate.
 * \param[in] use_se If true the private key stored in the SE is used
 *                  (only usefull in certificate mode and only implemented for TizenRT).
 *
 *  \return handle to the client connection, or NULL if an error
 *  occured
 */
client_handle_t* lwm2m_client_start(object_container_t *init_val, char *root_ca, bool use_se);

/*!
 *  \brief Process current tasks
 *
 *  This function has to be called periodically from the application
 *  code. It performs necessary updates needed throughout the
 *  lifetime of the LWM2M client connections.
 *
 *  \param[in] handles Array of client_handle_t returned by \ref lwm2m_client_start
 *  \param[in] number_handles Size of \ref handles
 *  \param[in] timeout Max time (in milliseconds) the lwm2m client waits for data. If timeout is
 *                     greater than the number of seconds returned by the function, the function
 *                     waits for the number of seconds returned. If timeout is equal to zero, the
 *                     function also waits for the number of seconds returned by the function.
 *
 *  \return the number of seconds after which the function
 *  must be called again.
 *   - LWM2M_CLIENT_ERROR in case an error happened
 *   - LWM2M_CLIENT_QUIT in case the connection was terminated
 */
int lwm2m_clients_service(client_handle_t **handles, int number_handles, int timeout_ms);

/*!
 *  \brief Process current tasks
 *
 *  This function has to be called periodically from the application
 *  code. It performs necessary updates needed throughout the
 *  lifetime of the LWM2M client connection.
 *
 *  \param[in] handle returned by \ref lwm2m_client_start
 *  \param[in] timeout Max time (in milliseconds) the lwm2m client waits for data. If timeout is
 *                     greater than the number of seconds returned by the function, the function
 *                     waits for the number of seconds returned. If timeout is equal to zero, the
 *                     function also waits for the number of seconds returned by the function.
 *
 *  \return the number of seconds after which the function
 *  must be called again. If an error happens, it returns a negative
 *  value:
 *   - LWM2M_CLIENT_ERROR in case an error happened
 *   - LWM2M_CLIENT_QUIT in case the connection was terminated
 */
int lwm2m_client_service(client_handle_t *handle, int timeout_ms);


/*!
 *  \brief Stops the LWM2M connection to the server
 *
 *  This function terminates the connection and release
 *  all the resources previously created during the call
 *  to \ref lwm2m_client_start
 *
 *  \param[in] handle returned by \ref lwm2m_client_start
 *
 */
void lwm2m_client_stop(client_handle_t* handle);

/*!
 *  \brief Register a callback for the application to
 *  be notified of specific events.
 *
 *  This function registers a callback function linked
 *  to events specified by \ref lwm2m_execute_callback_type.
 *  When the event occurs, the library calls the callback
 *  that was previously registered by the application.
 *  The application can then take action on this specific
 *  event.
 *
 *  \param[in] handle returned by \ref lwm2m_client_start
 *  \param[in] type event to link to the callback, among:
 *   - LWM2M_EXE_FACTORY_RESET: called when the server executes
 *   a factory reset request on the client device
 *   - LWM2M_EXE_DEVICE_REBOOT: called when the server executes
 *   a reboot request on the client device
 *   - LWM2M_EXE_FIRMWARE_UPDATE: called when the server executes
 *   a firmware update request on the client device
 *   - LWM2M_NOTIFY_RESOURCE_CHANGED: called when server writes a
 *   resource to any object on the client device
 *   \param[in] callback function pointer to call when the associated
 *   event occurs.
 *   \param[in] param pointer to any user data allocated by the calling
 *   application. This pointer will be passed back as a parameter to the
 *   callback function when calling it on an event.
 *
 *   The prototype of the callback function must be:
 *
 *   void lwm2m_exe_callback(void *param, void *extra)
 *   {
 *    ...
 *   }
 *
 *   The "param" parameter contains the pointer passed as a parameter
 *   to the \ref lwm2m_register_callback function. The "extra" parameter
 *   contains some event-specific data passed by the library.
 *
 *    - LWM2M_EXE_FACTORY_RESET, LWM2M_EXE_DEVICE_REBOOT, LWM2M_EXE_FIRMWARE_UPDATE:
 *    "extra" is set to NULL.
 *    - LWM2M_NOTIFY_RESOURCE_CHANGED: contains a pointer to a \ref lwm2m_resource_t
 *    object containing the URI and data of the resource that has been written
 *    by the LWM2M server.
 *
 */
void lwm2m_register_callback(client_handle_t* handle, enum lwm2m_execute_callback_type type,
        lwm2m_exe_callback callback, void *param);

/*!
 *  \brief Unregisters a callback to an event
 *
 *  This function unregisters a callback that has been
 *  previously registered to match an event. After this
 *  call the callback function will no longer be called by
 *  the library upon occurrence of the previously associated
 *  event.
 *
 *  \param[in] handle returned by \ref lwm2m_client_start
 *  \param[in] type event that was previously associated
 *  to the callback
 *
 */
void lwm2m_unregister_callback(client_handle_t* handle, enum lwm2m_execute_callback_type type);

/*!
 *  \brief Change the value of a resource
 *
 *  This function changes the value of a resource locally.
 *  All resources (even those that are not writable according
 *  to the LWM2M standard, therefore from the server point of
 *  view) can be modified by the application.
 *
 *  \param[in] handle returned by \ref lwm2m_client_start
 *  \param[in] resource pointer to a \ref lwm2m_resource_t object containing
 *  the URI and data of the resource to modify. The "buffer" field of the
 *  \ref lwm2m_resource_t structure must be allocated and freed by the calling
 *  application.
 *
 *  \return LWM2M_CLIENT_OK if no error occurred, LWM2M_CLIENT_ERROR otherwise
 *
 */
int lwm2m_write_resource(client_handle_t* handle, lwm2m_resource_t *res);

/*!
 *  \brief Read the value of a resource
 *
 *  This function reads the value of a resource locally.
 *  All resources (even those that are not readable according
 *  to the LWM2M standard, therefore from the server point of
 *  view) can be read by the application.
 *
 *  \param[in] handle returned by "lwm2m_client_start"
 *  \param[in] resource pointer to a "lwm2m_resource_t" object containing
 *  the URI of the resource to read. After successful return of the
 *  function, this object contains the data read from the resource.
 *  The "buffer" field of the "lwm2m_resource_t" is allocated and filled
 *  by the library, and must be freed by the calling application.
 *
 *  \return LWM2M_CLIENT_OK if no error occurred, LWM2M_CLIENT_ERROR otherwise
 *
 */
int lwm2m_read_resource(client_handle_t* handle, lwm2m_resource_t *res);

/*!
 *  \brief Serialize multiple strings into a TLV object
 *
 *  This function fills up a \ref lwm2m_resource_t with multiple
 *  strings passed as parameters. The generated object follows the
 *  binary TLV format. This function is used as an helper routine
 *  to format resource objects before writing multiple objects
 *  formatted resources.
 *
 *  \param[in] number of strings to process
 *  \param[in] pointer to the array of strings to process
 *  \param[in] resource pointer to a \ref lwm2m_resource_t object whose "buffer"
 *  field will be allocated and filled by the library with the content of the
 *  TLV. After usage, the calling application must free the memory allocated
 *  for the "buffer" field.
 *
 *  \return LWM2M_CLIENT_OK if no error occurred, LWM2M_CLIENT_ERROR otherwise
 *
 */
int lwm2m_serialize_tlv_string(int num, char **strs, lwm2m_resource_t* res);

/*!
 *  \brief Serialize multiple integers into a TLV object
 *
 *  This function fills up a \ref lwm2m_resource_t with multiple
 *  integers passed as parameters. The generated object follows the
 *  binary TLV format. This function is used as an helper routine
 *  to format resource objects before writing multiple objects
 *  formatted resources.
 *
 *  \param[in] number of integers to process
 *  \param[in] pointer to the array of integers to process
 *  \param[in] resource pointer to a \ref lwm2m_resource_t object whose "buffer"
 *  field will be allocated and filled by the library with the content of the
 *  TLV. After usage, the calling application must free the memory allocated
 *  for the "buffer" field.
 *
 *  \return LWM2M_CLIENT_OK if no error occurred, LWM2M_CLIENT_ERROR otherwise
 *
 */
int lwm2m_serialize_tlv_int(int num, int *ints, lwm2m_resource_t* res);

#ifdef __cplusplus
}
#endif

#endif /* _LWM2MCLIENT_H_ */
