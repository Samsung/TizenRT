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
 * drivers/mmwave/mmwave_dev.c
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
#include <fcntl.h>
#include <tinyara/fs/fs.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/sensors/iwrl6432.h>
#include <tinyara/wqueue.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#ifndef CONFIG_IWRL6432_SG_DEQUEUE_PRIO
#define CONFIG_IWRL6432_SG_DEQUEUE_PRIO 1
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/
 struct iwrl6432_dev_s {
	FAR struct iwrl6432_config_s *config;
	FAR struct spi_dev_s *spi;
 	sem_t exclsem;
	sem_t datasem;
	int crefs;
	mqd_t mq;
	bool enable;
	bool running;
	struct sq_queue_s pendq;
	struct sq_queue_s doneq;
	struct work_s work;
	uint16_t timestamp;
	int req_num;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int iwrl6432_open(FAR struct file *filep);
static int iwrl6432_close(FAR struct file *filep);
static ssize_t iwrl6432_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t iwrl6432_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int iwrl6432_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static int iwrl6432_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);

/****************************************************************************
 * Private Data
 ****************************************************************************/
static FAR struct iwrl6432_dev_s g_iwrl6432_priv;

static const struct file_operations g_iwrl6432_fileops = {
	iwrl6432_open,  /* open */
	iwrl6432_close, /* close */
	iwrl6432_read,  /* read */
	iwrl6432_write, /* write */
	NULL,           /* seek */
	iwrl6432_ioctl, /* ioctl */
};

static FAR struct iwrl6432_dev_s g_iwrl6432_priv;

/****************************************************************************
 * Private Function
 ****************************************************************************/


/************************************************************************************
 * Name: iwrl6432_semtake
 ************************************************************************************/
static inline int iwrl6432_semtake(FAR sem_t *sem, bool errout)
{
	/* Take a count from the semaphore, possibly waiting */
	while (sem_wait(sem) != OK) {
		/* EINTR is the only error that we expect */
		ASSERT(get_errno() == EINTR);
		if (errout) {
			return -EINTR;
		}
	}
	return OK;
}

/****************************************************************************
 * Name: iwrl6432_semgive
 ****************************************************************************/
static inline void iwrl6432_semgive(sem_t *sem)
{
	sem_post(sem);
}

/****************************************************************************
 * Name:  checksum_cal
 *
 ****************************************************************************/
static uint8_t checksum_cal(uint8_t *input_buf, int start_index, int end_index)
{
	uint32_t sum = 0;
	for (int i = start_index; i < end_index; i++) {
		sum += input_buf[i];
	}
	return (uint8_t)(sum & 0xFF);
}

static void make_ack(uint8_t request_number, uint8_t *ack_tx_buf)
{
	// Initialize packet header
	ack_tx_buf[0] = PAYLOAD_HEADER;
	ack_tx_buf[1] = PAYLOAD_SOF;

	// Set packet length (ACK_TX_SIZE - 7)
	uint32_t packet_length = ACK_TX_SIZE - 7;
	ack_tx_buf[2] = (uint8_t)(packet_length & 0xFF);
	ack_tx_buf[3] = (uint8_t)((packet_length >> 8) & 0xFF);
	ack_tx_buf[4] = (uint8_t)((packet_length >> 16) & 0xFF);
	ack_tx_buf[5] = (uint8_t)((packet_length >> 24) & 0xFF);

	// Fixed values in the packet
	ack_tx_buf[6] = 0x11;
	ack_tx_buf[7] = 0x24;
	ack_tx_buf[8] = 0x02;
	ack_tx_buf[9] = 0x00;
	ack_tx_buf[10] = 0x00;
	ack_tx_buf[11] = 0x00;
	ack_tx_buf[12] = 0x00;

	// Insert request number
	ack_tx_buf[13] = request_number;

	// Calculate checksum from index 1 to ACK_TX_SIZE-3 (excludes last 3 bytes)
	uint8_t checksum = checksum_cal(ack_tx_buf, 1, ACK_TX_SIZE - 2);
	ack_tx_buf[14] = checksum;

	// Set packet tail
	ack_tx_buf[15] = PAYLOAD_TAIL;
}

