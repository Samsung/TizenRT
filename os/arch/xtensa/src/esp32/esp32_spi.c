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

/*****************************************************************************
 * Included Files
 *****************************************************************************/
#include <tinyara/config.h>

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <stddef.h>

#include <tinyara/arch.h>
#include <tinyara/irq.h>
#include <tinyara/spi/spi.h>
#include <tinyara/kmalloc.h>
#include <tinyara/fs/fs.h>

#include <arch/irq.h>
#include <arch/board/board.h>
#include "arch/chip/spi_pins.h"
#include "arch/chip/spi_struct.h"

#include "chip/periph_defs.h"
#include "chip/esp32_soc.h"
#include "chip/esp32_iomux.h"
#include "chip/esp32_gpio_sigmap.h"
#include "chip/esp32_spi_reg.h"
#include "rom/esp32_gpio.h"
#include "rom/lldesc.h"
#include "esp32_cpuint.h"
#include "esp32_config.h"
#include "esp32_gpio.h"
#include "periph_ctrl.h"

#include "esp32_spi.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/
#ifndef GPIO_PIN_COUNT
#define GPIO_PIN_COUNT                  40
#endif

//
#define HSPI_IOMUX_PIN_NUM_MISO 12
#define HSPI_IOMUX_PIN_NUM_MOSI 13
#define HSPI_IOMUX_PIN_NUM_CLK  14
#define HSPI_IOMUX_PIN_NUM_CS   15
#define HSPI_IOMUX_PIN_NUM_WP   2
#define HSPI_IOMUX_PIN_NUM_HD   4
//
#define VSPI_IOMUX_PIN_NUM_MISO 19
#define VSPI_IOMUX_PIN_NUM_MOSI 23
#define VSPI_IOMUX_PIN_NUM_CLK  18
#define VSPI_IOMUX_PIN_NUM_CS   5
#define VSPI_IOMUX_PIN_NUM_WP   22
#define VSPI_IOMUX_PIN_NUM_HD   21
//
#define SPI_PIN_FUNC_SPI    (INPUT | OUTPUT | FUNCTION_1)
#define SPI_PIN_FUNC_GPIO   (INPUT | OUTPUT | FUNCTION_2)

//
#define SPICOMMON_BUSFLAG_SLAVE         0	///< Initialize I/O in slave mode
#define SPICOMMON_BUSFLAG_MASTER        (1<<0)	///< Initialize I/O in master mode
#define SPICOMMON_BUSFLAG_NATIVE_PINS   (1<<1)	///< Check using iomux pins. Or indicates the pins are configured through the IO mux rather than GPIO matrix.
#define SPICOMMON_BUSFLAG_SCLK          (1<<2)	///< Check existing of SCLK pin. Or indicates CLK line initialized.
#define SPICOMMON_BUSFLAG_MISO          (1<<3)	///< Check existing of MISO pin. Or indicates MISO line initialized.
#define SPICOMMON_BUSFLAG_MOSI          (1<<4)	///< Check existing of MOSI pin. Or indicates CLK line initialized.
#define SPICOMMON_BUSFLAG_DUAL          (1<<5)	///< Check MOSI and MISO pins can output. Or indicates bus able to work under DIO mode.
#define SPICOMMON_BUSFLAG_WPHD          (1<<6)	///< Check existing of WP and HD pins. Or indicates WP & HD pins initialized.
#define SPICOMMON_BUSFLAG_QUAD          (SPICOMMON_BUSFLAG_DUAL|SPICOMMON_BUSFLAG_WPHD)	///< Check existing of MOSI/MISO/WP/HD pins as output. Or indicates bus able to work under QIO mode.

#define SPI_DEVICE_TXBIT_LSBFIRST          (1<<0)	///< Transmit command/address/data LSB first instead of the default MSB first
#define SPI_DEVICE_RXBIT_LSBFIRST          (1<<1)	///< Receive data LSB first instead of the default MSB first
#define SPI_DEVICE_BIT_LSBFIRST            (SPI_DEVICE_TXBIT_LSBFIRST|SPI_DEVICE_RXBIT_LSBFIRST)	///< Transmit and receive LSB first
#define SPI_DEVICE_3WIRE                   (1<<2)	///< Use MOSI (=spid) for both sending and receiving data
#define SPI_DEVICE_POSITIVE_CS             (1<<3)	///< Make CS positive during a transaction instead of negative
#define SPI_DEVICE_HALFDUPLEX              (1<<4)	///< Transmit data before receiving it, instead of simultaneously
#define SPI_DEVICE_CLK_AS_CS               (1<<5)	///< Output clock on CS line if CS is active
/** There are timing issue when reading at high frequency (the frequency is related to whether iomux pins are used, valid time after slave sees the clock).
*     - In half-duplex mode, the driver automatically inserts dummy bits before reading phase to fix the timing issue. Set this flag to disable this feature.
*     - In full-duplex mode, however, the hardware cannot use dummy bits, so there is no way to prevent data being read from getting corrupted.
*       Set this flag to confirm that you're going to work with output only, or read without dummy bits at your own risk.
*/
#define SPI_DEVICE_NO_DUMMY                (1<<6)

#define SPI_DEFAULT_FREQUENCY              (20000000)

#define TIMEOUT_SLEEP           (2)
#define EIMEOUT_VALUE           (1000)
#define TIMEOUT_COUNT           (EIMEOUT_VALUE/TIMEOUT_SLEEP)
#define SPI_DATA_BITS_UNIT      (32)
#define SPI_DATA_BYTES_UNIT     (32/8)
#define SPI_ADDR_BITS_COUNT     (32)

#undef spiinfo
#define spiinfo(format, ...)


//
#define SPI_DMA_DISABLED    0
#define SPI_DMA_COUNT       2

//Maximum amount of bytes that limited by ESP32
#define SPI_MAX_NODMA_FRAME_LEN     (64)
#define SPI_MAX_DMA_FRAME_LEN       (4096 - 16)	//Limited by lldesc_t
#define SPI_MAX_BITS_LEN            (0xffffff)	//limited by register SPI_USR_MOSI_DBITLEN & SPI_USR_MISO_DBITLEN
#define SPI_MAX_DATA_LEN            (SPI_MAX_BITS_LEN / 8)

#define DMA_CHANNEL_ENABLED(dma_chan)    (BIT(dma_chan-1))

/****************************************************************************
* Private Types
****************************************************************************/
enum spi_dma_channel_e {
	DMA_CHANNEL_NONE = 0,
	DMA_CHANNEL_1 = 1,
	DMA_CHANNEL_2 = 2,
	DMA_CHANNEL_MAX = 3,
};

enum spi_cs_e {
	NO_CS = -1,
	CS0 = 0,
	CS1,
	CS2,
	CS_MAX
};

typedef struct {
	spi_clock_reg_t reg;
	int eff_clk;
	int dummy_num;
	int miso_delay;
} clock_config_t;

