// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _mailbox_channel_h_
#define _mailbox_channel_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <common/bk_typedef.h>
#include <driver/mailbox_types.h>


#define SYSTEM_CPU_NUM		CONFIG_CPU_CNT

#define DST_CPU_ID_MASK		0xC0
#define DST_CPU_ID_POS		6
#define SRC_CPU_ID_MASK		0x30
#define SRC_CPU_ID_POS		4

#define LOG_CHNL_ID_MASK	0x0F

#define GET_SRC_CPU_ID(log_chnl)		(((log_chnl) & SRC_CPU_ID_MASK) >> SRC_CPU_ID_POS)
#define GET_DST_CPU_ID(log_chnl)		(((log_chnl) & DST_CPU_ID_MASK) >> DST_CPU_ID_POS)

#define CPX_LOG_CHNL_START(src_cpu, dst_cpu)	((((dst_cpu) << DST_CPU_ID_POS) & DST_CPU_ID_MASK) + (((src_cpu) << SRC_CPU_ID_POS) & SRC_CPU_ID_MASK))

#define GET_LOG_CHNL_ID(log_chnl)		((log_chnl) & LOG_CHNL_ID_MASK)

/* ======================================================================================= */
/* ==========================  CPU0 --> Target_CPUs mailbox.   =========================== */
/* ======================================================================================= */
#ifdef CONFIG_SYS_CPU0

#define SELF_CPU		MAILBOX_CPU0

