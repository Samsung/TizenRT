/**
 * @file      rtk_bt_gattc.h
 * @author    benling_xu@realsil.com.cn
 * @brief     Bluetooth GATT client part type and function definition
 * @copyright Copyright (c) 2022. Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __RTK_BT_GATTC_H__
#define __RTK_BT_GATTC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <bt_api_config.h>
#include <dlist.h>
#include <rtk_bt_def.h>

#if defined(RTK_BLE_MGR_LIB) && RTK_BLE_MGR_LIB

/**
 * @struct    rtk_bt_gattc_uuid_t
 * @brief     Define service UUID structure
 */
typedef struct {
	bool is_uuid16;
	uint8_t instance_id;            /*!< Used when the uuid is the same. Start from zero. */
	union {
		uint16_t uuid16;
		uint8_t uuid128[16];
	} p;
} rtk_bt_gattc_uuid_t;

/**
 * @struct    rtk_bt_gattc_register_t
 * @brief     Bluetooth GATT client register callback for specific service.
 */
typedef struct {
	uint16_t profile_id;                    /*!< The id of client profile */
	rtk_bt_gattc_uuid_t srv_uuid;           /*!< Service UUID */
} rtk_bt_gattc_register_t;

/**
 * @typedef   rtk_bt_gattc_cccd_cfg_t
 * @brief     Bluetooth GATT cccd value.
 */
typedef enum {
	RTK_BT_GATT_CCCD_DISABLE = 0x00,        /*!< Client disable service cccd */
	RTK_BT_GATT_CCCD_NOTIFICATION = 0x01,   /*!< Client enable service notification */
	RTK_BT_GATT_CCCD_INDICATION = 0x02,     /*!< Client enable service indication */
	RTK_BT_GATT_CCCD_ALL = 0x03,            /*!< Client enable all properties service supported */
} rtk_bt_gattc_cccd_cfg_t;

/**
 * @typedef   rtk_bt_gattc_find_type_t
 * @brief     Bluetooth GATT client find type.
 */
typedef enum {
	RTK_BT_GATT_FIND_CHARACTERISTIC_HANDLE,     /*!< Find characteristic handle by characteristic UUID */
	RTK_BT_GATT_FIND_CHARACTERISTIC_DESC_RANGE, /*!< Find handle range of characteristic descriptors by characteristic UUID */
	RTK_BT_GATT_FIND_CHARACTERISTIC_CCCD_HANDLE,/*!< Find characteristic cccd descriptor by characteristic UUID */
	RTK_BT_GATT_FIND_INCLUDE_SERVICE,           /*!< Find if a service is included in specific primary service by service UUID */
} rtk_bt_gattc_find_type_t;

/**
 * @struct    rtk_bt_gattc_find_param_t
 * @brief     Bluetooth GATT client discover paramter for different Operation.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_gattc_find_type_t type;          /*!< Find type */

	union {
		struct find_char_param {
			rtk_bt_gattc_uuid_t srv_uuid;   /*!< Service UUID */
			rtk_bt_gattc_uuid_t char_uuid;  /*!< Characteristic UUID */
			uint16_t *p_handle;             /*!< Characteristic handle */
		} find_char;                        /*!< Find characteristic handle by characteristic UUID */

		struct find_char_range_param {
			rtk_bt_gattc_uuid_t srv_uuid;   /*!< Service UUID */
			rtk_bt_gattc_uuid_t char_uuid;  /*!< Characteristic UUID */
			uint16_t *p_handle_start;       /*!< Start handle of characteristic descriptors */
			uint16_t *p_handle_end;         /*!< End handle of characteristic descriptors */
			uint16_t *p_cccd_handle;        /*!< CCCD handle. When CCCD handle is zero,this characteristic does not contain the Client Characteristic Configuration descriptor. */
		} find_char_range;                  /*!< Find handle range of characteristic descriptors by characteristic UUID */

		struct find_char_cccd_param {
			rtk_bt_gattc_uuid_t srv_uuid;   /*!< Service UUID */
			rtk_bt_gattc_uuid_t char_uuid;  /*!< Characteristic UUID */
			uint16_t *p_handle;             /*!< Characteristic cccd handle */
			bool *p_notify;                 /*!< Characteristic support notify or not */
			bool *p_indicate;               /*!< Characteristic support indicate or not */
		} find_char_cccd;                   /*!< Find characteristic cccd descriptor by characteristic UUID */

		struct find_include_srv_param {
			rtk_bt_gattc_uuid_t primary_uuid;/*!< Primary service UUID */
			rtk_bt_gattc_uuid_t include_uuid;/*!< Include service UUID */
			uint8_t *p_instance_num;        /*!< Included service instance number */
			uint8_t instance_id[20];        /*!< Included service instance id array */
		} find_include_srv;                 /*!< Find if a service is included in specific primary service by service UUID */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_find_param_t */
} rtk_bt_gattc_find_param_t;

