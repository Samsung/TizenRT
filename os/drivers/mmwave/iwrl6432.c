/*
 * Copyright (c) 2025 Syntiant Corp.  All rights reserved.
 * Contact at http://www.syntiant.com
 *
 * This software is available to you under a choice of one of two licenses.
 * You may choose to be licensed under the terms of the GNU General Public
 * License (GPL) Version 2, available from the file LICENSE in the main
 * directory of this source tree, or the OpenIB.org BSD license below.  Any
 * code involving Linux software will require selection of the GNU General
 * Public License (GPL) Version 2.
 *
 * OPENIB.ORG BSD LICENSE
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ** SDK: v112.3.7-Samsung **
 */
#include <sys/types.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <poll.h>

#include <tinyara/arch.h>
#include <tinyara/spi/spi.h>
#include <tinyara/clock.h>
#include "iwrl6432.h"
#include <tinyara/mmwave/mmwave_dev.h>
#include <tinyara/mmwave/mmwave.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Verify that all configuration requirements have been met */

#ifndef CONFIG_MMWAVE_SENSOR_SPIMODE
#define CONFIG_MMWAVE_SENSOR_SPIMODE SPIDEV_MODE0
#endif

/* SPI frequency */

#define CONFIG_MMWAVE_SENSOR_FREQUENCY 12000000
#define CONFIG_MMWAVE_SENSOR_SPI_BITS 8
#define MMWAVE_SPI_CS 1
#define PAYLOAD_HEADER 0x32
#define PAYLOAD_TAIL 0x34
#define PAYLOAD_SOF 0xFF
#define PARAMETER_TX_SIZE 1032
#define ACK_TX_SIZE 16
#define CUBE_6SEGMENT_PACKET_SIZE 16408
#define CUBE_8SEGMENT_PACKET_SIZE 12312
#define CLOUD_TX_SIZE 1320
#define NO_OF_SEGMENTS 6

/****************************************************************************
 * Private Function Protototypes
 ****************************************************************************/

/* Misc. Helpers */
static void mmwave_lock(FAR struct spi_dev_s *dev);
static inline void mmwave_unlock(FAR struct spi_dev_s *dev);
static inline void mmwave_sendcmd(FAR struct iwrl6432_s *dev, uint8_t cmd);
static inline void mmwave_senddata(FAR struct iwrl6432_s *dev, uint8_t *txbuf, int size);
static inline void mmwave_getdata(FAR struct iwrl6432_s *dev, uint8_t *rxbuf, int size);
static inline void make_ack(uint8_t request_number, uint8_t *ack_tx_buf);
static inline uint8_t checksum_cal(uint8_t *input_buf, int start_index, int end_index);
static inline int mmwave_init(FAR struct iwrl6432_s *dev);
static int mmwave_get_cubes(int argc, char **argv);
static void mmwave_pause(FAR struct mmwave_dev_s *dev);
static void mmwave_resume(FAR struct mmwave_dev_s *dev);
static void mmwave_copy_buffer(FAR struct mmwave_dev_s *dev, uint8_t **buf);
uint32_t cube_data_length;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static const struct mmwave_ops_s g_iwrl6432_ops = {
	mmwave_pause,
	mmwave_resume,
	mmwave_copy_buffer};
/************************************************************************************
 * Name: mmwave_lock
 ************************************************************************************/

static void mmwave_lock(FAR struct spi_dev_s *dev)
{
	/* On SPI busses where there are multiple devices, it will be necessary to
	 * lock SPI to have exclusive access to the busses for a sequence of
	 * transfers.  The bus should be locked before the chip is selected.
	 *
	 * This is a blocking call and will not return until we have exclusiv access to
	 * the SPI buss.  We will retain that exclusive access until the bus is unlocked.
	 */

	(void)SPI_LOCK(dev, true);

	/* After locking the SPI bus, the we also need call the setfrequency, setbits, and
	 * setmode methods to make sure that the SPI is properly configured for the device.
	 * If the SPI buss is being shared, then it may have been left in an incompatible
	 * state.
	 */
	SPI_SETMODE(dev, CONFIG_MMWAVE_SENSOR_SPIMODE);
	SPI_SETBITS(dev, CONFIG_MMWAVE_SENSOR_SPI_BITS);
	SPI_SETFREQUENCY(dev, CONFIG_MMWAVE_SENSOR_FREQUENCY);
}

/************************************************************************************
 * Name: mmwave_unlock
 ************************************************************************************/

static inline void mmwave_unlock(FAR struct spi_dev_s *dev)
{
	(void)SPI_LOCK(dev, false);
}

