#ifndef _ROM_RTW_MESSAGE_
#define _ROM_RTW_MESSAGE_

#include <autoconf.h>
#if (CONFIG_PLATFORM_AMEBA_X == 1)
#include <platform_stdlib.h>
#endif //CONFIG_PLATFORM_AMEBA_X

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

#if defined(CONFIG_PLATFORM_8735B)
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
#else
extern const char *const rom_e_rtw_msgp_str_[ROM_E_RTW_MSGP_MAX];
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
#endif

#if defined(CONFIG_PLATFORM_8735B)
char **get_rom_f_rtw_msgp_str(void);
#define rom_f_rtw_msg_printf(name, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			char **rom_f_rtw_msgp_str_ = get_rom_f_rtw_msgp_str();\
			printf((char*)rom_f_rtw_msgp_str_[ROM_F_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#define rom_f_rtw_msg_871X_LEVEL(name, level, fmt, args...) \
	do {\
		if(GlobalDebugEnable){\
			char **rom_f_rtw_msgp_str_ = get_rom_f_rtw_msgp_str();\
			printf("\n\r");\
			printf((char*)rom_f_rtw_msgp_str_[ROM_F_RTW_MSGP_##name], ## args);\
		}\
	}while(0)
#else
extern const char *const rom_f_rtw_msgp_str_[ROM_F_RTW_MSGP_MAX];
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
#endif

#endif //_ROM_RTW_MESSAGE_
