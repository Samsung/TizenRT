/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_AVRCP_H_
#define _BT_AVRCP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_AVRCP BT AVRCP Profile
 *
 * \brief   Provide BT AVRCP profile interfaces.
 */

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP supported features.
 *
 * \ingroup BT_AVRCP
 */
#define BT_AVRCP_FEATURE_CATEGORY_1  (1 << 0)
#define BT_AVRCP_FEATURE_CATEGORY_2  (1 << 1)
#define BT_AVRCP_FEATURE_CATEGORY_3  (1 << 2)
#define BT_AVRCP_FEATURE_CATEGORY_4  (1 << 3)

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP command type.
 *
 * \ingroup BT_AVRCP
 */
typedef enum t_bt_avrcp_ctype
{
    BT_AVRCP_CTYPE_CONTROL            = 0x00,
    BT_AVRCP_CTYPE_STATUS             = 0x01,
    BT_AVRCP_CTYPE_SPECIFIC_INQUIRY   = 0x02,
    BT_AVRCP_CTYPE_NOTIFY             = 0x03,
    BT_AVRCP_CTYPE_GENERAL_INQUIRY    = 0x04,
} T_BT_AVRCP_CTYPE;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP response code.
 *
 * \ingroup BT_AVRCP
 */
typedef enum t_bt_avrcp_response
{
    BT_AVRCP_RESPONSE_NOT_IMPLEMENTED  = 0x08,
    BT_AVRCP_RESPONSE_ACCEPTED         = 0x09,
    BT_AVRCP_RESPONSE_REJECTED         = 0x0a,
    BT_AVRCP_RESPONSE_IN_TRANSITION    = 0x0b,
    BT_AVRCP_RESPONSE_IMPLEMENTED      = 0x0c,
    BT_AVRCP_RESPONSE_CHANGED          = 0x0d,
    BT_AVRCP_RESPONSE_INTERIM          = 0x0f,
} T_BT_AVRCP_RESPONSE;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP element attribute.
 *
 * \ingroup BT_AVRCP
 */
typedef enum t_bt_avrcp_elem_attr
{
    BT_AVRCP_ELEM_ATTR_TITLE             = 0x01,
    BT_AVRCP_ELEM_ATTR_ARTIST            = 0x02,
    BT_AVRCP_ELEM_ATTR_ALBUM             = 0x03,
    BT_AVRCP_ELEM_ATTR_TRACK             = 0x04,
    BT_AVRCP_ELEM_ATTR_TOTAL_TRACK       = 0x05,
    BT_AVRCP_ELEM_ATTR_GENRE             = 0x06,
    BT_AVRCP_ELEM_ATTR_PLAYING_TIME      = 0x07,
    BT_AVRCP_ELEM_ATTR_DEFAULT_COVER_ART = 0x08,
} T_BT_AVRCP_ELEM_ATTR;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP play status.
 *
 * \ingroup BT_AVRCP
 */
typedef enum t_bt_avrcp_play_status
{
    BT_AVRCP_PLAY_STATUS_STOPPED  = 0x00,
    BT_AVRCP_PLAY_STATUS_PLAYING  = 0x01,
    BT_AVRCP_PLAY_STATUS_PAUSED   = 0x02,
    BT_AVRCP_PLAY_STATUS_FWD_SEEK = 0x03,
    BT_AVRCP_PLAY_STATUS_REV_SEEK = 0x04,
    BT_AVRCP_PLAY_STATUS_FAST_FWD = 0x05,
    BT_AVRCP_PLAY_STATUS_REWIND   = 0x06,
    BT_AVRCP_PLAY_STATUS_ERROR    = 0xff,
} T_BT_AVRCP_PLAY_STATUS;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP element attribute.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_element_attr
{
    uint32_t attribute_id;
    uint16_t character_set_id;
    uint16_t length;
    uint8_t *p_buf;
} T_BT_AVRCP_ELEMENT_ATTR;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP app setting attribute value pair.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_app_setting
{
    uint8_t attr;
    uint8_t value;
} T_BT_AVRCP_APP_SETTING;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP item attribute.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_item_attr
{
    uint32_t attribute_id;
    uint16_t character_set_id;
    uint16_t length;
    uint8_t *p_buf;
} T_BT_AVRCP_ITEM_ATTR;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP media player item.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_media_player_item
{
    uint8_t   item_type;
    uint16_t  item_length;
    uint16_t  player_id;
    uint8_t   major_type;
    uint32_t  sub_type;
    uint8_t   play_status;
    uint8_t   feature_bitmask[16];
    uint16_t  character_set_id;
    uint16_t  display_name_length;
    uint8_t  *p_display_name;
} T_BT_AVRCP_MEDIA_PLAYER_ITEM;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP folder item.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_folder_item
{
    uint8_t   item_type;
    uint16_t  item_length;
    uint64_t  folder_uid;
    uint8_t   folder_type;
    uint8_t   is_playable;
    uint16_t  character_set_id;
    uint16_t  display_name_length;
    uint8_t  *p_display_name;
} T_BT_AVRCP_FOLDER_ITEM;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP media element item.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_media_element_item
{
    uint8_t                  item_type;
    uint16_t                 item_length;
    uint64_t                 media_element_uid;
    uint8_t                  media_type;
    uint16_t                 character_set_id;
    uint16_t                 display_name_length;
    uint8_t                 *p_display_name;
    uint8_t                  num_of_attr;
    T_BT_AVRCP_ELEMENT_ATTR *p_attr;
} T_BT_AVRCP_MEDIA_ELEMENT_ITEM;

