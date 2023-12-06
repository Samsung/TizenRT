
#ifndef __RTK_BT_MESH_DEF_H__
#define __RTK_BT_MESH_DEF_H__

#include <stdint.h>
#include <bt_api_config.h>

#ifndef _PACKED4_
#define _PACKED4_            __attribute__ ((packed))
#endif

#define ACCESS_OPCODE_SIZE(opcode)              ((opcode) >= 0xc00000 ? 3 : ((opcode) >= 0x8000 ? 2 : 1))

#define RTK_BT_STACK_MESH_ERROR_FLAG    0x0100
#define RTK_BT_MESH_EVT_MEM_OP_MAX_SIZE    0xffffffff

#define COMPANY_ID        0x005D
#define PRODUCT_ID        0x0000
#define VERSION_ID        0x0000

// Mesh default configuration paramters
#define BT_MESH_CONFIG_NET_TRANS_COUNTS          6
#define BT_MESH_CONFIG_NET_TRANS_STEPS           0
#define BT_MESH_CONFIG_RELAY_IS_ENABL            1
#define BT_MESH_CONFIG_RELAY_RETRANS_COUNTS      2
#define BT_MESH_CONFIG_RELAY_RETRANS_STEPS       0
#define BT_MESH_CONFIG_TRANS_RETRANS_COUNTS      4
#define BT_MESH_CONFIG_MSG_TTL                   5

// Mesh models enable configuration
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
#define BT_MESH_ENABLE_REMOTE_PROVISIONING_CLIENT_MODEL                1
#define BT_MESH_ENABLE_GENERIC_ON_OFF_CLIENT_MODEL                     1
#define BT_MESH_ENABLE_GENERIC_LEVEL_CLIENT_MODEL                      1
#define BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSITION_TIME_CLIENT_MODEL    1
#define BT_MESH_ENABLE_GENERIC_POWER_ONOFF_CLIENT_MODEL                1
#define BT_MESH_ENABLE_GENERIC_POWER_LEVEL_CLIENT_MODEL                1
#define BT_MESH_ENABLE_GENERIC_BATTERY_CLIENT_MODEL                    1
#define BT_MESH_ENABLE_GENERIC_LOCATION_CLIENT_MODEL                   1
#define BT_MESH_ENABLE_GENERIC_PROPERTY_CLIENT_MODEL                   1
#define BT_MESH_ENABLE_SENSOR_CLIENT_MODEL                             1
#define BT_MESH_ENABLE_TIME_CLIENT_MODEL                               1
#define BT_MESH_ENABLE_SCENE_CLIENT_MODEL                              1
#define BT_MESH_ENABLE_SCHEDULER_CLIENT_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_LIGHTNESS_CLIENT_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_CTL_CLIENT_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_HSL_CLIENT_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_XYL_CLIENT_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_LC_CLIENT_MODEL                           1
#endif