/**
 * @typedef   rtk_bt_gattc_read_type_t
 * @brief     Bluetooth GATT client read type.
 */
typedef enum {
	RTK_BT_GATT_CHAR_READ_BY_HANDLE,            /*!< Read characteristic value */
	RTK_BT_GATT_CHAR_READ_BY_UUID,              /*!< Read using characteristic UUID */
} rtk_bt_gattc_read_type_t;

/**
 * @struct    rtk_bt_gattc_read_param_t
 * @brief     Bluetooth GATT client read paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_gattc_read_type_t type;          /*!< Characteristic value read type */
	union {
		struct by_handle_param {
			uint16_t handle;                /*!< Characteristic value handle */
		} by_handle;                        /*!< Read characteristic value */

		struct by_uuid_param {
			uint16_t start_handle;          /*!< Start attribute handle for search range */
			uint16_t end_handle;            /*!< End attribute handle for search range */
			rtk_bt_gattc_uuid_t char_uuid;  /*!< Characteristic UUID */
		} by_uuid;                          /*!< Read using characteristic UUID */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_read_param_t */
} rtk_bt_gattc_read_param_t;

/**
 * @typedef   rtk_bt_gattc_write_type_t
 * @brief     Bluetooth GATT client write type.
 */
typedef enum {
	RTK_BT_GATT_CHAR_WRITE_REQ = 0x00,          /*!< Write characteristic value request */
	RTK_BT_GATT_CHAR_WRITE_NO_RSP,              /*!< Write characteristic value without response */
	RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED,       /*!< Write characteristic value without response and with signed data */
} rtk_bt_gattc_write_type_t;

/**
 * @struct    rtk_bt_gattc_write_param_t
 * @brief     Bluetooth GATT client write paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	rtk_bt_gattc_write_type_t type;         /*!< Characteristic write type */
	uint16_t handle;                        /*!< Characteristic value handle */
	uint16_t length;                        /*!< Length of the data */
	void *data;                             /*!< Data to be written */
} rtk_bt_gattc_write_param_t;

/**
 * @struct    rtk_bt_gattc_update_cccd_param_t
 * @brief     Bluetooth GATT client write cccd paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	bool srv_cfg;                           /*!< If cccd config is for whole service or a characteristic */
	rtk_bt_gattc_uuid_t srv_uuid;           /*!< Service UUID value */
	rtk_bt_gattc_uuid_t char_uuid;          /*!< Characteristic UUID value, ignored when srv_cfg is true. */
	bool bnotify;                           /*!< If notify bit is writed */
	bool bindicate;                         /*!< If indicate bit is writed */
} rtk_bt_gattc_update_cccd_param_t;

/**
 * @struct    rtk_bt_gattc_discover_ind_t
 * @brief     Bluetooth GATTC event discover result event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	bool is_found;                          /*!< If service is found */
	bool load_from_storage;                 /*!< GATTC discover result load from storage or not */
	uint8_t srv_instance_num;               /*!< Service instance number */
} rtk_bt_gattc_discover_ind_t;

/**
 * @struct    rtk_bt_gattc_discover_all_state_ind_t
 * @brief     Bluetooth GATTC event discover all state event msg.
 */
typedef struct {
	uint16_t conn_handle;                   /*!< Connection handle */
	bool is_success;                        /*!< GATTC discover all is success or not */
	bool load_from_storage;                 /*!< GATTC discover result load from storage or not */
} rtk_bt_gattc_discover_all_state_ind_t;

/**
 * @typedef   rtk_bt_gatt_service_char_t
 * @brief     GATT service characteristic exsit flag.
 */