/**
 * bt_avrcp.h
 *
 * \brief  BT AVRCP folder.
 *
 * \ingroup BT_AVRCP
 */
typedef struct t_bt_avrcp_folder
{
    uint16_t name_length;
    uint8_t *p_name;
} T_BT_AVRCP_FOLDER;

/**
 * bt_avrcp.h
 *
 * \brief   Initialize AVRCP profile.
 *
 * \param[in]  link_num    AVRCP maximum connected link number.
 *
 * \return          The status of initializing AVRCP profile.
 * \retval true     AVRCP profile was initialized successfully.
 * \retval false    AVRCP profile was failed to initialize.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_init(uint8_t link_num);

/**
 * bt_avrcp.h
 *
 * \brief   Set AVRCP supported features.
 *
 * \param[in]  ct_features   The supported features of controller.
 * \param[in]  tg_features   The supported features of target.
 *
 * \return          The status of setting AVRCP supported features.
 * \retval true     AVRCP supported features was set successfully.
 * \retval false    AVRCP supported features was failed to set.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_supported_features_set(uint8_t ct_features, uint8_t tg_features);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP connection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP connection request.
 * \retval true    AVRCP connection request was sent successfully.
 * \retval false   AVRCP connection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_connect_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP disconnection request.
 * \retval true    AVRCP disconnection request was sent successfully.
 * \retval false   AVRCP disconnection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_disconnect_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP connection confirmation.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  accept     Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication
 *
 * \return         The status of sending the AVRCP connection confirmation.
 * \retval true    AVRCP connection confirmation was sent successfully.
 * \retval false   AVRCP connection confirmation was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP browsing connection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP connection request.
 * \retval true    AVRCP connection request was sent successfully.
 * \retval false   AVRCP connection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_connect_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP browsing disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP disconnection request.
 * \retval true    AVRCP disconnection request was sent successfully.
 * \retval false   AVRCP disconnection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_disconnect_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP browsing connection confirmation.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  accept     Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication
 *
 * \return         The status of sending the AVRCP browsing connection confirmation.
 * \retval true    AVRCP browsing connection confirmation was sent successfully.
 * \retval false   AVRCP browsing connection confirmation was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP volume change response after received register notification volume change command.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  vol        Relative volume at local, 0 as Min and 0x7F as Maximum.
 *
 * \return         The status of sending the AVRCP volume change response.
 * \retval true    AVRCP volume change response was sent successfully.
 * \retval false   AVRCP volume change response was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_volume_change_register_rsp(uint8_t *bd_addr, uint8_t vol);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP volume change request to remote TG device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  vol        Relative volume at local, 0 as Min and 0x7F as Maximum.
 *
 * \return         The status of sending the AVRCP volume change request.
 * \retval true    AVRCP volume change request was sent successfully.
 * \retval false   AVRCP volume change request was failed to send.
 *
 * \note           This API shall be invoked only when remote TG device has registered volume change
 *                 notification, and event \ref BT_EVENT_AVRCP_REG_VOLUME_CHANGED is received on
 *                 the local device.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_volume_change_req(uint8_t *bd_addr, uint8_t vol);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP play status change response after received register notification play status change command.
 *
 * \param[in]  bd_addr       Remote BT address.
 * \param[in]  play_status   The current status of playback.
 *
 * \return         The status of sending the AVRCP play status change response.
 * \retval true    AVRCP play status change response was sent successfully.
 * \retval false   AVRCP play status change response was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_play_status_change_register_rsp(uint8_t *bd_addr, T_BT_AVRCP_PLAY_STATUS play_status);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP play status change request to remote CT device.
 *
 * \param[in]  bd_addr       Remote BT address.
 * \param[in]  play_status   The current status of playback.
 *
 * \return         The status of sending the AVRCP play status change request.
 * \retval true    AVRCP play status change request was sent successfully.
 * \retval false   AVRCP play status change request was failed to send.
 *
 * \note           This API shall be invoked only when remote CT device has registered play status
 *                 change notification, and event \ref BT_EVENT_AVRCP_REG_PLAY_STATUS_CHANGED is
 *                 received on the local device.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_play_status_change_req(uint8_t *bd_addr, T_BT_AVRCP_PLAY_STATUS play_status);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP get element attribute request.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  attr_num   The number of attribute.
 * \param[in]  p_attr     The requested attribute list.
 *
 * \return         The status of sending the AVRCP get element attribute request.
 * \retval true    AVRCP get element attribute request was sent successfully.
 * \retval false   AVRCP get element attribute request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_get_element_attr_req(uint8_t *bd_addr, uint8_t attr_num, uint8_t *p_attr);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP get play status request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP get play status request.
 * \retval true    AVRCP get play status request was sent successfully.
 * \retval false   AVRCP get play status request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_get_play_status_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to start playling an item on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_play(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to pause an item on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_pause(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to stop an item on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_stop(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to change the media which is playing to next one on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_forward(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to change the media which is playing to last one on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_backward(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to start rewind the media which is playing on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_rewind_start(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to stop rewind the media which is playing on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_rewind_stop(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to start fast forward the media which is playing on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_fast_forward_start(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command that simulate keystrokes to stop fast forward the media which is playing on remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_fast_forward_stop(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Request TG to provide target supported player application setting attributes.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_app_setting_attrs_list(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Request TG to list the set of possible values for the requested player application setting attribute.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  attr_id    The requested attribute id.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_app_setting_values_list(uint8_t *bd_addr, uint8_t attr_id);

/**
 * bt_avrcp.h
 *
 * \brief  Request TG to provide the current set values for the provided player application setting attributes list.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  attr_num   The number of requested attributes.
 * \param[in]  attr_list  The requested attribute ID list.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_app_setting_value_get(uint8_t *bd_addr, uint8_t attr_num, uint8_t *attr_list);

/**
 * bt_avrcp.h
 *
 * \brief  Set the player application setting list of player application setting values on TG.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  attr_num        The number of attributes.
 * \param[in]  attr_list       The attribute ID/value pair list.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_app_setting_value_set(uint8_t *bd_addr, uint8_t attr_num, uint8_t *attr_list);

/**
 * bt_avrcp.h
 *
 * \brief  Request for continuing response packets for the sent PDU command that has not completed.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_continuing_rsp_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command to abort continuing response.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_continuing_rsp_abort(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief  Set an absolute volume to be used by the rendering device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  volume     Absolute Volume.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_absolute_volume_set(uint8_t *bd_addr, uint8_t volume);

/**
 * bt_avrcp.h
 *
 * \brief  Inform the TG of which media player the CT wishes to control.
 *
 * \param[in]  bd_addr      Remote BT address.
 * \param[in]  player_id    Unique Media Player Id.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_addressed_player_set(uint8_t *bd_addr, uint16_t player_id);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command to start playing an item indicated by the UID.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  scope          The scope in which the UID of the media element item or folder item.
 * \param[in]  uid            The UID of the media element item or folder item.
 * \param[in]  uid_counter    UID Counter.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_item_play(uint8_t *bd_addr, uint8_t scope, uint64_t uid, uint16_t uid_counter);

/**
 * bt_avrcp.h
 *
 * \brief  Move to next or previous groups on TG.
 *
 * \param[in]  bd_addr          Remote BT address.
 * \param[in]  next             Next or previous.
 * \arg    true          Next group.
 * \arg    false         Previous group.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_group_navigate(uint8_t *bd_addr, bool next);

/**
 * bt_avrcp.h
 *
 * \brief  Send a vendor specific command to the peer entity.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  ctype          The command type.
 * \param[in]  company_id     24-bit unique ID obtained from IEEE RAC.
 * \param[in]  p_pdu          The command data.
 * \param[in]  pdu_length     Length of the command data.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_vendor_cmd_send(uint8_t *bd_addr, T_BT_AVRCP_CTYPE ctype, uint32_t company_id,
                              uint8_t *p_pdu, uint16_t pdu_length);

/**
 * bt_avrcp.h
 *
 * \brief  Send a vendor specific response to the peer entity.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  response       The response code.
 * \param[in]  company_id     24-bit unique ID obtained from IEEE RAC.
 * \param[in]  p_pdu          The reponse data.
 * \param[in]  pdu_length     Length of the response data.
 *
 * \return         The status of sending the response.
 * \retval true    Response was sent successfully.
 * \retval false   Response was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_vendor_rsp_send(uint8_t *bd_addr, T_BT_AVRCP_RESPONSE response, uint32_t company_id,
                              uint8_t *p_pdu, uint16_t pdu_length);

/**
 * bt_avrcp.h
 *
 * \brief  Send a command to retrieve a listing of the contents of a folder.
 *
 * \param[in]  bd_addr       Remote BT address.
 * \param[in]  scope         The scope.
 * \arg    0         Media Player List.
 * \arg    1         Media Player Virtual Filesystem.
 * \arg    2         Search.
 * \arg    3         Now Playing.
 * \param[in]  start_item    The offset within the listing of the item which should be the first returned item.
 * \param[in]  end_item      The offset within the listing of the item which should be the final returned item..
 * \param[in]  attr_num      The number of attribute.
 * \param[in]  p_attr        The requested attribute list.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_folder_items_get(uint8_t *bd_addr, uint8_t scope, uint32_t start_item,
                                        uint32_t end_item, uint8_t attr_num, uint8_t *p_attr);

/**
 * bt_avrcp.h
 *
 * \brief   Retrieve the metadata attributes for a particular media element item or folder item.
 *
 * \param[in]  bd_addr           Remote BT address.
 * \param[in]  scope             The scope in which the UID of the media element item or folder item is valid.
 * \param[in]  uid               The UID of the media element item or folder item.
 * \param[in]  uid_counter       UID Counter.
 * \param[in]  attr_num          The number of attribute.
 * \param[in]  p_attr            The requested attribute list.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_item_attrs_get(uint8_t *bd_addr, uint8_t scope, uint64_t uid,
                                      uint16_t uid_counter, uint8_t attr_num, uint8_t *p_attr);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP search request.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  length         Length of search string.
 * \param[in]  p_search_str   The search string.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_search(uint8_t *bd_addr, uint16_t length, uint8_t *p_search_str);

/**
 * bt_avrcp.h
 *
 * \brief   Send a command to control to which player browsing commands should be routed.
 *
 * \param[in]  bd_addr     Remote BT address.
 * \param[in]  player_id   Unique Media Player Id.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_browsed_player_set(uint8_t *bd_addr, uint16_t player_id);

/**
 * bt_avrcp.h
 *
 * \brief   Send a command to navigate one level up or down in the virtual filesystem.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  uid_counter     UID Counter.
 * \param[in]  direction       Direction.
 * \arg    0         Folder Up.
 * \arg    1         Folder Down.
 * \param[in]  folder_uid      The UID of the folder to navigate to.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_browsing_path_change(uint8_t *bd_addr, uint16_t uid_counter,
                                   uint8_t direction, uint64_t folder_uid);

/**
 * bt_avrcp.h
 *
 * \brief   Send an AVRCP cover art connection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  l2c_psm    L2CAP PSM.
 *
 * \return         The status of sending the AVRCP cover art connection request.
 * \retval true    AVRCP cover art connection request was sent successfully.
 * \retval false   AVRCP cover art connection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_cover_art_connect_req(uint8_t *bd_addr, uint16_t l2c_psm);

/**
 * bt_avrcp.h
 *
 * \brief  Send an AVRCP cover art disconnec request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the AVRCP cover art disconnec request.
 * \retval true    AVRCP cover art disconnection request was sent successfully.
 * \retval false   AVRCP cover art disconnection request was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_cover_art_disconnect_req(uint8_t *bd_addr);

/**
 * bt_avrcp.h
 *
 * \brief   Get cover art image.
 *
 * \param[in]  bd_addr         Remote BT address.
 * \param[in]  image_handle    Image handle, NULL terminated UTF-16 string.
 *
 * \return         The status of sending the command.
 * \retval true    Command was sent successfully.
 * \retval false   Command was failed to send.
 *
 * \ingroup BT_AVRCP
 */
bool bt_avrcp_cover_art_get(uint8_t *bd_addr, uint8_t image_handle[16]);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_AVRCP_H_ */
