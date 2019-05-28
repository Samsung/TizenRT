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

/**
 * @file iotbus_adc.c
 * @brief Iotbus APIs for ADC
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <tinyara/analog/adc.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_adc.h>

#include "iotbus_internal.h"

struct _iotbus_adc_s {
	int fd;
	uint8_t channel;
	iotbus_adc_state_e state;
	sem_t state_sem;
	adc_read_cb callback;
};

struct _iotbus_adc_wrapper_s {
	struct _iotbus_adc_s *handle;
};

#ifdef __cplusplus
extern "C" {
#endif

static void *iotbus_adc_handler(void *hnd)
{
	struct _iotbus_adc_s *handle;
	size_t readsize;
	ssize_t nbytes;
	int ret;
	int timeout = 1000;

	struct pollfd fds[1];
	struct adc_msg_s sample[1];

	handle = (struct _iotbus_adc_s *)hnd;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = handle->fd;
	fds[0].events = POLLIN | POLLERR;

	handle->state = IOTBUS_ADC_BUSY;

	while (handle->state != IOTBUS_ADC_STOP) {
		ret = poll(fds, 1, timeout);

		if (ret < 0) {
			idbg("[ADC] poll error(%d)\n", ret);
			continue;
		} else if (ret == 0) {
			idbg("[ADC] timeout(%d)\n", timeout);
			continue;
		}

		if (fds[0].revents & POLLIN) {
			readsize = sizeof(struct adc_msg_s);
			nbytes = read(handle->fd, sample, readsize);

			/* Handle unexpected return values */
			if (nbytes < 0) {
				idbg("[ADC] sampling: Fail to read...\n");
				break;
			} else if (nbytes == 0) {
				idbg("[ADC] sampling: No data read, Ignoring\n");
			} else {
				if (readsize != nbytes) {
					idbg("[ADC] sampling: read size=%ld is not a multiple of sample size=%d, Ignoring\n", (long)nbytes, sizeof(struct adc_msg_s));
				} else {
					// To Do : Now, get sample data only once.
					idbg("[ADC] sampling: channel: %d value: %d\n", sample->am_channel, sample->am_data);
					if (sample->am_channel == handle->channel) {
						handle->callback(sample->am_channel, sample->am_data);
					}
				}
			}
		}
	}
	handle->state = IOTBUS_ADC_RDY;
	sem_post(&handle->state_sem);
	idbg("[ADC] exit iotbus_adc handler\n");

	return 0;
}

iotbus_adc_context_h iotbus_adc_init(int bus, uint8_t channel)
{
	int fd;
	char dev_path[16] = { 0, };
	struct _iotbus_adc_s *handle;
	iotbus_adc_context_h dev;

	snprintf(dev_path, sizeof(dev_path), "/dev/adc%d", bus);
	fd = open(dev_path, O_RDWR);
	if (fd < 0) {
		return NULL;
	}

	handle = (struct _iotbus_adc_s *)malloc(sizeof(struct _iotbus_adc_s));
	if (!handle) {
		goto errout_with_close;
	}

	dev = (struct _iotbus_adc_wrapper_s *)malloc(sizeof(struct _iotbus_adc_wrapper_s));
	if (!dev) {
		free(handle);
		goto errout_with_close;
	}

	handle->fd = fd;
	handle->callback = NULL;
	handle->channel = channel;
	handle->state = IOTBUS_ADC_RDY;
	sem_init(&handle->state_sem, 0, 0);
	dev->handle = handle;

	return dev;

errout_with_close:
	close(fd);
	return NULL;
}

int iotbus_adc_deinit(iotbus_adc_context_h hnd)
{
	struct _iotbus_adc_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	close(handle->fd);
	free(handle);
	hnd->handle = NULL;
	free(hnd);

	return IOTBUS_ERROR_NONE;
}

int iotbus_adc_set_channel(iotbus_adc_context_h hnd, uint8_t channel)
{
	struct _iotbus_adc_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	handle->channel = channel;

	return IOTBUS_ERROR_NONE;
}

