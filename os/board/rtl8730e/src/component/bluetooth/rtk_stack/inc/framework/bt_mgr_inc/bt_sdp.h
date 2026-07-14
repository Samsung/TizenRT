/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_SDP_H_
#define _BT_SDP_H_

#include <stdint.h>
#include <stdbool.h>
#include "btm.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_SDP BT SDP Profile
 *
 * \brief   Provide BT SDP profile interfaces.
 */

/**
 * bt_sdp.h
 *
 * \brief   Start SDP discovery.
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 * \param[in]  uuid_type        Type of UUID to discovery.
 * \arg \c     BT_SDP_UUID16    UUID in 16 bits.
 * \arg \c     BT_SDP_UUID32    UUID in 32 bits.
 * \arg \c     BT_SDP_UUID128   UUID in 128 bits.
 * \param[in]  uuid_data        Discovery SDP UUID data corresponding to uuid_type.
 *
 * \return          The status of starting SDP discovery.
 * \retval true     SDP discovery was started successfully.
 * \retval false    SDP discovery was failed to start.
 *
 * \ingroup  BT_SDP
 */
bool bt_sdp_discov_start(uint8_t            *bd_addr,
                         T_BT_SDP_UUID_TYPE  uuid_type,
                         T_BT_SDP_UUID_DATA  uuid_data);

/**
 * bt_sdp.h
 *
 * \brief   Stop SDP discovery.
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 *
 * \return          The status of stopping SDP discovery.
 * \retval true     SDP discovery was stopped successfully.
 * \retval false    SDP discovery was failed to stop.
 *
 * \ingroup  BT_SDP
 */
bool bt_sdp_discov_stop(uint8_t *bd_addr);

/**
 * bt_sdp.h
 *
 * \brief   Start DID discovery.
 *
 * \param[in]  bd_addr          Bluetooth address of remote device.
 *
 * \return          The status of starting DID discovery.
 * \retval true     DID discovery was started successfully.
 * \retval false    DID discovery was failed to start.
 *
 * \ingroup  BT_SDP
 */

bool bt_did_discov_start(uint8_t *bd_addr);

/**
 * bt_sdp.h
 *
 * \brief   Add an SDP record to BT stack.
 *
 * \param[in]  p_buf    Address of SDP record buffer.
 *
 * \return          The status of adding an SDP record.
 * \retval true     SDP record was added successfully.
 * \retval false    SDP record was failed to add.
 *
 * \ingroup  BT_SDP
 */
bool bt_sdp_record_add(void *p_buf);

/**
 * bt_sdp.h
 *
 * \brief   Delete an SDP record from BT stack.
 *
 * \param[in]  p_buf    Address of SDP record buffer.
 *
 * \return          The status of deletinging an SDP record.
 * \retval true     SDP record was deleted successfully.
 * \retval false    SDP record was failed to delete.
 *
 * \ingroup  BT_SDP
 */
bool bt_sdp_record_delete(void *p_buf);

/**
 * bt_sdp.h
 *
 * \brief   Find an attribute of the given value in an SDP record data.
 *
 * \param[in]  buf            Buffer address of the SDP record data.
 * \param[in]  len            Length of the SDP record data.
 * \param[in]  attr_value     Attribute value to search for.
 *
 * \return          The address of the attribute.
 *
 * \ingroup  BT_SDP
 */
uint8_t *bt_sdp_attr_find(uint8_t  *buf,
                          uint16_t  len,
                          uint32_t  attr_value);

/**
 * bt_sdp.h
 *
 * \brief   Get an attribute value in an SDP element.
 *
 * \param[in]  buf            Buffer address of the SDP element.
 * \param[in]  len            Length of the SDP element.
 *
 * \return          The value of the attribute.
 *
 * \ingroup  BT_SDP
 */
uint32_t bt_sdp_value_get(uint8_t  *buf,
                          uint16_t  len);

/**
 * bt_sdp.h
 *
 * \brief   Get an UUID data value in an SDP element.
 *
 * \param[in]   buf            Buffer address of the SDP element.
 * \param[in]   len            Length of the SDP element.
 * \param[out]  p_type         The address to put in UUID data format.
 * \param[out]  p_data         The address to put in UUID data value.
 *
 * \return          The result of getting UUID value.
 *
 * \ingroup  BT_SDP
 */
bool bt_sdp_uuid_value_get(uint8_t            *buf,
                           uint16_t            len,
                           T_BT_SDP_UUID_TYPE *p_type,
                           T_BT_SDP_UUID_DATA *p_data);

/**
 * bt_sdp.h
 *
 * \brief   Get the address of SDP data element in an SDP element.
 *
 * \param[in]  buf            Buffer address of the SDP element.
 * \param[in]  len            Length of the SDP element.
 * \param[in]  index          Data element index number.
 *
 * \return          The address of found SDP data element.
 *
 * \ingroup  BT_SDP
 */
uint8_t *bt_sdp_elem_access(uint8_t *buf,
                            uint16_t len,
                            uint16_t index);

/**
 * bt_sdp.h
 *
 * \brief   Decode SDP data element in an SDP element.
 *
 * \param[in]   buf           Buffer address of the SDP element.
 * \param[in]   length        Length of the SDP element.
 * \param[out]  p_len         The address to put in UUID data length.
 * \param[out]  p_type        The address to put in UUID data format.
 *
 * \return          The address of SDP data element.
 *
 * \ingroup  BT_SDP
 */
uint8_t *bt_sdp_elem_decode(uint8_t  *buf,
                            uint16_t  length,
                            uint32_t *p_len,
                            uint8_t  *p_type);

#endif /* _BT_SDP_H_ */
