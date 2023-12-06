/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_HID_H_
#define _BT_HID_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_HID BT HID Profile
 *
 * \brief   Provide BT HID profile interfaces.
 */

/**
 * bt_hid.h
 *
 * \brief  BT HID protocol mode.
 *
 * \ingroup BT_HID
 */
typedef enum t_bt_hid_protocol_mode
{
    BT_HID_BOOT_PROTO_MODE             = 0x00,
    BT_HID_REPORT_PROTO_MODE           = 0x01,
} T_BT_HID_PROTOCOL_MODE;

/**
 * bt_hid.h
 *
 * \brief  BT HID report type.
 *
 * \ingroup BT_HID
 */
typedef enum t_bt_hid_report_type
{
    BT_HID_REPORT_TYPE_RESERVED          = 0x00,
    BT_HID_REPORT_TYPE_INPUT             = 0x01,
    BT_HID_REPORT_TYPE_OUTPUT            = 0x02,
    BT_HID_REPORT_TYPE_FEATURE           = 0x03,
} T_BT_HID_REPORT_TYPE;

/**
 * bt_hid.h
 *
 * \brief  Initialize HID profile.
 *
 * \param[in] link_num          HID maximum connected link number.
 * \param[in] boot_proto_mode   Support boot protocol mode.
 *
 * \return          The status of initializing HID profile.
 * \retval true     HID profile was initialized successfully.
 * \retval false    HID profile was failed to initialize.
 *
 * \ingroup BT_HID
 */
bool bt_hid_init(uint8_t link_num, bool boot_proto_mode);

/**
 * bt_hid.h
 *
 * \brief  Set HID descriptor.
 *
 * \param[in] descriptor          HID report descriptor.
 * \param[in] len                 The length of HID report descriptor.
 *
 * \return          The status of set HID descriptor.
 * \retval true     HID descriptor was set successfully.
 * \retval false    HID descriptor was failed to set.
 *
 * \ingroup BT_HID
 */
bool bt_hid_descriptor_set(const uint8_t *descriptor, uint16_t len);

/**
 * \brief  Send HID connection request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the HID connection request.
 * \retval true     HID connection request was sent successfully.
 * \retval false    HID connection request was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_connect_req(uint8_t *bd_addr);

/**
 * \brief  Send HID connection confirmation.
 *
 * \param[in] bd_addr   Remote BT address.
 * \param[in] accept    Accept or reject the connection indication.
 * \arg    true     Accept the connection indication.
 * \arg    false    Reject the connection indication.
 *
 * \return          The status of sending the HID connection confirmation.
 * \retval true     HID connection confirmation was sent successfully.
 * \retval false    HID connection confirmation was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_connect_cfm(uint8_t *bd_addr, bool accept);

/**
 * \brief  Send HID disconnection request.
 *
 * \param[in] bd_addr   Remote BT address.
 *
 * \return          The status of sending the HID disconnection request.
 * \retval true     HID disconnection request was sent successfully.
 * \retval false    HID disconnection request was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_disconnect_req(uint8_t *bd_addr);

/**
 * bt_hid.h
 *
 * \brief  Send HID get report response message over control channel.
 *
 * \param[in] bd_addr           Remote BT address.
 * \param[in] report_type       Report Type \ref T_BT_HID_REPORT_TYPE.
 * \param[in] buf               Message Buffer.
 * \param[in] len               Message length.
 *
 * \return          The status of sending HID get report response message.
 * \retval true     HID get report response message was send successfully.
 * \retval false    HID get report response message was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_control_get_report_rsp(uint8_t               *bd_addr,
                                   T_BT_HID_REPORT_TYPE   report_type,
                                   uint8_t               *buf,
                                   uint16_t               len);

/**
 * bt_hid.h
 *
 * \brief  Send HID get protocol response message over control channel.
 *
 * \param[in] bd_addr           Remote BT address.
 * \param[in] proto_mode        Protocol Mode \ref T_BT_HID_PROTOCOL_MODE.
 *
 * \return          The status of sending HID get protocol response message.
 * \retval true     HID get protocol response message was send successfully.
 * \retval false    HID get protocol response message was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_control_get_protocol_rsp(uint8_t                 *bd_addr,
                                     T_BT_HID_PROTOCOL_MODE   proto_mode);

/**
 * bt_hid.h
 *
 * \brief  Send HID message over interrupt channel.
 *
 * \param[in] bd_addr           Remote BT address.
 * \param[in] report_type       Report Type \ref T_BT_HID_REPORT_TYPE.
 * \param[in] buf               Message Buffer.
 * \param[in] len               Message length.
 *
 * \return          The status of sending HID message over interrupt channel.
 * \retval true     HID message was send successfully.
 * \retval false    HID message was failed to send.
 *
 * \ingroup BT_HID
 */
bool bt_hid_interrupt_data_send(uint8_t               *bd_addr,
                                T_BT_HID_REPORT_TYPE   report_type,
                                uint8_t               *buf,
                                uint16_t               len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_HID_H_ */
