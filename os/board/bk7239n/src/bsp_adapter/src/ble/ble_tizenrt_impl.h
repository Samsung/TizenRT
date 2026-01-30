#ifndef _BLE_TIZENRT_IMPL_H_
#define _BLE_TIZENRT_IMPL_H_
#include <tinyara/net/if/ble.h>
#include "components/bluetooth/bk_ble_types.h"
#include "components/bluetooth/bk_ble.h"
#include "components/bluetooth/bk_dm_bluetooth.h"
#include <os/os.h>
#include "ble_tizenrt_gap.h"

#define BT_BONDED_DEVICES_KEY "bluetooth_bond_info"
#define BT_WHITELIST_KEY "bluetooth_whitelist_info"

#define BT_LINKKEY_MAX_SAVE_COUNT   18
#define BT_WHITELIST_MAX_SAVE_COUNT 32

/**
 * @brief BLE linked list node structure
 *
 * Used to implement linked list storage for BLE-related data
 */
struct ble_list_node_t {
    struct ble_list_node_t *next;    /**< Pointer to the next node in the list */
    void *data;                      /**< Pointer to data stored in the node */
};

/**
 * @brief BLE linked list node type definition
 */
typedef struct ble_list_node_t ble_list_node_t;

/**
 * @brief BLE linked list structure
 *
 * Maintains head/tail pointers and length information for BLE linked lists
 */
typedef struct ble_list_t {
    ble_list_node_t *head;   /**< Linked list head node pointer */
    ble_list_node_t *tail;   /**< Linked list tail node pointer */
    uint16_t length;         /**< Number of nodes in the linked list */
} ble_list_t;

/**
 * @brief BLE attribute client configuration descriptor structure
 *
 * Stores client configuration descriptor information for GATT characteristics
 */
typedef struct
{
    uint8_t prf_task_id;     /**< Profile task ID */
    uint8_t att_idx;         /**< Descriptor handle */
    uint16_t cccbits;        /**< Client configuration bits (notification/indication) */
} bk_att_cccd_t;

/**
 * @brief BLE connection role enumeration
 *
 * Defines the role types for BLE devices in a connection
 */
enum
{
    LINK_ROLE_UNDEFINED,    /**< Undefined role */
    LINK_ROLE_MASTER,       /**< Master device role */
    LINK_ROLE_SLAVE         /**< Slave device role */
};


/**
 * @brief BLE command type enumeration
 *
 * Defines all command types supported by the BLE adapter
 */
typedef enum {
    CMD_BLE_CREATE_ADV,       /**< Create advertisement */
    CMD_BLE_START_ADV,        /**< Start advertisement */
    CMD_BLE_STOP_ADV,         /**< Stop advertisement */
    CMD_BLE_DEL_ADV,          /**< Delete advertisement */
    CMD_BLE_START_SCAN,       /**< Start scanning */
    CMD_BLE_STOP_SCAN,        /**< Stop scanning */
    CMD_BLE_CONN_DIS_CONN,    /**< Connect/disconnect */
    CMD_BLE_INIT_CREATE,      /**< Initialize creation */
    CMD_BLE_DELETE_THREAD,    /**< Delete thread */
    CMD_BLE_BOND,             /**< Bonding operation */
    CMD_BLE_MAX               /**< Maximum command type value */
}ble_cmd_type;

/**
 * @brief BLE HAL state enumeration
 *
 * Defines various operating states of the BLE Hardware Abstraction Layer
 */
enum {
    BLE_HAL_INIT_STATE_INIT,                /**< Initial state */
    BLE_HAL_INIT_STATE_STACK_READY,         /**< BLE protocol stack ready state */

    BLE_HAL_ADV_STATE_IDLE,                 /**< Advertisement idle state */
    BLE_HAL_ADV_STATE_CREATED,              /**< Advertisement created state */
    BLE_HAL_ADV_STATE_ADVERTISING,          /**< Advertising in progress state */
    BLE_HAL_ADV_STATE_STOP,                 /**< Advertisement stopped state */
    BLE_HAL_ADV_STATE_DELETING,             /**< Advertisement deleting state */
    BLE_HAL_ADV_TO_IDLE_CAUSE_STOP,         /**< Entering idle state due to stop */
    BLE_HAL_ADV_TO_IDLE_CAUSE_CONN,         /**< Entering idle state due to connection */

    BLE_HAL_SCAN_STATE_IDLE,                /**< Scanning idle state */
    BLE_HAL_SCAN_STATE_START,               /**< Scanning start state */
    BLE_HAL_SCAN_STATE_SCANNING,            /**< Scanning in progress state */
    BLE_HAL_SCAN_STATE_STOP,                /**< Scanning stopped state */
    BLE_HAL_CONN_DEV_STATE_IDLE,            /**< Connection device idle state */
    BLE_HAL_CONN_DEV_STATE_INITIATING,      /**< Connection initialization state */
};

/**
 * @brief BLE advertisement element type enumeration
 *
 * Defines different types of advertisement elements
 */
enum
{
    ADV_ELEM_NORMAL,  /**< Normal advertisement element (multiple) */
    ADV_ELEM_STATIC,  /**< Static advertisement element */
    ADV_ELEM_ONESHOT  /**< One-shot advertisement element */
};

