/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_ATT_H_
#define _BT_ATT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup  BT_ATT BT ATT Profile
 *
 * \brief Provide BT ATT profile interfaces.
 */

/**
 * bt_att.h
 *
 * \brief  BT ATT message type.
 *
 * \ingroup BT_ATT
 */
typedef enum t_bt_att_msg_type
{
    BT_ATT_MSG_CONN_CMPL       = 0x00,      /**< ATT connect successful */
    BT_ATT_MSG_CONN_FAIL       = 0x01,      /**< ATT connect fail */
    BT_ATT_MSG_DISCONN_CMPL    = 0x02,      /**< ATT disconnect successful */
    BT_ATT_MSG_DATA_IND        = 0x03,      /**< ATT data indicate */
} T_BT_ATT_MSG_TYPE;

/**
 * bt_att.h
 *
 * \brief  ATT message callback definition.
 *
 * \xrefitem Added_API_2_11_1_0 "Added Since 2.11.1.0" "Added API"
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  msg_type   ATT message type.
 * \param[in]  p_msg      Message buffer address.
 *
 * \ingroup BT_ATT
 */
typedef void (* P_BT_ATT_CBACK)(uint8_t *bd_addr, T_BT_ATT_MSG_TYPE msg_type, void *p_msg);

/**
 * bt_att.h
 *
 * \brief  Initialize ATT profile.
 *
 * \xrefitem Added_API_2_11_1_0 "Added Since 2.11.1.0" "Added API"
 *
 * \param[in] cback  Callback function used to handle ATT message.
 *
 * \return          The status of initializing ATT profile.
 * \retval true     ATT profile was initialized successfully.
 * \retval false    ATT profile was failed to initialize.
 *
 * \ingroup BT_ATT
 */
bool bt_att_init(P_BT_ATT_CBACK cback);

/**
 * bt_att.h
 *
 * \brief   Send a ATT connection request.
 *
 * \xrefitem Added_API_2_11_1_0 "Added Since 2.11.1.0" "Added API"
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the ATT connection request.
 * \retval true    ATT connection request was sent successfully.
 * \retval false   ATT connection request was failed to send.
 *
 * \ingroup BT_ATT
 */
bool bt_att_connect_req(uint8_t *bd_addr);

/**
 * bt_att.h
 *
 * \brief  Send a ATT disconnection request.
 *
 * \xrefitem Added_API_2_11_1_0 "Added Since 2.11.1.0" "Added API"
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the ATT disconnection request.
 * \retval true    ATT disconnection request was sent successfully.
 * \retval false   ATT disconnection request was failed to send.
 *
 * \ingroup BT_ATT
 */
bool bt_att_disconnect_req(uint8_t *bd_addr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_ATT_H_ */
