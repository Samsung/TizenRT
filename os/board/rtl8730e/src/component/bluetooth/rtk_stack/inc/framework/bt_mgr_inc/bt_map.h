/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_MAP_H_
#define _BT_MAP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * \defgroup    BT_MAP BT MAP Profile
 *
 * \brief   Provide BT MAP profile interfaces.
 */

/**
 * bt_map.h
 *
 * \brief  Define BT MAP Folder.
 *
 * \ingroup BT_MAP
 */
typedef enum t_bt_map_folder
{
    BT_MAP_FOLDER_ROOT           = 0x00,
    BT_MAP_FOLDER_TELECOM        = 0x01,
    BT_MAP_FOLDER_MSG            = 0x02,
    BT_MAP_FOLDER_INBOX          = 0x03,
    BT_MAP_FOLDER_OUTBOX         = 0x04,
    BT_MAP_FOLDER_SENT           = 0x05,
    BT_MAP_FOLDER_DELETED        = 0x06,
    BT_MAP_FOLDER_DRAFT          = 0x07,
} T_BT_MAP_FOLDER;

/**
 * bt_map.h
 *
 * \brief  BT MAP response code.
 *
 * \ingroup BT_MAP
 */
typedef enum t_bt_map_response_code
{
    BT_MAP_RSP_CONTINUE             = 0x90,
    BT_MAP_RSP_SUCCESS              = 0xA0,
    BT_MAP_RSP_BAD_REQUEST          = 0xC0,
    BT_MAP_RSP_UNAUTHORIZED         = 0xC1,
    BT_MAP_RSP_FORBIDDEN            = 0xC3,
    BT_MAP_RSP_NOT_IMPLEMENTED      = 0xD1,
    BT_MAP_RSP_SERVICE_UNAVAILABLE  = 0xD3
} T_BT_MAP_RESPONSE_CODE;

/**
 * bt_map.h
 *
 * \brief  BT MAP PUSH MESSAGE ACTION.
 *
 * \ingroup BT_MAP
 */
typedef enum t_bt_map_push_msg_action
{
    BT_MAP_PUSH_MSG_COMPLETE        = 0x00,  /* Push complete */
    BT_MAP_PUSH_MSG_CONTINUE        = 0x01,  /* Push next packet */
    BT_MAP_PUSH_MSG_AGAIN           = 0x02,  /* Push previous packet again */
    BT_MAP_PUSH_MSG_TO_END          = 0x03,  /* Push packets one by one from begining to end */
} T_BT_MAP_PUSH_MSG_ACTION;

/**
 * bt_map.h
 *
 * \brief   Initialize MAP profile.
 *
 * \param[in]  link_num          MAP maximum connected link number.
 * \param[in]  mns_server_chann  RFCOMM Server channel to be allocated for MNS.
 * \param[in]  mns_l2c_psm       L2CAP PSM to be allocated for MNS.
 * \param[in]  support_feat      MAP Client supported features.
 *
 * \return          The status of initializing MAP profile.
 * \retval true     MAP profile was initialized successfully.
 * \retval false    MAP profile was failed to initialize.
 *
 * \ingroup BT_MAP
 */
bool bt_map_init(uint8_t link_num, uint8_t mns_server_chann,
                 uint16_t mns_l2c_psm, uint32_t support_feat);

