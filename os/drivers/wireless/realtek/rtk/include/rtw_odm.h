/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/

#ifndef _RTW_ODM_H_
#define _RTW_ODM_H_

#include <drv_types.h>
#include <hal_data.h>

#if (PHYDM_NEW_INTERFACE == 1)
typedef enum _HAL_PHYDM_OPS {
	HAL_PHYDM_DIS_ALL_FUNC,
	HAL_PHYDM_FUNC_SET,
	HAL_PHYDM_FUNC_CLR,
	HAL_PHYDM_ABILITY_BK,
	HAL_PHYDM_ABILITY_RESTORE,
	HAL_PHYDM_ABILITY_SET,
	HAL_PHYDM_ABILITY_GET,
} HAL_PHYDM_OPS;

u32 rtw_phydm_ability_ops(_adapter *adapter, HAL_PHYDM_OPS ops, u32 ability);
bool rtw_odm_adaptivity_needed(_adapter *adapter);

#define rtw_phydm_func_disable_all(adapter) \
		rtw_phydm_ability_ops(adapter, HAL_PHYDM_DIS_ALL_FUNC, 0)

#define rtw_phydm_func_for_offchannel(adapter) \
		do { \
			rtw_phydm_ability_ops(adapter, HAL_PHYDM_DIS_ALL_FUNC, 0); \
			if (rtw_odm_adaptivity_needed(adapter)) \
				rtw_phydm_ability_ops(adapter, HAL_PHYDM_FUNC_SET, ODM_BB_ADAPTIVITY); \
		} while (0)

#define rtw_phydm_func_clr(adapter, ability)	\
		rtw_phydm_ability_ops(adapter, HAL_PHYDM_FUNC_CLR, ability)

#define rtw_phydm_ability_backup(adapter)	\
		rtw_phydm_ability_ops(adapter, HAL_PHYDM_ABILITY_BK, 0)

#define rtw_phydm_ability_restore(adapter)	\
		rtw_phydm_ability_ops(adapter, HAL_PHYDM_ABILITY_RESTORE, 0)

#define rtw_phydm_func_set(adapter, ability)	\
		rtw_phydm_ability_ops(adapter, HAL_PHYDM_ABILITY_SET, ability)

static inline u32 rtw_phydm_ability_get(_adapter *adapter)
{
	return rtw_phydm_ability_ops(adapter, HAL_PHYDM_ABILITY_GET, 0);
}

void rtw_phydm_set_dbg_info(_adapter *padapter, u32 dm_func);

#if (PHYDM_LINUX_CODING_STYLE == 1)
void rtw_odm_acquirespinlock(_adapter *adapter, enum rt_spinlock_type type);
void rtw_odm_releasespinlock(_adapter *adapter, enum rt_spinlock_type type);
#else
void rtw_odm_acquirespinlock(_adapter *adapter,	RT_SPINLOCK_TYPE type);
void rtw_odm_releasespinlock(_adapter *adapter,	RT_SPINLOCK_TYPE type);
#endif

#endif
#endif	//_RTW_ODM_H_