/****************************************************************************
 * Name: mmwave_senddata
 *
 * Description:
 *   Send data to the sensor.
 * data - tx buffer
 * size - total bytes of words
 *        word size(8 bits)
 ****************************************************************************/

static inline void mmwave_senddata(FAR struct iwrl6432_s *dev, uint8_t *txbuf, int size)
{
	SPI_SNDBLOCK(dev->spi, txbuf, size);
}

/****************************************************************************
 * Name: mmwave_getdata
 *
 * Description:
 *   Get data from sensor over SPI BUS.
 *
 * buf - rx buffer
 * size - total bytes of words
 *        word size(8 bits)
 ****************************************************************************/

static inline void mmwave_getdata(FAR struct iwrl6432_s *dev, uint8_t *rxbuf, int size)
{

	SPI_RECVBLOCK(dev->spi, rxbuf, size);
}

/****************************************************************************
 * Name: mmwave_pause
 *
 ****************************************************************************/

static void mmwave_pause(FAR struct mmwave_dev_s *dev)
{
	struct iwrl6432_s *priv = dev->priv;
	SPI_PAUSE(priv->spi);
}

/****************************************************************************
 * Name: mmwave_resume
 *
 ****************************************************************************/

static void mmwave_resume(FAR struct mmwave_dev_s *dev)
{
	struct iwrl6432_s *priv = dev->priv;
	SPI_RESUME(priv->spi);
}

/****************************************************************************
 * Name: mmwave_copy_buffer
 *
 ****************************************************************************/

static void mmwave_copy_buffer(FAR struct mmwave_dev_s *dev, uint8_t **buf)
{
	struct iwrl6432_s *priv = dev->priv;
	for (int i = 0; i < NO_OF_SEGMENTS; i++) {
		memcpy(buf[i], priv->data_buffer[i], cube_data_length);
	}
}

/****************************************************************************
 * Name:  checksum_cal
 *
 ****************************************************************************/

static uint8_t checksum_cal(uint8_t *input_buf, int start_index, int end_index)
{
	uint32_t sum = 0;
	for (int i = start_index; i < end_index; i++) {
		sum += input_buf[i];
	}
	return (uint8_t)(sum & 0xFF);
}

/****************************************************************************
 * Name:  make_ack
 *
 ****************************************************************************/

static void make_ack(uint8_t request_number, uint8_t *ack_tx_buf)
{
	// Initialize packet header
	ack_tx_buf[0] = PAYLOAD_HEADER;
	ack_tx_buf[1] = PAYLOAD_SOF;

	// Set packet length (ACK_TX_SIZE - 7)
	uint32_t packet_length = ACK_TX_SIZE - 7;
	ack_tx_buf[2] = (uint8_t)(packet_length & 0xFF);
	ack_tx_buf[3] = (uint8_t)((packet_length >> 8) & 0xFF);
	ack_tx_buf[4] = (uint8_t)((packet_length >> 16) & 0xFF);
	ack_tx_buf[5] = (uint8_t)((packet_length >> 24) & 0xFF);

	// Fixed values in the packet
	ack_tx_buf[6] = 0x11;
	ack_tx_buf[7] = 0x24;
	ack_tx_buf[8] = 0x02;
	ack_tx_buf[9] = 0x00;
	ack_tx_buf[10] = 0x00;
	ack_tx_buf[11] = 0x00;
	ack_tx_buf[12] = 0x00;

	// Insert request number
	ack_tx_buf[13] = request_number;

	// Calculate checksum from index 1 to ACK_TX_SIZE-3 (excludes last 3 bytes)
	uint8_t checksum = checksum_cal(ack_tx_buf, 1, ACK_TX_SIZE - 2);
	ack_tx_buf[14] = checksum;

	// Set packet tail
	ack_tx_buf[15] = PAYLOAD_TAIL;
}

/****************************************************************************
 * Name:  mmwave_get_cubes
 *
 ****************************************************************************/

