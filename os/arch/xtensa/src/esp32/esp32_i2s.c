/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>
#include <assert.h>
#include <queue.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/kmalloc.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>
#include <tinyara/audio/audio.h>
#include <tinyara/audio/i2s.h>

#include "irq/irq.h"

#include <arch/board/board.h>
#include "arch/chip/i2s_struct.h"
#include "arch/chip/gpio_struct.h"

#include "chip/periph_defs.h"
#include "chip/esp32_soc.h"
#include "chip/esp32_iomux.h"
#include "chip/esp32_gpio_sigmap.h"
#include "chip/esp32_i2s_reg.h"
#include "chip/esp32_efuse_reg.h"
#include "rom/esp32_gpio.h"
#include "rom/lldesc.h"
#include "esp32_cpuint.h"
#include "esp32_config.h"
#include "esp32_gpio.h"
#include "periph_ctrl.h"
#include "esp32_i2s.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/
#define I2S_MAX_BUFFER_SIZE     (4096 - 8)	//the maximum RAM limited by lldesc
#define PLL_CLK                 (320000000)
#define I2S_BASE_CLK            (PLL_CLK / 2)
#define I2S_FULL_DUPLEX_SLAVE_MODE_MASK   (I2S_MODE_TX | I2S_MODE_RX | I2S_MODE_SLAVE)
#define I2S_FULL_DUPLEX_MASTER_MODE_MASK  (I2S_MODE_TX | I2S_MODE_RX | I2S_MODE_MASTER)

#define APLL_MIN_FREQ (250000000)
#define APLL_MAX_FREQ (500000000)
#define APLL_I2S_MIN_RATE (10675)	//in Hz, I2S Clock rate limited by hardware

#ifndef CONFIG_SCHED_WORKQUEUE
#error Work queue support is required (CONFIG_SCHED_WORKQUEUE)
#endif

#ifndef CONFIG_AUDIO
#error CONFIG_AUDIO required by this driver
#endif

#ifndef CONFIG_ESP32_I2S_MAXINFLIGHT
#define CONFIG_ESP32_I2S_MAXINFLIGHT 32
#endif

/* Assume no RX/TX support until we learn better */
#undef I2S_HAVE_RX
#undef I2S_HAVE_TX

#if defined(CONFIG_ESP32_I2S)

/* Check for I2S RX support */
#if defined(CONFIG_ESP32_I2S_RX)
#define I2S_HAVE_RX     1
#endif

/* Check for I2S TX support */
#if defined(CONFIG_ESP32_I2S_TX)
#define I2S_HAVE_TX     1
#endif

#ifndef CONFIG_ESP32_I2S_DATALEN
#define CONFIG_ESP32_I2S_DATALEN        (I2S_BITS_PER_SAMPLE_16BIT)
#endif

#ifndef CONFIG_ESP32_I2S_SAMPLERATE
#define CONFIG_ESP32_I2S_SAMPLERATE     (44100)
#endif

#ifndef CONFIG_ESP32_I2S_CHANNEL_FMT
#define CONFIG_ESP32_I2S_CHANNEL_FMT    (I2S_CHANNEL_FMT_RIGHT_LEFT)
#endif

#ifndef CONFIG_ESP32_I2S_COMM_FMT
#define CONFIG_ESP32_I2S_COMM_FMT       I2S_COMM_FORMAT_I2S
#endif

#ifndef CONFIG_ESP32_I2S_INTR_LEVEL
#define CONFIG_ESP32_I2S_INTR_LEVEL    (1)
#endif

#endif

#ifdef CONFIG_DEBUG
#define CONFIG_DEBUG_FEATURES
#endif

#define DEBUG_I2S_DRIVER    0

#undef i2serr
#undef i2sinfo
#if defined(DEBUG_I2S_DRIVER) && (0 < DEBUG_I2S_DRIVER)
#define i2sinfo(format, ...)   printf(format, ##__VA_ARGS__)
#define i2serr(format, ...)   printf(format, ##__VA_ARGS__)
#else
#define i2sinfo(format, ...)
//#define i2serr(format, ...)
#endif
#define i2serr(format, ...)   printf(format, ##__VA_ARGS__)

#if defined(I2S_HAVE_RX) || defined(I2S_HAVE_TX)
/* Default pins */
#define CONFIG_I2S0_BCK_PIN_DEFAULT     26
#define CONFIG_I2S0_LRCK_PIN_DEFAULT    25
#define CONFIG_I2S0_DOUT_PIN_DEFAULT    22
#define CONFIG_I2S0_DIN_PIN_DEFAULT     23

#define CONFIG_I2S1_BCK_PIN_DEFAULT     13
#define CONFIG_I2S1_LRCK_PIN_DEFAULT    14
#define CONFIG_I2S1_DOUT_PIN_DEFAULT    15
#define CONFIG_I2S1_DIN_PIN_DEFAULT     16

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* I2S configuration parameters for i2s_param_config function */
typedef struct {
	i2s_mode_t mode;			/*!< I2S work mode */
	int sample_rate;			/*!< I2S sample rate */
	i2s_bits_per_sample_t bits_per_sample;	/*!< I2S bits per sample */
	i2s_channel_fmt_t channel_format;	/*!< I2S channel format */
	i2s_comm_format_t communication_format;	/*!< I2S communication format */

	int int_alloc_level;		/*INT level */

	bool use_apll;				/*!< I2S using APLL as main I2S clock, enable it to get accurate clock */
	int fixed_mclk;				/*!< I2S using fixed MCLK output. If use_apll = true and fixed_mclk > 0, then the clock output for i2s is fixed and equal to the fixed_mclk value. */
} i2s_config_t;

/* I2S pin number for i2s_set_pin */
typedef struct {
	int bck_io_num;				/*!< BCK in out pin */
	int ws_io_num;				/*!< WS in out pin */
	int data_out_num;			/*!< DATA out pin */
	int data_in_num;			/*!< DATA in pin */
} i2s_pin_config_t;

/* I2S buffer container */
struct esp32_buffer_s {
	struct esp32_buffer_s *flink;	/* Supports a singly linked list */

	i2s_callback_t callback;	/* Function to call when the transfer completes */
	void *arg;					/* The argument to be returned with the callback */

	uint32_t timeout;			/* The timeout value to use with DMA transfers */
	struct ap_buffer_s *apb;	/* The audio buffer */
	int result;					/* The result of the transfer */

	lldesc_chain_t desc_chain;	/* Dma description list */
};

/* This structure describes the state of one receiver or transmitter transport */

struct esp32_transport_s {
	WDOG_ID dog;				/* Watchdog that handles DMA timeouts */
	sq_queue_t pend;			/* A queue of pending transfers */
	sq_queue_t act;				/* A queue of active transfers */
	sq_queue_t done;			/* A queue of completed transfers */

	struct work_s work;			/* Supports worker thread operations */

	sem_t isrsem;				/* Assures mutually exclusive acess to I2S */

	int state;					/* Channel error state, 1:busy, 0: idle, <0: error, */
};

/* The state of the one I2S peripheral */

struct esp32_i2s_s {
	struct i2s_dev_s dev;		/* Externally visible I2S interface, must the first element!! */

	i2s_port_t i2s_num;			/* I2S controller register base address */
	uint8_t periph;				/* I2S peripheral ID */

	int intrupt_level;
	int cpu_int;
	int isr_num;				/* isr number */
	xcpt_t isr_handler;			/* irs handler */

	i2s_err_cb_t err_cb;		/* registered error callback function */
	void *err_cb_arg;			/* argiment to return with err cb call */

	sem_t exclsem;				/* Assures mutually exclusive acess to I2S */

	uint8_t rxenab:1;			/* True: RX transfers enabled */
	uint8_t txenab:1;			/* True: TX transfers enabled */

	i2s_mode_t mode;			/*!< I2S work mode */
	int sample_rate;			/*!< I2S sample rate */
	int channel_num;			/*!< Number of channels */
	int bytes_per_sample;		/*!< Bytes per sample */
	int bits_per_sample;		/*!< Bits per sample */

	bool use_apll;				/*!< I2S use APLL clock */
	int fixed_mclk;				/*!< I2S fixed MLCK clock */

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct esp32_transport_s rx;	/* RX transport state */
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct esp32_transport_s tx;	/* TX transport state */
#endif

	/* Pre-allocated pool of buffer containers */
	sem_t bufsem_tx;			/* Buffer wait semaphore */
	struct esp32_buffer_s *freelist_tx;	/* A list a free buffer containers */
	struct esp32_buffer_s containers_tx[CONFIG_ESP32_I2S_MAXINFLIGHT];

	sem_t bufsem_rx;			/* Buffer wait semaphore */
	struct esp32_buffer_s *freelist_rx;	/* A list a free buffer containers */
	struct esp32_buffer_s containers_rx[CONFIG_ESP32_I2S_MAXINFLIGHT];
};
/* I2S interrupt status */
union {
	struct {
		uint32_t rx_take_data:1;
		uint32_t tx_put_data:1;
		uint32_t rx_wfull:1;
		uint32_t rx_rempty:1;
		uint32_t tx_wfull:1;
		uint32_t tx_rempty:1;
		uint32_t rx_hung:1;
		uint32_t tx_hung:1;
		uint32_t in_done:1;
		uint32_t in_suc_eof:1;
		uint32_t in_err_eof:1;
		uint32_t out_done:1;
		uint32_t out_eof:1;
		uint32_t in_dscr_err:1;
		uint32_t out_dscr_err:1;
		uint32_t in_dscr_empty:1;
		uint32_t out_total_eof:1;
		uint32_t reserved17:15;
	};
	uint32_t val;
} int_st[I2S_NUM_MAX];
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register helpers */
#define CONFIG_ESP32_I2S_DUMPBUFFERS   0

#if defined(CONFIG_ESP32_I2S_DUMPBUFFERS) && (0 < CONFIG_ESP32_I2S_DUMPBUFFERS)
#define       i2s_init_buffer(b, s)   memset(b, 0x55, s);
#define       i2s_dump_buffer(m, b, s) lib_dumpbuffer(m, b, s)
#else
#define       i2s_init_buffer(b, s)
#define       i2s_dump_buffer(m, b, s)
#endif

/* Semaphore helpers */
static void i2s_exclsem_take(struct esp32_i2s_s *priv);
#define i2s_exclsem_give(priv) sem_post(&priv->exclsem)

/* Buffer container helpers */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
static struct esp32_buffer_s *i2s_buf_rx_allocate(struct esp32_i2s_s *priv);
static void i2s_buf_rx_free(struct esp32_i2s_s *priv, struct esp32_buffer_s *bfcontainer);
static void i2s_buf_rx_initialize(struct esp32_i2s_s *priv);

static int i2s_rx_start(struct esp32_i2s_s *priv);
static void i2s_rx_worker(void *arg);
static void i2s_rx_schedule(struct esp32_i2s_s *priv, int result);

static void i2s_bufsem_rx_take(struct esp32_i2s_s *priv);
#define i2s_bufsem_rx_give(priv) sem_post(&priv->bufsem_rx)
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
static struct esp32_buffer_s *i2s_buf_tx_allocate(struct esp32_i2s_s *priv);
static void i2s_buf_tx_free(struct esp32_i2s_s *priv, struct esp32_buffer_s *bfcontainer);
static void i2s_buf_tx_initialize(struct esp32_i2s_s *priv);

static int i2s_tx_start(struct esp32_i2s_s *priv);
static void i2s_tx_worker(void *arg);
static void i2s_tx_schedule(struct esp32_i2s_s *priv, int result);

static void i2s_bufsem_tx_take(struct esp32_i2s_s *priv);
#define i2s_bufsem_tx_give(priv) sem_post(&priv->bufsem_tx)
#endif

/* I2S methods (and close friends) */

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits);

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate);

static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits);

static int i2s_stop_transfer(struct i2s_dev_s *dev, i2s_ch_dir_t dir);

static int i2s_start(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir);
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir);

static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);
static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout);

static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg);

int i2s_set_clk(struct esp32_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch);

/****************************************************************************
* External functions
****************************************************************************/

/**
 * @brief Possible main XTAL frequency values.
 *
 * Enum values should be equal to frequency in MHz.
 */
typedef enum {
	RTC_XTAL_FREQ_AUTO = 0,		//!< Automatic XTAL frequency detection
	RTC_XTAL_FREQ_40M = 40,		//!< 40 MHz XTAL
	RTC_XTAL_FREQ_26M = 26,		//!< 26 MHz XTAL
	RTC_XTAL_FREQ_24M = 24,		//!< 24 MHz XTAL
} rtc_xtal_freq_t;

extern rtc_xtal_freq_t rtc_clk_xtal_freq_get();

