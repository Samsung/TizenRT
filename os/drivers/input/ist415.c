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

#include <tinyara/config.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <poll.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/i2c.h>
#include <tinyara/irq.h>
#include <tinyara/input/touchscreen.h>
#include <tinyara/input/ist415.h>

#if defined(CONFIG_TOUCH_CALLBACK)
#include <tinyara/wqueue.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define touchdbg printf
#define touchvdbg printf

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int ist415_read(struct touchscreen_s *dev, FAR char *buffer);
static void ist415_enable(struct touchscreen_s *dev);
static void ist415_disable(struct touchscreen_s *dev);
static bool ist415_istouchSet(struct touchscreen_s *dev);

#if defined(CONFIG_TOUCH_CALLBACK)
static void get_touch_data(struct ist415_dev_s *priv);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

struct touchscreen_ops_s g_ist415_ops = {
	.touch_read = ist415_read,
	.touch_enable = ist415_enable,
	.touch_disable = ist415_disable,
	.is_touchSet = ist415_istouchSet,
};

#if defined(CONFIG_TOUCH_CALLBACK)
static struct work_s ist415_work;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: put_unaligned_be32
 ****************************************************************************/

static __inline void put_unaligned_be32(uint32_t value, void *addr)
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

static __inline uint32_t cpu_to_be32(uint32_t value)
{
	return ((value & 0xFF000000) >> 24) |	// Move byte 3 to byte 0
		   ((value & 0x00FF0000) >> 8) |	// Move byte 2 to byte 1
		   ((value & 0x0000FF00) << 8) |	// Move byte 1 to byte 2
		   ((value & 0x000000FF) << 24);	// Move byte 0 to byte 3
}

/****************************************************************************
 * Name: ist415_i2c_read
 ****************************************************************************/

