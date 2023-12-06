/*
 * Copyright (c) 2018, Realsil Semiconductor Corporation. All rights reserved.
 */

#ifndef _BT_BOND_MGR_
#define _BT_BOND_MGR_

#ifdef  __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

/**
 * \defgroup    LEA_LIB BLE Audio Lib
 *
 * \brief   General interface accessing to LE Audio Lib.
 */

/**
 * \defgroup    LEA_LIB_Exported_Types BLE Audio Lib Exported Types
 *
 * \ingroup LEA_LIB
 * \{
 */

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Check Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_CHECK)(uint8_t *bd_addr, uint8_t bd_type, uint16_t conn_handle);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Get Key Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_GET_KEY)(uint8_t *bd_addr, uint8_t bd_type, uint16_t conn_handle,
                                  bool remote, uint8_t *p_key_len, uint8_t *p_key);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Get Max Bond Number Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef uint8_t (*P_BT_BOND_GET_MAX_NUM)(bool is_le);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Get Address Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_GET_ADDR)(bool is_le, uint8_t bond_idx, uint8_t *bd_addr,
                                   uint8_t *p_bd_type, uint8_t *local_bd_addr,
                                   uint8_t *p_local_bd_type);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Set CCCD Flag Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_SET_CCCD_FLAG)(uint8_t *bd_addr, uint8_t bd_type, uint8_t *local_bd_addr,
                                        uint8_t local_bd_type,
                                        uint16_t cccd_handle, uint16_t flags);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Clear CCCD Flag Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_CLEAR_CCCD_FLAG)(uint8_t *bd_addr, uint8_t bd_type, uint16_t conn_handle,
                                          uint16_t cccd_handle);

/**
 * bt_bond_mgr.h
 *
 * \brief  Define Bluetooth Bond Resolve Resolvable Private Address Function Type.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef bool (*P_BT_BOND_LE_RESOLVE_RPA)(uint8_t *unresolved_addr, uint8_t *identity_addr,
                                         uint8_t *p_identity_addr_type);

/**
 * bt_bond_mgr.h
 *
 * \brief  Bluetooth bond manager types.
 *
 * \ingroup LEA_LIB_Exported_Types
 */
typedef struct
{
    P_BT_BOND_CHECK             bond_check;
    P_BT_BOND_GET_KEY           bond_get_key;
    P_BT_BOND_GET_MAX_NUM       bond_get_max_num;
    P_BT_BOND_GET_ADDR          bond_get_addr;
    P_BT_BOND_SET_CCCD_FLAG     bond_set_cccd_flag;
    P_BT_BOND_CLEAR_CCCD_FLAG   bond_clear_cccd_flag;
    P_BT_BOND_LE_RESOLVE_RPA    bond_le_resolve_rpa;
} T_BT_BOND_MGR;
/**
 * End of LEA_LIB_Exported_Types
 * \}
 */

#ifdef  __cplusplus
}
#endif
#endif