extern void rtc_clk_apll_enable(bool enable, uint32_t sdm0, uint32_t sdm1, uint32_t sdm2, uint32_t o_div);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const i2s_config_t i2s0_default_config = {
#if defined(CONFIG_ESP32_I2S0_MODE_MASTER) && (0 < CONFIG_ESP32_I2S0_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif

	.sample_rate = CONFIG_ESP32_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_ESP32_I2S_DATALEN,
	.channel_format = CONFIG_ESP32_I2S_CHANNEL_FMT,
	.communication_format = CONFIG_ESP32_I2S_COMM_FMT,
	.int_alloc_level = CONFIG_ESP32_I2S_INTR_LEVEL,
};

static const i2s_config_t i2s1_default_config = {
#if defined(CONFIG_ESP32_I2S1_MODE_MASTER) && (0 < CONFIG_ESP32_I2S1_MODE_MASTER)
	.mode = I2S_MODE_MASTER,
#else
	.mode = I2S_MODE_SLAVE,
#endif

	.sample_rate = CONFIG_ESP32_I2S_SAMPLERATE,
	.bits_per_sample = CONFIG_ESP32_I2S_DATALEN,
	.channel_format = CONFIG_ESP32_I2S_CHANNEL_FMT,
	.communication_format = CONFIG_ESP32_I2S_COMM_FMT,
	.int_alloc_level = CONFIG_ESP32_I2S_INTR_LEVEL,
};

static const i2s_pin_config_t I2S0_pin_default = {
#if !defined(CONFIG_I2S0_BCK_PIN)
	.bck_io_num = CONFIG_I2S0_BCK_PIN_DEFAULT,
#else
	.bck_io_num = CONFIG_I2S0_BCK_PIN,
#endif

#if !defined(CONFIG_I2S0_LRCK_PIN)
	.ws_io_num = CONFIG_I2S0_LRCK_PIN_DEFAULT,
#else
	.ws_io_num = CONFIG_I2S0_LRCK_PIN,
#endif

#if !defined(CONFIG_I2S0_DOUT_PIN)
	.data_out_num = CONFIG_I2S0_DOUT_PIN_DEFAULT,
#else
	.data_out_num = CONFIG_I2S0_DOUT_PIN,
#endif

#if !defined(CONFIG_I2S0_DIN_PIN)
	.data_in_num = CONFIG_I2S0_DIN_PIN_DEFAULT,
#else
	.data_in_num = CONFIG_I2S0_DIN_PIN,
#endif
};

static const i2s_pin_config_t I2S1_pin_default = {
#if !defined(CONFIG_I2S1_BCK_PIN)
	.bck_io_num = CONFIG_I2S1_BCK_PIN_DEFAULT,
#else
	.bck_io_num = CONFIG_I2S1_BCK_PIN,
#endif

#if !defined(CONFIG_I2S1_LRCK_PIN)
	.ws_io_num = CONFIG_I2S1_LRCK_PIN_DEFAULT,
#else
	.ws_io_num = CONFIG_I2S1_LRCK_PIN,
#endif

#if !defined(CONFIG_I2S1_DOUT_PIN)
	.data_out_num = CONFIG_I2S1_DOUT_PIN_DEFAULT,
#else
	.data_out_num = CONFIG_I2S1_DOUT_PIN,
#endif

#if !defined(CONFIG_I2S1_DIN_PIN)
	.data_in_num = CONFIG_I2S1_DIN_PIN_DEFAULT,
#else
	.data_in_num = CONFIG_I2S1_DIN_PIN,
#endif
};

/* I2S device operations */
static const struct i2s_ops_s g_i2sops = {
	/* Receiver methods */

	.i2s_rxsamplerate = i2s_samplerate,
	.i2s_rxdatawidth = i2s_rxdatawidth,
	.i2s_receive = i2s_receive,

	/* Transmitter methods */

	.i2s_txsamplerate = i2s_samplerate,
	.i2s_txdatawidth = i2s_txdatawidth,
	.i2s_send = i2s_send,

	.i2s_stop = i2s_stop,
	.i2s_pause = i2s_pause,
	.i2s_resume = i2s_resume,
	.i2s_err_cb_register = i2s_err_cb_register,
};

static struct esp32_i2s_s *g_i2sdevice[I2S_NUM_MAX] = {NULL};
static i2s_dev_t *I2S[I2S_NUM_MAX] = {&I2S0, &I2S1};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if 1
void ESP32_I2S_Dump_isr(i2s_port_t i2s_num)
{
	i2sinfo("    int_raw=%08x\n", I2S[i2s_num]->int_raw.val);
	i2sinfo("    int_st=%08x\n", I2S[i2s_num]->int_st.val);
	i2sinfo("    int_ena=%08x\n", I2S[i2s_num]->int_ena.val);
}

void ESP32_I2S_Dump_outlink(i2s_port_t i2s_num)
{
	i2sinfo("    out_link=%08x\n", I2S[i2s_num]->out_link.val);
	i2sinfo("    out_eof_des_addr=%08x\n", I2S[i2s_num]->out_eof_des_addr);
	i2sinfo("    out_eof_bfr_des_addr=%08x\n", I2S[i2s_num]->out_eof_bfr_des_addr);
	i2sinfo("    out_link_dscr=%08x\n", I2S[i2s_num]->out_link_dscr);
	i2sinfo("    out_link_dscr_bf0=%08x\n", I2S[i2s_num]->out_link_dscr_bf0);
	i2sinfo("    out_link_dscr_bf1=%08x\n", I2S[i2s_num]->out_link_dscr_bf1);
	i2sinfo("    lc_state1=%08x\n", I2S[i2s_num]->lc_state1);
}

void ESP32_I2S_Dump_inlink(i2s_port_t i2s_num)
{
	i2sinfo("    rx_eof_num=%08x\n", I2S[i2s_num]->rx_eof_num);
	i2sinfo("    in_link=%08x\n", I2S[i2s_num]->in_link.val);
	i2sinfo("    in_eof_des_addr=%08x\n", I2S[i2s_num]->in_eof_des_addr);
	i2sinfo("    in_link_dscr=%08x\n", I2S[i2s_num]->in_link_dscr);
	i2sinfo("    in_link_dscr_bf0=%08x\n", I2S[i2s_num]->in_link_dscr_bf0);
	i2sinfo("    in_link_dscr_bf1=%08x\n", I2S[i2s_num]->in_link_dscr_bf1);
	i2sinfo("    lc_state0=%08x\n", I2S[i2s_num]->lc_state0);
}

void ESP32_I2S_Dump(i2s_port_t i2s_num)
{
	i2sinfo("I2S%d:\n", i2s_num);
	i2sinfo("    conf=%08x\n", I2S[i2s_num]->conf.val);
	i2sinfo("    int_raw=%08x\n", I2S[i2s_num]->int_raw.val);
	i2sinfo("    int_st=%08x\n", I2S[i2s_num]->int_st.val);
	i2sinfo("    int_ena=%08x\n", I2S[i2s_num]->int_ena.val);
	/* int_clr is writable only.
	   i2sinfo("    int_clr=%08x\n", I2S[i2s_num]->int_clr.val); */
	i2sinfo("    timing=%08x\n", I2S[i2s_num]->timing.val);
	i2sinfo("    fifo_conf=%08x\n", I2S[i2s_num]->fifo_conf.val);
	i2sinfo("    rx_eof_num=%08x\n", I2S[i2s_num]->rx_eof_num);
	i2sinfo("    conf_single_data=%08x\n", I2S[i2s_num]->conf_single_data);
	i2sinfo("    conf_chan=%08x\n", I2S[i2s_num]->conf_chan.val);
	i2sinfo("    out_link=%08x\n", I2S[i2s_num]->out_link.val);
	i2sinfo("    in_link=%08x\n", I2S[i2s_num]->in_link.val);
	i2sinfo("    out_eof_des_addr=%08x\n", I2S[i2s_num]->out_eof_des_addr);
	i2sinfo("    in_eof_des_addr=%08x\n", I2S[i2s_num]->in_eof_des_addr);
	i2sinfo("    out_eof_bfr_des_addr=%08x\n", I2S[i2s_num]->out_eof_bfr_des_addr);
	i2sinfo("    in_link_dscr=%08x\n", I2S[i2s_num]->in_link_dscr);
	i2sinfo("    in_link_dscr_bf0=%08x\n", I2S[i2s_num]->in_link_dscr_bf0);
	i2sinfo("    in_link_dscr_bf1=%08x\n", I2S[i2s_num]->in_link_dscr_bf1);
	i2sinfo("    out_link_dscr=%08x\n", I2S[i2s_num]->out_link_dscr);
	i2sinfo("    out_link_dscr_bf0=%08x\n", I2S[i2s_num]->out_link_dscr_bf0);
	i2sinfo("    out_link_dscr_bf1=%08x\n", I2S[i2s_num]->out_link_dscr_bf1);
	i2sinfo("    lc_conf=%08x\n", I2S[i2s_num]->lc_conf.val);
	/* fifo is not printed and keep it as it is.
	   i2sinfo("    out_fifo_push=%08x\n", I2S[i2s_num]->out_fifo_push.val);
	   i2sinfo("    in_fifo_pop=%08x\n", I2S[i2s_num]->in_fifo_pop.val); */
	i2sinfo("    lc_state0=%08x\n", I2S[i2s_num]->lc_state0);
	i2sinfo("    lc_state1=%08x\n", I2S[i2s_num]->lc_state1);
	i2sinfo("    lc_hung_conf=%08x\n", I2S[i2s_num]->lc_hung_conf.val);
	i2sinfo("    conf1=%08x\n", I2S[i2s_num]->conf1.val);
	i2sinfo("    conf2=%08x\n", I2S[i2s_num]->conf2.val);
	i2sinfo("    clkm_conf=%08x\n", I2S[i2s_num]->clkm_conf.val);
	i2sinfo("    sample_rate_conf=%08x\n", I2S[i2s_num]->sample_rate_conf.val);
	i2sinfo("    state=%08x\n", I2S[i2s_num]->state);
}
#endif

