/****************************************************************************
 *
 * Copyright 2025 Samsung Electronics All Rights Reserved.
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
 * drivers/sensors/ais25ba.c
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

#include "ais25ba.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/****************************************************************************
 * Private Types
 ****************************************************************************/
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static ssize_t ais25ba_read(FAR struct sensor_upperhalf_s *dev, FAR void *buffer);
static void ais25ba_set_mclk(struct sensor_upperhalf_s *priv, int mclk);
static void ais25ba_set_bclk(struct sensor_upperhalf_s *priv, int bclk);
static void ais25ba_start(struct sensor_upperhalf_s *priv);
static void ais25ba_stop(struct sensor_upperhalf_s *priv);
static void ais25ba_setchannel_count(struct sensor_upperhalf_s *priv, int channel_count);
static void ais25ba_setbit_perchannel(struct sensor_upperhalf_s *priv, int bit_per_channel);
static void ais25ba_set_samprate(struct sensor_upperhalf_s *priv, int samp_rate);
static int ais25ba_verify_sensor(struct sensor_upperhalf_s *upper, struct i2c_dev_s *i2c, struct i2c_config_s config);
static void ais25ba_alivecheck_work(struct ais25ba_dev_s *dev);
static void ais25ba_set_config_i2c(struct i2c_dev_s *i2c, struct i2c_config_s config);
static void ais25ba_timer_handler(int argc, uint32_t arg1);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct sensor_ops_s g_ais25ba_ops = {
    .sensor_read = ais25ba_read,
	.sensor_set_mclk = ais25ba_set_mclk,
	.sensor_set_bclk = ais25ba_set_bclk,
	.sensor_start = ais25ba_start,
	.sensor_stop = ais25ba_stop,
	.sensor_setchannel_count = ais25ba_setchannel_count,
	.sensor_setbit_perchannel = ais25ba_setbit_perchannel,
	.sensor_set_samprate = ais25ba_set_samprate,
	.sensor_verify = ais25ba_verify_sensor,
};

struct ap_buffer_s *g_apb;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static float ais25ba_raw_to_mg(int16_t lsb)
{
	return ((float)lsb) * 0.122f;
}

static void ais25ba_i2s_callback(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, FAR void *arg, int result)
{
	struct ais25ba_ctrl_s *ctrl = (struct ais25ba_ctrl_s *)arg;
	sem_post(&ctrl->callback_wait_sem);
	sem_post(&ctrl->read_sem);
}

static void ais25ba_set_mclk(struct sensor_upperhalf_s *priv, int mclk)
{
	return;
}

static void ais25ba_set_bclk(struct sensor_upperhalf_s *priv, int bclk)
{
	return;
}

static void ais25ba_start(struct sensor_upperhalf_s *upper)
{
	snvdbg("sensor start\n");
	struct ais25ba_dev_s *priv = upper->priv;
	struct i2s_dev_s *i2s = priv->i2s;
	I2S_RESUME(i2s, I2S_RX);
}

static void ais25ba_stop(struct sensor_upperhalf_s *upper)
{
	snvdbg("sensor stop\n");
	struct ais25ba_dev_s *priv = upper->priv;
	I2S_STOP(priv->i2s, I2S_RX);
}

static void ais25ba_setchannel_count(struct sensor_upperhalf_s *priv, int channel_count)
{
	return;
}

static void ais25ba_setbit_perchannel(struct sensor_upperhalf_s *upper, int bit_per_channel)
{
	struct ais25ba_dev_s *priv = upper->priv;
	snvdbg("bit per channel %d\n", bit_per_channel);
	I2S_RXDATAWIDTH(priv->i2s, bit_per_channel);
}

static void ais25ba_set_samprate(struct sensor_upperhalf_s *upper, int samp_rate)
{
	FAR struct ais25ba_dev_s *priv = upper->priv;
	snvdbg("i2s rx sample rate %d\n", samp_rate);
	I2S_RXSAMPLERATE(priv->i2s, samp_rate);
}

static int ais25ba_verify_sensor(struct sensor_upperhalf_s *upper, struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int reg[2];
	uint8_t data[2];
	reg[0] = AIS25BA_WHOAMI_REGISTER;                                  //WHO_AM_I

#ifdef CONFIG_I2C_WRITEREAD
	if (OK != i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1)) {
		return ERROR;
	}
