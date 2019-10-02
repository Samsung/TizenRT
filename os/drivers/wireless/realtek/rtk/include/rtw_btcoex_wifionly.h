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
