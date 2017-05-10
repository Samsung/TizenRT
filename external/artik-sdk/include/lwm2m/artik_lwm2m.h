#ifndef INCLUDE_ARTIK_LWM2M_H_
#define INCLUDE_ARTIK_LWM2M_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "artik_error.h"
#include "artik_types.h"

/*! \file artik_lwm2m.h
 *
 *  \brief LightWeight M2M module definition
 *
 *  Definitions and functions for accessing
 *  LWM2M module and connecting LWM2M servers
 *  as a client
 *
 *  The LWM2M technical specification is available
 *  <a href="http://dev_devtoolkit.openmobilealliance.org/IoT/LWM2M10/doc/">here</a>
 *
 *  \example lwm2m_test/artik_lwm2m_test_client.c
 *  \example lwm2m_test/artik_lwm2m_test_common.c
 *  \example lwm2m_test/artik_lwm2m_test_common.h
 */

#define ARTIK_LWM2M_URI_LEN 16

#define ARTIK_LWM2M_URI_DEVICE                 "/3/0"
#define ARTIK_LWM2M_URI_DEVICE_MANUFACTURER    ARTIK_LWM2M_URI_DEVICE"/0"
#define ARTIK_LWM2M_URI_DEVICE_MODEL_NUM       ARTIK_LWM2M_URI_DEVICE"/1"
#define ARTIK_LWM2M_URI_DEVICE_SERIAL_NUM      ARTIK_LWM2M_URI_DEVICE"/2"
#define ARTIK_LWM2M_URI_DEVICE_FW_VERSION      ARTIK_LWM2M_URI_DEVICE"/3"
#define ARTIK_LWM2M_URI_DEVICE_REBOOT          ARTIK_LWM2M_URI_DEVICE"/4"
#define ARTIK_LWM2M_URI_DEVICE_FACTORY_RESET   ARTIK_LWM2M_URI_DEVICE"/5"
#define ARTIK_LWM2M_URI_DEVICE_POWER_SOURCES   ARTIK_LWM2M_URI_DEVICE"/6"
#define ARTIK_LWM2M_URI_DEVICE_POWER_VOLTAGE   ARTIK_LWM2M_URI_DEVICE"/7"
#define ARTIK_LWM2M_URI_DEVICE_POWER_CURRENT   ARTIK_LWM2M_URI_DEVICE"/8"
#define ARTIK_LWM2M_URI_DEVICE_BATT_LEVEL      ARTIK_LWM2M_URI_DEVICE"/9"
#define ARTIK_LWM2M_URI_DEVICE_MEMORY_FREE     ARTIK_LWM2M_URI_DEVICE"/10"
#define ARTIK_LWM2M_URI_DEVICE_ERROR_CODE      ARTIK_LWM2M_URI_DEVICE"/11"
#define ARTIK_LWM2M_URI_DEVICE_RESET_ERR_CODE  ARTIK_LWM2M_URI_DEVICE"/12"
#define ARTIK_LWM2M_URI_DEVICE_CURRENT_TIME    ARTIK_LWM2M_URI_DEVICE"/13"
#define ARTIK_LWM2M_URI_DEVICE_UTC_OFFSET      ARTIK_LWM2M_URI_DEVICE"/14"
#define ARTIK_LWM2M_URI_DEVICE_TIMEZONE        ARTIK_LWM2M_URI_DEVICE"/15"
#define ARTIK_LWM2M_URI_DEVICE_SUPP_BIND_MODES ARTIK_LWM2M_URI_DEVICE"/16"
#define ARTIK_LWM2M_URI_DEVICE_DEVICE_TYPE     ARTIK_LWM2M_URI_DEVICE"/17"
#define ARTIK_LWM2M_URI_DEVICE_HW_VERSION      ARTIK_LWM2M_URI_DEVICE"/18"
#define ARTIK_LWM2M_URI_DEVICE_SW_VERSION      ARTIK_LWM2M_URI_DEVICE"/19"
#define ARTIK_LWM2M_URI_DEVICE_BATT_STATUS     ARTIK_LWM2M_URI_DEVICE"/20"
#define ARTIK_LWM2M_URI_DEVICE_MEMORY_TOTAL    ARTIK_LWM2M_URI_DEVICE"/21"

