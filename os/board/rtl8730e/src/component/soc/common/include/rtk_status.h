/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __RTK_STATUS_H__
#define __RTK_STATUS_H__

/* The following wifi doxygen groups are workaround for online docs display. SoC need replace these with SoC's own. */
/** @addtogroup WIFI_API Wi-Fi APIs
 *  @brief      WIFI_API module
 *  @{
 */
/** @addtogroup WIFI_Exported_Constants Wi-Fi Exported Constants
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#define RTK_SUCCESS	0
#define RTK_FAIL	(-1)

/** @} End of WIFI_Exported_Constants group*/


/** @addtogroup WIFI_Exported_Types Wi-Fi Exported Types
* @{
*/
/** @addtogroup WIFI_Exported_Enumeration_Types Enumeration Type
 * @{
 */

/**
* @brief Error codes for RTK system (size: u32).
*/
enum rtk_error_code {
	//  content in <!-- --> can be ignored by Doxygen
	RTK_ERR_BADARG              = 2,    /**< <!-- DIAG: --> Bad Argument */
	RTK_ERR_BUSY                = 3,    /**< <!-- DIAG: --> Busy*/
	RTK_ERR_NOMEM               = 4,    /**< <!-- DIAG: --> No Memory*/
	RTK_ERR_TIMEOUT             = 5,    /**< <!-- DIAG: --> Timeout */
	RTK_ERR_BUFFER_OVERFLOW     = 6,    /**< <!-- DIAG: --> Data exceeds allocated buffer size.*/

	RTK_ERR_DIAG_BASE               = 0x60,
	RTK_ERR_DIAG_UNINIT             = RTK_ERR_DIAG_BASE + 0x01,     // diag uninitialize
	RTK_ERR_DIAG_SEM_FAIL           = RTK_ERR_DIAG_BASE + 0x02,     // ipc sema get fail
	RTK_ERR_DIAG_MALLOC             = RTK_ERR_DIAG_BASE + 0x03,     // malloc fail
	RTK_ERR_DIAG_FREE               = RTK_ERR_DIAG_BASE + 0x04,     // free fail
	RTK_ERR_DIAG_TOO_LARGE_BUFF     = RTK_ERR_DIAG_BASE + 0x05,     // Too large a buffer
	RTK_ERR_DIAG_TOO_SMALL_BUFF     = RTK_ERR_DIAG_BASE + 0x06,     // Too small a buffer
	RTK_ERR_DIAG_SEND_TIMEOUT       = RTK_ERR_DIAG_BASE + 0x07,     // timeout in transfer of big event

	RTK_ERR_DIAG_EVT_ADD_FAIL       = RTK_ERR_DIAG_BASE + 0x10,     // add event fail
	RTK_ERR_DIAG_EVT_NO_MORE        = RTK_ERR_DIAG_BASE + 0x11,     // no more event
	RTK_ERR_DIAG_EVT_FIND_FAIL      = RTK_ERR_DIAG_BASE + 0x12,     // find event fail

	RTK_ERR_WIFI_BASE                       = 0x1000,
	RTK_ERR_WIFI_CONN_INVALID_KEY           = RTK_ERR_WIFI_BASE + 11,   /**< <!-- DIAG: --> Invalid key */
	RTK_ERR_WIFI_CONN_SCAN_FAIL             = RTK_ERR_WIFI_BASE + 12,   /**< <!-- DIAG: --> Can't find targrt AP*/
	RTK_ERR_WIFI_CONN_AUTH_FAIL             = RTK_ERR_WIFI_BASE + 13,   /**< <!-- DIAG: --> Auth fail*/
	RTK_ERR_WIFI_CONN_AUTH_PASSWORD_WRONG   = RTK_ERR_WIFI_BASE + 14,   /**< <!-- DIAG: --> Auth fail, password may wrong*/
	RTK_ERR_WIFI_CONN_ASSOC_FAIL            = RTK_ERR_WIFI_BASE + 15,   /**< <!-- DIAG: --> Assoc fail*/
	RTK_ERR_WIFI_CONN_4WAY_HANDSHAKE_FAIL   = RTK_ERR_WIFI_BASE + 16,   /**< <!-- DIAG: --> 4 way handshake fail*/
	RTK_ERR_WIFI_CONN_4WAY_PASSWORD_WRONG   = RTK_ERR_WIFI_BASE + 17,   /**< <!-- DIAG: --> 4 way handshake fail, password may wrong*/
	RTK_ERR_WIFI_CONN_APP_DISCONN           = RTK_ERR_WIFI_BASE + 18,   /**< <!-- DIAG: --> Connentting is aborted by application layer to disconnect*/

	RTK_ERR_WIFI_POWEROFF       = RTK_ERR_WIFI_BASE + 32,   /**< <!-- DIAG: --> Wi-Fi is powered off in IPS(Inactive Power Save) mode, unable to access Wi-Fi registers.*/
	RTK_ERR_WIFI_TX_BUF_FULL    = RTK_ERR_WIFI_BASE + 33,   /**< <!-- DIAG: --> tx buffer full(hw buffer & skb buffer) */
	RTK_ERR_WIFI_NOT_INIT       = RTK_ERR_WIFI_BASE + 34,   /**< <!-- DIAG: --> wifi not initial*/
};

/** @} End of WIFI_Exported_Enumeration_Types group*/
/** @} End of WIFI_Exported_Types group*/

/** @} End of WIFI_API group */

#ifdef __cplusplus
}
#endif

#endif
