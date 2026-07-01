/**
 * @file      rtk_bt_gatts.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth GATT server part type and function deffinition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_GATTS_H__
#define __RTK_BT_GATTS_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <bt_api_config.h>
#include <dlist.h>

/**
 * @def       RTK_BT_GATT_INTERNAL
 * @brief     This attribute will be handled by stack internal. When the attribute is a
 *            characteristic value, this flag is equal to @ref RTK_BT_GATT_VOID.
 */
#define RTK_BT_GATT_INTERNAL                0

/**
 * @def       RTK_BT_GATT_APP
 * @brief     This attribute will be handled by user APP. All read/write to this attribute
 *            will be indicated to user APP by a callback, and user APP need to call
 *            @ref rtk_bt_gatts_read_resp or @ref rtk_bt_gatts_write_resp to response to
 *            read/write request from remote and modify the attribute value maintained
 *            in user APP when recevie write request.
 */
#define RTK_BT_GATT_APP                     1

/**
 * @def       RTK_BT_GATT_VOID
 * @brief     This attribute value will be passed into lower stack as a pointer. If this
 *            attribute is read by remote, lower stack will auto send this attribute value
 *            to response to read request. This flag is not suggested to be used when the
 *            attribute is writable(including encrypted/authenticated write), because it
 *            will not indicate APP the write events.
 */
#define RTK_BT_GATT_VOID                    2

/**
 * @struct    rtk_bt_gatt_chrc
 * @brief     Characteristic Attribute Value.
 */
struct rtk_bt_gatt_chrc {
	const struct bt_uuid *uuid;             /*!< Characteristic UUID. */
	uint16_t value_handle;                  /*!< Characteristic Value handle. */
	uint8_t properties;                     /*!< Characteristic properties. */
};

/**
 * @def       RTK_BT_GATT_ATTRIBUTE
 * @brief     Declaration Macro.
 *            Helper macro to declare an attribute.
 * @param     _uuid Attribute uuid.
 * @param     _perm Attribute access permissions.
 * @param     _user_data Attribute user data.
 * @param     _len Attribute user data length.
 * @param     _flag Attribute flag.
 */
#define RTK_BT_GATT_ATTRIBUTE(_uuid, _perm, _user_data, _len, _flag) \
    {                                                                \
        .uuid = (struct bt_uuid *)_uuid,                             \
        .perm = _perm,                                               \
        .user_data = (void*)_user_data,                              \
        .len = _len,                                                 \
        .flag = _flag,                                               \
    }

/**
 * @def       RTK_BT_GATT_PRIMARY_SERVICE
 * @brief     Primary Service Declaration Macro.
 *            Helper macro to declare a primary service attribute.
 * @param     _service Service attribute value. @ref struct bt_uuid *
 */
