/*
*******************************************************************************
* Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*******************************************************************************
*/
#ifndef __BT_AUDIO_DEBUG_H__
#define __BT_AUDIO_DEBUG_H__

// #define CONFIG_BT_AUDIO_DEBUG

#ifdef CONFIG_BT_AUDIO_DEBUG
#define _dbgdump	printf("\n\r"); printf
#define PREFIX	"[BT_AUDIO] "
#if	defined (_dbgdump)
#undef DBG_BAD
#define DBG_BAD(...)     do {\
		_dbgdump(PREFIX __VA_ARGS__);\
	}while(0)
#endif
#else
#define DBG_BAD(x, ...) do {} while(0)
#endif /* CONFIG_BT_AUDIO_DEBUG */

#endif /* __BT_AUDIO_DEBUG_H__ */