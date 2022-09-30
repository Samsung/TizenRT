/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
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
 *****************************************************************************/
#ifndef __HAL_DM_H__
#define __HAL_DM_H__

//#define adapter_to_phydm(adapter) (&(GET_HAL_DATA(adapter)->odmpriv))
//#define dvobj_to_phydm(dvobj) adapter_to_phydm(dvobj_get_primary_adapter(dvobj))

void rtw_hal_turbo_edca(_adapter *adapter);
void rtw_phydm_ra_registed(_adapter *adapter, struct sta_info *psta);

#endif /* __HAL_DM_H__ */
