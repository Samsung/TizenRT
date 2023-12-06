/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_IAP_H_
#define _BT_IAP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup   BT_IAP BT iAP Profile
 *
 * \brief Provide BT iAP profile interfaces.
 */


/**
 * bt_iap.h
 *
 * \defgroup BT_IAP_EXTERNAL_PROTOCOL BT iAP External Protocol
 *
 * \brief Define BT iAP external protocol
 *
 * \ingroup BT_IAP
 * @{
 */
#define BT_IAP_EXT_ACC_PROTOCOL_ID             0
#define BT_IAP_EXT_ACC_PROTOCOL_NAME           1
#define BT_IAP_EXT_ACC_PROTOCOL_MATCHACTION    2
/**
 * @}
 */

/**
 * bt_iap.h
 *
 * \defgroup BT_IAP_MESSAGE BT iAP Message
 *
 * \brief Define BT iAP message
 *
 * \ingroup BT_IAP
 * @{
 */
#define BT_IAP_MSG_START_EA_PROTOCOL_SESSION             0xEA00
#define BT_IAP_MSG_STOP_EA_PROTOCOL_SESSION              0xEA01
#define BT_IAP_MSG_REQUEST_APP_LAUNCH                    0xEA02
#define BT_IAP_MSG_EA_PROTOCOL_SESSION_STATUS            0xEA03

/**
 * @}
 */


/**
 * bt_iap.h
 *
 * \brief Define BT iAP identification information id.
 *
 * \ingroup BT_IAP
 */
typedef enum t_bt_iap_identification_info_id
{
    BT_IAP_ID_NAME                                  = 0x00,
    BT_IAP_ID_MODEL_IDENTIFIER                      = 0x01,
    BT_IAP_ID_MANUFACTURER                          = 0x02,
    BT_IAP_ID_SERIAL_NUMBER                         = 0x03,
    BT_IAP_ID_FIRMWARE_VERSION                      = 0x04,

    BT_IAP_ID_HARDWARE_VERSION                      = 0x05,
    BT_IAP_ID_MESSAGES_SENT_BY_ACCESSORY            = 0x06,
    BT_IAP_ID_MESSAGES_RECEIVED_FROM_DEVICE         = 0x07,
    BT_IAP_ID_POWER_SOURCETYPE                      = 0x08,
    BT_IAP_ID_MAXIMUM_CURRENT_DRAWN_FROM_DEVICE     = 0x09,

    BT_IAP_ID_SUPPORTED_EXTERNAL_ACCESSORY_PROTOCOL = 0x0a,
    BT_IAP_ID_PREFERRED_APP_MATCH_TEAM_ID           = 0x0b,
    BT_IAP_ID_CURRENT_LANGUAGE                      = 0x0c,
    BT_IAP_ID_SUPPORTED_LANGUAGE                    = 0x0d,
    BT_IAP_ID_SERIAL_TRANSPORT_COMPONENT            = 0x0e,

    BT_IAP_ID_USB_DEVICE_TRANSPORT_COMPONENT        = 0x0f,
    BT_IAP_ID_USB_HOST_TRANSPORT_COMPONENT          = 0x10,
    BT_IAP_ID_BLUETOOTH_TRANSPORT_COMPONENT         = 0x11,
    BT_IAP_ID_IAP2_HID_COMPONENT                    = 0x12,
    BT_IAP_ID_USB_HOST_HID_COMPONENT                = 0x13,
    BT_IAP_ID_PRODUCT_PLAN_UID                      = 34,
} T_BT_IAP_IDENTIFICATION_INFO_ID;

/**
 * bt_iap.h
 *
 * \brief Define BT iAP transport.
 *
 * \ingroup BT_IAP
 */
typedef enum t_bt_iap_transport
{
    BT_IAP_TRANSPORT_COMPONENT_IDENTIFIER                   = 0x00,
    BT_IAP_TRANSPORT_COMPONENT_NAME                         = 0x01,
    BT_IAP_TRANSPORT_SUPPORTS_IAP2_CONNECTION               = 0x02,
    BT_IAP_BLUETOOTH_TRANSPORT_MEDIA_ACCESS_CONTROL_ADDRESS = 0x03,
} T_BT_IAP_TRANSPORT;