#define RTK_BT_GATT_PRIMARY_SERVICE(_service)                          \
    RTK_BT_GATT_ATTRIBUTE(BT_UUID_GATT_PRIMARY, RTK_BT_GATT_PERM_READ, \
                          _service, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_SECONDARY_SERVICE
 * @brief     Secondary Service Declaration Macro.
 *            Helper macro to declare a secondary service attribute.
 * @param     _service Service attribute value. @ref struct bt_uuid *
 */
#define RTK_BT_GATT_SECONDARY_SERVICE(_service)                          \
    RTK_BT_GATT_ATTRIBUTE(BT_UUID_GATT_SECONDARY, RTK_BT_GATT_PERM_READ, \
                          _service, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_INCLUDE_SERVICE
 * @brief     Include Service Declaration Macro.
 *            Helper macro to declare database internal include service attribute.
 * @param     _service_incl Pointer to a include service struct @ref struct rtk_bt_gatt_service *
 */
#define RTK_BT_GATT_INCLUDE_SERVICE(_service_incl)                     \
    RTK_BT_GATT_ATTRIBUTE(BT_UUID_GATT_INCLUDE, RTK_BT_GATT_PERM_READ, \
                          _service_incl, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_CHRC_INIT
 * @brief     Initialization Macro.
 *            Helper macro to init an attribute.
 * @param     _uuid Attribute uuid.
 * @param     _handle Attribute handle.
 * @param     _props Attribute access properties.
 */
#define RTK_BT_GATT_CHRC_INIT(_uuid, _handle, _props) \
    {                                                 \
        .uuid = (struct bt_uuid *)_uuid,                                \
        .value_handle = _handle,                      \
        .properties = _props,                         \
    }

/**
 * @def       RTK_BT_GATT_CHARACTERISTIC
 * @brief     Characteristic and Value Declaration Macro.
 *            Helper macro to declare a characteristic attribute along with its
 *            attribute value.
 * @param     _uuid Characteristic attribute uuid.
 * @param     _props Characteristic attribute properties.
 * @param     _perm Characteristic Attribute access permissions.
 */
#define RTK_BT_GATT_CHARACTERISTIC(_uuid, _props, _perm)                \
    RTK_BT_GATT_ATTRIBUTE(BT_UUID_GATT_CHRC, RTK_BT_GATT_PERM_READ,     \
                          ((struct rtk_bt_gatt_chrc[]){                 \
                              RTK_BT_GATT_CHRC_INIT(_uuid, 0U, _props), \
                          }), 0, RTK_BT_GATT_INTERNAL),                 \
    RTK_BT_GATT_ATTRIBUTE(_uuid, _perm, NULL, 0, RTK_BT_GATT_APP)

/**
 * @def       RTK_BT_GATT_DESCRIPTOR
 * @brief     Descriptor Declaration Macro.
 *            Helper macro to declare a descriptor attribute.
 * @note      If use RTK_BT_GATT_DESCRIPTOR directly, MUST use _user_data as a raw data!
 * @param     _uuid Descriptor attribute uuid.
 * @param     _perm Descriptor attribute access permissions.
 * @param     _user_data Descriptor attribute user data.
 * @param     _len Attribute user data length.
 * @param     _flag Attribute flag.
 */
#define RTK_BT_GATT_DESCRIPTOR(_uuid, _perm, _user_data, _len, _flag) \
    RTK_BT_GATT_ATTRIBUTE(_uuid, _perm, _user_data, _len, _flag)

/**
 * @def       RTK_BT_GATT_CEP
 * @brief     Characteristic Extended Properties Declaration Macro.
 *            Helper macro to declare a CEP attribute.
 * @param     _value Pointer to a struct rtk_bt_gatt_cep. (const struct rtk_bt_gatt_cep *)
 */
#define RTK_BT_GATT_CEP(_value) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_CEP, RTK_BT_GATT_PERM_READ, (void *)_value, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_CUD
 * @brief     Characteristic User Format Descriptor Declaration Macro.
 *            Helper macro to declare a CUD attribute.
 *
 * @param     _value User description NULL-terminated C string. (const uint8_t *value)
 * @param     _perm Descriptor attribute access permissions.
 */
#define RTK_BT_GATT_CUD(_value, _perm) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_CUD, _perm, (void *)_value, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_CCC
 * @brief     Client Characteristic Configuration Declaration Macro.
 *            Helper macro to declare a CCC attribute.
 * @note      Although CCCD value read/write will be handled internally, but User APP will be notified
 * @param     _perm CCC access permissions.
 */
#define RTK_BT_GATT_CCC(_perm) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_CCC, _perm, 0, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_SCC
 * @brief     Server Characteristic Configuration Declaration Macro.
 *            Helper macro to declare a CCC attribute.
 * @param     _value Poniter to Server Characteristic Configuration bits. (struct rtk_bt_gatt_scc *)
 * @param     _perm CCC access permissions.
 */
#define RTK_BT_GATT_SCC(_value, _perm) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_SCC, _perm, (void *)_value, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_CPF
 * @brief     Characteristic Presentation Format Descriptor Declaration Macro.
 *            Helper macro to declare a CPF attribute.
 * @param     _value Pointer to a struct rtk_bt_gatt_cpf. (struct rtk_bt_gatt_cpf*)
 */
#define RTK_BT_GATT_CPF(_value) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_CPF, RTK_BT_GATT_PERM_READ, (void *)_value, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_CAF
 * @brief     Characteristic Aggregate Format Descriptor Declaration Macro.
 *            Helper macro to declare a CAF attribute.
 * @param     _value Pointer to a struct rtk_bt_gatt_caf. (struct rtk_bt_gatt_caf*)
 */
#define RTK_BT_GATT_CAF(_value) \
    RTK_BT_GATT_DESCRIPTOR(BT_UUID_GATT_CAF, RTK_BT_GATT_PERM_READ, (void *)_value, 0, RTK_BT_GATT_INTERNAL)

/**
 * @def       RTK_BT_GATT_SERVICE
 * @brief     Service Structure Declaration Macro.
 *            Helper macro to declare a service structure.
 * @param     _attrs Service attributes.
 * @param     _app_id Service app id.
 */
#define RTK_BT_GATT_SERVICE(_attrs, _app_id)                  \
    {                                                         \
        .attrs = _attrs,                                      \
        .attr_count = (sizeof(_attrs) / sizeof((_attrs)[0])), \
        .app_id = _app_id,                                    \
    }

/**
 * @typedef   rtk_bt_gatts_client_supported_features_t
 * @brief     GATT Service Client Supported Features bit field.
 */
typedef enum {
	RTK_BT_GATTS_CLIENT_SUPPORTED_FEATURES_ROBUST_CACHING_BIT = 0x01, /*!< The client supports robust caching. */
	RTK_BT_GATTS_CLIENT_SUPPORTED_FEATURES_EATT_BEARER_BIT = 0x02,    /*!< The client supports Enhanced ATT bearer. */
	RTK_BT_GATTS_CLIENT_SUPPORTED_FEATURES_MULTI_NOTIF_BIT = 0x04,    /*!< The client supports receiving ATT_MULTIPLE_HANDLE_VALUE_NTF PDUs. */
} rtk_bt_gatts_client_supported_features_t;

/**
 * @typedef   rtk_bt_gatts_server_supported_features_t
 * @brief     GATT Service Server Supported Features bit field.
 */
typedef enum {
	RTK_BT_GATTS_SERVER_SUPPORTED_FEATURES_EATT_BIT = 0x01,     /*!< The server supports Enhanced ATT bearer. */
} rtk_bt_gatts_server_supported_features_t;

/**
 * @typedef   rtk_bt_gatts_srv_type_t
 * @brief     Service type.
 */
typedef enum {
	GATT_SERVICE_OVER_BREDR,                /*!< 0, Service over BREDR. */
	GATT_SERVICE_OVER_BLE,                  /*!< 1, Service over BLE. */
	GATT_SERVICE_TYPE_RESERVED,             /*!< 2, RESERVED. */
} rtk_bt_gatts_srv_type_t;


/**
 * @struct    rtk_bt_gatt_cpf
 * @brief     GATT Characteristic Presentation Format Attribute Value.
 */
struct rtk_bt_gatt_cpf {
	uint8_t format;                         /*!< Format of the value of the characteristic */
	int8_t exponent;                        /*!< Exponent field to determine how the value of this characteristic is further formatted */
	uint16_t unit;                          /*!< Unit of the characteristic */
	uint8_t name_space;                     /*!< Name space of the description */
	uint16_t description;                   /*!< Description of the characteristic as defined in a higher layer profile */
};

/**
 * @struct    rtk_bt_gatt_caf
 * @brief     GATT Characteristic Aggregate Format Attribute Value.
 */
struct rtk_bt_gatt_caf {
	uint16_t count;                         /*!< The count of List of Attribute Handles */
	uint16_t *handles;                      /*!< List of Attribute Handles */
};


/**
 * @struct    rtk_bt_gatt_cep
 * @brief     Characteristic Extended Properties Attribute Value.
 */
struct rtk_bt_gatt_cep {
	uint16_t properties;                    /*!< Characteristic Extended properties */
};


/**
 * @struct    rtk_bt_gatt_scc
 * @brief     Server Characterestic Configuration Attribute Value.
 */
struct rtk_bt_gatt_scc {
	uint16_t flags;                         /*!< Server Characteristic Configuration flags */
};

/**
 * @struct    rtk_bt_gatt_attr_t
 * @brief     GATT Attribute structure.
 */
typedef struct {
	const struct bt_uuid *uuid;             /*!< Attribute UUID */
	union {
		void *read;                         /*!< Attribute read callback */
		uint16_t flag;                      /*!< Attribute flag (RTK_BT_GATT_INTERNAL/RTK_BT_GATT_APP) */
	};
	union {
		void *write;                        /*!< Attribute write callback */
		uint16_t len;                       /*!< Attribute user data length */
	};
	void *user_data;                        /*!< Attribute user data */
	uint16_t handle;                        /*!< Attribute handle */
	uint8_t perm;                           /*!< Attribute permissions */
} rtk_bt_gatt_attr_t;

/**
 * @struct    rtk_bt_gatts_read_resp_param_t
 * @brief     Bluetooth GATT server response paramter for remote client's read request.
 */
typedef struct {
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< ID of L2CAP channel to send the response, MUST be same as cid in @ref rtk_bt_gatts_read_ind_t. Ignored when RTK_BLE_MGR_LIB is 0. */
	uint16_t index;                         /*!< Attribute index in service */
	uint8_t err_code;                       /*!< Error code, @ref rtk_bt_err_att , if NOT ERR_RESP, equals 0 */
	uint16_t len;                           /*!< Response Value length, when err_code == 0 */
	const void *data;                       /*!< Response Value data, when err_code == 0 */
} rtk_bt_gatts_read_resp_param_t;

/**
 * @struct    rtk_bt_gatts_write_resp_param_t
 * @brief     Bluetooth GATT server response paramter for remote client's write request.
 */
typedef struct {
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< ID of L2CAP channel to send the response, 0 indicates auto-select. Ignored when RTK_BLE_MGR_LIB is 0. */
	uint16_t index;                         /*!< Attribute index in service */
	uint8_t type;                           /*!< Write type */
	uint8_t err_code;                       /*!< Error code, @ref rtk_bt_err_att , if NOT ERR_RESP, equals 0 */
} rtk_bt_gatts_write_resp_param_t;

/**
 * @struct    rtk_bt_gatts_ntf_and_ind_param_t
 * @brief     Bluetooth GATT server indicate paramter definition.
 */
typedef struct {
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< ID of L2CAP channel to send the response, 0 indicates auto-select. Ignored when RTK_BLE_MGR_LIB is 0. */
	uint16_t index;                         /*!< Attribute index in service */
	uint16_t len;                           /*!< Indicate Value length */
	const void *data;                       /*!< Indicate Value data */
} rtk_bt_gatts_ntf_and_ind_param_t;

/**
 * @struct    rtk_bt_gatts_service_changed_indicate_param_t
 * @brief     Bluetooth GATT server service changed indicate paramter definition.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< ID of L2CAP channel, 0 indicates auto-select. Ignored when RTK_BLE_MGR_LIB is 0. */
	uint16_t start_handle;                  /*!< Start of affected attribute handle range */
	uint16_t end_handle;                    /*!< End of affected attribute handle range */
} rtk_bt_gatts_service_changed_indicate_param_t;

/**
 * @struct    rtk_bt_gatts_get_attr_handle_param_t
 * @brief     Bluetooth GATT server get attribute handle paramter definition.
 */
typedef struct {
	uint16_t app_id;                        /*!< Every service has a app_id */
	uint16_t attr_index;                    /*!< Attribute index in a service */
	uint16_t *attr_handle;                  /*!< Handle of attribute */
} rtk_bt_gatts_get_attr_handle_param_t;

/**
 * @struct    rtk_bt_gatt_service
 * @brief     GATT Service structure.
 */
struct rtk_bt_gatt_service {
	struct list_head list;                  /*!< Service List */
	uint8_t alloc_ind;                      /*!< indicate whether this rtk_bt_gatt_service is mallocated */
	uint16_t app_id;                        /*!< Service app_id */
	rtk_bt_gatt_attr_t *attrs;              /*!< Service Attributes */
	uint16_t attr_count;                    /*!< Service Attribute count */
#if !defined(RTK_BLE_MGR_LIB) || !RTK_BLE_MGR_LIB
	bool assgin_handle_flag;                /*!< Flag of if the service start attr handle is assigned by user */
	uint16_t start_handle;                  /*!< User assigned start attr handle of service */
#endif
	rtk_bt_gatts_srv_type_t type;           /*!< Service type */
	uint8_t register_status;                /*!< Service register status */
	void *user_data;                        /*!< Service user data */
	uint32_t server_info;                   /*!< Service info */
};


/**
 * @struct    rtk_bt_gatts_reg_ind_t
 * @brief     Bluetooth GATT service register event msg.
 */
typedef struct {
	uint16_t reg_status;                    /*!< Service register status */
	uint16_t app_id;                        /*!< Registered service's app_id */
} rtk_bt_gatts_reg_ind_t;

/**
 * @struct    rtk_bt_gatt_mtu_exchange_ind_t
 * @brief     MTU exchange indication.
 */
typedef struct {
	uint16_t result;                        /*!< MTU exchange result */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t mtu_size;                      /*!< MTU size */
} rtk_bt_gatt_mtu_exchange_ind_t;

/**
 * @struct    rtk_bt_gatts_read_ind_t
 * @brief     Bluetooth GATT server received remote client read request event msg.
 */
typedef struct {
	uint16_t app_id;                        /*!< Every service has a app_id */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID to indicate which channel the request is received from */
	uint16_t index;                         /*!< Attribute index in service */
	uint16_t offset;                        /*!< offset in data, for blob read */
} rtk_bt_gatts_read_ind_t;

/**
 * @enum   rtk_bt_gatts_write_type_t
 * @brief     Bluetooth GATT server received remote write request type definition.
 */
typedef enum {
	RTK_BT_GATTS_WRITE_REQ,             /*!< GATT client write request */
	RTK_BT_GATTS_WRITE_NO_RESP,         /*!< GATT client write cmd */
	RTK_BT_GATTS_WRITE_NO_RESP_SIGNED,  /*!< GATT client write cmd (signed) */
	RTK_BT_GATTS_WRITE_LONG,            /*!< GATT client write long */
} rtk_bt_gatts_write_type_t;


/**
 * @struct    rtk_bt_gatts_write_ind_t
 * @brief     Bluetooth GATT server received remote client write request event msg.
 */
typedef struct {
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID to indicate which channel the request is received from */
	uint16_t index;                         /*!< Attribute index in service */
	uint16_t offset;                        /*!< offset in data, for queue write */
	rtk_bt_gatts_write_type_t  type;        /*!< Write type */
	uint16_t len;                           /*!< Length of write data */
	uint8_t *value;                         /*!< Write data */
} rtk_bt_gatts_write_ind_t;

/**
 * @struct    rtk_bt_gatts_cccd_ind_t
 * @brief     Bluetooth GATTS cccd updated event msg.
 */
typedef struct {
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID to indicate which channel the request is received from */
	uint16_t index;                         /*!< Attribute index in service */
	uint16_t value;                         /*!< cccd value */
} rtk_bt_gatts_cccd_ind_t;

/**
 * @struct    rtk_bt_gatts_ntf_and_ind_ind_t
 * @brief     Bluetooth GATT server send notify or indicate complete msg.
 */
typedef struct {
	uint16_t app_id;                        /*!< Every service has a app_id. */
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID to indicate which channel the data is sent to */
	uint16_t index;                         /*!< Attribute index in service */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	uint16_t err_code;                      /*!< Error code, if no error is 0. */
} rtk_bt_gatts_ntf_and_ind_ind_t;

/**
 * @struct    rtk_bt_gatts_client_supported_features_ind_t
 * @brief     Bluetooth GATT server's Client Supported Features Characteristic writed event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID to indicate which channel the write procedure is processed. */
	uint8_t features;                       /*!< Client Supported Features after writed, which is bit combination of @ref rtk_bt_gatts_client_supported_features_t. */
} rtk_bt_gatts_client_supported_features_ind_t;

/**
 * @struct    rtk_bt_gatts_service_changed_cccd_ind_t
 * @brief     Bluetooth GATT server's Service Changed Characteristic cccd updated event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle for a client */
	uint16_t cid;                           /*!< L2CAP channel ID */
	bool cccd_enable;                       /*!< cccd bits is enabled or disabled */
} rtk_bt_gatts_service_changed_cccd_ind_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_gatts BT GATT Server APIs
 * @brief     BT GATT server related function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief     Register GATT service, will cause event @ref RTK_BT_GATTS_EVT_REGISTER_SERVICE
 * @param[in] param: The parameters for registering service app.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_register_service(struct rtk_bt_gatt_service *param);