static int iwrl6432_send_result(FAR struct iwrl6432_dev_s *priv)
{
	struct iwrl6432_msg_s msg;
	struct iwrl6432_buf_s *buf;
	int ret = OK;
	while(sq_peek(&priv->doneq) != NULL) {
		buf = (struct iwrl6432_buf_s *)sq_remfirst(&priv->doneq);
		msg.msgId = buf->msgId;
		msg.pData = (FAR void *)buf;
		ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO);
		if (ret != OK) {
			lldbg("mq_send error...errno : %d\n", errno);
		}
	}
	return ret;
}

static void iwrl6432_request_cube(FAR struct iwrl6432_dev_s *priv)
{
	uint8_t ack_tx_buf[ACK_TX_SIZE];
	lldbg("req_num : %d\n", priv->req_num);
	make_ack(priv->req_num, ack_tx_buf);
	SPI_LOCK(priv->spi, true);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, true);
	SPI_SNDBLOCK(priv->spi, ack_tx_buf, ACK_TX_SIZE);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, false);
	SPI_LOCK(priv->spi, false);
}

static void iwrl6432_request_cube_data(FAR struct iwrl6432_dev_s *priv)
{
	struct timespec ts_start, ts_end;
	struct iwrl6432_buf_s *rxbuf;
	int elapsed_time;
	uint16_t time_stamp;
	size_t size;
	if (sq_empty(&priv->pendq)) {
		lldbg("Error!!! queue is empty\n");
	}
	rxbuf = (struct iwrl6432_buf_s *)sq_remfirst(&priv->pendq);
	if (priv->req_num != 1) {
		size = IWRL6432_DATA_SIZE;
	} else {
		size = CLOUD_TX_SIZE;
	}

	clock_gettime(CLOCK_MONOTONIC, &ts_start);
	SPI_LOCK(priv->spi, true);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, true);
	SPI_RECVBLOCK(priv->spi, rxbuf->data, size);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, false);
	SPI_LOCK(priv->spi, false);
	clock_gettime(CLOCK_MONOTONIC, &ts_end);
	elapsed_time = (int)(ts_end.tv_sec - ts_start.tv_sec) * 1000 + (int)(ts_end.tv_nsec - ts_start.tv_nsec) / 1000000;
	lldbg("elapsed time : %d\n", elapsed_time);

	/* TODO Should we handle Header & checksum here?? */
	time_stamp = (uint16_t)ts_end.tv_sec * 1000 + (uint16_t)(ts_end.tv_nsec) / 1000000;
	lldbg("priv->timestamp :%d cur time_stamp : %d\n", priv->timestamp, time_stamp);

	if (priv->mq == NULL) {
		lldbg("mq is not registered!!!\n");
		return;
	}

	if (time_stamp > INT_MAX) { // TODO this is temp value!
		rxbuf->msgId = IWRL6432_MSG_UNDERRUN;
	} else if (priv->enable == false) {
		rxbuf->msgId = IWRL6432_MSG_STOP_FORCELY;
	} else {
		rxbuf->msgId = IWRL6432_MSG_DEQUEUE;
	}
	/* add last to doneq & sempost. then increase req_num */
	sq_addlast((sq_entry_t *)&rxbuf->entry, &priv->doneq);
	sem_post(&priv->datasem);
	priv->req_num++;
	if (priv->req_num > IWRL6432_BUF_NUM) {
		priv->req_num = 1;
	}
	if (priv->running) {
		priv->config->irq_enable(true);
		iwrl6432_request_cube(priv);
	}
}

static void iwrl6432_work_handler(void *arg)
{
	struct iwrl6432_dev_s *priv = (struct iwrl6432_dev_s *)arg;
	/* Handle cube data here */
	iwrl6432_request_cube_data(priv);
}
/****************************************************************************
 * Name: iwrl6432_semgive
 * description:
 *  Handle iwrl6432's DataReady interrupt
 ****************************************************************************/
