/*
 * Copyright (c) 2017, Realtek Semiconductor Corporation. All rights reserved.
 */

#ifndef __GAP_STORAGE_LEGACY_H__
#define __GAP_STORAGE_LEGACY_H__

#include "platform_opts_bt.h"

#if UPPER_STACK_VERSION == VERSION_2021

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include "upperstack_config.h"

/**
 * \defgroup    GAP_BREDR_STORAGE      GAP BR/EDR Storage
 *
 * \brief   Provide BR/EDR key storage related funtions.
 *
 */

/**
 * gap_storage_legacy.h
 *
 * \brief   Get link key and key type information for a particular address.
 *
 * \param[in]   bd_addr     Bluetooth address of the device.
 * \param[out]  link_key    Address to put in the link key.
 * \param[out]  key_type    Address to put in key type.
 *
 * \return      Result of getting link key and key type of the address.
 * \retval    true          Get link key and key type successfully.
 * \retval    false         Failed to get link key and key type.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       uint8_t link_key[16];
       uint8_t key_type;

       legacy_get_bond(bd_addr, link_key, &key_type);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_get_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t *key_type);

/**
 * gap_storage_legacy.h
 *
 * \brief   Save link key and key type information for a particular address.
 *
 * \param[in]  bd_addr     Bluetooth address of the device.
 * \param[in]  link_key    Address of link key.
 * \param[in]  key_type    Link key type.
 *
 * \return    Result of saving link key and key type of the address.
 * \retval    true          Save link key and key type successfully.
 * \retval    false         Failed to save link key and key type.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       uint8_t link_key[16] = {0xAA, 0x00};
       uint8_t key_type = 0x04;

       legacy_save_bond(bd_addr, link_key, key_type);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_save_bond(uint8_t *bd_addr, uint8_t *link_key, uint8_t key_type);

/**
 * gap_storage_legacy.h
 *
 * \brief   Delete bond record for a particular address.
 *
 * \param[in]   bd_addr     Bluetooth address of the device whose bond record will be deleted.
 *
 * \return      Result of deleting bond record of the address.
 * \retval    true          Delete bond record successfully.
 * \retval    false         Failed to delete bond record.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       legacy_delete_bond(addr);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_delete_bond(uint8_t *bd_addr);

/**
 * gap_storage_legacy.h
 *
 * \brief   Clear all stored BR/EDR keys.
 *
 * \param[in]   void.
 *
 * \return      void.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       legacy_clear_all_keys();
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
void legacy_clear_all_keys(void);

/**
 * gap_storage_legacy.h
 *
 * \brief   Get index of a particular address in the paired list.
 *
 * \param[in]   bd_addr     Bluetooth address of the device.
 * \param[out]  p_idx       Address to store paired index.
 *
 * \return      Result of getting paired index of the address.
 * \retval    true          Get paired index successfully.
 * \retval    false         Failed to get paired index.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       uint8_t paired_index;

       legacy_get_paired_idx(bd_addr, &paired_index);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_get_paired_idx(uint8_t *bd_addr, uint8_t *p_idx);

/**
 * gap_storage_legacy.h
 *
 * \brief   Set bond flag of a particular address.
 *
 * \param[in]   bd_addr     Bluetooth address of the device.
 * \param[in]   bond_flag   Bond flag value to be set.
 *
 * \return      Result of setting bond flag.
 * \retval    true          Set bond flag successfully.
 * \retval    false         Failed to set bond flag.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       uint8_t bond_flag = 0x85;

       legacy_set_bond_flag(bd_addr, bond_flag);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_set_bond_flag(uint8_t *bd_addr, uint32_t bond_flag);

/**
 * gap_storage_legacy.h
 *
 * \brief   Get bond flag stored in FTL for a particular address.
 *
 * \param[in]   bd_addr     Bluetooth address of the device.
 * \param[out]  bond_flag   Bond flag of the address.
 *
 * \return      Result of getting bond flag.
 * \retval    true          Get bond flag successfully.
 * \retval    false         Failed to get bond flag.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
       uint8_t bond_flag;

       legacy_get_bond_flag(bd_addr, &bond_flag);
   }
   \endcode
 *
 * \ingroup  GAP_LEGACY_STORAGE
 */
bool legacy_get_bond_flag(uint8_t *bd_addr, uint32_t *bond_flag);

/**
 * gap_storage_legacy.h
 *
 * \brief   Get number of bonded devices stored in FTL.
 *
 * \return      Number of bonded devices stored in FTL.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t bond_num;

       bond_num = legacy_get_bond_num();
   }
   \endcode
 *
 * \ingroup  GAP_LEGACY_STORAGE
 */
uint8_t legacy_get_bond_num(void);

/**
 * gap_storage_legacy.h
 *
 * \brief   Get max number of bonded devices can be stored in FTL.
 *
 * \return      Max number of bonded devices can be stored in FTL.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t max_bond_num;

       max_bond_num = legacy_get_max_bond_num();
   }
   \endcode
 *
 * \ingroup  GAP_LEGACY_STORAGE
 */
uint8_t legacy_get_max_bond_num(void);

/**
 * gap_storage_legacy.h
 *
 * \brief   Get address of a bond device in FTL by priority. Priority 1 means the highest priority.
 *
 * \param[in]   priority    Priority of device to be found.
 * \param[out]  bd_addr     Address to store bluetooth address of the device.
 *
 * \return      Result of getting address by priority.
 * \retval    true          Get address of the given priority successfully.
 * \retval    false         Failed to get address of the priority.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6];

       legacy_get_addr_by_priority(1, addr);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_get_addr_by_priority(uint8_t priority, uint8_t *bd_addr);

/**
 * gap_storage_legacy.h
 *
 * \brief   Change the priority of the given address to the highest one.
 *
 * \param[in]   bd_addr     Bluetooth address of the device.
 *
 * \return      Result of changing the priority of the given address.
 * \retval    true          Change the priority successfully.
 * \retval    false         Failed to change the priority of the address.
 *
 * <b>Example usage</b>
   \code{.c}
   int test(void)
   {
       uint8_t addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

       legacy_adjust_dev_priority(addr);
   }
   \endcode
 *
 * \ingroup  GAP_BREDR_STORAGE
 */
bool legacy_adjust_dev_priority(uint8_t *bd_addr);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _GAP_STORAGE_LEGACY_H_ */