/****************************************************************************
 * Name: i2s_exclsem_take
 *
 * Description:
 *   Take the exclusive access semaphore handling any exceptional conditions
 *
 * Input Parameters:
 *   priv - A reference to the I2S peripheral state
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

static void i2s_exclsem_take(struct esp32_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */
	do {
		ret = sem_wait(&priv->exclsem);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
/****************************************************************************
 * Name: i2s_bufsem_rx_take
 *
 * Description:
 *   Take the buffer semaphore handling any exceptional conditions
 *
 * Input Parameters:
 *   priv - A reference to the I2S peripheral state
 *
 * Returned Value:
 *  None
 *
 ****************************************************************************/

static void i2s_bufsem_rx_take(struct esp32_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */
	do {
		ret = sem_wait(&priv->bufsem_rx);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/****************************************************************************
 * Name: i2s_buf_rx_allocate
 *
 * Description:
 *   Allocate a buffer container by removing the one at the head of the
 *   free list
 *
 * Input Parameters:
 *   priv - i2s state instance
 *
 * Returned Value:
 *   A non-NULL pointer to the allocate buffer container on success; NULL if
 *   there are no available buffer containers.
 *
 * Assumptions:
 *   The caller does NOT have exclusive access to the I2S state structure.
 *   That would result in a deadlock!
 *
 ****************************************************************************/

static struct esp32_buffer_s *i2s_buf_rx_allocate(struct esp32_i2s_s *priv)
{
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */
	i2s_bufsem_rx_take(priv);

	/* Get the buffer from the head of the free list */
	flags = irqsave();
	bfcontainer = priv->freelist_rx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */
	priv->freelist_rx = bfcontainer->flink;
	irqrestore(flags);

	return bfcontainer;
}

/****************************************************************************
* Name: i2s_buf_rx_free
*
* Description:
*   Free buffer container by adding it to the head of the free list
*
* Input Parameters:
*   priv - I2S state instance
*   bfcontainer - The buffer container to be freed
*
* Returned Value:
*   None
*
* Assumptions:
*   The caller has exclusive access to the I2S state structure
*
****************************************************************************/

static void i2s_buf_rx_free(struct esp32_i2s_s *priv, struct esp32_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */
	flags = irqsave();
	bfcontainer->flink = priv->freelist_rx;
	priv->freelist_rx = bfcontainer;
	irqrestore(flags);

	/* Wake up any threads waiting for a buffer container */
	i2s_bufsem_rx_give(priv);
}

/****************************************************************************
* Name: i2s_buf_rx_initialize
*
* Description:
*   Initialize the buffer container allocator by adding all of the
*   pre-allocated buffer containers to the free list
*
* Input Parameters:
*   priv - I2S state instance
*
* Returned Value:
*   None
*
* Assumptions:
*   Called early in I2S initialization so that there are no issues with
*   concurrency.
*
****************************************************************************/

static void i2s_buf_rx_initialize(struct esp32_i2s_s *priv)
{
	int i;

	priv->freelist_rx = NULL;
	sem_init(&priv->bufsem_rx, 0, 1);

	for (i = 0; i < CONFIG_ESP32_I2S_MAXINFLIGHT; i++) {
		i2s_buf_rx_free(priv, &priv->containers_rx[i]);
	}
}
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
/****************************************************************************
* Name: i2s_bufsem_tx_take
*
* Description:
*   Take the buffer semaphore handling any exceptional conditions
*
* Input Parameters:
*   priv - A reference to the I2S peripheral state
*
* Returned Value:
*  None
*
****************************************************************************/

static void i2s_bufsem_tx_take(struct esp32_i2s_s *priv)
{
	int ret;

	/* Wait until we successfully get the semaphore.  EINTR is the only
	 * expected 'failure' (meaning that the wait for the semaphore was
	 * interrupted by a signal.
	 */
	do {
		ret = sem_wait(&priv->bufsem_tx);
		DEBUGASSERT(ret == 0 || errno == EINTR);
	} while (ret < 0);
}

/****************************************************************************
* Name: i2s_buf_tx_allocate
*
* Description:
*   Allocate a buffer container by removing the one at the head of the
*   free list
*
* Input Parameters:
*   priv - i2s state instance
*
* Returned Value:
*   A non-NULL pointer to the allocate buffer container on success; NULL if
*   there are no available buffer containers.
*
* Assumptions:
*   The caller does NOT have exclusive access to the I2S state structure.
*   That would result in a deadlock!
*
****************************************************************************/

static struct esp32_buffer_s *i2s_buf_tx_allocate(struct esp32_i2s_s *priv)
{
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Set aside a buffer container.  By doing this, we guarantee that we will
	 * have at least one free buffer container.
	 */
	i2s_bufsem_tx_take(priv);

	/* Get the buffer from the head of the free list */
	flags = irqsave();
	bfcontainer = priv->freelist_tx;
	ASSERT(bfcontainer);

	/* Unlink the buffer from the freelist */
	priv->freelist_tx = bfcontainer->flink;
	irqrestore(flags);

	return bfcontainer;
}

/****************************************************************************
* Name: i2s_buf_tx_free
*
* Description:
*   Free buffer container by adding it to the head of the free list
*
* Input Parameters:
*   priv - I2S state instance
*   bfcontainer - The buffer container to be freed
*
* Returned Value:
*   None
*
* Assumptions:
*   The caller has exclusive access to the I2S state structure
*
****************************************************************************/

static void i2s_buf_tx_free(struct esp32_i2s_s *priv, struct esp32_buffer_s *bfcontainer)
{
	irqstate_t flags;

	/* Put the buffer container back on the free list */
	flags = irqsave();
	bfcontainer->flink = priv->freelist_tx;
	priv->freelist_tx = bfcontainer;
	irqrestore(flags);

	/* Wake up any threads waiting for a buffer container */
	i2s_bufsem_tx_give(priv);
}

/****************************************************************************
 * Name: i2s_buf_tx_initialize
 *
 * Description:
 *   Initialize the buffer container allocator by adding all of the
 *   pre-allocated buffer containers to the free list
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Called early in I2S initialization so that there are no issues with
 *   concurrency.
 *
 ****************************************************************************/

static void i2s_buf_tx_initialize(struct esp32_i2s_s *priv)
{
	int i;

	priv->freelist_tx = NULL;
	sem_init(&priv->bufsem_tx, 0, 1);

	for (i = 0; i < CONFIG_ESP32_I2S_MAXINFLIGHT; i++) {
		i2s_buf_tx_free(priv, &priv->containers_tx[i]);
	}
}
#endif

int free_dma_desc_links(lldesc_chain_t *desc_chain)
{
	for (lldesc_t *cdesc = desc_chain->head; cdesc != NULL;) {
		lldesc_t *prvdesc = cdesc;
		cdesc = prvdesc->qe;
		kmm_free(prvdesc);
	}
	desc_chain->head = NULL;
	desc_chain->tail = NULL;
	return OK;
}

int setup_dma_desc_links(lldesc_chain_t *desc_chain, int len, const uint8_t *data)
{
	int ret = ERROR;
	if (desc_chain == NULL || len <= 0 || data == NULL) {
		ret = -EFAULT;
		i2serr("[I2S] param error: %p, %d, %p!\n", desc_chain, len, data);
		return ret;
	}
	desc_chain->head = NULL;
	desc_chain->tail = NULL;
	int n = 0;
	while (n < len) {
		lldesc_t *cdesc = (lldesc_t *)kmm_zalloc(sizeof(lldesc_t));
		if (cdesc == NULL) {
			ret = -ENOMEM;
			i2serr("[I2S] setup dma links alloc mem fails!\n");
			goto errout_with_lldesc;
		}
		if (desc_chain->head == NULL) {
			desc_chain->head = cdesc;
		}

		int count = len - n;
		if (count > I2S_MAX_BUFFER_SIZE) {
			count = I2S_MAX_BUFFER_SIZE;
		}

		cdesc->length = count;
		cdesc->size = count;
		cdesc->offset = 0;
		cdesc->owner = 1;
		cdesc->eof = 0;
		cdesc->sosf = 0;
		cdesc->buf = (uint8_t *)data;
		cdesc->empty = 0;

		if (desc_chain->tail != NULL) {
			desc_chain->tail->qe = cdesc;
		}
		desc_chain->tail = cdesc;

		n += count;
		data += count;
	}
	/*Mark last DMA desc as end of stream. */
	if (desc_chain->tail != NULL) {
		desc_chain->tail->eof = 1;
		desc_chain->tail->qe = NULL;

		ret = OK;
	} else {
		i2serr("[I2S] links tail is NULL: len %d;!\n", len);
	}
	return ret;

errout_with_lldesc:
	free_dma_desc_links(desc_chain);
	return ret;
}

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)

/****************************************************************************
 * Name: i2s_rx_start
 *
 * Description:
 *   Setup and initiate first RX DMA transfer
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure
 *
 * Assumptions:
 *   Interrupts are disabled
 *
 ****************************************************************************/
static inline void i2s_rxdma_callback(struct esp32_i2s_s *priv, int result)
{
	/* Cancel the watchdog timeout */
	(void)wd_cancel(priv->rx.dog);

	/* Then schedule completion of the transfer to occur on the worker thread */
	i2s_rx_schedule(priv, result);
}

static void i2s_rxdma_timeout(int argc, uint32_t arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Timeout: set the result to -ETIMEDOUT. */
	i2s_rx_schedule(priv, -ETIMEDOUT);
}

static int i2s_rx_start(struct esp32_i2s_s *priv)
{
	struct esp32_buffer_s *bfcontainer;
	int ret;
	irqstate_t flags;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->rx.act)) {
		i2sinfo("[RX start] RX active!\n");
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->rx.pend)) {
		i2sinfo("[RX start] RX pend empty!\n");
		return OK;
	}

	/* If I2S RX DMA is active, pend buffers will be fetched and processed */
	if (I2S[priv->i2s_num]->conf.rx_start) {
		i2sinfo("[RX start] RX started!\n");
		return OK;
	}

	flags = irqsave();
	/* Remove the pending RX transfer at the head of the RX pending queue. */
	bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.pend);

	if (bfcontainer != NULL && NULL != bfcontainer->apb) {

		/* Add the container to the list of active DMAs */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.act);
		/* Start RX DMA */
		I2S[priv->i2s_num]->rx_eof_num = (bfcontainer->apb->nmaxbytes + 3) / 4;
		I2S[priv->i2s_num]->in_link.addr = (uint32_t)(bfcontainer->desc_chain.head);

		i2s_start((struct i2s_dev_s *)priv, I2S_RX);
		i2sinfo("[RX start] %d start RX %08x: %d...%08x\n", priv->i2s_num, &(bfcontainer->apb), bfcontainer->apb->nmaxbytes, I2S[priv->i2s_num]->conf);
	}
	irqrestore(flags);

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0) {
		ret = wd_start(priv->rx.dog, bfcontainer->timeout, (wdentry_t)i2s_rxdma_timeout, 1, (uint32_t)priv);

		/* Check if we have successfully started the watchdog timer.  Note
		 * that we do nothing in the case of failure to start the timer.  We
		 * are already committed to the DMA anyway.  Let's just hope that the
		 * DMA does not hang.
		 */
		if (ret < 0) {
			i2serr("ERROR: wd_start failed: %d\n", errno);
		}
	}

	return OK;
}

/****************************************************************************
 * Name: i2s_rx_worker
 *
 * Description:
 *   RX transfer done worker
 *
 * Input Parameters:
 *   arg - the I2S device instance cast to void*
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
static void i2s_rx_worker(void *arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)arg;
	struct ap_buffer_s *apb;
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	DEBUGASSERT(priv);

	/* Process each buffer in the rx.done queue */
	while (sq_peek(&priv->rx.done) != NULL) {
		/* Remove the buffer container from the rx.done queue.  NOTE that
		 * interrupts must be disabled to do this because the rx.done queue is
		 * also modified from the interrupt level.
		 */
		flags = irqsave();
		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.done);
		irqrestore(flags);

		/* Perform the RX transfer done callback */
		if (NULL == bfcontainer) {
			break;
		}
		if (NULL == bfcontainer->apb) {
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_rx_free(priv, bfcontainer);
			break;
		}
		apb = bfcontainer->apb;

		/* If the DMA was successful, then update the number of valid bytes in
		 * the audio buffer.
		 */
		if (bfcontainer->result == OK) {
			apb->nbytes = apb->nmaxbytes;
		} else {
			apb->nbytes = -1;
		}
		/* Perform the RX transfer done callback */
		if (NULL != bfcontainer->callback) {
			bfcontainer->callback(&priv->dev, apb, bfcontainer->arg, bfcontainer->result);
		}
		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */
		apb_free(apb);

		/* And release the buffer container */
		free_dma_desc_links(&(bfcontainer->desc_chain));
		i2s_buf_rx_free(priv, bfcontainer);
	}
}

/****************************************************************************
 * Name: i2s_rx_schedule
 *
 * Description:
 *   An RX DMA completion or timeout has occurred.  Schedule processing on
 *   the working thread.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   Interrupts are disabled
 *
 ****************************************************************************/

static void i2s_rx_schedule(struct esp32_i2s_s *priv, int result)
{
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->rx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both via the i2s_rxdma_callback() logic and the i2s_rxdma_timeout()
	 * logic.
	 */

	if (!up_interrupt_context()) {
		i2s_exclsem_take(priv);
		flags = irqsave();
	}
	/* Move first entry from the rx.act queue to the rx.done queue */

	if (!sq_empty(&priv->rx.act)) {
		/* Remove the next buffer container from the rx.act list */

		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.act);

		/* Report the result of the transfer */

		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the rx.done queue */

		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.done);
	}

	if (!sq_empty(&priv->rx.pend)) {
		/* Remove the next buffer container from the rx.pend list */
		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.pend);

		/* Add the completed buffer container to the tail of the rx.act queue */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.act);

		/* Start next DMA transfer */
		I2S[priv->i2s_num]->rx_eof_num = (bfcontainer->apb->nmaxbytes) / 4;
		I2S[priv->i2s_num]->in_link.addr = (uint32_t)(bfcontainer->desc_chain.head);

		i2s_start((struct i2s_dev_s *)priv, I2S_RX);
		i2sinfo("[RX start] start RX %p:%d...\n", bfcontainer->apb, bfcontainer->apb->nmaxbytes);
	}

	if (sq_empty(&priv->rx.pend) && sq_empty(&priv->rx.act)) {
		i2s_stop_transfer((struct i2s_dev_s *)priv, I2S_RX);
		i2sinfo("[RX] Stop: pend and act are empty!\n");
	}

	if (!up_interrupt_context()) {
		irqrestore(flags);
		i2s_exclsem_give(priv);
	}

	/* If the worker has completed running, then reschedule the working thread.
	 * REVISIT:  There may be a race condition here.  So we do nothing is the
	 * worker is not available.
	 */
	if (!sq_empty(&priv->rx.done) && work_available(&priv->rx.work)) {
		int ret;
		/* Schedule the RX DMA done processing to occur on the worker thread. */

		ret = work_queue(HPWORK, &priv->rx.work, i2s_rx_worker, priv, 0);
		if (ret != 0) {
			i2serr("ERROR: Failed to queue RX work: %d\n", ret);
		}
	}
}

