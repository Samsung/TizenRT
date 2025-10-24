// Copyright 2020-2021 Beken
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

#pragma once

#include <common/bk_err.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RX_INT_FLAG_GROUP               ((1 << CAN_IE_RIF_POS) | (1 << CAN_IE_RAFIF_POS) | (1 << CAN_IE_RAFIF_POS))
#define TX_INT_FLAG_GROUP               ((1 << CAN_IE_TSIF_POS)| (1 << CAN_IE_TPIF_POS))
#define ERR_INT_FLAG_GROUP              ((1 << CAN_IE_EIF_POS) | (1 << CAN_IE_BEIF_POS) | (1 << CAN_IE_ALIF_POS))
#define TT_INT_FLAG_GROUP               ((1 << POS_bc_TTIF) | (1 << POS_bc_TEIF) | (1 << POS_bc_WTIF))
#define MEM_INT_FLAG_GROUP              ((1 << POS_c8_MDWIF) | (1 << POS_c8_MDEIF) | (1 << POS_c8_MAEIF))
#define SAFE_INT_FLAG_GROUP             ((1 << POS_d0_SEIF) | (1 << POS_d0_SWIF))
#define OTHER_INT_FLAG_GROUP            ((1 << POS_a4_RTIF_ROIF) | (1 << POS_a4_RTIF_AIF))

#define CC_RAM_MEMTYPE                  1
#define CC_RAM_TYPE                     3
#define CC_RAM_ECC                      1
#define CC_RAM_ES                       0
#define CC_RBUF_SLOTS                   16
#define CC_STB_ENABLE                   1
#define CC_STB_SLOTS                    16
#define CC_STB_PRIO                     1
#define CC_ACF_NUMBER                   16
#define CC_CAN_FD                       1
#define CC_UPWARD_COMPATIBILITY         1
#define CC_TTCAN                        1
#define CC_CIA603                       2
#define CC_SAFETY_ENABLE                0

#define CAN_20_MAX_PAYLOAD              8
#define CAN_FD_MAX_PAYLOAD              64

#define MAX_ACF_NUM                     (CC_ACF_NUMBER - 1)

// #define CAN_FD_BOSCH                    0
// #define CAN_FD_ISO                      1

#define FDF_CAN_20                      0
#define FDF_CAN_FD                      1

#define CAN_STAND_ID_MASK               (0x7FF)
#define CAN_EXT_ID_MASK                 CAN_TID_ESI_ID_MASK

#define POS_TTSEN                       CAN_TID_ESI_POS
#define TTSEN_MASK                      CAN_TID_ESI_MASK

#define RSTAT_EMPTY                     0
#define RSTAT_AFWL                      1
#define RSTAT_NOFULL                    2       // >empty and <almost full
#define RSTAT_FULL                      3

#define BUF_REG_NUM                     (64/4)


typedef enum {
	CAN_CHAN_0 = 0,		/**< can gpio44/gpio45/gpuo46 */
	CAN_CHAN_MAX,
} can_channel_t;

typedef enum {
    CAN_BR_125K,	//81%
	CAN_BR_250K,	//81%
	CAN_BR_500K,	//81%
	CAN_BR_800K,	//80%
	CAN_BR_1M,      //81%
	CAN_BR_2M,      //81%
	CAN_BR_4M,      //81%
	CAN_BR_5M,      //77%
} can_bit_rate_e;

typedef struct {
    can_bit_rate_e      br;
    uint32_t            reg_val;
} can_core_br_tab_s;

typedef enum {
	CMD_CAN_MODUILE_INIT,
	CMD_CAN_SET_RX_CALLBACK,
	CMD_CAN_SET_TX_CALLBACK,
	CMD_CAN_RESET_REQ,
	CMD_CAN_ACC_FILTER_SET,
	CMD_CAN_SET_TX_FIFO,
	CMD_CAN_GET_TX_SIZE,
	CMD_CAN_SET_RX_FIFO,
	CMD_CAN_FD_BOSCH_MODE,
	CMD_CAN_GET_RX_FRAME_TAG,
	CMD_CAN_SET_TX_FRAME_TAG,
	CMD_CAN_PTB_INBUF,
	CMD_CAN_TRANS_SWITCH,
	CMD_CAN_STB_INBUF,
	CMD_CAN_SET_ERR_CALLBACK,
	CMD_CAN_BUSOFF_CLR,
	CMD_CAN_GET_KOER,
} can_ctrl_cmd_e;