#else
	i2c_write(i2c, &config, (uint8_t *)reg, 1);
	i2c_read(i2c, &config, (uint8_t *)data, 1);
	/*
	if (i2c_write(i2c, &config, (uint8_t *)reg, 1) == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
	} else {
		lldbg("%d\n", __LINE__);
		return ERROR;
	}
	*/
#endif

	snvdbg("Alive check return data : %8x\n", data[0]);
	if (data[0] == AIS25BA_WHOAMI_VALUE) {
		return OK;
	}
	return ERROR;
}

static void ais25ba_i2c_read_data(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int ret = 0;
	uint8_t reg[2];
	uint8_t data[2];
#ifdef CONFIG_I2C_WRITEREAD

	reg[0] = AIS25BA_TEST_REG;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);

	reg[0] = AIS25BA_CTRL_REG_1;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	printf("ret 26 data : %8x\n",  data[0]);

	reg[0] = AIS25BA_CTRL_REG_2;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	printf("ret 2F data : %8x\n",  data[0]);

	reg[0] = AIS25BA_CTRL_REG_FS;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	printf("ret 30 data : %8x\n", data[0]);

	reg[0] = AIS25BA_TDM_CTRL_REG;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	printf("ret 2E data : %8x\n", data[0]);

#else
	reg[0] = AIS25BA_TEST_REG;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		printf("data read 0B : %8x\n", data[0]); // this should be 0x20
	}

	reg[0] = AIS25BA_CTRL_REG_1;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		printf("data read 26 : %8x\n", data[0]); // this should be 0x20
	}

	reg[0] = AIS25BA_CTRL_REG_2;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		printf("data read 2F : %8x\n", data[0]); // this should be 0x20
	}

	reg[0] = AIS25BA_CTRL_REG_FS;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		printf("data read 30 : %8x\n", data[0]); // this should be 0x20
	}

	reg[0] = AIS25BA_TDM_CTRL_REG;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 1);
	if (ret == 1) {
		i2c_read(i2c, &config, (uint8_t *)data, 1);
		printf("data read 2E : %8x\n", data[0]); // this should be 0x20
	}
#endif
}

static void ais25ba_i2c_write_data(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int ret = 0;
	uint8_t reg[2];
	uint8_t data[2];
#ifdef CONFIG_I2C_WRITEREAD
	reg[0] = 0x26;
	reg[1] = 0x00;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
	//printf("\nret 26 data : %8x\n",  data[0]);
	DelayMs(100);
	reg[0] = 0x2E;
	reg[1] = 0x62;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
	//printf("\nret 2E data : %8x\n",  data[0]);
#else
	reg[0] = 0x26;
	reg[1] = 0x00;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);

	reg[0] = 0x2E;
	reg[1] = 0x62;
	ret = i2c_write(i2c, &config, (uint8_t *)reg, 2);
#endif
}

static void ais25ba_set_config_i2c(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	ais25ba_i2c_read_data(i2c, config);
	DelayMs(2000);
	ais25ba_i2c_write_data(i2c, config);
	DelayMs(2000);
	ais25ba_i2c_read_data(i2c, config);
	DelayMs(2000);
}

static int ais25ba_read_i2s(struct i2s_dev_s *i2s, struct ais25ba_ctrl_s *ctrl, FAR void *buffer)
{
	struct audio_buf_desc_s desc;
	int sem_cnt;
	int prev_sem_cnt;
	desc.numbytes = 512;
	desc.u.ppBuffer = &g_apb;

	int ret = apb_alloc(&desc);
	if (ret < 0) {
			printf("ERROR: apb_alloc: apb buffer allocation failed\n");
			return;
	}

	//sem_timedwait(&ctrl->read_sem, &ctrl->sem_timeout);				/* To prevent deadlock in I2S_RECEIVE */
	sem_wait(&ctrl->read_sem);

    ret = I2S_RECEIVE(i2s, g_apb, ais25ba_i2s_callback, ctrl, 100);	/* 100 ms timeout for read data */
	if (ret != OK) {
		sndbg("ERROR: I2S_RECEIVE FAILED\n");
	}

	//sem_timedwait(&ctrl->callback_wait_sem, &ctrl->sem_timeout);	/* To prevent deadlock in I2S_RECEIVE */
	sem_wait(&ctrl->callback_wait_sem);

	sensor_data_s *data = (sensor_data_s *)buffer;
	int16_t *samp_data = (int16_t *)&g_apb->samp[0];

	for (int i = 0, j = 0; i < g_apb->nbytes; i+=16, j++) {
		data[j].x = ais25ba_raw_to_mg(*samp_data);
		samp_data++;
		data[j].y = ais25ba_raw_to_mg(*samp_data);
		samp_data++;
		data[j].z = ais25ba_raw_to_mg(*samp_data);
		samp_data += 6;		/* Vendor specific skip bits */
	}
	apb_free(g_apb);
	return ret;
}