/**
 * bt_iap.h
 *
 * \brief Define BT iAP External Accessory Protocol session status.
 *
 * \ingroup BT_IAP
 */
typedef enum t_bt_iap_eap_session_status
{
    BT_IAP_EAP_SESSION_STATUS_OK     = 0x00,
    BT_IAP_EAP_SESSION_STATUS_CLOSED = 0x01,
} T_BT_IAP_EAP_SESSION_STATUS;

/**
 * bt_iap.h
 *
 * \brief Define BT iAP app launch method.
 *
 * \ingroup BT_IAP
 */
typedef enum t_bt_iap_app_launch_method
{
    BT_IAP_APP_LAUNCH_WITH_USER_ALERT    = 0x00,
    BT_IAP_APP_LAUNCH_WITHOUT_USER_ALERT = 0x01,
} T_BT_IAP_APP_LAUNCH_METHOD;

/**
 * bt_iap.h
 *
 * \brief Define BT iAP parameter type
 *
 * \ingroup BT_IAP
 */
typedef enum t_bt_iap_param_type
{
    BT_IAP_PARAM_ACC_MAX_PKT_LEN         = 0x00,
    BT_IAP_PARAM_ACC_RETRANS_TOUT        = 0x01,
    BT_IAP_PARAM_ACC_CUMULATIVE_ACK_TOUT = 0x02,
    BT_IAP_PARAM_ACC_MAX_OUT_PKT_NUM     = 0x03,
    BT_IAP_PARAM_ACC_MAX_RETRANS_NUM     = 0x04,
    BT_IAP_PARAM_ACC_MAX_CULUMATIVE_ACK  = 0x05,

    BT_IAP_PARAM_TX_Q_ELEM_NUM           = 0x80,
} T_BT_IAP_PARAM_TYPE;

typedef bool (*P_CP_WRITE_FUNC)(uint8_t start_addr, uint8_t *data, uint16_t len);
typedef bool (*P_CP_READ_FUNC)(uint8_t start_addr, uint8_t *data, uint16_t len);