typedef union {
	struct {
		uint32_t rd_buf_done:1;	/*The interrupt raw bit for the completion of read-buffer operation in the slave mode. */
		uint32_t wr_buf_done:1;	/*The interrupt raw bit for the completion of write-buffer operation in the slave mode. */
		uint32_t rd_sta_done:1;	/*The interrupt raw bit for the completion of read-status operation in the slave mode. */
		uint32_t wr_sta_done:1;	/*The interrupt raw bit for the completion of write-status operation in the slave mode. */
		uint32_t trans_done:1;	/*The interrupt raw bit for the completion of any operation in both the master mode and the slave mode. */
		uint32_t rd_buf_inten:1;	/*The interrupt enable bit for the completion of read-buffer operation in the slave mode. */
		uint32_t wr_buf_inten:1;	/*The interrupt enable bit for the completion of write-buffer operation in the slave mode. */
		uint32_t rd_sta_inten:1;	/*The interrupt enable bit for the completion of read-status operation in the slave mode. */
		uint32_t wr_sta_inten:1;	/*The interrupt enable bit for the completion of write-status operation in the slave mode. */
		uint32_t trans_inten:1;	/*The interrupt enable bit for the completion of any operation in both the master mode and the slave mode. */
		uint32_t cs_i_mode:2;	/*In the slave mode  this bits used to synchronize the input spi cs signal and eliminate spi cs  jitter. */
		uint32_t reserved12:5;	/*reserved */
		uint32_t last_command:3;	/*In the slave mode it is the value of command. */
		uint32_t last_state:3;	/*In the slave mode it is the state of spi state machine. */
		uint32_t trans_cnt:4;	/*The operations counter in both the master mode and the slave mode. 4: read-status */
		uint32_t cmd_define:1;	/*1: slave mode commands are defined in SPI_SLAVE3.  0: slave mode commands are fixed as: 1: write-status 2: write-buffer and 3: read-buffer. */
		uint32_t wr_rd_sta_en:1;	/*write and read status enable  in the slave mode */
		uint32_t wr_rd_buf_en:1;	/*write and read buffer enable in the slave mode */
		uint32_t slave_mode:1;	/*1: slave mode 0: master mode. */
		uint32_t sync_reset:1;	/*Software reset enable, reset the spi clock line cs line and data lines. */
	};
	uint32_t val;
} spi_reg_slave_t;

typedef struct {
	const uint8_t port;
	const uint8_t spiclk_out;	//
	const uint8_t spiclk_in;
	const uint8_t spimosi_out;	//SPID
	const uint8_t spimosi_in;	//
	const uint8_t spimiso_out;	//SPIQ
	const uint8_t spimiso_in;	//
	const uint8_t spiwp_out;
	const uint8_t spiwp_in;
	const uint8_t spihd_out;
	const uint8_t spihd_in;
	const uint8_t spics_out[3];	// /CS GPIO output mux signals
	const uint8_t spics_in;

} esp32_spi_pins_sig_t;

struct esp32_spidev_s {
	struct spi_dev_s spidev;

	spi_dev_t *hw;

	uint8_t port;				/* port */

	uint8_t periph_module;

	uint8_t initiallized;		/* Is initiallized */

	uint8_t periph_isr;			/* ISR peripheral ID */
	int intrupt_level;
	int cpu_int;
	int isr_num;				/* isr number */
	xcpt_t isr_handler;			/* irs handler */

	enum spi_mode_e comm_mode;	/* SPI mode */
	uint32_t frequency;

	uint32_t work_mode;			/* */
	int8_t dma_chan;			/* */

	uint8_t gpio_clk;
	uint8_t gpio_miso;
	uint8_t gpio_mosi;
	uint8_t gpio_nss[MAX_CS_NUM];
	uint8_t cur_cs;
	uint8_t prev_cs;
	uint8_t gpio_quadwp;
	uint8_t gpio_quadhd;

	uint32_t max_transfer_sz;

	clock_config_t clk_cfg;

#ifndef CONFIG_SPI_POLLWAIT
	sem_t xfrsem;				/* Wait for transfer to complete */
#endif

#ifndef CONFIG_SPI_OWNBUS
	sem_t exclsem;
#endif
	lldesc_chain_t dmadesc_rx;
	lldesc_chain_t dmadesc_tx;
};

typedef struct {
	uint8_t command_bits;		///< Default amount of bits in command phase (0-16), used when ``SPI_TRANS_VARIABLE_CMD`` is not used, otherwise ignored.
	uint8_t address_bits;		///< Default amount of bits in address phase (0-64), used when ``SPI_TRANS_VARIABLE_ADDR`` is not used, otherwise ignored.
	uint8_t dummy_bits;			///< Amount of dummy bits to insert between address and data phase
	uint8_t mode;				///< SPI mode (0-3)
	uint8_t duty_cycle_pos;		///< Duty cycle of positive clock, in 1/256th increments (128 = 50%/50% duty). Setting this to 0 (=not setting it) is equivalent to setting this to 128.
	uint8_t cs_ena_pretrans;	///< Amount of SPI bit-cycles the cs should be activated before the transmission (0-16). This only works on half-duplex transactions.
	uint8_t cs_ena_posttrans;	///< Amount of SPI bit-cycles the cs should stay active after the transmission (0-16)
	int clock_speed_hz;			///< Clock speed, divisors of 80MHz, in Hz. See ``SPI_MASTER_FREQ_*``.
	int input_delay_ns;			/**< Maximum data valid time of slave. The time required between SCLK and MISO
								valid, including the possible clock delay from slave to master. The driver uses this value to give an extra
								delay before the MISO is ready on the line. Leave at 0 unless you know you need a delay. For better timing
								performance at high frequency (over 8MHz), it's suggest to have the right value.
								*/
	int spics_io_num;			///< CS GPIO pin for this device, or -1 if not used
	uint32_t flags;				///< Bitwise OR of SPI_DEVICE_* flags
} spi_device_interface_config_t;