typedef enum {
	RTK_BT_GATT_SVC_DATABASE_HASH_FLAG = 0x01,              /*!< Database hash characteristic bit. */
	RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG = 0x02,  /*!< Client supported features characteristic bit. */
	RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG = 0x04,  /*!< Server supported features characteristic bit. */
	RTK_BT_GATT_SVC_SERVICE_CHANGED_FLAG = 0x08,            /*!< Service changed characteristic bit. */
} rtk_bt_gatt_service_char_t;

/**
 * @def       RTK_BT_GATT_SVC_DATABASE_HASH_LEN
 * @brief     GATT Service Database Hash length.
 */
#define RTK_BT_GATT_SVC_DATABASE_HASH_LEN   16

/**
 * @def       RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN
 * @brief     GATT Service Client supported features maximum length.
 */
#define RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN 1

/**
 * @def       RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN
 * @brief     GATT Service Server supported features maximum length.
 */
#define RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN 1

/**
 * @struct    rtk_bt_gattc_gatt_service_info_ind_t
 * @brief     Bluetooth GATTC event discover all state event msg.
 */
typedef struct {
	uint16_t conn_handle;                       /*!< Connection handle */
	bool is_found;                              /*!< GATT service info is found or not */
	bool load_from_storage;                     /*!< GATT service info load from storage or not */
	uint8_t char_flag;                          /*!< Characteristic exist bit field.  @ref rtk_bt_gatt_service_char_t */
	uint8_t database_hash[RTK_BT_GATT_SVC_DATABASE_HASH_LEN]; /*!< The value can be used when the bit of RTK_BT_GATT_SVC_DATABASE_HASH_LEN in char_flag is set.*/
	uint8_t client_features_len;                /*!< The value can be used when the bit of RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG in char_flag is set.*/
	uint8_t client_features[RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_MAX_LEN]; /*!< The value can be used when the bit of RTK_BT_GATT_SVC_CLIENT_SUPPORTED_FEATURES_FLAG in char_flag is set.*/
	uint8_t server_features_len;                /*!< The value can be used when the bit of RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG in char_flag is set.*/
	uint8_t server_features[RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_MAX_LEN]; /*!< @ref rtk_bt_gatts_server_supported_features_t. The value can be used when the bit of RTK_BT_GATT_SVC_SERVER_SUPPORTED_FEATURES_FLAG in char_flag is set.*/
} rtk_bt_gattc_gatt_service_info_ind_t;

/**
 * @typedef   rtk_bt_gattc_char_type_t
 * @brief     Define characteristic type
 */
typedef enum {
	RTK_BT_GATT_CHAR_UNKNOWN = 0x00,
	RTK_BT_GATT_CHAR_VALUE = 0x01,      /**< Characteristic value*/
	RTK_BT_GATT_CHAR_CCCD = 0x02,       /**< Characteristic cccd*/
	RTK_BT_GATT_CHAR_DESCRIPTOR = 0x03, /**< Characteristic descriptor*/
} rtk_bt_gattc_char_type_t;

/**
 * @struct    rtk_bt_gattc_read_ind_t
 * @brief     Bluetooth GATTC event read result event msg.
 */
typedef struct {
	uint16_t profile_id;                /*!< Profile ID */
	uint16_t conn_handle;               /*!< Connection handle */
	rtk_bt_gattc_char_type_t char_type; /*!< Characteristic type */
	uint8_t srv_instance_id;            /*!< ID of the service in which the characteristic is contained */
	rtk_bt_gattc_uuid_t char_uuid;      /*!< Characteristic UUID */
	uint16_t handle;                    /*!< Characteristic handle */
	uint16_t len;                       /*!< Characteristic value length */
	uint8_t *value;                     /*!< Characteristic value */
	rtk_bt_status_t status;             /*!< Characteristic read result */
	uint16_t err_code;                  /*!< Characteristic read error code, 0: success, others: failure cause */
} rtk_bt_gattc_read_ind_t;

/**
 * @struct    rtk_bt_gattc_write_ind_t
 * @brief     Bluetooth GATTC event write result event msg.
 */
typedef struct {
	uint16_t profile_id;                /*!< Profile ID */
	uint16_t conn_handle;               /*!< Connection handle */
	rtk_bt_gattc_char_type_t char_type; /*!< Characteristic type */
	uint8_t  srv_instance_id;           /*!< ID of the service in which the characteristic is contained */
	rtk_bt_gattc_uuid_t char_uuid;      /*!< Characteristic UUID */
	rtk_bt_gattc_write_type_t type;     /*!< Characteristic write type */
	uint16_t handle;                    /*!< Characteristic handle */
	rtk_bt_status_t status;             /*!< Characteristic write result */
	uint16_t err_code;                  /*!< Characteristic write error code, 0: success, others: failure cause */
} rtk_bt_gattc_write_ind_t;

