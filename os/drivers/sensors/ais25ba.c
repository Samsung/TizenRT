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

#include <tinyara/sensors/ais25ba.h>

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
static int ais25ba_start(struct sensor_upperhalf_s *priv);
static int ais25ba_stop(struct sensor_upperhalf_s *priv);
static void ais25ba_setbit_perchannel(struct sensor_upperhalf_s *priv, int bit_per_channel);
static void ais25ba_set_samprate(struct sensor_upperhalf_s *priv, int samp_rate);
static int ais25ba_verify_sensor(struct sensor_upperhalf_s *upper, struct i2c_dev_s *i2c, struct i2c_config_s config);
static void ais25ba_alivecheck_work(struct ais25ba_dev_s *dev);
static void ais25ba_set_config_i2c(struct i2c_dev_s *i2c, struct i2c_config_s config);
static void ais25ba_timer_handler(int argc, uint32_t arg1);
static int ais25ba_register_mq(struct sensor_upperhalf_s *priv, mqd_t g_mems_mq);
static void ais25ba_get_bufsize(struct sensor_upperhalf_s *upper, int* buf_size);
static void ais25ba_get_bufnum(struct sensor_upperhalf_s *upper, int* buf_num);
static void ais25ba_send_buffer(struct sensor_upperhalf_s *upper, unsigned long buffer);
static int ais25ba_read_data(struct ais25ba_dev_s *priv);
static int ais25ba_send_result(FAR struct ais25ba_dev_s *priv);
static void ais25ba_show(unsigned long sensor_status_buffer);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct sensor_ops_s g_ais25ba_ops = {
    .sensor_read = ais25ba_read,
	.sensor_start = ais25ba_start,
	.sensor_stop = ais25ba_stop,
	.sensor_setbit_perchannel = ais25ba_setbit_perchannel,
	.sensor_set_samprate = ais25ba_set_samprate,
	.sensor_verify = ais25ba_verify_sensor,
	.sensor_register_mq = ais25ba_register_mq,
	.sensor_get_bufsize = ais25ba_get_bufsize,
	.sensor_get_bufnum = ais25ba_get_bufnum,
	.sensor_send_buffer = ais25ba_send_buffer,
	.sensor_show = ais25ba_show,
};

static struct ais25ba_dev_s g_ais25ba_priv;
static struct sensor_info_s g_sensor_info;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int ais25ba_sem_wait(sem_t *sem)
{
	int status;

	do {
		status = sem_wait(sem);
	} while (status == -1 && get_errno() == EINTR);

	return status;
}

static int ais25ba_mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio)
{
	int status;

	do {
		status = mq_send(mqdes, msg_ptr, msg_len, msg_prio);
	} while (status == -1 && get_errno() == EINTR);

	return status;
}

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

static void ais25ba_enqueue_data(FAR struct ais25ba_dev_s *priv, struct ais25ba_buf_s *buf)
{
	sq_addlast((sq_entry_t *)&buf->entry, &priv->pendq);
}

static int ais25ba_start(struct sensor_upperhalf_s *upper)
{
	snvdbg("sensor start\n");
	struct ais25ba_dev_s *priv = upper->priv;
	struct i2s_dev_s *i2s = priv->i2s;

	if (priv->mq == NULL) {
		sndbg("ERROR: sensor mq is NULL\n");
		return -ENOTTY;
	}

	if (g_sensor_info.sensor_is_prepared == false) {
		sndbg("ERROR: Sensor is not prepared, prepare sensor first");
		return ERROR;
	}
	if (g_sensor_info.sensor_is_running) {
		sndbg("Error: Another instance of sensor is already running\n");
		return ERROR;
	}

	g_sensor_info.sensor_is_running = true;
	g_sensor_info.total_frame_last_start = 0;
	sem_post(&(priv->sensor_run_on));
	return OK;
}