#define ARTIK_LWM2M_URI_FIRMWARE               "/5/0"
#define ARTIK_LWM2M_URI_FIRMWARE_PACKAGE       ARTIK_LWM2M_URI_FIRMWARE"/0"
#define ARTIK_LWM2M_URI_FIRMWARE_PACKAGE_URI   ARTIK_LWM2M_URI_FIRMWARE"/1"
#define ARTIK_LWM2M_URI_FIRMWARE_UPDATE        ARTIK_LWM2M_URI_FIRMWARE"/2"
#define ARTIK_LWM2M_URI_FIRMWARE_STATE         ARTIK_LWM2M_URI_FIRMWARE"/3"
#define ARTIK_LWM2M_URI_FIRMWARE_UPD_SUPP_OBJ  ARTIK_LWM2M_URI_FIRMWARE"/4"
#define ARTIK_LWM2M_URI_FIRMWARE_UPDATE_RES    ARTIK_LWM2M_URI_FIRMWARE"/5"
#define ARTIK_LWM2M_URI_FIRMWARE_PKG_NAME      ARTIK_LWM2M_URI_FIRMWARE"/6"
#define ARTIK_LWM2M_URI_FIRMWARE_PKG_URI       ARTIK_LWM2M_URI_FIRMWARE"/7"

#define ARTIK_LWM2M_FIRMWARE_UPD_RES_DEFAULT   "0"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_SUCCESS   "1"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_SPACE_ERR "2"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_OOM       "3"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_CONNE_ERR "4"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_CRC_ERR   "5"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_PKG_ERR   "6"
#define ARTIK_LWM2M_FIRMWARE_UPD_RES_URI_ERR   "7"

#define ARTIK_LWM2M_FIRMWARE_STATE_IDLE        "0"
#define ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADING "1"
#define ARTIK_LWM2M_FIRMWARE_STATE_DOWNLOADED  "2"
#define ARTIK_LWM2M_FIRMWARE_STATE_UPDATING    "3"

/*!
 *	\brief This enum defines the types of supported LWM2M objects
 */
typedef enum {
	ARTIK_LWM2M_OBJECT_DEVICE = 0,
	ARTIK_LWM2M_OBJECT_FIRMWARE,
	ARTIK_LWM2M_OBJECT_CONNECTIVITY_MONITORING,
	ARTIK_LWM2M_OBJECT_COUNT
} artik_lwm2m_object_type_t;

/*!
 *	\brief This enum defines the events that can be associated to
 *	callback functions
 */
typedef enum {
	ARTIK_LWM2M_EVENT_ERROR = 0,
	ARTIK_LWM2M_EVENT_RESOURCE_EXECUTE,
	ARTIK_LWM2M_EVENT_RESOURCE_CHANGED,
	ARTIK_LWM2M_EVENT_COUNT
} artik_lwm2m_event_t;

/*!
 *	\brief Type representing LWM2M resource data content
 */
typedef void *artik_lwm2m_object_content_t;

/*!
 *	\brief Structure representing a LWM2M object
 */
typedef struct {
	artik_lwm2m_object_type_t type;
	artik_lwm2m_object_content_t *content;
} artik_lwm2m_object;

typedef struct {
    char *uri;
    unsigned char *buffer;
    int length;
} artik_lwm2m_resource_t;

/*!
 * \brief LWM2M Handle type
 *
 * Handle type used to carry instance specific
 * information for an LWM2M object
 */
typedef void *artik_lwm2m_handle;

/*!
 * \brief Callback function type. These callback functions can be associated
 * to events from artik_lwm2m_event_t by calling the \ref set_callback function
 */
typedef void (*artik_lwm2m_callback) (void *data, void *user_data);

/*!
 * \brief LWM2M configuration structure
 *
 * Structure containing LWM2M connection information fields
 */
typedef struct {
	/*!
	 * \brief Unique ID of the server
	 */
	int server_id;
	/*!
	 * \brief URI of the server.
	 *
	 * Format is <prefix>://<host>:<port>
	 * with prefix being one of the following options:
	 *
	 * "coap" for connecting using unsecure UDP
	 * "coaps" for connecting using UDP/DTLS
	 * "coap+tcp" for connecting using unsecure TCP
	 * "coaps+tcp" for connecting using TCP/TLS
	 *
	 */
	char *server_uri;
	/*!
	 * \brief Name of this client
	 */
	char *name;
	/*!
	 * \brief Lifetime of this client
	 */
	int lifetime;
	/*!
	 * \brief Certificate path for certificate based DTLS/TLS connection
	 */
	char *tls_cert_path;
	/*!
	 * \brief Key path for certificate based DTLS/TLS connection
	 */
	char *tls_key_path;
	/*!
	 * \brief Public Identity for PSK based DTLS/TLS connection
	 */
	char *tls_psk_identity;
	/*!
	 * \brief Secret key for PSK based DTLS/TLS connection
	 */
	char *tls_psk_key;
	/*!
	 * \brief LWM2M objects to expose
	 */
	artik_lwm2m_object * objects[ARTIK_LWM2M_OBJECT_COUNT];

} artik_lwm2m_config;