/**
 * @struct    rtk_bt_gattc_cccd_value_ind_t
 * @brief     Bluetooth GATTC client receive remote server's notify or indicate,
 *            transfer notify/indicate data to event callback by this event msg.
 */
typedef struct {
	uint16_t profile_id;                /*!< Profile ID */
	uint16_t conn_handle;               /*!< Connection handle */
	uint16_t cid;                       /*!< L2CAP channel ID to indicate which channel the value is received from */
	uint8_t  srv_instance_id;           /*!< ID of the service in which the characteristic is contained */
	rtk_bt_gattc_uuid_t char_uuid;      /*!< Characteristic UUID */
	uint16_t value_handle;              /*!< Characteristic handle */
	uint16_t len;                       /*!< Characteristic value length */
	uint8_t *value;                     /*!< Characteristic value */
} rtk_bt_gattc_cccd_value_ind_t;

/**
 * @struct    rtk_bt_gattc_cccd_update_ind_t
 * @brief     Bluetooth GATT client write cccd complete event msg.
 */
typedef struct {
	uint16_t profile_id;                /*!< Profile ID */
	uint16_t conn_handle;               /*!< Connection handle */
	bool srv_cfg;                       /*!< If cccd config is for whole service or a characteristic */
	uint8_t srv_instance_id;            /*!< ID of the service in which the characteristic is contained */
	uint16_t cccd_data;                 /*!< Attribute value of CCCD */
	rtk_bt_gattc_uuid_t uuid;           /*!< Characteristic UUID */
	rtk_bt_status_t status;             /*!< GATTC cccd update result */
	uint16_t err_code;                  /*!< GATTC cccd update error code, 0: success, others: failure cause */
	bool bnotify;                       /*!< If notification status changed */
	bool bindicate;                     /*!< If indication status changed */
} rtk_bt_gattc_cccd_update_ind_t;

/**
 * @typedef   rtk_bt_gap_chann_type_t
 * @brief     Bluetooth GAP channel type.
 */
typedef enum {
	RTK_BT_GAP_CHANN_TYPE_LE_ATT,       /*!< ATT bearer on LE transport. */
	RTK_BT_GAP_CHANN_TYPE_LE_ECFC,      /*!< EATT bearer on LE transport. */
	RTK_BT_GAP_CHANN_TYPE_BREDR_ATT,    /*!< ATT bearer on BR/EDR transport. */
	RTK_BT_GAP_CHANN_TYPE_BREDR_ECFC,   /*!< EATT bearer on BR/EDR transport. */
} rtk_bt_gap_chann_type_t;

/**
 * @brief Client handle disconect result
 *        The event data for GATT_CLIENT_EVENT_CONN_DEL.
 */
typedef struct {
	uint16_t profile_id;                /*!< Profile ID */
	uint16_t cid;                       /*!< L2CAP channel id */
	rtk_bt_gap_chann_type_t chann_type; /*!< L2CAP channel type */
} rtk_bt_gattc_conn_del_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_gattc_ble_mgr BT GATT Client ble_mgr APIs
 * @brief     BT GATT client related ble_mgr function APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief      Register GATT client profile.
 * @param[in]  p_reg_param: Pointer to GATT client profile parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_register_profile(uint16_t profile_id, rtk_bt_gattc_uuid_t srv_uuid);