/**
 * bt_iap.h
 *
 * \brief  Initialize iAP profile.
 *
 * \param[in]  link_num            iAP maximum connected link number.
 * \param[in]  rfc_iap_chann_num   RFCOMM channel num used for iAP.
 * \param[in]  p_cp_write          The cp write func.
 * \param[in]  p_cp_read           The cp burst read func.
 *
 * \return          The status of initializing iAP profile.
 * \retval true     iAP profile was initialized successfully.
 * \retval false    iAP profile was failed to initialize.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_init(uint8_t link_num, uint8_t rfc_iap_chann_num,
                 P_CP_WRITE_FUNC p_cp_write, P_CP_READ_FUNC p_cp_read);

/**
 * bt_iap.h
 *
 * \brief  Send iAP data to remote device.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  session_id     The session identifier used for sending data.
 * \param[in]  p_data         The start address of the data buffer.
 * \param[in]  data_len       The length of the data.
 *
 * \return         The status of sending data.
 * \retval true    Data was sent successfully.
 * \retval false   Data was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_data_send(uint8_t *bd_addr, uint16_t session_id, uint8_t *p_data, uint16_t data_len);

/**
 * bt_iap.h
 *
 * \brief  Send External Accessory Protocol session status.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  session_id         The External Accessory Protocol session identifier.
 * \param[in]  status             The External Accessory Protocol session status.
 *
 * \return         The result of sending External Accessory Protocol session status.
 * \retval true    External Accessory Protocol session status was sent successfully.
 * \retval false   External Accessory Protocol session status was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_eap_session_status_send(uint8_t *bd_addr, uint16_t session_id,
                                    T_BT_IAP_EAP_SESSION_STATUS status);

/**
 * bt_iap.h
 *
 * \brief  Send a request to launch specific app which has been installed on the apple device.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  boundle_id         The start address of the AppBundleID buffer.
 * \param[in]  len_boundle_id     The length of the AppBundleID.
 * \param[in]  method             APP launch method \ref T_BT_IAP_APP_LAUNCH_METHOD.
 *
 * \return         The status of launching app request.
 * \retval true    Request was sent successfully.
 * \retval false   Request was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_app_launch(uint8_t *bd_addr, char *boundle_id, uint8_t len_boundle_id,
                       T_BT_IAP_APP_LAUNCH_METHOD method);

/**
 * bt_iap.h
 *
 * \brief  Send an iAP connection request.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  server_chann       The remote server_channel which can be found from the sdp info.
 * \param[in]  frame_size         The max frame_size supported by local device.
 * \param[in]  init_credits       The number of packet that remote can be send. This para is used for flow control.A sending
 *                                entity may send as many frames on an iAP connection as it has credits;if the credit count
 *                                reaches zero,the sender will stop and wait for further credits from peer.
 *
 * \return         The status of sending the iAP connection request.
 * \retval true    iAP connection request was sent successfully.
 * \retval false   iAP connection request was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_connect_req(uint8_t *bd_addr, uint8_t server_chann, uint16_t frame_size,
                        uint8_t init_credits);

/**
 * bt_iap.h
 *
 * \brief  Send an iAP connection confirmation.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  accept             Confirmation message.
 * \arg    true    Accept the received iAP connection request.
 * \arg    false   Reject the received iAP connection request.
 * \param[in]  frame_size         The max frame_size supported by local device.
 * \param[in]  init_credits       The number of packet that remote can be send. This para is used for flow control.A sending
 *                                entity may send as many frames on an iAP connection as it has credits;if the credit count
 *                                reaches zero,the sender will stop and wait for further credits from peer.
 *
 * \return         The status of sending the iAP connection confirmation.
 * \retval true    iAP connection confirmation was sent successfully.
 * \retval false   iAP connection confirmation was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_connect_cfm(uint8_t *bd_addr, bool accept, uint16_t frame_size, uint8_t init_credits);

/**
 * bt_iap.h
 *
 * \brief  Send an iAP disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the iAP disconnection request.
 * \retval true    iAP disconnection request was sent successfully.
 * \retval false   iAP disconnection request was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_disconnect_req(uint8_t *bd_addr);

/**
 * bt_iap.h
 *
 * \brief  Format the parameter and store in the buffer to be send later.
 *
 * \param[in]  p_param        The start address to store the formatted parameter.
 * \param[in]  param_id       The parameter id \ref BT_IAP_EXTERNAL_PROTOCOL, \ref T_BT_IAP_IDENTIFICATION_INFO_ID, \ref T_BT_IAP_TRANSPORT.
 * \param[in]  p_data         The parameter data to be formatted.
 * \param[in]  data_len       Length of the parameter data.
 *
 * \return uint8_t *      The next address of the buffer which is immediately following the end address of the formatted param.
 *
 * \ingroup BT_IAP
 */
uint8_t *bt_iap2_prep_param(uint8_t *p_param, uint16_t param_id, void *p_data, uint16_t data_len);