/**
 * @brief BLE event type enumeration
 *
 * Defines all event types supported by the BLE adapter
 */
typedef enum
{
    EVT_BLE_BONDED_MSG,                      /**< Bonded message event */
    EVT_BLE_CONNECTED_MSG,                   /**< Connected message event */
    EVT_BLE_SCAN_STATE_MSG,                  /**< Scan state message event */
    EVT_BLE_SCANNED_DEVICE_MSG,              /**< Scanned device message event */
    EVT_BLE_DISCONNECTED_MSG,                /**< Disconnected message event */
    EVT_BLE_NOTIFICATION_MSG,                /**< Notification message event */
    EVT_BLE_INDICATION_MSG,                  /**< Indication message event */
    EVT_BLE_READ_RESULT_MSG,                 /**< Read result message event */
    EVT_BLE_PAIRING_NUMBER_COMPARE_REQ_MSG,  /**< Pairing number comparison request message event */

    //server
    EVT_BLE_SERVER_CONNECTED,                /**< Server connected event */
    EVT_BLE_SERVER_DISCONNECT,               /**< Server disconnected event */
    EVT_BLE_SERVER_MTU_CHANGE,               /**< Server MTU change event */
    EVT_BLE_SERVER_PASSKEY,                  /**< Server passkey event */
    EVT_BLE_SERVER_ATTR_CB,                  /**< Server attribute callback event */
    EVT_BLE_SERVER_SET_BUFFER,               /**< Server set buffer event (special) */

    EVT_BLE_TASK_EXIT,                       /**< BLE task exit event */

    EVT_BLE_DO_CB_ONLY,                      /**< do cb only */

    EVT_BLE_APP_MSG_MAX                      /**< Maximum application message value */
} ble_evt_type;

/**
 * @brief BLE advertisement event type enumeration
 *
 * Defines types of BLE advertisement packets
 */
enum
{
    ADV_EVT_TYPE_UNDIRECTED = 0,    /**< Connectable undirected advertising */
    ADV_EVT_TYPE_DIRECTED   = 1,    /**< Connectable directed advertising */
    ADV_EVT_TYPE_SCANNABLE  = 2,    /**< Scannable undirected advertising */
    ADV_EVT_TYPE_NON_CONNECTABLE = 3,   /**< Non-connectable undirected advertising */
    ADV_EVT_TYPE_SCAN_RSP = 4,          /**< Scan response */
};

/**
 * @brief BLE advertisement message structure
 */
typedef struct ble_adv_msg {
    ble_recv_adv_t *r_ind;   /**< Pointer to received advertisement data */
} ble_adv_msg_t;

/**
 * @brief BLE disconnected message structure
 */
typedef struct ble_disconnected_msg
{
    uint16_t conn_id;   /**< Connection ID */
    uint16_t reason;    /**< Disconnection reason */
    uint8_t role;       /**< Connection role */
} ble_disconnected_msg_t;

/**
 * @brief BLE event message element structure
 *
 * This structure contains various event-specific data elements that can be used
 * with the BLE event message system. It uses a union to efficiently store
 * different types of event data depending on the event type.
 */
typedef struct
{
    union
    {
        /**
         * @brief Server connection event data
         *
         * Contains information about a new connection established to the server
         */
        struct
        {
            uint8_t conn_idx;            /**< Connection index */
            uint8_t peer_addr[6];        /**< Peer device address */
            uint8_t type;                /**< Connection type */
            uint8_t relate_adv_handle;   /**< Related advertisement handle */
        } server_connect_evt;

        /**
         * @brief Server disconnection event data
         *
         * Contains information about a disconnection from a client
         */
        struct
        {
            uint8_t conn_idx;            /**< Connection index */
            uint8_t reason;              /**< Disconnection reason */
        } server_disconnect_evt;

        /**
         * @brief MTU change event data
         *
         * Contains information about an MTU (Maximum Transmission Unit) change
         */
        struct
        {
            uint8_t conn_idx;            /**< Connection index */
            uint16_t mtu;                /**< New MTU value */
        } mtu_change_evt;

        /**
         * @brief Passkey event data
         *
         * Contains the passkey used for security pairing
         */
        struct
        {
            uint8_t conn_idx;            /**< Connection index */
            uint32_t passkey;            /**< Passkey value */
        } passkey_evt;

        /**
         * @brief Attribute callback event data
         *
         * Contains data for attribute-related callback events
         */
        struct
        {
            trble_server_cb_t cb;        /**< Server callback function */

            trble_attr_cb_type_e type;   /**< Attribute callback type */
            trble_conn_handle con_handle;/**< Connection handle */
            trble_attr_handle handle;    /**< Attribute handle */
            void *arg;                   /**< Additional argument */
            uint16_t result;             /**< Operation result */
            uint16_t pending;            /**< Pending operations flag */

            uint8_t *tmp_buffer;         /**< Temporary buffer pointer */
            uint32_t tmp_buffer_len;     /**< Temporary buffer length */
            uint8_t service_index;       /**< Service index */
            uint8_t att_index;           /**< Attribute index */
        } attr_cb_evt;

        /**
         * @brief Set server buffer command data
         *
         * Contains parameters for setting server buffer configurations
         */
        struct
        {
            uint8_t *buffer;             /**< Buffer pointer */
            uint16_t buffer_len;         /**< Current buffer length */
            uint16_t buffer_max_len;     /**< Maximum buffer length */
            uint8_t service_index;       /**< Service index */
            uint8_t att_index;           /**< Attribute index */
            void *pm;                    /**< Pointer to profile manager */
        } set_server_buffer_cmd;

        /**
         * @brief do cb only
         *
         * Do cb only
         */
        struct
        {
            uint32_t evt;
            void *param;
        } cb_only_evt;
    };
} ble_evt_msg_elem_t;

