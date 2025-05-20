/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2022 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 *  process, and are protected by trade secret or copyright law.  Dissemination
 *  of this information or reproduction of this material is strictly forbidden
 *  unless prior written permission is obtained from Syntiant Corporation.
	** SDK: v104 **
*/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/kmalloc.h>
#include <tinyara/wqueue.h>
#include <tinyara/pm/pm.h>

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

#include <syntiant_ilib/syntiant_ndp.h>
#include <syntiant_ilib/syntiant_ndp120.h>
#include <syntiant_ilib/syntiant_ndp_error.h>
#include <syntiant_ilib/syntiant_ndp120_mailbox.h>
#include <syntiant-firmware/ndp120_firmware.h>
#include <syntiant_ilib/ndp120_spi_regs.h>

#include <tinyara/spi/spi.h>

#include "../../../audio/ndp120_voice.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define round_down(x, y) ((x) - ((x) % (y)))
#define STRING_LEN		256

/* currently the max size of tank is limited to ~580ms  */
#define AUDIO_BEFORE_MATCH_MS	(1500)

#define PDM_CLOCK_PDM_RATE 1536000

/* this is the higher bound of the keyword length, it will be rounded down to multiple of frame size */
#define KEYWORD_BUFFER_LEN      (SYNTIANT_NDP120_AUDIO_SAMPLE_RATE * SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD * AUDIO_BEFORE_MATCH_MS / 1000)
#define NDP120_SPI_FREQ_HIGH    12000000
#define NDP120_SPI_FREQ_INIT    1000000
#define FF_ID NDP120_DSP_DATA_FLOW_FUNCTION_FULL_FF_49
#define KEYWORD_NETWORK_ID 0

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_NDP120_AEC_SUPPORT
enum ndp120_state_e {
	IS_RECORDING = 0,
	NOT_RECORDING = 1,
};

enum ndp120_state_e g_ndp120_state;
#endif

static const unsigned int NDP_NOTIFICATION_ERRORS =
	SYNTIANT_NDP_NOTIFICATION_ERROR
	| SYNTIANT_NDP_NOTIFICATION_ALGO_ERROR
	| SYNTIANT_NDP_NOTIFICATION_DNN_MEM_ERROR
	| SYNTIANT_NDP_NOTIFICATION_DSP_MEM_ERROR
	| SYNTIANT_NDP_NOTIFICATION_SPI_READ_FAILURE;

static char s_label_data[NDP120_MCU_LABELS_MAX_LEN];
static char *s_labels[MAX_LABELS];
static unsigned int s_num_labels;

static struct work_s ndp120_work;

/* only used for debugging purposes */
static struct ndp120_dev_s *_ndp_debug_handle = NULL;

static int check_status(char *message, int s)
{
	if (s) {
		auddbg("%s failed: %s\n", message, syntiant_ndp_error_name(s));
	}
	return s;
}

static int check_io(char *message, int expected_len, int len)
{
	if (len < expected_len) {
		if (len < 0) {
			auddbg("%s failed\n", message);
		} else {
			auddbg("%s truncated\n", message);
		}
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	return SYNTIANT_NDP_ERROR_NONE;
}

static void timer_start(struct timespec *ts)
{
	int s;

	s = clock_gettime(CLOCK_REALTIME, ts);
}


static int timer_check(struct timespec *start, unsigned int timeout)
{
	int s, done;
	struct timespec ts;

	s = clock_gettime(CLOCK_REALTIME, &ts);

	done = start->tv_sec + timeout < ts.tv_sec
		|| (start->tv_sec + timeout == ts.tv_sec
			&& start->tv_nsec <= ts.tv_nsec);
	return done;
}

int tizenrt_io_spi_transfer(struct ndp120_dev_s *dev, int mcu, uint32_t addr, const void *out, void *in, int count)
{
	uint8_t spi_cmd[4];
	int s = SYNTIANT_NDP_ERROR_NONE;
	int read_delay = 4; /* check later */

	if (in && out) {
		s = SYNTIANT_NDP_ERROR_ARG;
		goto error;
	}

	SPI_LOCK(dev->spi, 1);

	/* reconfigure SPI parameters for NDP120 */
	SPI_SETMODE(dev->spi, dev->lower->spi_config.mode);
	SPI_SETFREQUENCY(dev->spi, dev->lower->spi_config.freq);
	SPI_SETBITS(dev->spi, dev->lower->spi_config.bpw);

	if (mcu) {
		if ((count & 0x3) != 0) {
			s = SYNTIANT_NDP_ERROR_ARG;
			goto error;
		}

		uint8_t mcu_addr[10];

		mcu_addr[0] = NDP120_SPI_MADDR(0);
		memcpy(&mcu_addr[1], &addr, sizeof(addr));

		/* for reads, need to restart a read command packet */
		if (in) {
			uint8_t mcu_read_tx = 0x80 | (NDP120_SPI_MDATA(0) - read_delay);
			uint32_t read_delay_data;

			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, mcu_addr, 5);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);

			/* restart a new transfer for read... */

			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, &mcu_read_tx, 1);
			SPI_RECVBLOCK(dev->spi, &read_delay_data, 4);
			SPI_RECVBLOCK(dev->spi, in, count);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);

		} else {
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
			SPI_SNDBLOCK(dev->spi, mcu_addr, 5);
			SPI_SNDBLOCK(dev->spi, out, count);
			SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);
		}
	} else {

		if (0xff < addr) {
				s = SYNTIANT_NDP_ERROR_ARG;
				goto error;
		}
		spi_cmd[0] = (in ? 0x80 : 0) | addr;

		SPI_SELECT(dev->spi, dev->lower->spi_config.cs, true);
		SPI_SNDBLOCK(dev->spi, spi_cmd, 1);

		if (in) {
				SPI_RECVBLOCK(dev->spi, in, count);
		} else {
				SPI_SNDBLOCK(dev->spi, out, count);
		}
		SPI_SELECT(dev->spi, dev->lower->spi_config.cs, false);
	}
error:
	SPI_LOCK(dev->spi, 0);
	return s;
}

static int transfer(void *d, int mcu, uint32_t addr, const void *out, void *in, unsigned int count)
{
	return tizenrt_io_spi_transfer((struct ndp120_dev_s *) d, mcu, addr, out, in, count);
}