#endif

/****************************************************************************
 * Name: i2s_rxdatawidth
 *
 * Description:
 *   Set the I2S RX data width.  The RX bitrate is determined by
 *   sample_rate * data_width.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   width - The I2S data with in bits.
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static uint32_t i2s_rxdatawidth(struct i2s_dev_s *dev, int bits)
{
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	int ret;

	/* Support 16/32bits only */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT));

	if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
		priv->bits_per_sample = bits;
		ret = i2s_set_clk(priv, priv->sample_rate, priv->bits_per_sample, priv->channel_num);
		if (ret == OK) {
			return priv->bits_per_sample * priv->sample_rate;
		}
	}
#endif

	return 0;
}

/****************************************************************************
 * Name: i2s_receive
 *
 * Description:
 *   Receive a block of data from I2S.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   apb      - A pointer to the audio buffer in which to recieve data
 *   callback - A user provided callback function that will be called at
 *              the completion of the transfer.  The callback will be
 *              performed in the context of the worker thread.
 *   arg      - An opaque argument that will be provided to the callback
 *              when the transfer complete
 *   timeout  - The timeout value to use.  The transfer will be canceled
 *              and an ETIMEDOUT error will be reported if this timeout
 *              elapsed without completion of the DMA transfer.  Units
 *              are system clock ticks.  Zero means no timeout.
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure.  NOTE:  This function
 *   only enqueues the transfer and returns immediately.  Success here only
 *   means that the transfer was enqueued correctly.
 *
 *   When the transfer is complete, a 'result' value will be provided as
 *   an argument to the callback function that will indicate if the transfer
 *   failed.
 *
 ****************************************************************************/

static int i2s_receive(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;

	i2sinfo("[I2S RX] apb=%p nmaxbytes=%d samp=%p arg=%p timeout=%d\n", apb, apb->nmaxbytes, apb->samp, arg, timeout);

	i2s_init_buffer(apb->samp, apb->nmaxbytes);

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	int ret = OK;
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;
	/* Has the RX channel been enabled? */
	if (!priv->rxenab) {
		i2serr("ERROR: I2S has no receiver\n");
		return -EAGAIN;
	}

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("RX Exclusive Enter\n");

	/* Allocate a buffer container in advance */
	bfcontainer = i2s_buf_rx_allocate(priv);
	if (NULL == bfcontainer) {
		goto errout_with_exclsem;
	}

	/* Add a reference to the audio buffer */
	apb_reference(apb);

	/* Initialize the buffer container structure */
	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = EBUSY;

	/* Allocate memory for desc list, and inintiallize it! */
	ret = setup_dma_desc_links(&(bfcontainer->desc_chain), apb->nmaxbytes, (const uint8_t *)apb->samp);
	if (ret != OK) {
		i2serr("I2S RX set dma desc links fails:%d\n", ret);
		goto errout_with_lldesc;
	}
	/* No data received yet */
	apb->nbytes = 0;
	apb->curbyte = 0;

	/* Add the buffer container to the end of the RX pending queue */
	flags = irqsave();
	sq_addlast((sq_entry_t *)bfcontainer, &priv->rx.pend);
	irqrestore(flags);

	/* Start transfer */
	ret = i2s_rx_start(priv);

	/* Exit this function */
	i2s_exclsem_give(priv);
	i2sinfo("RX Exclusive Exit\n");

	return OK;

errout_with_lldesc:
	free_dma_desc_links(&(bfcontainer->desc_chain));
	i2s_buf_rx_free(priv, bfcontainer);
errout_with_exclsem:
	i2s_exclsem_give(priv);
	return ret;

#else
	i2serr("ERROR: I2S has no receiver\n");
	UNUSED(priv);
	return -ENOSYS;
#endif

}

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)

/****************************************************************************
 * Name: i2s_tx_start
 *
 * Description:
 *   Setup and initiate the first TX DMA transfer
 *
 * Input Parameters:
 *   priv - I2S state instance
 *
 * Returned Value:
 *   OK on success; a negated errno value on failure
 *
 ****************************************************************************/

static inline void i2s_txdma_callback(struct esp32_i2s_s *priv, int result)
{
	/* Cancel the watchdog timeout */
	(void)wd_cancel(priv->tx.dog);

	/* Then schedule completion of the transfer to occur on the worker thread */
	i2s_tx_schedule(priv, result);
}

static void i2s_txdma_timeout(int argc, uint32_t arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)arg;
	DEBUGASSERT(priv != NULL);

	/* Then schedule completion of the transfer to occur on the worker thread.
	 * Set the result with -ETIMEDOUT.
	 */
	i2s_tx_schedule(priv, -ETIMEDOUT);
}

static int i2s_tx_start(struct esp32_i2s_s *priv)
{
	struct esp32_buffer_s *bfcontainer = NULL;
	int ret;
	irqstate_t flags;

	/* Check if the DMA is IDLE */
	if (!sq_empty(&priv->tx.act)) {
		i2sinfo("[TX] actived!\n");
		return OK;
	}

	/* If there are no pending transfer, then bail returning success */
	if (sq_empty(&priv->tx.pend)) {
		i2sinfo("[TX] empty!\n");
		return OK;
	}

	/* If I2S TX is active, pend buffers will be fetched and processed */
	if (I2S[priv->i2s_num]->conf.tx_start) {
		i2sinfo("[TX] started!\n");
		return OK;
	}

	flags = irqsave();

	/* Remove the pending TX transfer at the head of the TX pending queue. */
	bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.pend);
	if (NULL != bfcontainer && NULL != bfcontainer->apb) {

		/* Add the container to the list of active DMAs */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.act);

		/* Start the DMA, saving the container as the current active transfer */
		I2S[priv->i2s_num]->out_link.addr = (uint32_t)(bfcontainer->desc_chain.head);

		i2s_start((struct i2s_dev_s *)priv, I2S_TX);
		i2sinfo("[TX start] apb=%x\n", bfcontainer->apb);
	}

	irqrestore(flags);

	/* Start a watchdog to catch DMA timeouts */
	if (bfcontainer->timeout > 0) {
		ret = wd_start(priv->tx.dog, bfcontainer->timeout, (wdentry_t)i2s_txdma_timeout, 1, (uint32_t)priv);

		/* Check if we have successfully started the watchdog timer.  Note
		 * that we do nothing in the case of failure to start the timer.  We
		 * are already committed to the DMA anyway.  Let's just hope that the
		 * DMA does not hang.
		 */
		if (ret < 0) {
			i2serr("ERROR: wd_start failed: %d\n", errno);
		}
	}

	return OK;
}

/****************************************************************************
 * Name: i2s_tx_worker
 *
 * Description:
 *   Primary TX transfer worker
 *
 * Input Parameters:
 *   arg - the I2S device instance cast to void*
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void i2s_tx_worker(void *arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)arg;
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	DEBUGASSERT(priv);

	/* When the transfer was started, the active buffer containers were removed
	 * from the txp.pend queue and saved in the txp.act queue.  We get here when the
	 * DMA is finished... either successfully, with a DMA error, or with a DMA
	 * timeout.
	 *
	 * In any case, the buffer containers in txp.act will be moved to the end
	 * of the txp.done queue and txp.act will be emptied before this worker is
	 * started.
	 */
	/* Process each buffer in the tx.done queue */
	while (sq_peek(&priv->tx.done) != NULL) {
		/* Remove the buffer container from the tx.done queue.  NOTE that
		 * interrupts must be enabled to do this because the tx.done queue is
		 * also modified from the interrupt level.
		 */
		flags = irqsave();
		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.done);
		irqrestore(flags);

		if (NULL == bfcontainer) {
			i2serr("[I2S] bfcontainer empty\n");
			break;
		}
		if (NULL == bfcontainer->apb) {
			i2serr("[I2S] apb empty\n");
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_tx_free(priv, bfcontainer);

			break;
		}

		/* Perform the TX transfer done callback */
		if (NULL != bfcontainer->callback) {
			bfcontainer->callback(&priv->dev, bfcontainer->apb, bfcontainer->arg, bfcontainer->result);
		}
		/* Release our reference on the audio buffer.  This may very likely
		 * cause the audio buffer to be freed.
		 */
		apb_free(bfcontainer->apb);

		/* And release the buffer container */
		free_dma_desc_links(&(bfcontainer->desc_chain));
		i2s_buf_tx_free(priv, bfcontainer);
	}
}

/****************************************************************************
 * Name: i2s_tx_schedule
 *
 * Description:
 *   A TX DMA completion or timeout has occurred.  Schedule processing on
 *   the working thread.
 *
 * Input Parameters:
 *   handle - The DMA handler
 *   arg - A pointer to the chip select struction
 *   result - The result of the DMA transfer
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   - Interrupts are disabled
 *   - The TX timeout has been canceled.
 *
 ****************************************************************************/
static void i2s_tx_schedule(struct esp32_i2s_s *priv, int result)
{
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;

	/* Upon entry, the transfer(s) that just completed are the ones in the
	 * priv->tx.act queue.  NOTE: In certain conditions, this function may
	 * be called an additional time, hence, we can't assert this to be true.
	 * For example, in the case of a timeout, this function will be called by
	 * both the i2s_txdma_callback() logic and the i2s_txdma_timeout() logic.
	 */

	/* Move all entries from the tx.act queue to the tx.done queue */
	if (!up_interrupt_context()) {
		i2s_exclsem_take(priv);
		flags = irqsave();
	}
	if (!sq_empty(&priv->tx.act)) {
		/* Remove the next buffer container from the tx.act list */
		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.act);

		/* Report the result of the transfer */
		bfcontainer->result = result;

		/* Add the completed buffer container to the tail of the tx.done queue */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.done);
	}

	if (!sq_empty(&priv->tx.pend)) {
		/* Remove the next buffer container from the tx.pend list */
		bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.pend);

		/* Add the completed buffer container to the tail of the tx.act queue */
		sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.act);

		/* Start next DMA transfer */
		I2S[priv->i2s_num]->out_link.addr = (uint32_t)(bfcontainer->desc_chain.head);

		i2s_start((struct i2s_dev_s *)priv, I2S_TX);
	}

	if (sq_empty(&priv->tx.pend) && sq_empty(&priv->tx.act)) {
		i2s_stop_transfer((struct i2s_dev_s *)priv, I2S_TX);
	}
	if (!up_interrupt_context()) {
		irqrestore(flags);
		i2s_exclsem_give(priv);
	}

	/* If the worker has completed running, then reschedule the working thread.
	 * REVISIT:  There may be a race condition here.  So we do nothing is the
	 * worker is not available.
	 */
	if (!sq_empty(&priv->tx.done) && work_available(&priv->tx.work)) {
		int ret = OK;
		/* Schedule the TX DMA done processing to occur on the worker thread. */
		ret = work_queue(HPWORK, &priv->tx.work, i2s_tx_worker, priv, 0);
		if (ret != OK && ret != -EALREADY) {
			i2serr("ERROR: Failed to queue TX work: %d\n", ret);
		}
	} else {
		i2sinfo("[tx shec] tx.work not empty! \n");
	}
}

#endif
/****************************************************************************
 * Name: i2s_txdatawidth
 *
 * Description:
 *   Set the I2S TX data width.  The TX bitrate is determined by
 *   sample_rate * data_width.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   width - The I2S data with in bits.
 *
 * Returned Value:
 *   Returns the resulting bitrate
 *
 ****************************************************************************/

static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits)
{
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	int ret;
	/* Support 16/32bits only */
	DEBUGASSERT(priv && (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT));

	if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
		priv->bits_per_sample = bits;

		if (bits == I2S_BITS_PER_SAMPLE_16BIT || bits == I2S_BITS_PER_SAMPLE_32BIT) {
			priv->bits_per_sample = bits;
			ret = i2s_set_clk(priv, priv->sample_rate, priv->bits_per_sample, priv->channel_num);
			if (ret == OK) {
				return priv->bits_per_sample * priv->sample_rate;
			}
		}
	}
#endif

	return 0;
}

/****************************************************************************
* Name: i2s_send
*
* Description:
*   Send a block of data on I2S.
*
* Input Parameters:
*   dev      - Device-specific state data
*   apb      - A pointer to the audio buffer from which to send data
*   callback - A user provided callback function that will be called at
*              the completion of the transfer.  The callback will be
*              performed in the context of the worker thread.
*   arg      - An opaque argument that will be provided to the callback
*              when the transfer complete
*   timeout  - The timeout value to use.  The transfer will be canceled
*              and an ETIMEDOUT error will be reported if this timeout
*              elapsed without completion of the DMA transfer.  Units
*              are system clock ticks.  Zero means no timeout.
*
* Returned Value:
*   OK on success; a negated errno value on failure.  NOTE:  This function
*   only enqueues the transfer and returns immediately.  Success here only
*   means that the transfer was enqueued correctly.
*
*   When the transfer is complete, a 'result' value will be provided as
*   an argument to the callback function that will indicate if the transfer
*   failed.
*
****************************************************************************/