/**
 * @brief Message event callback function type
 *
 * Callback function prototype for handling BLE events
 *
 * @param evt Event identifier
 * @param status Operation status
 * @param param Additional event parameters
 * @return Operation result
 */

typedef int32_t (*msg_evt_cb)(uint32_t evt, int32_t status, void *param);

/**
 * @brief BLE event message structure
 *
 * This structure represents a BLE event message that can be used to transport
 * various types of event data between different components of the BLE stack.
 * It supports both simple parameter values and complex data structures.
 */
typedef struct ble_evt_msg
{
    uint16_t type;                /**< Event type identifier */
    union
    {
        uint32_t param;           /**< Simple parameter value */
        void *buf;                /**< Pointer to complex data structure (see ble_evt_msg_elem_t) */
    }u;
    uint32_t buff_size;           /**< Size of the buffer when using the buf union member */
    msg_evt_cb cb;                /**< Callback function for event handling */
} ble_evt_msg_t;

/**
 * @brief BLE command message element structure
 *
 * This structure contains various indices and parameters used for BLE command
 * processing, particularly related to advertisement operations.
 */
typedef struct
{
    uint8_t conn_idx;             /**< Connection index */
    uint8_t adv_elem;             /**< Advertisement element identifier */
    uint8_t adv_index;            /**< Advertisement index */
    uint8_t hal_adv_index;        /**< Hardware abstraction layer advertisement index */
    uint8_t oneshot_index;        /**< One-shot advertisement index */
    void *param;                  /**< Generic parameter pointer */
    void *nest_ret;               /**< Nested return value pointer */
} ble_cmd_msg_elem_t;


/**
 * @brief Message command callback function type
 *
 * Callback function prototype for handling BLE command responses and status updates
 *
 * @param evt Event identifier
 * @param status Operation status
 * @param param Additional command parameters
 * @return Operation result
 */
typedef int32_t (*msg_cmd_cb)(uint32_t evt, int32_t status, void *param);

/**
 * @brief BLE command message structure
 *
 * This structure represents a BLE command message used to issue commands
 * to the BLE stack and receive responses through callbacks.
 */
typedef struct ble_cmd_msg
{
    uint16_t event;               /**< Command event identifier */
    uint16_t idx;                 /**< Command index */
    uint32_t arg0;                /**< First command argument */

    void *param;                  /**< Generic parameter pointer (input/output) */
    uint32_t size;                /**< Size of the parameter buffer */
    msg_cmd_cb cb;                /**< Command callback function */
} ble_cmd_msg_t;

#define BLE_RPA_ADDR_MASK          0xC0 /**< Mask for RPA address type */
#define BLE_ADDR_TYPE_PUBLIC       0x00 /**< Public address type */
#define BLE_ADDR_TYPE_RANDOM       0x01 /**< Random address type */
#define BLE_RANDOM_ADDR_NON_PRA    0x00 /**< MSB mask for non-resolvable private address */
#define BLE_RANDOM_ADDR_STATIC     0xC0 /**< MSB mask for static address */
#define BLE_RANDOM_ADDR_PRA        0x40 /**< MSB mask for resolvable private address */

// Check if it is a non-resolvable private address
#define BLE_IS_NON_PRA(type, msb) ((BLE_ADDR_TYPE_RANDOM == type) && (BLE_RANDOM_ADDR_NON_PRA == (msb & BLE_RPA_ADDR_MASK)))
// Check if it is a static address
#define BLE_IS_STATIC_ADDR(type, msb) ((BLE_ADDR_TYPE_RANDOM == type) && (BLE_RANDOM_ADDR_STATIC == (msb & BLE_RPA_ADDR_MASK)))
// Check if it is a resolvable private address
#define BLE_IS_RPA(type, msb) ((BLE_ADDR_TYPE_RANDOM == type) && (BLE_RANDOM_ADDR_PRA == (msb & BLE_RPA_ADDR_MASK)))


#define BLE_ADV_INTERVAL_MIN 120
#define BLE_ADV_INTERVAL_MAX 160

#define BLE_SCAN_INTERVAL 0x64
#define BLE_SCAN_WINDOW   0x1C

#define BLE_HAL_FILTER_DUPLICATES_MAX   500

#define BLE_ADV_MAX_COUNT 6
#define BLE_STATIC_ADV_INDEX 0
#define BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT 2

/**
 * @brief Structure for storing BLE advertisement parameters at the HAL level.
 *
 * This structure contains various parameters related to BLE advertisement operations
 * managed by the Hardware Abstraction Layer (HAL).
 */
