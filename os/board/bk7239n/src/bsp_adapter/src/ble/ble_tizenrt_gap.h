#ifndef _BLE_GAP_H_
#define _BLE_GAP_H_

#include <stdint.h>

#define GAP_BD_ADDR_LEN               6     //!< Default Bluetooth Device Address Length.

#define GAP_MSG_LE_DEV_STATE_CHANGE        0x01 //!< Device state change msg type.
#define GAP_MSG_LE_CONN_STATE_CHANGE       0x02 //!< Connection state change msg type.
#define GAP_MSG_LE_CONN_PARAM_UPDATE       0x03 //!< Connection parameter update changed msg type.
#define GAP_MSG_LE_CONN_MTU_INFO           0x04 //!< Connection MTU size info msg type.

#define GAP_MSG_LE_AUTHEN_STATE_CHANGE     0x05 //!< Authentication state change msg type.
#define GAP_MSG_LE_BOND_PASSKEY_DISPLAY    0x06 //!< Bond passkey display msg type.
#define GAP_MSG_LE_BOND_PASSKEY_INPUT      0x07 //!< Bond passkey input msg type.
#define GAP_MSG_LE_BOND_OOB_INPUT          0x08 //!< Bond passkey oob input msg type.
#define GAP_MSG_LE_BOND_USER_CONFIRMATION  0x09 //!< Bond user confirmation msg type.
#define GAP_MSG_LE_BOND_JUST_WORK          0x0A //!< Bond user confirmation msg type.

#define GAP_INIT_STATE_INIT          0   //!< Waiting to be started
#define GAP_INIT_STATE_STACK_READY   1   //!< Stack is ready

#define GAP_ADV_STATE_IDLE           0   //!< Idle, no advertising
#define GAP_ADV_STATE_START          1   //!< Start Advertising. A temporary state, haven't received the result.
#define GAP_ADV_STATE_ADVERTISING    2   //!< Advertising
#define GAP_ADV_STATE_STOP           3   //!< Stop Advertising. A temporary state, haven't received the result.

#define GAP_ADV_TO_IDLE_CAUSE_STOP   0 //!<Advertising is stopped by user stop or high duty cycle directed advertising timeout
#define GAP_ADV_TO_IDLE_CAUSE_CONN   1 //!<Advertising is stopped for link establishment

#define GAP_SCAN_STATE_IDLE          0   //!< Idle, no scanning
#define GAP_SCAN_STATE_START         1   //!< Start scanning. A temporary state, haven't received the result.
#define GAP_SCAN_STATE_SCANNING      2   //!< Scanning
#define GAP_SCAN_STATE_STOP          3   //!< Stop scanning, A temporary state, haven't received the result.

#define GAP_CONN_DEV_STATE_IDLE          0   //!< Idle
#define GAP_CONN_DEV_STATE_INITIATING    1   //!< Initiating Connection

#define GAP_AUTHEN_STATE_STARTED             0x00  //!< Authentication started
#define GAP_AUTHEN_STATE_COMPLETE            0x01  //!< Authentication complete

#define GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS      0 //!< Connection paramter update status success.
#define GAP_CONN_PARAM_UPDATE_STATUS_FAIL         1 //!< Connection paramter update status failed.
#define GAP_CONN_PARAM_UPDATE_STATUS_PENDING      2 //!< Connection paramter update status pending.

#define GAP_PAIRING_MODE_NO_PAIRING          0x00  //!< Pairing is not allowed.
#define GAP_PAIRING_MODE_PAIRABLE            0x01  //!< Pairable, Wait for a pairing request from master or security request from slave.

#define GAP_AUTHEN_BIT_NONE                  0      //!<  No authentication required.
#define GAP_AUTHEN_BIT_BONDING_FLAG          0x0001 //!<  Bonding is required
#define GAP_AUTHEN_BIT_MITM_FLAG             0x0004 //!<  Mitm is preferred
#define GAP_AUTHEN_BIT_SC_FLAG               0x0008 //!<  Secure connection is preferred
#define GAP_AUTHEN_BIT_SC_ONLY_FLAG          0x0200 //!<  Secure connection only mode for BLE is required
#define GAP_AUTHEN_BIT_FORCE_BONDING_FLAG    0x0100 //!<  Force bonding is required

