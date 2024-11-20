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

#include <tinyara/wqueue.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

#define EVENT_PACKET_SIZE 16
#define TIMEOUT (2*CLK_TCK)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int ist415_read(struct touchscreen_s *dev, FAR char *buffer);
static void ist415_enable(struct touchscreen_s *dev);
static void ist415_disable(struct touchscreen_s *dev);
static int ist415_firmware_update(struct touchscreen_s *dev);
static bool ist415_istouchSet(struct touchscreen_s *dev);
static void ist415_check_status(FAR void *arg);
static int ist415_i2c_readword(struct ist415_dev_s *priv, u32 regaddr, u32 *pdata, u32 len);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct touchscreen_ops_s g_ist415_ops = {
	.touch_read = ist415_read,
	.touch_enable = ist415_enable,
	.touch_disable = ist415_disable,
	.firmware_update = ist415_firmware_update,
	.is_touchSet = ist415_istouchSet,
};

static struct work_s ist415_status_work;
/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: ist415_i2c_readword
 ****************************************************************************/
static int ist415_i2c_readword(struct ist415_dev_s *priv, u32 regaddr, u32 *pdata, u32 len)
{
	s32 ret;
	u32 i, j;
	u8 *senddata;
	u32 senddatalen;
	u8 cmd = HCOM_SW_DA;
	u32 bytelen = (len * sizeof(u32));
	u8 *pbuf;
	u32 tmpdata;
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	if (len == 0)
		return 0;
	pbuf = (u8 *)kmm_calloc(bytelen, sizeof(u8));
	if (pbuf == NULL) {
		printf("mem alloc fail !!\r\n");
		return -1;
	}
	senddatalen = sizeof(cmd) + sizeof(regaddr);
	senddata = (u8 *)kmm_calloc(senddatalen, sizeof(u8));
	if (senddata == NULL) {
		free(pbuf);
		senddata = NULL;
		printf("mem alloc fail !!\r\n");
		return -1;
	}
	*senddata = cmd;
	// ### Align the data for register address.
	for (i = 0; i < sizeof(regaddr); i++) {
		*(senddata + sizeof(cmd) + i) = (u8)((regaddr >> (((sizeof(u32) - 1) - i) * 8)) & 0xFF);
	}
	// ### I2C repeat start processing.
	ret = i2c_write(i2c, &config, senddata, senddatalen);
	if (ret != senddatalen) {
		printf("i2c write fail !!\r\n");
		return -1;
	} else {
		ret = i2c_read(i2c, &config, pbuf, bytelen);
		if (ret != bytelen) {
			printf("i2c read fail !!\r\n");
			return -1;
		}
		// ### Align the data.
		for (i = 0; i < len; i++) {
			tmpdata = 0;
			for(j=0; j<sizeof(u32); j++) {
				tmpdata |= (u32)(*(pbuf + ((i * sizeof(u32)) + j)) << (((sizeof(u32) - 1) - j) * 8));
			}
			*(pdata+i) = tmpdata;
		}
	}
	if (pbuf) {
		free(pbuf);
		pbuf = NULL;
	}
	if (senddata) {
		free(senddata);
		senddata = NULL;
	}
	return ret;
}
static int ct = 0;
static void status_timerfn(int argc, uint32_t arg, ...)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)arg;
	//lldbg("in timer fn....\n");
        work_queue(HPWORK, &ist415_status_work, ist415_check_status, (void *)priv, 0);
}

/****************************************************************************
 * Name: ist415_check_status
 ****************************************************************************/
static void ist415_check_status(FAR void *arg)
{
	u32 touch_status = 0;
	ct += 5;
	struct ist415_dev_s *priv = (struct ist415_dev_s *)arg;
	while (sem_wait(&priv->status_check_sem) != 0) {
                ASSERT(errno == EINTR);
        }	
        int ret = ist415_i2c_readword(priv, IST415X_TOUCH_STATUS, &touch_status, 1);
        if ((touch_status & TOUCH_STATUS_MASK) != TOUCH_STATUS_MAGIC) {
                lldbg("Touch status is not corrected (0x%08x)\n", touch_status);
		priv->ops->reset();
		sem_post(&priv->status_check_sem);
        } else {
		sem_post(&priv->status_check_sem);
		lldbg("touch is working correctly\n");
		int ret = wd_start(priv->wdog, TIMEOUT, status_timerfn, 1,  (uint32_t)priv);
                if (ret != OK) {
			wd_cancel(priv->wdog);
		}
	}
}

/****************************************************************************
 * Name: ist415_get_touch_data
 ****************************************************************************/