struct adv_hal_param
{
    uint8_t adv_idx;            /**< Advertisement index. */
    uint8_t adv_status;         /**< Advertisement status. */
    bd_addr_t own_addr;         /**< Own device address. */
    ble_adv_param_t adv_param;  /**< BLE advertisement parameters. */
    uint8_t advDataLen;         /**< Length of the advertisement data. */
    uint8_t respDataLen;        /**< Length of the response data. */
    uint8_t advData[BK_BLE_MAX_ADV_DATA_LEN];  /**< Advertisement data buffer. */
    uint8_t respData[BK_BLE_MAX_ADV_DATA_LEN]; /**< Response data buffer. */
    uint8_t random_adv_addr[6]; /**< Random advertisement address. */
    uint8_t deleting;           /**< Flag indicating whether the advertisement is being deleted. */
    uint8_t adv_elem;           /**< Advertisement element type. */
    uint8_t oneshot_app_id;     /**< One-shot advertisement application ID. */
    uint8_t max_evt;            /**< Maximum number of advertisement events. */
};
/**
 * @brief Structure for storing the BLE advertisement environment at the HAL level.
 *
 * This structure contains information about the initialization status, advertisement parameters,
 * and indices for different types of advertisements in the Hardware Abstraction Layer (HAL).
 */
typedef struct
{
    uint8_t init;               /**< Initialization status flag */

    struct adv_hal_param array[BLE_ADV_MAX_COUNT];  /**< Array of advertisement parameters for normal advertisements */
    struct adv_hal_param static_adv_param;          /**< Static advertisement parameters */
    struct adv_hal_param static_oneshot_adv_param[BLE_ONTSHOT_MAX_STATIC_PARAM_COUNT];  /**< Static one-shot advertisement parameters */

    uint8_t static_hal_index;   /**< Hardware abstraction layer index for static advertisements */
    uint8_t static_index;       /**< Index for static advertisements */
    uint8_t oneshot_init;       /**< Initialization status flag for one-shot advertisements */
} hal_ble_adv_env_t;

/**
 * @brief Structure for storing advertisement duplicate filter entries.
 *
 * This structure holds information about the advertisement address, event type, and address type
 * for duplicate filtering purposes.
 */
struct adv_dup_filter_entry
{
   uint8_t adv_addr[6];        /**< Advertisement address */
   uint8_t adv_evt_type;       /**< Advertisement event type */
   uint8_t addr_type;          /**< Address type */
};

/**
 * @brief Structure for storing BLE scan info filter parameters.
 *
 * This structure contains parameters for the scan info comparison function,
 * including enable flag, offset, length, and filter data.
 */
typedef struct {
	bool enable;		/*!< Whether to enable the scan info comparison function */
	uint8_t offset;		/*!< The start offset of the scan info to compare */
	uint8_t len;		/*!< Length of data to compare */
	uint8_t p_filter[TRBLE_ADV_RAW_DATA_MAX_LEN];	/*!< Pointer to the data to compare with the scan info */
} bt_le_scan_info_filter_param_t;

/**
 * @brief Structure for storing the BLE scan environment at the HAL level.
 *
 * This structure contains information about the scan index, status, stop cause,
 * scan parameters, duplicate filtering, and other related information in the
 * Hardware Abstraction Layer (HAL).
 */
typedef struct
{
    uint8_t scan_idx;           /**< Scan index */
    uint8_t scan_status;        /**< Scan status */
    uint8_t stop_cause;         /**< Scan stop cause (1: auto, 0: user) */
    ble_scan_param_t scan_param;/**< BLE scan parameters */
    uint8_t filt_duplicate;     /**< Flag for duplicate filtering */
    uint16_t duration;          /**< Scan duration */
    uint16_t period;            /**< Scan period */
    struct adv_dup_filter_entry *table; /**< Pointer to the advertisement duplicate filter entry table */
    uint16_t curr_pos;          /**< Current position in the duplicate filter table */
    uint16_t nb_adv;            /**< Number of advertisements */
    bt_le_scan_info_filter_param_t filter_param; /**< Scan info filter parameters */
}hal_ble_scan_env_t;

#define BLE_SYNC_CMD_TIMEOUT_MS   5000

/**
 * @brief Enumeration of BLE connection states at the HAL level.
 *
 * Defines the possible states of a BLE connection managed by the Hardware Abstraction Layer (HAL).
 */
enum
{
    HAL_CONN_STATE_DISCONNECTED, /**< The connection is disconnected. */
    HAL_CONN_STATE_CONNECTING,   /**< The connection is in the process of being established. */
    HAL_CONN_STATE_CONNECTED,    /**< The connection is successfully established. */
    HAL_CONN_STATE_DISCONNECTING /**< The connection is in the process of being disconnected. */
};

/**
 * @brief Structure for storing client CCCD (Client Characteristic Configuration Descriptor) information.
 *
 * This structure holds the client flag and CCCD handle related to a client's characteristic configuration.
 */
typedef struct {
    uint16_t client_flag;  /**< Flag indicating client-specific settings. */
    uint16_t cccd_handle;  /**< Handle of the Client Characteristic Configuration Descriptor. */
} client_cccd_info_t;

/**
 * @brief Structure for storing write operation information.
 *
 * This structure contains the client ID and attribute handle related to a write operation.
 */
typedef struct {
    uint8_t client_id;    /**< Identifier of the client performing the write operation. */
    uint16_t att_handle;  /**< Handle of the attribute to be written. */
} wr_info_t;