/**
 * bt_iap.h
 *
 * \brief  Send identification information.
 *
 * \param[in]  bd_addr        Remote BT address.
 * \param[in]  p_ident        The start address of the identification information buffer.
 * \param[in]  ident_len      The length of the identification information.
 *
 * \return         The status of sending identification information.
 * \retval true    Data was sent successfully.
 * \retval false   Data was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_ident_info_send(uint8_t *bd_addr, uint8_t *p_ident, uint16_t ident_len);

/**
 * bt_iap.h
 *
 * \brief  Set local iAP parameter.
 *
 * \param[in]  type        Parameter type.
 * \param[in]  len         The length of parameter value.
 * \param[in]  p_value     The start address of the parameter value buffer.
 *
 * \return         The result of setting local iAP parameter.
 * \retval true    The parameter was set successfully.
 * \retval false   The parameter was failed to set.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_param_set(T_BT_IAP_PARAM_TYPE type, uint8_t len, void *p_value);

/**
 * bt_iap.h
 *
 * \brief  Send an acknowledge for a specific received packet.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  ack_seq    The seq number which can be found from the packet to be acknowledged.
 *
 * \return         The result of sending acknowledge.
 * \retval true    The acknowledge was sent successfully.
 * \retval false   The acknowledge was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_ack_send(uint8_t *bd_addr, uint8_t ack_seq);

/**
 * bt_iap.h
 *
 * \brief  Send bluetooth component information to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  comp_id    The Component identifier.
 * \param[in]  enable     True if the Bluetooth component is ready for connections to the device.
 *
 * \return         The result of sending component information.
 * \retval true    The component information was sent successfully.
 * \retval false   The component information was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_comp_info_send(uint8_t *bd_addr, uint16_t comp_id, bool enable);

/**
 * bt_iap.h
 *
 * \brief  Send bluetooth connection update start message to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  comp_id    The Component identifier.
 *
 * \return         The result of sending bluetooth connection update start message.
 * \retval true    The message was sent successfully.
 * \retval false   The message was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_conn_update_start(uint8_t *bd_addr, uint16_t comp_id);

/**
 * bt_iap.h
 *
 * \brief  Send communications updates start message to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  param_id   The communications sub-features identifier.
 *
 * \return         The result of sending communications updates start message.
 * \retval true    The message was sent successfully.
 * \retval false   The message was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_comm_update_start(uint8_t *bd_addr, uint16_t param_id);

/**
 * bt_iap.h
 *
 * \brief  Send mute status message to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  status     The current mute status of local device.
 *
 * \return         The result of sending mute status message.
 * \retval true    The message was sent successfully.
 * \retval false   The message was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_mute_status_update(uint8_t *bd_addr, bool status);

/**
 * bt_iap.h
 *
 * \brief  Send iAP message to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  msg_type   The message type.
 * \param[in]  p_param    The start address of the param buffer.
 * \param[in]  param_len  The param length.
 *
 * \return         The result of sending iAP message.
 * \retval true    The message was sent successfully.
 * \retval false   The message was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_send_cmd(uint8_t *bd_addr, uint16_t msg_type, uint8_t *p_param, uint16_t param_len);

/**
 * bt_iap.h
 *
 * \brief  Send a request to start using a HID report descriptor over iAP control session.
 *
 * \param[in]  bd_addr                Remote BT address.
 * \param[in]  hid_component_id       HID component identifier.
 * \param[in]  vid                    HID vender identifier.
 * \param[in]  pid                    HID product identifier.
 * \param[in]  hid_report_desc        The start address of report descriptor buffer.
 * \param[in]  hid_report_desc_len    The length of report descriptor.
 *
 * \return         The status of sending request.
 * \retval true    The request was sent successfully.
 * \retval false   The request was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_hid_start(uint8_t *bd_addr, uint16_t hid_component_id, uint16_t vid, uint16_t pid,
                      uint8_t *hid_report_desc, uint16_t hid_report_desc_len);

/**
 * bt_iap.h
 *
 * \brief  Send an HID report message.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  hid_component_id   HID component identifier.
 * \param[in]  hid_report         The start address of report data buffer.
 * \param[in]  hid_report_len     The length of report data.
 *
 * \return         The status of sending report.
 * \retval true    The report data was sent successfully.
 * \retval false   The report data was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_hid_report_send(uint8_t *bd_addr, uint16_t hid_component_id,
                            uint8_t *hid_report, uint16_t hid_report_len);

/**
 * bt_iap.h
 *
 * \brief  Send a request to stop using a HID.
 *
 * \param[in]  bd_addr            Remote BT address.
 * \param[in]  hid_component_id   HID component identifier.
 *
 * \return         The status of sending request.
 * \retval true    The request was sent successfully.
 * \retval false   The request was failed to send.
 *
 * \ingroup BT_IAP
 */
bool bt_iap_hid_stop(uint8_t *bd_addr, uint16_t hid_component_id);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_IAP_H_ */