static int ais25ba_stop(struct sensor_upperhalf_s *dev)
{
	FAR struct ais25ba_dev_s *priv = dev->priv;
	struct i2s_dev_s *i2s = priv->i2s;
	int ret;

	snvdbg("sensor stop\n");

	if (g_sensor_info.sensor_is_running == false) {
		sndbg("ERROR: sensor is not running\n");
		return ERROR;
	}

	ais25ba_sem_wait(&(priv->sensor_run_on));
	/* TODO: Replace sem_wait with sem_tickwait after callback stuck issue is resolved */
	/*ret = sem_tickwait(&(priv->sensor_run_on), clock_systimer(), MSEC2TICK(405));
	if (ret == ETIMEDOUT) {
		lldbg("Timeout for sensor_run_on semaphore, driver not ready\n");
	}*/
	g_sensor_info.sensor_is_running = false;
	g_sensor_info.sensor_is_prepared = false;
	while (sq_peek(&priv->pendq) != NULL) {
		sq_remfirst(&priv->pendq);
	}
	while (sq_peek(&priv->doneq) != NULL) {
		sq_remfirst(&priv->doneq);
	}

	ret = I2S_STOP(i2s, I2S_RX);
	if (ret != OK) {
		sndbg("ERROR: I2S_STOP api failed\n");
		return ret;
	}
	return OK;
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

static void ais25ba_get_bufsize(struct sensor_upperhalf_s *upper, int* buf_size)
{
	*buf_size = AIS25BA_BUFSIZE;
}

static void ais25ba_get_bufnum(struct sensor_upperhalf_s *upper, int* buf_num)
{
	*buf_num = AIS25BA_PREPARING_BUFFER_NUMBER;
}

static void ais25ba_send_buffer(struct sensor_upperhalf_s *upper, unsigned long buffer)
{
	FAR struct ais25ba_dev_s *priv = upper->priv;
	struct ais25ba_buf_s *buf = (FAR struct ais25ba_buf_s *)buffer;
	ais25ba_enqueue_data(priv, buf);
	g_sensor_info.sensor_is_prepared = true;
}

static int ais25ba_register_mq(struct sensor_upperhalf_s *upper, mqd_t g_mems_mq)
{
	FAR struct ais25ba_dev_s *priv = upper->priv;
	priv->mq = (mqd_t)g_mems_mq;
	return OK;
}

static int ais25ba_verify_sensor(struct sensor_upperhalf_s *upper, struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int reg[2];
	uint8_t data[2];
	reg[0] = AIS25BA_WHOAMI_REGISTER;

	if (i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1) != 1){
		return ERROR;
	}

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

	reg[0] = AIS25BA_TEST_REG;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	if (ret != 1) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p\n", reg[0]);
		return;
	}
	reg[0] = AIS25BA_CTRL_REG_1;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	if (ret != 1) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p\n", reg[0]);
		return;
	}
	reg[0] = AIS25BA_CTRL_REG_2;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	if (ret != 1) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p\n", reg[0]);
		return;
	}
	reg[0] = AIS25BA_CTRL_REG_FS;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	if (ret != 1) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p\n", reg[0]);
		return;
	}
	reg[0] = AIS25BA_TDM_CTRL_REG;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 1, data, 1);
	if (ret != 1) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p\n", reg[0]);
		return;
	}
}

static void ais25ba_i2c_write_data(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	int ret = 0;
	uint8_t reg[2];
	uint8_t data[2];

	reg[0] = 0x26;
	reg[1] = 0x00;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
	if (ret != 2) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p  %p, ret: %d\n", reg[0], reg[1]);
		return;
	}
	usleep(1000 *100);
	reg[0] = 0x2E;
	reg[1] = 0x02;
	ret = i2c_writeread(i2c, &config, (uint8_t *)reg, 2, data, 0);
	if (ret != 2) {
		sndbg("ERROR: I2C writeread failed, reg_addr: %p  %p\n", reg[0], reg[1]);
		return;
	}
	return;
}

static void ais25ba_set_config_i2c(struct i2c_dev_s *i2c, struct i2c_config_s config)
{
	ais25ba_i2c_read_data(i2c, config);
	usleep(1000 *2000);
	ais25ba_i2c_write_data(i2c, config);
	usleep(1000 *2000);
	ais25ba_i2c_read_data(i2c, config);
	usleep(1000 *2000);
}