static int mbwait(void *d)
{
	struct ndp120_dev_s *dev = (struct ndp120_dev_s *) d;
	struct timespec ts_start;
	int s = SYNTIANT_NDP_ERROR_NONE;
	uint32_t notifications;

	if (dev->ndp_interrupts_enabled) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec++;
		int err = pthread_cond_timedwait(&dev->ndp_cond_mcu_mb_in,
					&dev->ndp_mutex_mcu_mb_in, &abstime);
		if (err == ETIMEDOUT) {
			auddbg("NDP mbwait timeout\n");
			s = SYNTIANT_NDP_ERROR_TIMEOUT;
		} else if (err) {
			auddbg("Error waiting on ndp_cond_mcu_mb_in\n");
			s = SYNTIANT_NDP_ERROR_FAIL;
		}
	} else {
		timer_start(&ts_start);

		do {
			if (timer_check(&ts_start, 1)) {
				s = SYNTIANT_NDP_ERROR_TIMEOUT;
				auddbg("NDP mbwait timeout (polled)\n");
				break;
			}

			s = syntiant_ndp120_poll(dev->ndp, &notifications, 1);

			if (check_status("mbwait poll", s)) {
				return s;
			}

			if (notifications & NDP_NOTIFICATION_ERRORS) {
				auddbg("NDP error reported: 0x%x", notifications);
				return SYNTIANT_NDP_ERROR_FAIL;
			}

		} while (((notifications & SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN) == 0)
				&& ((notifications
					& SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT) == 0));
	}

	return s;
}

static int get_type(void *d, unsigned int *type)
{
	struct ndp120_dev_s *ndp120 = (struct ndp120_dev_s *) d;
	int s = SYNTIANT_NDP_ERROR_NONE;
	uint8_t data;

	s = transfer(ndp120, 0, NDP120_SPI_ID0, NULL, &data, 1);

	if (check_status("get id", s)) {
		return s;
	}

	*type = data;

	return s;
}