/**
 * @brief      Discover all attributes.
 * @param[in]  conn_handle: The handle of connection.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_discover_all(uint16_t conn_handle);

/**
 * @brief      GATT client Discovery operations, will cause event @ref RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND
 * @param p_find_param: Pointer to GATT client find parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_find(rtk_bt_gattc_find_param_t *p_find_param);

/**
 * @brief      GATT client Read operations, will cause event @ref RTK_BT_GATTC_EVT_READ_RESULT_IND
 * @param p_read_param: Pointer to GATT client Read parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_read(rtk_bt_gattc_read_param_t *p_read_param);

/**
 * @brief      GATT client write operations, will cause event @ref RTK_BT_GATTC_EVT_WRITE_RESULT_IND
 * @param p_write_param: Pointer to GATT client write parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_write(rtk_bt_gattc_write_param_t *p_write_param);

/**
 * @brief      GATT client enable notify or indicate, will cause event @ref RTK_BT_GATTC_EVT_CCCD_ENABLE_IND
 * @param p_update_cccd_param: Pointer to GATT client update cccd parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_enable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param);

/**
 * @brief      GATT client disable notify or indicate, will cause event @ref RTK_BT_GATTC_EVT_CCCD_DISABLE_IND
 * @param p_update_cccd_param: Pointer to GATT client update cccd parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_disable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param);
/**
 * @}
 */
#else /******************************* #if RTK_BLE_MGR_LIB *****************************/

#define RTK_BT_GATTC_ATTACH_ID(_profile_id,_conn_handle)        \
        (((_profile_id)<<16) | (_conn_handle))                  \

#define RTK_BT_GET_PROFILE_ID(_val)                             \
        (((_val) & 0xFFFF0000) >> 16)                           \

#define RTK_BT_GET_CONN_HANDLE(_val)                            \
        ((_val)&0x0000FFFF)                                     \

/**
 * @typedef   rtk_bt_gattc_discover_type_t
 * @brief     Bluetooth GATT client discover type.
 */
typedef enum {
	RTK_BT_GATT_DISCOVER_PRIMARY_ALL,           /*!< Discover all primary services */
	RTK_BT_GATT_DISCOVER_PRIMARY_BY_UUID,       /*!< Discover primary services By service UUID */
	RTK_BT_GATT_DISCOVER_INCLUDE,               /*!< Find included services */
	RTK_BT_GATT_DISCOVER_CHARACTERISTIC_ALL,    /*!< Discover all characteristic of a service */
	RTK_BT_GATT_DISCOVER_CHARACTERISTIC_BY_UUID,/*!< Discover characteristic by UUID */
	RTK_BT_GATT_DISCOVER_DESCRIPTORS_ALL,       /*!< Discover all characteristic descriptors */
} rtk_bt_gattc_discover_type_t;

/**
 * @struct    rtk_bt_gattc_discover_param_t
 * @brief     Bluetooth GATT client discover paramter for different Operation.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_discover_type_t type;      /*!< Discover type */

	union {
		struct disc_primary_all_param {
			uint8_t dummy;                  /*!< Useless, just for integrity */
		} disc_primary_all;                 /*!< Discover All Primary Services */

		struct disc_primary_by_uuid_param {
			uint8_t uuid[16];               /*!< Discover UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Discover UUID type @rtk_bt_uuid_type_t */
		} disc_primary_by_uuid;             /*!< Discover primary service by service UUID */

		struct disc_inc_param {
			uint16_t start_handle;          /*!< Included service start handle */
			uint16_t end_handle;            /*!< Included service end handle */
		} disc_inc;                         /*!< Find included services */

		struct disc_char_all_param {
			uint16_t start_handle;          /*!< Included service start handle */
			uint16_t end_handle;            /*!< Included service end handle */
		} disc_char_all;                    /*!< Discover all characteristics of a service */

		struct disc_char_by_uuid_param {
			uint16_t start_handle;          /*!< Included service start handle */
			uint16_t end_handle;            /*!< Included service end handle */
			uint8_t uuid[16];               /*!< Discover UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Discover UUID type @rtk_bt_uuid_type_t */
		} disc_char_by_uuid;                /*!< Discover characteristics by UUID */

		struct disc_descriptor_param {
			uint16_t start_handle;          /*!< Included service start handle */
			uint16_t end_handle;            /*!< Included service end handle */
		} disc_descriptor;                  /*!< Discover all characteristic descriptors */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_discover_param_t */
} rtk_bt_gattc_discover_param_t;

