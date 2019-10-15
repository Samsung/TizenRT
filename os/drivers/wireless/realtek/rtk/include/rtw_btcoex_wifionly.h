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
#ifndef __RTW_BTCOEX_WIFIONLY_H__
#define __RTW_BTCOEX_WIFIONLY_H__

#if defined(CONFIG_RTL8721D) || defined(CONFIG_RTL8710C) || defined(CONFIG_RTL8821C)
void rtw_btcoex_wifionly_switchband_notify(PADAPTER padapter);
void rtw_btcoex_wifionly_scan_notify(PADAPTER padapter);
void rtw_btcoex_wifionly_connect_notify(PADAPTER padapter);
void rtw_btcoex_wifionly_hw_config(PADAPTER padapter);
void rtw_btcoex_wifionly_initialize(PADAPTER padapter);
void rtw_btcoex_wifionly_AntInfoSetting(PADAPTER padapter);
#else
#define rtw_btcoex_wifionly_switchband_notify(padapter)
#define rtw_btcoex_wifionly_scan_notify(padapter)
#define rtw_btcoex_wifionly_connect_notify(padapter)
#define rtw_btcoex_wifionly_hw_config(padapter)
#define rtw_btcoex_wifionly_initialize(padapter)
#define rtw_btcoex_wifionly_AntInfoSetting(padapter)
#endif
#endif