/**
 * @brief      Notify action initiated by server, will cause event @ref RTK_BT_GATTS_EVT_NOTIFY_COMPLETE_IND.
 *             If attribute value is longer than ATT_MTU-3 octects, it will fail.
 * @param[in]  param: The parameters for notification.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_notify(rtk_bt_gatts_ntf_and_ind_param_t *param);

/**
 * @brief      Indicate action initiated by server, will cause event @ref RTK_BT_GATTS_EVT_INDICATE_COMPLETE_IND
 *             If attribute value is longer than ATT_MTU-3 octects, it will fail.
 * @param[in]  param: The parameters for indication.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_indicate(rtk_bt_gatts_ntf_and_ind_param_t *param);

/**
 * @brief      Server response for client read operation.
 * @param[in]  param: The parameters for read response.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_read_resp(rtk_bt_gatts_read_resp_param_t *param);

/**
 * @brief     Server response for client write operation.
 * @param[in] param: The parameters for write response.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_write_resp(rtk_bt_gatts_write_resp_param_t *param);

/**
 * @brief     Server send service changed indication when builtin service is used.
 * @param[in] conn_handle: Connection handle for a client.
 * @param[in] cid: ID of L2CAP channel, 0 indicates auto-select. Ignored when RTK_BLE_MGR_LIB is 0.
 * @param[in] start_handle: Start of affected attribute handle range.
 * @param[in] end_handle: End of affected attribute handle range.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_service_changed_indicate(uint16_t conn_handle, uint16_t cid,
											   uint16_t start_handle, uint16_t end_handle);

/**
 * @brief     Server get attribute handle.
 * @param[in] app_id: Every service has a app_id.
 * @param[in] attr_index: Attribute index in a service.
 * @param[out] attr_handle: Handle of attribute.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gatts_get_attribute_handle(uint16_t app_id, uint16_t attr_index, uint16_t *attr_handle);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_GATTS_H__ */