static int iif_sync(void *d)
{
	struct ndp120_dev_s *dev  = (struct ndp120_dev_s *) d;
	int s0, s = SYNTIANT_NDP_ERROR_NONE;

	/* here, the mutex lock can return EINTR */
	do {
		s0 = pthread_mutex_lock(&dev->ndp_mutex_mbsync);
	} while (s0 == EINTR);
	if (s0) {
		auddbg("error locking ndp_mutex_mbsync s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto error;
	}
	do {
		s0 = pthread_mutex_lock(&dev->ndp_mutex_mcu_mb_in);
	} while (s0 == EINTR);
	if (s0) {
		auddbg("error locking ndp_mutex_mcu_mb_in s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
	}

error:
	return s;
}

static int iif_unsync(void *d)
{
	struct ndp120_dev_s *dev  = (struct ndp120_dev_s *) d;
	int s0, s = SYNTIANT_NDP_ERROR_NONE;

	s0 = pthread_mutex_unlock(&dev->ndp_mutex_mcu_mb_in);
	if (s0) {
		auddbg("error unlocking ndp_mutex_mcu_mb_in s0=%d\n", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto error;
	}

	s0 = pthread_mutex_unlock(&dev->ndp_mutex_mbsync);
	if (s0) {
		auddbg("error unlocking ndp_mutex_mbsync s0=%d", s0);
		s = SYNTIANT_NDP_ERROR_FAIL;
	}

error:
	return s;
}

static int iif_udelay(unsigned int n)
{
	up_udelay(n);
	return 0;
}

static int initialize_ndp(struct ndp120_dev_s *dev)
{
	struct syntiant_ndp_integration_interfaces_s iif;
	struct syntiant_ndp_device_s *ndp = NULL;
	syntiant_ndp120_config_clk_pll_t config_clk_pll;
	syntiant_ndp120_config_clk_src_t config_clk_src;
	int s;

	/* stuff the ILib integration interfaces */
	iif.d = dev;
	iif.malloc = (void * (*)(int)) kmm_malloc;
	iif.free = free;
	iif.mbwait = mbwait;
	iif.get_type = get_type;
	iif.sync = iif_sync;
	iif.unsync = iif_unsync;
	iif.transfer = transfer;
	iif.udelay = iif_udelay;

	dev->ndp_interrupts_enabled = false;

	int full_speed_freq = dev->lower->spi_config.freq;
	dev->lower->spi_config.freq = NDP120_SPI_FREQ_INIT;

	/*
	 * initialize the ILib with the integration interface functions and
	 * reset the NDP device
	 */
	s = syntiant_ndp_init(&dev->ndp, &iif, SYNTIANT_NDP_INIT_MODE_RESET);
	if (check_status("ndp_init", s)) {
		return s;
	}

	/*
	 * syntiant_ndp_init() returns the malloced pointer to the NDP ILib
	 * internal state for use by future syntiant_ndp*() API calls
	 */
	ndp = dev->ndp;

	/* enable xtal */
	syntiant_ndp120_config_clk_xtal_t config_clk_xtal;
	memset(&config_clk_xtal, 0, sizeof(config_clk_xtal));
	config_clk_xtal.set = SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OUT | SYNTIANT_NDP120_CONFIG_SET_CLK_XTAL_OSC;
	config_clk_xtal.out = true;
	config_clk_xtal.osc = true;
	s = syntiant_ndp120_config_clk_xtal(ndp, &config_clk_xtal);
	if (check_status("config_clk_xtal", s)) {
		return s;
	}

	/* lock then switch to pll without touching refclk field */
	memset(&config_clk_pll, 0, sizeof(config_clk_pll));
	config_clk_pll.set = SYNTIANT_NDP120_CONFIG_SET_CLK_PLL_PRESET;
	config_clk_pll.preset = SYNTIANT_NDP120_CLOCK_MODE_PLL_1P1V_32768_98P304MHZ;
	s = syntiant_ndp120_config_clk_pll(ndp, &config_clk_pll);
	if (check_status("config_clk_pll", s)) {
		return s;
	}

	/* supply the PLL-generated clock to the NDP */
	memset(&config_clk_src, 0, sizeof(config_clk_src));
	config_clk_src.set = SYNTIANT_NDP120_CONFIG_SET_CLK_SRC_REFSEL;
	config_clk_src.refsel = true;
	s = syntiant_ndp120_config_clk_src(ndp, &config_clk_src);
	check_status("config_clk_src", s);
	auddbg("Changing SPI speed to %dHz\n", NDP120_SPI_FREQ_HIGH);
	dev->lower->spi_config.freq = NDP120_SPI_FREQ_HIGH;

	return s;
}

static int load_synpkg(struct syntiant_ndp_device_s *ndp, const char *p)
{
	struct stat st;
	int package_len;
	void *package;
	int s = 0;
	int pfd;
	int rl;

	/*
	 * load synpkg file
	 */
	if (stat(p, &st) < 0) {
		auddbg("unable to stat synpkg file\n");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	package_len = st.st_size;

	package = kmm_malloc(package_len);
	if (!package) {
		auddbg("no memory for package_load\n");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	pfd = open(p, O_RDONLY);
	if (pfd < 0) {
		auddbg("unable to open synpkg file\n");
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto errorout_with_package;
	}

	rl = read(pfd, package, package_len);
	if (check_io("synpkg file read", package_len, rl)) {
		s = SYNTIANT_NDP_ERROR_FAIL;
		goto errorout_with_package;
	}

	close(pfd);

	auddbg("Loading %d bytes of package data\n", package_len);

	/*
	 * reset the ILib synpkg parser state
	 */
	s = syntiant_ndp_load(ndp, NULL, 0);
	if (s != SYNTIANT_NDP_ERROR_MORE) {
		auddbg("Error resetting package load state\n");
		goto errorout_with_package;
	}

	/*
	 * load the entire synpkg object with a single call
	 */
	s = syntiant_ndp_load(ndp, package, package_len);
	if (check_status("load", s)) {
		goto errorout_with_package;
	}

errorout_with_package:
	free(package);
	return s;
}

/* build an array that hold all labels based on network number */
int save_labels_per_network(struct syntiant_ndp_device_s *ndp,
						char **labels, int num_labels)
{
	int i, j, len, s = SYNTIANT_NDP_ERROR_NONE;
	int class_num = 0, nn_num = 0, prev_nn_num = 0;
	char *label_string;
	struct ndp120_dev_s *dev;
	for (j = 0; j < num_labels; j++) {
		label_string = labels[j];
		len = strlen(label_string);
		i = 0;
		/*Assuming lables are in the form NN<network_num>:<class_name>*/
		while (len) {
			if (isdigit(label_string[i])) {
				nn_num = label_string[i] - '0';
				if (nn_num < 0 || nn_num >= MAX_NNETWORKS) {
					s = SYNTIANT_NDP_ERROR_INVALID_NETWORK;
					auddbg("Invalid network type label\n");
					return s;
				}
				break;
			}
			i++;
			len--;
		}
		if (nn_num != prev_nn_num) {
			class_num = 0;
		}
		auddbg("  [%d][%d] label: %s\n", nn_num, class_num, label_string);
		dev = ndp->iif.d;
		dev->labels_per_network[nn_num][class_num++] = label_string;
		prev_nn_num = nn_num;
	}
	return s;
}

static int get_versions_and_labels(struct syntiant_ndp_device_s *ndp,
						char *label_data, unsigned int label_data_length,
						char **labels, unsigned int *num_labels)
{
	char fwver[NDP120_MCU_FW_VER_MAX_LEN] = "";
	char dspfwver[NDP120_MCU_DSP_FW_VER_MAX_LEN] = "";
	char pkgver[NDP120_MCU_PKG_VER_MAX_LEN] = "";
	struct syntiant_ndp_config_s config;
	unsigned int num_labels_;
	unsigned int i;
	int s;

	memset(&config, 0, sizeof(config));
	config.firmware_version = fwver;
	config.firmware_version_len = STRING_LEN;
	config.dsp_firmware_version = dspfwver;
	config.dsp_firmware_version_len = STRING_LEN;
	config.pkg_version = pkgver;
	config.pkg_version_len = STRING_LEN;
	config.labels = label_data;
	config.labels_len = label_data_length;

	s = syntiant_ndp_get_config(ndp, &config);
	if (check_status("get_config", s)) {
		return s;
	}

	if (STRING_LEN < config.pkg_version_len) {
		auddbg("package version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.firmware_version_len) {
		auddbg("firmware version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.dsp_firmware_version_len) {
		auddbg("dsp firmware version string too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}
	if (STRING_LEN < config.labels_len) {
		auddbg("labels strings too long");
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	/* get pointers to the labels */
	num_labels_ = 0;
	i = 0;

	/*
	 * labels_len is 4 byte aligned.  Continue processing labels until the
	 * running sum of label characters processed is within 3 bytes of
	 * labels_len
	 */
	while ((config.labels_len - i > 3) && (num_labels_ < MAX_LABELS)) {
		labels[num_labels_] = &label_data[i];
		num_labels_++;
		for (; label_data[i]; i++);
		i++;
	}
	/* print status information obtained from package */
	auddbg("chip: %s\n", config.device_type);
	auddbg("package version: %s\n", pkgver);
	auddbg("firmware version: %s\n", fwver);
	auddbg("dsp firmware version: %s\n", dspfwver);
	auddbg("labels: \n");

	save_labels_per_network(ndp, labels, num_labels_);
	*num_labels = num_labels_;
	return s;
}

static int configure_audio(struct ndp120_dev_s *dev, unsigned int pdm_in_shift)
{
	const unsigned int PDM_MAX_OUT_SHIFT = 7; /* always set to max */
	struct syntiant_ndp120_config_decimation_s decimation_config;
	struct syntiant_ndp120_config_pdm_s pdm_config;
	struct syntiant_ndp120_config_misc_s misc_config;
	ndp120_dsp_config_t dsp_config = {0};
	int s;

	/* configure PDM in shift for both mics */
	/* mic 0 */
	memset(&decimation_config, 0, sizeof(decimation_config));
	decimation_config.set =
		SYNTIANT_NDP120_CONFIG_SET_DECIMATION_INSHIFT
		| SYNTIANT_NDP120_CONFIG_SET_DECIMATION_OUTSHIFT;
	decimation_config.mic = 0;
	decimation_config.inshift = pdm_in_shift;
	decimation_config.outshift = PDM_MAX_OUT_SHIFT;
	s = syntiant_ndp120_config_decimation(dev->ndp, &decimation_config);
	if (check_status("config decimation", s)) {
		goto errout_configure_audio;
	}

	/* mic1 */
	decimation_config.mic = 1;
	s = syntiant_ndp120_config_decimation(dev->ndp, &decimation_config);
	check_status("syntiant_ndp120_config_decimation", s);


	/* Configure gain */
	syntiant_ndp120_config_gain_t config_gain;
	memset(&config_gain, 0, sizeof(config_gain));
	config_gain.mic = 0;
	config_gain.dcremovalmode = SYNTIANT_NDP120_CONFIG_VALUE_GAIN_DCREMOVALMODE_ON;
	config_gain.agcshiftdir = SYNTIANT_NDP120_CONFIG_VALUE_GAIN_AGCSHIFTDIR_LEFT;
	config_gain.agcshiftcnt = 0;
	config_gain.agcfinegrainmul = 0x2000;
	config_gain.set = SYNTIANT_NDP120_CONFIG_SET_GAIN_DCREMOVALMODE
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTDIR
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCSHIFTCNT
					| SYNTIANT_NDP120_CONFIG_SET_GAIN_AGCFINEGRAINMUL;
	s = syntiant_ndp120_config_gain(dev->ndp, &config_gain);

	/* mic 1 */
	config_gain.mic = 1;
	s = syntiant_ndp120_config_gain(dev->ndp, &config_gain);
	check_status("syntiant_ndp120_config_gain", s);

 	/* Note: this test code always sets up for internal clock and then adds intermediate test apis to either;
		1) switch to external clock - for use when buffer is in use (also allows for switching back again)
		or
		2) switch to using PCLK1 input as passthrough (also allows for switching back again)

		The assumption in this code is that at NDP initialization time, the buffer on i2s bclk is set to disabled (OE=0) so NDP can take control of the PDM clock

		To switch the clock in the two different cases:
		1) with "buffer approach":  Generally, this needs to be done in "break before make" fashion
		   A) Switching from internal -> external
		  	  I) Switch NDP to external using ndp120_test_internal_external_switch(dev, 0) API, this will stop the PDM clock from NDP without a hard reconfig of the rest of the PDM interface
			  II) Set buffer OE=1 to allow i2s clk to DMIC

			B) Switching from external -> internal, assuming buffer is on = set to NOT tri-state output
			  I) Set buffer to OE=0, to enable the tri-state of i2s clk to DMIC
			  II) Switch NDP to external using ndp120_test_internal_external_switch(dev, 1) API, this will start the PDM clock from NDP

		2) with "PDM passthrough" approach:
		   A) Switching from internal -> external:
		      Call ndp120_test_internal_passthrough_switch(dev, 0) API
		   A) Switching from external -> internal:
		      Call ndp120_test_internal_passthrough_switch(dev, 1) API

     */

	/* enable the PDM clock (for the default, pdm0 aka 'left' mic) */
	memset(&pdm_config, 0, sizeof(pdm_config));
	pdm_config.interface = 0;
	pdm_config.clk = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_ON;
	pdm_config.sample_rate = 16000;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	pdm_config.clk_mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_INTERNAL;
#else
	pdm_config.clk_mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_CLK_MODE_EXTERNAL;
#endif
	pdm_config.mode = SYNTIANT_NDP120_CONFIG_VALUE_PDM_MODE_STEREO;
	pdm_config.pdm_rate = PDM_CLOCK_PDM_RATE;

	pdm_config.set = SYNTIANT_NDP120_CONFIG_SET_PDM_CLK
					| SYNTIANT_NDP120_CONFIG_SET_PDM_MODE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_CLK_MODE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_PDM_RATE
					| SYNTIANT_NDP120_CONFIG_SET_PDM_SAMPLE_RATE;

	s = syntiant_ndp120_config_pdm(dev->ndp, &pdm_config);
	if (check_status("config pdm clock on", s)) {
		goto errout_configure_audio;
	}


	/*
	 * get the current audio frame size which is governed by the audio
	 * & neural algorithms loaded into the NDP
	 */
	memset(&misc_config, 0, sizeof(misc_config));
	s = syntiant_ndp120_config_misc(dev->ndp, &misc_config);
	if (check_status("config_misc", s)) {
		goto errout_configure_audio;
	}

	dev->sample_size_orig_annot = misc_config.audio_sample_size_bytes;

	s = syntiant_ndp120_read_sample_config(dev->ndp, &dsp_config);
	if (s) {
		goto errout_configure_audio;
	}

	dev->sample_size = dsp_config.aud_samp_size_bytes;

	auddbg("Sample size: %d   annotated: %d\n", dev->sample_size, dev->sample_size_orig_annot);

	int channels_active;
	s = syntiant_ndp120_get_active_configurations(dev->ndp, &channels_active);
	if (check_status("syntiant_ndp120_get_active_configurations", s)) {
		goto errout_configure_audio;
	}
	s = syntiant_ndp120_get_extract_sample_size(dev->ndp, NDP120_SPI_FREQ_HIGH, dev->sample_size, channels_active, &dev->extract_size);
	if (check_status("syntiant_ndp120_get_extract_sample_size", s)) {
		goto errout_configure_audio;
	}

	auddbg("Channels active: 0x%x  extract_size = %d\n", channels_active, dev->extract_size);

errout_configure_audio:
	return s;

}

#ifdef CONFIG_DEBUG_AUDIO_INFO
void print_flow_rule(ndp120_dsp_data_flow_rule_t *flow, int type, size_t len)
{
	int i;
	for(i = 0; i < len; i++) {
		if (flow->dst_type) {
			auddbg("src_type:%s%d dst_type:%s{%d} algo_config_id:%d\n",
				NDP120_DSP_DATA_FLOW_SRC_TYPE_STR(type),
				flow->src_param,
				NDP120_DSP_DATA_FLOW_RULE_DST_STR(*flow),
				flow->dst_param,
				flow->algo_config_index);
		}
		flow++;
	}
}
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(*(x)))

void dsp_flow_show(struct syntiant_ndp_device_s *ndp)
{
	for(uint32_t v = 0; v < 3; v++) {
		ndp120_dsp_data_flow_setup_t filtered_flow_setup = {0};
		syntiant_ndp120_dsp_flow_setup_get_rules(ndp, v, &filtered_flow_setup);
		auddbg("\n flowset ID: %d \n", v);
		auddbg("----> PCM RULES\n");
		print_flow_rule(filtered_flow_setup.src_pcm_audio, NDP120_DSP_DATA_FLOW_SRC_TYPE_PCM_AUDIO,
			ARRAY_SIZE(filtered_flow_setup.src_pcm_audio));
		auddbg("----> FUNC RULES\n");
		print_flow_rule(filtered_flow_setup.src_function, NDP120_DSP_DATA_FLOW_SRC_TYPE_FUNCTION,
			ARRAY_SIZE(filtered_flow_setup.src_function));
		auddbg("----> NN RULES\n");
		print_flow_rule(filtered_flow_setup.src_nn, NDP120_DSP_DATA_FLOW_SRC_TYPE_NN,
			ARRAY_SIZE(filtered_flow_setup.src_nn));
		auddbg("----> MCU RULES\n");
		print_flow_rule(filtered_flow_setup.src_mcu, NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU,
			ARRAY_SIZE(filtered_flow_setup.src_mcu));
		auddbg("----> SENSOR RULES\n");
		print_flow_rule(filtered_flow_setup.src_sensor, NDP120_DSP_DATA_FLOW_SRC_TYPE_MCU,
			ARRAY_SIZE(filtered_flow_setup.src_sensor));
	}
	auddbg("\n======================\n");
}

#endif	/* CONFIG_DEBUG_AUDIO_INFO */

static
void add_dsp_flow_rules(struct syntiant_ndp_device_s *ndp, int include_pcm_audio_extract)
{
	int s = 0;
	ndp120_dsp_data_flow_setup_t setup = {0};

	/* PCM3->FUNCx */
	setup.src_pcm_audio[0].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO;
	setup.src_pcm_audio[0].dst_param = FF_ID;
	setup.src_pcm_audio[0].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_FUNCTION;
	setup.src_pcm_audio[0].algo_config_index = 0;
	setup.src_pcm_audio[0].set_id = 0;
	setup.src_pcm_audio[0].algo_exec_property = 0;

	if (include_pcm_audio_extract) {
		/* PCM3->HOST_EXT_AUDIO */
		setup.src_pcm_audio[1].src_param = NDP120_DSP_DATA_FLOW_SRC_PARAM_AUD0_STEREO;
		setup.src_pcm_audio[1].dst_param = NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO;
		setup.src_pcm_audio[1].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
		setup.src_pcm_audio[1].algo_config_index = 0;
		setup.src_pcm_audio[1].set_id = 0;
		setup.src_pcm_audio[1].algo_exec_property = 0;
	}

	/* FUNCx->NN0 */
	setup.src_function[0].src_param = FF_ID;
	setup.src_function[0].dst_param = KEYWORD_NETWORK_ID;
	setup.src_function[0].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_NN;
	setup.src_function[0].algo_config_index = -1;
	setup.src_function[0].set_id = 0;
	setup.src_function[0].algo_exec_property = 0;

	/* FUNCx->HOST_EXT_AUDIO */
	setup.src_function[1].src_param = FF_ID;
	setup.src_function[1].dst_param = NDP120_DSP_DATA_FLOW_DST_SUBTYPE_AUDIO;
	setup.src_function[1].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_HOST_EXTRACT;
	setup.src_function[1].algo_config_index = 0;
	setup.src_function[1].set_id = 0;
	setup.src_function[1].algo_exec_property = 0;

	/* NN0->MCU */
	setup.src_nn[0].src_param = 0;
	setup.src_nn[0].dst_param = 0;
	setup.src_nn[0].dst_type = NDP120_DSP_DATA_FLOW_DST_TYPE_MCU;
	setup.src_nn[0].algo_config_index = -1;
	setup.src_nn[0].set_id = 0;
	setup.src_nn[0].algo_exec_property = 0;

	printf("applied flow rules\n");
	s = syntiant_ndp120_dsp_flow_setup_apply(ndp, &setup);
	check_status("syntiant_ndp120_dsp_flow_setup_apply", s);

}

static
void attach_algo_config_area(struct syntiant_ndp_device_s *ndp, int32_t algo_id, int32_t algo_config_index)
{
	int s = SYNTIANT_NDP_ERROR_NONE;
	ndp120_dsp_algo_t algos[NDP120_DSP_ALGO_MAX_COUNT];

	memset(algos, 0, sizeof(algos));

	/* read algo bindings */
	s = syntiant_ndp120_read_write_algo_bindings(ndp, algos,
												 sizeof(algos), 1);
	if (s == SYNTIANT_NDP_ERROR_NONE) {
		for (uint8_t algo = 0; algo < NDP120_DSP_ALGO_MAX_COUNT; algo++) {
			if (algos[algo].algo_id == algo_id) {
				/* write algo bindings for the given algo id */
				algos[algo].algo_config_index = algo_config_index;
				s = syntiant_ndp120_read_write_algo_bindings(ndp,
						algos, sizeof(algos), 0);
				if (s != SYNTIANT_NDP_ERROR_NONE) {
					auddbg("failed to write algo bindings\n");
				}
				check_status("write algo_bindings", s);
				auddbg("algo_id: %d, algo_config_index: %d, init status: %d\n",
					algos[algo].algo_id,
					algos[algo].algo_config_index,
					algos[algo].algo_init_status);
			}
		}
	}
	else {
		auddbg("failed to read algo bindings\n");
	}
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/
/* debug function, useful for doing debugging via shell */
struct ndp120_dev_s * ndp120_get_debug_handle(void)
{
	return _ndp_debug_handle;
}

/* debug function, useful for doing debugging via shell */
void ndp120_show_debug(int include_spi)
{
	uint8_t tmp;
	int i, s;
	ndp120_dsp_counters_t dsp_cnts;
	int flowset_id;

	if (!_ndp_debug_handle) return;

	struct syntiant_ndp_device_s *ndp = _ndp_debug_handle->ndp;

	if (include_spi) {
		printf("NDP SPI REGISTERS:\n");
		for (i = 0; i <= 0x12; i++) {
			syntiant_ndp120_read(ndp, 0, i, &tmp);
			printf("0x%02X = %02X\n", i, tmp);
		}

		for (i = 0x30; i <= 0x33; i++) {
			syntiant_ndp120_read(ndp, 0, i, &tmp);
			printf("0x%02X = %02X\n", i, tmp);
		}
	}

	s = syntiant_ndp120_get_dsp_counters(ndp, &dsp_cnts);
	check_status("syntiant_ndp120_get_dsp_counters", s);
	printf("frame_cnt: %d\n", dsp_cnts.frame_cnt);
	printf("dnn_int_cnt: %d\n", dsp_cnts.dnn_int_cnt);
	printf("dnn_err_cnt: %d\n", dsp_cnts.dnn_err_cnt);

	syntiant_ndp120_config_tank_t tank_config;
	memset(&tank_config, 0, sizeof(tank_config));
	tank_config.get = 1;
	s = syntiant_ndp120_config_dsp_tank_memory(ndp, &tank_config);
	check_status("syntiant_ndp120_config_dsp_tank_memory", s);
	printf("NDP120 Tank memory type: %s\n", tank_config.sampletank_mem_type == 1? "HEAP": "DNN");
	printf("NDP120 Tank size (ms)  : %d\n", tank_config.sampletank_msec);

	flowset_id = -1; /* indicates read */
	s = syntiant_ndp120_dsp_flow_get_put_set_id(ndp, &flowset_id);
	check_status("syntiant_ndp120_dsp_flow_get_put_set_id", s);
	printf("NDP120 current flowset id: %d\n", flowset_id);

	printf("KD Enabled: %d\n",_ndp_debug_handle->kd_enabled);
	printf("NDP interrupts enabled : %d\n", _ndp_debug_handle->ndp_interrupts_enabled);

	uint32_t buffill[NDP120_DSP_CONFIG_BUFFILLLEVEL_COUNT];
	for (i = 0; i < NDP120_DSP_CONFIG_BUFFILLLEVEL_COUNT; i++) {
		ndp_mcu_read(NDP120_DSP_CONFIG_BUFFILLLEVEL(i), &buffill[i]);
	}
	for (i = 0; i < NDP120_DSP_CONFIG_BUFFILLLEVEL_COUNT; i++) {
		printf("BUFFILLLEVEL[%d]: 0x%X\n", i, buffill[i]);
	}
}

#ifdef CONFIG_NDP120_ALIVE_CHECK
static int
check_firmware_aliveness(struct ndp120_dev_s *dev, uint32_t wait_period_ms)
{
	int s;
	enum syntiant_ndp_fw_state state;

	/* wait 1 ms more */
	s = syntiant_ndp120_check_fw(dev->ndp, &state,
		(wait_period_ms + 1) * 1000);
	if (s) {
		auddbg("Error in getting the status of firmware: %d\n", s);
		goto out;
	}

	if (state != SYNTIANT_NDP_BOTH_FW_ALIVE) {
		auddbg("<device health>: %s\n",
			state == SYNTIANT_NDP_MCU_FW_ALIVE ?
				"DSP FW Dead and MCU FW Alive" :
			state == SYNTIANT_NDP_DSP_FW_ALIVE ?
				"MCU FW Dead and DSP FW Alive" :
				"MCU and DSP FW Dead");
	}

out:
	return s;
}


static void *
ndp120_app_device_health_check(void *d)
{
	int s;
	struct ndp120_dev_s *dev = (struct ndp120_dev_s *)d;

	uint32_t wait_period_ms;

	wait_period_ms = dev->sample_size * 1000 /
		(SYNTIANT_NDP120_AUDIO_SAMPLE_RATE *
		SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD);

	while (1) {
		s = check_firmware_aliveness(dev, wait_period_ms);
		if (s) {
			printf("Error: %d in check_firmware_aliveness\n", s);
			goto out;
		}
		pm_sleep(5000);
	}

out:
	/* normally won't reach */
	return NULL;
}
#endif

int ndp120_init(struct ndp120_dev_s *dev)
{
	/* File names */
	int s;

	const char *mcu_package = "/mnt/kernel/audio/mcu_fw";
	const char *dsp_package = "/mnt/kernel/audio/dsp_fw";
	const char *neural_package = "/mnt/kernel/audio/kd_local";

	const unsigned int AUDIO_TANK_MS =
		AUDIO_BEFORE_MATCH_MS  /* max word length + ~500 MS preroll */
		+ 300  /* posterior latency of <= 24 MS/frame * 12 frames == 288 MS */
		+ 100; /* generous allowance for RTL8730E match-to-extract time */

	const unsigned int DMIC_1536KHZ_PDM_IN_SHIFT_FF = 6;

	/* save handle so we can use it from debug routine later, e.g. from other util/shell */
	_ndp_debug_handle = dev;

	s = pthread_mutex_init(&dev->ndp_mutex_mbsync, NULL);
	if (s) {
		auddbg("failed to initialize mb sync mutex variable\n");
	}

	s = pthread_mutex_init(&dev->ndp_mutex_mcu_mb_in, NULL);
	if (s) {
		auddbg("failed to initialize mb in mutex variable\n");
	}

	s = pthread_mutex_init(&dev->ndp_mutex_notification_sample, NULL);
	if (s) {
		auddbg("failed to initialize mutex notification match\n");
	}

	s = pthread_cond_init(&dev->ndp_cond_mcu_mb_in, NULL);
	if (s) {
		auddbg("failed to initialize ndp_cond_notification_sample\n");
	}

	s = pthread_cond_init(&dev->ndp_cond_notification_sample, NULL);
	if (s) {
		auddbg("failed to initialize ndp_cond_notification_sample\n");
	}

	/* initialize NDP */
	s = initialize_ndp(dev);
	if (s) {
		auddbg("initialize_ndp failed\n");
		goto errout_ndp120_init;
	}

	/*
	 * load M0, Xtensa and Syntiant Core 2 firmware neural network
	 * perform audio & neural agorithm and associated NDP configuration
	 */
	s = load_synpkg(dev->ndp, mcu_package);
	if (s) {
		auddbg("mcu package load failed\n");
		goto errout_ndp120_init;
	}

	s = load_synpkg(dev->ndp, dsp_package);
	if (s) {
		auddbg("dsp package load failed\n");
		goto errout_ndp120_init;
	}

	s = load_synpkg(dev->ndp, neural_package);
	if (s) {
		auddbg("neural package load failed\n");
		goto errout_ndp120_init;
	}

	attach_algo_config_area(dev->ndp, 49, 0);
	add_dsp_flow_rules(dev->ndp, 0);

	struct syntiant_ndp120_config_tank_s tank_config;
	memset(&tank_config, 0, sizeof(tank_config));
	tank_config.set = SYNTIANT_NDP120_CONFIG_SET_TANK_SAMPLETANK_MSEC;
	tank_config.sampletank_msec = AUDIO_TANK_MS;
	s = syntiant_ndp120_config_dsp_tank_memory(dev->ndp, &tank_config);

	s = configure_audio(dev, DMIC_1536KHZ_PDM_IN_SHIFT_FF);
	if (s) {
		auddbg("audio configure failed\n");
		goto errout_ndp120_init;
	}

	dev->keyword_bytes = round_down(KEYWORD_BUFFER_LEN, dev->sample_size);

	dev->keyword_buffer = (uint8_t *)kmm_malloc(dev->keyword_bytes);

	if (dev->keyword_buffer == NULL) {
		auddbg("keyword buffer allocation failed\n");
		s = SYNTIANT_NDP_ERROR_NOMEM;
		return s;
	}

	s_num_labels = 16;
	s = get_versions_and_labels(dev->ndp, s_label_data, sizeof(s_label_data), s_labels, &s_num_labels);
#ifdef CONFIG_DEBUG_AUDIO_INFO
	dsp_flow_show(dev->ndp);
#endif
	ndp120_kd_stop_match_process(dev);
	ndp120_kd_start(dev);

#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
	dev->extclk_inuse = false;
	ndp120_aec_enable(dev);
#endif

#ifdef CONFIG_NDP120_ALIVE_CHECK
	if (1) {
		pthread_t thread;
		int result = pthread_create(&thread, NULL, ndp120_app_device_health_check, dev);
		if (result) {
			auddbg("Device health check thread creation failed\n");
		}
	}
#endif
errout_ndp120_init:
	return s;
}

static inline void ndp120_poll_for_sample_ready(struct ndp120_dev_s *dev)
{
	uint32_t notifications;
	do {
		syntiant_ndp120_poll(dev->ndp, &notifications, 1);
	} while (!(notifications & SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY));
}

static void extract_keyword(struct ndp120_dev_s *dev)
{
	int s;
	unsigned int extract_bytes;
	int extracted = 0;
	int total_len = dev->keyword_bytes;

	/* when the tank resides in DNN memory there is a risk of contention when extracting larger portions of data
	 * hence use a special approach */
	dev->ndp_interrupts_enabled = false;
	ndp120_set_sample_ready_int(dev, 1);

	extract_bytes = total_len;
	s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
				SYNTIANT_NDP_EXTRACT_FROM_MATCH, NULL,
				&extract_bytes);

	while (extracted < total_len) {
		do {
			extract_bytes = dev->extract_size;
			if (extracted + extract_bytes > total_len) {
				extract_bytes = dev->keyword_bytes - extracted;
			}
			ndp120_poll_for_sample_ready(dev);
			s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
				SYNTIANT_NDP_EXTRACT_FROM_UNREAD, &dev->keyword_buffer[extracted],
				&extract_bytes);
		} while (s == SYNTIANT_NDP_ERROR_DATA_REREAD);
		extracted += extract_bytes;
	}
	dev->keyword_bytes_left = dev->keyword_bytes;
	ndp120_set_sample_ready_int(dev, 0);
	dev->ndp_interrupts_enabled = true;
}

static void ndp120_signal_sample(struct ndp120_dev_s *dev)
{
	int err_no = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err_no) {
	  auddbg("Error obtaining lock: %d\n", err_no);
	  return;
	}

	err_no = pthread_cond_signal(&dev->ndp_cond_notification_sample);
	if (err_no) {
	  	auddbg("Error signaling: %d\n", err_no);
		/* NB: fall through to unlock */
	}

	err_no = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err_no) {
	  auddbg("Error unlock: %d\n", err_no);
	}
}

static void ndp120_signal_mb(struct ndp120_dev_s *dev)
{
	int err_no = pthread_mutex_lock(&dev->ndp_mutex_mcu_mb_in);
	if (err_no) {
		auddbg("Error obtaining lock: %d\n", err_no);
		return;
	}

	err_no = pthread_cond_signal(&dev->ndp_cond_mcu_mb_in);
	if (err_no) {
	  	auddbg("Error signaling: %d\n", err_no);
		/* NB: fall through to unlock */
	}
	err_no = pthread_mutex_unlock(&dev->ndp_mutex_mcu_mb_in);
	if (err_no) {
		auddbg("Error unlock: %d\n", err_no);
	}
}

int ndp120_irq_handler_work(struct ndp120_dev_s *dev)
{

	/* Note that currently this is only used to detect keyword match,
	 * but later will be expanded to AI related notifications */
	struct syntiant_ndp_device_s *ndp = dev->ndp;
	uint32_t notifications = 0; /*initialize this as this is checked if io wait fails and might return wrong results if it holds garbage value*/
	uint32_t summary;
	unsigned int winner;
	int s, serialno = 0;
	uint32_t network_id;
	int ret = SYNTIANT_NDP_ERROR_NONE;

	s = syntiant_ndp120_poll(ndp, &notifications, 1);
	if (check_status("poll", s)) {
		ret = s;
		goto errout_with_irq;
	}

	if (notifications & NDP_NOTIFICATION_ERRORS) {
		ret = SYNTIANT_NDP_ERROR_FAIL;
		goto errout_with_irq;
	}

	if (notifications & SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY) {
		/* signal that we have data to extract */
		ndp120_signal_sample(dev);
	}

	if (notifications & (SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN | SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT)) {
		ndp120_signal_mb(dev);
	}

	if ((notifications & SYNTIANT_NDP_NOTIFICATION_MATCH) && (dev->dev.process_mq != NULL)) {
		s = syntiant_ndp_get_match_summary(ndp, &summary);
		if (check_status("get_match_summary", s)) {
			ret = s;
			goto errout_with_irq;
		}

		if (!(summary & NDP120_SPI_MATCH_MATCH_MASK)) {
			/* no actual match, so this is a match-per-frame interrupt, also commonly used as sample ready indication */
			ndp120_signal_sample(dev);
			goto errout_with_irq;
		}
		network_id = ndp->d.ndp120.last_network_id;
		winner = summary & NDP120_SPI_MATCH_WINNER_MASK;
		if (s_num_labels <= winner) {
			auddbg("winner %u exceeds label count %u\n", winner, s_num_labels - 1);
			ret = SYNTIANT_NDP_ERROR_FAIL;
			goto errout_with_irq;
		}

		if (dev->kd_enabled) {
			auddbg("#################### winner : %d summary : %d network_id : %d\n", winner, summary, network_id);
			/* TODO Local command also need to be handled here */
			switch(network_id) {
				case 0:
					serialno++;
					auddbg("[#%d Hi-Bixby] matched: %s\n", serialno, dev->labels_per_network[network_id][winner]);
					/* extract keyword immediately */
					extract_keyword(dev);
#ifdef CONFIG_NDP120_AEC_SUPPORT
					g_ndp120_state = IS_RECORDING;
#endif
					break;
				case 1:
					auddbg("[#%d Voice Commands] matched: %s\n", serialno, dev->labels_per_network[network_id][winner]);
					break;
				default:
					break;
			}

			struct audio_msg_s msg;
			msg.u.pPtr = NULL;
			msg.msgId = AUDIO_MSG_NONE;
			if (network_id == 0 && !dev->recording) {
				msg.msgId = AUDIO_MSG_KD;
			} else if (network_id == 1) {
				switch (winner) {
				case 0:
					msg.msgId = AUDIO_MSG_LOCAL0;
					break;
				case 1:
					msg.msgId = AUDIO_MSG_LOCAL1;
					break;
				case 2:
					msg.msgId = AUDIO_MSG_LOCAL2;
					break;
				case 3:
					msg.msgId = AUDIO_MSG_LOCAL3;
					break;
				default:
					ret = SYNTIANT_NDP_ERROR_INVALID_NETWORK;
					goto errout_with_irq;
				}
			}
			audvdbg("msgId = %d\n", msg.msgId);
			if (msg.msgId != AUDIO_MSG_NONE) {
				mq_send(dev->dev.process_mq, (FAR const char *)&msg, sizeof(msg), 100);
			}
		}
	}

errout_with_irq:
	/* re-enable interrupts as we have finished the interrupt related work */
	dev->lower->irq_enable(true);
	return ret;
}

int ndp120_irq_handler(struct ndp120_dev_s *dev)
{
	/* add work to the work queue, dont enable interrupts until we handled this one compeltely */
	return work_queue(HPWORK, &ndp120_work, ndp120_irq_handler_work, (void *)dev, 0);
}

int ndp120_set_sample_ready_int(struct ndp120_dev_s *dev, int on)
{
	int s;
	s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, on);
	return s;
}

int ndp120_extract_audio(struct ndp120_dev_s *dev, struct ap_buffer_s *apb)
{
	int s;

	if (apb->nmaxbytes < dev->sample_size) {
		/* currently we always try to flush of sample size at each call to this function
		 * TODO : apply buffering in future if possible
		 */
		return SYNTIANT_NDP_ERROR_NOMEM;
	}

	/* buffer size (apb size) for ndp120 is set equal to the sample size.
	 * Hence, all the calls to extract_audio will have total bytes as sample size
	 * So, we can safely copy the extracted data without checking the limits on the
	 * apb size
	 */
	uint32_t sample_size = dev->sample_size;
	s = SYNTIANT_NDP_ERROR_NONE;
	if (dev->keyword_bytes_left != 0) {
		memcpy(apb->samp, &dev->keyword_buffer[dev->keyword_bytes - dev->keyword_bytes_left], sample_size);
		dev->keyword_bytes_left -= sample_size;
		apb->nbytes = dev->sample_size;
		return SYNTIANT_NDP_ERROR_NONE;
	}

	/* wait for sample interrupt for a given time */

	int err = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex lock err: %d\n", err);
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	/* Set timeout value to three sample sizes (2 should be enough). Generally the time values are in milli seconds */
	/* 16KHz, 1 channel, 2bytes = 16000 * 1 * 2 = 32000 bytes per second, 32 bytes per millisecond, (sample size is 640 or 768) */
	uint32_t timeout_in_msec = (3 * dev->sample_size * 1000) / (SYNTIANT_NDP120_AUDIO_SAMPLE_RATE * SYNTIANT_NDP120_AUDIO_SAMPLES_PER_WORD);
	abstime.tv_nsec += timeout_in_msec * 1000000;

	int timedwait_err = pthread_cond_timedwait(&dev->ndp_cond_notification_sample,
							&dev->ndp_mutex_notification_sample, &abstime);
	if (timedwait_err) {
		auddbg("NDP sample wait err: %d\n", timedwait_err);
		/* Fall through to unlock the mutex */
	}

	err = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex unlock err: %d\n", err);
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	if (timedwait_err) {
		/* Underrun need to be handled here, however, as of now we are doing
		 * data extraction in sync. So, send back the apb with zero bytes
		 * TODO : apply async method of extraction, then handle the underrun
		 * case appropriately as we can add this buffer to pendq and use it
		 */
		apb->nbytes = 0;
		return SYNTIANT_NDP_ERROR_FAIL;
	}

	/* since apb can hold 4 samples, set extraction size to 4 */
	uint32_t extract_size = apb->nmaxbytes;
	s = syntiant_ndp_extract_data(dev->ndp,
		SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
		SYNTIANT_NDP_EXTRACT_FROM_UNREAD, apb->samp, &extract_size);

	if (s) {
		auddbg("error occured : %d\n", s);
	}

	apb->nbytes = extract_size;

	return s;
}

int ndp120_kd_start(struct ndp120_dev_s *dev)
{
	int s = SYNTIANT_NDP_ERROR_NONE;

	/* Enable interrupts from ndp120 side */
	int on = SYNTIANT_NDP_INTERRUPT_DEFAULT;
	s = syntiant_ndp_interrupts(dev->ndp, &on);
	dev->ndp_interrupts_enabled = true;
	return s;
}

int ndp120_kd_stop(struct ndp120_dev_s *dev)
{
	int s = SYNTIANT_NDP_ERROR_NONE;

	/* Disable interrupts from ndp120 side */
	int off = 0;
	s = syntiant_ndp_interrupts(dev->ndp, &off);
	dev->ndp_interrupts_enabled = false;
	return s;
}

int ndp120_kd_stop_match_process(struct ndp120_dev_s *dev) {
	int s = SYNTIANT_NDP_ERROR_NONE;
	/* nothing here, we check priv->kd_enabled in the match callprocessing instead */
	/* alternative solution could be to configure the posterior to ignore
	 * matches, in which case we could let this function configure the PH action for the keyword to be "stay" */
	return s;
}

int ndp120_kd_start_match_process(struct ndp120_dev_s *dev) {
#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
#endif
	int s = SYNTIANT_NDP_ERROR_NONE;
	/* nothing here, we check priv->kd_enabled in the match callprocessing instead */
	/* alternative solution could be to configure the posterior to ignore
	 * matches, in which case we could let this function configure the PH action for the keyword to be "match" */
	return s;
}

int ndp120_start_sample_ready(struct ndp120_dev_s *dev)
{
	int s;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = IS_RECORDING;
#endif

	dev->recording = true;

	s =  ndp120_set_sample_ready_int(dev, 1);

	uint32_t bytes_before_match = 0;

	if (!dev->running) {
		/* we need not do this if this is resume case, we only need to do it if its recorder start case after keyword detection */
		s = syntiant_ndp_extract_data(dev->ndp, SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
																SYNTIANT_NDP_EXTRACT_FROM_MATCH, NULL,
																&bytes_before_match);
	}

	return s;
}

int ndp120_stop_sample_ready(struct ndp120_dev_s *dev)
{
	int s;

	s = ndp120_set_sample_ready_int(dev, 0);

	dev->recording = false;
#ifdef CONFIG_NDP120_AEC_SUPPORT
	g_ndp120_state = NOT_RECORDING;
#endif
	return s;
}

#ifdef CONFIG_NDP120_AEC_SUPPORT
void ndp120_test_internal_external_switch(struct ndp120_dev_s *dev, int internal)
{
    uint32_t audctrl = 0;
    int intf = 0;

    syntiant_ndp120_read(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), &audctrl);
    if (internal) {
		/* set to internal */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 1);
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 1);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
    } else {
		/* set to external */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 0);
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_OE_MASK_INSERT(audctrl, 0);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_IN);
    }

    syntiant_ndp120_write(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), audctrl);
}


