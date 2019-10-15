/******************************************************************************
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#ifndef _RTW_INTFS_H_
#define _RTW_INTFS_H_

extern u8 rtw_init_default_value(_adapter *padapter);
#ifdef CONFIG_WOWLAN
void rtw_cancel_dynamic_chk_timer(_adapter *padapter);
#endif
extern void rtw_cancel_all_timer(_adapter *padapter);

extern u8 rtw_init_drv_sw(_adapter *padapter);
extern u8 rtw_free_drv_sw(_adapter *padapter);
extern u8 rtw_reset_drv_sw(_adapter *padapter);

extern int rtw_drv_init(ADAPTER *padapter);
extern void rtw_drv_deinit(ADAPTER *Adapter);

extern u32 rtw_start_drv_threads(_adapter *padapter);
extern void rtw_stop_drv_threads(_adapter *padapter);

#endif //_RTW_INTFS_H_
