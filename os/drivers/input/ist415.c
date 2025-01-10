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
 * drivers/input/ist415.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <unistd.h>

#include <tinyara/config.h>
#include <tinyara/i2c.h>
#include <tinyara/irq.h>
#include <tinyara/wdog.h>
#include <tinyara/wqueue.h>

#include <tinyara/input/touchscreen.h>
#include <tinyara/input/ist415.h>

#include "ist415.h"
#include "ist415_misc.h"
#include "ist415_update.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void ist415_enable_touch(struct touchscreen_s *upper);
static void ist415_disable_touch(struct touchscreen_s *upper);
static int ist415_cmd(struct touchscreen_s *upper, int argc, char **argv);
static int ist415_process_event(struct ist415_dev_s *dev);
static void ist415_timer_handler(int argc, uint32_t arg1);
/****************************************************************************
 * Private Data
 ****************************************************************************/
struct i2c_config_s g_ist415_i2c_config = {
	.frequency = IST415_I2C_FREQ,
	.address = IST415_I2C_ADDR,
	.addrlen = IST415_I2C_ADDRLEN,
};

static const struct touchscreen_ops_s g_ist415_ops = {
	ist415_enable_touch,	/* enable */
	ist415_disable_touch,	/* disable */
	ist415_cmd				/* cmd */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ist415_forced_release
 ****************************************************************************/
static void ist415_forced_release(struct ist415_dev_s *dev)
{
	struct touch_sample_s data;

	data.npoints = 0;
	for (int i = 0; i < TOUCH_MAX_POINTS; i++) {
		if (dev->touched[i] == true) {
			ist415vdbg("Forced touch release: %d\n", i);

			memset(&data.point[data.npoints], 0, sizeof(struct touch_point_s));
			data.point[data.npoints].id = i;
			data.point[data.npoints].flags = TOUCH_UP;
			data.npoints++;

			dev->touched[i] = false;
		}
	}
	touch_report(dev->upper, &data);
}

/****************************************************************************
 * Name: ist415_set_palm_state
 ****************************************************************************/

static void ist415_set_palm_state(struct ist415_dev_s *dev, uint32_t id, uint8_t state)
{
	if (state == PALM_ON) {
		dev->palm_state |= (1 << id);
	} else {
		dev->palm_state &= ~(1 << id);
	}
}

/****************************************************************************
 * Name: ist415_check_coord_palm
 ****************************************************************************/

static void ist415_check_coord_palm(struct ist415_dev_s *dev, uint32_t id, uint8_t status, uint8_t type)
{
	if ((status == TOUCH_STA_PRESS || status == TOUCH_STA_MOVE) && (type == TOUCH_TYPE_PALMLARGE)) {
		ist415_set_palm_state(dev, id, PALM_ON);
	} else {
		ist415_set_palm_state(dev, id, PALM_OFF);
	}
}

/****************************************************************************
 * Name: ist415_check_event_palm
 ****************************************************************************/

static void ist415_check_event_palm(struct ist415_dev_s *dev, uint8_t sdata)
{
	if (sdata == 1) {
		ist415_set_palm_state(dev, EVENT_PALM_ID, PALM_ON);
	} else {
		ist415_set_palm_state(dev, EVENT_PALM_ID, PALM_OFF);
	}
}

/****************************************************************************
 * Name: ist415_get_touch_data
 ****************************************************************************/

static int ist415_process_event(struct ist415_dev_s *dev)
{
	struct ts_event_coordinate *p_evt_coord;
	struct ts_event_status *p_evt_status;
	struct ts_event_gesture *p_evt_gesture;
	struct touch_sample_s data;
	uint8_t event[EVENT_PACKET_SIZE];
	uint8_t left_e;
	uint8_t *touch_event;
	uint8_t eid;
	uint8_t coord_type;
	bool is_status_palmlarge = false;
	int ret = 0;
	ret = ist415_i2c_read(dev, HCOM_GET_EVENT_1ST, (uint8_t *) event, EVENT_PACKET_SIZE);
	if (ret) {
		ist415dbg("Fail to read 1st event\n");
		return ret;
	}

	left_e = event[7] & 0x1F;
	touch_event = (uint8_t *)kmm_malloc((left_e + 1) * EVENT_PACKET_SIZE);
	if (!touch_event) {
		return ERROR;
	}
	memcpy(touch_event, event, sizeof(event));

	if ((left_e > 0) && (left_e < 0x1F)) {
		ret = ist415_i2c_read(dev, HCOM_GET_EVENT_ALL, (uint8_t *)(touch_event + EVENT_PACKET_SIZE), left_e * EVENT_PACKET_SIZE);
		if (ret) {
			ist415dbg("Fail to read left data\n");
			kmm_free(touch_event);
			return ret;
		}
	} else if (left_e >= 0x1F) {
		ist415dbg("Touch events buffer overflow\n");
		kmm_free(touch_event);
		return -EIO;
	}

	for (int i = 0; i < (left_e + 1); i++) {
		eid = (touch_event + (i * EVENT_PACKET_SIZE))[0] & 0x3;
		if (eid == EID_COORD) {
			p_evt_coord = (struct ts_event_coordinate *)(touch_event + (i * EVENT_PACKET_SIZE));
			coord_type = (p_evt_coord->ttype_3_2 << 2) | p_evt_coord->ttype_1_0;
			ist415_check_coord_palm(dev, p_evt_coord->tid - 1, p_evt_coord->tsta, coord_type);
		}
	}
	data.npoints = 0;
	for (int i = 0; i < (left_e + 1); i++) {
		eid = (touch_event + (i * EVENT_PACKET_SIZE))[0] & 0x3;
		if (eid == EID_COORD) {
			if (dev->pre_palm_state || dev->palm_state) {
				continue;
			}

			p_evt_coord = (struct ts_event_coordinate *)(touch_event + (i * EVENT_PACKET_SIZE));
			if (p_evt_coord->tsta != TOUCH_STA_NONE) {
				data.point[data.npoints].id = p_evt_coord->tid - 1;
				data.point[data.npoints].x = (p_evt_coord->x_11_4 << 4) | p_evt_coord->x_3_0;
				data.point[data.npoints].y = (p_evt_coord->y_11_4 << 4) | p_evt_coord->y_3_0;
				data.point[data.npoints].h = p_evt_coord->major;
				data.point[data.npoints].w = p_evt_coord->minor;
				data.point[data.npoints].pressure = p_evt_coord->z;
				switch (p_evt_coord->tsta) {
				case TOUCH_STA_PRESS:
					if (dev->touched[data.point[data.npoints].id] == false) {
						dev->touched[data.point[data.npoints].id] = true;
					}
					data.point[data.npoints].flags = TOUCH_DOWN;
					ist415vdbg("COORDDINATES: [P] Point ID=%2d, X=%4d, Y=%4d\n", data.point[data.npoints].id, data.point[data.npoints].x, data.point[data.npoints].y);
					break;
				case TOUCH_STA_MOVE:
					if (dev->touched[data.point[data.npoints].id] == false) {
						dev->touched[data.point[data.npoints].id] = true;
					}
					data.point[data.npoints].flags = TOUCH_MOVE;
					ist415vdbg("COORDDINATES: [M] Point ID=%2d, X=%4d, Y=%4d\n", data.point[data.npoints].id, data.point[data.npoints].x, data.point[data.npoints].y);
					break;
				case TOUCH_STA_RELEASE:
					if (dev->touched[data.point[data.npoints].id]) {
						dev->touched[data.point[data.npoints].id] = false;
					}
					data.point[data.npoints].flags = TOUCH_UP;
					ist415vdbg("COORDDINATES: [R] Point ID=%2d, X=%4d, Y=%4d\n", data.point[data.npoints].id, data.point[data.npoints].x, data.point[data.npoints].y);
					break;
				default:
					break;
				}
				data.npoints++;
			} else {
				ist415wdbg("COORDDINATES: Not Support Coord status (%d)", p_evt_coord->tsta);
			}
		} else if (eid == EID_STATUS) {
			p_evt_status = (struct ts_event_status *)(touch_event + (i * EVENT_PACKET_SIZE));
			switch (p_evt_status->stype) {
			case STS_TYPE_CMD_DRIVEN:
				if (p_evt_status->sid == DRIVEN_CALIBRATION) {
					dev->calib_result = p_evt_status->sdata[0];
					ist415vdbg("Calibration Trigger~ (0x%02X)\n", dev->calib_result);
				}
				break;
			case STS_TYPE_INFO_REPORT:
				if (p_evt_status->sid == INFO_TSP_READY) {
					dev->ready = true;
					ist415vdbg("TSC Ready~\n");
				}
				break;
			case STS_TYPE_EVENT:
				if (p_evt_status->sid == EVENT_PALMLARGE) {
					is_status_palmlarge = true;
					ist415_check_event_palm(dev, p_evt_status->sdata[0]);
				}
				break;
			case STS_TYPE_VENDOR_INFO:
				if (p_evt_status->sid == VENDOR_SELFTEST) {
					dev->cmcs = p_evt_status->sdata[0];
					dev->cmcs_msg = p_evt_status->sdata[1];
					dev->cmcs_min = (((uint16_t) p_evt_status->sdata[2] & 0xF) << 8) | (uint16_t) p_evt_status->sdata[3];
					dev->cmcs_max = (((uint16_t) p_evt_status->sdata[2] & 0xF0) << 4) | (uint16_t) p_evt_status->sdata[4];
					ist415vdbg("SelfTest Trigger~ (0x%02X, 0x%02X)\n", dev->cmcs, dev->cmcs_msg);
				} else if (p_evt_status->sid == VENDOR_REC_REPORT) {
					dev->rec_idle = true;
				}
				break;
			default:
				ist415wdbg("Not support status type: %d\n", p_evt_status->stype);
				break;
			}
		} else if (eid == EID_GESTURE) {
			p_evt_gesture = (struct ts_event_gesture *)(touch_event + (i * EVENT_PACKET_SIZE));
			if (p_evt_gesture->gtype == GESTURE_TYPE_DOUBLETAP) {
				if (p_evt_gesture->gid == GESTURE_DOUBLETAP_WAKEUP) {
					uint16_t x = ((uint16_t) p_evt_gesture->gdata[1] << 8) | (uint16_t) p_evt_gesture->gdata[0];
					uint16_t y = ((uint16_t) p_evt_gesture->gdata[3] << 8) | (uint16_t) p_evt_gesture->gdata[2];
					ist415vdbg("Double Tap Wakeup~(%d, %d)\n", x, y);
					// TODO: KnockKnock Event Process~
				}
			} else {
				ist415wdbg("Not support gesture type:%d\n", p_evt_gesture->gtype);
			}
		} else {
			ist415wdbg("Not supprot EID: %d\n", eid);
		}
	}

	if (data.npoints > 0) {
		touch_report(dev->upper, &data);
	}

	if (eid == EID_COORD) {
		if ((dev->pre_palm_state == 0) && dev->palm_state) {
			// TODO: Report PalmLarge On Process
			ist415vdbg("PalmLarge On (T)\n");
			ist415_forced_release(dev);
		} else if (dev->pre_palm_state && (dev->palm_state == 0)) {
			// TODO: Report PalmLarge Off Process
			ist415vdbg("PalmLarge Off (T)\n");
		}
	} else if ((eid == EID_STATUS) && is_status_palmlarge) {
		if (dev->palm_state == 0) {
			// TODO: Report PalmLarge Off Process
			ist415vdbg("PalmLarge Off (S)\n");
		} else {
			// TODO: Report PalmLarge On Process
			ist415vdbg("PalmLarge On (S)\n");
		}
	}

	dev->pre_palm_state = dev->palm_state ? 1 : 0;

	kmm_free(touch_event);

	if (dev->rec_mode) {
		ist415_recording(dev);
	}

	return OK;
}

/****************************************************************************
 * Name: ist415_set_dbg
 ****************************************************************************/

static int ist415_set_dbg(struct ist415_dev_s *dev, int argc, char **argv)
{
	int level;
	if (argc < 3) {
		return -EINVAL;
	}
	
	level = atoi(argv[2]);
	if (level < IST415_LOG_LEVEL_ERRO || level > IST415_LOG_LEVEL_INFO) {
		return -EINVAL;
	}

	dev->log = level;

	return OK;
}


/****************************************************************************
 * Name: ist415_cmd_show_usage
 ****************************************************************************/

static void ist415_cmd_show_usage(void)
{
	printf("\nusage touchscreen <command #>\n\n");
	printf("The ist415 touchscreen IC specific test command: \n");
	printf("<command #>\n");
	printf("	clb     : Auto Calibration\n");
	printf("	raw     : Display Raw Data\n");
	printf("	cpc     : Display CPC\n");
	printf("	update  : Update binary\n");
	printf("	intrdbg : Set Intr Debug\n");
	printf("	dbg     : Log level, 1:ERROR, 2:WARNING, 3:INFO\n");
	printf("	selftest: Run selftest\n");
}

/****************************************************************************
 * Name: ist415_cmd
 ****************************************************************************/

static int ist415_cmd(struct touchscreen_s *upper, int argc, char **argv)
{
	struct ist415_dev_s *dev = (struct ist415_dev_s *)upper->priv;
	int ret = OK;
	if (argc < 2 || !strcmp(argv[1], "-h") ||!strcmp(argv[1], "--help")) {
		ist415_cmd_show_usage();
		return -EINVAL;
	} 
	
	if (strncmp(argv[1], "clb", 4) == 0) {
		ist415_autocalibration(dev);
	} else if (strncmp(argv[1], "raw", 4) == 0) {
		ist415_display_rawdata(dev);
	} else if (strncmp(argv[1], "cpc", 4) == 0) {
		ist415_display_cpc(dev);
	} else if (strncmp(argv[1], "intdbg", 7) == 0) {
		ret = ist415_set_intr_debug(dev, argc, argv);
	} else if (strncmp(argv[1], "update", 7) == 0) {
		ret = ist415_force_update(dev, argc, argv);
	} else if (strncmp(argv[1], "dbg", 4) == 0) {
		ret = ist415_set_dbg(dev, argc, argv);
	} else if (strncmp(argv[1], "selftest", 9) == 0) {		
		ist415_selftest(dev);
	} else if (strncmp(argv[1], "rec", 4) == 0) {		
		ret = ist415_rec_mode(dev, argc, argv);
	} else {
		ret = -EINVAL;
	}

	if (ret == -EINVAL) {
		ist415_cmd_show_usage();
	}
	return ret;
}


/****************************************************************************
 * Name: ist415_power_on
 ****************************************************************************/

static void ist415_power_on(struct ist415_dev_s *dev, bool download)
{
	struct ist415_config_s *lower = dev->lower;
	int ready_wait = 50;
	
	/*
	 * TODO: Touch Power On Sequence
	 * 1. 3.3v On
	 * 2. If download is true, a delay of 10ms is given.
	 *              Otherwise, a delay of 15ms is given.
	 * 3. Wait ready event
	 */

	ist415vdbg("%s\n", __func__);

	dev->ready = false;
	if (lower->ops && lower->ops->power_on) {
		lower->ops->power_on(lower);
	}

	if (download) {
		msleep(10);
	} else {
		msleep(1);
		ist415_enable(dev);

		while ((!dev->ready) && (ready_wait-- > 0)) {
			msleep(1);
		}

		if (!dev->pre_enable) {
			ist415_disable(dev);
		}
	}

	dev->ready = true;
}

/****************************************************************************
 * Name: ist415_power_off
 ****************************************************************************/

static void ist415_power_off(struct ist415_dev_s *dev)
{
	struct ist415_config_s *lower = dev->lower;

	ist415vdbg("%s\n", __func__);
	
	DEBUGASSERT(dev)

	dev->pre_enable = dev->enable;

	if (dev->enable) {
		ist415_disable(dev);
	}

	if (lower->ops && lower->ops->power_off) {
		lower->ops->power_off(lower);
	}
}

/****************************************************************************
 * Name: ist415_stop_device
 ****************************************************************************/

static void ist415_stop_device(struct ist415_dev_s *dev)
{
	ist415vdbg("%s\n", __func__);

	sem_wait(&dev->sem);

	dev->suspend = true;

	if (dev->knockknock) {
		if (dev->sys_mode != SYS_MODE_LPM) {
			ist415_disable(dev);
			dev->sys_mode = SYS_MODE_LPM;
			ist415_power_mode(dev, dev->sys_mode);
			ist415_enable(dev);
		}
	} else {
		ist415_disable(dev);
		ist415_power_off(dev);
	}

	ist415_forced_release(dev);

	sem_post(&dev->sem);
}

/****************************************************************************
 * Name: ist415_start_device
 ****************************************************************************/

static void ist415_start_device(struct ist415_dev_s *dev)
{
	ist415vdbg("%s\n", __func__);

	sem_wait(&dev->sem);

	dev->suspend = false;

	if (dev->knockknock) {
		if (dev->sys_mode != SYS_MODE_TOUCH) {
			dev->sys_mode = SYS_MODE_TOUCH;
			ist415_power_mode(dev, dev->sys_mode);
		}
	} else {
		ist415_reset(dev, false);
		ist415_enable(dev);
		ist415_start(dev);
	}

	sem_post(&dev->sem);
}

/****************************************************************************
 * Name: ist415_disable_touch
 ****************************************************************************/

static void ist415_disable_touch(struct touchscreen_s *upper)
{
	struct ist415_dev_s *dev = upper->priv;
	ist415vdbg("%s\n", __func__);
	ist415_disable(upper->priv);
}

/****************************************************************************
 * Name: ist415_enable_touch
 ****************************************************************************/

static void ist415_enable_touch(struct touchscreen_s *upper)
{
	struct ist415_dev_s *dev = upper->priv;
	ist415vdbg("%s\n", __func__);
	ist415_enable(upper->priv);
}

/****************************************************************************
 * Name: ist415_lockup_work
 ****************************************************************************/

static void ist415_lockup_work(struct ist415_dev_s *dev)
{
	uint32_t touch_status = 0;
	uint32_t scan_count = 0;
	int ret = 0;

	if (dev->irq_working == false) {
		if (dev->event_mode == true) {
			ret = ist415_i2c_read_da(dev, IST415_TOUCH_STATUS, &touch_status, 1);
			if (ret) {
				ist415dbg("Fail to read touch status\n");
				goto retry_handler;
			}

			if ((touch_status & TOUCH_STATUS_MASK) != TOUCH_STATUS_MAGIC) {
				ist415dbg("Touch status is not corrected (0x%08X)\n", touch_status);
				goto retry_handler;
			}

			if (GET_FINGER_ENABLE(touch_status) == 0) {
				ist415_forced_release(dev);
			}

			scan_count = GET_SCAN_CNT(touch_status);

			if (dev->scan_count == scan_count) {
				ist415dbg("Touch IC is not responded (0x%08X)\n", touch_status);
				goto retry_handler;
			}
		}
	}

	dev->alive_retry = 0;
	dev->scan_count = scan_count;
	(void)wd_start(dev->wdog, MSEC2TICK(IST415_LOOKUP_MS), (wdentry_t)ist415_timer_handler, 1, (uint32_t)dev);

	return;

retry_handler:
	dev->alive_retry++;
	ist415dbg("Retry touch status(%d)\n", dev->alive_retry);
	if (dev->alive_retry == IST415_MAX_ALIVE_CNT) {
		sem_wait(&dev->sem);

		ist415_disable(dev);
		ist415_forced_release(dev);
		ist415_reset(dev, false);
		if (dev->suspend) {
			if (dev->knockknock) {
				if (dev->sys_mode == SYS_MODE_LPM) {
					ist415_power_mode(dev, dev->sys_mode);
					ist415_start(dev);
					ist415_enable(dev);
				} else {
					ist415_start(dev);
					ist415_enable(dev);
				}
			} else {
				ist415_power_off(dev);
			}
		} else {
			ist415_start(dev);
			ist415_enable(dev);
		}

		dev->alive_retry = 0;
		dev->scan_count = 0;

		sem_post(&dev->sem);		
	} else {
		(void)wd_start(dev->wdog, MSEC2TICK(IST415_LOOKUP_RETRY_MS), (wdentry_t)ist415_timer_handler, 1, (uint32_t)dev);
	}
}

/****************************************************************************
 * Name: ist415_timer_handler
 ****************************************************************************/

static void ist415_timer_handler(int argc, uint32_t arg1)
{
	struct ist415_dev_s *dev = (struct ist415_dev_s *)arg1;

	work_queue(HPWORK, &dev->work, (worker_t)ist415_lockup_work, dev, 0);
}

/****************************************************************************
 * Name: touch_interrupt
 ****************************************************************************/

static void touch_interrupt(struct ist415_config_s *config)
{
	int sem_cnt;
	int ret;
	struct ist415_dev_s *dev = (struct ist415_dev_s *)config->upper;

	ret = sem_getvalue(&dev->wait_irq, &sem_cnt);
	if (ret == OK && sem_cnt < 1) {
		sem_post(&dev->wait_irq);
	}
}

/****************************************************************************
 * Name: ist415_irq_thread
 ****************************************************************************/

static int ist415_event_thread(int argc, char **argv)
{
	struct ist415_dev_s *dev;

	DEBUGASSERT(argc == 2);
	dev = (struct ist415_dev_s *)strtoul(argv[1], NULL, 16);

	while (1) {
		while (sem_wait(&dev->wait_irq) != OK) {
			ASSERT(get_errno() == EINTR);
		}

		dev->lower->ops->irq_disable(dev->lower);
		dev->irq_working = true;

		ist415_run_intr_debug(dev);
		if (ist415_process_event(dev) != OK) {
			ist415dbg("Fail to process event\n");
		}
		
		dev->irq_working = false;
		dev->lower->ops->irq_enable(dev->lower);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: put_unaligned_be32
 ****************************************************************************/

void put_unaligned_be32(uint32_t value, void *addr)
{
	uint8_t *p = (uint8_t *) addr;

	p[0] = (value >> 24) & 0xFF;	// Most significant byte
	p[1] = (value >> 16) & 0xFF;
	p[2] = (value >> 8) & 0xFF;
	p[3] = value & 0xFF;		// Least significant byte
}

/****************************************************************************
 * Name: cpu_to_be32
 ****************************************************************************/

uint32_t cpu_to_be32(uint32_t value)
{
	return ((value & 0xFF000000) >> 24) |	// Move byte 3 to byte 0
		   ((value & 0x00FF0000) >> 8) |	// Move byte 2 to byte 1
		   ((value & 0x0000FF00) << 8) |	// Move byte 1 to byte 2
		   ((value & 0x000000FF) << 24);	// Move byte 0 to byte 3
}

/****************************************************************************
 * Name: ist415_i2c_read
 ****************************************************************************/

int ist415_i2c_read(struct ist415_dev_s *dev, uint8_t cmd, uint8_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	int wsize = sizeof(cmd);
	int ret = 0;

	if (size <= 0) {
		ist415dbg("Check read size (%d)\n", size);
		return -EINVAL;
	}

	ret = i2c_write(i2c, config, (uint8_t *)&cmd, wsize);
	if (ret != wsize) {
		ist415dbg("Fail to read i2c\n");
		return ret;
	}

	ret = i2c_read(i2c, config, (uint8_t *) data, size);
	if (ret != size) {
		ist415dbg("Fail to read i2c\n");
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_write
 ****************************************************************************/

int ist415_i2c_write(struct ist415_dev_s *dev, uint8_t cmd, uint8_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	int wsize = sizeof(cmd) + size;
	uint8_t *buf;
	int ret = 0;

	buf = (uint8_t *)kmm_malloc(wsize);
	if (!buf) {
		return -ENOMEM;
	}

	buf[0] = cmd;
	if (size > 0) {
		memcpy(buf + sizeof(cmd), data, size);
	}

	ret = i2c_write(i2c, config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		ist415dbg("Fail to write i2c\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_read_da
 ****************************************************************************/

int ist415_i2c_read_da(struct ist415_dev_s *dev, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	uint8_t cmd = HCOM_SW_DA;
	int wsize = sizeof(cmd) + sizeof(addr);
	int rsize = size * sizeof(uint32_t);
	uint8_t *buf = NULL;
	uint32_t *buf32 = NULL;
	int ret = 0;

	if (size <= 0) {
		ist415dbg("Check read da size (%d)\n", size);
		return -EINVAL;
	}

	buf = (uint8_t *)kmm_malloc(wsize);
	if (!buf) {
		return -ENOMEM;
	}

	buf32 = (uint32_t *)kmm_malloc(rsize);
	if (!buf32) {
		kmm_free(buf);
		return -ENOMEM;
	}

	buf[0] = cmd;
	put_unaligned_be32(addr, buf + sizeof(cmd));

	ret = i2c_write(i2c, config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		ist415dbg("Fail to read DA\n");
		kmm_free(buf);
		kmm_free(buf32);
		return ret;
	}

	ret = i2c_read(i2c, config, (uint8_t *) buf32, rsize);
	if (ret != rsize) {
		ist415dbg("Fail to read DA\n");
		kmm_free(buf);
		kmm_free(buf32);
		return ret;
	}

	for (int i = 0; i < size; i++) {
		data[i] = cpu_to_be32(buf32[i]);
	}

	kmm_free(buf);
	kmm_free(buf32);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_write_da
 ****************************************************************************/

int ist415_i2c_write_da(struct ist415_dev_s *dev, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s *config = dev->i2c_config;
	uint8_t cmd = HCOM_SW_DA;
	int hsize = sizeof(cmd) + sizeof(addr);
	int wsize = hsize + (size * sizeof(uint32_t));
	uint8_t *buf = NULL;
	int ret = 0;

	if (size <= 0) {
		ist415dbg("Check write da size (%d)\n", size);
		return -EINVAL;
	}

	buf = (uint8_t *)kmm_malloc(wsize);
	if (!buf) {
		return -ENOMEM;
	}

	buf[0] = cmd;
	put_unaligned_be32(addr, buf + sizeof(cmd));
	for (int i = 0; i < size; i++) {
		put_unaligned_be32(data[i], buf + (i * sizeof(uint32_t)) + hsize);
	}

	ret = i2c_write(i2c, config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		ist415dbg("Fail to write DA\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_enable
 ****************************************************************************/

void ist415_enable(struct ist415_dev_s *dev)
{
	struct ist415_config_s *lower = dev->lower;

	if (dev->enable == true) {
		return;
	}

	ist415vdbg("%s\n", __func__);
	lower->ops->irq_enable(lower);

	dev->enable = true;
	dev->event_mode = true;
}

/****************************************************************************
 * Name: ist415_disable
 ****************************************************************************/

void ist415_disable(struct ist415_dev_s *dev)
{
	struct ist415_config_s *lower = dev->lower;

	if (dev->enable == false) {
		return;
	}

	ist415vdbg("%s\n", __func__);

	dev->event_mode = false;
	dev->enable = false;

	lower->ops->irq_disable(lower);
}

/****************************************************************************
 * Name: ist415_reset
 ****************************************************************************/

void ist415_reset(struct ist415_dev_s *dev, bool download)
{
	ist415vdbg("%s\n", __func__);

	ist415_power_off(dev);
	msleep(100);
	ist415_power_on(dev, download);
}

/****************************************************************************
 * Name: ist415_sensor
 ****************************************************************************/

void ist415_sensor(struct ist415_dev_s *dev, bool on)
{
	uint8_t cmd = 0;
	int ret = 0;

	if (on) {
		cmd = HCOM_SENSE_ON;
	} else {
		cmd = HCOM_SENSE_OFF;
	}

	ret = ist415_i2c_write(dev, cmd, NULL, 0);
	if (ret) {
		ist415dbg("Fail to write sense on/off command\n");
	}

	ist415vdbg("%s %s\n", __func__, on ? "on" : "off");

	msleep(50);
}

/****************************************************************************
 * Name: ist415_power_mode
 ****************************************************************************/

void ist415_power_mode(struct ist415_dev_s *dev, uint8_t data)
{
	int ret = 0;

	ret = ist415_i2c_write(dev, HCOM_CTRL_POWER_MODE, (uint8_t *)&data, 1);
	if (ret) {
		ist415dbg("Fail to write power mode command\n");
	}
}

/****************************************************************************
 * Name: ist415_touchtype
 ****************************************************************************/

void ist415_touchtype(struct ist415_dev_s *dev, uint8_t *data)
{
	int ret = 0;

	ret = ist415_i2c_write(dev, HCOM_CTRL_TOUCH_TYPE, (uint8_t *) data, 2);
	if (ret) {
		ist415dbg("Fail to write touchtype command\n");
	}
}

/****************************************************************************
 * Name: ist415_fw_integrity
 ****************************************************************************/

uint8_t ist415_fw_integrity(struct ist415_dev_s *dev)
{
	uint8_t integrity = 0;
	int ret = 0;

	ret = ist415_i2c_read(dev, HCOM_GET_FW_INTEGRITY, (uint8_t *)&integrity, 1);

	ist415vdbg("firmware integrity : 0x%02X (%d)\n", integrity, ret);

	return (ret < 0) ? 0xFF : integrity;
}

/****************************************************************************
 * Name: ist415_start
 ****************************************************************************/

void ist415_start(struct ist415_dev_s *dev)
{
	uint8_t mode;
	int ret = 0;

	(void)wd_start(dev->wdog, MSEC2TICK(IST415_LOOKUP_MS), (wdentry_t)ist415_timer_handler, 1, (uint32_t)dev);

	if (dev->rec_mode) {
		mode = REC_ENABLE;
		ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, &mode, 1);
		if (ret) {
			ist415dbg("Fail to write Recording mode command (Enable)\n");
		} else {
			ist415vdbg("Recording mode start\n");
		}
	}

	ist415_touchtype(dev, (uint8_t *)&dev->touch_type);
	ist415_power_mode(dev, dev->sys_mode);
	ist415_sensor(dev, true);

	if (dev->rec_mode) {
		msleep(100);
		mode = REC_START_SCAN;
		ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, &mode, 1);
		if (ret) {
			ist415dbg("Fail to write Recording mode command (Start scan)\n");
		}
	}

	ist415vdbg("%s\n", __func__);
}


/****************************************************************************
 * Name: ist415_touchinitialize
 *
 * Description:
 *   This function is called by board-specific, setup logic to configure
 *   and register the IST415 Touch device.  This function will register the driver
 *   as /dev/touchN where N is determined by the minor device number.
 *
 * Input Parameters:
 *   i2c_dev - The i2c device structure for the I2C bus that the touch controller is connected to
 *   config - The lower half touch driver structure that contains the touch configuration information.
 *
 * Returned Value:
 *   On success, a non-NULL pointer to the touch device structure touch_dev_s is returned.
 *   On a failure, NULL is returned.
 *
 ****************************************************************************/

int ist415_initialize(const char *path, struct i2c_dev_s *i2c, struct ist415_config_s *lower)
{
	int ret;
	struct touchscreen_s *upper;
	struct ist415_dev_s *dev;
	char *parm[2];
	char parm_buf[9];

	dev = (struct ist415_dev_s*)kmm_zalloc(sizeof(struct ist415_dev_s));
	if (!dev) {
		ist415dbg("Fail to alloc ist415_dev_s\n");
		return ERROR;
	}
	dev->i2c = i2c;
	dev->i2c_config = &g_ist415_i2c_config;
	dev->lower = lower;
	lower->upper = dev;

	dev->ready = false;
	dev->enable = false;
	dev->pre_enable = false;
	dev->suspend = false;
	dev->log = IST415_LOG_LEVEL_INFO;

	dev->sys_mode = SYS_MODE_TOUCH;
	dev->touch_type = (1 << TOUCH_TYPE_NORMAL) | (1 << TOUCH_TYPE_WET) | (1 << TOUCH_TYPE_PALMLARGE);
	dev->knockknock = false;

	dev->irq_working = false;
	dev->event_mode = false;

	dev->alive_retry = 0;
	dev->scan_count = 0;

	dev->pre_palm_state = 0;
	dev->palm_state = 0;

	dev->rec_mode = 0;
	dev->rec_addr = 0;
	dev->rec_size = 0;
	dev->cmcs = 0;

	dev->intr_debug_addr = 0;
	dev->intr_debug_size = 0;

	for (int i = 0; i < TOUCH_MAX_POINTS; i++) {
		dev->touched[i] = false;
	}

	ist415_disable(dev);

	// Semaphore reset scenario
	sem_init(&dev->sem, 0, 1);

	dev->lower->handler = touch_interrupt;
	sem_init(&dev->wait_irq, 0, 0);
	itoa((int)dev, parm_buf, 16);
	parm[0] = parm_buf;
	parm[1] = NULL;
	dev->pid = kernel_thread("ist415_isr", CONFIG_IST415_WORKPRIORITY , 2048, (main_t)ist415_event_thread, (FAR char *const *)parm);
	if (dev->pid < 0) {
		ist415dbg("Fail to create kernel thread\n");
		kmm_free(dev);
		return ERROR;
	}

	/* Init System Power */
	ist415_reset(dev, false);

	// check to need firmwar update
	ret = ist415_check_fw((void *)dev);
	if (ret) {
		ist415dbg("Fail to check firmware\n");
	}

	// get to touch info
	ret = ist415_get_info((void *)dev);
	if (ret) {
		ist415dbg("Fail to get info\n");
		kmm_free(dev);
		return ret;
	} else {
		ist415vdbg("Get info success\n");
	}

	ist415_start(dev);
	// ist415_enable(dev);

	dev->wdog = wd_create();
	if (dev->wdog) {
		if (wd_start(dev->wdog, MSEC2TICK(IST415_LOOKUP_MS), (wdentry_t)ist415_timer_handler, 1, (uint32_t)dev) != OK) {
			ist415dbg("Fail to start ist415 wdog\n");
		}
	} else {
		ist415dbg("Fail to alloc ist415 wdog\n");
	}

	upper = (struct touchscreen_s *)kmm_zalloc(sizeof(struct touchscreen_s));
	if (!upper) {
		ist415dbg("Fail to alloc touchscreen_s\n");
		kmm_free(dev);
		ist415_disable(dev);
		return ERROR;
	}
	upper->ops = &g_ist415_ops;
	upper->priv = dev;
	dev->upper = upper;

	ist415dbg("Touch Driver registered Successfully\n");
	return touch_register(path, upper);
}
