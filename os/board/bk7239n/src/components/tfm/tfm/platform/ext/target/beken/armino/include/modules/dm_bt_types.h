// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDE_MODULES_BK_DM_BT_TYPES_H_
#define INCLUDE_MODULES_BK_DM_BT_TYPES_H_

#include <modules/bt_types.h>


/// Discoverability and Connectability mode
typedef enum {
    BK_BT_NON_CONNECTABLE,             /*!< Non-connectable */
    BK_BT_CONNECTABLE,                 /*!< Connectable */
} bk_bt_conn_mode_t;

typedef enum {
    BK_BT_NON_DISCOVERABLE,            /*!< Non-discoverable */
    BK_BT_DISCOVERABLE,                /*!< Discoverable */
} bk_bt_disc_mode_t;

/*
 * @brief common class of device
 */
typedef enum
{
    COD_PHONE    = 0x40020C,
    COD_SOUNDBAR = 0x240414,
    COD_HEADSET  = 0x240404,
} common_cod_t;

/**
 * @brief A2DP callback events
 */
typedef enum {
    BK_A2DP_CONNECTION_STATE_EVT = 0,           /*!< connection state changed event */
    BK_A2DP_AUDIO_STATE_EVT,                    /*!< audio stream transmission state changed event */
    BK_A2DP_AUDIO_CFG_EVT,                      /*!< audio codec is configured, only used for A2DP SINK */
} bk_a2dp_cb_event_t;

/**
 * @brief Bluetooth A2DP connection states
 */
typedef enum {
    BK_A2DP_CONNECTION_STATE_DISCONNECTED = 0, /*!< connection released  */
    BK_A2DP_CONNECTION_STATE_CONNECTING,       /*!< connecting remote device */
    BK_A2DP_CONNECTION_STATE_CONNECTED,        /*!< connection established */
    BK_A2DP_CONNECTION_STATE_DISCONNECTING     /*!< disconnecting remote device */
} bk_a2dp_connection_state_t;

/**
 * @brief Bluetooth A2DP disconnection reason
 */
typedef enum {
    BK_A2DP_DISC_RSN_NORMAL = 0,               /*!< Finished disconnection that is initiated by local or remote device */
    BK_A2DP_DISC_RSN_ABNORMAL                  /*!< Abnormal disconnection caused by signal loss */
} bk_a2dp_disc_rsn_t;

/**
 * @brief Bluetooth A2DP datapath states
 */
typedef enum {
    BK_A2DP_AUDIO_STATE_SUSPEND = 0,           /*!< audio stream datapath suspend */
    BK_A2DP_AUDIO_STATE_STARTED,               /*!< audio stream datapath started */
} bk_a2dp_audio_state_t;

/**
 * @brief A2DP media codec capabilities union
 */
typedef struct {
    uint8_t type;                              /*!< A2DP media codec type */

    union {
        /**
         * @brief  SBC codec capabilities
         */
        struct
        {
            uint8_t channels;
            uint8_t channel_mode;
            uint8_t block_len;
            uint8_t subbands;
            uint32_t sample_rate;
            uint8_t bit_pool;
            uint8_t alloc_mode;
        }sbc_codec;

        /**
         * @brief  MPEG-2, 4 AAC codec capabilities
         */
        struct
        {
            uint8_t channels;
            uint32_t sample_rate;
        }aac_codec;
    } cie;                                     /*!< A2DP codec information element */
} __attribute__((packed)) bk_a2dp_mcc_t;

/**
 * @brief A2DP state callback parameters
 */
typedef union {
    /**
     * @brief  BK_A2DP_CONNECTION_STATE_EVT
     */
    struct a2dp_conn_state_param {
        bk_a2dp_connection_state_t state;      /*!< one of values from bk_a2dp_connection_state_t */
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
        bk_a2dp_disc_rsn_t disc_rsn;           /*!< reason of disconnection for "DISCONNECTED" */
    } conn_state;                              /*!< A2DP connection status */

    /**
     * @brief BK_A2DP_AUDIO_STATE_EVT
     */
    struct a2dp_audio_state_param {
        bk_a2dp_audio_state_t state;           /*!< one of the values from bk_a2dp_audio_state_t */
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
    } audio_state;                             /*!< audio stream playing state */

    /**
     * @brief BK_A2DP_AUDIO_CFG_EVT
     */
    struct a2dp_audio_cfg_param {
        uint8_t remote_bda[6];                 /*!< remote bluetooth device address */
        bk_a2dp_mcc_t mcc;                     /*!< A2DP media codec capability information */
    } audio_cfg;                               /*!< media codec configuration information */
}bk_a2dp_cb_param_t;


/*
 * @brief used in bk_bt_gap_set_event_callback, this enum show as "event", you must analyse param in same time
 */
typedef enum
{
    /// bt stack init ok, param NULL
    BK_DM_BT_EVENT_STACK_OK,

    /// inquiry result, not used now
    BK_DM_BT_EVENT_INQUIRY_RESULT,

    /// disconnect completed, not used now
    BK_DM_BT_EVENT_DISCONNECT,

    //BK_DM_BT_EVENT_CMD_COMPLETE,

    /// connection completed, not used now
    BK_DM_BT_EVENT_CONNECTION_COMPLETE,

    /// recv cb when pair success and get link key, param bk_bt_linkkey_storage_t
    BK_DM_BT_EVENT_LINKKEY_NOTIFY,

    /// recv cb when peer need user to input link key, param bd_addr_t
    BK_DM_BT_EVENT_LINKKEY_REQ,

} bt_event_enum_t;





/**
 * @brief bt link key storage
 */
typedef struct
{
    /// this struct size
    uint16_t size;

    /// bt addr
    //bd_addr_t addr;
    uint8_t addr[6];

    /// bt link key
    uint8_t link_key[16];//BT_LINK_KEY_SIZE];

}__attribute__((packed)) bk_bt_linkkey_storage_t;//SM_DEVICE_ENTITY




/**
 * @brief           A2DP profile callback function type
 *
 * @param           event : Event type
 *
 * @param           param : Pointer to callback parameter
 */
typedef void (* bk_bt_a2dp_cb_t)(bk_a2dp_cb_event_t event, bk_a2dp_cb_param_t *param);

/**
 * @brief           A2DP sink data callback function
 *
 * @param[in]       buf : pointer to the data received from A2DP source device
 *
 * @param[in]       len : size(in bytes) in buf
 */
typedef void (* bk_bt_sink_data_cb_t)(const uint8_t *buf, uint16_t len);





/**
 * @brief for async dm_bt api event.
 *
 * bt event report.
 *
 * @param
 * - event: event id. see bt_event_enum_t
 * - param: param
 *
**/
typedef uint32_t (*bt_event_cb_t)(bt_event_enum_t event, void *param);

#endif
