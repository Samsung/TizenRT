#ifndef _RAW_LINK_H_
#define _RAW_LINK_H_

#include <common/bk_err.h>
#include <modules/wifi_types.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RLK_WIFI_MAC_ADDR_LEN 6

typedef enum {
    WIFI_IF_STA = 0,
    WIFI_IF_MAX
} bk_rlk_wifi_if_t;

/**
 * \brief          BK Raw Link(RLK) peer list context structure
 */
typedef struct bk_rlk_peer_info {
    uint8_t mac_addr[RLK_WIFI_MAC_ADDR_LEN];
    uint8_t channel;

    bk_rlk_wifi_if_t ifidx;
    uint8_t state;
    bool encrypt;
} bk_rlk_peer_info_t;

/**
  * @brief     Callback function of sending BK Raw Link data
  * @param     args callback arguments
  * @param     status status of sending BK Raw Link data (succeed or fail)
  */
typedef void (*bk_rlk_send_ex_cb_t)(void *args, bool status);

/**
 * \brief          BK Raw Link(RLK) configure tx information
 */
typedef struct bk_rlk_config_info {
    uint16_t len;               /**< Rx length of BK RLK packet */
    uint8_t *data;              /**< Rx data address of BK RLK packet */
    bk_rlk_send_ex_cb_t cb;    /**<  callback of TX,cb has two arguments, first is args, second is tx success flag  */
    void *args;                 /**< callback arguments */
    uint32_t tx_rate;           /**<  TX rate information */
    uint32_t tx_power;          /**<  TX power information */
    uint32_t tx_retry_cnt;      /**<  TX retry count */
}bk_rlk_config_info_t;

/**
 * @brief BK RLK Receive packet information
 */
typedef struct bk_rlk_recv_info {
    uint8_t src_addr[RLK_WIFI_MAC_ADDR_LEN];    /**< Source address of BK RLK packet */
    uint8_t des_addr[RLK_WIFI_MAC_ADDR_LEN];    /**< Destination address of BK RLK packet */
    int rssi;                                   /**< Rx RSSI of BK RLK packet */
    uint16_t len;                               /**< Rx length of BK RLK packet */
    uint8_t *data;                              /**< Rx data address of BK RLK packet */
} bk_rlk_recv_info_t;

/**
 * @brief Status of sending BK Raw Link data .
 */
typedef enum {
    BK_RLK_SEND_SUCCESS = 0,       /**< Send BK Raw Link data successfully */
    BK_RLK_SEND_FAIL,              /**< Send BK Raw Link data fail */
} bk_rlk_send_status_t;

/**
  * @brief     Callback function of receiving BK Raw Link data
  * @param     rx_info received BK Raw Link packet information
  * @attention bk_rlk_info is a local variable，it can only be used in the callback.
  */
typedef bk_err_t (*bk_rlk_recv_cb_t)(bk_rlk_recv_info_t *rx_info);

/**
  * @brief     Callback function of sending BK Raw Link data
  * @param     peer_mac_addr peer MAC address
  * @param     status status of sending BK Raw Link data (succeed or fail)
  */
typedef bk_err_t (*bk_rlk_send_cb_t)(const uint8_t *peer_mac_addr, bk_rlk_send_status_t status);

/**
  * @brief     Initialize Raw Link function
  *
  * @return
  *    - BK_OK: succeed
  *    - others: other errors
  */
bk_err_t bk_rlk_init(void);

/**
  * @brief     De-Initialize Raw Link function
  *
  * @return
  *    - BK_OK: succeed
  *    - others: other errors
  */
bk_err_t bk_rlk_deinit(void);

/**
  * @brief     Register callback function of receiving Raw Link data
  *
  * @param     cb  callback function of receiving Raw Link data
  *
  * @return
  *          - BK_OK: succeed
  *          - BK_ERR_NOT_INIT: Raw Link is not initialized
  *          - others : other errors
  */
bk_err_t bk_rlk_register_recv_cb(bk_rlk_recv_cb_t cb);

/**
  * @brief     Unregister callback function of receiving Raw Link data
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - others : other errors
  */
bk_err_t bk_rlk_ungister_recv_cb(void);

/**
  * @brief     Register callback function of sending BK RLK data
  *
  * @param     cb  callback function of sending BK RLK data for MAC send status
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  */
bk_err_t bk_rlk_register_send_cb(bk_rlk_send_cb_t cb);

/**
  * @brief     Unregister callback function of sending Raw Link data
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  */
bk_err_t bk_rlk_unregister_send_cb(void);

/**
 * @brief     Set BK Raw Link channel
 *
 * @attention 1. Currently we only support HT20, chan->second should always be 0.
 * @attention 2. This API is only used for settting Raw Link channel, NOT for other purpose.
 * @attention 3. The channel range can be set is from channel 1 to channel 13,
 *
 * @param chan channel of Raw Link
 *
 * @return
 *    - BK_OK: succeed
 *    - others: other errors
 */
bk_err_t bk_rlk_set_channel(uint8_t chan);

/**
  * @brief     Send BK Raw Links data
  *
  * @attention 1. If peer_addr is not NULL, send data to the peer whose MAC address matches peer_addr
  * @attention 2. If peer_addr is NULL, send data to all of the peers that are added to the peer list
  *
  * @param     peer_mac_addr  peer MAC address
  * @param     data  data to send
  * @param     len  length of data
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_NO_MEM : out of memory
  *          - BK_ERR_NOT_FOUND : peer is not found
  */