/*! \struct artik_lwm2m_module
 *
 *  \brief LWM2M module operations
 *
 *  Structure containing all the operations exposed by the LWM2M module
 */
typedef struct {
	/*!
	 * \brief Create an LWM2M client and connect it to the server
	 *
	 * \param[in] handle Pointer of a handle variable
	 * \param[in] params Pointer of a structure configuring client
	 *
	 * \return S_OK on success, error code otherwise
	 */
	artik_error(*client_connect)(artik_lwm2m_handle * handle, artik_lwm2m_config * config);
	/*!
	 * \brief Disconnect from the LWM2M server
	 *
	 * \param[in] handle client-specific handle returned by \ref client_connect
	 *
	 * \return S_OK on success, error code otherwise
	 */
	artik_error(*client_disconnect)(artik_lwm2m_handle handle);
	/*!
	 * \brief Write LWM2M resource value
	 *
	 * \param[in] handle client-specific handle returned by \ref client_connect
	 * \param[in] uri URI of resource to write
	 * \param[in] buffer pointer to data buffer that should be written to the resource
	 * \param[in] length length of the data buffer
	 *
	 * \return S_OK on success, error code otherwise
	 */
	artik_error(*client_write_resource)(artik_lwm2m_handle handle,
			const char *uri, unsigned char *buffer, int length);
	/*!
	 * \brief Read LWM2M resource value
	 *
	 * \param[in] handle client-specific handle returned by \ref client_connect
	 * \param[in] uri URI of resource to read
	 * \param[in] buffer pointer to a preallocated data buffer that will be filled
	 * by the function with the resource data
	 * \param[inout] length pointer to a variable containing the length of the preallocated
	 * data buffer. Upon successful read, the function fills this value with the actual
	 * length of the data that has been read and copied into the buffer.
	 *
	 * \return S_OK on success, error code otherwise. If the buffer passed to hold
	 * the read data is too small, E_NO_MEM is returned.
	 */
	artik_error(*client_read_resource)(artik_lwm2m_handle handle,
			const char *uri, unsigned char *buffer, int *length);
	/*!
	 * \brief Set a callback function to be called upon event occurrence
	 *
	 * \param[in] handle client-specific handle returned by \ref client_connect
	 * \param[in] event type of event to associate the callback function to
	 * \param[in] user_callback callback function to call upon occurrence of the associated event
	 * \param[in] user_data pointer to some user data that will be passed back as a parameter
	 * to the callback function
	 *
	 * \return S_OK on success, error code otherwise.
	 */
	artik_error(*set_callback)(artik_lwm2m_handle handle, artik_lwm2m_event_t event,
			artik_lwm2m_callback user_callback, void *user_data);
	/*!
	 * \brief Unset a callback function. It will no longer be called upon event occurrence
	 *
	 * \param[in] handle client-specific handle returned by \ref client_connect
	 * \param[in] event type of event to unregister the callback function from
	 *
	 * \return S_OK on success, error code otherwise.
	 */
	artik_error(*unset_callback)(artik_lwm2m_handle handle, artik_lwm2m_event_t event);
	/*!
	 * \brief Create a LWM2M "Device" object to be exposed by the client, and
	 * initialize it with default values.
	 *
	 * Details about the LWM2M Device object resources can be found
	 * <a href="http://dev_devtoolkit.openmobilealliance.org/IoT/LWM2M10/doc/TS/index.html#!Documents/lwm2mobjectdevice.htm">
	 *     here
	 * </a>
	 *
	 * \param[in] manufacturer Manufacturer name
	 * \param[in] model Model number
	 * \param[in] serial Serial Number
	 * \param[in] fw_version Firmware Version
	 * \param[in] hw_version Hardware Version
	 * \param[in] sw_version Software Version
	 * \param[in] device_type Device Type
	 * \param[in] power_source Power Source
	 * \param[in] power_volt Power Voltage (mV)
	 * \param[in] power_current Power Current (mA)
	 * \param[in] battery_level Battery Level (%)
	 * \param[in] memory_total Total amount of storage memory (KB)
	 * \param[in] memory_free Current amount of free storage memory (KB)
	 * \param[in] time_zone Time Zone according to the IANA standard
	 * \param[in] utc_offset Time Zone offset with respect to UTC (format: <+|->HH:MM)
	 * \param[in] binding Binding Modes (e.g. "U", "UQ, "S", "SQ")
	 *
	 * \return pointer to the newly created object. When this object is no longer used,
	 * it must be released by calling \ref free_object.
	 */
	artik_lwm2m_object*(*create_device_object)(const char *manufacturer, const char *model,
			const char *serial, const char *fw_version, const char *hw_version,
			const char *sw_version,	const char *device_type, int power_source,
			int power_volt, int power_current, int battery_level, int memory_total,
			int memory_free, const char *time_zone, const char *utc_offset,
			const char *binding);
	/*!
	 * \brief Create a LWM2M "Firmware" object to be exposed by the client, and
	 * initialize it with default values.
	 *
	 * Details about the LWM2M Device object resources can be found
	 * <a href="http://dev_devtoolkit.openmobilealliance.org/IoT/LWM2M10/doc/TS/index.html#!Documents/lwm2mobjectdevice.htm">
	 *     here
	 * </a>
	 *
	 * \param[in] supported Update supported objects
	 * \param[in] pkg_name Package name
	 * \param[in] pkg_version Package version
	 *
	 * \return pointer to the newly created object. When this object is no longer used,
	 * it must be released by calling \ref free_object.
	 */
	artik_lwm2m_object *(*create_firmware_object)(bool supported, char *pkg_name,
			char *pkg_version);
	/*!
	 * \brief Create a LWM2M "Connectivity monitoring" object connectivity monitoring to be exposed by the client, and
	 * initialize it with default values.
	 *
	 * Details about the LWM2M Connectivity monitoring object resources can be found
	 * <a href="http://dev_devtoolkit.openmobilealliance.org/IoT/LWM2M10/doc/TS/index.html#!Documents/lwm2mobjectdevice.htm">
	 *     here
	 * </a>
	 *
	 * \param[in] netbearer Indicates the network bearer used for the current LWM2M communication session
	 * \param[in] avlnetbearer Indicates list of current available network bearer.
	 * \param[in] signalstrength Average value of the received signal strength indication used in the current network bearer.
	 * \param[in] linkquality This contains received link quality.
	 * \param[in] lenip Is the size of the array ipaddr .
	 * \param[in] ipaddr The IP addresses assigned to the connectivity interface.
	 * \param[in] lenroute Is the size of the array routeaddr .
	 * \param[in] routeraddr The IP address of the next-hop IP router (Only two field are read).
	 * \param[in] linkutilization The average utilization of the link to the next-hop IP router in %.
	 * \param[in] apn Access Point Name in case Network Bearer Resource is a Cellular Network.
	 * \param[in] cellid Serving Cell ID in case Network Bearer Resource is a Cellular Network.
	 * \param[in] smnc Serving Mobile Network Code.
	 * \param[in] smcc Serving Mobile Country Code.
	 *
	 * \return pointer to the newly created object. When this object is no longer used,
	 * it must be released by calling \ref free_object.
	 */
	artik_lwm2m_object*(*create_connectivity_monitoring_object)(int netbearer, int avlnetbearer,
								    int signalstrength, int linkquality,
								    int lenip, const char **ipaddr,
								    int lenroute, const char **routeaddr,
								    int linkutilization, const char *apn,
								    int cellid, int smnc, int smcc);

	/*!
	 * \brief Free a previously created LWM2M object
	 *
	 * \param[in] object object to release, all its associated memory will be released
	 *
	 * \return none
	 */
	void (*free_object)(artik_lwm2m_object *object);

	/*!
	 * \brief Serialized data integer into resource lwm2m
	 *
	 * \param[in] data Array of integer to serialize
	 * \param[in] size of the data array
	 * \param[out] buffer Pointer on a array wich it will be allocated with the serialized data.
	 * \param[out] lenbuffer Size is the number of cells into the buffer.
	 *
	 * \return S_OK on success, error code otherwise.
	 */
	artik_error(*serialize_tlv_int)(int *data, int size, unsigned char **buffer, int *lenbuffer);

	/*!
	 * \brief Serialized data string into resource lwm2m
	 *
	 * \param[in] data Array of string to serialize
	 * \param[in] size of the data array
	 * \param[out] buffer Pointer on a array wich it will be allocated with the serialized data.
	 * \param[out] lenbuffer Size is the number of cells into the buffer.
	 *
	 * \return S_OK on success, error code otherwise.
	 */
	artik_error(*serialize_tlv_string)(char **data, int size, unsigned char **buffer, int *lenbuffer);

} artik_lwm2m_module;

extern const artik_lwm2m_module lwm2m_module;

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_ARTIK_LWM2M_H_ */
