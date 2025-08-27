/*
 * Copyright (c) 2024 Syntiant Corp.  All rights reserved.
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
*/

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

#include "../../../audio/ndp120_voice.h"
#include <syntiant_ilib/syntiant_ndp_error.h>

static int pdm_clk_en(struct syntiant_ndp_device_s *ndp, uint32_t clk)
{
	struct syntiant_ndp120_config_pdm_s pdm_config;
	int s;

	memset(&pdm_config, 0, sizeof(pdm_config));
	pdm_config.interface = 0;
	pdm_config.clk = clk;

	pdm_config.set = SYNTIANT_NDP120_CONFIG_SET_PDM_CLK;
	s = syntiant_ndp120_config_pdm(ndp, &pdm_config);
	if (s) {
		auddbg("PDM clock set (%d) failed\n", clk);
	}
	return s;
}

int ndp120_utils_stream_init(struct ndp120_dev_s *dev, unsigned int duration, int verbose, int* dev_extract_size)
{
	int num_channels = 4;
	int s;

	auddbg("Stream.... Duration: %d.  verbose: %d\n", duration, verbose);

	auddbg("num_channels: %d    Sample size: %d   Annotated: %d dev_extract_size: %d\n", num_channels, dev->sample_size, dev->sample_size_orig_annot, dev->extract_size);
	*dev_extract_size =  dev->extract_size;

	s = pdm_clk_en(dev->ndp, 0);
	if (s) {
		auddbg("PDM clock disable failed\n");
	}
	s = syntiant_ndp120_init_ring_buffer_pointers(dev->ndp, 0);
	if (s) {
		auddbg("syntiant_ndp120_init_ring_buffer_pointers failed\n");
	}
	s = syntiant_ndp120_dsp_restart(dev->ndp);
	if (s) {
		auddbg("syntiant_ndp120_dsp_restart failed\n");
	}
	s = pdm_clk_en(dev->ndp, 1);
	if (s) {
		auddbg("PDM clock enable failed\n");
	}

    auddbg("Turning on sample ready...\n");
    s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, 1);
	if (s) {
		auddbg("syntiant_ndp120_config_notify_on_sample_ready failed\n");
	}
	return s;
}

int ndp120_utils_stream_deinit(struct ndp120_dev_s *dev)
{
	auddbg("Turning off sample ready...\n");
    int s = syntiant_ndp120_config_notify_on_sample_ready(dev->ndp, 0);
	if (s) {
		auddbg("syntiant_ndp120_config_notify_on_sample_ready failed\n");
	}
	return s;
}

int ndp120_utils_stream_get_data(struct ndp120_dev_s *dev, uint8_t *data, uint32_t *extracted_size)
{
	static int started = 0;
	static unsigned long n_extractions = 0;
	uint32_t extract_size;
	int s;

	/* wait for sample interrupt */
	int err = pthread_mutex_lock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex lock err: %d\n", err);
		goto err_out;
	}

	/* wait with timeout if already started */
	if (started) {
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);

		{
			// Convert to nanoseconds
			long nanoseconds = 80000000;

			// Add nanoseconds to the current nanosecond value
			abstime.tv_nsec += nanoseconds;

			// Check for overflow and adjust seconds and nanoseconds accordingly
			while (abstime.tv_nsec >= 1000000000) {
				abstime.tv_nsec -= 1000000000;
				abstime.tv_sec++;
			}
		}
		err = pthread_cond_timedwait(&dev->ndp_cond_notification_sample,
								&dev->ndp_mutex_notification_sample, &abstime);
		if (err) {
			/*
			this is not a real error, we may just have read the data in an earlier round
			syntiant_ms_time tnow;
			syntiant_get_ms_time(&tnow);
			audvdbg("Sample ready notification timeout @ %u\n", tnow);
			*/
		}
	} else {
		err = pthread_cond_wait(&dev->ndp_cond_notification_sample,
								&dev->ndp_mutex_notification_sample);
		if (err) {
			auddbg("NDP sample wait err: %d\n", err);
			goto err_out;
		}
		started = 1;
		auddbg("1st sample ready arrived\n");
	}
	err = pthread_mutex_unlock(&dev->ndp_mutex_notification_sample);
	if (err) {
		auddbg("NDP sample mutex unlock err: %d\n", err);
		goto err_out;
	}

	do {
		extract_size = dev->extract_size;
		s = syntiant_ndp_extract_data(dev->ndp,
			SYNTIANT_NDP_EXTRACT_TYPE_INPUT,
			SYNTIANT_NDP_EXTRACT_FROM_UNREAD, data, &extract_size);
	} while (s == SYNTIANT_NDP_ERROR_DATA_REREAD);
	n_extractions++;
	*extracted_size = extract_size;

err_out:
	return err;
}
