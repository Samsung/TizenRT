#include <common/bk_include.h>
#include "bk_arm_arch.h"

#include "bk_sdio.h"
#include "sdio.h"
#include "sutil.h"
#include "sdma_pub.h"
#include "bk_drv_model.h"
#include "bk_uart.h"
#include "bk_icu.h"
#include <os/mem.h>
//#include "co_math.h"
#include "bk_gpio.h"
#include "sdma.h"
//#include "mac.h"
#include"icu_driver.h"
#if CFG_WMM_PATCH
//#include "mm_timer.h"
//#include "ke_timer.h"
#endif

#if defined(CONFIG_SDIO) || defined(CONFIG_SDIO_TRANS)
#if defined(CONFIG_SDIO_BLOCK_512)
#include "sdio_intf.h"
UINT8 beken_tx_sdio_s = 1;
UINT8 da_hd_err = 0;
#endif
STATIC SDIO_S sdio;

#ifdef SDIO_MEM_DEBUG
#define SDIO_MAGIC_TAIL_STR		"\xAA\xBB\xCC\xDD\xEE\xFF"
#endif

int sdio_ac_credits[AC_MAX];

STATIC const DD_OPERATIONS sdio_op = {
	sdio_open,
	sdio_close,
	sdio_read,
	sdio_write,
	sdio_ctrl
};

SDIO_NODE_PTR sdio_alloc_valid_node(UINT32 buf_size)
{
	SDIO_NODE_PTR temp_node_ptr;
	SDIO_NODE_PTR mem_node_ptr = 0;

	if (0 == buf_size)
		goto av_exit;

#ifdef SUPPORT_STM32
	buf_size = su_align_power2(buf_size);
#endif

	temp_node_ptr = su_pop_node(&sdio.free_nodes);
	if (temp_node_ptr) {
#ifdef SDIO_PREALLOC
		BK_ASSERT(buf_size <= 1600);
		temp_node_ptr->addr   = temp_node_ptr->orig_addr + CONFIG_MSDU_RESV_HEAD_LENGTH;
		temp_node_ptr->length = buf_size;
		mem_node_ptr = temp_node_ptr;
#else
		UINT8 *buff_ptr = (UINT8 *)os_malloc(CONFIG_MSDU_RESV_HEAD_LENGTH + buf_size
											 + CONFIG_MSDU_RESV_TAIL_LEN
											 + MALLOC_MAGIC_LEN);
		if (buff_ptr) {
#ifdef SDIO_MEM_DEBUG
			if ((UINT32)buff_ptr > 0x00400020 + 262112)
				BK_ASSERT(0);
#endif
			//if(MALLOC_MAGIC_LEN)
			//{
			//buff_ptr[(buf_size + MALLOC_MAGIC_LEN) - 1] = magic_byte0 ++; // MALLOC_MAGIC_BYTE0;
			//}

			temp_node_ptr->orig_addr = buff_ptr;
			temp_node_ptr->addr   = (UINT8 *)((UINT32)buff_ptr + CONFIG_MSDU_RESV_HEAD_LENGTH);
			temp_node_ptr->length = buf_size;

			temp_node_ptr->ac = -1;
			mem_node_ptr = temp_node_ptr;
		} else {
			fatal_prf("alloc_null:%d\n", buf_size);
			su_push_node(&sdio.free_nodes, temp_node_ptr);
		}
#endif
	} else
		fatal_prf("*");

av_exit:
	return mem_node_ptr;
}


void sdio_free_valid_node(SDIO_NODE_PTR node_ptr)
{
	//UINT8 *buff_ptr;

	//buff_ptr = node_ptr->addr;
	if (MALLOC_MAGIC_LEN) {
		//BK_ASSERT(buff_ptr[node_ptr->length + MALLOC_MAGIC_LEN - 1] == (magic_byte00 ++)/*MALLOC_MAGIC_BYTE0*/);
		//buff_ptr[node_ptr->length + MALLOC_MAGIC_LEN - 1] = MALLOC_MAGIC_BYTE1;
		//os_memset(node_ptr->addr, MALLOC_MAGIC_BYTE1, node_ptr->length + MALLOC_MAGIC_LEN - 1);
	}

#ifndef SDIO_PREALLOC
	os_free(node_ptr->orig_addr);
	node_ptr->orig_addr   = 0;
#endif
	node_ptr->addr   = 0;
	node_ptr->length = 0;

	su_push_node(&sdio.free_nodes, node_ptr);
}