bk_err_t bk_rlk_send(const uint8_t *peer_mac_addr, const void *data,size_t len);

/**
  * @brief     Send Raw Links data
  *
  * @attention 1. If peer_addr is not NULL, send data to the peer whose MAC address matches peer_addr
  * @attention 2. If peer_addr is NULL, send data to all of the peers that are added to the peer list
  * @attention 3. if set cb is not NULL,cb prototype void (*)(void *, bool)
  *               cb has two arguments, first is args, second is tx success flag
  * @param     peer_mac_addr   peer MAC address
  * @param     rlk_tx  configure BK Raw Link sending data information
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_NO_MEM : out of memory
  *          - BK_ERR_NOT_FOUND : peer is not found
  */
bk_err_t bk_rlk_send_ex(const uint8_t *peer_mac_addr,const bk_rlk_config_info_t *rlk_tx);

/**
  * @brief     Send BK Raw Links data
  *
  * @attention 1. If peer_addr is not NULL, send data to the peer whose MAC address matches peer_addr
  * @attention 2. If peer_addr is NULL, send data to all of the peers that are added to the peer list
  *
  * @param     peer_mac_addr  peer MAC address
  * @param     data  data to send
  * @param     len  length of data
  * @param     mac_type  frame type
  * @param     oui  vendor oui
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_NO_MEM : out of memory
  *          - BK_ERR_NOT_FOUND : peer is not found
  */
bk_err_t bk_rlk_send_by_oui(const uint8_t *peer_mac_addr, const void *data,size_t len, uint8_t mac_type, uint8_t *oui);

/**
  * @brief     Add a peer to peer list
  *
  * @param     peer  peer information
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_IS_FALL : peer list is full
  *          - BK_ERR_NO_MEM : out of memory
  *          - BK_ERR_IS_EXIST : peer has existed
  */
bk_err_t bk_rlk_add_peer(const bk_rlk_peer_info_t *peer);

/**
  * @brief     Delete a peer from peer list
  *
  * @param     peer_mac_addr  peer MAC address
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_NOT_FOUND : peer is not found
  */
bk_err_t bk_rlk_del_peer(const uint8_t *peer_mac_addr);

/**
  * @brief     Get a peer whose MAC address matches peer_addr from peer list
  *
  * @param     peer_mac_addr  peer MAC address
  * @param     peer  peer information
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_NOT_INIT : Raw Link is not initialized
  *          - BK_ERR_NOT_FOUND : peer is not found
  */
bk_err_t bk_rlk_get_peer(const uint8_t *peer_mac_addr, bk_rlk_peer_info_t *peer);

/**
  * @brief     Peer exists or not
  *
  * @param     peer_mac_addr  peer MAC address
  *
  * @return
  *          - BK_OK : peer exists
  *          - others : peer not exists
  */
bk_err_t bk_rlk_is_peer_exist(const uint8_t *peer_mac_addr);

/**
  * @brief     Get the number of peers
  *
  * @param     num  number of peers
  *
  * @return
  *          - BK_OK : succeed
  *          - BK_ERR_RLK_NOT_INIT : Raw Link is not initialized
  */
bk_err_t bk_rlk_get_peer_num(uint32_t *total_num);

/**
 * @brief     Set BK Raw Link TX AC
 *
 * @param     ac  sending Raw Link data AC
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Set TX AC error
 */
bk_err_t bk_rlk_set_tx_ac(uint8_t ac);

 /**
 * @brief     Set BK Raw Link TX timeout
 *
 * @param     ac  sending Raw Link data timeout
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Set TX timeout error
 */
bk_err_t bk_rlk_set_tx_timeout_ms(uint16_t timeout_ms);

/**
 * @brief     Set BK Raw Link TX power
 *
 * @param     power  TX power control
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Set TX power error
 */
bk_err_t bk_rlk_set_tx_power(uint32_t power);

/**
 * @brief     Set BK Raw Link TX rate
 *
 * @param     rate  TX rate control
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Set TX rate error
 */
bk_err_t bk_rlk_set_tx_rate(uint32_t rate);

/**
 * @brief     Set BK Raw Link TX retry count
 *
 * @param     rate  TX retry count control
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Set TX rate error
 */
bk_err_t bk_rlk_set_tx_retry_cnt(uint32_t retry_cnt);

/**
 * @brief     Let Raw Link goto sleep mode to reduce power consumption
 *
 * @param     void
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Raw Link failed to sleep
 */
bk_err_t bk_rlk_sleep(void);

/**
 * @brief     wakeup Raw Link
 *
 * @param     void
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Failed to wakeup raw link
 */
bk_err_t bk_rlk_wakeup(void);

/**
 * @brief     Raw Link add white list
 *
 * @param     mac_type  frame type
 * @param     oui  vendor oui
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Failed to add white list
 */
bk_err_t bk_rlk_add_white_list(uint8_t mac_type, uint8_t *oui);

/**
 * @brief     Raw Link delete white list
 *
 * @param     mac_type  frame type
 * @param     oui  vendor oui
 *
 * @return
 *          - BK_OK : succeed
 *          - others : Failed to delete white list
 */
bk_err_t bk_rlk_del_white_list(uint8_t mac_type, uint8_t *oui);

#ifdef __cplusplus
}
#endif
#endif //_RAW_LINK_H_
// eof