#define GAP_SUCCESS                         0x00
#define SM_ERR                  0x0500
#define SM_ERR_NO_ENTRY                         0xE1
#define SM_ERR_DB_FULL                          0xE2
#define SM_ERR_INVALID_PARAM                    0xE3
#define SM_ERR_INSUFFICIENT_LINK_KEY            0xE4
#define SM_ERR_LE_ADDR_NOT_RESOLVED             0xE5
#define SM_ERR_INVALID_STATE                    0xE6
#define SM_ERR_NO_RESOURCE                      0xE7
#define SM_ERR_LINK_KEY_MISSING                 0xE8
#define SM_ERR_DISCONNECT                       0xE9
#define SM_ERR_PARING_NOT_ALLOWED               0xEA
#define SM_ERR_KEY_SAVE_FAILED                  0xEB
#define SM_ERR_TIMEOUT                          0xEC
#define SM_ERR_UNKNOWN                          0xED
#define SM_ERR_NO_PAIRABLE_MODE                 0xEF
#define SM_ERR_VENDOR                           0xFF


/** @brief GAP Remote Address Type */
typedef struct
{
    uint8_t gap_init_state: 1;  //!< @ref GAP_INIT_STATE
    uint8_t gap_adv_sub_state: 1;  //!< @ref GAP_ADV_SUB_STATE
    uint8_t gap_adv_state: 2;   //!< @ref GAP_ADV_STATE
    uint8_t gap_scan_state: 2;  //!< @ref GAP_SCAN_STATE
    uint8_t gap_conn_state: 2;  //!< @ref GAP_CONN_STATE
} T_GAP_DEV_STATE;

/** @brief GAP Remote Address Type */
typedef enum
{
    GAP_REMOTE_ADDR_LE_PUBLIC          = 0x00, /**< LE Public device address type. */
    GAP_REMOTE_ADDR_LE_RANDOM          = 0x01, /**< LE Random device address type. */
    GAP_REMOTE_ADDR_LE_ANONYMOUS       = 0xff, /**< LE anonymous device address type. */
} T_GAP_REMOTE_ADDR_TYPE;

/** @brief  Definition of LE white list operation.*/
typedef enum
{
    GAP_WHITE_LIST_OP_CLEAR = 0,    /**<  Clear white list. */
    GAP_WHITE_LIST_OP_ADD,          /**<  Add a device to the white list. */
    GAP_WHITE_LIST_OP_REMOVE        /**<  Remove a device from the white list. */
} T_GAP_WHITE_LIST_OP;

/** @brief  The msg_data of GAP_MSG_LE_DEV_STATE_CHANGE.*/
typedef struct
{
    T_GAP_DEV_STATE new_state;
    uint16_t cause;
} T_GAP_DEV_STATE_CHANGE;


/** @brief  The msg_data of GAP_MSG_LE_CONN_STATE_CHANGE.*/
typedef struct
{
    uint8_t conn_id;
    uint8_t new_state;
    uint16_t disc_cause;
} T_GAP_CONN_STATE_CHANGE;

/** @brief  The msg_data of GAP_MSG_LE_CONN_PARAM_UPDATE.*/
typedef struct
{
    uint8_t conn_id;
    uint8_t status;
    uint16_t cause;
} T_GAP_CONN_PARAM_UPDATE;

/** @brief  The msg_data of GAP_MSG_LE_AUTHEN_STATE_CHANGE.*/
typedef struct
{
    uint8_t conn_id;
    uint8_t new_state;
    uint16_t status;
} T_GAP_AUTHEN_STATE;

/** @brief  The msg_data of GAP_MSG_LE_BOND_PASSKEY_DISPLAY.*/
typedef struct
{
    uint8_t conn_id;
} T_GAP_BOND_PASSKEY_DISPLAY;

/** @brief  The msg_data of GAP_MSG_LE_BOND_USER_CONFIRMATION.*/
typedef struct
{
    uint8_t conn_id;
} T_GAP_BOND_USER_CONF;

/** @brief  The msg_data of T_GAP_BOND_JUST_WORK_CONF.*/
typedef struct
{
    uint8_t conn_id;
} T_GAP_BOND_JUST_WORK_CONF;
/** @brief  The msg_data of GAP_MSG_LE_BOND_PASSKEY_INPUT.*/
typedef struct
{
    uint8_t conn_id;
    uint8_t key_press;
} T_GAP_BOND_PASSKEY_INPUT;

/** @brief  The msg_data of GAP_MSG_LE_BOND_OOB_INPUT.*/
typedef struct
{
    uint8_t conn_id;
} T_GAP_BOND_OOB_INPUT;