/**
 * @brief Structure for storing read operation information.
 *
 * This structure contains the client ID related to a read operation.
 */
typedef struct {
    uint8_t client_id;  /**< Identifier of the client performing the read operation. */
} rd_info_t;

/**
 * @brief Structure for storing service discovery operation information.
 *
 * This structure contains the client ID related to a service discovery operation.
 */
typedef struct {
    uint8_t client_id;  /**< Identifier of the client performing the service discovery operation. */
} dis_info_t;

/**
 * @brief Structure for storing coc information.
 *
 * This structure contains the coc releated a connection.
 */
typedef struct {
    uint16_t local_cid;  /**< local channel id. */
} coc_info_t;

/**
 * @brief Structure for storing BLE connection information at the HAL level.
 *
 * This structure contains various parameters related to a BLE connection managed by the Hardware Abstraction Layer (HAL).
 */
typedef struct {
    uint8_t con_status;           /**< Connection status, corresponding to the HAL_CONN_STATE_* enumeration. */
    uint8_t peer_addr_type;       /**< Type of the peer device address. */
    uint8_t peer_addr[6];         /**< Address of the peer device. */
    uint8_t role;                 /**< Role in the connection (master or slave). */
    uint16_t intv;                /**< Connection interval. */
    uint16_t con_latency;         /**< Connection latency. */
    uint16_t sup_to;              /**< Supervision timeout. */
    uint16_t mtu_size;            /**< Maximum Transmission Unit (MTU) size. */
    uint8_t auth_state;           /**< Authentication state. */
    ble_list_t *notify_list;      /**< List of clients currently performing write operations for notifications. */
    uint32_t notify_pending_count;/**< Count of pending notification operations. */
    beken_mutex_t notify_list_mutex; /**< Mutex for protecting the notification list. */
    ble_list_t *rd_list;          /**< List of clients currently performing read operations. */
    ble_list_t *wr_list;          /**< List of clients currently performing write operations. */
    ble_list_t *dis_list;         /**< List of clients currently performing service discovery operations. */
    uint8_t relate_adv_index;     /**< Related advertisement index. */
    beken_semaphore_t connection_sem; /**< Semaphore for connection management. */
    beken_semaphore_t data_sem;   /**< Semaphore for data management. */
    uint8_t app_connected;        /**< Flag indicating whether the application is connected. */
} hal_ble_conn_t;

#define HAL_BLE_CON_NUM  8
#define BLE_CON_INTERVAL_MIN 67
#define BLE_CON_INTERVAL_MAX 67
#define BLE_CON_CON_LATENCY 0
#define BLE_CON_CON_TIMEOUT 0x1f4
#define BLE_CON_SCAN_TIMEOUT (10000)
/**
 * @brief Structure for storing the BLE connection environment at the HAL level.
 *
 * This structure contains information about the initialization index and status,
 * connection parameters, peer address and its type, security connection status,
 * MTU size, connection scan timeout, and an array of HAL BLE connection devices.
 */
typedef struct
{
    uint8_t init_idx;           /**< Initialization index */
    uint8_t init_status;        /**< Initialization status */
    ble_conn_param_t conn_param;/**< BLE connection parameters */
    uint8_t peer_addr_type;     /**< Peer device address type */
    bd_addr_t peer_addr;        /**< Peer device address */
    bool is_secured_connect;    /**< Flag indicating whether it is a secure connection */
    uint8_t mtu;                /**< Maximum Transmission Unit size */
    uint16_t conn_scan_timeout; /**< Connection scan timeout value */
    hal_ble_conn_t con_dev[HAL_BLE_CON_NUM]; /**< Array of HAL BLE connection devices */
} hal_ble_con_env_t;

#define BK_MAX_SERVICES  (3)
#define BK_ATTR_IDX_MAX  (120)
#define BK_ATTR_MAX_SIZE (2000)
#define BK_ATTR_DEFAULT_SIZE (128)
#define BK_CCCD_MAX (50)
#define BK_DEFAULT_PRF_TASK_ID (0xFF)

#define BK_GATT_PRI_SERVICE_DECL_UUID 0x2800
#define BK_GATT_CHAR_DECL_UUID 0x2803
#define BK_GATT_CHAR_CLIENT_CONFIG_UUID 0x2902


#define BK_GATT_CCC_NOTIFY          0x0001 /**< The Characteristic Value shall be notified. */
#define BK_GATT_CCC_INDICATE        0x0002 /**< The Characteristic Value shall be indicated. */

#define BLE_DEFAULT_MAX_MTU   247//2010
#define BLE_MAX_CLIENT_COUNT  3
#define BLE_HAL_ADV_MAX_COUNT  3
#define CLIENT_PROFILE_GENERAL_ID   0xff
#define BK_UUID_16_LEN     2
#define BK_UUID_128_LEN    16

#define BLE_MAX_BONDED_DEVICE_NUM  18

