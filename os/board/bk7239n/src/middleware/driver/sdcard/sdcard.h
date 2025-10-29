#ifndef _SDCARD_H_
#define _SDCARD_H_

#if defined(CONFIG_SDCARD)
#include "sdcard_pub.h"

typedef void (*SD_DETECT_FUN)(void);
typedef void (*sdcard_ps_callback_t)(void);

typedef struct _sdcard_ {
	UINT32  total_block;
	UINT16  block_size;
	UINT16  card_rca;
	UINT16	Addr_shift_bit;
	UINT8  	init_flag;
	UINT8 	clk_cfg;
} SDCARD_S, *SDCARD_PTR;

/* API */
extern void sdcard_get_card_info(SDCARD_S *card_info);
extern UINT32 sdcard_open(UINT32 op_falag);
extern UINT32 sdcard_close(void);
extern UINT32 sdcard_read(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 sdcard_write(char *user_buf, UINT32 count, UINT32 op_flag);
extern UINT32 sdcard_ctrl(UINT32 cmd, void *parm);
extern void sdcard_register_ps_suspend_callback(sdcard_ps_callback_t ps_suspend_cb);
extern void sdcard_register_ps_resume_callback(sdcard_ps_callback_t ps_resume_cb);

#endif // CONFIG_SDCARD
#endif
// eof

