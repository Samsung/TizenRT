/******************************************************************************
 *
 * Copyright(c) 2007 - 2020  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/
#ifndef __HALBB_IC_SW_INFO_H__
#define __HALBB_IC_SW_INFO_H__

#define HLABB_CODE_BASE			"HALBB_029"
#define HALBB_RELEASE_DATE		"2022_0223"

/*HALBB API status*/
#define	HALBB_SET_FAIL			0
#define	HALBB_SET_SUCCESS		1
#define	HALBB_SET_NO_NEED		3

/*HALBB Set/Revert*/
#define	HALBB_SET			1
#define	HALBB_REVERT			2

/****************************************************************
 * 1 ============================================================
 * 1  enumeration
 * 1 ============================================================
 ***************************************************************/

enum halbb_api_host {
	RUN_IN_FW		= 0,
	RUN_IN_DRIVER		= 1
};

enum halbb_backup_type {
	HALBB_BACKUP	= 1,
	HALBB_RESTORE	= 2
};

#endif