#define ADV_IND (REPORT_INFO_SCAN_ADV_BIT | REPORT_INFO_CONN_ADV_BIT | REPORT_INFO_COMPLETE_BIT | REPORT_TYPE_ADV_LEG)
#define ADV_DIRECT_IND (REPORT_INFO_DIR_ADV_BIT | REPORT_INFO_CONN_ADV_BIT | REPORT_INFO_COMPLETE_BIT | REPORT_TYPE_ADV_LEG)
#define ADV_SCAN_IND (REPORT_INFO_SCAN_ADV_BIT | REPORT_INFO_COMPLETE_BIT | REPORT_TYPE_ADV_LEG)
#define ADV_NONCONN_IND (REPORT_INFO_COMPLETE_BIT | REPORT_TYPE_ADV_LEG)
#define SCAN_RSP (REPORT_INFO_COMPLETE_BIT | REPORT_TYPE_SCAN_RSP_LEG)

/**
 * @brief Structure for storing the BLE environment at the HAL level.
 *
 * This structure contains various parameters related to the BLE device state, authentication,
 * security, pairing, and application queue handles at the Hardware Abstraction Layer (HAL).
 */
typedef struct
{
    T_GAP_DEV_STATE dev_state;      /**< Device state of the GAP layer */
    uint16_t auth_flags;            /**< Authentication flags */
    uint8_t sec_req;                /**< Security requirements */
    uint8_t auth_io_cap;            /**< IO capabilities */
    uint8_t auth_oob;               /**< OOB data flag */
    uint8_t bond_flag;              /**< Bonding flags */
    uint8_t mitm_flag;              /**< MITM (Man-in-the-Middle) flag */
    uint8_t sec_pair_flag;          /**< Secure connection pairing support flag */
    uint8_t sec_pair_only_flag;     /**< Only accept secure connection pairing when local sec_pair_flag is set */
    uint8_t use_fixed_key;          /**< Pairing use fixed passkey */
    uint32_t fixed_key;             /**< Fixed passkey value */

    void *app_evt_queue_handle;     /**< Event queue handle */
    void *app_io_queue_handle;      /**< IO queue handle */
    uint8_t slave_connected;        /**< Flag indicating whether the slave is connected */
    uint8_t max_services;           /**< Maximum number of services */
    uint8_t client_idx;             /**< Client index */
    uint8_t client_num;             /**< Number of clients */
    uint8_t master_count;           /**< Count of master devices */
} hal_ble_env_t;

/**
 * @brief Structure for storing a whitelist element in the Bluetooth storage.
 *
 * This structure contains the address type and address of a peer device that is part of the whitelist.
 */
typedef struct
{
    uint8_t peer_addr_type;         /**< Type of the peer device address */
    uint8_t peer_addr[6];           /**< Address of the peer device */
} bt_storage_elem_whitelist_t;

/**
 * @brief Structure for storing bonding information in the Bluetooth storage.
 *
 * This structure contains an array of link keys for bonded devices, with a maximum count defined by BT_LINKKEY_MAX_SAVE_COUNT.
 */
typedef struct
{
    bk_ble_key_t linkkey[BT_LINKKEY_MAX_SAVE_COUNT];  /**< Array of link keys for bonded devices */
} bt_bond_info_storage_t;

/**
 * @brief Structure for storing whitelist information in the Bluetooth storage.
 *
 * This structure contains an array of whitelist elements, with a maximum count defined by BT_WHITELIST_MAX_SAVE_COUNT.
 */
typedef struct
{
    bt_storage_elem_whitelist_t device[BT_WHITELIST_MAX_SAVE_COUNT];  /**< Array of whitelist elements */
} bt_whitelist_info_storage_t;

/**
 * @brief Structure for storing BLE authentication key confirmation information.
 *
 * This structure contains the connection index and the passkey used for authentication confirmation.
 */
typedef struct
{
    uint16_t con_idx;               /**< Connection index */
    uint32_t passkey;               /**< Passkey value for authentication */
}ble_auth_key_cfm_t;

/**
 * @brief Structure for storing BLE bonding information.
 *
 * This structure contains a single whitelist element representing the bonded device.
 */
typedef struct
{
    bt_storage_elem_whitelist_t device;  /**< Whitelist element representing the bonded device */
}bt_bond_info_t;

#define BD_ADDR_FMT "0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x"
#define BD_ADDR_ARG(x) (x)[0],(x)[1],(x)[2],(x)[3],(x)[4],(x)[5]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]

/**
 * @brief Create a new BLE linked list.
 * @return Pointer to the newly created BLE linked list, or NULL if allocation fails.
 */
ble_list_t *ble_list_new(void);

/**
 * @brief Check if the BLE linked list is empty.
 * @param list Pointer to the BLE linked list to check.
 * @return true if the list is empty, false otherwise.
 */
bool ble_list_is_empty(const ble_list_t *list);

/**
 * @brief Free a node from the BLE linked list.
 * @param list Pointer to the BLE linked list.
 * @param node Pointer to the node to be freed.
 * @return Pointer to the next node in the list, or NULL if there is no next node.
 */
ble_list_node_t *ble_list_free_node(ble_list_t *list, ble_list_node_t *node);

/**
 * @brief Clear all nodes in the BLE linked list.
 * @param list Pointer to the BLE linked list to clear.
 */
void ble_list_clear(ble_list_t *list);

/**
 * @brief Free the entire BLE linked list.
 * @param list Pointer to the BLE linked list to free.
 */
void ble_list_free(ble_list_t *list);

/**
 * @brief Append a new node with the given data to the end of the BLE linked list.
 * @param list Pointer to the BLE linked list.
 * @param data Pointer to the data to be stored in the new node.
 * @return true if the append operation is successful, false otherwise.
 */
