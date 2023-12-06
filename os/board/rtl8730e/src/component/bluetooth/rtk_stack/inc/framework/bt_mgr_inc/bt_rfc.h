/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_RFC_H_
#define _BT_RFC_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \defgroup  BT_RFC BT RFC Protocol
 *
 * \brief  Provide BT RFC protocol interfaces.
 */

/**
 * bt_rfc.h
 *
 * \brief  BT RFCOMM message type.
 *
 * \ingroup BT_RFC
 */
typedef enum t_bt_rfc_msg_type
{
    BT_RFC_MSG_CONN_IND        = 0x00,      /**< RFCOMM connect indicate */
    BT_RFC_MSG_CONN_CMPL       = 0x01,      /**< RFCOMM connect successful */
    BT_RFC_MSG_DISCONN_CMPL    = 0x02,      /**< RFCOMM disconnect successful */
    BT_RFC_MSG_DATA_IND        = 0x03,      /**< RFCOMM data indicate */
    BT_RFC_MSG_CREDIT_INFO     = 0x04,      /**< RFCOMM remote credits infomation */
} T_BT_RFC_MSG_TYPE;

/**
 * bt_rfc.h
 *
 * \brief  Indication of RFCOMM connection request from remote device. It will be received
 *         in the callback function registered by \ref bt_rfc_profile_register with message
 *         type as \ref BT_RFC_MSG_CONN_IND.
 *
 * \ingroup BT_RFC
 */
typedef struct t_bt_rfc_conn_ind
{
    uint8_t     local_server_chann;
    uint16_t    frame_size;
} T_BT_RFC_CONN_IND;

/**
 * bt_rfc.h
 *
 * \brief  Indication of completion of creating RFCOMM connection. It will be received in
 *         the callback function registered by \ref bt_rfc_profile_register with message
 *         type as \ref BT_RFC_MSG_CONN_CMPL.
 *
 * \ingroup BT_RFC
 */
typedef struct t_bt_rfc_conn_cmpl
{
    uint8_t     local_server_chann;
    uint16_t    frame_size;
    uint8_t     remain_credits;
} T_BT_RFC_CONN_CMPL;

/**
 * bt_rfc.h
 *
 * \brief  Indication of completion of breaking RFCOMM connection. It will be received in
 *         the callback function registered by \ref bt_rfc_profile_register with message
 *         type as \ref BT_RFC_MSG_DISCONN_CMPL.
 *
 * \ingroup BT_RFC
 */
typedef struct t_bt_rfc_disconn_cmpl
{
    uint8_t     local_server_chann;
    uint16_t    cause;
} T_BT_RFC_DISCONN_CMPL;

/**
 * bt_rfc.h
 *
 * \brief  Indication of RFCOMM data. It will be received in the callback
 *         function registered by \ref bt_rfc_profile_register with message
 *         type as \ref BT_RFC_MSG_DATA_IND.
 *
 * \ingroup BT_RFC
 */
typedef struct t_bt_rfc_data_ind
{
    uint8_t     local_server_chann;
    uint8_t     remain_credits;
    uint16_t    length;
    uint8_t    *buf;
} T_BT_RFC_DATA_IND;

/**
 * bt_rfc.h
 *
 * \brief  Indication of RFCOMM credits info. It will be received in the callback function
 *         registered by \ref bt_rfc_profile_register with message type as \ref BT_RFC_MSG_CREDIT_INFO.
 *
 * \ingroup BT_RFC
 */
typedef struct t_bt_rfc_credit_info
{
    uint8_t     local_server_chann;
    uint8_t     remain_credits;
} T_BT_RFC_CREDIT_INFO;

/**
 * bt_rfc.h
 *
 * \brief  RFCOMM message callback definition.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  msg_type   RFCOMM message type.
 * \param[in]  p_msg      Message buffer address.
 *
 * \ingroup BT_RFC
 */
typedef void (* P_BT_RFC_PROFILE_CBACK)(uint8_t *bd_addr, T_BT_RFC_MSG_TYPE msg_type, void *p_msg);

