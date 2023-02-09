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
#ifndef AUTOCONF_DEBUG_H
#define AUTOCONF_DEBUG_H


/***************************** for Debug message ******************************/
//#define CONFIG_DEBUG
//#define CONFIG_DEBUG_RTL871X
#define DBG 0 /* for phy dm debug */

#if (CONFIG_PLATFORM_AMEBA_X == 1)
#define WLAN_INTF_DBG		0
//#define CONFIG_DEBUG_DYNAMIC
//#define DBG_TX 1
//#define DBG_XMIT_BUF 1
//#define DBG_XMIT_BUF_EXT 1
#define DBG_TX_DROP_FRAME
#define CONFIG_DBG_DISABLE_RDU_INTERRUPT

/***************************** phy dynamic debug ******************************/
/* Patch when dynamic mechanism is not ready */
//#define CONFIG_DM_PATCH
#define DM_DBG 0
#if (DM_DBG == 1)
/* For DM debug*/
/***************************** BB debug ***************************************/
#define DBG_RX_INFO 1
#define DBG_DM_DIG 1			/* DebugComponents: bit0 */
#define DBG_DM_RA_MASK 1		/* DebugComponents: bit1 */
#define DBG_DM_ANT_DIV 1		/* DebugComponents: bit6 */
#define DBG_TX_RATE 1			/* DebugComponents: bit9 */
#define DBG_DM_RA 1			/* DebugComponents: bit9 */
#define DBG_DM_ADAPTIVITY 1		/* DebugComponents: bit17 */
/***************************** RF debug ***************************************/
#define DBG_PWR_TRACKING 1		/* DebugComponents: bit24 */
#define DBG_RF_IQK 1			/* DebugComponents: bit26 */
#define DBG_RF_DPK 1			/* DebugComponents: */
/************************** PHY common debug **********************************/
#define DBG_PWR_INDEX 1			// DebugComponents: bit30 */
#endif /* (DM_DBG == 1) */
#else /* (CONFIG_PLATFORM_AMEBA_X != 1) */
//#define WLAN_INTF_DBG
#endif /* (CONFIG_PLATFORM_AMEBA_X == 1) */
/***************************** for Debug message ******************************/


#endif /* #ifndef AUTOCONF_DEBUG_H */