static int i2s_send(struct i2s_dev_s *dev, struct ap_buffer_s *apb, i2s_callback_t callback, void *arg, uint32_t timeout)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;

	DEBUGASSERT(priv && apb);

	i2sinfo("[I2S TX] apb=%p nbytes=%d arg=%p samp=%p timeout=%d\n", apb, apb->nbytes - apb->curbyte, apb->samp, arg, timeout);

	i2s_dump_buffer("Sending", &apb->samp[apb->curbyte], apb->nbytes - apb->curbyte);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	struct esp32_buffer_s *bfcontainer;
	irqstate_t flags;
	int ret;

	/* Has the TX channel been enabled? */
	if (!priv->txenab) {
		i2serr("ERROR: I2S has no transmitter\n");
		return -EAGAIN;
	}

	/* Get exclusive access to the I2S driver data */
	i2s_exclsem_take(priv);
	i2sinfo("TX Exclusive Enter\n");

	/* Allocate a buffer container in advance */
	bfcontainer = i2s_buf_tx_allocate(priv);
	if (NULL == bfcontainer) {
		i2serr("[I2S] allocate TX container fails\n");
		ret = -ENOMEM;
		goto errout_with_exclsem;
	}

	/* Add a reference to the audio buffer */
	apb_reference(apb);

	/* Initialize the buffer container structure */
	bfcontainer->callback = (void *)callback;
	bfcontainer->timeout = timeout;
	bfcontainer->arg = arg;
	bfcontainer->apb = apb;
	bfcontainer->result = EBUSY;

	/* alloc memory for desc list, and inintiallize it! */
	ret = setup_dma_desc_links(&(bfcontainer->desc_chain), apb->nmaxbytes, (const uint8_t *)apb->samp);
	if (ret != OK) {
		i2serr("I2S tx setup dma links fails:L%d\n", ret);
		goto errout_with_lldesc;
	}

	/* Add the buffer container to the end of the TX pending queue */
	flags = irqsave();
	sq_addlast((sq_entry_t *)bfcontainer, &priv->tx.pend);
	irqrestore(flags);

	/* Then start the next transfer.  If there is already a transfer in progess,
	 * then this will do nothing.
	 */
	ret = i2s_tx_start(priv);

	i2s_exclsem_give(priv);
	i2sinfo("TX Exclusive Exit\n");

	return OK;

errout_with_lldesc:
	free_dma_desc_links(&(bfcontainer->desc_chain));
	i2s_buf_tx_free(priv, bfcontainer);
errout_with_exclsem:
	i2s_exclsem_give(priv);
	return ret;

#else
	i2serr("ERROR: I2S has no transmitter\n");
	UNUSED(priv);
	return -ENOSYS;
#endif
}

/****************************************************************************
* Name: i2s_pause
*
* Description:
*   Pause data transfer on I2S.
*
* Input Parameters:
*   dev      - Device-specific state data
*   dir      - the data flow direction of I2S
*
* Returned Value:
*   OK on success;
*
****************************************************************************/
static int i2s_pause(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		I2S[priv->i2s_num]->out_link.stop = 1;
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		I2S[priv->i2s_num]->in_link.stop = 1;
	}
#endif

	return OK;
}

/****************************************************************************
* Name: i2s_resume
*
* Description:
*   Resume data transfer on I2S if it is paused.
*
* Input Parameters:
*   dev      - Device-specific state data
*   dir      - the data flow direction of I2S
*
* Returned Value:
*   OK on success;
*
****************************************************************************/
static int i2s_resume(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX && priv->txenab) {
		I2S[priv->i2s_num]->out_link.stop = 0;
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX && priv->rxenab) {
		I2S[priv->i2s_num]->in_link.stop = 0;
	}
#endif

	return 0;
}

/****************************************************************************
* Name: Reset fifo
*
* Description:
*   Reset fifo
*
* Input Parameters:
*   i2s_num  - i2s module number
*
* Returned Value:
*  OK
*
****************************************************************************/
static int i2s_reset_fifo(i2s_port_t i2s_num)
{
	DEBUGASSERT(i2s_num < I2S_NUM_MAX);

	i2s_exclsem_take(g_i2sdevice[i2s_num]);
	I2S[i2s_num]->conf.rx_fifo_reset = 1;
	I2S[i2s_num]->conf.rx_fifo_reset = 0;
	while (I2S[i2s_num]->state.rx_fifo_reset_back) {
		;
	}
	I2S[i2s_num]->conf.tx_fifo_reset = 1;
	I2S[i2s_num]->conf.tx_fifo_reset = 0;
	while (I2S[i2s_num]->state.tx_fifo_reset_back) {
		;
	}
	i2s_exclsem_give(g_i2sdevice[i2s_num]);
	return OK;
}

static inline void i2s_reset_txfifo(i2s_port_t i2s_num)
{
	I2S[i2s_num]->conf.tx_fifo_reset = 1;
	I2S[i2s_num]->conf.tx_fifo_reset = 0;
	while (I2S[i2s_num]->state.tx_fifo_reset_back) {
		;
	}
}

static inline void i2s_reset_rxfifo(i2s_port_t i2s_num)
{
	I2S[i2s_num]->conf.rx_fifo_reset = 1;
	I2S[i2s_num]->conf.rx_fifo_reset = 0;
	while (I2S[i2s_num]->state.rx_fifo_reset_back) {
		;
	}
}

/****************************************************************************
* Name: Reset fifo
*
* Description:
*   Reset fifo
*
* Input Parameters:
*   i2s_num  - i2s module number
*
* Returned Value:
*  OK
*
****************************************************************************/

static inline void i2s_clear_intr_status(i2s_port_t i2s_num, uint32_t clr_mask)
{
	I2S[i2s_num]->int_clr.val = clr_mask;
}

static inline void i2s_enable_rx_intr(i2s_port_t i2s_num)
{
	I2S[i2s_num]->int_ena.in_suc_eof = 1;
	I2S[i2s_num]->int_ena.in_dscr_err = 1;
}

static inline void i2s_disable_rx_intr(i2s_port_t i2s_num)
{
	I2S[i2s_num]->int_ena.in_suc_eof = 0;
	I2S[i2s_num]->int_ena.in_dscr_err = 0;
}

static inline void i2s_disable_tx_intr(i2s_port_t i2s_num)
{
	I2S[i2s_num]->int_ena.out_eof = 0;
	I2S[i2s_num]->int_ena.out_dscr_err = 0;
}

static inline void i2s_enable_tx_intr(i2s_port_t i2s_num)
{
	I2S[i2s_num]->int_ena.out_eof = 1;
	I2S[i2s_num]->int_ena.out_dscr_err = 1;
}

/****************************************************************************
* Name: i2s_start
*
* Description:
*   start i2s transferring
*
* Input Parameters:
*   dev     - device handler
*   dir     - transferring direction
*
* Returned Value:
*  OK
*
****************************************************************************/

int i2s_start(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	int i2s_num = priv->i2s_num;
	irqstate_t flags;

	flags = irqsave();
	I2S[i2s_num]->int_clr.val = 0xFFFFFFFF;
	if (0 < (priv->mode & I2S_MODE_RX) && dir == I2S_RX) {
		i2s_reset_rxfifo(i2s_num);

		I2S[i2s_num]->lc_conf.in_rst = 1;
		I2S[i2s_num]->lc_conf.in_rst = 0;

		I2S[i2s_num]->conf.rx_reset = 1;
		I2S[i2s_num]->conf.rx_reset = 0;

		i2s_enable_rx_intr(i2s_num);
		I2S[i2s_num]->in_link.start = 1;
		I2S[i2s_num]->conf.rx_start = 1;
	}

	if (0 < (priv->mode & I2S_MODE_TX) && dir == I2S_TX) {
		i2s_reset_txfifo(i2s_num);

		I2S[i2s_num]->lc_conf.out_rst = 1;
		I2S[i2s_num]->lc_conf.out_rst = 0;

		I2S[i2s_num]->conf.tx_reset = 1;
		I2S[i2s_num]->conf.tx_reset = 0;

		i2s_enable_tx_intr(i2s_num);
		I2S[i2s_num]->out_link.start = 1;
		I2S[i2s_num]->conf.tx_start = 1;
	}
	irqrestore(flags);

	return OK;
}

/****************************************************************************
* Name: i2s_stop
*
* Description:
*   stop i2s transferring
*
* Input Parameters:
*   dev     - device handler
*   dir     - transferring direction
*
* Returned Value:
*  OK
*
****************************************************************************/
static int i2s_stop_transfer(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	DEBUGASSERT(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		i2sinfo("[I2S stop] TX\n");
		I2S[priv->i2s_num]->out_link.stop = 1;
		I2S[priv->i2s_num]->conf.tx_start = 0;
		i2s_disable_tx_intr(priv->i2s_num);
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		i2sinfo("[I2S stop] RX\n");
		I2S[priv->i2s_num]->in_link.stop = 1;
		I2S[priv->i2s_num]->conf.rx_start = 0;
		i2s_disable_rx_intr(priv->i2s_num);
	}
#endif

	return 0;
}

static int i2s_stop(struct i2s_dev_s *dev, i2s_ch_dir_t dir)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	irqstate_t flags;
	struct esp32_buffer_s *bfcontainer;
	DEBUGASSERT(priv);

	i2s_exclsem_take(priv);

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	if (dir == I2S_TX) {
		I2S[priv->i2s_num]->out_link.stop = 1;
		I2S[priv->i2s_num]->conf.tx_start = 0;
		i2s_disable_tx_intr(priv->i2s_num);

		while (sq_peek(&priv->tx.pend) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.pend);
			irqrestore(flags);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_tx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->tx.done) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->tx.done);
			irqrestore(flags);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_tx_free(priv, bfcontainer);
		}
	}
#endif

#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	if (dir == I2S_RX) {
		I2S[priv->i2s_num]->in_link.stop = 1;
		I2S[priv->i2s_num]->conf.rx_start = 0;
		i2s_disable_rx_intr(priv->i2s_num);

		while (sq_peek(&(priv->rx.pend)) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.pend);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.act) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.act);
			irqrestore(flags);
			apb_free(bfcontainer->apb);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_rx_free(priv, bfcontainer);
		}

		while (sq_peek(&priv->rx.done) != NULL) {
			flags = irqsave();
			bfcontainer = (struct esp32_buffer_s *)sq_remfirst(&priv->rx.done);
			irqrestore(flags);
			free_dma_desc_links(&(bfcontainer->desc_chain));
			i2s_buf_rx_free(priv, bfcontainer);
		}
	}
#endif

	i2s_exclsem_give(priv);

	return 0;
}

/****************************************************************************
* Name: i2s_err_cb_register
*
* Description:
*   Register the error callback
*
* Input Parameters:
*   dev      - Device-specific state data
*   cb       - the function pointer to callback
*   arg      - argument to be transferred to the callback function
*
* Returned Value:
*  OK
*
****************************************************************************/
static int i2s_err_cb_register(struct i2s_dev_s *dev, i2s_err_cb_t cb, void *arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	DEBUGASSERT(priv);

	priv->err_cb = cb;
	priv->err_cb_arg = arg;

	/* Set initial state to error, it will be cleaned when operation starts */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	priv->rx.state = -1;
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	priv->tx.state = -1;
#endif

	return 0;
}

/****************************************************************************
 * Name: i2s_configure
 *
 * Description:
 *   Configure i2s
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.  These functions
 *          will complete the I2S specific portions of the initialization
 *
 * Returned Value:
 *   configure status
 *
 ****************************************************************************/