static ssize_t ais25ba_read(FAR struct sensor_upperhalf_s *dev, FAR void *buffer)
{
	FAR struct ais25ba_dev_s *priv = dev->priv;
	int ret;

	struct i2c_dev_s *i2c = priv->i2c;
	struct i2s_dev_s *i2s = priv->i2s;
	struct i2c_config_s config = priv->i2c_config;

	ret = ais25ba_read_i2s(i2s, &priv->ctrl, buffer);
	DelayMs(5000);

	return ret;
}

static void ais25ba_alivecheck_work(struct ais25ba_dev_s *dev)
{
	int sensor_status;
	int retry_count = AIS25BA_ALIVECHECK_RETRY_COUNT;
	sensor_status = ais25ba_verify_sensor(dev->upper, dev->i2c, dev->i2c_config);

	if (sensor_status != OK) {
		// Retry Sensor Verification
		lldbg("Sensor verification failed, applying retry and recover\n");
		goto retry_sensor_verification;
	}
	(void)wd_start(dev->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)dev);
	return;

retry_sensor_verification:
	sensor_status = OK;
	while (retry_count > 0 && sensor_status == OK) {
		sensor_status = ais25ba_verify_sensor(dev->upper, dev->i2c, dev->i2c_config);
		retry_count--;
		DelayMs(1000);
	}

	if (sensor_status != OK) {
		// Initialize sensor again;
		struct ais25ba_ctrl_s *ctrl = &(dev->ctrl);
		sem_wait(&ctrl->read_sem);
		ais25ba_set_config_i2c(dev->i2c, dev->i2c_config);
		sndbg("Sensor reinitialized");
		(void)wd_start(dev->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)dev);
		sem_post(&ctrl->read_sem);
	}
}

static void ais25ba_timer_handler(int argc, uint32_t arg1)
{
	struct ais25ba_dev_s *priv = (struct ais25ba_dev_s *)arg1;
	work_queue(HPWORK, &priv->work, (worker_t)ais25ba_alivecheck_work, priv, 0);
}

int ais25ba_initialize(const char *devpath, struct ais25ba_dev_s *priv)
{
	/* Setup device structure. */
	int sensor_verify_status;
	struct sensor_upperhalf_s *upper = (struct sensor_upperhalf_s *)kmm_zalloc(sizeof(struct sensor_upperhalf_s));
	if (!upper) {
		sndbg("ERROR: upperhalf memory allocation failed\n");
	}
	lldbg("%d\n", __LINE__);
	upper->ops = &g_ais25ba_ops;
	upper->priv = priv;
	priv->upper = upper;

	lldbg("%d\n", __LINE__);
	/* Sensor Connection Verification */
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	sem_init(&priv->ctrl.read_sem, 0, 1);
	sem_init(&priv->ctrl.callback_wait_sem, 0, 0);
	priv->ctrl.sem_timeout.tv_sec = 10;		// Seconds
	priv->ctrl.sem_timeout.tv_nsec = 100000000;	// nanoseconds

	lldbg("%d\n", __LINE__);
	if (ais25ba_verify_sensor(upper, i2c, config) == OK) {
		snvdbg("Sensor connection verification success\n");
	} else{
		sndbg("ERROR: Sensor verification failed, sensor not found/not responding\n");
	}

	lldbg("%d\n", __LINE__);
	//I2C config set. Read data is to check if write register is successful or not
	ais25ba_set_config_i2c(i2c, config);

	lldbg("%d\n", __LINE__);
	priv->wdog = wd_create();
	if (wd_start(priv->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)priv) != OK) {
		lldbg("Fail to start AIS25BA alive-check wdog, errno : %d\n", get_errno());
	}

	lldbg("%d\n", __LINE__);
	return sensor_register(devpath, upper);
}