static int ist415_get_touch_data(struct ist415_dev_s *dev, FAR void *buf)
{
	struct i2c_dev_s *i2c = dev->i2c;
	struct i2c_config_s config = dev->i2c_config;
	struct ts_event_coordinate *p_evt_coord;
	struct touch_sample_s *data = buf;
	u8 event[EVENT_PACKET_SIZE];
	u8 touch_point;
	u8 *touch_event;
	u8 eid;
	printf("Alive check seconds %d\n", ct*2);
	int reg[2] = {HCOM_GET_EVENT_1ST, 0};
	while (sem_wait(&dev->status_check_sem) != 0) {
		ASSERT(errno == EINTR);
        }
	int ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret != 1) {
		touchdbg("ERROR: I2C write failed\n");
		goto errout_touch;
	}
	ret = i2c_read(i2c, &config, (uint8_t *)event, EVENT_PACKET_SIZE);
	if (ret != EVENT_PACKET_SIZE) {
		touchdbg("ERROR: I2C read failed\n");
		goto errout_touch;
	}
	touch_point = event[7] & 0x1F;
	touch_event = (u8 *)kmm_malloc((touch_point + 1) * EVENT_PACKET_SIZE);
	memcpy(touch_event, event, sizeof(event));
	if ((touch_point > 0) && (touch_point < 0x1F)) {
		reg[0] = HCOM_GET_EVENT_ALL;
		ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
		if (ret != 1) {
			touchdbg("ERROR: I2C write failed\n");
			goto errout_touch;
		}
		ret = i2c_read(i2c, &config, touch_event + EVENT_PACKET_SIZE, touch_point * EVENT_PACKET_SIZE);
		if (ret != touch_point * EVENT_PACKET_SIZE) {
			touchdbg("ERROR: I2C read failed\n");
			goto errout_touch;
		}
	} else if (touch_point >= 0x1F) {
		touchdbg("ERROR:touch events bufer overflow\n");
		ret = -EIO;
		goto errout_touch;
	}
	sem_post(&dev->status_check_sem);
	touchvdbg("touch_point %d\n", touch_point + 1);

	data->npoints = touch_point + 1;
	for (int i = 0; i < (touch_point + 1); i++) {
		eid = (touch_event + (i * EVENT_PACKET_SIZE))[0] & 0x3;
		if (eid == EID_COORD) {
			p_evt_coord = (struct ts_event_coordinate *)(touch_event + (i * EVENT_PACKET_SIZE));
			data->point[i].id = p_evt_coord->tid - 1;
			data->point[i].x  = (p_evt_coord->x_11_4 << 4) | p_evt_coord->x_3_0;
			data->point[i].y  = (p_evt_coord->y_11_4 << 4) | p_evt_coord->y_3_0;
			data->point[i].h = 0;
			data->point[i].w = 0;
			data->point[i].pressure = 0;
			switch (p_evt_coord->tsta) {
			case 1:
				data->point[i].flags = TOUCH_DOWN;
				break;
			case 2:
				data->point[i].flags = TOUCH_MOVE;
				break;
			case 3:
				data->point[i].flags = TOUCH_UP;
				break;
			}
			printf("COORDINATES: id %d status %d type %d x : %d y : %d\n", data->point[i].id, p_evt_coord->tsta, (p_evt_coord->ttype_3_2 << 2) | p_evt_coord->ttype_1_0, data->point[i].x, data->point[i].y);
		}
	}
	return OK;
errout_touch:
	sem_post(&dev->status_check_sem);
	return ret;
}

/****************************************************************************
 * Name: ist415_enable
 ****************************************************************************/

static void ist415_enable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	priv->wdog = wd_create();
	sem_post(&priv->status_check_sem);
	int ret = wd_start(priv->wdog, TIMEOUT, status_timerfn, 1,  (uint32_t)priv);
	if (ret != OK) {
		wd_delete(priv->wdog);
	}
	printf("enable irq\n");
	priv->ops->irq_enable();
}

/****************************************************************************
 * Name: ist415_disable
 ****************************************************************************/

static void ist415_disable(struct touchscreen_s *dev)
{
	struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	priv->ops->irq_disable();
	while (sem_wait(&priv->status_check_sem) != 0) {
                ASSERT(errno == EINTR);
        }
	printf("irq disable\n");
	work_cancel(HPWORK, &ist415_status_work);
	wd_delete(priv->wdog);
}

/****************************************************************************
 * Name: ist415_firmware_update
 ****************************************************************************/

static int ist415_firmware_update(struct touchscreen_s *dev)
{
	int ret = OK;
        struct ist415_dev_s *priv = (struct ist415_dev_s *)dev->priv;
	priv->ops->irq_disable();
	while (sem_wait(&priv->status_check_sem) != 0) {
                ASSERT(errno == EINTR);
        }
	ret = ist415x_fwdn_fwupdate(0x4158, priv);
        priv->ops->reset();
	DelayMs(2000);
	printf("firmware update done\n");
	sem_post(&priv->status_check_sem);
	priv->ops->irq_enable();
	return ret;
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
		return ist415_get_touch_data(priv, buffer);
	} else {
		touchvdbg("No data to read\n");
	}
	return OK;
}

/****************************************************************************
 * Name: touch_interrupt
 ****************************************************************************/

static void touch_interrupt(struct ist415_dev_s *priv)
{

	FAR struct touchscreen_s *upper = priv->upper;
	irqstate_t state = enter_critical_section();
	priv->int_pending = true;
	leave_critical_section(state);
	if (upper->notify_touch) {
		upper->notify_touch(upper);
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
	uint8_t reg[1];
	priv->ops->irq_disable();

	reg[0] = 0x23;
	ret = i2c_write(priv->i2c, &(priv->i2c_config), (uint8_t *)reg, 1);
	if (ret < OK) {
		touchdbg("ERROR: i2c_write failed\n");
		return NULL;
	}
	struct touchscreen_s *upper = (struct touchscreen_s *)kmm_zalloc(sizeof(struct touchscreen_s));
	upper->ops = &g_ist415_ops;
	upper->priv = priv;

	//priv->wdog = wd_create();
	sem_init(&priv->status_check_sem, 0, 0);
	priv->upper = upper;
	priv->handler = touch_interrupt;
	//ist415x_fwdn_fwupdate(0x4158, priv);

	//priv->ops->reset();
	//lldbg("hiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n");
	//printf("Touch Driver registered Successfully\n");
	return touch_register(path, upper);
}