int i2s_set_pin(i2s_port_t i2s_num, const i2s_pin_config_t *pin)
{
	DEBUGASSERT(i2s_num < I2S_NUM_MAX);

	if (i2s_num >= I2S_NUM_MAX || pin == NULL) {
		return ERROR;
	}

	if (pin->bck_io_num >= 0 && pin->bck_io_num >= GPIO_PIN_COUNT) {
		i2sinfo("[I2S] setpin:bck_io_num error\n");
		return ERROR;
	}

	if (pin->ws_io_num >= 0 && pin->ws_io_num >= GPIO_PIN_COUNT) {
		i2sinfo("[I2S] setpin:ws_io_num error\n");
		return ERROR;
	}

	if (pin->data_out_num >= 0 && pin->data_out_num >= GPIO_PIN_OUT_MAX) {
		i2sinfo("[I2S] setpin:data_out_num error\n");
		return ERROR;
	}

	if (pin->data_in_num >= 0 && pin->ws_io_num >= GPIO_PIN_COUNT) {
		i2sinfo("[I2S] setpin:data_in_num error\n");
		return ERROR;
	}

	int bck_sig = -1, ws_sig = -1, data_out_sig = -1, data_in_sig = -1;

	//Each IIS hw module has a RX and TX unit.
	//For TX unit, the output signal index should be I2SnO_xxx_OUT_IDX
	//For TX unit, the input signal index should be I2SnO_xxx_IN_IDX
	if (g_i2sdevice[i2s_num]->mode & I2S_MODE_TX) {
		if (g_i2sdevice[i2s_num]->mode & I2S_MODE_MASTER) {
			if (i2s_num == I2S_NUM_0) {
				bck_sig = I2S0O_BCK_OUT_IDX;
				ws_sig = I2S0O_WS_OUT_IDX;
				data_out_sig = I2S0O_DATA_OUT23_IDX;
			} else {
				bck_sig = I2S1O_BCK_OUT_IDX;
				ws_sig = I2S1O_WS_OUT_IDX;
				data_out_sig = I2S1O_DATA_OUT23_IDX;
			}
		} else if (g_i2sdevice[i2s_num]->mode & I2S_MODE_SLAVE) {
			if (i2s_num == I2S_NUM_0) {
				bck_sig = I2S0O_BCK_IN_IDX;
				ws_sig = I2S0O_WS_IN_IDX;
				data_out_sig = I2S0O_DATA_OUT23_IDX;
			} else {
				bck_sig = I2S1O_BCK_IN_IDX;
				ws_sig = I2S1O_WS_IN_IDX;
				data_out_sig = I2S1O_DATA_OUT23_IDX;
			}
		}
	}
	//For RX unit, the output signal index should be I2SnI_xxx_OUT_IDX
	//For RX unit, the input signal index shuld be I2SnI_xxx_IN_IDX
	if (g_i2sdevice[i2s_num]->mode & I2S_MODE_RX) {
		if (g_i2sdevice[i2s_num]->mode & I2S_MODE_MASTER) {
			if (i2s_num == I2S_NUM_0) {
				bck_sig = I2S0I_BCK_OUT_IDX;
				ws_sig = I2S0I_WS_OUT_IDX;
				data_in_sig = I2S0I_DATA_IN15_IDX;
			} else {
				bck_sig = I2S1I_BCK_OUT_IDX;
				ws_sig = I2S1I_WS_OUT_IDX;
				data_in_sig = I2S1I_DATA_IN15_IDX;
			}
		} else if (g_i2sdevice[i2s_num]->mode & I2S_MODE_SLAVE) {
			if (i2s_num == I2S_NUM_0) {
				bck_sig = I2S0I_BCK_IN_IDX;
				ws_sig = I2S0I_WS_IN_IDX;
				data_in_sig = I2S0I_DATA_IN15_IDX;
			} else {
				bck_sig = I2S1I_BCK_IN_IDX;
				ws_sig = I2S1I_WS_IN_IDX;
				data_in_sig = I2S1I_DATA_IN15_IDX;
			}
		}
	}
	//For "full-duplex + slave" mode, we should select RX signal index for ws and bck.
	//For "full-duplex + master" mode, we should select TX signal index for ws and bck.
	if ((g_i2sdevice[i2s_num]->mode & I2S_FULL_DUPLEX_SLAVE_MODE_MASK) == I2S_FULL_DUPLEX_SLAVE_MODE_MASK) {
		if (i2s_num == I2S_NUM_0) {
			bck_sig = I2S0I_BCK_IN_IDX;
			ws_sig = I2S0I_WS_IN_IDX;
		} else {
			bck_sig = I2S1I_BCK_IN_IDX;
			ws_sig = I2S1I_WS_IN_IDX;
		}
	} else if ((g_i2sdevice[i2s_num]->mode & I2S_FULL_DUPLEX_MASTER_MODE_MASK) == I2S_FULL_DUPLEX_MASTER_MODE_MASK) {
		if (i2s_num == I2S_NUM_0) {
			bck_sig = I2S0O_BCK_OUT_IDX;
			ws_sig = I2S0O_WS_OUT_IDX;
		} else {
			bck_sig = I2S1O_BCK_OUT_IDX;
			ws_sig = I2S1O_WS_OUT_IDX;
		}
	}

	if (g_i2sdevice[i2s_num]->mode & I2S_MODE_MASTER) {
		esp32_configgpio(pin->ws_io_num, INPUT | OUTPUT | FUNCTION_2);
		gpio_matrix_out(pin->ws_io_num, ws_sig, 0, 0);
		gpio_matrix_in(pin->ws_io_num, ws_sig, 0);
		i2sinfo("[I2S] set pin %d ws: %d...%d\n", pin->ws_io_num, ws_sig, GPIO.func_in_sel_cfg[ws_sig].func_sel);

		esp32_configgpio(pin->bck_io_num, INPUT | OUTPUT | FUNCTION_2);
		gpio_matrix_out(pin->bck_io_num, bck_sig, 0, 0);
		gpio_matrix_in(pin->bck_io_num, bck_sig, 0);
		i2sinfo("[I2S] set pin %d bck: %d...%d\n", pin->bck_io_num, bck_sig, GPIO.func_in_sel_cfg[bck_sig].func_sel);
	} else if (g_i2sdevice[i2s_num]->mode & I2S_MODE_SLAVE) {
		esp32_configgpio(pin->ws_io_num, INPUT_FUNCTION_2);
		gpio_matrix_in(pin->ws_io_num, ws_sig, 0);
		i2sinfo("[I2S] set pin %d ws: %d...%d\n", pin->ws_io_num, ws_sig, GPIO.func_in_sel_cfg[ws_sig].func_sel);

		esp32_configgpio(pin->bck_io_num, INPUT_FUNCTION_2);
		gpio_matrix_in(pin->bck_io_num, bck_sig, 0);
		i2sinfo("[I2S] set pin %d bck: %d...%d\n", pin->bck_io_num, bck_sig, GPIO.func_in_sel_cfg[bck_sig].func_sel);
	}

	esp32_configgpio(pin->data_out_num, OUTPUT_FUNCTION_2);
	gpio_matrix_out(pin->data_out_num, data_out_sig, 0, 0);
	i2sinfo("[I2S] set pin out: pin%d, %d...%d\n", pin->data_out_num, data_out_sig, GPIO.func_in_sel_cfg[data_out_sig].func_sel);

	esp32_configgpio(pin->data_in_num, INPUT_FUNCTION_2);
	gpio_matrix_in(pin->data_in_num, data_in_sig, 0);
	i2sinfo("[I2S] set pin in: pin%d %d...%d\n", pin->data_in_num, data_in_sig, GPIO.func_in_sel_cfg[data_in_sig].func_sel);

	return OK;
}

static int i2s_param_config(i2s_port_t i2s_num, const i2s_config_t *i2s_config)
{
	//reset i2s
	I2S[i2s_num]->conf.tx_reset = 1;
	I2S[i2s_num]->conf.tx_reset = 0;
	I2S[i2s_num]->conf.rx_reset = 1;
	I2S[i2s_num]->conf.rx_reset = 0;

	// reset data fifo
	i2s_reset_fifo(i2s_num);

	//reset dma
	I2S[i2s_num]->lc_conf.in_rst = 1;
	I2S[i2s_num]->lc_conf.in_rst = 0;
	I2S[i2s_num]->lc_conf.out_rst = 1;
	I2S[i2s_num]->lc_conf.out_rst = 0;
	//Enable and configure DMA
	I2S[i2s_num]->lc_conf.check_owner = 0;
	I2S[i2s_num]->lc_conf.out_loop_test = 0;
	I2S[i2s_num]->lc_conf.out_auto_wrback = 0;
	I2S[i2s_num]->lc_conf.out_data_burst_en = 0;
	I2S[i2s_num]->lc_conf.outdscr_burst_en = 0;
	I2S[i2s_num]->lc_conf.out_no_restart_clr = 0;
	I2S[i2s_num]->lc_conf.indscr_burst_en = 0;
	I2S[i2s_num]->lc_conf.out_eof_mode = 1;

	I2S[i2s_num]->conf2.lcd_en = 0;
	I2S[i2s_num]->conf2.camera_en = 0;

	I2S[i2s_num]->pdm_conf.pcm2pdm_conv_en = 0;
	I2S[i2s_num]->pdm_conf.pdm2pcm_conv_en = 0;

	I2S[i2s_num]->fifo_conf.dscr_en = 0;

	I2S[i2s_num]->conf_chan.tx_chan_mod = i2s_config->channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT ? i2s_config->channel_format : (i2s_config->channel_format >> 1);	// 0-two channel;1-right;2-left;3-righ;4-left
	I2S[i2s_num]->fifo_conf.tx_fifo_mod = i2s_config->channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT ? 0 : 1;	// 0-right&left channel;1-one channel
	I2S[i2s_num]->conf.tx_mono = 0;

	I2S[i2s_num]->conf_chan.rx_chan_mod = i2s_config->channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT ? i2s_config->channel_format : (i2s_config->channel_format >> 1);	// 0-two channel;1-right;2-left;3-righ;4-left
	I2S[i2s_num]->fifo_conf.rx_fifo_mod = i2s_config->channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT ? 0 : 1;	// 0-right&left channel;1-one channel
	I2S[i2s_num]->conf.rx_mono = 0;

	i2sinfo("[I2S] tx_chan_mode:%d, tx_fifi_mode: %d; rx_chan_mode:%d, rx_fifi_mode: %d; \n", I2S[i2s_num]->conf_chan.tx_chan_mod, I2S[i2s_num]->fifo_conf.tx_fifo_mod, I2S[i2s_num]->conf_chan.rx_chan_mod, I2S[i2s_num]->fifo_conf.rx_fifo_mod);

	I2S[i2s_num]->fifo_conf.dscr_en = 1;	//connect dma to fifo
	I2S[i2s_num]->fifo_conf.tx_fifo_mod_force_en = 1;
	I2S[i2s_num]->fifo_conf.rx_fifo_mod_force_en = 1;

	I2S[i2s_num]->conf.tx_start = 0;
	I2S[i2s_num]->conf.rx_start = 0;

	if (i2s_config->mode & I2S_MODE_TX) {
		I2S[i2s_num]->conf.tx_msb_right = 0;
		I2S[i2s_num]->conf.tx_right_first = 0;
		I2S[i2s_num]->conf.tx_slave_mod = 0;	// Master
		I2S[i2s_num]->fifo_conf.tx_fifo_mod_force_en = 1;
		if (i2s_config->mode & I2S_MODE_SLAVE) {
			I2S[i2s_num]->conf.tx_slave_mod = 1;	//TX Slave
		}
	}

	if (i2s_config->mode & I2S_MODE_RX) {
		I2S[i2s_num]->conf.rx_msb_right = 0;
		I2S[i2s_num]->conf.rx_right_first = 0;
		I2S[i2s_num]->conf.rx_slave_mod = 0;	// Master
		I2S[i2s_num]->fifo_conf.rx_fifo_mod_force_en = 1;
		if (i2s_config->mode & I2S_MODE_SLAVE) {
			I2S[i2s_num]->conf.rx_slave_mod = 1;	//RX Slave
		}
	}

	if (i2s_config->mode & (I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN)) {
		I2S[i2s_num]->conf2.lcd_en = 1;
		I2S[i2s_num]->conf.tx_right_first = 1;
		I2S[i2s_num]->conf2.camera_en = 0;
	}

	if (i2s_config->mode & I2S_MODE_PDM) {
		I2S[i2s_num]->fifo_conf.rx_fifo_mod_force_en = 1;
		I2S[i2s_num]->fifo_conf.tx_fifo_mod_force_en = 1;
		I2S[i2s_num]->pdm_freq_conf.tx_pdm_fp = 960;
		I2S[i2s_num]->pdm_freq_conf.tx_pdm_fs = i2s_config->sample_rate / 1000 * 10;
		I2S[i2s_num]->pdm_conf.tx_sinc_osr2 = I2S[i2s_num]->pdm_freq_conf.tx_pdm_fp / I2S[i2s_num]->pdm_freq_conf.tx_pdm_fs;
		I2S[i2s_num]->pdm_conf.rx_sinc_dsr_16_en = 0;
		I2S[i2s_num]->pdm_conf.rx_pdm_en = 1;
		I2S[i2s_num]->pdm_conf.tx_pdm_en = 1;
		I2S[i2s_num]->pdm_conf.pcm2pdm_conv_en = 1;
		I2S[i2s_num]->pdm_conf.pdm2pcm_conv_en = 1;
	} else {
		I2S[i2s_num]->pdm_conf.rx_pdm_en = 0;
		I2S[i2s_num]->pdm_conf.tx_pdm_en = 0;
	}

	if (i2s_config->communication_format & I2S_COMM_FORMAT_I2S) {
		I2S[i2s_num]->conf.tx_short_sync = 0;
		I2S[i2s_num]->conf.rx_short_sync = 0;
		I2S[i2s_num]->conf.tx_msb_shift = 1;
		I2S[i2s_num]->conf.rx_msb_shift = 1;
		i2sinfo("[I2S] comm_mode: i2s,");
		if (i2s_config->communication_format & I2S_COMM_FORMAT_I2S_LSB) {
			i2sinfo(" LSB ");
			if (i2s_config->mode & I2S_MODE_TX) {
				I2S[i2s_num]->conf.tx_msb_shift = 0;
				i2sinfo(" TX ");
			}
			if (i2s_config->mode & I2S_MODE_RX) {
				I2S[i2s_num]->conf.rx_msb_shift = 0;
				i2sinfo(" RX ");
			}
		}
		i2sinfo("\n");
	}

	if (i2s_config->communication_format & I2S_COMM_FORMAT_PCM) {
		I2S[i2s_num]->conf.tx_msb_shift = 0;
		I2S[i2s_num]->conf.rx_msb_shift = 0;
		I2S[i2s_num]->conf.tx_short_sync = 0;
		I2S[i2s_num]->conf.rx_short_sync = 0;
		if (i2s_config->communication_format & I2S_COMM_FORMAT_PCM_SHORT) {
			if (i2s_config->mode & I2S_MODE_TX) {
				I2S[i2s_num]->conf.tx_short_sync = 1;
			}
			if (i2s_config->mode & I2S_MODE_RX) {
				I2S[i2s_num]->conf.rx_short_sync = 1;
			}
		}
	}

	if ((g_i2sdevice[i2s_num]->mode & I2S_MODE_RX) && (g_i2sdevice[i2s_num]->mode & I2S_MODE_TX)) {
		I2S[i2s_num]->conf.sig_loopback = 1;
		if (g_i2sdevice[i2s_num]->mode & I2S_MODE_MASTER) {
			I2S[i2s_num]->conf.tx_slave_mod = 0;	//TX MASTER
			I2S[i2s_num]->conf.rx_slave_mod = 1;	//RX Slave
		} else {
			I2S[i2s_num]->conf.tx_slave_mod = 1;	//RX Slave
			I2S[i2s_num]->conf.rx_slave_mod = 1;	//RX Slave
		}
	}

	g_i2sdevice[i2s_num]->use_apll = i2s_config->use_apll;
	g_i2sdevice[i2s_num]->fixed_mclk = i2s_config->fixed_mclk;

	return OK;
}