int iotbus_adc_get_channel(iotbus_adc_context_h hnd)
{
	struct _iotbus_adc_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	return (int)handle->channel;
}

int iotbus_adc_get_state(iotbus_adc_context_h hnd)
{
	struct _iotbus_adc_s *handle;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	return (int)handle->state;
}

int iotbus_adc_start(iotbus_adc_context_h hnd, const adc_read_cb read_cb)
{
	struct _iotbus_adc_s *handle;

	if (!hnd || !hnd->handle || !read_cb) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	if (handle->state != IOTBUS_ADC_RDY) {
		return IOTBUS_ERROR_DEVICE_NOT_READY;
	}

	handle->callback = read_cb;

	pthread_t tid;
	int ret;
	ret = pthread_create(&tid, NULL, iotbus_adc_handler, (void *)handle);
	if (ret < 0) {
		idbg("[ADC] create iotapi handler fail(%d)\n", ret);
		return IOTBUS_ERROR_UNKNOWN;
	}
	pthread_detach(tid);

	return IOTBUS_ERROR_NONE;
}

int iotbus_adc_stop(iotbus_adc_context_h hnd)
{
	struct _iotbus_adc_s *handle;
	struct timespec abstime;
	struct timespec before;
	int status;

	if (!hnd || !hnd->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_adc_s *)hnd->handle;

	if (handle->state == IOTBUS_ADC_RDY) {
		return IOTBUS_ERROR_NONE;
	}

	if (handle->state == IOTBUS_ADC_BUSY) {
		handle->state = IOTBUS_ADC_STOP;

		(void)clock_gettime(CLOCK_REALTIME, &before);
		abstime.tv_sec  = before.tv_sec + 1;
		abstime.tv_nsec = before.tv_nsec;

		status = sem_timedwait(&handle->state_sem, &abstime);
		if (status != OK) {
			idbg("[ADC] sem_timedwait Timeout\n");
			return IOTBUS_ERROR_TIMED_OUT;
		}
	}

	return IOTBUS_ERROR_NONE;
}

uint32_t iotbus_adc_get_sample(iotbus_adc_context_h hnd, int timeout)
{
	struct _iotbus_adc_s *handle;
	size_t readsize;
	ssize_t nbytes;
	int ret;

	struct adc_msg_s sample[1];

	struct pollfd fds[1];

	handle = (struct _iotbus_adc_s *)hnd->handle;
	handle->state = IOTBUS_ADC_BUSY;

	memset(fds, 0, sizeof(fds));
	fds[0].fd = handle->fd;
	fds[0].events = POLLIN | POLLERR;

	readsize = sizeof(struct adc_msg_s);

	while (1) {
		ret = poll(fds, 1, timeout);
		if (ret < 0) {
			continue;
		} else if (ret == 0) {
			idbg("[ADC] POLL timeout[%d]\n", received);
			ret = IOTBUS_ERROR_TIMED_OUT;
			break;
		}

		if (fds[0].revents & POLLIN) {
			nbytes = read(handle->fd, sample, readsize);

			if (nbytes <= 0) {
				idbg("[ADC] sampling: Fail to read...\n");
				ret = IOTBUS_ERROR_UNKNOWN;
				break;
			} else {
				if (readsize != nbytes) {
					idbg("[ADC] sampling: read size=%ld is not a multiple of sample size=%d, Ignoring\n", (long)nbytes, sizeof(struct adc_msg_s));
					ret = IOTBUS_ERROR_DEVICE_FAIL;
					break;
				} else {
					// To Do : Now, get sample data only once.
					idbg("[ADC] sampling: channel: %d value: %d\n", sample->am_channel, sample->am_data);
					if (sample->am_channel == handle->channel) {
						ret = sample->am_data;
						break;
					}
				}
			}
		}
	}

iobus_adc_read_done:
	handle->state = IOTBUS_ADC_RDY;
	idbg("[ADC] exit iotbus_adc handler\n");

	return ret;
}

#ifdef __cplusplus
}
#endif