static int mmwave_get_cubes(int argc, char **argv)
{
	struct iwrl6432_s *dev;
	DEBUGASSERT(argc == 2);
	dev = (struct iwrl6432_s *)strtoul(argv[1], NULL, 16);

	uint8_t ack_tx_buf[ACK_TX_SIZE];
	int req_num = 2;
	uint8_t cube_segment_count = NO_OF_SEGMENTS;
	uint8_t cube_segment_index = 0;
	int frame_count = 0;
	int time_sum;
	struct timespec ts_start, ts_end;
	int cube_cnt;
	uint32_t length;
	uint8_t checksum;
	uint8_t cksm;
	uint8_t *payload;
	uint32_t payload_length;
	uint8_t *cube_data;
	int elapsed_time;
	int rx_bytes;
	int cube_packet_size;

	if (cube_segment_count == 6) {
		cube_packet_size = CUBE_6SEGMENT_PACKET_SIZE;
	} else {
		cube_packet_size = CUBE_8SEGMENT_PACKET_SIZE;
	}

	uint8_t *rxbuf = (uint8_t *)kmm_malloc(cube_packet_size);

	rx_bytes = 0;
	cube_segment_index = 0;
	time_sum = 0;
	cube_cnt = 0;
	while (1) {

		if (SPI_GETSTATUS(dev->spi)) {
			cube_segment_index = 0;
			time_sum = 0;
			cube_cnt = 0;
			req_num = 2;
		}

		if (req_num == 1) {
			rx_bytes = CLOUD_TX_SIZE;
		} else {
			rx_bytes = cube_packet_size;
		}
		while (true) {
			if (dev->config->busy_status() == 1) {
				// lldbg("Writing Ack for cube no: %d----------------------------------------------------------\n", req_num - 1);
				make_ack(req_num, ack_tx_buf);
				mmwave_lock(dev->spi);
				SPI_SELECT(dev->spi, MMWAVE_SPI_CS, true);
				mmwave_senddata(dev, ack_tx_buf, ACK_TX_SIZE);
				SPI_SELECT(dev->spi, MMWAVE_SPI_CS, false);
				mmwave_unlock(dev->spi);
				break;
			}
		}
		while (true) {
			if (dev->config->busy_status() == 0) {
				// lldbg("Reading cube no: %d---------------------------------------\n", cube_segment_index + 1);
				clock_gettime(CLOCK_MONOTONIC, &ts_start);
				mmwave_lock(dev->spi);
				SPI_SELECT(dev->spi, MMWAVE_SPI_CS, true);
				mmwave_getdata(dev, rxbuf, rx_bytes);
				SPI_SELECT(dev->spi, MMWAVE_SPI_CS, false);
				mmwave_unlock(dev->spi);
				clock_gettime(CLOCK_MONOTONIC, &ts_end);
				break;
			}
		}
		elapsed_time = (int)(ts_end.tv_sec - ts_start.tv_sec) * 1000 + (int)(ts_end.tv_nsec - ts_start.tv_nsec) / 1000000;
		time_sum += elapsed_time;
		cube_cnt++;
		req_num++;
		if (req_num > (cube_segment_count + 1)) {
			req_num = 2;
		}

		length = (rxbuf[2]) | (rxbuf[3] << 8) | (rxbuf[4] << 16) | (rxbuf[5] << 24);
		checksum = rxbuf[length + 5];
		cksm = checksum_cal(rxbuf, 1, length + 5);
		payload = rxbuf + 7;
		payload_length = length - 2;
		cube_data = payload + 15;
		cube_data_length = payload_length - 15;
		if (!dev->data_buffer[cube_segment_index]) {
			dev->data_buffer[cube_segment_index] = (uint8_t *)kmm_malloc(cube_data_length);
			if (!dev->data_buffer[cube_segment_index]) {
				lldbg("Failed to allocate memory for cube data\n");
			}
		}
		memcpy(dev->data_buffer[cube_segment_index], cube_data, cube_data_length);

		if (checksum == cksm) {
			cube_segment_index++;
		} else {
			lldbg("############################ Checksum Error ###########################\n");
		}

		if (cube_segment_index >= (cube_segment_count)) {
			lldbg("frame %d tx done  Average time elapsed %d ms\n", frame_count, time_sum / cube_cnt);
			frame_count += 1;
			if (frame_count > 100000000) {
				frame_count = 0;
			}

			cube_segment_index = 0;
			time_sum = 0;
			cube_cnt = 0;

			mmwave_pollnotify(dev->upper, POLLIN);
		}
	}

	return OK;
}

/****************************************************************************
 * Name:  mmwave_init
 *
 ****************************************************************************/