/**
 * @struct    rtk_bt_gattc_discover_ind_t
 * @brief     Bluetooth GATTC event discover result event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_discover_type_t type;      /*!< Discover type */
	rtk_bt_status_t status;                 /*!< GATTC discover status,
                                                @note: One discover request may raise multiple discover events because many attribute entries may be
                                                reported as results when discover complete. Each event reports one attribute entry.
                                                if status is @ref RTK_BT_STATUS_CONTINUE, means one discover attribute entry is reported with this msg,
                                                    discover data of union below is not empty, and still have another msg follow this.
                                                if status is @ref RTK_BT_STATUS_DONE, means discover is finished, and discover data of union below is empty.
                                                if status is @ref RTK_BT_STATUS_FAIL, means discover is failed, inquire err_code for the cause of failure */
	uint16_t err_code;                      /*!< GATTC discover err_code, 0: success, others: failure cause */

	union {
		struct disc_primary_all_per_param {
			uint16_t start_handle;          /*!< Primary service start handle */
			uint16_t end_handle;            /*!< Primary service end handle */
			uint8_t uuid[16];               /*!< Primary service UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Primary service UUID type @rtk_bt_uuid_type_t */
		} disc_primary_all_per;             /*!< Discover primary all services result (per attribute) */

		struct disc_primary_by_uuid_per_param {
			uint16_t start_handle;          /*!< Primary service start handle */
			uint16_t end_handle;            /*!< Primary service end handle */
		} disc_primary_by_uuid_per;         /*!< Discover primary services by service UUID result (per attribute) */

		struct disc_inc_per_param {
			uint16_t handle;                /*!< Include service declaration handle */
			uint16_t start_handle;          /*!< Include service start handle */
			uint16_t end_handle;            /*!< Include service end handle */
			uint8_t uuid[16];               /*!< Include service UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Include service UUID type @rtk_bt_uuid_type_t */
		} disc_inc_per;                     /*!< Find included services result (per service) */

		struct disc_char_all_per_param {
			uint16_t handle;                /*!< Characteristic declaration handle */
			uint8_t properties;             /*!< Characteristic properties */
			uint16_t value_handle;          /*!< Characteristic value attribute handle */
			uint8_t uuid[16];               /*!< Characteristic UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Characteristic UUID type @rtk_bt_uuid_type_t */
		} disc_char_all_per;                /*!< Discover all characteristic of a service result (per attribute) */

		struct disc_char_by_uuid_per_param {
			uint16_t handle;                /*!< Characteristic declaration handle */
			uint8_t properties;             /*!< Characteristic properties */
			uint16_t value_handle;          /*!< Characteristic value attribute handle */
			uint8_t uuid[16];               /*!< Characteristic UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Characteristic UUID type @rtk_bt_uuid_type_t */
		} disc_char_by_uuid_per;            /*!< Discover characteristic by UUID result (per attribute) */

		struct disc_descriptor_per_param {
			uint16_t handle;                /*!< Characteristic descriptor attribute handle */
			uint8_t uuid[16];               /*!< Characteristic descriptor UUID value, in little-endian format */
			uint8_t uuid_type;              /*!< Characteristic descriptor UUID type @rtk_bt_uuid_type_t */
		} disc_descriptor_per;              /*!< Discover all characteristic descriptors result (per attribute) */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_discover_ind_t */
} rtk_bt_gattc_discover_ind_t;

/**
 * @typedef   rtk_bt_gattc_read_type_t
 * @brief     Bluetooth GATT client read type.
 */
typedef enum {
	RTK_BT_GATT_CHAR_READ_BY_HANDLE,            /*!< Read characteristic value */
	RTK_BT_GATT_CHAR_READ_BY_UUID,              /*!< Read using characteristic UUID */
	RTK_BT_GATT_CHAR_READ_MULTIPLE,             /*!< Read multiple characteristic values */
	RTK_BT_GATT_CHAR_READ_MULTIPLE_VARIABLE,    /*!< Read multiple variable characteristic values */
} rtk_bt_gattc_read_type_t;


/**
 * @struct    rtk_bt_gattc_read_param_t
 * @brief     Bluetooth GATT client read paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_read_type_t type;          /*!< Characteristic value read type */
	union {
		struct by_handle_param {
			uint16_t handle;                /*!< Characteristic value handle */
		} by_handle;                        /*!< Read Characteristic Value */

		struct by_uuid_param {
			uint16_t start_handle;          /*!< Start of read handle range */
			uint16_t end_handle;            /*!< End of read handle range */
			uint8_t uuid[16];               /*!< 2 or 16 octet UUID */
			uint8_t uuid_type;              /*!< Read UUID type @rtk_bt_uuid_type_t */
		} by_uuid;                          /*!< Read using characteristic UUID */

		struct multiple_param {
			uint32_t handle_count;          /*!< The number of multiple handles are used */
			uint16_t *handles;              /*!< Characteristic value handles array */
		} multiple;                         /*!< Read multiple characteristic values */

		struct multiple_variable_param {
			uint32_t handle_count;          /*!< The number of multiple handles are used */
			uint16_t *handles;              /*!< Characteristic value handles array */
		} multiple_variable;                /*!< Read multiple variable length characteristic values */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_read_param_t */

} rtk_bt_gattc_read_param_t;