bool ble_list_append(ble_list_t *list, void *data);

/**
 * @brief Get the data of the first node in the BLE linked list.
 * @param list Pointer to the BLE linked list.
 * @return Pointer to the data of the first node, or NULL if the list is empty.
 */
void *ble_list_front(const ble_list_t *list);

/**
 * @brief Remove the first node containing the given data from the BLE linked list.
 * @param list Pointer to the BLE linked list.
 * @param data Pointer to the data to be removed.
 * @return true if the removal operation is successful, false otherwise.
 */
bool ble_list_remove(ble_list_t *list, void *data);

/**
 * @brief Compare two Bluetooth device addresses.
 * @param addr1 Pointer to the first Bluetooth device address.
 * @param addr2 Pointer to the second Bluetooth device address.
 * @return true if the addresses are equal, false otherwise.
 */
bool bdaddr_compare(uint8_t *addr1, uint8_t *addr2);

/**
 * @brief Convert the address type to the BK adapter format.
 * @param addr_type The original address type.
 * @return The converted address type in BK adapter format.
 */
uint8_t bk_adapter_ble_convert_addr_type_2_bk(uint8_t addr_type);

/**
 * @brief Convert the address type to the TizenRT format.
 * @param addr_type The original address type.
 * @param addr Pointer to the Bluetooth device address.
 * @return The converted address type in TizenRT format.
 */
uint8_t bk_adapter_ble_convert_addr_type_2_tr(uint8_t addr_type, uint8_t *addr);

/**
 * @brief Check if the advertisement report is in the list.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @param adv_evt_type The advertisement event type.
 * @return true if the report is in the list, false otherwise.
 */
bool hal_ble_adv_reports_list_check(uint8_t *addr, uint8_t addr_type, uint8_t adv_evt_type);

/**
 * @brief Push read operation information into the stack.
 * @param conn_id The connection ID.
 * @param client_id The client ID.
 * @return 0 on success, negative value on error.
 */
int ble_push_read_info(uint8_t conn_id, uint8_t client_id);

/**
 * @brief Pop read operation information from the stack.
 * @param conn_id The connection ID.
 * @param client_id Pointer to store the popped client ID.
 * @return 0 on success, negative value on error.
 */
int ble_pop_read_info(uint8_t conn_id, uint8_t *client_id);

/**
 * @brief Push write operation information into the stack.
 * @param conn_id The connection ID.
 * @param client_id The client ID.
 * @param handle The attribute handle.
 * @return 0 on success, negative value on error.
 */
int ble_push_write_info(uint8_t conn_id, uint8_t client_id, uint16_t handle);

/**
 * @brief Pop write operation information from the stack.
 * @param conn_id The connection ID.
 * @param client_id Pointer to store the popped client ID.
 * @param handle Pointer to store the popped attribute handle.
 * @return 0 on success, negative value on error.
 */
int ble_pop_write_info(uint8_t conn_id, uint8_t *client_id, uint16_t *handle);

/**
 * @brief Allocate a link key information entry in storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return Index of the allocated entry on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_alloc_linkkey_info(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Find the index of the link key information entry in storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return Index of the entry on success, negative value if not found.
 */