static int ist415_i2c_read(struct ist415_dev_s *dev, uint8_t cmd, uint8_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	int wsize = sizeof(cmd);
	int ret = 0;

	if (size <= 0) {
		touchdbg("Check read size (%d)\n", size);
		return -EINVAL;
	}

	ret = i2c_write(i2c, &config, (uint8_t *)&cmd, wsize);
	if (ret != wsize) {
		touchdbg("Fail to read i2c\n");
		return ret;
	}

	ret = i2c_read(i2c, &config, (uint8_t *) data, size);
	if (ret != size) {
		touchdbg("Fail to read i2c\n");
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_write
 ****************************************************************************/

static int ist415_i2c_write(struct ist415_dev_s *dev, uint8_t cmd, uint8_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	int wsize = sizeof(cmd) + size;
	uint8_t *buf;
	int ret = 0;

	buf = (uint8_t *)kmm_malloc(wsize);

	buf[0] = cmd;
	if (size > 0) {
		memcpy(buf + sizeof(cmd), data, size);
	}

	ret = i2c_write(i2c, &config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		touchdbg("Fail to write i2c\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_read_da
 ****************************************************************************/

static int ist415_i2c_read_da(struct ist415_dev_s *dev, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	uint8_t cmd = HCOM_SW_DA;
	int wsize = sizeof(cmd) + sizeof(addr);
	int rsize = size * sizeof(uint32_t);
	uint8_t *buf = NULL;
	uint32_t *buf32 = NULL;
	int ret = 0;

	if (size <= 0) {
		touchdbg("Check read da size (%d)\n", size);
		return -EINVAL;
	}

	buf = (uint8_t *)kmm_malloc(wsize);
	buf32 = (uint32_t *)kmm_malloc(rsize);

	buf[0] = cmd;
	put_unaligned_be32(addr, buf + sizeof(cmd));

	ret = i2c_write(i2c, &config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		touchdbg("Fail to read DA\n");
		kmm_free(buf);
		kmm_free(buf32);
		return ret;
	}

	ret = i2c_read(i2c, &config, (uint8_t *) buf32, rsize);
	if (ret != rsize) {
		touchdbg("Fail to read DA\n");
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

static int ist415_i2c_write_da(struct ist415_dev_s *dev, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	uint8_t cmd = HCOM_SW_DA;
	int hsize = sizeof(cmd) + sizeof(addr);
	int wsize = hsize + (size * sizeof(uint32_t));
	uint8_t *buf = NULL;
	int ret = 0;

	if (size <= 0) {
		touchdbg("Check write da size (%d)\n", size);
		return -EINVAL;
	}

	buf = (uint8_t *)kmm_malloc(wsize);

	buf[0] = cmd;
	put_unaligned_be32(addr, buf + sizeof(cmd));
	for (int i = 0; i < size; i++) {
		put_unaligned_be32(data[i], buf + (i * sizeof(uint32_t)) + hsize);
	}

	ret = i2c_write(i2c, &config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		touchdbg("Fail to write DA\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_wait_isp
 ****************************************************************************/

static int ist415_i2c_wait_isp(struct ist415_dev_s *dev)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	uint8_t cmd = HCOM_ISP;
	uint8_t sub1 = ISP_TARGET_STATUS;
	uint8_t sub2 = ISP_CMD_READ;
	uint32_t addr = 0;
	int wsize = sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr);
	uint8_t *buf;
	uint32_t buf32 = ISP_STATUS_BUSY;
	uint32_t status;
	int ret = 0;
	int retry = 200;

	buf = (uint8_t *)kmm_malloc(wsize);

	buf[0] = cmd;
	buf[1] = sub1;
	buf[2] = sub2;
	put_unaligned_be32(addr, buf + sizeof(cmd) + sizeof(sub1) + sizeof(sub2));

	do {
		retry--;
		if (retry == 0) {
			kmm_free(buf);
			touchdbg("Fail to wait ISP\n");
			return ERROR;
		}

		msleep(1);
		ret = i2c_write(i2c, &config, (uint8_t *) buf, wsize);
		if (ret != wsize) {
			continue;
		}
		ret = i2c_read(i2c, &config, (uint8_t *)&buf32, sizeof(status));
		if (ret != sizeof(status)) {
			continue;
		}

		status = cpu_to_be32(buf32);
	} while (status != ISP_STATUS_DONE);

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: ist415_i2c_write_isp
 ****************************************************************************/

int ist415_i2c_write_isp(struct ist415_dev_s *dev, uint8_t sub1, uint8_t sub2, uint32_t addr, uint32_t *data, int size)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	uint8_t cmd = HCOM_ISP;
	int hsize = sizeof(cmd) + sizeof(sub1) + sizeof(sub2) + sizeof(addr);
	int wsize = hsize + (size * sizeof(uint32_t));
	uint8_t *buf = NULL;
	int ret = 0;

	buf = (uint8_t *)kmm_malloc(wsize);

	buf[0] = cmd;
	buf[1] = sub1;
	buf[2] = sub2;
	put_unaligned_be32(addr, buf + sizeof(cmd) + sizeof(sub1) + sizeof(sub2));
	for (int i = 0; i < size; i++) {
		put_unaligned_be32(data[i], buf + (i * sizeof(uint32_t)) + hsize);
	}

	ret = i2c_write(i2c, &config, (uint8_t *) buf, wsize);
	if (ret != wsize) {
		kmm_free(buf);
		touchdbg("Fail to write ISP\n");
		return ret;
	}

	ret = ist415_i2c_wait_isp(dev);
	if (ret < 0) {
		kmm_free(buf);
		touchdbg("Fail to write ISP\n");
		return ret;
	}

	kmm_free(buf);

	return OK;
}

/****************************************************************************
 * Name: touch_interrupt
 ****************************************************************************/
#if defined(CONFIG_TOUCH_POLL)
static void touch_interrupt(struct ist415_dev_s *dev)
{
	FAR struct touchscreen_s *upper = dev->upper;
	irqstate_t state = enter_critical_section();
	dev->int_pending = true;
	leave_critical_section(state);
	if (upper->notify_touch) {
		upper->notify_touch(upper);
	}
}

#elif defined(CONFIG_TOUCH_CALLBACK)

static void touch_interrupt(struct ist415_dev_s *priv)
{
	FAR struct touchscreen_s *upper = priv->upper;
	priv->ops->irq_enable();
	work_queue(HPWORK, &ist415_work, get_touch_data, priv, 0);
}

static void get_touch_data(struct ist415_dev_s *priv)
{
	FAR struct touchscreen_s *upper = priv->upper;
	struct touch_sample_s touch_points;
	int ret = 0;

	if (!upper->app_touch_point_buffer) {
		touchdbg("Application buffer touch data is NULL\n");
		if (upper->is_touch_detected) {
			upper->is_touch_detected(-EINVAL);
			return;
		}
	}

	ist415_run_intr_debug(priv);
	ret = ist415_get_touch_data(priv, upper->app_touch_point_buffer);

	if (upper->is_touch_detected) {
		upper->is_touch_detected(ret);
	}
}

#endif

/****************************************************************************
 * Name: ist415_enable
 ****************************************************************************/

static void ist415_enable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;

	if (priv->enable == true) {
		return;
	}

	touchvdbg("%s\n", __func__);

	priv->enable = true;

	priv->ops->irq_enable();
}

/****************************************************************************
 * Name: ist415_disable
 ****************************************************************************/

static void ist415_disable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;

	if (priv->enable == false) {
		return;
	}

	touchvdbg("%s\n", __func__);

	priv->enable = false;

	priv->ops->irq_disable();
}

/****************************************************************************
 * Name: ist415_power_on
 ****************************************************************************/

static void ist415_power_on(struct touchscreen_s *dev, bool download)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	struct touchscreen_s *upper = priv->upper;
	int ready_wait = 50;

	/*
	 * TODO: Touch Power On Sequence
	 * 1. 3.3v On
	 * 2. If download is true, a delay of 10ms is given.
	 *              Otherwise, a delay of 15ms is given.
	 * 3. Wait ready event
	 */

	touchvdbg("%s\n", __func__);

	priv->ready = false;

	priv->ops->power_on();

	if (download) {
		msleep(10);
	} else {
		msleep(15);
		ist415_enable(upper);

		while ((!priv->ready) && (ready_wait-- > 0)) {
			msleep(1);
		}

		if (!priv->pre_enable) {
			ist415_disable(dev);
		}
	}

	priv->ready = true;
}

/****************************************************************************
 * Name: ist415_power_off
 ****************************************************************************/

static void ist415_power_off(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;

	/*
	 * TODO: Touch Power On Sequence
	 * 1. 3.3v LDO Off
	 */

	touchvdbg("%s\n", __func__);

	priv->pre_enable = priv->enable;

	if (priv->enable) {
		ist415_disable(dev);
	}

	priv->ops->power_off();
}

/****************************************************************************
 * Name: ist415_reset
 ****************************************************************************/

static void ist415_reset(struct touchscreen_s *dev, bool download)
{
	touchvdbg("%s\n", __func__);

	ist415_power_off(dev);
	msleep(100);
	ist415_power_on(dev, download);
}

/****************************************************************************
 * Name: ist415_sensor
 ****************************************************************************/

static void ist415_sensor(struct ist415_dev_s *dev, bool on)
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
		touchdbg("Fail to write sense on/off command\n");
	}

	touchvdbg("%s %s\n", __func__, on ? "on" : "off");

	msleep(50);
}

/****************************************************************************
 * Name: ist415_power_mode
 ****************************************************************************/

static void ist415_power_mode(struct ist415_dev_s *dev, uint8_t data)
{
	int ret = 0;

	ret = ist415_i2c_write(dev, HCOM_CTRL_POWER_MODE, (uint8_t *)&data, 1);
	if (ret) {
		touchdbg("Fail to write power mode command\n");
	}
}

/****************************************************************************
 * Name: ist415_touchtype
 ****************************************************************************/

static void ist415_touchtype(struct ist415_dev_s *dev, uint8_t *data)
{
	int ret = 0;

	ret = ist415_i2c_write(dev, HCOM_CTRL_TOUCH_TYPE, (uint8_t *) data, 2);
	if (ret) {
		touchdbg("Fail to write touchtype command\n");
	}
}

/****************************************************************************
 * Name: ist415_start
 ****************************************************************************/

static void ist415_start(struct ist415_dev_s *dev)
{
	uint8_t mode;
	int ret = 0;

	if (dev->rec_mode) {
		mode = REC_ENABLE;
		ret = ist415_i2c_write(dev, HCOM_SET_REC_MODE, &mode, 1);
		if (ret) {
			touchdbg("Fail to write Recording mode command (Enable)\n");
		} else {
			touchvdbg("Recording mode start\n");
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
			touchdbg("Fail to write Recording mode command (Start scan)\n");
		}
	}

	touchvdbg("%s\n", __func__);
}

/****************************************************************************
 * Name: ist415_run_intr_debug
 ****************************************************************************/

static void ist415_run_intr_debug(struct ist415_dev_s *dev)
{
	uint32_t *buf32;
	int ret = 0;

	if (dev->intr_debug_size <= 0) {
		return;
	}

	buf32 = (uint32_t *)kmm_malloc(dev->intr_debug_size);

	ret = ist415_i2c_read_da(dev, dev->intr_debug_addr, (uint32_t *) buf32, dev->intr_debug_size);
	if (ret) {
		kmm_free(buf32);
		touchdbg("Fail to read intr debug\n");
		return;
	}

	touchvdbg("Intr Debug (Addr : 0x%08X)\n", dev->intr_debug_addr);
	for (int i = 0; i < dev->intr_debug_size; i++) {
		touchvdbg(" %08X\n", buf32[i]);
	}

	kmm_free(buf32);
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

static int ist415_get_touch_data(struct ist415_dev_s *dev, FAR void *buf)
{
	struct ts_event_coordinate *p_evt_coord;
	struct ts_event_status *p_evt_status;
	struct ts_event_gesture *p_evt_gesture;
	struct touch_sample_s *data = buf;
	uint8_t event[EVENT_PACKET_SIZE];
	uint8_t left_e;
	uint8_t *touch_event;
	uint8_t eid;
	uint8_t coord_type;
	bool is_status_palmlarge = false;
	int ret = 0;

	ret = ist415_i2c_read(dev, HCOM_GET_EVENT_1ST, (uint8_t *) event, EVENT_PACKET_SIZE);
	if (ret) {
		touchdbg("Fail to read 1st evnet\n");
		return ret;
	}

	left_e = event[7] & 0x1F;
	touch_event = (uint8_t *)kmm_malloc((left_e + 1) * EVENT_PACKET_SIZE);
	memcpy(touch_event, event, sizeof(event));

	if ((left_e > 0) && (left_e < 0x1F)) {
		ist415_i2c_read(dev, HCOM_GET_EVENT_ALL, (uint8_t *)(touch_event + EVENT_PACKET_SIZE), left_e * EVENT_PACKET_SIZE);
		if (ret) {
			touchdbg("Fail to read left data\n");
			return ret;
		}
	} else if (left_e >= 0x1F) {
		touchdbg("Touch events buffer overflow\n");
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

	data->npoints = 0;
	for (int i = 0; i < (left_e + 1); i++) {
		eid = (touch_event + (i * EVENT_PACKET_SIZE))[0] & 0x3;
		if (eid == EID_COORD) {
			if (dev->pre_palm_state || dev->palm_state) {
				continue;
			}

			p_evt_coord = (struct ts_event_coordinate *)(touch_event + (i * EVENT_PACKET_SIZE));
			if (p_evt_coord->tsta != TOUCH_STA_NONE) {
				data->point[data->npoints].id = p_evt_coord->tid - 1;
				data->point[data->npoints].x = (p_evt_coord->x_11_4 << 4) | p_evt_coord->x_3_0;
				data->point[data->npoints].y = (p_evt_coord->y_11_4 << 4) | p_evt_coord->y_3_0;
				data->point[data->npoints].h = p_evt_coord->major;
				data->point[data->npoints].w = p_evt_coord->minor;
				data->point[data->npoints].pressure = p_evt_coord->z;
				switch (p_evt_coord->tsta) {
				case TOUCH_STA_PRESS:
					data->point[data->npoints].flags = TOUCH_DOWN;
					touchvdbg("COORDDINATES: [P] Point ID=%2d, X=%4d, Y=%4d\n", data->point[data->npoints].id, data->point[data->npoints].x, data->point[data->npoints].y);
					break;
				case TOUCH_STA_MOVE:
					data->point[data->npoints].flags = TOUCH_MOVE;
					touchvdbg("COORDDINATES: [M] Point ID=%2d, X=%4d, Y=%4d\n", data->point[data->npoints].id, data->point[data->npoints].x, data->point[data->npoints].y);
					break;
				case TOUCH_STA_RELEASE:
					data->point[data->npoints].flags = TOUCH_UP;
					touchvdbg("COORDDINATES: [R] Point ID=%2d, X=%4d, Y=%4d\n", data->point[data->npoints].id, data->point[data->npoints].x, data->point[data->npoints].y);
					break;
				default:
					break;
				}

				data->npoints++;
			} else {
				touchwdbg("COORDDINATES: Not Support Coord status (%d)", p_evt_coord->tsta);
			}
		} else if (eid == EID_STATUS) {
			p_evt_status = (struct ts_event_status *)(touch_event + (i * EVENT_PACKET_SIZE));
			switch (p_evt_status->stype) {
			case STS_TYPE_CMD_DRIVEN:
				if (p_evt_status->sid == DRIVEN_CALIBRATION) {
					dev->calib_result = p_evt_status->sdata[0];
					touchvdbg("Calibration Trigger~ (0x%02X)\n", dev->calib_result);
				}
				break;
			case STS_TYPE_INFO_REPORT:
				if (p_evt_status->sid == INFO_TSP_READY) {
					dev->ready = true;
					touchvdbg("TSC Ready~\n");
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
					touchvdbg("SelfTest Trigger~ (0x%02X, 0x%02X)\n", dev->cmcs, dev->cmcs_msg);
				} else if (p_evt_status->sid == VENDOR_REC_REPORT) {
					dev->rec_idle = true;
				}
				break;
			default:
				touchwdbg("Not support status type: %d\n", p_evt_status->stype);
				break;
			}
		} else if (eid == EID_GESTURE) {
			p_evt_gesture = (struct ts_event_gesture *)(touch_event + (i * EVENT_PACKET_SIZE));
			if (p_evt_gesture->gtype == GESTURE_TYPE_DOUBLETAP) {
				if (p_evt_gesture->gid == GESTURE_DOUBLETAP_WAKEUP) {
					uint16_t x = ((uint16_t) p_evt_gesture->gdata[1] << 8) | (uint16_t) p_evt_gesture->gdata[0];
					uint16_t y = ((uint16_t) p_evt_gesture->gdata[3] << 8) | (uint16_t) p_evt_gesture->gdata[2];
					touchvdbg("Double Tap Wakeup~(%d, %d)\n", x, y);
					// TODO: KnockKnock Event Process~
				}
			} else {
				touchwdbg("Not support gesture type:%d\n", p_evt_gesture->gtype);
			}
		} else {
			touchwdbg("Not supprot EID: %d\n", eid);
		}

		if (eid == EID_COORD) {
			if ((dev->pre_palm_state == 0) && dev->palm_state) {
				/*
				 * TODO: PalmLarge On
				 * 1. All Touch Release
				 * 2. PalmLarge On Process
				 */
				touchvdbg("PalmLarge On (T)\n");
			} else if (dev->pre_palm_state && (dev->palm_state == 0)) {
				// TODO: PalmLarge Off Process
				touchvdbg("PalmLarge Off (T)\n");
			}
		} else if ((eid == EID_STATUS) && is_status_palmlarge) {
			if (dev->palm_state == 0) {
				// TODO: PalmLarge Off Process
				touchvdbg("PalmLarge Off (S)\n");
			} else {
				/*
				 * TODO: PalmLarge On
				 * 1. All Touch Release
				 * 2. PalmLarge On Process
				 */
				touchvdbg("PalmLarge On (S)\n");
			}
		}

		dev->pre_palm_state = dev->palm_state ? 1 : 0;
	}

	kmm_free(touch_event);

	return OK;
}

/****************************************************************************
 * Name: is_touchSet
 ****************************************************************************/

static bool ist415_istouchSet(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	return priv->int_pending;
}

/****************************************************************************
 * Name: ist415_read
 ****************************************************************************/

static int ist415_read(struct touchscreen_s *dev, FAR char *buffer)
{
	irqstate_t flags;
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	if (priv->int_pending) {
		flags = enter_critical_section();
		priv->int_pending = false;
		leave_critical_section(flags);
		ist415_run_intr_debug(priv);
		return ist415_get_touch_data(priv, buffer);
	} else {
		touchvdbg("No data to read\n");
	}
	return OK;
}

/****************************************************************************
 * Name: ist415_parse_ver
 ****************************************************************************/

static uint32_t ist415_parse_ver(struct ist415_dev_s *dev, int flag, const uint8_t *buf)
{
	uint32_t ver = 0;
	uint32_t *buf32 = (uint32_t *) buf;

	if (flag == FLAG_MAIN) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xFC) >> 2];
	} else if (flag == FLAG_RELEASE) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xF0) >> 2];
	} else if (flag == FLAG_TEST) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xF4) >> 2];
	} else if (flag == FLAG_CORE) {
		ver = ((uint32_t) buf32[(dev->tags.flag_addr + 0xEC) >> 2] >> 16) & 0xFFFF;
	} else if (flag == FLAG_CONFIG) {
		ver = (uint32_t) buf32[(dev->tags.flag_addr + 0xEC) >> 2] & 0xFFFF;
	} else {
		touchdbg("parsing ver's flag is not corrent\n");
	}

	return ver;
}

