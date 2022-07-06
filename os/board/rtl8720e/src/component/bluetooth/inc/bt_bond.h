/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_BOND_H_
#define _BT_BOND_H_

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup    BT_BOND BT Bond
 *
 * \brief   Manipulate BT bond information.
 */

/**
 * bt_bond.h
 *
 * \brief   Get value of supported maximum bond device num.
 *
 * \return  The value of supported maximum bond device num.
 *
 * \ingroup BT_BOND
 */
uint8_t bt_max_bond_num_get(void);

/**
 * bt_bond.h
 *
 * \brief   Get bond device num from bond storage.
 *
 * \return  The bond device num.
 *
 * \ingroup BT_BOND
 */
uint8_t bt_bond_num_get(void);

/**
 * bt_bond.h
 *
 * \brief   Get Bluetooth device address from bond storage by priority.
 *
 * \param[in]  priority      The priority of bonded device.
 * \param[out] bd_addr       The Bluetooth device address got by priority.
 *
 * \return          The status of getting Bluetooth device address.
 * \retval true     The Bluetooth device address was got successfully.
 * \retval false    The Bluetooth device addresss was failed to got.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_addr_get(uint8_t priority, uint8_t *bd_addr);

/**
 * bt_bond.h
 *
 * \brief   Get bond index from device address.
 *
 * \param[in]  bd_addr       The Bluetooth device address.
 * \param[out] index         The bond index.
 *
 * \return          The status of getting bond index.
 * \retval true     The bond index was got successfully.
 * \retval false    The bond index was failed to got.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_index_get(uint8_t *bd_addr, uint8_t *index);

/**
 * bt_bond.h
 *
 * \brief   Get bond flag with Bluetooth device address from bond storage.
 *
 * \param[in]  bd_addr       The Bluetooth device address.
 * \param[out] bond_flag    The obtained bond flag.
 *
 * \return          The status of getting bond falg.
 * \retval true     The bond flag was got successfully.
 * \retval false    The bond flag was failed to get.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_flag_get(uint8_t *bd_addr, uint32_t *bond_flag);

/**
 * bt_bond.h
 *
 * \brief   Save bond flag.
 *
 * \param[in] bd_addr       The Bluetooth device address.
 * \param[in] bond_flag     The bond flag to be saved.
 *
 * \return          The status of saving bond flag.
 * \retval true     The bond flag was saved successfully.
 * \retval false    The bond flag was failed to save.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_flag_set(uint8_t *bd_addr, uint32_t bond_flag);

/**
 * bt_bond.h
 *
 * \brief   Add bond flag.
 *
 * \param[in] bd_addr       The Bluetooth device address.
 * \param[in] bond_flag     The bond flag to be added.
 *
 * \return          The status of adding bond flag.
 * \retval true     The bond flag was added successfully.
 * \retval false    The bond flag was failed to add.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_flag_add(uint8_t *bd_addr, uint32_t bond_flag);

/**
 * bt_bond.h
 *
 * \brief   Remove bond flag.
 *
 * \param[in] bd_addr       The Bluetooth device address.
 * \param[in] bond_flag     The bond flag to be removed.
 *
 * \return          The status of removing bond flag.
 * \retval true     The bond flag was removed successfully.
 * \retval false    The bond flag was failed to remove.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_flag_remove(uint8_t *bd_addr, uint32_t bond_flag);

/**
 * bt_bond.h
 *
 * \brief   Get link key with Bluetooth device address from bond storage.
 *
 * \param[in]  bd_addr       The Bluetooth device address.
 * \param[out] link_key      The link key.
 * \param[out] key_type      The type of link key.
 *
 * \return          The status of getting link key.
 * \retval true     The link key was got successfully.
 * \retval false    The link key was failed to got.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_key_get(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type);

/**
 * bt_bond.h
 *
 * \brief   Save link key.
 *
 * \param[in] bd_addr       The Bluetooth device address.
 * \param[in] linkkey       The link key to be saved.
 * \param[in] key_type      The type of link key.
 *
 * \return          The status of saving link key.
 * \retval true     The link key was saved successfully.
 * \retval false    The link key was failed to save.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_key_set(uint8_t *bd_addr, uint8_t *linkkey, uint8_t key_type);

/**
 * bt_bond.h
 *
 * \brief   Adjust priority to highest in bond storage.
 *
 * \param[in] bd_addr       The Bluetooth device address that need to adjust priority.
 *
 * \return          The status of adjusting priority.
 * \retval true     The priority was adjusted successfully.
 * \retval false    The priority was failed to adjust.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_priority_set(uint8_t *bd_addr);

/**
 * bt_bond.h
 *
 * \brief   Delete bond information in bond storage.
 *
 * \param[in] bd_addr       The Bluetooth device address that need to delete bond information.
 *
 * \return          The status of deleting bond information.
 * \retval true     The bond information was deleted successfully.
 * \retval false    The bond information was failed to delete.
 *
 * \ingroup BT_BOND
 */
bool bt_bond_delete(uint8_t *bd_addr);

/**
 * bt_bond.h
 *
 * \brief   Delete all bond information in bond storage.
 *
 * \ingroup BT_BOND
 */
void bt_bond_clear(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

#endif /* _BT_BOND_H_ */
