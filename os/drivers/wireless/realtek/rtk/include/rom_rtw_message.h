#ifndef _ROM_RTW_MESSAGE_
#define _ROM_RTW_MESSAGE_

#if defined(CONFIG_PLATFORM_8195A) || defined(CONFIG_PLATFORM_8711B) || defined(CONFIG_PLATFORM_8721D) || defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
#include <platform_stdlib.h>
#endif

extern u32 GlobalDebugEnable;

// e-version
typedef enum {
#define ROM_E_RTW_MSGPOOL(name,str)  ROM_E_RTW_MSGP_##name,
#include "rom_rtw_message_e.h"
  ROM_E_RTW_MSGP_MAX
} rom_e_rtw_msgp_t;

// f-version
typedef enum {
#define ROM_F_RTW_MSGPOOL(name,str)  ROM_F_RTW_MSGP_##name,
#include "rom_rtw_message_f.h"
  ROM_F_RTW_MSGP_MAX
} rom_f_rtw_msgp_t;

#if defined(CONFIG_PLATFORM_8195BHP) || defined(CONFIG_PLATFORM_8710C)
char **get_rom_e_rtw_msgp_str(void);
#define rom_e_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			char **rom_e_rtw_msgp_str_ = get_rom_e_rtw_msgp_str();\
			printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			char **rom_e_rtw_msgp_str_ = get_rom_e_rtw_msgp_str();\
			printf("\n\r");\
			printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#elif ROM_E_RTW_MSG
extern const char *rom_e_rtw_msgp_str_[];
#define rom_e_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
	
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf("\n\r");\
			printf((char*)rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#else
#define rom_e_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf(fmt, ## args);\
		}\
	}while(0)
#if defined (__GNUC__)
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf("\n\r");\
			printf(fmt, ## args);\
		}\
	}while(0)
#else
	
#define rom_e_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf("\n\r");\
			printf(DRIVER_PREFIX ##fmt, ## args);\
		}\
	}while(0)
#endif
#endif //ROM_E_RTW_MSG

#if ROM_F_RTW_MSG
extern const char *rom_f_rtw_msgp_str_[];
#define rom_f_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf((char*)rom_f_rtw_msgp_str_[ROM_F_RTW_MSGP_##name], ## args);\
		}\
	}while(0)

#define rom_f_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf("\n\r");\
			printf((char*)rom_f_rtw_msgp_str_[ROM_F_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#else
#define rom_f_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
				printf(fmt, ## args);\
		}\
	}while(0)
	
#define rom_f_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			printf("\n\r");\
			printf(DRIVER_PREFIX ##fmt, ## args);\
		}\
	}while(0)
#endif //ROM_F_RTW_MSG

#endif //_ROM_RTW_MESSAGE_
