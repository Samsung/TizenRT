#ifndef __MBOX0_HAL_H__
#define __MBOX0_HAL_H__

#include <stdint.h>
#include "mbox0_ll.h"

#ifndef BIT
#define BIT(x) (1U << (x))
#endif

#define mbox0_hal_set_reg_0x2_chn_pro_disable(hal, val) mbox0_ll_set_reg_0x2_chn_pro_disable((hal)->hw, val)

#define mbox0_hal_set_chn0_int_wrerr_en(hal, enable) mbox0_ll_set_reg_0x10_int_wrerr_en((hal)->hw, enable)
#define mbox0_hal_set_chn0_int_rderr_en(hal, enable) mbox0_ll_set_reg_0x10_int_rderr_en((hal)->hw, enable)
#define mbox0_hal_set_chn0_int_wrfull_en(hal, enable) mbox0_ll_set_reg_0x10_int_wrfull_en((hal)->hw, enable)
#define mbox0_hal_get_chn0_wrerr_int_sta(hal) mbox0_ll_get_reg_0x10_wrerr_int_sta((hal)->hw)
#define mbox0_hal_get_chn0_rderr_int_sta(hal) mbox0_ll_get_reg_0x10_rderr_int_sta((hal)->hw)
#define mbox0_hal_get_chn0_wrfull_int_sta(hal) mbox0_ll_get_reg_0x10_wrfull_int_sta((hal)->hw)
#define mbox0_hal_get_reg_0x10_value(hal) mbox0_ll_get_reg_0x10_value((hal)->hw)
#define mbox0_hal_set_reg_0x10_value(hal, val) mbox0_ll_set_reg_0x10_value((hal)->hw, val)

#define mbox0_hal_set_chn1_int_wrerr_en(hal, enable) mbox0_ll_set_reg_0x20_int_wrerr_en((hal)->hw, enable)
#define mbox0_hal_set_chn1_int_rderr_en(hal, enable) mbox0_ll_set_reg_0x20_int_rderr_en((hal)->hw, enable)
#define mbox0_hal_set_chn1_int_wrfull_en(hal, enable) mbox0_ll_set_reg_0x20_int_wrfull_en((hal)->hw, enable)
#define mbox0_hal_get_chn1_wrerr_int_sta(hal) mbox0_ll_get_reg_0x20_wrerr_int_sta((hal)->hw)
#define mbox0_hal_get_chn1_rderr_int_sta(hal) mbox0_ll_get_reg_0x20_rderr_int_sta((hal)->hw)
#define mbox0_hal_get_chn1_wrfull_int_sta(hal) mbox0_ll_get_reg_0x20_wrfull_int_sta((hal)->hw)
#define mbox0_hal_get_reg_0x20_value(hal) mbox0_ll_get_reg_0x20_value((hal)->hw)
#define mbox0_hal_set_reg_0x20_value(hal, val) mbox0_ll_set_reg_0x20_value((hal)->hw, val)

#define mbox0_hal_set_chn2_int_wrerr_en(hal, enable) mbox0_ll_set_reg_0x30_int_wrerr_en((hal)->hw, enable)
#define mbox0_hal_set_chn2_int_rderr_en(hal, enable) mbox0_ll_set_reg_0x30_int_rderr_en((hal)->hw, enable)
#define mbox0_hal_set_chn2_int_wrfull_en(hal, enable) mbox0_ll_set_reg_0x30_int_wrfull_en((hal)->hw, enable)
#define mbox0_hal_get_chn2_wrerr_int_sta(hal) mbox0_ll_get_reg_0x30_wrerr_int_sta((hal)->hw)
#define mbox0_hal_get_chn2_rderr_int_sta(hal) mbox0_ll_get_reg_0x30_rderr_int_sta((hal)->hw)
#define mbox0_hal_get_chn2_wrfull_int_sta(hal) mbox0_ll_get_reg_0x30_wrfull_int_sta((hal)->hw)
#define mbox0_hal_get_reg_0x30_value(hal) mbox0_ll_get_reg_0x30_value((hal)->hw)
#define mbox0_hal_set_reg_0x30_value(hal, val) mbox0_ll_set_reg_0x30_value((hal)->hw, val)


typedef enum
{
	MBOX0_HAL_OK = 0,
	MBOX0_HAL_HW_ERR_BASE=0x10,
	MBOX0_HAL_SW_ERR_BASE=0x80,
	MBOX0_HAL_SW_NO_INIT,
	MBOX0_HAL_SW_FIFO_OVERFLOW,
	MBOX0_HAL_SW_PARAM_ERR,
	MBOX0_HAL_SW_FIFO_FULL,
} mbox0_hal_err_t;

typedef struct
{
	mbox0_hw_t * hw;
	uint8_t      fifo_size;
} mbox0_hal_t;

typedef struct
{
	union
	{
		uint8_t src_cpu;
		uint8_t dest_cpu;
	} ;
	
	uint32_t data[2];
} mbox0_message_t;

enum
{
	TX_FIFO_STAT_WR_FULL     = BIT(0),
	TX_FIFO_STAT_RD_ERR      = BIT(1),
	TX_FIFO_STAT_WR_ERR      = BIT(2),
} ;

enum
{
	RX_FIFO_STAT_EMPTY       = BIT(0),
	RX_FIFO_STAT_FULL        = BIT(1),
	RX_FIFO_STAT_NOT_EMPTY   = BIT(2),
} ;

typedef int (* chn_cfg_fifo_func_t)(mbox0_hal_t* hal, uint8_t start, uint8_t len);
typedef int (* chn_send_func_t)(mbox0_hal_t* hal, mbox0_message_t * msg);
typedef int (* chn_recv_func_t)(mbox0_hal_t* hal, mbox0_message_t * msg);
typedef int (* chn_int_enable_func_t)(mbox0_hal_t* hal, uint8_t enable);
typedef uint32_t (* chn_get_int_status_func_t)(mbox0_hal_t* hal);
typedef uint32_t (* chn_get_rx_fifo_stat_func_t)(mbox0_hal_t* hal);
typedef uint32_t (* chn_get_rx_fifo_count_func_t)(mbox0_hal_t* hal);
typedef uint32_t (* chn_clear_tx_fifo_stat_func_t)(mbox0_hal_t* hal);

typedef struct
{
	chn_cfg_fifo_func_t      chn_cfg_fifo;
	chn_send_func_t          chn_send;
	chn_recv_func_t          chn_recv;
	chn_int_enable_func_t    chn_int_enable;
	chn_get_int_status_func_t      chn_get_int_status;
	chn_get_rx_fifo_stat_func_t    chn_get_rx_fifo_stat;
	chn_get_rx_fifo_count_func_t   chn_get_rx_fifo_count;
	chn_clear_tx_fifo_stat_func_t  chn_clear_tx_fifo_stat;
} hal_chn_drv_t;

extern const hal_chn_drv_t  hal_chn0_drv;
extern const hal_chn_drv_t  hal_chn1_drv;
extern const hal_chn_drv_t  hal_chn2_drv;

int mbox0_hal_init(mbox0_hal_t * hal);
int mbox0_hal_dev_init(mbox0_hal_t * hal);
int mbox0_hal_deinit(mbox0_hal_t * hal);
int mbox0_hal_dev_deinit(mbox0_hal_t * hal);

#endif