/****************************************************************************
* Name: i2s_set_clk
*
* Description:
*   Set the I2S clock.
*
* Input Parameters:
*   dev  - Device-specific state data
*   rate - The I2S sample rate in samples (not bits) per second
*   bits - bits in per sample
*   ch   - channel count
* Returned Value:
*   Returns the resulting bitrate
*
****************************************************************************/
#define Denom_Int           (64UL)
#define APPL_ENABLE         1

#if defined(APPL_ENABLE) && (0 < APPL_ENABLE)
static float i2s_apll_get_fi2s(int bits_per_sample, int sdm0, int sdm1, int sdm2, int odir)
{

	int f_xtal = (int)rtc_clk_xtal_freq_get() * 1000000;
	uint32_t is_rev0 = (GET_PERI_REG_BITS2(EFUSE_BLK0_RDATA3_REG, 1, 15) == 0);

	if (is_rev0) {
		sdm0 = 0;
		sdm1 = 0;
	}

	float fout = f_xtal * (sdm2 + sdm1 / 256.0f + sdm0 / 65536.0f + 4);
	if (fout < APLL_MIN_FREQ || fout > APLL_MAX_FREQ) {
		return APLL_MAX_FREQ;
	}

	float fpll = fout / (2 * (odir + 2));	//== fi2s (N=1, b=0, a=1)
	return fpll / 2;
}

static int i2s_apll_calculate_fi2s(int rate, int bits_per_sample, int *sdm0, int *sdm1, int *sdm2, int *odir)
{

	int _odir, _sdm0, _sdm1, _sdm2;
	float avg;
	float min_rate, max_rate, min_diff;
	if (rate / bits_per_sample / 2 / 8 < APLL_I2S_MIN_RATE) {
		return ERROR;
	}

	*sdm0 = 0;
	*sdm1 = 0;
	*sdm2 = 0;
	*odir = 0;

	min_diff = APLL_MAX_FREQ;
	for (_sdm2 = 4; _sdm2 < 9; _sdm2++) {
		max_rate = i2s_apll_get_fi2s(bits_per_sample, 255, 255, _sdm2, 0);
		min_rate = i2s_apll_get_fi2s(bits_per_sample, 0, 0, _sdm2, 31);
		avg = (max_rate + min_rate) / 2;
		if (abs(avg - rate) < min_diff) {
			min_diff = abs(avg - rate);
			*sdm2 = _sdm2;
		}
	}

	min_diff = APLL_MAX_FREQ;

	for (_odir = 0; _odir < 32; _odir++) {
		max_rate = i2s_apll_get_fi2s(bits_per_sample, 255, 255, *sdm2, _odir);
		min_rate = i2s_apll_get_fi2s(bits_per_sample, 0, 0, *sdm2, _odir);
		avg = (max_rate + min_rate) / 2;
		if (abs(avg - rate) < min_diff) {
			min_diff = abs(avg - rate);
			*odir = _odir;
		}
	}

	min_diff = APLL_MAX_FREQ;
	for (_sdm1 = 0; _sdm1 < 256; _sdm1++) {
		max_rate = i2s_apll_get_fi2s(bits_per_sample, 255, _sdm1, *sdm2, *odir);
		min_rate = i2s_apll_get_fi2s(bits_per_sample, 0, _sdm1, *sdm2, *odir);
		avg = (max_rate + min_rate) / 2;
		if (abs(avg - rate) < min_diff) {
			min_diff = abs(avg - rate);
			*sdm1 = _sdm1;
		}
	}

	min_diff = APLL_MAX_FREQ;
	for (_sdm0 = 0; _sdm0 < 256; _sdm0++) {
		avg = i2s_apll_get_fi2s(bits_per_sample, _sdm0, *sdm1, *sdm2, *odir);
		if (abs(avg - rate) < min_diff) {
			min_diff = abs(avg - rate);
			*sdm0 = _sdm0;
		}
	}

	return OK;
}

#endif							
							
int i2s_set_clk(struct esp32_i2s_s *priv, uint32_t rate, i2s_bits_per_sample_t bits, i2s_channel_t ch)
{
	/* According to hardware codec requirement(supported 256fs or 384fs) */
	int factor = (256 % bits) ? 384 : 256;
	int clkmInteger = 0;
#if defined(APPL_ENABLE) && (0 < APPL_ENABLE)
	double clkmDecimals = 0;
	double bck = 0;
	double clkmdiv = 0;
#else
	int clkmDecimals = 0;
	int bck = 0;
	int clkmdiv = 0;
#endif
	int channel = 2;
	int i2s_num = priv->i2s_num;

	if (bits > I2S_BITS_PER_SAMPLE_32BIT || bits < I2S_BITS_PER_SAMPLE_16BIT || (bits % 8) != 0) {
		i2sinfo("[I2S] Invalid bits per sample\n");
		return ERROR;
	}

	i2sinfo("[I2S] enter set clk:%d, %d, %d...\n", rate, bits, ch);
	if (NULL == priv) {
		i2sinfo("[I2S] g_i2sdevice Not initialized yet\n");
		return ERROR;
	}
	priv->sample_rate = rate;

	clkmdiv = I2S_BASE_CLK / (rate * factor);
	if (clkmdiv > 256) {
		i2sinfo("[I2S] clkmdiv is too large\n");
		return ERROR;
	}
	/* stop all transfer before configure I2S */
	i2s_stop((struct i2s_dev_s *)priv, I2S_RX);
	i2s_stop((struct i2s_dev_s *)priv, I2S_TX);

	// wait all on-going writing finish
	i2s_exclsem_take(priv);

	uint32_t cur_mode = 0;
	if (priv->channel_num != ch) {
		priv->channel_num = (ch == 2) ? 2 : 1;
		cur_mode = I2S[i2s_num]->fifo_conf.tx_fifo_mod;
		I2S[i2s_num]->fifo_conf.tx_fifo_mod = (ch == 2) ? cur_mode - 1 : cur_mode + 1;
		cur_mode = I2S[i2s_num]->fifo_conf.rx_fifo_mod;
		I2S[i2s_num]->fifo_conf.rx_fifo_mod = (ch == 2) ? cur_mode - 1 : cur_mode + 1;
		I2S[i2s_num]->conf_chan.tx_chan_mod = (ch == 2) ? 0 : 1;
		I2S[i2s_num]->conf_chan.rx_chan_mod = (ch == 2) ? 0 : 1;

		i2sinfo("[I2S] channel_num:%d tx_chan_mode:%d, tx_fifi_mode: %d; rx_chan_mode:%d, rx_fifi_mode: %d; \n", priv->channel_num, I2S[i2s_num]->conf_chan.tx_chan_mod, I2S[i2s_num]->fifo_conf.tx_fifo_mod, I2S[i2s_num]->conf_chan.rx_chan_mod, I2S[i2s_num]->fifo_conf.rx_fifo_mod);
	}

	if (bits != priv->bits_per_sample) {
		//change fifo mode
		if (priv->bits_per_sample <= 16 && bits > 16) {
			I2S[i2s_num]->fifo_conf.tx_fifo_mod += 2;
			I2S[i2s_num]->fifo_conf.rx_fifo_mod += 2;
		} else if (priv->bits_per_sample > 16 && bits <= 16) {
			I2S[i2s_num]->fifo_conf.tx_fifo_mod -= 2;
			I2S[i2s_num]->fifo_conf.rx_fifo_mod -= 2;
		}

		priv->bits_per_sample = bits;

		// Round bytes_per_sample up to next multiple of 16 bits
		int halfwords_per_sample = (priv->bits_per_sample + 15) / 16;
		priv->bytes_per_sample = halfwords_per_sample * 2;

		i2sinfo("[I2S] bits_per_sample:%d tx_chan_mode:%d, tx_fifi_mode: %d; rx_chan_mode:%d, rx_fifi_mode: %d; \n", priv->bits_per_sample, I2S[i2s_num]->conf_chan.tx_chan_mod, I2S[i2s_num]->fifo_conf.tx_fifo_mod, I2S[i2s_num]->conf_chan.rx_chan_mod, I2S[i2s_num]->fifo_conf.rx_fifo_mod);
	}
#if defined(APPL_ENABLE) && (0 < APPL_ENABLE)
	double mclk;
	if (g_i2sdevice[i2s_num]->mode & (I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN)) {
		//DAC uses bclk as sample clock, not WS. WS can be something arbitrary.
		//Rate as given to this function is the intended sample rate;
		//According to the TRM, WS clk equals to the sample rate, and bclk is double the speed of WS
		uint32_t b_clk = rate * 2;
		int factor2 = 60;
		mclk = b_clk * factor2;
		clkmdiv = ((double)I2S_BASE_CLK) / mclk;
		clkmInteger = clkmdiv;
		clkmDecimals = (clkmdiv - clkmInteger) / Denom_Int;
		bck = mclk / b_clk;
	} else if (g_i2sdevice[i2s_num]->mode & I2S_MODE_PDM) {
		uint32_t b_clk = 0;
		if (g_i2sdevice[i2s_num]->mode & I2S_MODE_TX) {
			int fp = I2S[i2s_num]->pdm_freq_conf.tx_pdm_fp;
			int fs = I2S[i2s_num]->pdm_freq_conf.tx_pdm_fs;
			b_clk = rate * 64 * (fp / fs);
		} else if (g_i2sdevice[i2s_num]->mode & I2S_MODE_RX) {
			b_clk = rate * 64 * (I2S[i2s_num]->pdm_conf.rx_sinc_dsr_16_en + 1);
		}
		int factor2 = 5;
		mclk = b_clk * factor2;
		clkmdiv = ((double)I2S_BASE_CLK) / mclk;
		clkmInteger = clkmdiv;
		clkmDecimals = (clkmdiv - clkmInteger) * Denom_Int;
		bck = mclk / b_clk;
	} else {
		clkmInteger = clkmdiv;
		clkmDecimals = (clkmdiv - clkmInteger) * Denom_Int;
		mclk = clkmInteger + clkmDecimals / Denom_Int;
		bck = factor / (bits * channel);
	}

	int sdm0, sdm1, sdm2, odir, m_scale = 8;
	int fi2s_clk = rate * channel * bits * m_scale;
	if (g_i2sdevice[i2s_num]->use_apll && g_i2sdevice[i2s_num]->fixed_mclk) {
		fi2s_clk = g_i2sdevice[i2s_num]->fixed_mclk;
		m_scale = fi2s_clk / bits / rate / channel;
	}
	if (g_i2sdevice[i2s_num]->use_apll && i2s_apll_calculate_fi2s(fi2s_clk, bits, &sdm0, &sdm1, &sdm2, &odir) == OK) {
		i2sinfo("sdm0=%d, sdm1=%d, sdm2=%d, odir=%d", sdm0, sdm1, sdm2, odir);
		rtc_clk_apll_enable(1, sdm0, sdm1, sdm2, odir);
		I2S[i2s_num]->clkm_conf.clkm_div_num = 1;
		I2S[i2s_num]->clkm_conf.clkm_div_b = 0;
		I2S[i2s_num]->clkm_conf.clkm_div_a = 1;
		I2S[i2s_num]->sample_rate_conf.tx_bck_div_num = m_scale;
		I2S[i2s_num]->sample_rate_conf.rx_bck_div_num = m_scale;
		I2S[i2s_num]->clkm_conf.clka_en = 1;
		clkmdiv = i2s_apll_get_fi2s(bits, sdm0, sdm1, sdm2, odir);
		i2sinfo("APLL: Req RATE: %d, real rate: %0.3f, BITS: %u, CLKM: %u, BCK_M: %u, MCLK: %0.3f, SCLK: %f, diva: %d, divb: %d", rate, clkmdiv / bits / channel / m_scale, bits, 1, m_scale, clkmdiv, clkmdiv / 8, 1, 0);
	} else {
		I2S[i2s_num]->clkm_conf.clka_en = 0;
		I2S[i2s_num]->clkm_conf.clkm_div_a = 63;
		I2S[i2s_num]->clkm_conf.clkm_div_b = clkmDecimals;
		I2S[i2s_num]->clkm_conf.clkm_div_num = clkmInteger;
		I2S[i2s_num]->sample_rate_conf.tx_bck_div_num = bck;
		I2S[i2s_num]->sample_rate_conf.rx_bck_div_num = bck;
		clkmdiv = (double)(I2S_BASE_CLK / (bck * bits * clkmInteger) / 2);
		i2sinfo("PLL_D2: Req RATE: %d, real rate: %0.3f, BITS: %u, CLKM: %u, BCK: %u, MCLK: %0.3f, SCLK: %f, diva: %d, divb: %d", rate, clkmdiv, bits, clkmInteger, bck, (double)I2S_BASE_CLK / mclk, clkmdiv * bits * channel, 64, clkmDecimals);
	}
#else
	clkmInteger = clkmdiv;
	clkmDecimals = I2S_BASE_CLK % (rate * factor);
	clkmDecimals = (int)((Denom_Int * clkmDecimals + rate * factor / 2) / (rate * factor));
	bck = factor / (bits * channel);
	i2sinfo("[I2S] clkmInteger=%d, clkmDecimals=%d, bck=%d,\n", clkmInteger, clkmDecimals, bck);

	I2S[i2s_num]->clkm_conf.clka_en = 0;
	I2S[i2s_num]->clkm_conf.clkm_div_a = 63;
	I2S[i2s_num]->clkm_conf.clkm_div_b = clkmDecimals;
	I2S[i2s_num]->clkm_conf.clkm_div_num = clkmInteger;
	I2S[i2s_num]->sample_rate_conf.tx_bck_div_num = bck;
	I2S[i2s_num]->sample_rate_conf.rx_bck_div_num = bck;
#endif

	I2S[i2s_num]->sample_rate_conf.tx_bits_mod = bits;
	I2S[i2s_num]->sample_rate_conf.rx_bits_mod = bits;

	// wait all writing on-going finish
	i2s_exclsem_give(priv);

	return OK;
}