#define SDIO_PATCH
#ifdef SDIO_PATCH
static UINT8 null_data[8] = {0};
#endif

/* in IRQ */
void sdio_cmd_handler(void *buf, UINT32 len)
{
	UINT32 count;
	UINT32 txlen;
	SDIO_PTR sdio_ptr;
	SDIO_DCMD_PTR cmd_ptr;
	SDIO_CMD_PTR reg_cmd_ptr;
	SDIO_NODE_PTR mem_node_ptr = NULL;
#ifdef SDIO_MEM_DEBUG
	int tnf = 0, rnf = 0;
	UINT8 *end;
#endif

	BK_ASSERT(NULL != buf);
	BK_ASSERT(buf == &sdio.cmd);
	cmd_ptr = (SDIO_DCMD_PTR)buf;
	reg_cmd_ptr = (SDIO_CMD_PTR)buf;

	//BK_LOG_RAW("C\n");

	if (sdio_debug_level) {
		int pl;
		u8 *cbuf;
		cbuf = buf;

		pl = len;
		if (pl > 16)
			pl = 16;
		print_hex_dump("CMD: ", cbuf, pl);
	}

	sdma_fake_stop_dma();

	sdio_ptr = &sdio;
	switch (cmd_ptr->op_code) {

	/* Host -> Target */
	case OPC_WR_DTCM : {
#ifdef SDIO_MEM_DEBUG
		if (sdio_ptr->rx_len) {
			BK_LOG_RAW("RnF\n");
			rnf = 1;
		}
#endif
		sdio_ptr->rx_len = cmd_ptr->data_len;
		count = su_get_node_count(&sdio.rxing_list);
#ifdef SDIO_MEM_DEBUG
		if (rnf)
			SDIO_LOGI("rc %d\n", count);
#endif

		if (count) {
			mem_node_ptr = su_pop_node(&sdio.rxing_list);
			/* Delete current node */
			sdio_free_valid_node(mem_node_ptr);
		}

		mem_node_ptr = sdio_alloc_valid_node(cmd_ptr->data_len + 20/* dummy len*/);

		sdio_ptr->rc_len = 0;
		sdio_ptr->rx_transaction_len = 0;
		if (mem_node_ptr) {
			sdio_ptr->next_seq = 0;
			//sdio_ptr->rc_len = 0;
			sdio_ptr->rx_status = RX_NODE_OK;
			//BK_ASSERT(cmd_ptr->size == cmd_ptr->data_len);
			sdio_ptr->rx_transaction_len = MIN(BLOCK_LEN - SDIO_TAIL_LEN, cmd_ptr->data_len);

			su_push_node(&sdio.rxing_list, mem_node_ptr);
#ifdef SDIO_MEM_DEBUG
			end = mem_node_ptr->addr + cmd_ptr->data_len + SDIO_TAIL_LEN + 2;
			os_memcpy(end, SDIO_MAGIC_TAIL_STR, 6);
#endif
			sdio_ptr->rx_addr = (UINT32)mem_node_ptr->addr;
			sdma_start_rx(mem_node_ptr->addr, sdio_ptr->rx_transaction_len);
		} else
			sdio_ptr->rx_status = RX_NO_NODE;
		break;
	}

	case OPC_RD_DTCM : {
#ifdef SDIO_MEM_DEBUG
		if (sdio_ptr->tx_len) {
			BK_LOG_RAW("TnF %d\n", sdio_ptr->tx_len);
			tnf = 1;
		}
#endif
#ifdef SDIO_PATCH
		if (su_get_node_count(&sdio.tx_dat) == 0)
			sdio_ctrl(SDIO_CMD_CLEAR_TX_VALID, 0);
#else
		if (su_get_node_count(&sdio.tx_dat) <= 1)
			sdio_ctrl(SDIO_CMD_CLEAR_TX_VALID, 0);
#endif
		//sdio_ptr->tx_len = cmd_ptr->data_len;

		count = su_get_node_count(&sdio.txing_list);
#ifdef SDIO_MEM_DEBUG
		if (tnf)
			BK_LOG_RAW("tc %d\n", count);
#endif

		if (count >= 2)
			SDIO_LOGW("txing_list:%d\n", count);
		BK_ASSERT(count < 2);

		if (count) {
			mem_node_ptr = su_pop_node(&sdio.txing_list);
			sdio_free_valid_node(mem_node_ptr);
		}

		mem_node_ptr = su_pop_node(&sdio.tx_dat);

		if (mem_node_ptr) {
#if defined(CONFIG_SDIO_CREDITS)
			int ac = mem_node_ptr->ac;
			if (ac >= 0)
				sdio_ac_credits[ac]++;
#endif

			sdio_ptr->tc_len = 0;
			sdio_ptr->tx_status = TX_NODE_OK;
			//BK_LOG_RAW("l:%d/%d\n", sdio_ptr->tx_len, mem_node_ptr->length);

			//if (mem_node_ptr->length < sdio_ptr->tx_len)
			sdio_ptr->tx_len = mem_node_ptr->length;
			txlen = sdio_ptr->tx_len;

			sdio_ctrl(SDIO_CMD_SET_TX_LEN, &txlen);

			sdio_ptr->tx_transaction_len = MIN(BLOCK_LEN, txlen);
			su_push_node(&sdio.txing_list, mem_node_ptr);
			sdma_start_tx(mem_node_ptr->addr, sdio_ptr->tx_transaction_len);
		} else {
#ifdef SDIO_PATCH
			txlen = sizeof(null_data);
			sdio_ctrl(SDIO_CMD_SET_TX_LEN, &txlen);
			sdma_start_tx(null_data, sizeof(null_data));
#endif
			sdio_ptr->tx_status = TX_NO_NODE;
		}

		break;
	}

	case OPC_WR_REG : {
		UINT8 reg_numb = (reg_cmd_ptr->len - 4) >> 3;
		UINT8 i;
		UINT32 reg_addr, reg_val;
		for (i = 0; i < reg_numb; i++) {
			reg_addr = reg_cmd_ptr->content[i];
			reg_val = reg_cmd_ptr->content[i + reg_numb];
			*((UINT32 *)reg_addr) = reg_val;
		}

		break;
	}

	case OPC_RD_REG : {
		UINT8 reg_numb = (reg_cmd_ptr->len - 4) >> 2;
		UINT8 i;
		UINT32 reg_addr, reg_val[16];
		for (i = 0; i < reg_numb; i++) {
			reg_addr = reg_cmd_ptr->content[i];
			reg_val[i] = *((UINT32 *)reg_addr);
		}

		sdio_ptr->transaction_len = MIN(64, reg_numb << 2);
		sdma_start_tx((UINT8 *)reg_val, sdio_ptr->transaction_len);

		break;
	}

	default: {
		SDIO_LOGW("Exceptional cmd of sdio\n");
		break;
	}
	}
}