typedef struct {
	sem_t dma_lock;
	uint8_t chan_enabled;
} spi_dma_claim_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* SPI Driver Methods */
static int spi_lock(FAR struct spi_dev_s *dev, bool lock);
static void spi_select(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
static uint32_t spi_setfrequency(FAR struct spi_dev_s *dev, uint32_t frequency);
static void spi_setmode(FAR struct spi_dev_s *dev, enum spi_mode_e mode);
static void spi_setbits(FAR struct spi_dev_s *dev, int nbits);
static uint16_t spi_send(FAR struct spi_dev_s *dev, uint16_t wd);
static void spi_exchange(FAR struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords);

#ifndef CONFIG_SPI_EXCHANGE
static void spi_sndblock(FAR struct spi_dev_s *dev, const void *txbuffer, size_t nwords);
static void spi_recvblock(FAR struct spi_dev_s *dev, void *rxbuffer, size_t nwords);
#endif

static int esp32_spi_cal_clock(int fapb, int hz, int duty_cycle, uint32_t *reg_o);
static void esp32_spi_get_timing(bool gpio_is_used, int input_delay_ns, int eff_clk, int *dummy_o, int *cycles_remain_o);
static inline void esp32_spi_set_clock(struct esp32_spidev_s *priv, spi_clock_reg_t reg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static const struct spi_ops_s g_spiops = {
#ifndef CONFIG_SPI_OWNBUS
	.lock = spi_lock,
#endif
	.select = spi_select,
	.setfrequency = spi_setfrequency,
	.setmode = (void *)spi_setmode,
	.setbits = (void *)spi_setbits,
	.status = 0,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata = 0,
#endif
	.send = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange = spi_exchange,
#else
	.sndblock = spi_sndblock,
	.recvblock = spi_recvblock,
#endif
	.registercallback = 0,
};

const esp32_spi_pins_sig_t s_spi2_pin_sig = {
	.port = HSPI_PORT,
	.spiclk_out = HSPICLK_OUT_IDX,
	.spiclk_in = HSPICLK_IN_IDX,
	.spimosi_out = HSPID_OUT_IDX,
	.spimiso_out = HSPIQ_OUT_IDX,
	.spiwp_out = HSPIWP_OUT_IDX,
	.spihd_out = HSPIHD_OUT_IDX,
	.spimosi_in = HSPID_IN_IDX,
	.spimiso_in = HSPIQ_IN_IDX,
	.spiwp_in = HSPIWP_IN_IDX,
	.spihd_in = HSPIHD_IN_IDX,
	.spics_out = {HSPICS0_OUT_IDX, HSPICS1_OUT_IDX, HSPICS2_OUT_IDX},
	.spics_in = HSPICS0_IN_IDX,
};

const esp32_spi_pins_sig_t s_spi3_pin_sig = {
	.port = VSPI_PORT,
	.spiclk_out = VSPICLK_OUT_MUX_IDX,
	.spiclk_in = VSPICLK_IN_IDX,
	.spimosi_out = VSPID_OUT_IDX,
	.spimiso_out = VSPIQ_OUT_IDX,
	.spiwp_out = VSPIWP_OUT_IDX,
	.spihd_out = VSPIHD_OUT_IDX,
	.spimosi_in = VSPID_IN_IDX,
	.spimiso_in = VSPIQ_IN_IDX,
	.spiwp_in = VSPIWP_IN_IDX,
	.spihd_in = VSPIHD_IN_IDX,
	.spics_out = {VSPICS0_OUT_IDX, VSPICS1_OUT_IDX, VSPICS2_OUT_IDX},
	.spics_in = VSPICS0_IN_IDX,
};

const spi_device_interface_config_t def_dev_config = {
	.command_bits = 0,
	.address_bits = 0,
	.dummy_bits = 0,
	.mode = 0,
	.duty_cycle_pos = 128,
	.cs_ena_pretrans = 1,
	.cs_ena_posttrans = 1,
	.clock_speed_hz = 1,
	.input_delay_ns = 0,
	.spics_io_num = 0,
	.flags = SPI_DEVICE_HALFDUPLEX,
};

static struct esp32_spidev_s g_spi2dev = {
	.spidev = {.ops = &g_spiops},
	.hw = &SPI2,
#if defined(CONFIG_SPI2_DMA_CHANNEL) && (CONFIG_SPI2_DMA_CHANNEL > SPI_DMA_DISABLED) && (CONFIG_SPI2_DMA_CHANNEL <= SPI_DMA_COUNT)
	.dma_chan = CONFIG_SPI2_DMA_CHANNEL,
#else
	.dma_chan = 0,
#endif
	.port = HSPI_PORT,
	.periph_module = PERIPH_HSPI_MODULE,
	.initiallized = 0,

#if defined(CONFIG_GPIO_SPI2_CLK_PIN) || defined(CONFIG_GPIO_SPI2_MISO_PIN) || defined(CONFIG_GPIO_SPI2_MOSI_PIN)
	.work_mode = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_DUAL ,
#else
	.work_mode = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_DUAL | SPICOMMON_BUSFLAG_NATIVE_PINS,
#endif
	.comm_mode = SPIDEV_MODE0,
	.frequency = SPI_DEFAULT_FREQUENCY,

	.periph_isr = ESP32_PERIPH_SPI2,
	.intrupt_level = 1,
	.isr_num = ESP32_IRQ_SPI2,

	.gpio_nss = {
#if defined(CONFIG_GPIO_SPI2_CS0_PIN)
		CONFIG_GPIO_SPI2_CS0_PIN,
#else
		HSPI_IOMUX_PIN_NUM_CS,
#endif
#if defined(CONFIG_GPIO_SPI2_CS1_PIN)
		CONFIG_GPIO_SPI2_CS1_PIN,
#else
		-1,
#endif
#if defined(CONFIG_GPIO_SPI2_CS2_PIN)
		CONFIG_GPIO_SPI2_CS2_PIN,
#else
		-1
#endif
	},

#if defined(CONFIG_GPIO_SPI2_CLK_PIN)
	.gpio_clk = CONFIG_GPIO_SPI2_CLK_PIN,
#else
	.gpio_clk = HSPI_IOMUX_PIN_NUM_CLK,
#endif

#if defined(CONFIG_GPIO_SPI2_MISO_PIN)
	.gpio_miso = CONFIG_GPIO_SPI2_MISO_PIN,
#else
	.gpio_miso = (HSPI_IOMUX_PIN_NUM_MISO),
#endif

#if defined(CONFIG_GPIO_SPI2_MOSI_PIN)
	.gpio_mosi = CONFIG_GPIO_SPI2_MOSI_PIN,
#else
	.gpio_mosi = HSPI_IOMUX_PIN_NUM_MOSI,
#endif

#if defined(CONFIG_GPIO_SPI2_QUADWP_PIN)
	.gpio_quadwp = CONFIG_GPIO_SPI2_QUADWP_PIN,
#else
	.gpio_quadwp = -1,
#endif
#if defined(CONFIG_GPIO_SPI2_QUADHD_PIN)
	.gpio_quadhd = CONFIG_GPIO_SPI2_QUADHD_PIN;
#else
	.gpio_quadhd = -1,
#endif
};

static struct esp32_spidev_s g_spi3dev = {
	.spidev = {.ops = &g_spiops},
	.hw = &SPI3,
	.frequency = SPI_DEFAULT_FREQUENCY,
#if defined(CONFIG_SPI3_DMA_CHANNEL) && (CONFIG_SPI3_DMA_CHANNEL > SPI_DMA_DISABLED) && (CONFIG_SPI3_DMA_CHANNEL <= SPI_DMA_COUNT)
	.dma_chan = CONFIG_SPI3_DMA_CHANNEL,
#else
	.dma_chan = 0,
#endif
#if defined(CONFIG_GPIO_SPI3_CLK_PIN) || defined(CONFIG_GPIO_SPI3_MISO_PIN) || defined(CONFIG_GPIO_SPI3_MOSI_PIN)
	.work_mode = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_DUAL ,
#else
	.work_mode = SPICOMMON_BUSFLAG_MASTER | SPICOMMON_BUSFLAG_DUAL | SPICOMMON_BUSFLAG_NATIVE_PINS,
#endif
	.comm_mode = SPIDEV_MODE0,

	.port = VSPI_PORT,
	.periph_module = PERIPH_VSPI_MODULE,
	.initiallized = 0,

	.periph_isr = ESP32_PERIPH_SPI3,
	.intrupt_level = 1,
	.isr_num = ESP32_IRQ_SPI3,

	.gpio_nss = {
#if defined(CONFIG_GPIO_SPI3_CS0_PIN)
		CONFIG_GPIO_SPI3_CS0_PIN,
#else
		VSPI_IOMUX_PIN_NUM_CS,
#endif
#if defined(CONFIG_GPIO_SPI3_CS1_PIN)
		CONFIG_GPIO_SPI3_CS1_PIN,
#else
		-1,
#endif
#if defined(CONFIG_GPIO_SPI3_CS2_PIN)
		CONFIG_GPIO_SPI3_CS2_PIN,
#else
		-1
#endif
	},
	
#if defined(CONFIG_GPIO_SPI3_CLK_PIN)
	.gpio_clk = CONFIG_GPIO_SPI3_CLK_PIN,
#else
	.gpio_clk = VSPI_IOMUX_PIN_NUM_CLK,
#endif

#if defined(CONFIG_GPIO_SPI3_MISO_PIN)
	.gpio_miso = CONFIG_GPIO_SPI3_MISO_PIN,
#else
	.gpio_miso = (VSPI_IOMUX_PIN_NUM_MISO),
#endif
#if defined(CONFIG_GPIO_SPI3_MOSI_PIN)
	.gpio_mosi = CONFIG_GPIO_SPI3_MOSI_PIN,
#else
	.gpio_mosi = (VSPI_IOMUX_PIN_NUM_MOSI),
#endif
#if defined(CONFIG_GPIO_SPI3_QUADWP_PIN)
	.gpio_quadwp = CONFIG_GPIO_SPI3_QUADWP_PIN,
#else
	.gpio_quadwp = -1,
#endif
#if defined(CONFIG_GPIO_SPI3_QUADHD_PIN)
	.gpio_quadhd = CONFIG_GPIO_SPI3_QUADHD_PIN;
#else
	.gpio_quadhd = -1,
#endif
};

static volatile spi_dma_claim_t g_spi_dma = {
	.dma_lock = {0},
	.chan_enabled = 0,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: spi_lock
 *
 * Description:
 *   On SPI busses where there are multiple devices, it will be necessary to
 *   lock SPI to have exclusive access to the busses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the SPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the SPI is properly
 *   configured for the device.  If the SPI buss is being shared, then it
 *   may have been left in an incompatible state.
 *
 ****************************************************************************/
static int spi_lock(struct spi_dev_s *dev, bool lock)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;

	if (lock) {
		while (sem_wait(&priv->exclsem) != 0) {
			DEBUGASSERT(errno == EINTR);
		}
	} else {
		(void)sem_post(&priv->exclsem);
	}

	return OK;
}

/****************************************************************************
 * Name: spi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 ****************************************************************************/
static uint32_t spi_setfrequency(struct spi_dev_s *dev, uint32_t frequency)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;

	if (frequency <= APB_CLK_FREQ && frequency != priv->frequency) {
		const int duty_cycle = 128;
		spi_clock_reg_t clk_reg = { 0 };
		int dummy_required, miso_delay;
		priv->frequency = frequency;
		int eff_clk = esp32_spi_cal_clock(APB_CLK_FREQ, priv->frequency, duty_cycle, (uint32_t *)&clk_reg);
		esp32_spi_get_timing(!(priv->work_mode & SPICOMMON_BUSFLAG_NATIVE_PINS), def_dev_config.input_delay_ns, eff_clk, &dummy_required, &miso_delay);
		priv->clk_cfg.eff_clk = eff_clk;
		priv->clk_cfg.reg = clk_reg;
		priv->clk_cfg.dummy_num = dummy_required;
		priv->clk_cfg.miso_delay = miso_delay;

		//Configure clock settings
		esp32_spi_set_clock(priv, priv->clk_cfg.reg);
		spiinfo("[SPI] frequency %d, reg: %08x\n", frequency, clk_reg.val);
	}
	return OK;
}

/****************************************************************************
 * Name: spi_select
 *
 * Description:
 *   Enable/disable the SPI slave select. The implementation of this method
 *   must include handshaking:  If a device is selected, it must hold off
 *   all other attempts to select the device until the device is deselecte.
 *
 ****************************************************************************/
static void spi_select(struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;

	uint32_t value = selected > 0 ? 0 : 1;
	switch (priv->cur_cs) {
	case 0:
		priv->hw->pin.cs0_dis = value;
		break;
	case 1:
		priv->hw->pin.cs1_dis = value;
		break;
	case 2:
		priv->hw->pin.cs2_dis = value;
		break;

	default:
		break;
	}
}

/****************************************************************************
 * Name: spi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e for mode definitions
 *
 ****************************************************************************/
static void spi_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;

	//Configure polarity
	switch (mode) {
	case SPIDEV_MODE0: {
		priv->hw->pin.ck_idle_edge = 0;
		priv->hw->user.ck_out_edge = 0;
	}
	break;
	case SPIDEV_MODE1: {
		priv->hw->pin.ck_idle_edge = 0;
		priv->hw->user.ck_out_edge = 1;
	}
	break;
	case SPIDEV_MODE2: {
		priv->hw->pin.ck_idle_edge = 1;
		priv->hw->user.ck_out_edge = 1;
	}
	break;
	case SPIDEV_MODE3: {
		priv->hw->pin.ck_idle_edge = 1;
		priv->hw->user.ck_out_edge = 0;
	}
	break;
	default:
		break;
	}
}

