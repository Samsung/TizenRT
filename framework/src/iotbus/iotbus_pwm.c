/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * @file iotbus_pwm.c
 * @brief Iotbus APIs for PWM
 */

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/pwm.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_pwm.h>

#include "iotbus_internal.h"

/*
 * private types
 */
#define DEFAULT_PWM_DEVICE_NUM 0
#define IOTBUS_PWM_MAX_PERIOD 1000000 // 1s
#define IOTBUS_PWM_MAX_RESOLUTION 65535 // (b16ONE - 1)

struct _iotbus_pwm_s {
	int fd;
	int enabled;
	iotbus_pwm_val_e idle;
	struct pwm_info_s config;
};

struct _iotbus_pwm_wrapper_s {
	struct _iotbus_pwm_s *handle;
};

#ifdef __cplusplus
extern "C" {
#endif

iotbus_pwm_context_h iotbus_pwm_open(int device, int channel)
{
	int fd;
	char pwm_dev[32] = { 0, };
	struct _iotbus_pwm_s *handle;
	iotbus_pwm_context_h dev;

	if (device != DEFAULT_PWM_DEVICE_NUM) {
		return NULL;
	}

	snprintf(pwm_dev, sizeof(pwm_dev), "/dev/pwm%d", channel);
	fd = open(pwm_dev, O_RDONLY);
	if (fd < 0) {
		ibdbg("open %s failed: %d\n", pwm_dev, errno);
		return NULL;
	}

	handle = (struct _iotbus_pwm_s *)malloc(sizeof(struct _iotbus_pwm_s));
	if (!handle) {
		goto errout_with_close;
	}

	dev = (struct _iotbus_pwm_wrapper_s *)malloc(sizeof(struct _iotbus_pwm_wrapper_s));
	if (!dev) {
		free(handle);
		goto errout_with_close;
	}

	memset(handle, 0, sizeof(*handle));
	handle->fd = fd;
	dev->handle = handle;

	return dev;

errout_with_close:
	close(fd);
	return NULL;
}

int iotbus_pwm_close(iotbus_pwm_context_h pwm)
{
	int fd;
	int ret = -1;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	fd = handle->fd;

	if (handle->enabled) {
		ret = ioctl(fd, PWMIOC_STOP, 0);
		if (ret < 0) {
			ibdbg("stop fail\n");
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	close(handle->fd);
	free(handle);
	pwm->handle = NULL;
	free(pwm);

	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_set_duty_cycle(iotbus_pwm_context_h pwm, percent_t duty_cycle)
{
	int ret;

	if (!pwm || !pwm->handle || duty_cycle > 100) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	uint16_t duty = ((duty_cycle * IOTBUS_PWM_MAX_RESOLUTION) / 100.0) + 0.5;
	ret = iotbus_pwm_set_duty_cycle_in_integer(pwm, duty);

	return ret;
}

int iotbus_pwm_set_duty_cycle_in_integer(iotbus_pwm_context_h pwm, uint16_t duty_cycle)
{
	int fd;
	int ret;
	struct pwm_info_s *info;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle || duty_cycle > IOTBUS_PWM_MAX_RESOLUTION) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	fd = handle->fd;
	info = &(handle->config);
	ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	info->duty = duty_cycle;
	ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_set_idle(iotbus_pwm_context_h pwm, iotbus_pwm_val_e val)
{
	int ret;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;
	handle->idle = val;

	ret = ioctl(handle->fd, PWMIOC_IDLE, (unsigned long)(handle->idle));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_IDLE) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

// period : us

int iotbus_pwm_set_period(iotbus_pwm_context_h pwm, uint32_t period)
{
	int fd;
	int ret;
	struct pwm_info_s *info;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	// TBD: check period is valid, get the min and max period in each board.

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	fd = handle->fd;
	info = &(handle->config);
	ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	info->frequency = IOTBUS_PWM_MAX_PERIOD / period;
	ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_set_enabled(iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable)
{
	int fd;
	int ret;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if (!(enable == 0 || enable == 1)) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	fd = handle->fd;
	handle->enabled = enable;

	if (enable) {
		ret = ioctl(fd, PWMIOC_START, 0);
	} else {
		ret = ioctl(fd, PWMIOC_STOP, 0);
	}

	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_START) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_is_enabled(iotbus_pwm_context_h pwm)
{
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	return handle->enabled;
}

int iotbus_pwm_get_duty_cycle(iotbus_pwm_context_h pwm)
{
	int fd;
	int ret;
	struct pwm_info_s *info;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	info = &(handle->config);
	fd = handle->fd;
	ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return (int)((info->duty * 100.0 / IOTBUS_PWM_MAX_RESOLUTION) + 0.5);
}

int iotbus_pwm_get_period(iotbus_pwm_context_h pwm)
{
	int fd;
	int ret;
	struct pwm_info_s *info;
	struct _iotbus_pwm_s *handle;

	if (!pwm || !pwm->handle) {
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	handle = (struct _iotbus_pwm_s *)pwm->handle;

	info = &(handle->config);
	fd = handle->fd;
	ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		ibdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return (int)(IOTBUS_PWM_MAX_PERIOD / info->frequency);
}

#ifdef __cplusplus
}
#endif

