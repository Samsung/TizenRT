#pragma once

#include <tinyara/net/if/ble.h>

/**
 * @brief GATT Client Read Indication Structure
 * 
 * This structure contains information about a GATT client read operation
 * including connection index, attribute handle, data length, and data pointer.
 */
typedef struct
{
    uint8_t conidx;     /**< Connection index */
    uint16_t hdl;       /**< Attribute handle */
    uint16_t len;       /**< Data length */
    uint8_t *data;      /**< Pointer to data buffer */
} ble_gattc_read_ind_t;

/**
 * @brief BLE Notification Structure
 * 
 * This structure contains notification data received from a BLE peripheral
 * including the operation handle and notification data.
 */
typedef struct
{
  trble_operation_handle handle;    /**< Operation handle for the notification */
  trble_data noti_data;             /**< Notification data */
} ble_tizenrt_notification_t;

/**
 * @brief BLE Indication Structure
 * 
 * This structure contains indication data received from a BLE peripheral
 * including the operation handle and indication data.
 */
typedef struct
{
  trble_operation_handle handle;    /**< Operation handle for the indication */
  trble_data indi_data;             /**< Indication data */
} ble_tizenrt_indication_t;

/**
 * @brief Get BLE Client Initialization Parameters
 * 
 * This function retrieves the BLE client initialization configuration parameters.
 * 
 * @return Pointer to trble_client_init_config structure containing client parameters
 */
trble_client_init_config *bktr_ble_client_get_param(void);

/**
 * @brief Initialize BLE Client
 * 
 * This function initializes the BLE client with the provided configuration.
 * 
 * @param config Pointer to trble_client_init_config structure containing
 *                initialization parameters
 * @return int32_t Operation result:
 *         - 0: Success
 *         - Negative value: Error code
 */
int32_t bk_tr_ble_client_init(trble_client_init_config *config);

/**
 * @brief Deinitialize BLE Client
 * 
 * This function deinitializes the BLE client and releases associated resources.
 * 
 * @return int32_t Operation result:
 *         - 0: Success
 *         - Negative value: Error code
 */
int32_t bk_tr_ble_client_deinit(void);

/**
 * @brief BLE Client Notice Callback
 * 
 * This callback function handles various BLE client events and notifications.
 * It processes different types of BLE notices and takes appropriate actions.
 * 
 * @param notice Type of BLE notice/event
 * @param param Pointer to event-specific parameters
 * @return int32_t Operation result:
 *         - 0: Success
 *         - Negative value: Error code
 */
int32_t bk_tr_ble_client_notice_cb(ble_notice_t notice, void *param);