/****************************************************************************
 * Name: spi_setbits
 *
 * Description:
 *   Support 8 bits only.
 *
 ****************************************************************************/
static void spi_setbits(struct spi_dev_s *dev, int nbits)
{
	unsigned int mode_cfg = 0;

	switch (nbits) {
	case 8:
		mode_cfg = mode_cfg;
		break;

	default:
		DEBUGASSERT(0 == 1);
		break;
	}
}

/****************************************************************************
 * Name: spi_send
 *
 * Description:
 *   Exchange one word on SPI. Currently support only byte transfers.
 *
 ****************************************************************************/
static uint16_t spi_send(struct spi_dev_s *dev, uint16_t wd)
{
	uint8_t txbyte;
	uint8_t rxbyte;

	txbyte = (uint8_t) wd;
	rxbyte = (uint8_t) 0;
	spi_exchange(dev, &txbyte, &rxbyte, 1);

	return (uint16_t) rxbyte;
}

/****************************************************************************
 * Name: spi_exchange
 *
 * Description:
 *   Exchange a block data with the SPI device. Support only byte transfers.
 *
 ****************************************************************************/

void esp32_spi_Enable_ints(struct esp32_spidev_s *priv)
{
	priv->hw->slave.trans_inten = 1;
	priv->hw->slave.rd_buf_inten = 0;
	priv->hw->slave.wr_buf_inten = 0;
	priv->hw->slave.rd_sta_inten = 0;
	priv->hw->slave.wr_sta_inten = 0;
}

void esp32_spi_Disable_ints(struct esp32_spidev_s *priv)
{
	priv->hw->slave.trans_inten = 0;
	priv->hw->slave.rd_buf_inten = 0;
	priv->hw->slave.wr_buf_inten = 0;
	priv->hw->slave.rd_sta_inten = 0;
	priv->hw->slave.wr_sta_inten = 0;
}

void esp32_spi_Clear_ints(struct esp32_spidev_s *priv)
{
	priv->hw->slave.rd_buf_done = 0;
	priv->hw->slave.wr_buf_done = 0;
	priv->hw->slave.rd_sta_done = 0;
	priv->hw->slave.wr_sta_done = 0;
	priv->hw->slave.trans_done = 0;
}

/*Set up a list of dma descriptors. dmadesc is an array of descriptors. Data is the buffer to point to.*/
int spicommon_free_dma_desc_links(lldesc_chain_t *desc_chain)
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

int spicommon_setup_dma_desc_links(lldesc_chain_t *desc_chain, int len, const uint8_t *data, bool isrx)
{
	int ret = ERROR;
	desc_chain->head = NULL;
	desc_chain->tail = NULL;
	int n = 0;
	while (n < len) {
		lldesc_t *cdesc = (lldesc_t *)kmm_zalloc(sizeof(lldesc_t));
		if (cdesc == NULL) {
			ret = -ENOMEM;
			spiinfo("[SPI] setup dma links fails\n");
			goto errout_with_lldesc;
		}
		if (desc_chain->head == NULL) {
			desc_chain->head = cdesc;
		}

		int count = len - n;
		if (count > SPI_MAX_DMA_FRAME_LEN) {
			count = SPI_MAX_DMA_FRAME_LEN;
		}
		if (isrx) {
			/*Receive needs DMA length rounded to next 32-bit boundary */
			count = (count + 3) & (~3);
		}

		cdesc->length = count;
		cdesc->size = count;
		cdesc->offset = 0;
		cdesc->owner = 1;
		cdesc->eof = 0;
		cdesc->sosf = 0;
		cdesc->buf = (uint8_t *) data;
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
	}
	return ret;

errout_with_lldesc:
	spicommon_free_dma_desc_links(desc_chain);
	return ret;
}

int spicommon_dma_chan_claim(int dma_chan)
{
	int ret = ERROR;
	DEBUGASSERT(dma_chan == DMA_CHANNEL_1 || dma_chan == DMA_CHANNEL_2);

	sem_wait((sem_t *)&g_spi_dma.dma_lock);
	if (!(g_spi_dma.chan_enabled & DMA_CHANNEL_ENABLED(dma_chan))) {
		// get the channel only when it's not claimed yet.
		g_spi_dma.chan_enabled |= DMA_CHANNEL_ENABLED(dma_chan);
		ret = OK;
	}
	periph_module_enable(PERIPH_SPI_DMA_MODULE);
	sem_post((sem_t *)&g_spi_dma.dma_lock);

	return ret;
}

int spicommon_dma_chan_in_use(int dma_chan)
{
	DEBUGASSERT(dma_chan == DMA_CHANNEL_1 || dma_chan == DMA_CHANNEL_2);
	if (dma_chan != DMA_CHANNEL_1 && dma_chan != DMA_CHANNEL_2) {
		return 0;
	}
	return (g_spi_dma.chan_enabled & DMA_CHANNEL_ENABLED(dma_chan));
}

int spicommon_dma_chan_free(int dma_chan)
{
	if (0 < spicommon_dma_chan_in_use(dma_chan)) {
		sem_wait((sem_t *)&g_spi_dma.dma_lock);
		g_spi_dma.chan_enabled &= ~DMA_CHANNEL_ENABLED(dma_chan);
		if (g_spi_dma.chan_enabled == 0) {
			//disable the DMA only when all the channels are freed.
			periph_module_disable(PERIPH_SPI_DMA_MODULE);
		}
		sem_post((sem_t *)&g_spi_dma.dma_lock);
	}

	return OK;
}