void sdio_tx_cb(void)
{
	UINT32 remain;
	SDIO_PTR sdio_ptr;
	SDIO_NODE_PTR mem_node_ptr;
	sdio_ptr = &sdio;

	//BK_LOG_RAW("T\n");
	mem_node_ptr = su_pop_node(&sdio.txing_list);
	if (0 == mem_node_ptr && (TX_NO_NODE == sdio_ptr->tx_status)) {
#ifdef SDIO_MEM_DEBUG
		UINT32 addr = REG_READ(REG_SDMA_ADDR);
#ifdef SDIO_PATCH
		if (addr == (UINT32)null_data)
			sdio_ptr->tx_len = 0;
#endif
#endif
		SDIO_LOGI("tx_cb_no_node\n");
		sdio_ptr->tx_status = TX_NODE_OK;
		return;
	}

	BK_ASSERT(mem_node_ptr);
	sdio_ptr->tc_len += sdio_ptr->tx_transaction_len;

	if (sdio_ptr->tc_len >= mem_node_ptr->length) {
		/* all data are sent */
		if (mem_node_ptr->callback)
			(mem_node_ptr->callback)(mem_node_ptr->Lparam, mem_node_ptr->Rparam);

		mem_node_ptr->callback = NULLPTR;
		mem_node_ptr->Lparam   = NULL;
		mem_node_ptr->Rparam   = NULL;
		sdma_fake_stop_dma();
		sdio_ptr->tx_len = 0;

		sdio_free_valid_node(mem_node_ptr);
	} else {
		remain = sdio_ptr->tx_len - sdio_ptr->tc_len;
		sdio_ptr->tx_transaction_len = MIN(BLOCK_LEN, remain);

		su_push_node(&sdio.txing_list, mem_node_ptr);
		sdma_start_tx(mem_node_ptr->addr + sdio_ptr->tc_len, sdio_ptr->tx_transaction_len);
	}
}


