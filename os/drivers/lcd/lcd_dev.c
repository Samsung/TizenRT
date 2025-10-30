/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
 * drivers/lcd/lcd_dev.c
 *
 *   Copyright (C) 2017=-2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <poll.h>
#include <errno.h>
#include <debug.h>
#include <fcntl.h>
#include <stdio.h>
#include <sched.h>

#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/lcd/lcd_dev.h>
#include <tinyara/pm/pm.h>
#include <tinyara/silent_reboot.h>

#define MAX_NO_PLANES 3
/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure provides the state of the lcd_dev driver */

struct lcd_s {
	FAR struct lcd_dev_s *dev;
	struct lcd_planeinfo_s planeinfo[MAX_NO_PLANES];
	sem_t sem;
	int16_t crefs;
#ifdef CONFIG_PM
	struct pm_domain_s *pm_domain;
#endif
#if defined(CONFIG_LCD_FLUSH_THREAD)
	uint8_t *lcd_kbuffer;
	FAR const struct lcddev_area_s *lcd_area;
	sem_t flush_buffer_sem;
	sem_t copy_buffer_sem;
#endif
};
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Character driver methods */

static int lcddev_open(FAR struct file *filep);
static int lcddev_close(FAR struct file *filep);
static int lcddev_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t lcddev_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t lcddev_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_lcddev_fops = {
	lcddev_open,				/* open */
	lcddev_close,				/* close */
	lcddev_read,				/* read */
	lcddev_write,				/* write */
	NULL,						/* seek */
	lcddev_ioctl,				/* ioctl */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: lcd_semtake
 ************************************************************************************/

void lcd_semtake(sem_t *sem)
{
	/* Take the semaphore (perhaps waiting) */

	while (sem_wait(sem) != 0) {
		/* The only case that an error should occur here is if
		 * the wait was awakened by a signal.
		 */

		ASSERT(*get_errno_ptr() == EINTR);
	}
}

/****************************************************************************
 * Name: lcd_semgive
 ****************************************************************************/

void lcd_semgive(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name: lcddev_open
 ****************************************************************************/

static int lcddev_open(FAR struct file *filep)
{
	FAR struct lcd_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);
	priv->crefs++;
	DEBUGASSERT(priv->crefs > 0);
	lcd_semgive(&priv->sem);

	return OK;
}

/****************************************************************************
 * Name: lcddev_close
 ****************************************************************************/

static int lcddev_close(FAR struct file *filep)
{
	FAR struct lcd_s *priv;
	priv = filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	lcd_semgive(&priv->sem);

	return OK;
}

static ssize_t lcddev_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return OK;
}

static ssize_t lcddev_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return OK;
}

/****************************************************************************
 * Name: lcddev_ioctl
 ****************************************************************************/