enum
{
	/* CPU0 --> CPU0 */
	CP0_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU0),
	CP0_MB_CHNL_CTRL        = CP0_MB_LOG_CHNL_START,

	CP0_MB_LOG_CHNL_END,
	CP0_MB_LOG_CHNL_MAX = (CP0_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU0 --> CPU1 */
	CP1_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU1),
	MB_CHNL_HW_CTRL         = CP1_MB_LOG_CHNL_START,
	CP1_MB_CHNL_IPC,
	MB_CHNL_AUD,
	MB_CHNL_VID,
	MB_CHNL_PWC,
	MB_CHNL_MEDIA,
	MB_CHNL_MIPC_SYNC,
	MB_CHNL_LVGL,
	MB_CHNL_USB,
	MB_CHNL_AT,
	MB_CHNL_COM,   /* MB_CHNL_COM is default mailbox channel used for audio and video */
	MB_CHNL_DEC,
	MB_CHNL_UART,
	MB_CHNL_FLASH,

	/*                  !!!!!   Note   !!!!!              */
	/* ===> MB_CHNL_LOG should be the LAST one. LOWEST priority. */
	MB_CHNL_LOG,   /* MB_CHNL_LOG should be the LAST one. LOWEST priority. */
	
	CP1_MB_LOG_CHNL_END,
	CP1_MB_LOG_CHNL_MAX = (CP1_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU0 --> CPU2 */
	CP2_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU2),
	CP2_MB_CHNL_HW_CTRL         = CP2_MB_LOG_CHNL_START,

	CP2_MB_CHNL_USB,

	CP2_MB_CHNL_LOG,   /* MB_CHNL_LOG should be the LAST one. LOWEST priority. */

	CP2_MB_LOG_CHNL_END,
	CP2_MB_LOG_CHNL_MAX = (CP2_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

#endif

/* ======================================================================================= */
/* ==========================  CPU1 --> Target_CPUs mailbox.   =========================== */
/* ======================================================================================= */
#ifdef CONFIG_SYS_CPU1

#define SELF_CPU		MAILBOX_CPU1

enum
{
	/* CPU1 --> CPU0 */
	CP0_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU0),
	MB_CHNL_HW_CTRL         = CP0_MB_LOG_CHNL_START,
	CP0_MB_CHNL_IPC,
	MB_CHNL_AUD,
	MB_CHNL_VID,
	MB_CHNL_PWC,
	MB_CHNL_MEDIA,
	MB_CHNL_MIPC_SYNC,
	MB_CHNL_LVGL,
	MB_CHNL_USB,
	MB_CHNL_AT,
	MB_CHNL_COM,   /* MB_CHNL_COM is default mailbox channel used for audio and video */
	MB_CHNL_DEC,
	MB_CHNL_UART,
	MB_CHNL_FLASH,

	/*                  !!!!!   Note   !!!!!              */
	/* ===> MB_CHNL_LOG should be the LAST one. LOWEST priority. */
	MB_CHNL_LOG,   /* MB_CHNL_LOG should be the LAST one. LOWEST priority. */

	CP0_MB_LOG_CHNL_END,
	CP0_MB_LOG_CHNL_MAX = (CP0_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU1 --> CPU1 */
	CP1_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU1),
	CP1_MB_CHNL_CTRL        = CP1_MB_LOG_CHNL_START,

	CP1_MB_LOG_CHNL_END,
	CP1_MB_LOG_CHNL_MAX = (CP1_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU1 --> CPU2 */
	CP2_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU2),
	CP2_MB_CHNL_CTRL        = CP2_MB_LOG_CHNL_START,
	CP2_MB_CHNL_IPC,

	CP2_MB_CHNL_MEDIA,
	CP2_MB_CHNL_USB,
	CP2_MB_CHNL_MIPC,

	CP2_MB_LOG_CHNL_END,
	CP2_MB_LOG_CHNL_MAX = (CP2_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

#endif

/* ======================================================================================= */
/* ==========================  CPU2 --> Target_CPUs mailbox.   =========================== */
/* ======================================================================================= */
#ifdef CONFIG_SYS_CPU2

#define SELF_CPU		MAILBOX_CPU2

enum
{
	/* CPU2 --> CPU0 */
	CP0_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU0),
	CP0_MB_CHNL_HW_CTRL         = CP0_MB_LOG_CHNL_START,

	CP0_MB_CHNL_USB,

//	CP0_MB_CHNL_LOG,   /* MB_CHNL_LOG should be the LAST one. LOWEST priority. */

	CP0_MB_LOG_CHNL_END,
	CP0_MB_LOG_CHNL_MAX = (CP0_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU2 --> CPU1 */
	CP1_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU1),
	CP1_MB_CHNL_CTRL        = CP1_MB_LOG_CHNL_START,
	CP1_MB_CHNL_IPC,

	CP1_MB_CHNL_MEDIA,

	CP1_MB_CHNL_USB,
	CP1_MB_CHNL_MIPC,

	CP1_MB_CHNL_LOG,   /* MB_CHNL_LOG should be the LAST one. LOWEST priority. */

	CP1_MB_LOG_CHNL_END,
	CP1_MB_LOG_CHNL_MAX = (CP1_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

/* the BIGGER the value, the LOWER the channel priority. */
enum
{
	/* CPU2 --> CPU2 */
	CP2_MB_LOG_CHNL_START   = CPX_LOG_CHNL_START(SELF_CPU, MAILBOX_CPU2),
	CP2_MB_CHNL_CTRL        = CP2_MB_LOG_CHNL_START,

	CP2_MB_LOG_CHNL_END,
	CP2_MB_LOG_CHNL_MAX = (CP2_MB_LOG_CHNL_START + LOG_CHNL_ID_MASK), // max 16 channels.
};

#endif


#define SRC_CPU		SELF_CPU

#define CHNL_STATE_MASK			0xF
#define CHNL_STATE_COM_FAIL		0x1		/* cmd NO target app, it is an ACK bit to peer CPU. */

typedef union
{
	struct
	{
		u32		cmd           :  8;
		u32		state         :  4;
		u32		Reserved      :  20;	/* reserved for system. */
	} ;
	u32		data;
} mb_chnl_hdr_t;

typedef struct
{
	mb_chnl_hdr_t	hdr;

	u32		param1;
	u32		param2;
	u32		param3;
} mb_chnl_cmd_t;

enum
{
	ACK_STATE_PENDING = 0x01,		/* cmd is being handled in the peer CPU. */
	ACK_STATE_COMPLETE, 			/* cmd handling is competed, addtional infos in ack_data1&ack_data2. */
	ACK_STATE_FAIL, 				/* cmd failed, addtional infos in ack_data1&ack_data2. */
};

typedef struct
{
	mb_chnl_hdr_t	hdr;

	u32		ack_data1;
	u32		ack_data2;
	union
	{
		u32		ack_data3;
		u32		ack_state;				/* ack_state or ack_data3, depends on applications. */
	};
} mb_chnl_ack_t;

typedef void  (* chnl_tx_cmpl_isr_t)(void *param, mb_chnl_ack_t *ack_buf);
typedef void  (* chnl_tx_isr_t)(void *param);

/*
 *  rx_isr firstly use the buf as the command buffer, after handled the cmd, 
 *  then use the buf as acknowledge buffer to fill the handle result.
 */
typedef void  (* chnl_rx_isr_t)(void *param, mb_chnl_cmd_t *cmd_buf);

enum
{
	MB_CHNL_GET_STATUS = 0,
	MB_CHNL_SET_RX_ISR,
	MB_CHNL_SET_TX_ISR,
	MB_CHNL_SET_TX_CMPL_ISR,
	MB_CHNL_WRITE_SYNC,
	MB_CHNL_TX_RESET,
};

/*
typedef struct   // MB_CHNL_GET_STATUS  cmd_param
{
	u8    chnl_busy;
} mb_chnl_get_status_param_t;

typedef struct   // MB_CHNL_SET_RX_ISR  cmd_param
{
	void * rx_isr;
} mb_chnl_set_rx_isr_param_t;

typedef struct   // MB_CHNL_SET_TX_ISR  cmd_param
{
	void * tx_isr;
} mb_chnl_set_tx_isr_param_t;

typedef struct   // MB_CHNL_SET_TX_CMPL_ISR  cmd_param
{
	void * tx_cmpl_isr;
} mb_chnl_set_tx_cmpl_isr_param_t;
*/

/*
  * init logical chnanel module.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_init(void);

/*
  * open logical chnanel.
  * input:
  *     log_chnl  : logical channel id to open.
  *     callback_param : param passsed to all callbacks.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_open(u8 log_chnl, void * callback_param);

/*
  * close logical chnanel.
  * input:
  *     log_chnl  : logical channel id to close.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_close(u8 log_chnl);

/*
  * read from logical chnanel.
  * input:
  *     log_chnl     : logical channel id to read.
  *     read_buf       : buffer to receive channel cmd data.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_read(u8 log_chnl, mb_chnl_cmd_t * read_buf);

/*
  * write to logical chnanel.
  * input:
  *     log_chnl     : logical channel id to write.
  *     cmd_buf       : command buffer to send.
  * 
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_write(u8 log_chnl, mb_chnl_cmd_t * cmd_buf);

/*
  * logical chnanel misc io (set/get param).
  * input:
  *     log_chnl     : logical channel id to set/get param.
  *     cmd          : control command for logical channel.
  *     param      :  parameter of the command.
  *        MB_CHNL_GET_STATUS:   param, (u8 *) point to buffer (one byte in size.) to receive status data.
  *        MB_CHNL_SET_RX_ISR:   param, pointer to rx_isr_callback.
  *        MB_CHNL_SET_TX_ISR:   param, pointer to tx_isr_callback.
  *        MB_CHNL_SET_TX_CMPL_ISR:   param, pointer to tx_cmpl_isr_callback.
  *        MB_CHNL_WRITE_SYNC:   param, pointer to mb_chnl_cmd_t buffer, write to mailbox synchronously.
  *        MB_CHNL_TX_RESET: param is NULL.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
bk_err_t mb_chnl_ctrl(u8 log_chnl, u8 cmd, void * param);

/* =====================      physical channel APIs      ==================*/
/*
  * get communication statistics for target CPU (phy_chnl).
  * input:
  *     dst_cpu  : get statistics for this core.
  * output:
  *     rx_cnt : if not NULL, return cmds count received by this core.
  *     tx_cnt : if not NULL, return cmds count sent by this core.
  * return:
  *     succeed: BK_OK;
  *     failed  : fail code.
  *
  */
void mb_chnl_get_statis(u8 dst_cpu, u32 *rx_cnt, u32 * tx_cnt);


#ifdef __cplusplus
}
#endif

#endif /* _mailbox_channel_h_ */