void ndp120_test_internal_passthrough_switch(struct ndp120_dev_s *dev, int internal)
{
    uint32_t audctrl = 0;
    int intf = 0;

    syntiant_ndp120_read(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), &audctrl);
    if (internal) {
		/* set to internal */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 1);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 5);   /* pclk0, pdat */
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_OUT);
    } else {
		/* set to pass-thru */
		audctrl = NDP120_CHIP_CONFIG_AUDCTRL_PDMCLKOUTNEEDED_MASK_INSERT(audctrl, 0);
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_IE_MASK_INSERT(audctrl, 7);   /* pclk0, pclk1, pdat */
        audctrl = NDP120_CHIP_CONFIG_AUDCTRL_MODE_MASK_INSERT(audctrl, NDP120_CHIP_CONFIG_AUDCTRL_MODE_PDM_THRU); /* pdm-thru */
    }

    syntiant_ndp120_write(dev->ndp, 1, NDP120_CHIP_CONFIG_AUDCTRL(intf), audctrl);
}

void ndp120_aec_enable(struct ndp120_dev_s *dev)
{
	if (g_ndp120_state == NOT_RECORDING && !dev->extclk_inuse) {
		ndp120_test_internal_passthrough_switch(dev, 0);
		dev->extclk_inuse = true;
	}
}

void ndp120_aec_disable(struct ndp120_dev_s *dev)
{
	ndp120_test_internal_passthrough_switch(dev, 1);
	dev->extclk_inuse = false;
}
#endif