int32_t bk_adapter_bt_storage_find_linkkey_info_index(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Save the link key information to storage.
 * @param linkkey Pointer to the link key information.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_save_linkkey_info(bk_ble_key_t *linkkey);

/**
 * @brief Delete the link key information entry from storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_del_linkkey_info(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Clean all link key information entries from storage.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_clean_linkkey_info(void);

/**
 * @brief Get the number of bonded devices.
 * @return The number of bonded devices.
 */
uint8_t bk_adapter_bt_storage_get_bond_device_num(void);

/**
 * @brief Get information about bonded devices.
 * @param info Pointer to the buffer to store bonded device information.
 * @param size The size of the buffer.
 * @return The number of devices retrieved.
 */
uint8_t bk_adapter_bt_storage_get_bond_device(bt_bond_info_t *info, uint8_t size);

/**
 * @brief Sync bond information to flash memory.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_sync_bond_info_to_flash(void);

/**
 * @brief Allocate a whitelist information entry in storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return Index of the allocated entry on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_alloc_whitelist_info(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Find the index of the whitelist information entry in storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return Index of the entry on success, negative value if not found.
 */
int32_t bk_adapter_bt_storage_find_whitelist_info_index(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Save the whitelist information to storage.
 * @param device Pointer to the whitelist device information.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_save_whitelist_info(bt_storage_elem_whitelist_t *device);

/**
 * @brief Delete the whitelist information entry from storage.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_del_whitelist_info(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Clean all whitelist information entries from storage.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_clean_whitelist_info(void);

/**
 * @brief Sync whitelist information to flash memory.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_sync_whitelist_info_to_flash(void);

/**
 * @brief Show Bluetooth storage information.
 */
void bk_adapter_bt_storage_info_show(void);

/**
 * @brief Display the data in the Bluetooth whitelist.
 */
void bk_adapter_ble_display_white_list_data(void);

/**
 * @brief Check if the address is in the Bluetooth whitelist.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 * @return 1 if the address is in the whitelist, 0 otherwise.
 */
uint8_t bk_adapter_ble_check_white_list_addr(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Clear all data in the Bluetooth whitelist.
 */
void bk_adapter_ble_clear_white_list_data(void);

/**
 * @brief Add an address to the Bluetooth whitelist.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 */
void bk_adapter_ble_add_white_list_data(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Remove an address from the Bluetooth whitelist.
 * @param addr Pointer to the Bluetooth device address.
 * @param addr_type The address type.
 */
void bk_adapter_ble_remove_white_list_data(uint8_t *addr, uint8_t addr_type);

/**
 * @brief Initialize the Bluetooth storage.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_init(void);

/**
 * @brief Deinitialize the Bluetooth storage.
 * @return 0 on success, negative value on error.
 */
int32_t bk_adapter_bt_storage_deinit(void);

/**
 * @brief Find the initial pseudo address.
 * @param addr Pointer to store the initial pseudo address.
 */
void bk_adapter_ble_find_initial_pseudo_addr(uint8_t *addr);

/**
 * @brief Reset the client information.
 * @param conidx The connection index.
 */
void bk_adapter_reset_client_info(uint8 conidx);

/**
 * @brief Clear all bonding keys.
 */
void le_bond_clear_all_keys(void);

/**
 * @brief Delete the bonding information by Bluetooth device address.
 * @param bd_addr Pointer to the Bluetooth device address.
 * @param bd_type The address type.
 */
void le_bond_delete_by_bd(uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
 * @brief Get the number of active links.
 * @return The number of active links.
 */
uint8_t le_get_active_link_num(void);

/**
 * @brief Get the number of idle links.
 * @return The number of idle links.
 */
uint8_t le_get_idle_link_num(void);

/**
 * @brief Update the connection parameters.
 * @param conn_id The connection ID.
 * @param conn_interval_min The minimum connection interval.
 * @param conn_interval_max The maximum connection interval.
 * @param conn_latency The connection latency.
 * @param supervision_timeout The supervision timeout.
 * @param ce_length_min The minimum connection event length.
 * @param ce_length_max The maximum connection event length.
 * @return 0 on success, non-zero value on error.
 */
uint8_t le_update_conn_param(uint8_t conn_id, uint16_t  conn_interval_min, uint16_t  conn_interval_max, uint16_t  conn_latency, uint16_t  supervision_timeout, uint16_t  ce_length_min, uint16_t  ce_length_max);

/**
 * @brief Notify the Bluetooth status.
 * @param sub_type The sub-type of the Bluetooth status.
 * @param msg_data Pointer to the message data.
 */
void bk_adapter_notify_bt_status(uint16_t sub_type, T_LE_GAP_MSG_DATA *msg_data);

/**
 * @brief Convert the HCI error code to the BK adapter format.
 * @param code The original HCI error code.
 * @return The converted HCI error code in BK adapter format.
 */
uint16_t bk_adapter_ble_hci_error_code(uint16_t code);

/**
 * @brief Modify the Bluetooth whitelist.
 * @param operation The operation type.
 * @param bd_addr Pointer to the Bluetooth device address.
 * @param bd_type The address type.
 * @return 0 on success, non-zero value on error.
 */
uint8_t le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr, T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
 * @brief Set the BLE command semaphore.
 */
void bk_set_ble_cmd_sema(void);

/**
 * @brief Push an event into the BLE event queue.
 * @param type The event type.
 * @param param Pointer to the event parameter.
 * @return 0 on success, negative value on error.
 */
int ble_evt_queue_push(uint16_t type, void *param);

/**
 * @brief Push an extended event into the BLE event queue.
 * @param type The event type.
 * @param param Pointer to the event parameter.
 * @param size The size of the event parameter.
 * @param cb The callback function for the event.
 * @return 0 on success, negative value on error.
 */
int ble_evt_queue_push_ext(uint16_t type, void *param, uint32_t size, msg_evt_cb cb);

/**
 * @brief Push a command into the BLE command queue.
 * @param event The command event.
 * @param idx The command index.
 * @return 0 on success, negative value on error.
 */
int ble_cmd_queue_push(uint16_t event, uint16_t idx);

/**
 * @brief Push an extended command into the BLE command queue.
 * @param event The command event.
 * @param idx The command index.
 * @param param Pointer to the command parameter.
 * @param size The size of the command parameter.
 * @param cb The callback function for the command.
 * @return 0 on success, negative value on error.
 */
int ble_cmd_queue_push_ext(uint16_t event, uint16_t idx,
                           void *param,
                           uint32_t size,
                           msg_cmd_cb cb);

/**
 * @brief Perform a bonding pairing operation.
 * @param conn_id The connection ID.
 * @return 0 on success, negative value on error.
 */
int le_bond_pair(uint8_t conn_id);

/**
 * @brief Find the advertisement index by the advertisement handle.
 * @param adv_handle The advertisement handle.
 * @return The advertisement index on success, negative value if not found.
 */
int8_t hal_ble_find_adv_index_by_handle(uint8 adv_handle);

extern hal_ble_env_t hal_ble_env;
extern hal_ble_adv_env_t hal_ble_adv_env;
extern hal_ble_scan_env_t hal_ble_scan_env;
extern hal_ble_con_env_t hal_ble_con_env;

#endif