/**
 * @struct    rtk_bt_gattc_read_ind_t
 * @brief     Bluetooth GATTC event read result event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_read_type_t type;          /*!< Characteristic value read type */
	rtk_bt_status_t status;                 /*!< GATTC read status,
                                                @note: One read request may raise multiple read events because many attribute entries may be
                                                reported as results when read complete. Each event reports one attribute entry.
                                                if status is @ref RTK_BT_STATUS_CONTINUE, means one read attribute entry is reported with this msg,
                                                    read data of union below is not empty, and still have another msg follow this.
                                                if status is @ref RTK_BT_STATUS_DONE, means read is finished, and read data of union below is empty.
                                                if status is @ref RTK_BT_STATUS_FAIL, means read is failed, inquire err_code for the cause of failure */
	uint16_t err_code;                      /*!< GATTC read err_code, 0: success, others: failure cause */

	union {
		struct by_handle_res_param {
			uint16_t handle;                /*!< Characteristic value handle */
			uint16_t len;                   /*!< The length of characteristic value */
			uint8_t *value;                 /*!< Characteristic value */
		} by_handle;                        /*!< Read characteristic value result */

		struct by_uuid_per_param {
			uint16_t handle;                /*!< Characteristic value handle */
			uint16_t len;                   /*!< The length of characteristic value */
			uint8_t *value;                 /*!< Characteristic value */
		} by_uuid_per;                      /*!< Read using characteristic UUID result (per attribute) */

		struct multiple_per_param {
			uint16_t dummy;                 /*!< Useless, just for padding, make the 4 union element the same memory structure */
			uint16_t len;                   /*!< The length of characteristic value */
			uint8_t *value;                 /*!< Characteristic value */
		} multiple_per;                     /*!< Read multiple characteristic values (per attribute) */

		struct multiple_variable_per_param {
			uint16_t dummy;                 /*!< Useless, just for padding, make the 4 union element the same memory structure */
			uint16_t len;                   /*!< The length of characteristic value */
			uint8_t *value;                 /*!< Characteristic value */
		} multiple_variable_per;            /*!< Read multiple variable length characteristic values (per attribute) */
	};                                      /*!< Anonymous union member in rtk_bt_gattc_read_ind_t */
} rtk_bt_gattc_read_ind_t;

/**
 * @typedef   rtk_bt_gattc_write_type_t
 * @brief     Bluetooth GATT client write type.
 */
typedef enum {
	RTK_BT_GATT_CHAR_WRITE_REQ = 0x00,          /*!< Write characteristic value request */
	RTK_BT_GATT_CHAR_WRITE_NO_RSP,              /*!< Write characteristic value without response */
	RTK_BT_GATT_CHAR_WRITE_NO_RSP_SIGNED,       /*!< Write characteristic value without response and with signed data */
} rtk_bt_gattc_write_type_t;

/**
 * @struct    rtk_bt_gattc_write_param_t
 * @brief     Bluetooth GATT client write paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_write_type_t type;         /*!< Characteristic value write type */
	uint16_t handle;                        /*!< Characteristic value handle */
	uint16_t length;                        /*!< Length of the data */
	void *data;                             /*!< Data to be written */
} rtk_bt_gattc_write_param_t;

/**
 * @struct    rtk_bt_gattc_write_ind_t
 * @brief     Bluetooth GATTC event write result event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_gattc_write_type_t type;         /*!< Characteristic value write type */
	rtk_bt_status_t status;                 /*!< GATTC write status,
                                                if status is @ref RTK_BT_STATUS_DONE, means write is success,
                                                if status is @ref RTK_BT_STATUS_FAIL, means write is failed, inquire err_code for the cause of failure */
	uint16_t err_code;                      /*!< GATTC write err_code, 0: success, others: failure cause */
	uint16_t handle;                        /*!< Characteristic value handle */
} rtk_bt_gattc_write_ind_t;

