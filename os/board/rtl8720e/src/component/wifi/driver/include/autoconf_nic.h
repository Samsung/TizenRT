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
#ifndef AUTOCONF_NIC_H
#define AUTOCONF_NIC_H

/*************************** Non Ameba Series Start ***************************/
#if !defined(CONFIG_HARDWARE_8192E)
#define NOT_SUPPORT_RF_MULTIPATH
#endif

/* Config for TRX thread */
#define CONFIG_XMIT_THREAD_MODE
/* PHY layer band config */
//#define SUPPORT_5G_CHANNEL
#define CONFIG_INIT_CHAN 1
#define NOT_SUPPORT_40M
/* 0: 20 MHz, 1: 40 MHz, 2: 80 MHz, 3: 160MHz, 4: 80+80MHz
* 2.4G use bit 0 ~ 3, 5G use bit 4 ~ 7
* 0x21 means enable 2.4G 40MHz & 5G 80MHz */
#define  CONFIG_BW_MODE	0x00
/* PHY layer band config end */
#if defined(CONFIG_PLATFOMR_CUSTOMER_RTOS)
#define CONFIG_LWIP_LAYER 0
#else
#define PLATFORM_FREERTOS 1
#endif /* CONFIG_PLATFOMR_CUSTOMER_RTOS */

#define BE_I_CUT 1

/* For multicast */
#define CONFIG_MULTICAST

#define HAL_MAC_ENABLE 1
#define HAL_BB_ENABLE 1
#define HAL_RF_ENABLE 1

#define USE_XMIT_EXTBUFF 0

/* enable 1X code in lib_wlan as default (increase 380 bytes) */
#define CONFIG_EAP

/* For phydm configurations */
#define CONFIG_FW_C2H_PKT
#define PHYDM_VERSION	2 /*phydm trunk*/

/*************************** Non Ameba Series End *****************************/

#define CONFIG_REG_ENABLE_KFREE 0	// 0: Depend on efuse(flash), 1: enable, 2: disable

/*************************** Config for MP_MODE *******************************/
//#define CONFIG_MP_INCLUDED
#ifdef CONFIG_MP_INCLUDED
#define MP_DRIVER 1
#undef CONFIG_ANTENNA_DIVERSITY
#undef CONFIG_BT_COEXIST_SOC
#undef CONFIG_REG_ENABLE_KFREE
#define CONFIG_REG_ENABLE_KFREE 1	 // 1: enable, 2: disable
#else /* undef CONFIG_MP_INCLUDED  */
#define MP_DRIVER 0
#endif /* #ifdef CONFIG_MP_INCLUDED */
/************************* Config for MP_MODE end *****************************/

#endif /* #ifndef AUTOCONF_NIC_H */

