/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_SPP_H_
#define _BT_SPP_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_SPP BT SPP Profile
 *
 * \brief   Provide BT SPP profile interfaces.
 */

/**
 * bt_spp.h
 *
 * \brief    BT SPP UUID TYPE.
 *
 * \ingroup  BT_SPP
 */
typedef enum t_bt_spp_uuid_type
{
    BT_SPP_UUID16,    /**< UUID type 16 bits */
    BT_SPP_UUID32,    /**< UUID type 32 bits */
    BT_SPP_UUID128    /**< UUID type 128 bits */
} T_BT_SPP_UUID_TYPE;

/**
 * bt_spp.h
 *
 * \brief    BT SPP UUID DATA.
 *
 * \ingroup  BT_SPP
 */
typedef union t_bt_spp_uuid_data
{
    uint16_t    uuid_16;        /**< UUID data in 16 bits */
    uint32_t    uuid_32;        /**< UUID data in 32 bits */
    uint8_t     uuid_128[16];   /**< UUID data in 128 bits */
} T_BT_SPP_UUID_DATA;

/**
 * bt_spp.h
 *
 * \brief  Initialize SPP profile.
 *
 * \param[in]    link_num       Spp maximum connected link number
 * \param[in]    service_num    Supported service number.
 *
 * \return          The status of initializing SPP profile.
 * \retval true     SPP profile was initialized successfully.
 * \retval false    SPP profile was failed to initialize.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_init(uint8_t link_num, uint8_t service_num);

/**
 * bt_spp.h
 *
 * \brief  Register SPP service.
 *
 * \note   This function shall be used after \ref bt_spp_int() called.
 *
 * \param[in]   service_uuid        SPP service uuid.
 * \param[in]   server_chann        Local server channel.
 *
 * \return         The status of registering SPP service.
 * \retval true    SPP service was registered successfully.
 * \retval false   SPP service was failed to register.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_service_register(uint8_t *service_uuid, uint8_t server_chann);

/**
 * bt_spp.h
 *
 * \brief  Check the uuid is registered and bond local server channel with the checked uuid.
 *
 * \param[in]  type                    UUID type \ref T_GAP_UUID_TYPE.
 * \param[in]  data                    UUID data \ref T_GAP_UUID_DATA.
 * \param[in]  local_server_chann      Local server channel.
 *
 * \return         The status of checking uuid.
 * \retval true    The uuid was registered and the local server channel was bonded.
 * \retval false   The uuid was not registered and the local server channel was not bonded.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_registered_uuid_check(T_BT_SPP_UUID_TYPE type, T_BT_SPP_UUID_DATA *data,
                                  uint8_t *local_server_chann);

/**
 * bt_spp.h
 *
 * \brief  Set SPP ERTM mode.
 *
 * \param[in]  enable    Enable or disable ERTM mode.
 * \arg    true      Enable ERTM mode.
 * \arg    false     Disable ERTM mode.
 *
 * \return           The status of setting ERTM mode.
 * \retval true      ERTM mode was set successfully.
 * \retval false     ERTM mode was failed to set.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_ertm_mode_set(bool enable);

/**
 * bt_spp.h
 *
 * \brief  Send an SPP connection request.
 *
 * \param[in]  bd_addr             Remote BT address.
 * \param[in]  rmt_server_chann    Remote server channel.
 * \param[in]  frame_size          The max frame_size supported by local device.
 * \param[in]  credits             The number of packet that remote can be send. This para is used for flow control.A sending
 *                                 entity may send as many frames on a spp connection as it has credits;if the credit count
 *                                 reaches zero,the sender will stop and wait for further credits from peer.
 * \param[in]  local_server_chann  Local server channel.
 *
 * \return         The status of sending the SPP connection request.
 * \retval true    SPP connection request was sent successfully.
 * \retval false   SPP connection request was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_connect_req(uint8_t *bd_addr, uint8_t rmt_server_chann, uint16_t frame_size,
                        uint8_t credits, uint8_t local_server_chann);

/**
 * bt_spp.h
 *
 * \brief  Send an SPP connection confirmation.
 *
 * \param[in]  bd_addr             Remote BT address.
 * \param[in]  local_server_chann  Local server channel.
 * \param[in]  accept              Confirmation message.
 * \arg    true    Accept the received spp connection request.
 * \arg    false   Reject the received spp connection request.
 * \param[in]  frame_size         The max frame_size supported by local device.
 * \param[in]  credits            The number of packet that remote can be send. This para is used for flow control.A sending
 *                                entity may send as many frames on a spp connection as it has credits;if the credit count
 *                                reaches zero,the sender will stop and wait for further credits from peer.
 *
 * \return         The status of sending the SPP connection confirmation.
 * \retval true    SPP connection confirmation was sent successfully.
 * \retval false   SPP connection confirmation was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_connect_cfm(uint8_t *bd_addr, uint8_t local_server_chann, bool accept,
                        uint16_t frame_size, uint8_t credits);

/**
 * bt_spp.h
 *
 * \brief  Send an SPP disconnection request.
 *
 * \param[in]  bd_addr                Remote BT address.
 * \param[in]  local_server_chann     Local server channel.
 *
 * \return         The status of sending the SPP disconnection request.
 * \retval true    SPP disconnection request was sent successfully.
 * \retval false   SPP disconnection request was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_disconnect_req(uint8_t *bd_addr, uint8_t local_server_chann);

/**
 * bt_spp.h
 *
 * \brief  Send a request to disconnect all SPP connection.
 *
 * \param[in]  bd_addr    Remote BT address.
 *
 * \return         The status of sending all SPP disconnection request.
 * \retval true    All SPP disconnection request was sent successfully.
 * \retval false   All SPP disconnection request was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_disconnect_all_req(uint8_t *bd_addr);

/**
 * bt_spp.h
 *
 * \brief  Send SPP data to remote device.
 *
 * \param[in]  bd_addr                Remote BT address.
 * \param[in]  local_server_chann     Local server channel.
 * \param[in]  data                   The start address of the data buffer.
 * \param[in]  len                    The length of the data.
 * \param[in]  ack                    Enable/disable ACK after data sent.
 * \arg    true    Enable ACK after data sent.
 * \arg    false   Disable ACK after data sent.
 *
 * \return         The status of sending data.
 * \retval true    Data was sent successfully.
 * \retval false   Data was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_data_send(uint8_t *bd_addr, uint8_t local_server_chann,
                      uint8_t *data, uint16_t len, bool ack);

/**
 * bt_spp.h
 *
 * \brief  Give SPP credits to remote device.
 *
 * \param[in]  bd_addr                Remote BT address.
 * \param[in]  local_server_chann     Local server channel.
 * \param[in]  credits                Credits value.
 *
 * \return         The status of sending credits.
 * \retval true    Credits was sent successfully.
 * \retval false   Credits was failed to send.
 *
 * \ingroup BT_SPP
 */
bool bt_spp_credits_give(uint8_t *bd_addr, uint8_t local_server_chann, uint8_t credits);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _BT_SPP_H_ */