static ssize_t ais25ba_read(FAR struct sensor_upperhalf_s *dev, FAR void *buffer)
{
	FAR struct ais25ba_dev_s *priv = dev->priv;
	int ret;

	struct i2s_dev_s *i2s = priv->i2s;
	struct i2c_config_s config = priv->i2c_config;
	struct ais25ba_ctrl_s *ctrl = &(priv->ctrl);
	struct audio_buf_desc_s desc;
	int sem_cnt;
	int prev_sem_cnt;
	struct ap_buffer_s *g_apb;
	//desc.numbytes = 256;
	desc.numbytes = AIS25BA_DMA_BUFF_SAMPLE_NUMBER * 16;	// N * 16 bytes
	desc.u.ppBuffer = &g_apb;

	ret = apb_alloc(&desc);
	if (ret < 0) {
		sndbg("ERROR: apb_alloc: apb buffer allocation failed\n");
		return ret;
	}

	ais25ba_sem_wait(&ctrl->read_sem);
	/* TODO: Replace sem_wait with sem_tickwait after callback stuck issue is resolved */
	/*ret = sem_tickwait(&ctrl->read_sem, clock_systimer(), MSEC2TICK(405));
	if (ret == ETIMEDOUT) {
		lldbg("Timeout for read_sem semaphore, previous read not finished\n");
	}*/

	ret = I2S_RECEIVE(i2s, g_apb, ais25ba_i2s_callback, ctrl, 100);	/* 100 ms timeout for read data */
	if (ret != OK) {
		sndbg("ERROR: I2S_RECEIVE FAILED\n");
		goto error;
	}

	ais25ba_sem_wait(&ctrl->callback_wait_sem);
	/* TODO: Replace sem_wait with sem_tickwait after callback stuck issue is resolved */
	/*ret = sem_tickwait(&ctrl->callback_wait_sem, clock_systimer(), MSEC2TICK(405));
	if (ret == ETIMEDOUT) {
		lldbg("I2S BSP callback not received\n");
	}*/

	sensor_data_s *data = (sensor_data_s *)buffer;
	int16_t *samp_data = (int16_t *)&g_apb->samp[0];

	for (int i = 0, j = 0; i < g_apb->nbytes; i+=16, j++) {
		// data[j].x = ais25ba_raw_to_mg(*samp_data);
		// samp_data++;
		// data[j].y = ais25ba_raw_to_mg(*samp_data);
		// samp_data++;
		// data[j].z = ais25ba_raw_to_mg(*samp_data);
		// samp_data += 6;		/* Vendor specific skip bits */
		memcpy(data[j].samples, samp_data, 16);
		samp_data += 8; 
	}

error:
	apb_free(g_apb);
	return ret;
}

static int ais25ba_read_data(struct ais25ba_dev_s *priv)
{
	int ret = OK;
	struct ais25ba_buf_s *buf = (struct ais25ba_buf_s *)sq_remfirst(&priv->pendq);

	ret = ais25ba_read(priv->upper, buf->data);
	if (ret != OK) {
		sndbg("ERROR: Sensor read failed\n");
	}

	sq_addlast((sq_entry_t *)&buf->entry, &priv->doneq);

	return ret;
}

static void ais25ba_show(unsigned long buffer)
{
	sensor_info_s *sensor_status_buffer = (sensor_info_s *)buffer;
	memcpy(sensor_status_buffer, &g_sensor_info, sizeof(sensor_info_s));
}

static void ais25ba_alivecheck_work(struct ais25ba_dev_s *dev)
{
	int sensor_status;
	int retry_count = AIS25BA_ALIVECHECK_RETRY_COUNT;
	sensor_status = ais25ba_verify_sensor(dev->upper, dev->i2c, dev->i2c_config);

	if (sensor_status != OK) {
		// Retry Sensor Verification
		sndbg("Sensor verification failed, applying retry and recover\n");
		goto retry_sensor_verification;
	}
	++g_sensor_info.alive_check_count;
	(void)wd_start(dev->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)dev);
	return;

retry_sensor_verification:
	sensor_status = OK;
	while (retry_count > 0 && sensor_status == OK) {
		sensor_status = ais25ba_verify_sensor(dev->upper, dev->i2c, dev->i2c_config);
		retry_count--;
		usleep(1000 *1000);
	}

	if (sensor_status != OK) {
		// Initialize sensor again;
		struct ais25ba_ctrl_s *ctrl = &(dev->ctrl);
		ais25ba_sem_wait(&ctrl->read_sem);
		ais25ba_set_config_i2c(dev->i2c, dev->i2c_config);
		sndbg("Sensor reinitialized");
		(void)wd_start(dev->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)dev);
		sem_post(&ctrl->read_sem);
		++g_sensor_info.alive_check_fail_count;
	}
}

static void ais25ba_timer_handler(int argc, uint32_t arg1)
{
	struct ais25ba_dev_s *priv = (struct ais25ba_dev_s *)arg1;
	work_queue(HPWORK, &priv->work, (worker_t)ais25ba_alivecheck_work, priv, 0);
}