/**
 * bt_rfc.h
 *
 * \brief  Register profile to RFCOMM.
 *
 * \param[in] server_chann    Server channel number of the registered profile.
 * \param[in] cback           Callback function used to handle RFCOMM message.
 *
 * \return    The result of Registration.
 * \retval true    Registration has been completed successfully.
 * \retval false   Registration was failed to complete.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_profile_register(uint8_t server_chann, P_BT_RFC_PROFILE_CBACK cback);

/**
 * bt_rfc.h
 *
 * \brief  Send a request to create a RFCOMM connection.
 *
 * \param[in] bd_addr             Remote BT address.
 * \param[in] local_server_chann  The local server channel of the service that sending this request.
 * \param[in] remote_server_chann The remote server channel which can be found from the sdp info.
 * \param[in] frame_size          The max frame_size supported by local device.
 * \param[in] init_credits        The number of packet that remote can be send. This para is used for flow control. A
 *                                sending entity may send as many frames on a RFCOMM channel as it has credits;if the
 *                                credit count reaches zero,the sender will stop and wait for further credits from peer.
 *
 * \return    The status of sending connection request.
 * \retval true    Request has been sent successfully.
 * \retval false   Request was fail to send.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_conn_req(uint8_t *bd_addr, uint8_t local_server_chann, uint8_t remote_server_chann,
                     uint16_t frame_size, uint8_t init_credits);

/**
 * bt_rfc.h
 *
 * \brief  Send a confirmation to accept or reject the received RFCOMM connection request.
 *
 * \param[in] bd_addr            Remote BT address.
 * \param[in] local_server_chann Local server channel.
 * \param[in] accept             Confirmation message.
 * \arg true    Accept the received RFCOMM connection request.
 * \arg false   Reject the received RFCOMM connection request.
 * \param[in] frame_size         The max frame_size supported by local device.
 * \param[in] credits            The number of packet that remote can be send. This para is used for flow control.A sending
 *                               entity may send as many frames on a RFCOMM channel as it has credits;if the credit count
 *                               reaches zero,the sender will stop and wait for further credits from peer.
 *
 * \return    The result of sending confirmation.
 * \retval true    The confirmation has been sent successfully.
 * \retval false   The confirmation was fail to send.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_conn_cfm(uint8_t *bd_addr, uint8_t local_server_chann, bool accept,
                     uint16_t frame_size, uint8_t init_credits);

/**
 * bt_rfc.h
 *
 * \brief  Send a request to send data to remote device.
 *
 * \param[in] bd_addr            Remote BT address.
 * \param[in] local_server_chann Local server channel.
 * \param[in] p_data             The start address of the data buffer.
 * \param[in] data_len           The length of the data to be sent.
 *
 * \return    The status of sending data.
 * \retval true    Data has been sent successfully.
 * \retval false   Data was fail to send.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_data_send(uint8_t *bd_addr, uint8_t local_server_chann,
                      uint8_t *p_data, uint16_t data_len);

/**
 * bt_rfc.h
 *
 * \brief  Send a confirmation to indicate the number of received packets.
 *
 * \param[in] bd_addr            Remote BT address.
 * \param[in] local_server_chann Local server channel.
 * \param[in] credits            The number of credit.
 *
 * \return    The result of sending confirmation.
 * \retval true    The confirmation has been sent successfully.
 * \retval false   The confirmation was fail to send.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_credits_give(uint8_t *bd_addr, uint8_t local_server_chann, uint8_t credits);

/**
 * bt_rfc.h
 *
 * \brief  Send a request to disconnect a RFCOMM channel.
 *
 * \param[in] bd_addr            Remote BT address.
 * \param[in] local_server_chann Local server channel.
 *
 * \return    The result of sending request.
 * \retval true    The request has been sent successfully.
 * \retval false   The request was fail to send.
 *
 * \ingroup BT_RFC
 */
bool bt_rfc_disconn_req(uint8_t *bd_addr, uint8_t local_server_chann);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_RFC_H_ */