#if defined(RTK_BLE_MESH_DEVICE_SUPPORT) && RTK_BLE_MESH_DEVICE_SUPPORT
#define BT_MESH_ENABLE_GENERIC_ON_OFF_SERVER_MODEL                     1
#define BT_MESH_ENABLE_GENERIC_LEVEL_SERVER_MODEL                      1
#define BT_MESH_ENABLE_GENERIC_DEFAULT_TRANSTION_TIME_SERVER_MODEL     1
#define BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SERVER_MODEL               1
#define BT_MESH_ENABLE_GENERIC_POWER_ON_OFF_SETUP_SERVER_MODEL         1
#define BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SERVER_MODEL                1
#define BT_MESH_ENABLE_GENERIC_POWER_LEVEL_SETUP_SERVER_MODEL          1
#define BT_MESH_ENABLE_GENERIC_BATTERY_SERVER_MODEL                    1
#define BT_MESH_ENABLE_GENERIC_LOCATION_SERVER_MODEL                   1
#define BT_MESH_ENABLE_GENERIC_LOCATION_SETUP_SERVER_MODEL             1
#define BT_MESH_ENABLE_GENERIC_ADMIN_PROPERTY_SERVER_MODEL             1
#define BT_MESH_ENABLE_GENERIC_MANUFACTURER_PROPERTY_SERVER_MODEL      1
#define BT_MESH_ENABLE_GENERIC_USER_PROPERTY_SERVER_MODEL              1
#define BT_MESH_ENABLE_GENERIC_CLIENT_PROPERTY_SERVER_MODEL            1
#define BT_MESH_ENABLE_SENSOR_SERVER_MODEL                             1
#define BT_MESH_ENABLE_SENSOR_SETUP_SERVER_MODEL                       1
#define BT_MESH_ENABLE_TIME_SERVER_MODEL                               1
#define BT_MESH_ENABLE_TIME_SETUP_SERVER_MODEL                         1
#define BT_MESH_ENABLE_SCENE_SERVER_MODEL                              1
#define BT_MESH_ENABLE_SCENE_SETUP_SERVER_MODEL                        1
#define BT_MESH_ENABLE_SCHEDULER_SERVER_MODEL                          1
#define BT_MESH_ENABLE_SCHEDULER_SETUP_SERVER_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_LIGHTNESS_SERVER_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_LIGHTNESS_SETUP_SERVER_MODEL              1
#define BT_MESH_ENABLE_LIGHT_CTL_SERVER_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_CTL_SETUP_SERVER_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_CTL_TEMPERATURE_SERVER_MODEL              1
#define BT_MESH_ENABLE_LIGHT_HSL_SERVER_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_HSL_SETUP_SERVER_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_HSL_HUE_SERVER_MODEL                      1
#define BT_MESH_ENABLE_LIGHT_HSL_SATURATION_SERVER_MODEL               1
#define BT_MESH_ENABLE_LIGHT_XYL_SERVER_MODEL                          1
#define BT_MESH_ENABLE_LIGHT_XYL_SETUP_SERVER_MODEL                    1
#define BT_MESH_ENABLE_LIGHT_LC_SERVER_MODEL                           1
#define BT_MESH_ENABLE_LIGHT_LC_SETUP_SERVER_MODEL                     1
#endif

#define BT_MESH_ENABLE_DATATRANS_MODEL                                 1


// Mesh provisioning capability releate setting for device
#define PROV_SUPPORT_STATIC_OOB           RTK_BT_MESH_PROV_CAP_NOT_SUPPORT_STATIC_OOB    // @ref rtk_bt_mesh_stack_prov_cap_static_oob_t
#define PROV_SUPPORT_OUTPUT_OOB_ACTION    RTK_BT_MESH_PROV_CAP_OUTPUT_OOB_ACTION_NOT_ENABLE    // @ref rtk_bt_mesh_stack_prov_cap_output_oob_action_t
#define PROV_SUPPORT_OUTPUT_OOB_SIZE      0    // The bytes store random number for mesh stack
#define PROV_SUPPORT_INPUT_OOB_ACTION     RTK_BT_MESH_PROV_CAP_INPUT_OOB_ACTION_NOT_ENABLE    // @ref rtk_bt_mesh_stack_prov_cap_input_oob_action_t
#define PROV_SUPPORT_INPUT_OOB_SIZE       0    // The bytes store random number for mesh stack

#define STATIC_OOB_VALUE_FOR_AUTHENTICATION    {0x1,0x2,0x3}

/**
 * @typedef   rtk_bt_mesh_api_result
 * @brief     BLE MESH api result definition.
 */