/****************************************************************************
 * Name: esp32_i2s_isr_initialize
 *
 * Description:
 *   allocate isr for i2s modules
 *
 * Input Parameters:
 *   priv - Partially initialized I2S device structure.  These functions
 *          will complete the I2S specific portions of the initialization
 *
 * Returned Value:
 *   configure status
 *
 *
 ****************************************************************************/
static int esp32_i2s_isr_initialize(struct esp32_i2s_s *priv)
{
	/*register isr */
	priv->cpu_int = esp32_alloc_levelint(priv->intrupt_level);
	if (priv->cpu_int < 0) {
		/* Failed to allocate a CPU interrupt of this type */
		return priv->cpu_int;
	}

	/* Set up to receive peripheral interrupts on the current CPU */
	int cpu = 1;
#ifdef CONFIG_SMP
	cpu = up_cpu_index();
#else
	cpu = 0;
#endif

	/* Attach the GPIO peripheral to the allocated CPU interrupt */
	up_disable_irq(priv->cpu_int);
	esp32_attach_peripheral(cpu, priv->periph, priv->cpu_int);

	/* Attach and enable the IRQ */
	int ret = irq_attach(priv->isr_num, priv->isr_handler, priv);
	if (ret == OK) {
		/* Enable the CPU interrupt */
		up_enable_irq(priv->cpu_int);
	}
	return ret;
}

/****************************************************************************
* Name: i2s_samplerate
*
* Description:
*   Set the I2S sample rate.
*
* Input Parameters:
*   dev  - Device-specific state data
*   rate - The I2S sample rate in samples (not bits) per second
*
* Returned Value:
*   Returns the resulting bitrate
*
****************************************************************************/

static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)dev;
	DEBUGASSERT(priv && rate > 0);
	return i2s_set_clk(priv, rate, priv->bits_per_sample, priv->channel_num);
}

/****************************************************************************
* Name: i2s_irq_handler
*
* Description:
*   ISR function for I2S.
*
* Input Parameters:
*   irq  - irq number
*   context - context
*   arg - Device-specific state data
*
* Returned Value:
*
*
****************************************************************************/

static int i2s_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	struct esp32_i2s_s *priv = (struct esp32_i2s_s *)arg;
	uint8_t i2s_num = priv->i2s_num;
	i2s_dev_t *i2s_reg = I2S[i2s_num];

	int_st[priv->i2s_num].val = i2s_reg->int_raw.val;

	if (int_st[priv->i2s_num].out_dscr_err || int_st[priv->i2s_num].in_dscr_err) {
		i2serr("[I2S] dma desc error, interrupt status: 0x%08x\n", i2s_reg->int_st.val);
	}

	if (priv->txenab) {
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
		if (int_st[priv->i2s_num].out_eof) {
			i2s_txdma_callback(priv, OK);
		}
#endif
	}

	if (priv->rxenab) {
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
		if (int_st[priv->i2s_num].in_suc_eof) {
			i2s_rxdma_callback(priv, OK);
		}
#endif
	}

	i2s_reg->int_clr.val = int_st[priv->i2s_num].val;

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:
 *
 * Description:
 *   Initialize the selected i2s port
 *
 * Input Parameter:
 *   none
 *
 * Returned Value:
 *   Valid i2s device structure reference on succcess; a NULL on failure
 *
 ****************************************************************************/
struct i2s_dev_s *esp32_i2s_initialize(uint16_t port)
{
	if (port >= I2S_NUM_MAX) {
		i2serr("ERROR: Port number outside the allowed port number range\n");
		return NULL;
	}
	if (g_i2sdevice[port] != NULL) {
		return &g_i2sdevice[port]->dev;
	}

	struct esp32_i2s_s *priv;
	int ret;
	i2s_config_t i2s_config = { 0 };
	const i2s_pin_config_t *p_pin_config = NULL;

	/* Allocate a new state structure for this chip select.  NOTE that there
	 * is no protection if the same chip select is used in two different
	 * chip select structures.
	 */
	priv = (struct esp32_i2s_s *)kmm_zalloc(sizeof(struct esp32_i2s_s));
	if (!priv) {
		i2serr("ERROR: Failed to allocate a chip select structure\n");
		return NULL;
	}

	/* Initialize the I2S priv device structure  */
	sem_init(&priv->exclsem, 0, 1);
	priv->dev.ops = &g_i2sops;

	/* if port does not exist or is not enabled, return error */
	switch (port) {
	case I2S_NUM_0:
#if defined(CONFIG_ESP32_I2S0) && (1 == CONFIG_ESP32_I2S0)
	{
		periph_module_enable(PERIPH_I2S0_MODULE);

		priv->periph = ESP32_PERIPH_I2S0;

		priv->isr_num = ESP32_IRQ_I2S0;

		i2s_config = i2s0_default_config;
		p_pin_config = &I2S0_pin_default;
	}
	break;
#else
	i2serr("ERROR: I2S %d is not enabled in defconfig. Run menuconfig to enable it!\n", I2S_NUM_0);
	goto errout_with_alloc;
#endif
	case I2S_NUM_1:
#if defined(CONFIG_ESP32_I2S1) && (1 == CONFIG_ESP32_I2S1)
	{
		periph_module_enable(PERIPH_I2S1_MODULE);

		priv->periph = ESP32_PERIPH_I2S1;

		priv->isr_num = ESP32_IRQ_I2S1;

		i2s_config = i2s1_default_config;
		p_pin_config = &I2S1_pin_default;
	}
	break;
#else
	i2serr("ERROR: I2S %d is not enabled in defconfig. Run menuconfig to enable it!\n", I2S_NUM_1);
	goto errout_with_clocking;
#endif
	default:
		i2serr("ERROR: Port number outside the allowed port number range\n");
		goto errout_with_clocking;
	}

	/* Initialize buffering */
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	i2s_buf_rx_initialize(priv);
	i2s_config.mode |= I2S_MODE_RX;
	sem_init(&priv->rx.isrsem, 0, 0);
	sem_setprotocol(&priv->rx.isrsem, SEM_PRIO_NONE);
	priv->rxenab = 1;
	priv->rx.dog = wd_create();
#endif

#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	i2s_buf_tx_initialize(priv);
	i2s_config.mode |= I2S_MODE_TX;
	sem_init(&priv->tx.isrsem, 0, 0);
	sem_setprotocol(&priv->tx.isrsem, SEM_PRIO_NONE);
	priv->txenab = 1;
	priv->tx.dog = wd_create();
#endif

	/* Basic settings */
	priv->i2s_num = port;
	priv->mode = i2s_config.mode;
	priv->bytes_per_sample = 0;
	priv->intrupt_level = i2s_config.int_alloc_level;

	/* configures IRQ */
	priv->isr_handler = &i2s_irq_handler;
	ret = esp32_i2s_isr_initialize(priv);
	if (ret != OK) {
		i2serr("I2S initialize: isr fails\n");
		goto errout_with_alloc;
	}

	g_i2sdevice[port] = priv;

	i2s_stop((struct i2s_dev_s *)priv, I2S_RX);
	i2s_stop((struct i2s_dev_s *)priv, I2S_TX);
	i2s_clear_intr_status(priv->i2s_num, 0xffffffff);

	ret = i2s_param_config(port, (const i2s_config_t *)&i2s_config);
	if (ret != OK) {
		i2serr("I2S initialize: i2s_param_config fails\n");
		goto errout_with_alloc;
	}
	priv->channel_num = 2;
	priv->bits_per_sample = 16;

	ret = i2s_set_clk(priv, i2s_config.sample_rate, i2s_config.bits_per_sample, (i2s_config.channel_format < I2S_CHANNEL_FMT_ONLY_RIGHT) ? 2 : 1);
	if (ret != OK) {
		i2serr("I2S initialize: i2s_set_clk fails\n");
		goto errout_with_alloc;
	}

	i2s_set_pin(port, p_pin_config);

#if defined(CONFIG_DEBUG) && defined(DEBUG_I2S_DRIVER) && (0 < DEBUG_I2S_DRIVER)
	ESP32_I2S_Dump(port);
#endif

	/* Success exit */
	return &priv->dev;

	/* Failure exits */
errout_with_alloc:
#if defined(I2S_HAVE_RX) && (0 < I2S_HAVE_RX)
	sem_destroy(&priv->bufsem_rx);
#endif
#if defined(I2S_HAVE_TX) && (0 < I2S_HAVE_TX)
	sem_destroy(&priv->bufsem_tx);
#endif

errout_with_clocking:
	sem_destroy(&priv->exclsem);
	kmm_free(priv);
	return NULL;
}
#endif							/* I2S_HAVE_RX || I2S_HAVE_TX */