static void iwrl6432_interrupt_handler(void *arg)
{
	struct iwrl6432_dev_s *priv = (struct iwrl6432_dev_s *)arg;
	/* If it's not running, then handle reset routine here */
	if (!priv->running) {
		lldbg("Interrupt handler!! but it's not running so ignore\n");
		return;
	}
	iwrl6432_semgive(&priv->datasem);
}

/****************************************************************************
 * Name: iwrl6432_open
 ****************************************************************************/
static int iwrl6432_open(FAR struct file *filep)
{
	struct iwrl6432_dev_s *priv = (struct iwrl6432_dev_s *)filep->f_inode->i_private;
	uint8_t tmp;
	int ret;
	if (!priv) {
		return -EINVAL;
	}

	iwrl6432_semtake(&priv->exclsem, false);

	tmp = priv->crefs + 1;
	if (tmp == 0) {
		ret = -EMFILE;
		goto error_with_sem;
	}
	priv->crefs = tmp;

	if (priv->crefs == 1) {
		priv->mq = NULL;
	}
	ret = OK;
error_with_sem:
	iwrl6432_semgive(&priv->exclsem);
	return ret;
}

/****************************************************************************
 * Name: iwrl6432_close
 ****************************************************************************/
static int iwrl6432_close(FAR struct file *filep)
{
	struct iwrl6432_dev_s *priv = (struct iwrl6432_dev_s *)filep->f_inode->i_private;

	if (!priv) {
		return -EINVAL;
	}

	iwrl6432_semtake(&priv->exclsem, false);
	DEBUGASSERT(priv->crefs > 0);
	priv->crefs--;
	iwrl6432_semgive(&priv->exclsem);
	return OK;
}

/****************************************************************************
 * Name: iwrl6432_read
 ****************************************************************************/
static ssize_t iwrl6432_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
	return OK;
}

/****************************************************************************
 * Name: iwrl6432_write
 ****************************************************************************/
static ssize_t iwrl6432_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
	return OK;
}

static int iwrl6432_flush(FAR struct iwrl6432_dev_s *priv)
{
	/* TODO Should we dequeue all buffer from pendq and reinit here ? */
	priv->timestamp = 0;
	return OK;
}

static int iwrl6432_enqueue_data(FAR struct iwrl6432_dev_s *priv, struct iwrl6432_buf_s *buf)
{
	lldbg("buf : %x\n", buf);
	sq_addlast((sq_entry_t *)&buf->entry, &priv->pendq);
	return OK;
}

static int iwrl6432_start(FAR struct iwrl6432_dev_s *priv)
{
	if (priv->mq == NULL) {
		lldbg("mq is null...\n");
		return -ENOTTY;
	}
	priv->running = true;
	priv->req_num = 1;
	priv->config->irq_enable(true);
	iwrl6432_request_cube(priv);
	return OK;
}

static int iwrl6432_stop(FAR struct iwrl6432_dev_s *priv)
{
	int ret = OK;
	priv->config->irq_enable(false);
	priv->running = 0;

	sq_entry_t *tmp;
	/* Remove data from pendq  */
	while ((tmp = sq_remfirst(&priv->pendq)) != NULL) {
		lldbg("removed buffer : %x\n", tmp);
	}
	ret = iwrl6432_flush(priv);
	if (ret != OK) {
		lldbg("flush failed..\n");
	}

	priv->req_num = 1;

	return OK;
}

static int iwrl6432_prepare(FAR struct iwrl6432_dev_s *priv)
{
	priv->timestamp = 0;
	priv->req_num = 0;
	return OK;
}

/****************************************************************************
 * Name: iwrl6432_ioctl
 *
 * Description:
 *   This IOCTL is used to configure the iwrl6432
 *
 ****************************************************************************/
