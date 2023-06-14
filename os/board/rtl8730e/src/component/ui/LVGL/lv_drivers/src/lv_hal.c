/*
 * Copyright (c) 2021 Realtek, LLC.
 * All rights reserved.
 *
 * Licensed under the Realtek License, Version 1.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License from Realtek
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <sys/time.h>
#include "lvgl.h"
#include "display.h"
#include "touch.h"

#include "FreeRTOS.h"
#include "task.h"

#include "lv_hal.h"

#define DISP_BUF_SIZE (480 * 800)

#define CMD_TEST
#define OSAL_TIME_MSEC_PER_SEC     1000LL
#define OSAL_TIME_NSEC_PER_MSEC    1000000LL
#define OSAL_TIME_NSEC_PER_SEC     1000000000LL

typedef struct {
	int64_t sec;   // seconds
	int64_t nsec;  // nano seconds
} OsalTimespec;

void lv_hal_init()
{
	lv_init();
	display_init();
	touch_init();

	/*A small buffer for LittlevGL to draw the screen's content*/
	static lv_color_t buf1[DISP_BUF_SIZE];

	/*Initialize a descriptor for the buffer*/
	static lv_disp_draw_buf_t disp_buf;
	lv_disp_draw_buf_init(&disp_buf, buf1, NULL, DISP_BUF_SIZE);

	/*Initialize and register a display driver*/
	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.draw_buf   = &disp_buf;
	disp_drv.flush_cb   = display_flush;
	disp_drv.hor_res    = 480;
	disp_drv.ver_res    = 800;
	lv_disp_drv_register(&disp_drv);

	static lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.type = LV_INDEV_TYPE_POINTER;
	indev_drv.read_cb = touch_read;
	lv_indev_drv_register(&indev_drv);
}

int OsalGetTime(OsalTimespec *time)
{
	if (!time) {
		RTLOGE("%s: invalid parameter", __func__);
		return -1;
	}

	xTimeOutType current_time = {0};
	uint64_t tick_count = 0ULL;
	vTaskSetTimeOutState(&current_time);
	tick_count = (uint64_t) current_time.xOverflowCount << (sizeof(TickType_t) * 8);
	tick_count += current_time.xTimeOnEntering;

	tick_count = (int64_t) tick_count * OSAL_TIME_NSEC_PER_SEC / configTICK_RATE_HZ;

	int64_t carry_sec = 0;
	time->sec = (int64_t) tick_count / OSAL_TIME_NSEC_PER_SEC;
	time->nsec = (int64_t) tick_count % OSAL_TIME_NSEC_PER_SEC;

	if (time->sec < 0L) {
		carry_sec = (time->sec / (int64_t) OSAL_TIME_NSEC_PER_SEC) + 1L;
		time->sec -= (int64_t) carry_sec;
		time->sec += carry_sec * (int64_t) OSAL_TIME_NSEC_PER_SEC;
	}

	return 0;
}

int64_t OsalGetSysTimeMs()
{
	OsalTimespec time;
	time.sec = 0;
	time.nsec = 0;
	OsalGetTime(&time);
	return time.sec * OSAL_TIME_MSEC_PER_SEC + time.nsec / OSAL_TIME_NSEC_PER_MSEC;
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
	return OsalGetSysTimeMs();
}