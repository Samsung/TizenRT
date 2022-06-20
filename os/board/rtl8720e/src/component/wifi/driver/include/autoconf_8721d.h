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
#ifndef AUTOCONF_8721D_H
#define AUTOCONF_8721D_H


#ifndef CONFIG_RTL8721D
#define CONFIG_RTL8721D
#endif
#undef RTL8721D_SUPPORT
#define RTL8721D_SUPPORT 1
#ifdef CONFIG_MCC_MODE
#define CONFIG_MCC_IQK_OFFLOAD
#endif
/* Configure for bus.
 * 8721D uses the AIX bus, but didn't change the macro in 8731D when porting. */
#define CONFIG_LX_HCI
/* For efuse or flash config start */
#define CONFIG_EFUSE_SEPARATE
/* For efuse or flash config end */
/* PHY layer band config */
#define CONFIG_DFS
/* PHY layer band config end */
/* Configurations for customer */
/* for ROKU */
#define CONFIG_XMIT_ACK
#define USER_CTL_POWER_SAVE
/* for SONY */
#define CONFIG_APP_CTRL_RF_ONOFF
/* Configurations for customer end */
/* ANTENNA_DIVERSITY provided by BB */
#define CONFIG_ANTENNA_DIVERSITY
/* Config the BT_COEXIST for wlan5 verison */
#define CONFIG_BT_COEXIST_SOC
/* For DM support */
#define RATE_ADAPTIVE_SUPPORT 0
//#define CONFIG_ODM_REFRESH_RAMASK
/* Configurations for power saving */
#define CONFIG_WOWLAN
/* enable 1X code in lib_wlan as default (increase 380 bytes) */
#define CONFIG_EAP
/* For phydm configurations */
#define SUPPORT_5G_CHANNEL
#define CONFIG_INIT_CHAN 1
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define  CONFIG_BW_MODE	0x11
#define CONFIG_FW_C2H_PKT
#define PHYDM_VERSION	2 /*phydm trunk*/
/* config in concurrent mode */
#ifdef CONFIG_CONCURRENT_MODE
#define CONFIG_RUNTIME_PORT_SWITCH
#endif
/* config for AP mode */
#define AP_STA_NUM (5)
//Decrease STA due to memory limitation - Alex Fang
#define NUM_STA (2 + AP_STA_NUM)	//2 + supported clients

//#define RX_SHORTCUT
//#define TX_SHORTCUT
#define TXBD_PRE_MALLOC
#define RTL8721D_SPECIFIC		//Patch for rtl8721d, avoid misunderstanding when porting to next new IC
//#define CONFIG_SUPPORT_DYNAMIC_TXPWR   //rtw_phydm_fill_desc_dpt todo
//#define CONFIG_WIFI_MESH	0

#if defined(CONFIG_TWO_MAC_DRIVER) || defined(CONFIG_MAC_LOOPBACK_DRIVER_AMEBA)
#define CONFIG_SUDO_PHY_SETTING
#define HAL_MAC_ENABLE 1
#define DISABLE_BB_RF 1
#else
#define HAL_MAC_ENABLE 1
#define HAL_BB_ENABLE 1
#define HAL_RF_ENABLE 1
#define DISABLE_BB_RF 0
#endif

#define CONFIG_REG_ENABLE_KFREE 0	// 0: Depend on efuse(flash), 1: enable, 2: disable

#endif  /* #ifndef AUTOCONF_8721D_H */