/** @brief  The msg_data of GAP_MSG_LE_CONN_MTU_INFO.*/
typedef struct
{
    uint8_t   conn_id;
    uint16_t  mtu_size;
} T_GAP_CONN_MTU_INFO;

/** @brief  The msg_data of T_LE_GAP_MSG. */
typedef union
{
    T_GAP_DEV_STATE_CHANGE     gap_dev_state_change;
    T_GAP_CONN_STATE_CHANGE    gap_conn_state_change;
    T_GAP_CONN_PARAM_UPDATE    gap_conn_param_update;
    T_GAP_CONN_MTU_INFO        gap_conn_mtu_info;

    T_GAP_AUTHEN_STATE         gap_authen_state;
    T_GAP_BOND_PASSKEY_DISPLAY gap_bond_passkey_display;
    T_GAP_BOND_PASSKEY_INPUT   gap_bond_passkey_input;
    T_GAP_BOND_OOB_INPUT       gap_bond_oob_input;
    T_GAP_BOND_USER_CONF       gap_bond_user_conf;
    T_GAP_BOND_JUST_WORK_CONF  gap_bond_just_work_conf;
} T_LE_GAP_MSG_DATA;

/** @brief GAP connection states*/
typedef enum
{
    GAP_CONN_STATE_DISCONNECTED, /*!< Disconnected. */
    GAP_CONN_STATE_CONNECTING,   /*!< Connecting. */
    GAP_CONN_STATE_CONNECTED,    /*!< Connected. */
    GAP_CONN_STATE_DISCONNECTING /*!< Disconnecting. */
} T_GAP_CONN_STATE;

/** @brief GAP link roles */
typedef enum
{
    GAP_LINK_ROLE_UNDEFINED,    //!< Unknown.
    GAP_LINK_ROLE_MASTER,       //!< Role is master.
    GAP_LINK_ROLE_SLAVE         //!< Role is slave.
} T_GAP_ROLE;

/** @brief  Connected device information.*/
typedef struct
{
    T_GAP_CONN_STATE conn_state;             //!< Connection state
    T_GAP_ROLE       role;                   //!< Device role
    uint8_t          remote_bd[GAP_BD_ADDR_LEN];  //!< Remote address
    uint8_t          remote_bd_type;         //!< Remote address type
} T_GAP_CONN_INFO;

/**
 * @brief Characteristic Properties
 *  \defgroup gatt_characteristic_properties Characteristic Properties
 *  \{
 *  Characteristic Properties - Combination of these properties (combined
 *  using bitwise OR) describe properties of the Characteristic. Also
 *  see \ref GATT_DB_ATTR_LIST.
 */
enum
{
    /**
     *  Characteristic support Broadcast of its value to the peer.
     *  Setting this property automatically includes Characteristic Server
     *  Configuration Descriptor to the Characteristic
     */
    GATT_DB_CHAR_BROADCAST_PROPERTY =                0x00000001U,

    /** Characteristic support Reading its value by peer */
    GATT_DB_CHAR_READ_PROPERTY =                     0x00000002U,

    /** Characteristic support Writing its value by peer Without Response  */
    GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY =        0x00000004U,

    /** Characteristic supports Writing its value by peer */
    GATT_DB_CHAR_WRITE_PROPERTY =                    0x00000008U,

    /**
     *  Characteristic supports Notifying its value to the peer.
     *  Setting this property automatically includes Characteristic Server
     *  Configuration Descriptor to the Characteristic
     */
    GATT_DB_CHAR_NOTIFY_PROPERTY =                   0x00000010U,

    /** Characteristic supports Indicating its value to the peer */
    GATT_DB_CHAR_INDICATE_PROPERTY =                 0x00000020U,

    /** Characteristic supports Signed Write on its value */
    GATT_DB_CHAR_SIGNED_WRITE_PROPERTY =             0x00000040U,

    /** Characteristic supports write on its User Description Descriptor */
    GATT_DB_CHAR_WRIEABLE_AUX_PROPERTY =             0x00000200U,

    /** No Auxillary Property */
    GATT_DB_NO_AUXILLARY_PROPERTY =                  0x00U,

    /** Characteristic Value is Fixed Length */
    GATT_DB_FIXED_LENGTH_PROPERTY =                  0x01U,

    GATT_DB_AUTHORIZATION_PROPERTY =                 0x02U,

    GATT_DB_PEER_SPECIFIC_VAL_PROPERTY =             0x40U,

    GATT_DB_CONST_ATTR_VAL_PROPERTY =                0x80U,
};

#endif