/* The function is called to send a new transaction.
 * Setup the transaction-specified registers and linked-list used by the DMA.
 */
static void spi_new_trans(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nbits, uint16_t cmd, uint64_t trans_addr)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;
	int extra_dummy = 0;
	int cmdlen = 0;
	int addrlen = 0;

	/*We should be done with the transmission. */
	assert(priv->hw->cmd.usr == 0);

	/*clear interrupt bits */
	esp32_spi_Disable_ints(priv);
	esp32_spi_Clear_ints(priv);

	/* Reset DMA peripheral */
	priv->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
	priv->hw->dma_out_link.addr = 0;
	priv->hw->dma_out_link.start = 0;
	priv->hw->dma_in_link.addr = 0;
	priv->hw->dma_in_link.start = 0;
	priv->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);
	priv->hw->dma_conf.out_data_burst_en = 1;
	priv->hw->dma_conf.indscr_burst_en = 1;
	priv->hw->dma_conf.outdscr_burst_en = 1;

	/*Fill DMA descriptors */
	if (rxbuffer != NULL) {
		if (priv->dma_chan == 0) {
			//No need to setup anything; we'll copy the result out of the work registers directly later.
		} else {
			spicommon_setup_dma_desc_links(&(priv->dmadesc_rx), nbits / 8, (uint8_t *)rxbuffer, true);
			//priv->hw->dma_in_link.addr = (int)(priv->dmadesc_rx.head) & 0xFFFFF;
			priv->hw->dma_in_link.addr = (uint32_t)(priv->dmadesc_rx.head);
			priv->hw->dma_in_link.start = 1;
			spiinfo("[SPI] DMA start rx %p~%p; %d...\n", priv->dmadesc_rx.head, priv->dmadesc_rx.tail, nbits);
		}
		//when no_dummy is not set and in half-duplex mode, sets the dummy bit if RX phase exist
		if (((def_dev_config.flags & SPI_DEVICE_NO_DUMMY) == 0) && (def_dev_config.flags & SPI_DEVICE_HALFDUPLEX)) {
			extra_dummy = priv->clk_cfg.dummy_num;
		}
	} else {
		//DMA temporary workaround: let RX DMA work somehow to avoid the issue in ESP32 v0/v1 silicon
		if (priv->dma_chan != 0) {
			priv->hw->dma_in_link.addr = 0;
			priv->hw->dma_in_link.start = 1;
		}
	}

	if (txbuffer != NULL) {
		/*Copy data to registers manually if Need */
		if (priv->dma_chan == 0) {
			uint32_t *buf = (uint32_t *) txbuffer;
			for (int x = 0; x < nbits; x += SPI_DATA_BITS_UNIT) {
				addrlen = x / SPI_DATA_BITS_UNIT;
				priv->hw->data_buf[addrlen] = buf[addrlen];
			}
		} else {
			spicommon_setup_dma_desc_links(&(priv->dmadesc_tx), nbits / 8, (uint8_t *)txbuffer, false);
			priv->hw->dma_out_link.addr = (uint32_t)(priv->dmadesc_tx.head);
			priv->hw->dma_out_link.start = 1;
			spiinfo("[SPI] DMA start tx %p~%p; %d...\n", priv->dmadesc_tx.head, priv->dmadesc_tx.tail, nbits);
		}
	}
	/* SPI iface needs to be configured for a delay in some cases.
	   configure dummy bits */
	priv->hw->user.usr_dummy = (def_dev_config.dummy_bits + extra_dummy) ? 1 : 0;
	priv->hw->user1.usr_dummy_cyclelen = def_dev_config.dummy_bits + extra_dummy - 1;

	int miso_long_delay = 0;
	if (priv->clk_cfg.miso_delay < 0) {
		//if the data comes too late, delay half a SPI clock to improve reading
		miso_long_delay = 1;
		priv->hw->ctrl2.miso_delay_num = 0;
	} else {
		//if the data is so fast that dummy_bit is used, delay some apb clocks to meet the timing
		priv->hw->ctrl2.miso_delay_num = extra_dummy ? priv->clk_cfg.miso_delay : 0;
	}

	if (miso_long_delay) {
		switch (def_dev_config.mode) {
		case 0:
			priv->hw->ctrl2.miso_delay_mode = 2;
			break;
		case 1:
			priv->hw->ctrl2.miso_delay_mode = 1;
			break;
		case 2:
			priv->hw->ctrl2.miso_delay_mode = 1;
			break;
		case 3:
			priv->hw->ctrl2.miso_delay_mode = 2;
			break;
		}
	} else {
		priv->hw->ctrl2.miso_delay_mode = 0;
	}

	priv->hw->mosi_dlen.usr_mosi_dbitlen = nbits - 1;
	if (def_dev_config.flags & SPI_DEVICE_HALFDUPLEX) {
		priv->hw->miso_dlen.usr_miso_dbitlen = nbits - 1;
	} else {
		//rxlength is not used in full-duplex mode
		priv->hw->miso_dlen.usr_miso_dbitlen = nbits - 1;
	}

	//Configure bit sizes, load addr and command
	if (!(def_dev_config.flags & SPI_DEVICE_HALFDUPLEX) && def_dev_config.cs_ena_pretrans != 0) {
		/* The command and address phase is not compatible with cs_ena_pretrans
		 * in full duplex mode.
		 */
		cmdlen = 0;
		addrlen = 0;
	} else {
		cmdlen = def_dev_config.command_bits;
		addrlen = def_dev_config.address_bits;
	}

	priv->hw->user1.usr_addr_bitlen = addrlen - 1;
	priv->hw->user2.usr_command_bitlen = cmdlen - 1;
	priv->hw->user.usr_addr = addrlen ? 1 : 0;
	priv->hw->user.usr_command = cmdlen ? 1 : 0;

	if ((def_dev_config.flags & SPI_DEVICE_TXBIT_LSBFIRST) == 0) {
		/* Output command will be sent from bit 7 to 0 of command_value, and
		 * then bit 15 to 8 of the same register field. Shift and swap to send
		 * more straightly.
		 */
		priv->hw->user2.usr_command_value = __builtin_bswap32(cmd);

		/* shift the address to MSB of addr (and maybe slv_wr_status) register.
		 * output address will be sent from MSB to LSB of addr register, then
		 * comes the MSB to LSB of slv_wr_status register.*/
		if (addrlen > SPI_ADDR_BITS_COUNT) {
			priv->hw->addr = trans_addr >> (addrlen - SPI_ADDR_BITS_COUNT);
			priv->hw->slv_wr_status = trans_addr << (SPI_ADDR_BITS_COUNT * 2 - addrlen);
		} else {
			priv->hw->addr = trans_addr << (SPI_ADDR_BITS_COUNT - addrlen);
		}
	} else {
		/* The output command start from bit0 to bit 15, kept as is.
		 * The output address start from the LSB of the highest byte, i.e.
		 * addr[24] -> addr[31]
		 * ...
		 * addr[0] -> addr[7]
		 * slv_wr_status[24] -> slv_wr_status[31]
		 * ...
		 * slv_wr_status[0] -> slv_wr_status[7]
		 * So swap the byte order to let the LSB sent first.
		 */
		priv->hw->user2.usr_command_value = cmd;
		uint64_t addr = __builtin_bswap64(trans_addr);
		priv->hw->addr = addr >> SPI_ADDR_BITS_COUNT;
		priv->hw->slv_wr_status = addr;
	}

	if ((!(def_dev_config.flags & SPI_DEVICE_HALFDUPLEX) && NULL != rxbuffer) || NULL != txbuffer) {
		priv->hw->user.usr_mosi = 1;
	} else {
		priv->hw->user.usr_mosi = 0;
	}
	priv->hw->user.usr_miso = (NULL != rxbuffer) ? 1 : 0;

#ifndef CONFIG_SPI_POLLWAIT
	esp32_spi_Enable_ints(priv);
#endif

	//Kick off transfer
	priv->hw->cmd.usr = 1;
}