static int lcddev_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct lcd_s *priv;
	int ret = OK;
	priv = filep->f_inode->i_private;
	if (!priv) {
		return -EINVAL;
	}

	lcd_semtake(&priv->sem);

	switch (cmd) {
	case LCDDEVIO_GETRUN: {
		FAR struct lcddev_run_s *lcd_run = (FAR struct lcddev_run_s *)arg;
		if (priv->planeinfo[lcd_run->planeno].getrun) {
			ret = priv->planeinfo[lcd_run->planeno].getrun(priv->dev, lcd_run->row, lcd_run->col, lcd_run->data, lcd_run->npixels);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_PUTRUN: {
		FAR const struct lcddev_run_s *lcd_run = (FAR const struct lcddev_run_s *)arg;
		if (priv->planeinfo[lcd_run->planeno].putrun) {
			ret = priv->planeinfo[lcd_run->planeno].putrun(priv->dev, lcd_run->row, lcd_run->col, lcd_run->data, lcd_run->npixels);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETAREA: {
		FAR struct lcddev_area_s *lcd_area = (FAR struct lcddev_area_s *)arg;
		size_t cols = lcd_area->col_end - lcd_area->col_start + 1;
		size_t pixel_size = priv->planeinfo[lcd_area->planeno].bpp > 1 ? priv->planeinfo[lcd_area->planeno].bpp >> 3 : 1;
		size_t row_size = lcd_area->stride > 0 ? lcd_area->stride : cols * pixel_size;

		if (priv->planeinfo[lcd_area->planeno].getarea) {
			ret = priv->planeinfo[lcd_area->planeno].getarea(priv->dev, lcd_area->row_start, lcd_area->row_end, lcd_area->col_start, lcd_area->col_end, lcd_area->data, row_size);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_PUTAREA: {
#if defined(CONFIG_LCD_FLUSH_THREAD)
		sem_wait(&priv->copy_buffer_sem);
		priv->lcd_area = (FAR const struct lcddev_area_s *)arg;
		if (priv->planeinfo[priv->lcd_area->planeno].putarea) {
			memcpy(priv->lcd_kbuffer, priv->lcd_area->data, CONFIG_LCD_XRES * CONFIG_LCD_YRES * sizeof(uint16_t));
			sem_post(&priv->flush_buffer_sem);
		} else {
			ret = -ENOSYS;
		}
#else
		// No flush thread condition. Will always wait for previous frame return before processing next frame
		FAR const struct lcddev_area_s *lcd_area = (FAR const struct lcddev_area_s *)arg;
		size_t cols = lcd_area->col_end - lcd_area->col_start + 1;
		size_t pixel_size = priv->planeinfo[lcd_area->planeno].bpp > 1 ? priv->planeinfo[lcd_area->planeno].bpp >> 3 : 1;
		size_t row_size = lcd_area->stride > 0 ? lcd_area->stride : cols * pixel_size;

		if (priv->planeinfo[lcd_area->planeno].putarea) {
			ret = priv->planeinfo[lcd_area->planeno].putarea(priv->dev, lcd_area->row_start, lcd_area->row_end, lcd_area->col_start, lcd_area->col_end, lcd_area->data, row_size);
		} else {
			ret = -ENOSYS;
		}
#endif
	}
	break;
	case LCDDEVIO_GETPOWER: {
		if (priv->dev->getpower) {
			*((FAR int *)arg) = priv->dev->getpower(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETPOWER: {

#ifdef CONFIG_PM
		if (!priv->dev->setpower || !priv->dev->getpower) {
			ret = -ENOSYS;
			break;
		}
		int new_power = (int)arg;
		int old_power = priv->dev->getpower(priv->dev);

		lcddbg("LCD Backlight %d -> %d\n", old_power, new_power);

		/* To ensure setpower operation from power saving,
		 * pm_suspend is calling first. */
		if (old_power == 0 && new_power > 0) {
			lcddbg("Lock pm & silent reboot\n");
			silent_reboot_lock();
			(void)pm_suspend(priv->pm_domain);
		}

		ret = priv->dev->setpower(priv->dev, new_power);
		if (ret != OK) {
			lcddbg("ERROR: Failed to set power level %d -> %d (errno: %d)\n", old_power, new_power, ret);
			break;
		}

		/* To ensure setpower operation from power saving,
		 * pm_resume is calling after setpower. */
		if (old_power > 0 && new_power == 0) {
			lcddbg("Unlock pm & silent reboot\n");
			silent_reboot_unlock();
			silent_reboot_delay(3600);
			(void)pm_resume(priv->pm_domain);
		}
#else
		if (priv->dev->setpower) {
			ret = priv->dev->setpower(priv->dev, (int)arg);
		} else {
			ret = -ENOSYS;
		}
#endif
	}
	break;
	case LCDDEVIO_GETCONTRAST: {
		if (priv->dev->getcontrast) {
			*((FAR int *)arg) = priv->dev->getcontrast(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETCONTRAST: {
		if (priv->dev->setcontrast) {
			ret = priv->dev->setcontrast(priv->dev, (unsigned int)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETPLANEINFO: {
		*((FAR struct lcd_planeinfo_s *)arg) = priv->planeinfo[0];
	}
	break;
	case LCDDEVIO_GETVIDEOINFO: {
		if (priv->dev->getvideoinfo) {
			ret = priv->dev->getvideoinfo(priv->dev, (FAR struct fb_videoinfo_s *)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_INIT: {
		if (priv->dev->init) {
			ret = priv->dev->init(priv->dev);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_SETORIENTATION: {
		if (priv->dev->setorientation) {
			ret = priv->dev->setorientation(priv->dev, (unsigned int)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	case LCDDEVIO_GETLCDINFO: {
		if (priv->dev->getlcdinfo) {
			ret = priv->dev->getlcdinfo(priv->dev, (FAR struct lcd_info_s *)arg);
		} else {
			ret = -ENOSYS;
		}
	}
	break;
	default: {
		lcddbg("ERROR: Unsupported IOCTL command: %d\n", cmd);
		ret = -ENOSYS;
	}
	break;
	}
	lcd_semgive(&priv->sem);
	return ret;
}

#if defined(CONFIG_LCD_FLUSH_THREAD)
static void lcd_flushing_thread(int argc, char **argv)
{
	int ret;
	DEBUGASSERT(argc == 2);
	FAR struct lcddev_area_s *lcd_area;
	struct lcd_s *lcd_info = (struct lcd_s *)strtoul(argv[1], NULL, 16);
	lcd_area = &(lcd_info->lcd_area);
	while (true) {
		while (sem_wait(&lcd_info->flush_buffer_sem) != 0) {
			ASSERT(errno == EINTR);
		}
		size_t cols = lcd_area->col_end - lcd_area->col_start + 1;
		size_t pixel_size = lcd_info->planeinfo[lcd_area->planeno].bpp > 1 ? lcd_info->planeinfo[lcd_area->planeno].bpp >> 3 : 1;
		size_t row_size = lcd_area->stride > 0 ? lcd_area->stride : cols * pixel_size;
		// NULL check of putarea has been added in ioctl, Not required here
		lcd_info->planeinfo[lcd_area->planeno].putarea(lcd_info->dev, lcd_area->row_start, lcd_area->row_end, lcd_area->col_start, lcd_area->col_end, lcd_info->lcd_kbuffer, row_size);
		sem_post(&lcd_info->copy_buffer_sem);
	}
	return;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_LCD_SW_ROTATION
#define NUM_OF_LCD_BUFFER	2
uint8_t *lcd_buffer[NUM_OF_LCD_BUFFER] = { NULL, NULL };	//Two lcd buffers to avoid screen tearing
int lcd_buffer_index = 0;
void lcd_rotate_buffer(short int* src, short int* dst)
{
	int row;
	int col;
	int dst_inc = 2 * CONFIG_LCD_XRES;
	short int val0;
	short int val1;
	short int val2;
	short int val3;
	short int *psrc;
	short int *pdst;

#if defined(CONFIG_LCD_LANDSCAPE)
	for (row = 0; row < CONFIG_LCD_XRES; row += 2) {
		psrc = src + row * CONFIG_LCD_YRES;
		pdst = dst + CONFIG_LCD_XRES - row - 2;
		for (col = 0; col < CONFIG_LCD_YRES; col += 2) {
			val0 = *(psrc + 0);
			val1 = *(psrc + 1);
			val2 = *(psrc + CONFIG_LCD_YRES + 0);
			val3 = *(psrc + CONFIG_LCD_YRES + 1);
			psrc += 2;
			*(pdst + 0) = val2;
			*(pdst + 1) = val0;
			*(pdst + CONFIG_LCD_XRES) = val3;
			*(pdst + CONFIG_LCD_XRES + 1) = val1;
			pdst += dst_inc;
		}
	}
#elif defined(CONFIG_LCD_RLANDSCAPE)
	for (row = 0; row < CONFIG_LCD_XRES; row += 2) {
		psrc = src + row * CONFIG_LCD_YRES;
		pdst = dst + row + (CONFIG_LCD_YRES - 1) * CONFIG_LCD_XRES;
		for (col = 0; col < CONFIG_LCD_YRES; col += 2) {
			val0 = *(psrc + 0);
			val1 = *(psrc + 1);
			val2 = *(psrc + CONFIG_LCD_YRES + 0);
			val3 = *(psrc + CONFIG_LCD_YRES + 1);
			psrc += 2;
			*(pdst + 0) = val0;
			*(pdst + 1) = val2;
			*(pdst - CONFIG_LCD_XRES) = val1;
			*(pdst - CONFIG_LCD_XRES + 1) = val3;
			pdst -= dst_inc;
		}
	}
#else
	#error LCD Screen Rotation support only available from PORTRAIT to LANDSCAPE AND RLANDSCAPE
#endif
}
#endif /* CONFIG_LCD_SW_ROTATION */

/****************************************************************************
 * Name: lcddev_register
 *
 * Description:
 *   Register the LCD character driver.
 *   This function register LCD driver as PM domain and render initial image
 *   to screen. 
 *   And if CONFIG_LCD_FLUSH_THREAD is enabled, it will create a separate 
 *   thread which will handle screen flush operations.
 *
 * Input Parameters:
 *   dev - The LCD device instance to be registered.
 *
 * Returned Value:
 *   Zero (OK) is returned on success. Otherwise a negated errno value is
 *   returned to indicate the nature of the failure.
 *
 ****************************************************************************/

int lcddev_register(struct lcd_dev_s *dev)
{
	char devname[16] = { 0, };
	int ret;
	struct lcd_s *lcd_info;
	bool is_silent_mode;
	char splash_image_path[50];
	uint8_t *fullscreen_buffer = NULL;
	int xres = CONFIG_LCD_XRES;
	int yres = CONFIG_LCD_YRES;
#if defined(CONFIG_LCD_FLUSH_THREAD)
	int pid;
	char *flushing_thread_args[2];
	char lcd_info_addr[9]; /* for storing 32 bit address */
#endif

	if (!dev) {
		return -EINVAL;
	}

	/* Allocate a new lcd_dev driver instance */
	lcd_info = (struct lcd_s *)kmm_zalloc(sizeof(struct lcd_s));
	if (!lcd_info) {
		return -ENOMEM;
	}

	lcd_info->dev = dev;
#if defined(CONFIG_LCD_FLUSH_THREAD)
	sem_init(&lcd_info->flush_buffer_sem, 0, 0);
	sem_init(&lcd_info->copy_buffer_sem, 0, 1);
	lcd_info->lcd_kbuffer = (uint8_t *)kmm_malloc(CONFIG_LCD_XRES * CONFIG_LCD_YRES * sizeof(uint16_t));
	if (!lcd_info->lcd_kbuffer) {
		sem_destroy(&lcd_info->flush_buffer_sem);
		sem_destroy(&lcd_info->copy_buffer_sem);
		kmm_free(lcd_info);
		lcddbg("ERROR: Failed to allocate memory for LCD flush swap buffer\n");
		return -ENOMEM;
	}
	itoa((int)lcd_info, lcd_info_addr, 16);
	flushing_thread_args[0] = lcd_info_addr;
	flushing_thread_args[1] = NULL;
	pid = kernel_thread("LCD Frame flusing", 204, 8192, lcd_flushing_thread, (FAR char *const *)flushing_thread_args);
	if (pid < 0) {
		kmm_free(lcd_info->lcd_kbuffer);
		sem_destroy(&lcd_info->flush_buffer_sem);
		sem_destroy(&lcd_info->copy_buffer_sem);
		kmm_free(lcd_info);
		lcddbg("ERROR: Failed to start LCD Frame Flusing thread\n");
		return -ENOMEM;
	}
	lcdvdbg("lcd flushing thread %d created \n", pid);
#endif

#ifdef CONFIG_PM
	lcd_info->pm_domain = pm_domain_register("LCD");
#endif

	sem_init(&lcd_info->sem, 0, 1);

	if (lcd_info->dev->getplaneinfo) {
		lcd_info->dev->getplaneinfo(lcd_info->dev, 0, &lcd_info->planeinfo);	//plane no is taken 0 here
	}

#if defined(CONFIG_LCD_SW_ROTATION)
	fullscreen_buffer = lcd_buffer[lcd_buffer_index];
	lcd_buffer_index = (1 - lcd_buffer_index);
#else
	fullscreen_buffer = (uint8_t *)kmm_malloc(CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2 + 1);
	if (!fullscreen_buffer) {
		lcddbg("Failed to allocate memory for fullscreen buffer\n");
		goto errout;
	}
#endif
	memset(fullscreen_buffer, 0, CONFIG_LCD_XRES * CONFIG_LCD_YRES * 2);

#ifdef CONFIG_LCD_SPLASH_IMAGE
#ifdef CONFIG_LCD_SPLASH_SILENT_BOOT
	is_silent_mode = silent_reboot_is_silent_mode();
	if (!is_silent_mode) {
		snprintf(splash_image_path, sizeof(splash_image_path), "%s/%dx%d/splash_normal.bmp", CONFIG_LCD_SPLASH_IMAGE_PATH, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
	} else {
		snprintf(splash_image_path, sizeof(splash_image_path), "%s/%dx%d/splash_silent.bmp", CONFIG_LCD_SPLASH_IMAGE_PATH, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
	}
#else
	snprintf(splash_image_path, sizeof(splash_image_path), "%s/%dx%d/splash_normal.bmp", CONFIG_LCD_SPLASH_IMAGE_PATH, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
#endif /* CONFIG_LCD_SPLASH_SILENT_BOOT */

	int test_fd = open(splash_image_path, O_RDONLY);
	if (test_fd < 0) {
		lcddbg("No splash image found at %s.\n", splash_image_path);
		goto skip_splash_image;
	}
	close(test_fd);

	ret = image_load_bmp_file(splash_image_path, fullscreen_buffer, CONFIG_LCD_YRES, CONFIG_LCD_XRES);
	if (ret != OK) {
		lcddbg("ERROR: Failed to load splash image %s\n", splash_image_path);
		goto skip_splash_image;
	}
	if (!lcd_info->dev->setpower) {
		lcddbg("ERROR: LCD setpower is not supported");
		return -ENOSYS;
	}

	ret = lcd_info->dev->setpower(lcd_info->dev, 100);
	if (ret == OK) { // LCD ON
#ifdef CONFIG_PM
		(void)pm_suspend(lcd_info->pm_domain);
#endif
		silent_reboot_lock();
	} else {
		lcddbg("ERROR: Failed to turn on LCD\n");
		goto skip_splash_image;
	}

	ret = lcd_info->planeinfo[0].putarea(lcd_info->dev, 0, CONFIG_LCD_XRES - 1, 0, CONFIG_LCD_YRES - 1, fullscreen_buffer, CONFIG_LCD_XRES * 2);
	if (ret != OK) {
		lcd_info->dev->setpower(lcd_info->dev, 0);
#ifdef CONFIG_PM
		(void)pm_resume(lcd_info->pm_domain);
#endif
		silent_reboot_unlock();
	}

skip_splash_image:
#endif /* CONFIG_LCD_SPLASH_IMAGE */
	snprintf(devname, 16, "/dev/lcd0");
	ret = register_driver(devname, &g_lcddev_fops, 0666, lcd_info);
	if (ret == OK) {
		return ret;
	}

err_out:
	lcddbg("ERROR: Failed to register driver %s\n", devname);
#if defined(CONFIG_LCD_FLUSH_THREAD)
	task_delete(pid);
	sem_destroy(&lcd_info->flush_buffer_sem);
	sem_destroy(&lcd_info->copy_buffer_sem);
	kmm_free(lcd_info->lcd_kbuffer);
#endif
	sem_destroy(&lcd_info->sem);
	kmm_free(lcd_info);
	return -ENOSYS;
}