static int mmwave_init(FAR struct iwrl6432_s *dev)
{
	dev->config->reset();
	const char *commandParams = {"sensorStop 0\nchannelCfg 7 3 0\nchirpComnCfg 15 5 0 128 1 34 3\nchirpTimingCfg 6 26 1 47.95 60\nframeCfg 2 0 600 64 200 0\nantGeometryCfg 0 0 1 1 0 2 0 1 1 2 0 3 2.418 2.418\nguiMonitor 0 0 0 0 0 0 0 0 0 0 0\nsigProcChainCfg 32 2 1 0 64 1 0 0.3\ncfarCfg 2 8 4 3 0 7.0 0 0.5 0 1 1 1\naoaFovCfg -70 70 -40 40\nrangeSelCfg 1.2 10.0\nclutterRemoval 0\ncompRangeBiasAndRxChanPhase 0.0 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000\nadcDataSource 0 adc_data_0001_CtestAdc6Ant.bin\nadcLogging 2\nlowPowerCfg 0\nfactoryCalibCfg 1 0 36 3 0x1ff000\nsensorStart 0 0 0 0\n"};
	uint8_t spiData[PARAMETER_TX_SIZE];
	uint8_t param_ack[41];
	int parameter_packet_length = 1025;
	for (int i = 0; i < PARAMETER_TX_SIZE; i++) {
		spiData[i] = 0;
	}
	spiData[0] = PAYLOAD_HEADER;
	spiData[1] = PAYLOAD_SOF;
	spiData[2] = parameter_packet_length & 0xFF;
	spiData[3] = (parameter_packet_length >> 8) & 0xFF;
	spiData[4] = (parameter_packet_length >> 16) & 0xFF;
	spiData[5] = (parameter_packet_length >> 24) & 0xFF;
	spiData[6] = 0;
	memcpy(spiData + 7, commandParams, strlen(commandParams));
	spiData[PARAMETER_TX_SIZE - 2] = checksum_cal(spiData, 1, PARAMETER_TX_SIZE - 2);
	spiData[PARAMETER_TX_SIZE - 1] = PAYLOAD_TAIL;

	mmwave_lock(dev->spi);
	SPI_SELECT(dev->spi, MMWAVE_SPI_CS, true);
	mmwave_senddata(dev, spiData, PARAMETER_TX_SIZE);
	SPI_SELECT(dev->spi, MMWAVE_SPI_CS, false);
	mmwave_unlock(dev->spi);
	DelayMs(50);
	while (true) {
		if (dev->config->busy_status() == 0) {
			mmwave_lock(dev->spi);
			SPI_SELECT(dev->spi, MMWAVE_SPI_CS, true);
			mmwave_getdata(dev, param_ack, 41);
			SPI_SELECT(dev->spi, MMWAVE_SPI_CS, false);
			mmwave_unlock(dev->spi);
			break;
		}
	}

	char *ack_str = (char *)kmm_malloc(33);
	if (!ack_str) {
		lldbg("Failed to allocate memory for ack string\n");
	}
	memcpy(ack_str, param_ack + 7, 32);
	ack_str[32] = '\0';
	printf("****************************************%s\n", ack_str);
	kmm_free(ack_str);

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name:  mmwave_initialize
 *
 ****************************************************************************/

FAR int mmwave_initialize(FAR struct spi_dev_s *spi, struct mmwave_config_s *config)
{
	int ret;
	struct mmwave_dev_s *upper;
	struct iwrl6432_s *dev;
	char *parm[2];
	char parm_buf[9];
	dev = (struct iwrl6432_s *)kmm_zalloc(sizeof(struct iwrl6432_s));
	if (!dev) {
		lldbg("Fail to alloc mmwave_s\n");
		return ERROR;
	}
	/* Initialize the driver data structure */
	dev->config = config;
	config->upper = dev;
	dev->spi = spi;
	dev->data_buffer = (uint8_t **)kmm_malloc(NO_OF_SEGMENTS * sizeof(uint8_t *));
	for (int i = 0; i < NO_OF_SEGMENTS; i++) {
		dev->data_buffer[i] = NULL;
	}

	upper = (struct mmwave_dev_s *)kmm_zalloc(sizeof(struct mmwave_dev_s));
	if (!upper) {
		lldbg("Fail to alloc mmwave_dev_s\n");
		kmm_free(dev);
		return ERROR;
	}
	dev->upper = upper;
	upper->priv = dev;
	upper->ops = &g_iwrl6432_ops;
	mmwave_init(dev);
	itoa((int)dev, parm_buf, 16);
	parm[0] = parm_buf;
	parm[1] = NULL;
	pid_t pid = kernel_thread("SPI_BUS_MANAGE", 100, 4096, (main_t)mmwave_get_cubes, (FAR char *const *)parm);
	if (pid < 0) {
		lldbg("SPI_BUS_MANAGE thread creation failed\n");
	}

	return mmwave_register(upper);
}