static void spi_post_trans(struct spi_dev_s *dev, uint32_t *pRdBuf, int flen)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;
	esp32_spi_Disable_ints(priv);
	if (priv->dma_chan) {
		priv->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
		priv->hw->dma_out_link.addr = 0;
		priv->hw->dma_out_link.start = 0;
		priv->hw->dma_in_link.addr = 0;
		priv->hw->dma_in_link.start = 0;
		priv->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);

		spicommon_free_dma_desc_links(&priv->dmadesc_rx);
		spicommon_free_dma_desc_links(&priv->dmadesc_tx);
	}
	/* Read rx data to buffer if need */
	else if (pRdBuf != NULL) {
		int index;
		for (int x = 0; x < flen * 8; x += SPI_DATA_BITS_UNIT) {
			index = x / SPI_DATA_BITS_UNIT;
			pRdBuf[index] = priv->hw->data_buf[index];
		}
	}
}

static void spi_exchange(struct spi_dev_s *dev, const void *txbuffer, void *rxbuffer, size_t nwords)
{
	FAR struct esp32_spidev_s *priv = (FAR struct esp32_spidev_s *)dev;

	size_t sent = 0;
	size_t received = 0;
	int len = nwords;

	char *tbuf = (char *)txbuffer;
	char *rbuf = (char *)rxbuffer;

	int flen = len;
	uint32_t *pWrBuf = NULL;
	uint32_t *pRdBuf = NULL;
	while (len > 0) {
		flen = len;
		if (len > priv->max_transfer_sz) {
			flen = priv->max_transfer_sz;
		}
		if (NULL != rbuf) {
			pRdBuf = (uint32_t *)(rbuf + received);
		}
		if (NULL != tbuf) {
			pWrBuf = (uint32_t *)(tbuf + sent);
		}
		spi_new_trans(dev, pWrBuf, pRdBuf, flen * 8, 0, 0);

		/* SPI wait transfer done */
#ifndef CONFIG_SPI_POLLWAIT
		sem_wait(&priv->xfrsem);
#else
		int count = 0;
		while (!priv->hw->slave.trans_done) {
			usleep(TIMEOUT_SLEEP);
			if (++count > TIMEOUT_COUNT) {
				spiinfo("[SPI] polling trans timeout!\n");
				return;
			}
		}
#endif
		spi_post_trans(dev, pRdBuf, flen);

		sent += flen;
		received += flen;
		len -= flen;
	}
}

#ifndef CONFIG_SPI_EXCHANGE
/****************************************************************************
 * Name: spi_sndblock
 *
 * Description:
 *   Send a block of data on SPI. Support only byte transfers
 *
 ****************************************************************************/
static void spi_sndblock(struct spi_dev_s *dev, const void *txbuffer, size_t nwords)
{
	spi_exchange(dev, txbuffer, NULL, nwords);
}

/****************************************************************************
 * Name: spi_recvblock
 *
 * Description:
 *   Revice a block of data from SPI. Support only byte transfers.
 *
 ****************************************************************************/
