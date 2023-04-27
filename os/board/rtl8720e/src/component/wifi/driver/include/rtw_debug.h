/******************************************************************************
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
  *
******************************************************************************/

#ifndef __RTW_DEBUG_H__
#define __RTW_DEBUG_H__

#define RTW_RESIDENT(x,...) do {}while(0)
#define RTW_ERR(x, ...) do {} while (0)
#define RTW_WARN(x,...) do {} while (0)
#define RTW_INFO(x,...) do {} while (0)
#define RTW_DBG(x,...) do {} while (0)


/* add new definition of debug log level */
#define RTW_MSG_RESIDENT   2
#define RTW_MSG_ERROR         3
#define RTW_MSG_WARNING    4

/* reserve definition for macro rom_e_rtw_msg_871X_LEVEL*/
#define _drv_always_		1
#define _drv_err_			5
#define _drv_info_			8

extern u32 GlobalDebugEnable;
extern u8 OtherDebugPortEnable;
extern u16 GlobalDebugLevel;


#undef	_dbgdump
#undef   _dbgdump_nr

#define _dbgdump_nr	printf("\n\r"); printf
#define _dbgdump	printf


#include <diag.h> // for DRIVER_PREFIX


/* 220728:add new definition of RTW_ERR\RTW_WARN\RTW_RESIDENT */
#if defined(RTW_MSG_LEVEL) && defined(_dbgdump_nr)

#undef  RTW_RESIDENT
#define RTW_RESIDENT(...)   \
	do { \
		if (RTW_MSG_LEVEL >= RTW_MSG_RESIDENT) { \
			_dbgdump_nr(DRIVER_PREFIX __VA_ARGS__);\
			} \
	} while(0)


#undef RTW_ERR
#define RTW_ERR(...) 	\
	do {\
		if (RTW_MSG_LEVEL >= RTW_MSG_ERROR) { \
			_dbgdump_nr(DRIVER_PREFIX"[ERROR]" __VA_ARGS__);\
		} \
	} while(0)

#undef  RTW_WARN
#define RTW_WARN(...) \
	do{	\
		if (RTW_MSG_LEVEL >= RTW_MSG_WARNING) { \
			_dbgdump_nr(DRIVER_PREFIX"[WARNING]" __VA_ARGS__); \
		} \
	} while (0)
#endif


#if 	defined (_dbgdump)
#if defined (__ICCARM__) || defined (__CC_ARM) ||defined(__GNUC__)

extern int msg_uart_port(const char *fmt, ...);
#define RTW_PRINT_MSG(...) 	\
	do {\
		if(OtherDebugPortEnable){\
			msg_uart_port(__VA_ARGS__);\
		} else {\
			_dbgdump(__VA_ARGS__);\
		} \
	}while(0)
#endif
#endif


#endif	//__RTW_DEBUG_H__

