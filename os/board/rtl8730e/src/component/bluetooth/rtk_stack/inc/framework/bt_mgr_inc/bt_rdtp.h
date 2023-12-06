/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_RDTP_H_
#define _BT_RDTP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup  BT_RDTP BT RDTP Vendor Profile
 *
 * \brief Provide BT RDTP vendor profile interfaces.
 */

/**
 * bt_rdtp.h
 *
 * \brief  Initialize RDTP profile.
 *
 * \return          The status of initializing RDTP profile.
 * \retval true     RDTP profile was initialized successfully.
 * \retval false    RDTP profile was failed to initialize.
 *
 * \ingroup BT_RDTP
 */
bool bt_rdtp_init(void);

/**
 * bt_rdtp.h
 *
 * \brief   Send a RDTP connection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the RDTP connection request.
 * \retval true    RDTP connection request was sent successfully.
 * \retval false   RDTP connection request was failed to send.
 *
 * \ingroup BT_RDTP
 */
bool bt_rdtp_connect_req(uint8_t *bd_addr);

/**
 * bt_rdtp.h
 *
 * \brief  Send a RDTP disconnection request.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending the RDTP disconnection request.
 * \retval true    RDTP disconnection request was sent successfully.
 * \retval false   RDTP disconnection request was failed to send.
 *
 * \ingroup BT_RDTP
 */
bool bt_rdtp_disconnect_req(uint8_t *bd_addr);

/**
 * bt_rdtp.h
 *
 * \brief  Send data to remote device.
 *
 * \param[in]  bd_addr    Remote BT address.
 * \param[in]  buf        The start address of the data buffer.
 * \param[in]  len        The length of the data.
 * \param[in]  flush      Flush or not.
 *
 * \return         The status of sending data to remote device.
 * \retval true    Data was sent successfully.
 * \retval false   Data was failed to send.
 *
 * \ingroup BT_RDTP
 */
bool bt_rdtp_data_send(uint8_t *bd_addr, void *buf, uint16_t len, bool flush);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_RDTP_H_ */