static void spi_recvblock(struct spi_dev_s *dev, void *rxbuffer, size_t nwords)
{
	spi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/*
 * spi_irq_handler
 *
 */
static int spi_irq_handler(int irq, FAR void *context, FAR void *arg)
{
	struct esp32_spidev_s *priv = (struct esp32_spidev_s *)arg;
	spi_reg_slave_t slave;
	slave.val = priv->hw->slave.val;

	if (slave.trans_done) {
#ifndef CONFIG_SPI_POLLWAIT
		sem_post(&priv->xfrsem);
#endif
	}
	/*Disable unneeded ints */
	esp32_spi_Clear_ints(priv);

	return OK;
}

static void esp32_spi_pins_initialize(struct esp32_spidev_s *priv)
{
	bool use_iomux = true;
	//check pins existence and if the selected pins correspond to the iomux pins of the peripheral

	esp32_spi_pins_sig_t *p_pin_sig = NULL;
	int func = 0;
	switch (priv->port) {
	case HSPI_PORT: {
		p_pin_sig = (esp32_spi_pins_sig_t *)(&s_spi2_pin_sig);
#if defined(CONFIG_GPIO_SPI2_CS_PIN) || defined(CONFIG_GPIO_SPI2_CLK_PIN) || defined(CONFIG_GPIO_SPI2_MISO_PIN) || defined(CONFIG_GPIO_SPI2_MOSI_PIN)
		use_iomux = false;
#endif

	}
	break;
	case VSPI_PORT: {
		p_pin_sig = (esp32_spi_pins_sig_t *)(&s_spi3_pin_sig);
#if defined(CONFIG_GPIO_SPI3_CS_PIN) || defined(CONFIG_GPIO_SPI3_CLK_PIN) || defined(CONFIG_GPIO_SPI3_MISO_PIN) || defined(CONFIG_GPIO_SPI3_MOSI_PIN)
		use_iomux = false;
#endif

	}
	break;
	default:
		return;
	}

	if (use_iomux) {
		func = SPI_PIN_FUNC_SPI;
	} else {
		func = SPI_PIN_FUNC_GPIO;
	}

	if (priv->gpio_mosi >= 0) {
		gpio_matrix_in(priv->gpio_mosi, p_pin_sig->spimosi_in, 0);
		gpio_matrix_out(priv->gpio_mosi, p_pin_sig->spimosi_out, 0, 0);
		esp32_configgpio(priv->gpio_mosi, func);
	}

	if (priv->gpio_miso >= 0) {
		gpio_matrix_in(priv->gpio_miso, p_pin_sig->spimiso_in, 0);
		func = use_iomux ? (INPUT | FUNCTION_1) : (INPUT | FUNCTION_2);
		if (priv->work_mode & SPICOMMON_BUSFLAG_DUAL) {
			gpio_matrix_out(priv->gpio_miso, p_pin_sig->spimiso_out, 0, 0);
			func |= OUTPUT;
		}
		esp32_configgpio(priv->gpio_miso, func);
	}

	if (priv->gpio_quadwp >= 0 && priv->gpio_quadwp < GPIO_PIN_COUNT) {
		gpio_matrix_in(priv->gpio_quadwp, p_pin_sig->spiwp_in, 0);
		gpio_matrix_out(priv->gpio_quadwp, p_pin_sig->spiwp_out, 0, 0);
		esp32_configgpio(priv->gpio_quadwp, func);
	}

	if (priv->gpio_quadhd >= 0 && priv->gpio_quadhd < GPIO_PIN_COUNT) {
		gpio_matrix_in(priv->gpio_quadhd, p_pin_sig->spihd_in, 0);
		gpio_matrix_out(priv->gpio_quadhd, p_pin_sig->spihd_out, 0, 0);
		esp32_configgpio(priv->gpio_quadhd, func);
	}

	if (priv->gpio_clk >= 0) {
		gpio_matrix_in(priv->gpio_clk, p_pin_sig->spiclk_in, 0);
		gpio_matrix_out(priv->gpio_clk, p_pin_sig->spiclk_in, 0, 0);
		esp32_configgpio(priv->gpio_clk, func);
	}
	if (priv->work_mode & SPICOMMON_BUSFLAG_MASTER) {
		for (int i = 0; i < MAX_CS_NUM; i++) {
			if (priv->gpio_nss[i] >= 0 && priv->gpio_nss[i] < GPIO_PIN_COUNT) {
				if (i == 0) {
					gpio_matrix_in(priv->gpio_nss[i], p_pin_sig->spics_in, 0);
				}
				gpio_matrix_out(priv->gpio_nss[i], p_pin_sig->spics_out[i], 0, 0);
				esp32_configgpio(priv->gpio_nss[i], func);
			}
		}
	} else {
		if (priv->gpio_nss[0] >= 0 && priv->gpio_nss[0] < GPIO_PIN_COUNT) {
			gpio_matrix_in(priv->gpio_nss[0], p_pin_sig->spics_in, 0);
			gpio_matrix_out(priv->gpio_nss[0], p_pin_sig->spics_in, 0, 0);
			esp32_configgpio(priv->gpio_nss[0], func);
		}
	}
}

void spicommon_cs_initialize(struct esp32_spidev_s *priv, int cs_io_num, int cs_num, int force_gpio_matrix)
{
	int spics0_iomux_pin = -1;
	esp32_spi_pins_sig_t *p_pin_sig = NULL;
	switch (priv->port) {
	case HSPI_PORT: {
		p_pin_sig = (esp32_spi_pins_sig_t *)&s_spi2_pin_sig;
		spics0_iomux_pin = HSPI_IOMUX_PIN_NUM_CS;
	}
	break;
	case VSPI_PORT: {
		p_pin_sig = (esp32_spi_pins_sig_t *)&s_spi3_pin_sig;
		spics0_iomux_pin = VSPI_IOMUX_PIN_NUM_CS;
	}
	break;
	default: {
		return;
	}
	}
	if (!force_gpio_matrix && cs_io_num == spics0_iomux_pin && cs_num == 0) {
		//The cs0s for all SPI peripherals map to pin mux source 1, so we use that instead of a define.
		gpio_matrix_in(cs_io_num, p_pin_sig->spics_in, 0);
		gpio_matrix_out(cs_io_num, SPI_PIN_FUNC_SPI, false, 0);
	} else {
		gpio_matrix_out(priv->gpio_nss[cs_num], p_pin_sig->spics_out[cs_num], 0, 0);
		if (cs_num == 0) {
			gpio_matrix_in(priv->gpio_nss[cs_num], p_pin_sig->spics_out[cs_num], 0);
		}
		esp32_configgpio(priv->gpio_nss[cs_num], SPI_PIN_FUNC_GPIO);
	}
}

static int esp32_spi_isr_initialize(struct esp32_spidev_s *priv)
{
	/*register isr */
	priv->cpu_int = esp32_alloc_levelint(priv->intrupt_level);
	if (priv->cpu_int < 0) {
		/* Failed to allocate a CPU interrupt of this type */
		spiinfo("[SPI] esp32_alloc_levelint fails:%d \n", priv->cpu_int);
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
	esp32_attach_peripheral(cpu, priv->periph_isr, priv->cpu_int);

	/* Attach and enable the IRQ */
	int ret = irq_attach(priv->isr_num, priv->isr_handler, priv);
	if (ret == OK) {
		/* Enable the CPU interrupt */
		up_enable_irq(priv->cpu_int);
	} else {
		spiinfo("[SPI] irq_attach fails:%d \n", ret);
	}

	return ret;
}

static int spi_freq_for_pre_n(int fapb, int pre, int n)
{
	return (fapb / (pre * n));
}

int spi_get_freq_limit(bool gpio_is_used, int input_delay_ns)
{
	const int apbclk_kHz = APB_CLK_FREQ / 1000;
	const int gpio_delay_ns = gpio_is_used ? 25 : 0;

	//calculate how many apb clocks a period has, the 1 is to compensate in case ``input_delay_ns`` is rounded off.
	int apb_period_n = (1 + input_delay_ns + gpio_delay_ns) * apbclk_kHz / 1000 / 1000;
	int limit = APB_CLK_FREQ / (apb_period_n + 1);

	spiinfo("[SPI] gpio:%d, dely_ns:%d, apb_period=%d: limit=%d ...\n", gpio_is_used, input_delay_ns, apb_period_n, limit);
	return limit;
}

static int esp32_spi_cal_clock(int fapb, int hz, int duty_cycle, uint32_t *reg_o)
{
	spi_clock_reg_t reg;
	int eff_clk;

	//In hw, n, h and l are 1-64, pre is 1-8K. Value written to register is one lower than used value.
	if (hz > ((fapb / 4) * 3)) {
		//Using Fapb directly will give us the best result here.
		reg.clkcnt_l = 0;
		reg.clkcnt_h = 0;
		reg.clkcnt_n = 0;
		reg.clkdiv_pre = 0;
		reg.clk_equ_sysclk = 1;
		eff_clk = fapb;
	} else {
		//For best duty cycle resolution, we want n to be as close to 32 as possible, but
		//we also need a pre/n combo that gets us as close as possible to the intended freq.
		//To do this, we bruteforce n and calculate the best pre to go along with that.
		//If there's a choice between pre/n combos that give the same result, use the one
		//with the higher n.
		int pre, n, h, l;
		int bestn = -1;
		int bestpre = -1;
		int besterr = 0;
		int errval;
		/* Start at 2: we need to be able to set h/l so we have at least one high and one low pulse.
		 * Effectively, this does pre=round((fapb/n)/hz).
		 */
		for (n = 2; n <= 64; n++) {
			pre = ((fapb / n) + (hz / 2)) / hz;
			if (pre <= 0) {
				pre = 1;
			}
			if (pre > 8192) {
				pre = 8192;
			}
			errval = abs(spi_freq_for_pre_n(fapb, pre, n) - hz);
			if (bestn == -1 || errval <= besterr) {
				besterr = errval;
				bestn = n;
				bestpre = pre;
			}
		}

		n = bestn;
		pre = bestpre;
		l = n;
		/* This effectively does round((duty_cycle*n)/256) */
		h = (duty_cycle * n + 127) / 256;
		if (h <= 0) {
			h = 1;
		}

		reg.clk_equ_sysclk = 0;
		reg.clkcnt_n = n - 1;
		reg.clkdiv_pre = pre - 1;
		reg.clkcnt_h = h - 1;
		reg.clkcnt_l = l - 1;
		eff_clk = spi_freq_for_pre_n(fapb, pre, n);
		spiinfo("[SPI] clk=%d: n=%d, pre=%d, h=%d,l=%d, ...\n", eff_clk, n, pre, h, l);
	}

	if (reg_o != NULL) {
		*reg_o = reg.val;
	}

	return eff_clk;
}

/*
* Set the spi clock according to pre-calculated register value.
*/
static inline void esp32_spi_set_clock(struct esp32_spidev_s *priv, spi_clock_reg_t reg)
{
	priv->hw->clock.val = reg.val;
}

static void esp32_spi_get_timing(bool gpio_is_used, int input_delay_ns, int eff_clk, int *dummy_o, int *cycles_remain_o)
{
	const int apbclk_kHz = APB_CLK_FREQ / 1000;
	const int apbclk_n = APB_CLK_FREQ / eff_clk;
	const int gpio_delay_ns = gpio_is_used ? 25 : 0;

	/*calculate how many apb clocks a period has, the 1 is to compensate in case ``input_delay_ns`` is rounded off. */
	int apb_period_n = (1 + input_delay_ns + gpio_delay_ns) * apbclk_kHz / 1000 / 1000;
	int dummy_required = apb_period_n / apbclk_n;
	//spiinfo("[SPI] timing gpio:%d, i_delns:%d, g_delns:%d, clk_n:%d, prd_n:%d, dreq:%d!\n", gpio_is_used, input_delay_ns, gpio_delay_ns, apbclk_n, apb_period_n, dummy_required);

	int miso_delay = 0;
	if (dummy_required > 0) {
		/*due to the clock delay between master and slave, there's a range in which data is random
		 *give MISO a delay if needed to make sure we sample at the time MISO is stable
		 */
		miso_delay = (dummy_required + 1) * apbclk_n - apb_period_n - 1;
	} else {
		/* if the dummy is not required, maybe we should also delay half a SPI clock if the data comes too early
		 */
		if (apb_period_n * 4 <= apbclk_n) {
			miso_delay = -1;
		}
	}
	if (dummy_o != NULL) {
		*dummy_o = dummy_required;
	}
	if (cycles_remain_o != NULL) {
		*cycles_remain_o = miso_delay;
	}
	spiinfo("[SPI] eff: %d, lim: %dk(/%d), %d, %d\n", eff_clk / 1000, apbclk_kHz / (apb_period_n + 1), apb_period_n, dummy_required, miso_delay);
}

void spi_config_device(struct esp32_spidev_s *priv, const spi_device_interface_config_t *dev_config)
{
	/* Configure clock settings */
	esp32_spi_set_clock(priv, priv->clk_cfg.reg);

	/* Configure bit order */
	priv->hw->ctrl.rd_bit_order = (dev_config->flags & SPI_DEVICE_RXBIT_LSBFIRST) ? 1 : 0;
	priv->hw->ctrl.wr_bit_order = (dev_config->flags & SPI_DEVICE_TXBIT_LSBFIRST) ? 1 : 0;

	/* Configure polarity */
	if (priv->comm_mode == SPIDEV_MODE0) {
		priv->hw->pin.ck_idle_edge = 0;
		priv->hw->user.ck_out_edge = 0;
	} else if (priv->comm_mode == SPIDEV_MODE1) {
		priv->hw->pin.ck_idle_edge = 0;
		priv->hw->user.ck_out_edge = 1;
	} else if (priv->comm_mode == SPIDEV_MODE2) {
		priv->hw->pin.ck_idle_edge = 1;
		priv->hw->user.ck_out_edge = 1;
	} else if (priv->comm_mode == SPIDEV_MODE3) {
		priv->hw->pin.ck_idle_edge = 1;
		priv->hw->user.ck_out_edge = 0;
	}
	/* Configure misc stuff */
	priv->hw->user.doutdin = (dev_config->flags & SPI_DEVICE_HALFDUPLEX) ? 0 : 1;
	priv->hw->user.sio = (dev_config->flags & SPI_DEVICE_3WIRE) ? 1 : 0;
	/* Configure CS pin and timing */
	priv->hw->ctrl2.setup_time = dev_config->cs_ena_pretrans - 1;
	priv->hw->user.cs_setup = dev_config->cs_ena_pretrans ? 1 : 0;
	/* set hold_time to 0 will not actually append delay to CS
	 *set it to 1 since we do need at least one clock of hold time in most cases
	 */
	priv->hw->ctrl2.hold_time = dev_config->cs_ena_posttrans;
	if (priv->hw->ctrl2.hold_time == 0) {
		priv->hw->ctrl2.hold_time = 1;
	}
	priv->hw->user.cs_hold = 1;

	priv->hw->pin.cs0_dis = (priv->cur_cs == 0) ? 0 : 1;
	priv->hw->pin.cs1_dis = (priv->cur_cs == 1) ? 0 : 1;
	priv->hw->pin.cs2_dis = (priv->cur_cs == 2) ? 0 : 1;
}

/*
 * spi_select_device:
 * Input param:
 *      priv
 *      dev_id: cs index {0~2};
 *      dev_config:
 * return value:
 *      ok.
 */
int spi_select_device(struct esp32_spidev_s *priv, int dev_id, const spi_device_interface_config_t *dev_config)
{
	int freecs;
	int eff_clk;
	int dummy_required;
	int miso_delay;

	const int duty_cycle = 128;
	spi_clock_reg_t clk_reg = { 0 };

	/* GPIO matrix can only change data at 80Mhz rate, which only allows 40MHz SPI clock.
	 * Speed >=40MHz over GPIO matrix needs a dummy cycle, but these don't work for full-duplex connections.
	 */
	eff_clk = esp32_spi_cal_clock(APB_CLK_FREQ, priv->frequency, duty_cycle, (uint32_t *)&clk_reg);
	esp32_spi_get_timing(!(priv->work_mode & SPICOMMON_BUSFLAG_NATIVE_PINS), dev_config->input_delay_ns, eff_clk, &dummy_required, &miso_delay);
	spiinfo("[SPI] eff_clk=%d, dummy_required=%d, miso_delay=%d...\n", eff_clk, dummy_required, miso_delay);

	priv->clk_cfg = (clock_config_t) {
		.eff_clk = eff_clk, .dummy_num = dummy_required, .reg = clk_reg, .miso_delay = miso_delay,
	};

	/* Set CS pin, CS options */
	freecs = dev_id;
	if (dev_config->flags & SPI_DEVICE_CLK_AS_CS) {
		priv->hw->pin.master_ck_sel |= (1 << freecs);
	} else {
		priv->hw->pin.master_ck_sel &= (1 << freecs);
	}
	if (dev_config->flags & SPI_DEVICE_POSITIVE_CS) {
		priv->hw->pin.master_cs_pol |= (1 << freecs);
	} else {
		priv->hw->pin.master_cs_pol &= (1 << freecs);
	}
	priv->hw->ctrl2.mosi_delay_mode = 0;
	priv->hw->ctrl2.mosi_delay_num = 0;

	/*Record the device just configured to save time for next time */
	priv->prev_cs = priv->cur_cs;
	priv->cur_cs = dev_id;

	spi_config_device(priv, dev_config);

	return dev_id;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_spiinitialize
 *
 * Description:
 *   Initialize the selected SPI port
 *
 ****************************************************************************/

struct spi_dev_s *up_spiinitialize(int port)
{
	FAR struct esp32_spidev_s *priv = NULL;

	switch (port) {

	case HSPI_PORT: {
		priv = &g_spi2dev;
		periph_module_enable(PERIPH_HSPI_MODULE);
	}
	break;

	case VSPI_PORT: {
		priv = &g_spi3dev;
		periph_module_enable(PERIPH_VSPI_MODULE);
	}
	break;

	default:
		spiinfo("[SPI] unsupport spi port %d!\n", port);
		return NULL;
	}

	if (0 == priv->initiallized) {
		priv->port = port;

		esp32_spi_pins_initialize(priv);

		priv->max_transfer_sz = SPI_MAX_NODMA_FRAME_LEN;
		if (0 < priv->dma_chan) {
			if (OK == spicommon_dma_chan_claim(priv->dma_chan)) {
#if !defined(CONFIG_SPI_DMA_MAX_DATALEN) || (CONFIG_SPI_DMA_MAX_DATALEN < SPI_MAX_DMA_FRAME_LEN || CONFIG_SPI_DMA_MAX_DATALEN > SPI_MAX_DATA_LEN)
				priv->max_transfer_sz = SPI_MAX_DMA_FRAME_LEN;
#else
				priv->max_transfer_sz = CONFIG_SPI_DMA_MAX_DATALEN;
#endif

				DPORT_SET_PERI_REG_BITS(DPORT_SPI_DMA_CHAN_SEL_REG, 3, priv->dma_chan, ((priv->port - 1) * 2));

				spiinfo("[SPI] %d DMA channel %d; max: %d; reg:%08x\n", priv->port, priv->dma_chan, priv->max_transfer_sz, DPORT_READ_PERI_REG(DPORT_SPI_DMA_CHAN_SEL_REG));
			}
		}
#ifndef CONFIG_SPI_POLLWAIT
		sem_init(&priv->xfrsem, 0, 0);
		sem_setprotocol(&priv->xfrsem, SEM_PRIO_NONE);
#endif

#ifndef CONFIG_SPI_OWNBUS
		sem_init(&priv->exclsem, 0, 1);
#endif

		priv->isr_handler = &spi_irq_handler;
		esp32_spi_isr_initialize(priv);

		//Reset DMA
		priv->hw->dma_conf.val |= SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST;
		priv->hw->dma_out_link.start = 0;
		priv->hw->dma_in_link.start = 0;
		priv->hw->dma_conf.val &= ~(SPI_OUT_RST | SPI_IN_RST | SPI_AHBM_RST | SPI_AHBM_FIFO_RST);

		//Reset timing
		priv->hw->ctrl2.val = 0;

		//master use all 64 bytes of the buffer
		priv->hw->user.usr_miso_highpart = 0;
		priv->hw->user.usr_mosi_highpart = 0;

		//Disable unneeded ints
		esp32_spi_Disable_ints(priv);
		esp32_spi_Clear_ints(priv);

#ifdef CONFIG_ESP32_SPI_CS
		spi_select_device(priv, CONFIG_ESP32_SPI_CS, &def_dev_config);
#else
		spi_select_device(priv, 0, &def_dev_config);
#endif

		/* Initialize a semaphore for spi dma */
		sem_init((sem_t *)&g_spi_dma.dma_lock, 0, 1);
		priv->initiallized = true;
	}

	return (struct spi_dev_s *)priv;
}