/**
 * SDMA has received packets.
 *
 * @count: SDIO/DMA receive data counter.
 */
void sdio_rx_cb(UINT32 count)
{
	UINT32 remain;
	SDIO_PTR sdio_ptr;
	SDIO_NODE_PTR mem_node_ptr;
	struct stm32_frame_hdr *hdr;
	sdio_ptr = &sdio;
	UINT32 addr;
#ifdef SDIO_MEM_DEBUG
	UINT8 *buf;
#endif

	mem_node_ptr = su_pop_node(&sdio.rxing_list);
	if (!mem_node_ptr && sdio_ptr->rx_status == RX_NO_NODE) {
		SDIO_LOGW("rx_cb_no_node\n");
		sdio_ptr->rx_status = RX_NODE_OK;
		return;
	}

	if (!mem_node_ptr) {
		SDIO_LOGW("rx_no_nd %d\n", sdio_ptr->rc_len);
		SDIO_LOGW("rxing_list:%d txing_list:%d\n", su_get_node_count(&sdio.rxing_list),
				  su_get_node_count(&sdio.txing_list));

		sdio_ptr->rx_status = RX_NODE_OK;
		return;
	}

	if (unlikely(sdio_ptr->rc_len > 2000))
		BK_ASSERT(0);

	if (unlikely(sdio_ptr->rx_transaction_len > 512))
		BK_ASSERT(0);

	addr = REG_READ(REG_SDMA_ADDR);

	if (unlikely(sdio_ptr->rx_addr != addr)) {
		BK_LOG_RAW("rx_addr 0x%x, tx_addr 0x%x, hw addr 0x%x\n",
				  sdio_ptr->rx_addr, sdio_ptr->tx_addr, addr);
		BK_ASSERT(0);
	}

	/* check data */
	hdr = (struct stm32_frame_hdr *)(mem_node_ptr->addr + sdio_ptr->rc_len + sdio_ptr->rx_transaction_len);
	if (unlikely(hdr->seq != sdio_ptr->next_seq)) {
		BK_LOG_RAW("se %d %d\n", hdr->seq, sdio_ptr->next_seq);
		return;
	}
	if (unlikely(hdr->type != 0xAB)) {
		BK_LOG_RAW("te\n");
		return;
	}
	if (unlikely(hdr->len != sdio_ptr->rx_transaction_len)) {
		BK_LOG_RAW("le %d %d\n", hdr->len, sdio_ptr->rx_transaction_len);
		return;
	}

	sdio_ptr->next_seq++;

	//sdio_ptr->rc_len += count;	count is function 1's data len
	sdio_ptr->rc_len += sdio_ptr->rx_transaction_len;
	if (unlikely(sdio_ptr->rc_len > 2000))
		BK_ASSERT(0);

#if 0
	if (hdr->remain > sdio_ptr->rx_len - sdio_ptr->rc_len) {
		BK_LOG_RAW("ex: %d %d\n", hdr->remain > sdio_ptr->rx_len, sdio_ptr->rc_len);
		return;
	}
#endif

	if (sdio_ptr->rc_len >= sdio_ptr->rx_len) {
#ifdef SDIO_MEM_DEBUG
		buf = mem_node_ptr->addr + sdio_ptr->rx_len + SDIO_TAIL_LEN + 2;
		if (os_memcmp(buf, SDIO_MAGIC_TAIL_STR, 6)) {
			BK_LOG_RAW("sdma corrupt memory\n");
			BK_ASSERT(0);
		}
		os_memcpy(buf, "\xEE\xEE\xEE\xEE", 4);
		buf += 4;
		REG_PL_WR(buf, mem_node_ptr);
		//buf += 4;
		//REG_PL_WR(buf, mem_node_ptr->addr);
		buf += 4;
		REG_PL_WR(buf, sdio_ptr->rx_len);
		buf += 4;
		REG_PL_WR(buf, sdio_ptr->rc_len);
		//buf += 4;
		//REG_PL_WR(buf, sdio_ptr->rx_transaction_len);
		mm_magic_match(mem_node_ptr->orig_addr);
#endif

		su_push_node(&sdio.rx_dat, mem_node_ptr);
		if (sdio_ptr->rx_cb)
			(sdio_ptr->rx_cb)();
		sdma_fake_stop_dma();

		sdio_ptr->rx_len = 0;
		sdio_ptr->rx_addr = 0;
	} else {
		remain = sdio_ptr->rx_len - sdio_ptr->rc_len;
		sdio_ptr->rx_transaction_len = MIN(BLOCK_LEN - SDIO_TAIL_LEN, remain);
		su_push_node(&sdio.rxing_list, mem_node_ptr);
		sdio_ptr->rx_addr = (UINT32)(mem_node_ptr->addr + sdio_ptr->rc_len);
		sdma_start_rx(mem_node_ptr->addr + sdio_ptr->rc_len, sdio_ptr->rx_transaction_len);
	}
}