static int ais25ba_send_result(FAR struct ais25ba_dev_s *priv)		/* data transfer from doneq -> mq */
{
	struct ais25ba_msg_s msg;
	struct ais25ba_buf_s *buf;
	int ret = OK;
	while (sq_peek(&priv->doneq) != NULL) {
		buf = (struct ais25ba_buf_s *)sq_remfirst(&priv->doneq);
		msg.data = (FAR void *)buf;
		ret = ais25ba_mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_AIS25BA_SG_DEQUEUE_PRIO);
		if (ret != OK) {
			sndbg("mq_send error, errno : %d\n", errno);
		}
	}
	return ret;
}

static int ais25ba_mq_thread(int argc, char **argv)
{
	struct ais25ba_dev_s *priv;
	int ret = OK;

	priv = (struct ais25ba_dev_s *)strtoul(argv[1], NULL, 16);

	while (1) {
		ais25ba_sem_wait(&(priv->sensor_run_on));
		if (sq_peek(&priv->pendq) == NULL) {
			sem_post(&(priv->sensor_run_on));
			usleep(100000);
			continue;
		}
		ret = ais25ba_read_data(priv);
		if (ret != OK) {
			sndbg("ERROR: I2S read failed\n");
		}
		++g_sensor_info.total_frame_receive_count;
		++g_sensor_info.total_frame_last_start;
		if (work_available(&priv->work)) {
			ret = work_queue(HPWORK, &priv->work, ais25ba_send_result, priv, 0);
			if (ret != OK) {
				sndbg("ERROR: work_queue create failed\n");
			}
		}
		sem_post(&(priv->sensor_run_on));
	}
	sndbg("ERROR!!!!!!, mq_thread is exited\n");
	return OK;
}

int ais25ba_initialize(const char *devpath, struct ais25ba_dev_s *priv)
{
	/* Setup device structure. */
	char *parm[2];
	char parm_buf[9];
	int sensor_verify_status;
	pid_t pid;

	struct sensor_upperhalf_s *upper = (struct sensor_upperhalf_s *)kmm_zalloc(sizeof(struct sensor_upperhalf_s));
	if (!upper) {
		sndbg("ERROR: upperhalf memory allocation failed\n");
		goto cleanup_sem;
	}
	upper->ops = &g_ais25ba_ops;
	upper->priv = priv;
	priv->upper = upper;

	/* Sensor Connection Verification */
	struct i2c_dev_s *i2c = priv->i2c;
	struct i2c_config_s config = priv->i2c_config;
	sem_init(&priv->ctrl.read_sem, 0, 1);
	sem_init(&priv->ctrl.callback_wait_sem, 0, 0);
	sem_init(&priv->sensor_run_on, 0, 0);
	sq_init(&priv->pendq);
	sq_init(&priv->doneq);
	g_sensor_info.alive_check_count = 0;
	g_sensor_info.alive_check_fail_count = 0;
	g_sensor_info.total_frame_receive_count = 0;
	g_sensor_info.total_frame_last_start = 0;
	g_sensor_info.sensor_is_prepared = false;
	g_sensor_info.sensor_is_running = false;

	if (ais25ba_verify_sensor(upper, i2c, config) == OK) {
		sndbg("Sensor connection verification success\n");
	} else{
		sndbg("ERROR: Sensor verification failed, sensor not found/not responding\n");
		goto cleanup_with_upper;
	}

	//I2C config set. Read data is to check if write register is successful or not
	ais25ba_set_config_i2c(i2c, config);

	// priv->wdog = wd_create();
	// if (wd_start(priv->wdog, MSEC2TICK(AIS25BA_ALIVECHECK_TIME), (wdentry_t)ais25ba_timer_handler, 1, (uint32_t)priv) != OK) {
	// 	sndbg("Fail to start AIS25BA alive-check wdog, errno : %d\n", get_errno());
	// }

	itoa((int)priv, parm_buf, 16);
	parm[0] = parm_buf;
	parm[1] = NULL;
	pid = kernel_thread(AIS25BA_KERNEL_MQ_THREAD, 200, 18000, (main_t)ais25ba_mq_thread, (FAR char *const *)parm);
	if (pid < 0) {
		sndbg("ais25ba_mq_thread thread creation failed\n");
		goto cleanup_with_upper;
	}

	return sensor_register(devpath, upper);

cleanup_with_upper:
	free(upper);
cleanup_sem:
	sem_destroy(&priv->ctrl.read_sem);
	sem_destroy(&priv->ctrl.callback_wait_sem);
	sem_destroy(&priv->sensor_run_on);
	return ERROR;
}