/**
 * bt_map.h
 *
 * \brief   Accept or reject the incoming MNS Connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  accept     Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication
 *
 * \return         The status of sending the MNS connection confirmation.
 * \retval true    MNS connection confirmation was sent successfully.
 * \retval false   MNS connection confirmation was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mns_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * bt_map.h
 *
 * \brief  Send a MAS connection request over RFCOMM.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  server_chann    The remote server channel defined in SDP record.
 *
 * \return         The status of sending the MAS connection request.
 * \retval true    MAS connection request was sent successfully.
 * \retval false   MAS connection request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_connect_over_rfc_req(uint8_t *bd_addr, uint8_t server_chann);

/**
 * bt_map.h
 *
 * \brief  Send a MAS connection request over L2CAP.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  l2c_psm         The remote L2CAP psm defined in SDP record.
 *
 * \return         The status of sending the MAS connection request.
 * \retval true    MAS connection request was sent successfully.
 * \retval false   MAS connection request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_connect_over_l2c_req(uint8_t *bd_addr, uint16_t l2c_psm);

/**
 * bt_map.h
 *
 * \brief  Send a MAS disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the MAS disconnection request.
 * \retval true    MAS disconnection request was sent successfully.
 * \retval false   MAS disconnection request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_disconnect_req(uint8_t *bd_addr);

/**
 * bt_map.h
 *
 * \brief  Acknowledge on receiving an event report Indication.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  rsp_code   Response code on on receiving an event report.
 *
 * \return         The status of sending the SendEvent response.
 * \retval true    MNS SendEvent response was sent successfully.
 * \retval false   MNS SendEvent response was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mns_send_event_rsp(uint8_t *bd_addr, T_BT_MAP_RESPONSE_CODE rsp_code);

/**
 * bt_map.h
 *
 * \brief  Register or Unregister the message notifications.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  enable      true to register and false to unregister.
 *
 * \return         The status of sending the set folder request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_msg_notification_set(uint8_t *bd_addr, bool enable);

/**
 * bt_map.h
 *
 * \brief  Request to navigate the folders of MSE.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  folder     Folder to be navigated of MSE.
 *
 * \return         The status of sending the set folder request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_folder_set(uint8_t *bd_addr, T_BT_MAP_FOLDER folder);

/**
 * bt_map.h
 *
 * \brief  Request to get the remote folder listing.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  max_list_count  The maximum number of folders to be listed.
 * \param[in]  start_offset    Offset of the first entry to be listed.
 *
 * \return         The status of sending the get folder listing request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_folder_listing_get(uint8_t *bd_addr, uint16_t max_list_count,
                                   uint16_t start_offset);

/**
 * bt_map.h
 *
 * \brief  Request to get the list of messages from the MSE folder.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  folder          Name of the child folder or null for the current folder.
 * \param[in]  folder_len      Length of folder name.
 * \param[in]  max_list_count  The maximum number of folders to be listed.
 * \param[in]  start_offset    Offset of the first entry to be listed.
 *
 * \return         The status of sending the get message listing request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_msg_listing_get(uint8_t *bd_addr, uint8_t *folder, uint16_t folder_len,
                                uint16_t max_list_count, uint16_t start_offset);

/**
 * bt_map.h
 *
 * \brief  Request to get the message from the MSE folder.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  msg_handle      Message handle, null-terminated Unicode text string with 16 hexadecimal digits.
 * \param[in]  handle_len      Length of message handle.
 * \param[in]  native          true for native and false for UTF-8 message.
 *
 * \return         The status of sending the get message request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_msg_get(uint8_t *bd_addr, uint8_t *msg_handle, uint8_t handle_len, bool native);

/**
 * bt_map.h
 *
 * \brief  Request to get more data from the remote server.
 *
 * \param[in]  bd_addr     Remote BT address.
 *
 * \return         The status of sending get continue message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_get_continue(uint8_t *bd_addr);

/**
 * bt_map.h
 *
 * \brief  Request to terminate the ongoing get operation.
 *
 * \param[in]  bd_addr     Remote BT address.
 *
 * \return         The status of sending get abort message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_get_abort(uint8_t *bd_addr);

/**
 * bt_map.h
 *
 * \brief  Request to send a new message.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  folder      Name of the message folder.
 * \param[in]  folder_len  Length of folder name.
 * \param[in]  native      true for native and false for UTF-8 message.
 * \param[in]  more_data   true if the message is partial and more data to send later.
 * \param[in]  msg         The message to be sent.
 * \param[in]  msg_len     Length of message.
 *
 * \return         The status of sending the message.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_MAP
 */
bool bt_map_mas_msg_push(uint8_t *bd_addr, uint8_t *folder, uint16_t folder_len,
                         bool native, bool more_data, uint8_t *msg, uint16_t msg_len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_MAP_H_ */