typedef enum {
	/* The access msg is successfully processed by the stack! */
	RTK_BT_MESH_MSG_SEND_CAUSE_SUCCESS,
	/* The access msg shall be formatted with a valid access opcode as defined in the spec */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_ACCESS_OPCODE,
	/*
	 * The model send the access msg with an invalid element index.
	 * It maybe due to the model is unregistered.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_ELEMENT,
	/*
	 * The model send the access msg with an invalid model index.
	 * It maybe due to the model is unregistered.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_MODEL,
	/* Unprovisioned device can't send access msg! */
	RTK_BT_MESH_MSG_SEND_CAUSE_NODE_UNPROVISIONED,
	/*
	 * The app may send an access msg without a model, so it need appoint the src itself.
	 * In this case, the stack will check the validity of the src address.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_SRC,
	/*
	 * The dst address is assigned by the app.
	 * The stack will check the validity of the dst address.
	 * -# The dst addr can't be the unassigned address.
	 * -# When the message is encrypted by the device key, the device key shall exist.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_DST,
	/*
	 * Depending on the msg size, the access msg is send by unsegmented or segmented.
	 * The maximum size of the access pdu is @ref ACCESS_PAYLOAD_MAX_SIZE.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_PAYLOAD_SIZE_EXCEED,
	/*
	 * If the SZMIC bit is set to 0, the TransMIC is a 32-bit value.
	 * If the SZMIC bit is set to 1, the TransMIC is a 64-bit value.
	 * When the SZMIC bit is set to 1 then the maximum payload size of
	 * segmented access pdu has to minus 4.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_PAYLOAD_SIZE_EXCEED1, //!< 8
	/* The virtual address is invalid. */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_VIR_ADDR,
	/* the index exceeds the upper limit @ref mesh_node.app_key_num */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_APP_KEY_INDEX,
	/* The key hasn't been added */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_APP_KEY_STATE,
	/* The key hasn't been bound to the model when sending the msg with model. */
	RTK_BT_MESH_MSG_SEND_CAUSE_APP_KEY_NOT_BOUND_TO_MODEL,
	/* the index exceeds the upper limit @ref mesh_node.net_key_num */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_NET_KEY_INDEX,
	/* The key hasn't been added. */
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_NET_KEY_STATE,
	/*
	 * The mesh msg is send via the adverting channel, which takes some resource and time.
	 * When the resource is exhausted currently, the msg can't be send.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_NO_BUFFER_AVAILABLE,
	/* The heap space is exhausted which need be allocated in some cases. */
	RTK_BT_MESH_MSG_SEND_CAUSE_NO_MEMORY, //!< 16
	/*
	 * The segmented msg need to be buffered at the transport layer to retransmit.
	 * The buffer size at the transport layer is limited so that a new segmented msg
	 * can't be send when the buffer is full.
	 */
	RTK_BT_MESH_MSG_SEND_CAUSE_TRANS_TX_BUSY,
	RTK_BT_MESH_MSG_SEND_CAUSE_INVALID_ACCESS_PARAMETER,

	RTK_BT_MESH_MSG_SEND_CAUSE_FAIL,
} rtk_bt_mesh_msg_send_result;

/**
 * @typedef   rtk_bt_mesh_app_conf_t
 * @brief     BLE MESH app conf definition.
 */
typedef struct {
	uint8_t   bt_mesh_role;            /*!< Default bt mesh profile role */
	uint8_t   bt_mesh_uuid[16];        /*!< Default bt mesh device UUID */
	uint32_t  bt_mesh_flash_size;      /*!< FTL size for mesh */
	uint8_t   bt_mesh_rpl_num;         /*!< Message replay protection list num */
	uint8_t   trans_retrans_count;     /*!< Transport Transmit Count */
	uint8_t   net_trans_count:3;         /*!< Network Transmit Count */
	uint8_t   net_trans_interval_step:5; /*!< Network Transmit Interval Step */
	uint8_t   relay_retrans_count:3;             /*!< Relay Retransmit Count */
	uint8_t   relay_retrans_interval_step:5;     /*!< Relay Retransmit Interval step */
	uint8_t   ttl;                     /*!< Default TTL */
#if defined(RTK_BLE_MESH_PROVISIONER_SUPPORT) && RTK_BLE_MESH_PROVISIONER_SUPPORT
	uint8_t   bt_mesh_dev_key_num;      /*!< Device key num for provisioner */
#endif
} rtk_bt_mesh_app_conf_t;

#define MESH_LE_EXTRN2WORD(p) (((*(p)) & 0xff) + ((*((p)+1)) << 8))

#define MESH_LE_EXTRN2DWORD(p) (((unsigned long)(*((p)+0)) & 0xff) + ((unsigned long)(*((p)+1)) << 8) \
                           + ((unsigned long)(*((p)+2)) << 16)  + ((unsigned long)(*((p)+3)) << 24))

/**
 * @typedef GENERIC_TYPES_TYPES Generic Types
 * @brief Error code for models
 */
typedef enum {
	MESH_GENERIC_STAT_SUCCESS,
	MESH_GENERIC_STAT_CANNOT_SET_RANGE_MIN,
	MESH_GENERIC_STAT_CANNOT_SET_RANGE_MAX
} rtk_bt_mesh_generic_stat_t;

/**
 * @typedef rtk_bt_mesh_msg_subtype
 * @brief Subtype of T_IO_MSG for IO_MSG_TYPE_LE_MESH
 */
typedef enum {
	RTK_BT_MESH_IO_MSG_SUBTYPE_ADV,
} rtk_bt_mesh_msg_subtype;

/**
 * @defgroup  ble_mesh_api_group BT LE Mesh APIs
 * @brief     List all BT LE Mesh related APIs
 * @ingroup   BT_APIs
 */

#endif