typedef enum {
	DRIVER_DISABLE = 0,
	DRIVER_ENABLE = 1,
} drv_switch_e;


typedef struct {
	can_bit_rate_e s_speed;
	can_bit_rate_e f_speed;
} can_speed_t;

typedef enum
{
    CAN_ERRINT_BUS          = 0x01,
    CAN_ERRINT_ARB_LOST     = 0x02,
    CAN_ERRINT_PASSIVE      = 0x04,
    CAN_ERRINT_WARN_LIM     = 0x08,
} can_err_int_e;

typedef enum {
    CAN_PROTO_20,
    CAN_PROTO_FD,
} can_protocol_e;

typedef enum {
    CAN_FORM_STD,
    CAN_FORM_EXT,
} can_form_e;

typedef enum {
    CAN_FRAME_DATA,
    CAN_FRAME_REMT,
} can_frame_e;

typedef enum {
    CAN_BIT_RATE_SLOW,
    CAN_BIT_RATE_FAST,
} can_brate_e;

typedef enum {
    CAN_ACCEPT_BOTH = 0,
    CAN_ACCEPT_STANDARD,
    CAN_ACCEPT_EXTENDED,
} can_acc_mask_ide_e;

typedef enum {
    CAN_RESET_REQ_EN,
    CAN_RESET_REQ_NO,
} can_loc_reset_c;

typedef enum {
    CAN_TPE       = 0x01,
    CAN_TPA       = 0x02,
    CAN_TSONE     = 0x04,
    CAN_TSALL     = 0x08,
    CAN_TSA       = 0x10,
} can_trans_switch_c;

typedef enum {
    CAN_KOER_NO       = 0x00,
    CAN_KOER_BIT      = 0x01,
    CAN_KOER_FORM     = 0x02,
    CAN_KOER_STUFF    = 0x03,
    CAN_KOER_ACK      = 0x04,
    CAN_KOER_CRC      = 0x05,
    CAN_KOER_OTHER    = 0x06,
    CAN_KOER_NOUSE    = 0x07,
} can_koer_code_e;

typedef struct {
    can_acc_mask_ide_e  aide;
    uint32_t            code;           // no more than 29 bits
    uint32_t            mask;           // 1 disable, 0 enable
    uint8_t             seq;            // 0 to MAX_ACF_NUM, default 0 accept all
    drv_switch_e        onoff;
} can_acc_filter_cmd_s;

typedef struct {
    uint32_t            id;
    uint8_t             ide;
    uint8_t             rtr;            // only 2.0
    uint8_t             fdf;
    uint8_t             brs;            // only fd
    uint8_t             esi;            // only fd rx
    uint8_t             ttsen;
} can_frame_tag_t;

typedef struct {
    can_frame_tag_t     tag;
    uint32_t            size;
    uint8_t             *data;
} can_frame_s;

typedef void (*can_callback)(void *param);
typedef struct {
    can_callback        cb;
    void                *param;
} can_callback_des_t;

typedef uint32_t (*can_fifo_f)(uint8_t *buf, uint32_t len);
typedef uint32_t (*can_size_f)(void);

typedef struct
{
    can_callback_des_t  tx_cb;
    can_callback_des_t  rx_cb;
    can_callback_des_t  err_cb;
    can_fifo_f          tx_fifo_get;
    can_fifo_f          rx_fifo_put;
    can_frame_tag_t     tx_frame_tag;
    can_frame_tag_t     rx_frame_tag;
    can_size_f          tx_size;
} can_hal_t;

#ifdef __cplusplus
}
#endif