static int iwrl6432_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct iwrl6432_dev_s *priv;
	int ret = OK;
	priv = (struct iwrl6432_dev_s *)filep->f_inode->i_private;
	if (!priv) {
		return -EINVAL;
	}

	iwrl6432_semtake(&priv->exclsem, false);

	switch (cmd) {
	case SNIOC_GET_BUFSIZE: {
		*((int *)arg) = IWRL6432_DATA_SIZE;
	}
	break;
	case SNIOC_GET_BUFNUM: {
		*((int *)arg) = IWRL6432_BUF_NUM;
	}
	break;
	case SNIOC_PREPARE: {
		/* Here we can alloc buffer & make iwrl6432 ready */
		ret = iwrl6432_prepare(priv);
	}
	break;
	case SNIOC_REGISTERMQ: {
		if (arg == 0) {
			lldbg("Error : arg(mq) is null!!\n");
			ret = -EINVAL;
			goto error_with_sem;
		}
		priv->mq = (mqd_t)arg;
	}
	break;
	case SNIOC_START: {
		if (!priv->enable) {
			ret = ESHUTDOWN;
			lldbg("Error : iwrl6432 disabled now!!\n");
			goto error_with_sem;
		}
		ret = iwrl6432_start(priv);
	}
	break;
	case SNIOC_STOP: {
		if (!priv->running) {
			ret = EPERM;
			lldbg("Error : iwrl6432 is not running!!\n");
			goto error_with_sem;
		}
		ret = iwrl6432_stop(priv);
	}
	break;
	case SNIOC_SENDBUFFER: {
		if (arg == 0) {
			lldbg("Error : arg(mq) is null!!\n");
			ret = -EINVAL;
			goto error_with_sem;
		}
		struct iwrl6432_buf_s *buf = (FAR struct iwrl6432_buf_s *)arg;
		ret = iwrl6432_enqueue_data(priv, buf);
	}
	break;
	default: {
		lldbg("invalid value : %d\n", cmd);
		ret = -EINVAL;
	}
	break;
	}
error_with_sem:
	iwrl6432_semgive(&priv->exclsem);
	return ret;
}

void irwl6432_set_state(bool enable)
{
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	int ret = OK;
	struct iwrl6432_msg_s msg;

	iwrl6432_semtake(&priv->exclsem, false);
	auddbg("set Record enable : %d\n", enable);
	if (priv->enable == enable) {
		iwrl6432_semgive(&priv->exclsem);
		return;
	}

	priv->enable = enable;
	priv->config->irq_enable(enable);
	if (priv->running) {
		priv->running = false;
		ret = iwrl6432_stop(priv);
		if (ret != OK) {
			lldbg("try to stop forcely but failed!!\n");
		}
		msg.msgId = IWRL6432_MSG_STOP_FORCELY;
	} else {
		msg.msgId = IWRL6432_MSG_READY_TO_USE;
	}
	
	ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO);
	if (ret != OK) {
		lldbg("mq_send failed!!\n");
	}
	iwrl6432_semgive(&priv->exclsem);
}

/****************************************************************************
 * Name:  irwl6432_init
 *
 ****************************************************************************/