void sdio_register_rx_cb(FUNCPTR func)
{
	sdio.rx_cb = func;		// sdio_rxed_trigger_evt
}

static void sdio_intfer_gpio_config(void)
{
	UINT32 param;

	param = GFUNC_MODE_SD_DMA;
	sddev_control(DD_DEV_TYPE_GPIO, CMD_GPIO_ENABLE_SECOND, &param);
}

/**********************************************************************/
void sdio_init(void)
{
	sdio_intfer_gpio_config();

	os_memset(&sdio, 0, sizeof(sdio));

	INIT_LIST_HEAD(&sdio.tx_dat);
	INIT_LIST_HEAD(&sdio.rx_dat);
	INIT_LIST_HEAD(&sdio.txing_list);
	INIT_LIST_HEAD(&sdio.rxing_list);

	su_init(&sdio);

#if defined(CONFIG_SDIO_BLOCK_512)
	sdio.rc_len = 0;
	sdio.tc_len = 0;
#endif
	sdma_register_handler(sdio_tx_cb, sdio_rx_cb, sdio_cmd_handler);
	sdma_init();

	ddev_register_dev(DD_DEV_TYPE_SDIO, (DD_OPERATIONS *)&sdio_op);

	//sdio.int_gpio = GPIO32;
	sdio.int_gpio = GPIO24;
	BkGpioInitialize(sdio.int_gpio, OUTPUT_NORMAL);

	sdio_ac_credits[AC_BK] = 8;
	sdio_ac_credits[AC_BE] = 10;
	sdio_ac_credits[AC_VI] = 12;
	sdio_ac_credits[AC_VO] = 14;

	SDIO_LOGI("sdio_init\n");
}


void sdio_exit(void)
{
	BkGpioFinalize(sdio.int_gpio);

	sdma_uninit();

	ddev_unregister_dev(DD_DEV_TYPE_SDIO);
}

UINT32 sdio_open(UINT32 op_flag)
{
	UINT32 reg;

	sdma_open();
	sdma_start_cmd((UINT8 *)&sdio.cmd, sizeof(sdio.cmd));

	reg = *((volatile UINT32 *)((0x00802000 + 16 * 4)));
	if (reg & (1 << FIQ_SDIO_DMA)) {
		//BK_ASSERT(0);
		SDIO_LOGI("already enabled sdio fiq\n");
	}

	icu_enable_sdio_dma_interrupt();

	return SDIO_SUCCESS;
}

UINT32 sdio_close(void)
{
	sdma_close();

	return SDIO_SUCCESS;
}

UINT32 sdio_read(char *user_buf, UINT32 count, UINT32 op_flag)
{
	UINT32 ret;
	UINT32 len;
	SDIO_NODE_PTR mem_node_ptr;

	ret = SDIO_FAILURE;
	if (H2S_RD_SYNC == op_flag) {
		if (!user_buf || !count)
			goto rd_exit;

		mem_node_ptr = su_pop_node(&sdio.rx_dat);
		if (mem_node_ptr) {
			len = MIN(count, mem_node_ptr->length);
			os_memcpy(user_buf, mem_node_ptr->addr, len);
			ret = mem_node_ptr->length;

			sdio_free_valid_node(mem_node_ptr);
		} else
			ret = SDIO_FAILURE;
	} else if (H2S_RD_SPECIAL == op_flag) {
		mem_node_ptr = su_pop_node(&sdio.rx_dat);
		if (mem_node_ptr) {
			mem_node_ptr->Lparam = &sdio.free_nodes;
			mem_node_ptr->Rparam = mem_node_ptr;

			ret = (UINT32)mem_node_ptr;
		} else
			ret = SDIO_FAILURE;
	}

rd_exit:
	return ret;
}

