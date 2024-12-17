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

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#define EVENT_PACKET_SIZE 16

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void ist415_enable(struct touchscreen_s *dev);
static void ist415_disable(struct touchscreen_s *dev);

#if defined(CONFIG_TOUCH_CALLBACK)
static void get_touch_data(struct ist415_dev_s *priv);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct touchscreen_ops_s g_ist415_ops = {
	.touch_enable = ist415_enable,
	.touch_disable = ist415_disable,
};

#if defined(CONFIG_TOUCH_CALLBACK)
static struct work_s ist415_work;
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: ist415_get_touch_data
 ****************************************************************************/

static int ist415_process_event(struct ist415_dev_s *dev)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	struct ts_event_coordinate *p_evt_coord;
	struct touch_point_s point;
	u8 event[EVENT_PACKET_SIZE];
	u8 touch_point;
	u8 *touch_event;
	u8 eid;
	int reg[2] = {HCOM_GET_EVENT_1ST, 0};

	int ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret != 1) {
		touchdbg("ERROR: I2C write failed\n");
		return ret;
	}
	ret = i2c_read(i2c, &config, (uint8_t *)event, EVENT_PACKET_SIZE);
	if (ret != EVENT_PACKET_SIZE) {
		touchdbg("ERROR: I2C read failed\n");
		return ret;
	}
	touch_point = event[7] & 0x1F;
	touch_event = (u8 *)kmm_malloc((touch_point + 1) * EVENT_PACKET_SIZE);
	memcpy(touch_event, event, sizeof(event));
	if ((touch_point > 0) && (touch_point < 0x1F)) {
		reg[0] = HCOM_GET_EVENT_ALL;
		ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
		if (ret != 1) {
			touchdbg("ERROR: I2C write failed\n");
			return ret;
		}
		ret = i2c_read(i2c, &config, touch_event + EVENT_PACKET_SIZE, touch_point * EVENT_PACKET_SIZE);
		if (ret != touch_point * EVENT_PACKET_SIZE) {
			touchdbg("ERROR: I2C read failed\n");
			return ret;
		}
	} else if (touch_point >= 0x1F) {
		touchdbg("ERROR:touch events bufer overflow\n");
		return -EIO;
	}
	touchvdbg("touch_point %d\n", touch_point + 1);

	for (int i = 0; i < (touch_point + 1); i++) {
		eid = (touch_event + (i * EVENT_PACKET_SIZE))[0] & 0x3;
		if (eid == EID_COORD) {
			p_evt_coord = (struct ts_event_coordinate *)(touch_event + (i * EVENT_PACKET_SIZE));
			point.id = p_evt_coord->tid - 1;
			point.x  = (p_evt_coord->x_11_4 << 4) | p_evt_coord->x_3_0;
			point.y  = (p_evt_coord->y_11_4 << 4) | p_evt_coord->y_3_0;
			point.h = 0;
			point.w = 0;
			point.pressure = 0;
			switch (p_evt_coord->tsta) {
			case 1:
				point.flags = TOUCH_DOWN;
				break;
			case 2:
				point.flags = TOUCH_MOVE;
				break;
			case 3:
				point.flags = TOUCH_UP;
				break;
			}
			touchvdbg("COORDINATES: id %d status %d type %d x : %d y : %d\n", point.id, p_evt_coord->tsta, (p_evt_coord->ttype_3_2 << 2) | p_evt_coord->ttype_1_0, point.x, point.y);
			touch_report(dev->upper, point);
		}
	}
	return OK;
}

/****************************************************************************
 * Name: ist415_enable
 ****************************************************************************/

static void ist415_enable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	priv->ops->irq_enable();
}

/****************************************************************************
 * Name: ist415_disable
 ****************************************************************************/

static void ist415_disable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	priv->ops->irq_disable();
}

/****************************************************************************
 * Name: touch_interrupt
 ****************************************************************************/

static void touch_interrupt(struct ist415_dev_s *dev)
{
	sem_post(&dev->wait_irq);
}


/****************************************************************************
 * Name: ist415_irq_thread
 ****************************************************************************/

static int ist415_irq_thread(int argc, char **argv)
{
	struct ist415_dev_s *dev;

	DEBUGASSERT(argc == 2);
	dev = (struct ist415_dev_s *)strtoul(argv[1], NULL, 16);

	while (1) {
		sem_wait(&dev->wait_irq);
		if (ist415_process_event(dev) != OK) {
			touchdbg("Fail to process event\n");
		}
	}
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

int ist415_initialize(const char*path, struct ist415_dev_s *priv)
{
	int ret = 0;
	char *parm[2];
	char parm_buf[9];
	uint8_t reg[1];

	priv->ops->irq_disable();

	reg[0] = 0x23;
	ret = i2c_write(priv->i2c, &(priv->i2c_config), (uint8_t *)reg, 1);
	if (ret < OK) {
		touchdbg("ERROR: i2c_write failed\n");
		return NULL;
	}

	sem_init(&priv->wait_irq, 0, 0);
	itoa((int)priv, parm_buf, 16);
	parm[0] = parm_buf;
	parm[1] = NULL;
	priv->pid = kernel_thread("ist415_isr", CONFIG_IST415_WORKPRIORITY , 2048, (main_t)ist415_irq_thread, (FAR char *const *)parm);

	struct touchscreen_s *upper = (struct touchscreen_s *)kmm_zalloc(sizeof(struct touchscreen_s));
	upper->ops = &g_ist415_ops;
	upper->priv = priv;
#if defined(CONFIG_TOUCH_CALLBACK)
	upper->is_touch_detected = NULL;		/* Callback function will be set by UI */
	upper->app_touch_point_buffer = NULL; /* Buffer to store touch point data, set by UI */
#endif
	priv->upper = upper;
	priv->handler = touch_interrupt;

	touchvdbg("Touch Driver registered Successfully\n");
	return touch_register(path, upper);
}
