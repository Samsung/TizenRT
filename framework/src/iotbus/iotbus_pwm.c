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
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <tinyara/pwm.h>
#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_pwm.h>

// debugging
#include <stdio.h>
#define zdbg printf   // adbg, idbg are already defined

/*
 * private types
 */
#define DEFAULT_PWM_DEVICE_NUM 0
#define IOTBUS_PWM_MAX_PERIOD 1000000 // 1s
#define IOTBUS_PWM_MAX_RESOLUTION 65536 // b16ONE

struct _iotbus_pwm_s {
	int fd;
	int enabled;
#ifdef CONFIG_PWM
	struct pwm_info_s config;
#endif
};

//int g_board_init = 0;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PWM

iotbus_pwm_context_h iotbus_pwm_open(int device, int channel)
{
	if (device != DEFAULT_PWM_DEVICE_NUM)
		return NULL;

	char pwm_dev[32] = { 0, };
	int fd;

	snprintf(pwm_dev, 32, "/dev/pwm%d", channel);
	fd = open(pwm_dev, O_RDONLY);
	if (fd < 0) {
		zdbg("open %s failed: %d\n", pwm_dev, errno);
		return NULL;
	}

	struct _iotbus_pwm_s *hnd = (struct _iotbus_pwm_s *)malloc(sizeof(struct _iotbus_pwm_s));
	if (hnd == NULL) {
		zdbg("malloc failed\n");
		close(fd);
		return NULL;
	}
	memset(hnd, 0x0, sizeof(*hnd));
	hnd->fd = fd;
	return hnd;
}

int iotbus_pwm_close(iotbus_pwm_context_h pwm)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	int fd = pwm->fd, ret;
	if (pwm->enabled) {
		ret = ioctl(fd, PWMIOC_STOP, 0);
		if (ret < 0)
			zdbg("stop fail\n");
	}

	close(fd);
	free(pwm);

	return 0;
}

int iotbus_pwm_set_duty_cycle(iotbus_pwm_context_h pwm, uint32_t duty_cycle)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;
	if (duty_cycle > 100)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	int fd = pwm->fd;

	struct pwm_info_s *info = &(pwm->config);
	int ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	info->duty = (duty_cycle * IOTBUS_PWM_MAX_RESOLUTION) / 100;
	ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}
	return IOTBUS_ERROR_NONE;
}

// period : us

int iotbus_pwm_set_period(iotbus_pwm_context_h pwm, uint32_t period)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	// TBD: check period is valid, get the min and max period in each board.

	int fd = pwm->fd;
	struct pwm_info_s *info = &(pwm->config);
	int ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	info->frequency = IOTBUS_PWM_MAX_PERIOD / period;
	ret = ioctl(fd, PWMIOC_SETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_SETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}
	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_set_enabled(iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;
	if (!(enable == 0 || enable == 1))
		return IOTBUS_ERROR_INVALID_PARAMETER;
	int fd = pwm->fd, ret;
	pwm->enabled = enable;
	if (enable)
		ret = ioctl(fd, PWMIOC_START, 0);
	else
		ret = ioctl(fd, PWMIOC_STOP, 0);

	if (ret < 0) {
		zdbg("ioctl(PWMIOC_START) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}
	return IOTBUS_ERROR_NONE;
}

int iotbus_pwm_is_enabled(iotbus_pwm_context_h pwm)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	return pwm->enabled;
}

int iotbus_pwm_get_duty_cycle(iotbus_pwm_context_h pwm)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	struct pwm_info_s *info = &pwm->config;
	int fd = pwm->fd;
	int ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}
	return (int)((info->duty * 100.0 / IOTBUS_PWM_MAX_RESOLUTION) + 0.5);
}

int iotbus_pwm_get_period(iotbus_pwm_context_h pwm)
{
	if (!pwm)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	struct pwm_info_s *info = &(pwm->config);
	int fd = pwm->fd;
	int ret = ioctl(fd, PWMIOC_GETCHARACTERISTICS, (unsigned long)((uintptr_t)info));
	if (ret < 0) {
		zdbg("ioctl(PWMIOC_GETCHARACTERISTICS) failed: %d\n", errno);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return (int)(IOTBUS_PWM_MAX_PERIOD / info->frequency);
}

#else // CONFIG_PWM
iotbus_pwm_context_h
iotbus_pwm_open(int device, int channel)
{
	zdbg("pwm is not supported\n");
	return NULL;
}
int
iotbus_pwm_close(iotbus_pwm_context_h pwm)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_set_duty_cycle(iotbus_pwm_context_h pwm, uint32_t duty_cycle)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_set_period(iotbus_pwm_context_h pwm, uint32_t period)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_set_enabled(iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_is_enabled(iotbus_pwm_context_h pwm)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_get_duty_cycle(iotbus_pwm_context_h pwm)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}
int
iotbus_pwm_get_period(iotbus_pwm_context_h pwm)
{
	zdbg("pwm is not supported\n");
	return IOTBUS_ERROR_NOT_SUPPORTED;
}

#endif // CONFIG_PWM

#ifdef __cplusplus
}
#endif