/****************************************************************************
 * Name: ist415_parse_tags
 ****************************************************************************/

static int ist415_parse_tags(struct ist415_dev_s *dev, const uint8_t *buf, const uint32_t size)
{
	int ret = -ENOENT;
	struct ist415_tags *tags;

	tags = (struct ist415_tags *)(&buf[size - sizeof(struct ist415_tags)]);

	if (!strncmp(tags->magic1, IST415_TAG_MAGIC, sizeof(tags->magic1)) && !strncmp(tags->magic2, IST415_TAG_MAGIC, sizeof(tags->magic2))) {
		dev->tags = *tags;

		dev->tags.fw_addr -= dev->tags.rom_base;
		dev->tags.cfg_addr -= dev->tags.rom_base;
		dev->tags.sensor_addr -= dev->tags.rom_base;
		dev->tags.flag_addr -= dev->tags.rom_base;

		dev->fw.index = dev->tags.fw_addr;
		dev->fw.size = tags->flag_addr - tags->fw_addr + tags->flag_size;
		dev->fw.chksum = tags->chksum;

		touchvdbg("Tagts magic1: %s, magic2: %s\n", dev->tags.magic1, dev->tags.magic2);
		touchvdbg(" rom: %x\n", dev->tags.rom_base);
		touchvdbg(" ram: %x\n", dev->tags.ram_base);
		touchvdbg(" fw: %x(%x)\n", dev->tags.fw_addr, dev->tags.fw_size);
		touchvdbg(" cfg: %x(%x)\n", dev->tags.cfg_addr, dev->tags.cfg_size);
		touchvdbg(" sensor: %x(%x)\n", dev->tags.sensor_addr, dev->tags.sensor_size);
		touchvdbg(" flag: %x(%x)\n", dev->tags.flag_addr, dev->tags.flag_size);
		touchvdbg(" chksum: %x\n", dev->tags.chksum);
		touchvdbg(" chksum_all: %x\n", dev->tags.chksum_all);

		ret = OK;
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_get_update_info
 ****************************************************************************/

static int ist415_get_update_info(struct ist415_dev_s *dev, const uint8_t *buf, const uint32_t size)
{
	int ret = 0;

	ret = ist415_parse_tags(dev, buf, size);
	if (ret) {
		touchdbg("cannot find tags of firmware\n");
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_read_info
 ****************************************************************************/

static int ist415_read_info(struct ist415_dev_s *dev)
{
	int ret = 0;
	uint8_t panel_info[11];
	uint8_t version[6];
	uint8_t tsp_swap;
	uint8_t recording_info[6];
	uint32_t spec_temp;

	ret = ist415_i2c_read(dev, HCOM_GET_PANEL_INFO, panel_info, sizeof(panel_info));
	if (ret) {
		touchdbg("Fail to get panel info\n");
		return ret;
	}

	dev->width = ((uint16_t) panel_info[0] << 8) | (uint16_t) panel_info[1];
	dev->height = ((uint16_t) panel_info[2] << 8) | (uint16_t) panel_info[3];
	dev->tx_num = panel_info[8];
	dev->rx_num = panel_info[9];
	dev->slf_node_len = dev->tx_num + dev->rx_num;
	dev->mtl_node_len = dev->tx_num * dev->rx_num;

	ret = ist415_i2c_read(dev, HCOM_GET_FW_VER, version, sizeof(version));
	if (ret) {
		touchdbg("Fail to get version\n");
		return ret;
	}

	dev->fw.cur.core_ver = ((uint16_t) version[0] << 8) | (uint16_t) version[1];
	dev->fw.cur.config_ver = ((uint16_t) version[2] << 8) | (uint16_t) version[3];
	dev->fw.cur.release_ver = ((uint16_t) version[4] << 8) | (uint16_t) version[5];

	ret = ist415_i2c_read(dev, HCOM_GET_SWAP_INFO, &tsp_swap, sizeof(tsp_swap));
	if (ret) {
		touchdbg("Fail to get swap info\n");
		return ret;
	}

	dev->swap_xy = (tsp_swap & TSP_INFO_SWAP_XY) ? true : false;

	ret = ist415_i2c_read(dev, HCOM_GET_VER_MAIN, (uint8_t *)&dev->fw.cur.main_ver, sizeof(dev->fw.cur.main_ver));
	if (ret) {
		touchdbg("Fail to get main version\n");
		return ret;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_VER_TEST, (uint8_t *)&dev->fw.cur.test_ver, sizeof(dev->fw.cur.test_ver));
	if (ret) {
		touchdbg("Fail to get test version\n");
		return ret;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_ALGO_STATUS, recording_info, sizeof(recording_info));
	if (ret) {
		touchdbg("Fail to get recording info\n");
		return ret;
	}

	dev->rec_addr = ((uint32_t) recording_info[3] << 24) | ((uint32_t) recording_info[2] << 16) | ((uint32_t) recording_info[1] << 8) | (uint32_t) recording_info[0];
	dev->rec_size = ((uint16_t) recording_info[5] << 8) | (uint16_t) recording_info[4];

	ret = ist415_i2c_read(dev, HCOM_GET_JITTER_GAP_SPEC, (uint8_t *)&spec_temp, sizeof(spec_temp));
	if (ret) {
		touchdbg("Fail to get jitter & gap spec\n");
		return ret;
	}

	dev->spec_gap = spec_temp & 0xFFFF;
	dev->spec_jitter = (spec_temp >> 16) & 0xFFFF;

	ret = ist415_i2c_read(dev, HCOM_GET_CM_SPEC, (uint8_t *)&spec_temp, sizeof(spec_temp));
	if (ret) {
		touchdbg("Fail to get cm spec\n");
		return ret;
	}

	dev->spec_cm_min = spec_temp & 0xFFFF;
	dev->spec_cm_max = (spec_temp >> 16) & 0xFFFF;

	ret = ist415_i2c_read(dev, HCOM_GET_CS_SPEC, (uint8_t *)&spec_temp, sizeof(spec_temp));
	if (ret) {
		touchdbg("Fail to get cs spec\n");
		return ret;
	}

	dev->spec_cs_min = spec_temp & 0xFFFF;
	dev->spec_cs_max = (spec_temp >> 16) & 0xFFFF;

	ret = ist415_i2c_read(dev, HCOM_GET_CR_SPEC, (uint8_t *)&spec_temp, sizeof(spec_temp));
	if (ret) {
		touchdbg("Fail to get cr spec\n");
		return ret;
	}

	dev->spec_cr_min = spec_temp & 0xFFFF;
	dev->spec_cr_max = (spec_temp >> 16) & 0xFFFF;

	return ret;
}

/****************************************************************************
 * Name: ist415_print_info
 ****************************************************************************/

static void ist415_print_info(struct ist415_dev_s *dev)
{
	touchvdbg("*** TSP info ***\n");
	touchvdbg(" swap_xy: %d\n", dev->swap_xy);
	touchvdbg(" tx: %d, rx: %d\n", dev->tx_num, dev->rx_num);
	touchvdbg(" touch width: %d, height: %d\n", dev->width, dev->height);
	touchvdbg(" mutual node len: %d\n", dev->mtl_node_len);
	touchvdbg(" self node len: %d\n", dev->slf_node_len);
	touchvdbg("Main Ver: %X, Test Ver: %X\n", dev->fw.cur.main_ver, dev->fw.cur.test_ver);
	touchvdbg("Core Ver: %X, Config Ver: %X, Release Ver: %X\n", dev->fw.cur.core_ver, dev->fw.cur.config_ver, dev->fw.cur.release_ver);
}

/****************************************************************************
 * Name: ist415_fw_integrity
 ****************************************************************************/

static uint8_t ist415_fw_integrity(struct ist415_dev_s *dev)
{
	uint8_t integrity = 0;
	int ret = 0;

	ret = ist415_i2c_read(dev, HCOM_GET_FW_INTEGRITY, (uint8_t *)&integrity, 1);

	touchvdbg("firmware integrity : 0x%02X (%d)\n", integrity, ret);

	return (ret < 0) ? 0xFF : integrity;
}

/****************************************************************************
 * Name: ist415_wait_calibrate
 ****************************************************************************/

static int ist415_wait_calibrate(struct ist415_dev_s *dev)
{
	int wait = IST415_WAIT_TIME;

	dev->calib_result = 0;
	memset(dev->calib_msg, 0, sizeof(dev->calib_msg));

	while (wait-- > 0) {
		msleep(100);

		if ((dev->calib_result == 0x10) || (dev->calib_result == 0x80)) {
			ist415_i2c_read(dev, HCOM_GET_CAL_RESULT, (uint8_t *) dev->calib_msg, sizeof(dev->calib_msg));
			touchvdbg("MTL Calibration: %d, Max gap : %d - (%08x)\n", CALIB_TO_STATUS(dev->calib_msg[0]), CALIB_TO_GAP(dev->calib_msg[0]), dev->calib_msg[0]);
			touchvdbg("SLF Calibration: %d, Max gap : %d - (%08x)\n", CALIB_TO_STATUS(dev->calib_msg[1]), CALIB_TO_GAP(dev->calib_msg[1]), dev->calib_msg[1]);
		}

		if (dev->calib_result == 0x10) {
			touchvdbg("Calibration success\n");
			return OK;
		} else if (dev->calib_result == 0x80) {
			touchvdbg("Calibration fail\n");
			return -EAGAIN;
		}
	}

	touchvdbg("Calibration time out\n");

	return -ETIME;
}

/****************************************************************************
 * Name: ist415_calibrate
 ****************************************************************************/

static void ist415_calibrate(struct ist415_dev_s *dev, uint8_t retry)
{
	FAR struct touchscreen_s *upper = dev->upper;
	int ret = 0;

	touchvdbg("*** Calibrate %ds ***\n", IST415_WAIT_TIME / 10);

	while (retry-- > 0) {
		ist415_disable(upper);
		ist415_reset(upper, false);
		ist415_enable(upper);

		ret = ist415_i2c_write(dev, HCOM_CALIBRATION, NULL, 0);
		if (ret) {
			touchdbg("Fail to write calibration commnad\n");
			continue;
		}
		ret = ist415_wait_calibrate(dev);
		if (ret == OK) {
			break;
		}
	}

	ist415_reset(upper, false);
}

/****************************************************************************
 * Name: request_firmware
 ****************************************************************************/

static int request_firmware(struct firmware *firmware, char *path, struct ist415_dev_s **dev)
{
	int fd;
	int ret = 0;
	struct stat file_stat;

	if (firmware == NULL || path == NULL) {
		return ERROR;
	}

	if (stat(path, &file_stat) != 0) {
		dbg("Failed to stat file: %d\n", get_errno());
		return ERROR;
	}

	firmware->size = file_stat.st_size;
	firmware->data = (char *)kmm_malloc(firmware->size);

	fd = open(path, O_RDONLY);
	if (fd < 0) {
		dbg("Failed to open file: %d\n", get_errno());
		return ERROR;
	}

	ret = read(fd, firmware->data, firmware->size);
	if (ret != firmware->size) {
		dbg("Failed to read file: %d\n", get_errno());
		close(fd);
		return ERROR;
	}
	close(fd);
	return OK;
}

static void release_firmware(struct firmware *firmware)
{
	firmware->size = 0;
	kmm_free(firmware->data);

	dbg("firmware released \n");
}

/****************************************************************************
 * Name: ist415_isp_enable
 ****************************************************************************/

static int ist415_isp_enable(struct ist415_dev_s *dev, bool enable)
{
	int ret = 0;

	if (enable) {
		ret = ist415_i2c_write_isp(dev, ISP_TARGET_ENTER, ISP_CMD_ENTER, 0, NULL, 0);
	} else {
		ret = ist415_i2c_write_isp(dev, ISP_TARGET_EXIT, ISP_CMD_EXIT, 0, NULL, 0);
	}

	if (ret < 0) {
		touchdbg("Fail to isp %s\n", enable ? "enable" : "disable");
		return ret;
	}

	return OK;
}

/****************************************************************************
 * Name: ist415_isp_erase
 ****************************************************************************/

static int ist415_isp_erase(struct ist415_dev_s *dev, uint8_t target, uint8_t area, uint32_t addr, int size)
{
	int count = 0;
	int unit = 0;
	int ret = 0;
	int i;

	touchvdbg("%s\n", __func__);

	if (area == ISP_CMD_MASS_ERASE) {
		count = 1;
	} else if (area == ISP_CMD_SECTOR_ERASE) {
		count = size / IST415_ROM_SECTOR_SIZE;
		unit = IST415_ROM_SECTOR_SIZE;
	} else if (area == ISP_CMD_PAGE_ERASE) {
		count = size / IST415_ROM_PAGE_SIZE;
		unit = IST415_ROM_PAGE_SIZE;
	}

	for (i = 0; i < count; i++) {
		ret = ist415_i2c_write_isp(dev, target, area, addr + (i * unit), NULL, 0);
		if (ret < 0) {
			touchdbg("Fail to erase ISP\n");
		}
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_isp_program
 ****************************************************************************/

static int ist415_isp_program(struct ist415_dev_s *dev, uint8_t target, uint32_t addr, uint32_t *buf32, int size)
{
	int ret = 0;
	int i;

	touchvdbg("%s\n", __func__);

	for (i = 0; i < (size / IST415_ROM_SECTOR_SIZE); i++) {
		ret = ist415_i2c_write_isp(dev, target, ISP_CMD_PROG, addr, buf32, IST415_ROM_SECTOR_SIZE / sizeof(uint32_t));
		if (ret < 0) {
			touchdbg("Fail to write ISP (Sector %d)\n", i);
			break;
		}
		addr += IST415_ROM_SECTOR_SIZE;
		buf32 += (IST415_ROM_SECTOR_SIZE / sizeof(uint32_t));
	}

	return ret;
}

/****************************************************************************
 * Name: ist415_isp_fw_update
 ****************************************************************************/

static int ist415_isp_fw_update(struct ist415_dev_s *dev, const uint8_t *buf)
{
	FAR struct touchscreen_s *upper = dev->upper;
	int ret = 0;

	touchvdbg("%s\n", __func__);

	ist415_reset(upper, true);

	ret = ist415_isp_enable(dev, true);
	if (ret < 0) {
		return ret;
	}

	ret = ist415_isp_erase(dev, ISP_TARGET_MAIN, ISP_CMD_SECTOR_ERASE, IST415_ROM_BASE_ADDR, IST415_ROM_MAIN_SIZE);
	if (ret < 0) {
		return ret;
	}

	msleep(1);

	ret = ist415_isp_program(dev, ISP_TARGET_MAIN, IST415_ROM_BASE_ADDR, (uint32_t *) buf, IST415_ROM_MAIN_SIZE);
	if (ret < 0) {
		touchdbg("Fail to program ISP\n");
		return ret;
	}

	ist415_reset(upper, false);

	return ret;
}

/****************************************************************************
 * Name: ist415_check_auto_update
 ****************************************************************************/

int ist415_check_auto_update(struct ist415_dev_s *dev)
{
	FAR struct touchscreen_s *upper = dev->upper;
	int retry = IST415_RETRY_CNT;
	bool tsp_check = false;
	uint32_t chksum = 0;
	struct ist415_fw *fw = &dev->fw;
	int ret = 0;

	while (retry--) {
		ret = ist415_fw_integrity(dev);
		if (ret == FW_INTEGRITY_VALID) {
			tsp_check = true;
			break;
		} else if (ret == FW_INTEGRITY_INVALID) {
			break;
		}

		ist415_reset(upper, false);
	}

	if (!tsp_check) {
		return -EAGAIN;
	}

	ret = ist415_read_info(dev);
	if (ret < 0) {
		return -EAGAIN;
	}

	if ((fw->cur.release_ver > 0) && (fw->cur.release_ver < 0xFFFF)) {
		if (((fw->cur.main_ver & MAIN_VER_MASK) == MAIN_VER_MASK) || ((fw->cur.main_ver & MAIN_VER_MASK) == 0)) {
			return -EAGAIN;
		}

		touchvdbg("Version compare IC: %x(%x, %x), BIN: %x(%x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver);

		/* If FW version is same, check FW checksum */
		if ((fw->cur.core_ver == fw->bin.core_ver) && (fw->cur.config_ver == fw->bin.config_ver) && (fw->cur.release_ver == fw->bin.release_ver) && (fw->cur.test_ver == 0)) {
			ret = ist415_i2c_read(dev, HCOM_GET_CRC, (uint8_t *)&chksum, 2);
			if (ret || (chksum != fw->chksum)) {
				touchdbg("Mismatch checksum IC: %x, Bin: %x (ret: %d)\n", chksum, fw->chksum, ret);
				return -EAGAIN;
			}
		}

		/* If the ver of binary is higher than ver of IC, FW update operate. */
		if ((fw->cur.core_ver >= fw->bin.core_ver) && (fw->cur.config_ver >= fw->bin.config_ver) && (fw->cur.release_ver >= fw->bin.release_ver)) {
			return OK;
		}
	}

	return -EAGAIN;
}

/****************************************************************************
 * Name: ist415_fw_update
 ****************************************************************************/

static int ist415_fw_update(struct ist415_dev_s *dev, const uint8_t *buf, int size)
{
	FAR struct touchscreen_s *upper = dev->upper;
	struct ist415_fw *fw = &dev->fw;
	uint32_t core_ver = ist415_parse_ver(dev, FLAG_CORE, buf);
	uint32_t config_ver = ist415_parse_ver(dev, FLAG_CONFIG, buf);
	uint32_t release_ver = ist415_parse_ver(dev, FLAG_RELEASE, buf);
	uint32_t test_ver = ist415_parse_ver(dev, FLAG_TEST, buf);
	uint32_t chksum = 0;
	int ret = 0;

	touchvdbg("*** Firmware update ***\n");
	touchvdbg("core: %x, config: %x, release: %x, test: %x(addr: 0x%x ~ 0x%x)\n", core_ver, config_ver, release_ver, test_ver, fw->index, fw->index + fw->size);

	ret = ist415_isp_fw_update(dev, buf);
	if (ret < 0) {
		touchdbg("Fail to update firmware\n");
		goto reset;
	}

	ret = ist415_i2c_read(dev, HCOM_GET_CRC, (uint8_t *)&chksum, 2);
	if (ret < 0) {
		touchdbg("Fail to read checksum\n");
		goto reset;
	}

	if (chksum != fw->chksum) {
		touchdbg("Mismatch checksum IC: %x, Bin: %x\n", chksum, fw->chksum);
		goto reset;
	}

	ret = ist415_read_info(dev);
	if (ret < 0) {
		touchdbg("Fail to read info\n");
		goto reset;
	}

	touchvdbg("Firmware update success\n");

	return OK;

reset:

	ist415_reset(upper, false);

	return ret;
}

/****************************************************************************
 * Name: ist415_check_fw
 ****************************************************************************/

static int ist415_check_fw(struct ist415_dev_s *dev)
{
	struct firmware firmware;
	struct ist415_fw *fw = &dev->fw;
	int retry = 0;
	int ret = 0;

	ret = request_firmware(&firmware, FIRMWARE_PATH_WASHING_MACHINE, &dev);
	if (ret < 0) {
		touchdbg("Don't request firmware\n");
		return -ENOENT;
	}

	fw->buf = (uint8_t *)kmm_malloc(firmware.size);
	if (!fw->buf) {
		release_firmware(&firmware);
		touchdbg("Failed to allocate buffer\n");
		return -ENOMEM;
	} else {
		memcpy(fw->buf, firmware.data, firmware.size);
		fw->buf_size = firmware.size;
	}

	release_firmware(&firmware);

	touchvdbg("Firmware loaded successfully.\n");

	ret = ist415_get_update_info(dev, fw->buf, fw->buf_size);
	if (ret < 0) {
		kmm_free(fw->buf);
		return ret;
	}

	fw->bin.main_ver = ist415_parse_ver(dev, FLAG_MAIN, fw->buf);
	fw->bin.release_ver = ist415_parse_ver(dev, FLAG_RELEASE, fw->buf);
	fw->bin.test_ver = ist415_parse_ver(dev, FLAG_TEST, fw->buf);
	fw->bin.core_ver = ist415_parse_ver(dev, FLAG_CORE, fw->buf);
	fw->bin.config_ver = ist415_parse_ver(dev, FLAG_CONFIG, fw->buf);

	touchvdbg("Main : %08X Core: %x, Config %x, Release: %x, Test: %x\n", fw->bin.main_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.release_ver, fw->bin.test_ver);

	ret = ist415_check_auto_update(dev);
	if (ret >= 0) {
		kmm_free(fw->buf);
		return OK;
	}

	touchvdbg("Update version. release(core, config, test): %x(%x, %x, %x) -> %x(%x, %x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->cur.test_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.test_ver);

	retry = IST415_RETRY_CNT;
	while (retry--) {
		ret = ist415_fw_update(dev, fw->buf, fw->buf_size);
		if (ret == OK) {
			//ist415_calibrate(dev, IST415_RETRY_CNT);
			break;
		}
	}

	kmm_free(fw->buf);
	return ret;
}

/****************************************************************************
 * Name: ist415_get_info
 ****************************************************************************/

static int ist415_get_info(struct ist415_dev_s *dev)
{
	int ret = 0;

	ret = ist415_read_info(dev);
	if (ret) {
		return ret;
	}

	ist415_print_info(dev);

	return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ist415_close
 ****************************************************************************/

void ist415_close(struct ist415_dev_s *dev)
{
	FAR struct touchscreen_s *upper = dev->upper;

	if (dev->knockknock) {
		dev->sys_mode = SYS_MODE_LPM;
		ist415_power_mode(dev, dev->sys_mode);
	} else {
		ist415_disable(upper);
		ist415_power_off(upper);
	}
}

/****************************************************************************
 * Name: ist415_open
 ****************************************************************************/

void ist415_open(struct ist415_dev_s *dev)
{
	FAR struct touchscreen_s *upper = dev->upper;

	if (dev->knockknock) {
		dev->sys_mode = SYS_MODE_TOUCH;
		ist415_power_mode(dev, dev->sys_mode);
	} else {
		ist415_reset(upper, false);
		ist415_enable(upper);
		ist415_start(dev);
	}
}

/****************************************************************************
 * Name: ist415_autocalibration
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_autocalibration(struct ist415_dev_s *dev)
{
	FAR struct touchscreen_s *upper = dev->upper;

	ist415_calibrate(dev, 1);
	ist415_start(dev);
	ist415_enable(upper);
}

/****************************************************************************
 * Name: ist415_set_intr_debug
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_set_intr_debug(struct ist415_dev_s *dev, uint32_t addr, int size)
{
	dev->intr_debug_addr = addr;
	dev->intr_debug_size = size;

	touchvdbg("intr debug addr: 0x%x, size: %d\n", addr, size);
}

/****************************************************************************
 * Name: ist415_display_rawdata
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_display_rawdata(struct ist415_dev_s *dev)
{
	uint32_t *bufslf;
	uint32_t *bufmtl;
	int tx, rx;
	uint32_t cdc, baseline, diff;
	int ret = 0;

	bufslf = (uint32_t *)kmm_malloc((int)dev->slf_node_len * sizeof(uint32_t));
	bufmtl = (uint32_t *)kmm_malloc((int)dev->mtl_node_len * sizeof(uint32_t));

	ist415_sensor(dev, false);

	touchvdbg("SLF addr: %08X, size : %d\n", SLF_CDC_REGISTER, (int)dev->slf_node_len);
	ret = ist415_i2c_read_da(dev, SLF_CDC_REGISTER, (uint32_t *) bufslf, (int)dev->slf_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		touchdbg("Fail to read self raw data\n");
		return;
	}

	touchvdbg("MTL addr: %08X, size : %d\n", MTL_CDC_REGISTER, (int)dev->mtl_node_len);
	ret = ist415_i2c_read_da(dev, MTL_CDC_REGISTER, (uint32_t *) bufmtl, (int)dev->mtl_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		touchdbg("Fail to read mutual raw data\n");
		return;
	}

	ist415_sensor(dev, true);

	touchvdbg("[CDC]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cdc = bufslf[tx] & 0xFFF;
					touchvdbg(" %4d", cdc);
				}
				touchvdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					touchvdbg(" %4d", cdc);
					touchvdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				touchvdbg(" %4d", cdc);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					touchvdbg(" %4d", cdc);
				}
				touchvdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cdc = bufslf[tx] & 0xFFF;
					touchvdbg(" %4d", cdc);
					touchvdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				touchvdbg(" %4d", cdc);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	}

	touchvdbg("[BASELINE]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					touchvdbg(" %4d", baseline);
				}
				touchvdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					touchvdbg(" %4d", baseline);
					touchvdbg(" %4s", " ");
				}

				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				touchvdbg(" %4d", baseline);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					touchvdbg(" %4d", baseline);
				}
				touchvdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					touchvdbg(" %4d", baseline);
					touchvdbg(" %4s", " ");
				}

				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				touchvdbg(" %4d", baseline);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	}

	touchvdbg("[DIFF]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cdc = bufslf[tx] & 0xFFF;
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					touchvdbg(" %4d", diff);
				}
				touchvdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					touchvdbg(" %4d", diff);
					touchvdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				diff = cdc - baseline;
				touchvdbg(" %4d", diff);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cdc = bufslf[dev->tx_num + rx] & 0xFFF;
					baseline = (bufslf[dev->tx_num + rx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					touchvdbg(" %4d", diff);
				}
				touchvdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cdc = bufslf[tx] & 0xFFF;
					baseline = (bufslf[tx] >> 16) & 0xFFF;
					diff = cdc - baseline;
					touchvdbg(" %4d", diff);
					touchvdbg(" %4s", " ");
				}

				cdc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				baseline = (bufmtl[(tx * dev->rx_num) + rx] >> 16) & 0xFFF;
				diff = cdc - baseline;
				touchvdbg(" %4d", diff);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	}

	kmm_free(bufslf);
	kmm_free(bufmtl);
}

/****************************************************************************
 * Name: ist415_display_cpc
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

void ist415_display_cpc(struct ist415_dev_s *dev)
{
	uint32_t *bufslf;
	uint32_t *bufmtl;
	int tx, rx;
	uint32_t cpc;
	int ret = 0;

	bufslf = (uint32_t *)kmm_malloc(dev->slf_node_len * sizeof(uint32_t));
	bufmtl = (uint32_t *)kmm_malloc(dev->mtl_node_len * sizeof(uint32_t));

	ist415_sensor(dev, false);

	touchvdbg("SLF addr: %08X, size : %d\n", SLF_CPC_REGISTER, dev->slf_node_len);
	ret = ist415_i2c_read_da(dev, SLF_CPC_REGISTER, (uint32_t *) bufslf, dev->slf_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		touchdbg("Fail to read self cpc data\n");
		return;
	}

	touchvdbg("MTL addr: %08X, size : %d\n", MTL_CPC_REGISTER, dev->mtl_node_len);
	ret = ist415_i2c_read_da(dev, MTL_CPC_REGISTER, (uint32_t *) bufmtl, dev->mtl_node_len);
	if (ret) {
		kmm_free(bufslf);
		kmm_free(bufmtl);
		touchdbg("Fail to read mutual cpc data\n");
		return;
	}

	ist415_sensor(dev, true);

	touchvdbg("[CPC]\n");
	if (dev->swap_xy) {
		for (rx = 0; rx < dev->rx_num; rx++) {
			if (rx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (tx = 0; tx < dev->tx_num; tx++) {
					cpc = bufslf[tx] & 0xFFF;
					touchvdbg(" %4d", cpc);
				}
				touchvdbg("\n\n");
			}

			for (tx = 0; tx < dev->tx_num; tx++) {
				if (tx == 0) {
					cpc = bufslf[dev->tx_num + rx] & 0xFFF;
					touchvdbg(" %4d", cpc);
					touchvdbg(" %4s", " ");
				}

				cpc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				touchvdbg(" %4d", cpc);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	} else {
		for (tx = 0; tx < dev->tx_num; tx++) {
			if (tx == 0) {
				touchvdbg(" %4s", " ");
				touchvdbg(" %4s", " ");
				for (rx = 0; rx < dev->rx_num; rx++) {
					cpc = bufslf[dev->tx_num + rx] & 0xFFF;
					touchvdbg(" %4d", cpc);
				}
				touchvdbg("\n\n");
			}

			for (rx = 0; rx < dev->rx_num; rx++) {
				if (rx == 0) {
					cpc = bufslf[tx] & 0xFFF;
					touchvdbg(" %4d", cpc);
					touchvdbg(" %4s", " ");
				}

				cpc = bufmtl[(tx * dev->rx_num) + rx] & 0xFFF;
				touchvdbg(" %4d", cpc);
			}
			touchvdbg("\n");
		}
		touchvdbg("\n");
	}

	kmm_free(bufslf);
	kmm_free(bufmtl);
}

/****************************************************************************
 * Name: ist415_force_update
 *
 * Description
 *   This function is called by Uart Command.
 ****************************************************************************/

int ist415_force_update(struct ist415_dev_s *dev, bool cal)
{
	struct touchscreen_s *upper = dev->upper;
	struct firmware firmware;
	struct ist415_fw *fw = &dev->fw;

	int ret = request_firmware(&firmware, FIRMWARE_PATH_FORCE, &dev);
	if (ret < 0) {
		touchdbg("do not request firmware\n");
		return -ENOENT;
	}

	fw->buf = (uint8_t *)kmm_malloc(firmware.size);
	if (!fw->buf) {
		release_firmware(&firmware);
		touchdbg("Fail to allocate memory for firmware\n");
		return -ENOMEM;
	} else {
		memcpy(fw->buf, firmware.data, firmware.size);
		fw->buf_size = firmware.size;
	}

	release_firmware(&firmware);

	touchvdbg("Firmware loaded successfully.\n");

	ret = ist415_get_update_info(dev, fw->buf, fw->buf_size);
	if (ret < 0) {
		kmm_free(fw->buf);
		return ret;
	}

	fw->bin.main_ver = ist415_parse_ver(dev, FLAG_MAIN, fw->buf);
	fw->bin.release_ver = ist415_parse_ver(dev, FLAG_RELEASE, fw->buf);
	fw->bin.test_ver = ist415_parse_ver(dev, FLAG_TEST, fw->buf);
	fw->bin.core_ver = ist415_parse_ver(dev, FLAG_CORE, fw->buf);
	fw->bin.config_ver = ist415_parse_ver(dev, FLAG_CONFIG, fw->buf);

	touchvdbg("Update version. release(core, config, test): %x(%x, %x, %x) -> %x(%x, %x, %x)\n", fw->cur.release_ver, fw->cur.core_ver, fw->cur.config_ver, fw->cur.test_ver, fw->bin.release_ver, fw->bin.core_ver, fw->bin.config_ver, fw->bin.test_ver);

	ist415_disable(upper);

	ret = ist415_fw_update(dev, fw->buf, fw->buf_size);
	if (ret == OK) {
		if (cal) {
			ist415_calibrate(dev, 1);
		}
	}

	ist415_start(dev);
	ist415_enable(upper);

	kmm_free(fw->buf);

	return ret;
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

int ist415_initialize(const char *path, struct ist415_dev_s *dev)
{
	int ret = 0;

	dev->ops->irq_disable();

	struct touchscreen_s *upper = (struct touchscreen_s *)kmm_malloc(sizeof(struct touchscreen_s));
	upper->ops = &g_ist415_ops;
	upper->priv = dev;

	dev->upper = upper;
	dev->handler = touch_interrupt;

	dev->ready = false;
	dev->enable = false;
	dev->pre_enable = false;

	dev->sys_mode = SYS_MODE_TOUCH;
	dev->touch_type = (1 << TOUCH_TYPE_NORMAL) | (1 << TOUCH_TYPE_WET) | (1 << TOUCH_TYPE_PALMLARGE);
	dev->knockknock = true;

	dev->pre_palm_state = 0;
	dev->palm_state = 0;

	dev->rec_mode = 0;
	dev->rec_addr = 0;
	dev->rec_size = 0;

	dev->cmcs = 0;

	dev->intr_debug_addr = 0;
	dev->intr_debug_size = 0;

	ist415_disable(upper);

	/* Init System Power */
	ist415_reset(upper, false);

	// check to need firmwar update
	ret = ist415_check_fw(dev);
	if (ret) {
		touchdbg("Fail to check firmware\n");
		return ret;
	}

	// get to touch info
	ret = ist415_get_info(dev);
	if (ret) {
		touchdbg("Fail to get info\n");
		return ret;
	} else {
		touchvdbg("Get info success\n");
	}

	ist415_start(dev);
	ist415_enable(upper);

	touchvdbg("Touch Driver registered Successfully\n");
	return touch_register(path, upper);
}