/**
 * @struct    rtk_bt_gattc_update_cccd_param_t
 * @brief     Bluetooth GATT client write cccd paramter.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	uint16_t char_val_handle;               /*!< Characteristic value handle */
	uint16_t cccd_handle;                   /*!< Client characteristic configuration descriptor(CCCD) handle */
	bool bnotify;                           /*!< If notify bit need to write */
	bool bindicate;                         /*!< If indicate bit need to write */
} rtk_bt_gattc_update_cccd_param_t;

/**
 * @struct    rtk_bt_gattc_cccd_update_ind_t
 * @brief     Bluetooth GATT client write cccd complete event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t seq;                           /*!< Sequence number, for convinience, not mandatory */
	rtk_bt_status_t status;                 /*!< GATTC cccd update status,
                                                if status is @ref RTK_BT_STATUS_DONE, means write is success,
                                                if status is @ref RTK_BT_STATUS_FAIL, means write is failed, inquire err_code for the cause of failure */
	uint16_t err_code;                      /*!< GATTC write err_code, 0: success, others: failure cause */
	uint16_t cccd_handle;                   /*!< Client characteristic configuration descriptor(CCCD) handle */
	bool bnotify;                           /*!< If notify bit is writed */
	bool bindicate;                         /*!< If indicate bit is writed */
} rtk_bt_gattc_cccd_update_ind_t;

/**
 * @struct    rtk_bt_gattc_cccd_value_ind_t
 * @brief     Bluetooth GATTC client receive remote server's notify or indicate,
 *            transfer notify/indicate data to event callback by this event msg.
 */
typedef struct {
	uint16_t profile_id;                    /*!< Profile ID */
	uint16_t conn_handle;                   /*!< Connection handle */
	uint16_t value_handle;                  /*!< Characteristic value handle */
	uint16_t len;                           /*!< The length of characteristic value */
	uint8_t *value;                         /*!< characteristic value */
} rtk_bt_gattc_cccd_value_ind_t;

/********************************* Functions Declaration *******************************/
/**
 * @defgroup  bt_gattc_normal BT GATT Client Normal APIs
 * @brief     BT GATT client related function normal APIs
 * @ingroup   BT_APIs
 * @{
 */

/**
 * @brief      Register GATT client profile.
 * @param[in]  profile_id: The id of client profile.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_register_profile(uint16_t profile_id);

/**
 * @brief      Client initiate exchange MTU request, if exchange MTU success, will
 *             cause event @ref RTK_BT_GATTC_EVT_MTU_EXCHANGE
 * @param conn_handle: The handle of connection.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_exchange_mtu(uint16_t conn_handle);

/**
 * @brief      GATT client Discovery operations, will cause event @ref RTK_BT_GATTC_EVT_DISCOVER_RESULT_IND
 * @param p_dis_param: Pointer to GATT client Discovery parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_discover(rtk_bt_gattc_discover_param_t *p_dis_param);

/**
 * @brief      GATT client Read operations, will cause event @ref RTK_BT_GATTC_EVT_READ_RESULT_IND
 * @param p_read_param: Pointer to GATT client Read parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_read(rtk_bt_gattc_read_param_t *p_read_param);

/**
 * @brief      GATT client write operations, will cause event @ref RTK_BT_GATTC_EVT_WRITE_RESULT_IND
 * @param p_write_param: Pointer to GATT client write parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_write(rtk_bt_gattc_write_param_t *p_write_param);

/**
 * @brief      GATT client enable notify or indicate, will cause event @ref RTK_BT_GATTC_EVT_CCCD_ENABLE_IND
 * @param p_update_cccd_param: Pointer to GATT client update cccd parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_enable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param);

/**
 * @brief      GATT client disable notify or indicate, will cause event @ref RTK_BT_GATTC_EVT_CCCD_DISABLE_IND
 * @param p_update_cccd_param: Pointer to GATT client update cccd parameters.
 * @return
 *            - 0  : Succeed
 *            - Others: Error code
 */
uint16_t rtk_bt_gattc_disable_notify_or_indicate(
	rtk_bt_gattc_update_cccd_param_t *p_update_cccd_param);
/**
 * @}
 */
#endif /* #if RTK_BLE_MGR_LIB */

#ifdef __cplusplus
}
#endif

#endif /* __RTK_BT_GATTC_H__ */