static int irwl6432_init(FAR struct iwrl6432_dev_s *priv)
{
	priv->config->reset();
	const char *commandParams = {"sensorStop 0\nchannelCfg 7 3 0\nchirpComnCfg 15 5 0 128 1 34 3\nchirpTimingCfg 6 26 1 47.95 60\nframeCfg 2 0 600 64 200 0\nantGeometryCfg 0 0 1 1 0 2 0 1 1 2 0 3 2.418 2.418\nguionitor 0 0 0 0 0 0 0 0 0 0 0\nsigProcChainCfg 32 2 1 0 64 1 0 0.3\ncfarCfg 2 8 4 3 0 7.0 0 0.5 0 1 1 1\naoaFovCfg -70 70 -40 40\nrangeSelCfg 1.2 10.0\nclutterRemoval 0\ncompRangeBiasAndRxChanPhase 0.0 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000\nadcDataSource 0 adc_data_0001_CtestAdc6Ant.bin\nadcLogging 2\nlowPowerCfg 0\nfactoryCalibCfg 1 0 36 3 0x1ff000\nsensorStart 0 0 0 0\n"};
	uint8_t spiData[PARAMETER_TX_SIZE];
	int parameter_packet_length = 1025;
	for (int i = 0; i < PARAMETER_TX_SIZE; i++) {
		spiData[i] = 0;
	}
	spiData[0] = PAYLOAD_HEADER;
	spiData[1] = PAYLOAD_SOF;
	spiData[2] = parameter_packet_length & 0xFF;
	spiData[3] = (parameter_packet_length >> 8) & 0xFF;
	spiData[4] = (parameter_packet_length >> 16) & 0xFF;
	spiData[5] = (parameter_packet_length >> 24) & 0xFF;
	spiData[6] = 0;
	memcpy(spiData + 7, commandParams, strlen(commandParams));
	spiData[PARAMETER_TX_SIZE - 2] = checksum_cal(spiData, 1, PARAMETER_TX_SIZE - 2);
	spiData[PARAMETER_TX_SIZE - 1] = PAYLOAD_TAIL;

	SPI_LOCK(priv->spi, true);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, true);
	SPI_SNDBLOCK(priv->spi, spiData, PARAMETER_TX_SIZE);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, false);
	SPI_LOCK(priv->spi, false);

	uint8_t param_ack[41];
	up_mdelay(100);

	SPI_LOCK(priv->spi, true);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, true);
	SPI_RECVBLOCK(priv->spi, param_ack, 41);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, false);
	SPI_LOCK(priv->spi, false);

	char *ack_str = (char *)kmm_malloc(33);
	if (!ack_str) {
		lldbg("Failed to allocate memory for ack string\n");
	}
	memcpy(ack_str, param_ack + 7, 32);
	ack_str[32] = '\0';
	lldbg("irwl6432 init %s\n", ack_str);
	kmm_free(ack_str);
	/* set req_num as 2 */
	priv->req_num = 1;
	return OK;
}

static int iwrl6432_mq_thread(int argc, char **agrv)
{
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	struct iwrl6432_msg_s msg;
	size_t size;
	struct iwrl6432_buf_s *buf;
	int ret;
	while (1) {
		iwrl6432_semtake(&priv->datasem, false);
		if (!priv->running) {
			continue;
		}
		iwrl6432_request_cube_data(priv);
		/* Data is ready, here it will collect data from iwrl6432 through the SPI */
		if (work_available(&priv->work)) {
			/* TODO many module using HPWORK now, let's consider custom thread */
			ret = work_queue(HPWORK, &priv->work, iwrl6432_send_result, priv, 0);
			if (ret != 0) {
				lldbg("Error, failed to queue work queue ret : %d errno : %d\n", ret, errno);
			}
		}

		usleep(10);
	}
	return OK;
}

int irwl6432_register(FAR const char *devname, FAR struct iwrl6432_config_s *config, FAR struct spi_dev_s *spi)
{
	int ret;
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	priv->spi = spi;
	priv->config = config;
	priv->crefs = 0;
	priv->enable = true;
	priv->running = false;

	/* init semaphore */
	sem_init(&priv->exclsem, 0, 1);
	sem_init(&priv->datasem, 0, 1);
	ret = priv->config->attach(iwrl6432_interrupt_handler, (FAR void *)priv);
	if (ret < 0) {
		sem_destroy(&priv->exclsem);
		lldbg("irwl6432 interrupt handler registration failed\n");
		return ret;
	}

	sq_init(&priv->pendq);
	sq_init(&priv->doneq);

	irwl6432_init(priv);

	pid_t pid = kernel_thread("iwrl6432_mq_thread", 130, 4096, (main_t)iwrl6432_mq_thread, NULL);
	if (pid < 0) {
		lldbg("iwrl6432_mq_thread thread creation failed\n");
		return -1;
	}

	ret = register_driver(devname, &g_iwrl6432_fileops, 0666, priv);
	if (ret < 0) {
		sem_destroy(&priv->exclsem);
		lldbg("irwl6432 Driver registration failed\n");
		return ret;
	}

	lldbg("irwl6432 Driver registered Successfully\n");
	return OK;
}

