#pragma once

#include <tinyara/net/if/ble.h>
#include <stdint.h>

//see rtk_bt_le_coc_param_type_t
typedef enum
{
    BK_LE_COC_CHAN_PARAM_LOCAL_MTU     = 0x1,
} bk_le_coc_param_type_t;

//see rtk_bt_le_coc_chan_param_type_t
typedef enum
{
    BK_LE_COC_CHAN_PARAM_CUR_CREDITS   = 0x0,
    BK_LE_COC_CHAN_PARAM_MAX_CREDITS   = 0x1,
    BK_LE_COC_CHAN_PARAM_MTU           = 0x2,
} bk_le_coc_chan_param_type_t;

//see rtk_bt_le_coc_security_mode_t
typedef enum
{
    BK_LE_COC_SEC_NONE,                 /**< Security None */
    BK_LE_COC_SEC_UNAUTHEN_ENCRYPT,     /**< Security unauthenticated encryption */
    BK_LE_COC_SEC_AUTHEN_ENCRYPT,       /**< Security authenticated encryption */
    BK_LE_COC_SEC_UNAUTHEN_DATA_SIGN,   /**< Security unauthenticated data signed */
    BK_LE_COC_SEC_AUTHEN_DATA_SIGN,     /**< Security authenticated data signed */
    BK_LE_COC_SEC_AUTHOR,               /**< Security authorized */
    BK_LE_COC_SEC_SECURE_CONN_UNAUTHEN, /**< Security unauthenticated LE secure connection */
    BK_LE_COC_SEC_SECURE_CONN_AUTHEN,   /**< Security LE secure connection */
} bk_le_coc_security_mode_t;

/**
 * @brief Initialize common BLE coc components
 *
 * Prepares shared resources and configurations required by BLE features.
 * Should be called before any BLE operations.
 *
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful initialization
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_init_private(void);

/**
 * @brief Deinitialize BLE coc components
 *
 * Releases resources allocated by bk_tr_ble_coc_init_private().
 * Should be called after all BLE operations complete.
 *
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful deinitialization
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_deinit_private(void);

/**
 * @brief Initialize BLE Connection Oriented Channel (COC)
 *
 * Initializes the BLE COC feature with the provided configuration.
 * This function sets up callbacks and prepares the COC subsystem for use.
 *
 * @param[in] le_coc Pointer to the COC initialization configuration structure
 *                   containing callback functions for COC events
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful initialization
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_init(trble_le_coc_init_config *le_coc);

/**
 * @brief Register or unregister a Protocol/Service Multiplexer (PSM)
 *
 * Registers or unregisters a PSM value for BLE COC. PSM is used to identify
 * different services or protocols over a single connection.
 *
 * @param[in] is_reg Registration flag: 1 to register, 0 to unregister
 * @param[in] psm Protocol/Service Multiplexer value to register or unregister
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful registration/unregistration
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_register_psm(uint8_t is_reg, uint16_t psm);

/**
 * @brief Set security parameters for a PSM
 *
 * Configures security settings (encryption, authentication, key size) for
 * a specific PSM. This must be called before establishing connections on the PSM.
 *
 * @param[in] le_psm The PSM value to configure security for
 * @param[in] active Enable/disable security: 1 to enable, 0 to disable
 * @param[in] sec_mode Security mode
 * @param[in] key_size Encryption key size in bytes
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful configuration
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_set_psm_security(uint16_t le_psm, uint8_t active, bk_le_coc_security_mode_t sec_mode, uint8_t key_size);

/**
 * @brief Set COC parameter value
 *
 * Sets a general parameter value for the COC subsystem.
 *
 * @param[in] value The parameter value to set
 * @param[in] type The parameter type to set
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful parameter setting
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_set_param(bk_le_coc_param_type_t type, uint16_t value);

/**
 * @brief Get COC parameter value
 *
 * Retrieves a parameter value from the COC subsystem or a specific connection.
 *
 * @param[in] param_type Type of parameter to retrieve
 * @param[in] cid Connection ID (Channel ID) for connection-specific parameters,
 *                or 0 for global parameters
 * @param[out] value Pointer to store the retrieved parameter value
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful retrieval
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_chan_get_param(uint16_t conn_handle, bk_le_coc_chan_param_type_t param_type, uint16_t cid, uint16_t *value);

/**
 * @brief Establish a COC connection
 *
 * Initiates a connection-oriented channel connection to a remote device
 * using the specified connection handle and PSM.
 *
 * @param[in] conn_handle BLE connection handle to the remote device
 * @param[in] le_psm Protocol/Service Multiplexer value to connect to
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful connection initiation
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_connect(uint16_t conn_handle, uint16_t le_psm);

/**
 * @brief Disconnect a COC channel
 *
 * Closes an established connection-oriented channel.
 *
 * @param[in] conn_handle BLE connection handle to the remote device
 * @param[in] cid Channel ID of the COC connection to disconnect
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful disconnection
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_disconnect(uint16_t conn_handle, uint16_t cid);

/**
 * @brief Send data over a COC channel
 *
 * Transmits data through an established connection-oriented channel.
 *
 * @param[in] conn_handle BLE connection handle to the remote device
 * @param[in] cid Channel ID of the COC connection
 * @param[in] len Length of data to send in bytes
 * @param[in] data Pointer to the data buffer to send
 * @return int32_t Operation status
 * @retval BT_SUCCESS On successful data transmission
 * @retval Negative error code On failure
 */
int32_t bk_tr_ble_coc_send_data(uint16_t conn_handle, uint16_t cid, uint16_t len, uint8_t *data);

/**
 * @brief private adapter func, bk ble evt process in coc module
 *
 * @param
 *    - notice: ble evt type
 *    - param: param that assosicate with notice
 *
 * @return
 *    - 0: succeed
 *    - others: errors.
 */
int32_t bk_tr_ble_coc_notice_cb(ble_notice_t notice, void *param);