static void sdio_set_interrupt_host()
{
	BkGpioOutputHigh(sdio.int_gpio);
}

static void sdio_clear_interrupt_host()
{
	BkGpioOutputLow(sdio.int_gpio);
}

UINT32 sdio_write(char *user_buf, UINT32 count, UINT32 op_flag)
{
	UINT32 ret;
	SDIO_NODE_PTR mem_node_ptr;

	ret = SDIO_FAILURE;

	if (S2H_WR_SYNC == op_flag) {
		if (!user_buf || !count)
			goto exit_wr;

		mem_node_ptr = sdio_alloc_valid_node(count);
		if (mem_node_ptr) {
			//strcpy(mem_node_ptr->caller, "sdio_write");
			os_memcpy(mem_node_ptr->addr, user_buf, count);
			//BK_LOG_RAW("%s %d: len %d\n", __func__, __LINE__, mem_node_ptr->length);
			su_push_node(&sdio.tx_dat, mem_node_ptr);
			sdio_ctrl(SDIO_CMD_SET_TX_VALID, 0);

			ret = SDIO_SUCCESS;
		} else
			ret = SDIO_FAILURE;
	} else  if (S2H_WR_SPECIAL == op_flag) {
		if (!user_buf || !count)
			goto exit_wr;

		mem_node_ptr = (SDIO_NODE_PTR)user_buf;
		su_push_node(&sdio.tx_dat, mem_node_ptr);
		sdio_ctrl(SDIO_CMD_SET_TX_VALID, 0);

		ret = SDIO_SUCCESS;
	}

exit_wr:
	return ret;
}


UINT32 sdio_ctrl(UINT32 cmd, void *param)
{
	UINT32 ret;
	SDIO_NODE_PTR mem_node_ptr;

	ret = SDIO_SUCCESS;

	switch (cmd) {
	case SDIO_CMD_PUSH_FREE_NODE:
		mem_node_ptr = (SDIO_NODE_PTR)param;

		sdio_free_valid_node(mem_node_ptr);
		break;

	case SDIO_CMD_GET_FREE_NODE: {
		struct get_free_node_param *_param = (struct get_free_node_param *)param;
#if defined(CONFIG_SDIO_CREDITS)
		int ac = mac_tid2ac[_param->tid];
		int credits;
		GLOBAL_INT_DECLARATION();

		GLOBAL_INT_DISABLE();
		credits = sdio_ac_credits[ac];
		GLOBAL_INT_RESTORE();

		if (credits <= 0)
			mem_node_ptr = 0;
		else {
			mem_node_ptr = sdio_alloc_valid_node(_param->size);
			if (mem_node_ptr) {
				mem_node_ptr->ac = ac;
				GLOBAL_INT_DISABLE();
				sdio_ac_credits[ac]--;
				GLOBAL_INT_RESTORE();
			}
		}
#else /* !CONFIG_SDIO_CREDITS */
		mem_node_ptr = sdio_alloc_valid_node(_param->size);
#endif
#if 0
		if (su_get_node_count(&sdio.tx_dat) > (CELL_COUNT >> 1))
			mem_node_ptr = NULL;
		else
			mem_node_ptr = sdio_alloc_valid_node(_param->size, _param->force);
#endif
		//if (mem_node_ptr)
		//	strcpy(mem_node_ptr->caller, "rw");

		ret = (UINT32)mem_node_ptr;
	}
	break;

	case SDIO_CMD_REG_RX_CALLBACK:
		sdio_register_rx_cb((FUNCPTR)param);
		break;

	case SDIO_CMD_GET_CNT_FREE_NODE:
		*((UINT32 *)param) = su_get_node_count(&sdio.free_nodes);
		break;

	case SDIO_CMD_CLEAR_TX_VALID:
		sdma_clr_tx_valid();
		sdio_clear_interrupt_host();
		break;

	case SDIO_CMD_PEEK_S2H_COUNT:
		ret = su_get_node_count(&sdio.tx_dat);
		break;

	case SDIO_CMD_PEEK_H2S_COUNT:
		ret = su_get_node_count(&sdio.rx_dat);
		break;

	case SDIO_CMD_SET_TX_VALID:
		sdma_set_tx_valid();
		sdio_set_interrupt_host();
		break;

	case SDIO_CMD_SET_TX_LEN:
		sdma_set_tx_dat_count(*(UINT32 *)param);
		break;

	default:
		break;
	}

	return ret;
}

#endif

