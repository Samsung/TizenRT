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
 * drivers/sensors/iwrl6432.c
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
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
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
#include <tinyara/arch.h>

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
	sem_t acksem;
	int crefs;
	mqd_t mq;
	bool enable;
	bool running;
	struct sq_queue_s pendq;
	struct sq_queue_s doneq;
	struct work_s work;
	struct work_s irq_work;
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
static int iwrl6432_stop(FAR struct iwrl6432_dev_s *priv);
static void iwrl6432_show(FAR struct iwrl6432_dev_s *priv, FAR uint16_t *buf);
static int iwrl6432_init(FAR struct iwrl6432_dev_s *priv);
static void iwrl6432_spi_transfer(FAR struct iwrl6432_dev_s *priv, FAR char *txbuf, FAR char *rxbuf, size_t len);
static void make_param_packet(uint8_t *spiData);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_iwrl6432_fileops = {
	iwrl6432_open,	/* open */
	iwrl6432_close, /* close */
	iwrl6432_read,	/* read */
	iwrl6432_write, /* write */
	NULL,			/* seek */
	iwrl6432_ioctl, /* ioctl */
};

static FAR struct iwrl6432_dev_s g_iwrl6432_priv;
static uint16_t cloud_frame_id;
static uint8_t total_checksum_error;
static uint8_t total_timeout_count;
static uint16_t last_succeed_fid;
static uint8_t total_frame_mismatch_count;

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

/****************************************************************************
 * Name:  make_param_packet
 *
 ****************************************************************************/

static void make_param_packet(uint8_t *spiData)
{
	int fd = -1; // Initialize to invalid descriptor
	char *param_path = "/mnt/kernel/sensors/param_data.txt";
	struct stat file_stat;
	const char *default_command = {"sensorStop 0\nchannelCfg 7 3 0\nchirpComnCfg 15 5 0 128 1 34 3\nchirpTimingCfg 6 26 1 47.95 60\nframeCfg 2 0 600 64 200 0\nantGeometryCfg 0 0 1 1 0 2 0 1 1 2 0 3 2.418 2.418\nguiMonitor 0 0 0 0 0 0 0 0 0 0 0\nsigProcChainCfg 32 2 1 0 64 1 0 0.3\ncfarCfg 2 8 4 3 0 7.0 0 0.5 0 1 1 1\naoaFovCfg -70 70 -40 40\nrangeSelCfg 1.2 10.0\nclutterRemoval 0\ncompRangeBiasAndRxChanPhase 0.0 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000 1.00000 0.00000 -1.00000 0.00000\nadcDataSource 0 adc_data_0001_CtestAdc6Ant.bin\nadcLogging 2\nlowPowerCfg 0\nfactoryCalibCfg 1 0 36 3 0x1ff000\nsensorStart 0 0 0 0\n"};
	if (stat(param_path, &file_stat) != 0) {
		mmwavedbg("File not found/accessible: %s, Error: %d\n", param_path, get_errno());
		goto use_default;
	}

	if (file_stat.st_size <= 0) {
		mmwavedbg("Invalid file size: %ld\n", (long)file_stat.st_size);
		goto use_default;
	}

	if ((fd = open(param_path, O_RDONLY)) == -1) {
		mmwavedbg("Open failed: %d\n", get_errno());
		goto use_default;
	}

	char *param_data = kmm_malloc(file_stat.st_size + 1);
	if (!param_data) {
		mmwavedbg("Memory allocation failed\n");
		goto use_default;
	}

	ssize_t total_read = 0;
	while (total_read < file_stat.st_size) {
		ssize_t bytes_read = read(fd, param_data + total_read, file_stat.st_size - total_read);
		if (bytes_read <= 0) {
			mmwavedbg("Read error: %d\n", get_errno());
			if (param_data) {
				kmm_free(param_data);
				param_data = NULL;
			}
			goto use_default;
		}
		total_read += bytes_read;
	}
	param_data[file_stat.st_size] = '\0';

	// Safe copy to SPI buffer
	memcpy(spiData + 7, param_data, file_stat.st_size);
	if (param_data) {
		kmm_free(param_data);
		param_data = NULL;
	}
	param_data = NULL;
	goto cleanup;

use_default:
	// Handle default command case
	memcpy(spiData + 7, default_command, strlen(default_command));

cleanup:
	if (fd != -1) {
		close(fd);
	}

	spiData[0] = PAYLOAD_HEADER;
	spiData[1] = PAYLOAD_SOF;
	spiData[2] = PARAMETER_PACKET_LENGTH & 0xFF;
	spiData[3] = (PARAMETER_PACKET_LENGTH >> 8) & 0xFF;
	spiData[4] = (PARAMETER_PACKET_LENGTH >> 16) & 0xFF;
	spiData[5] = (PARAMETER_PACKET_LENGTH >> 24) & 0xFF;
	spiData[6] = 0;
	spiData[PARAMETER_TX_SIZE - 2] = checksum_cal(spiData, 1, PARAMETER_TX_SIZE - 2);
	spiData[PARAMETER_TX_SIZE - 1] = PAYLOAD_TAIL;
}

/****************************************************************************
 * Name:  make_ack
 *
 ****************************************************************************/

static void make_ack(uint8_t request_number, uint8_t *ack_tx_buf, int ack_bit)
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
	ack_tx_buf[12] = ack_bit;

	// Insert request number
	ack_tx_buf[13] = request_number;

	// Calculate checksum from index 1 to ACK_TX_SIZE-3 (excludes last 3 bytes)
	uint8_t checksum = checksum_cal(ack_tx_buf, 1, ACK_TX_SIZE - 2);
	ack_tx_buf[14] = checksum;

	// Set packet tail
	ack_tx_buf[15] = PAYLOAD_TAIL;
}

static void iwrl6432_spi_transfer(FAR struct iwrl6432_dev_s *priv, FAR char *txbuf, FAR char *rxbuf, size_t len)
{
	SPI_SETMODE(priv->spi, priv->config->spi_config.mode);
	SPI_SETFREQUENCY(priv->spi, priv->config->spi_config.freq);
	SPI_SETBITS(priv->spi, priv->config->spi_config.bpw);
	SPI_LOCK(priv->spi, true);
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, true);
	if (txbuf != NULL) {
		SPI_SNDBLOCK(priv->spi, txbuf, len);
	} else {
		SPI_RECVBLOCK(priv->spi, rxbuf, len);
	}
	SPI_SELECT(priv->spi, priv->config->spi_config.cs, false);
	SPI_LOCK(priv->spi, false);
}

static int iwrl6432_send_result(FAR struct iwrl6432_dev_s *priv)
{
	struct iwrl6432_msg_s msg;
	struct iwrl6432_buf_s *buf;
	int ret = OK;
	while (sq_peek(&priv->doneq) != NULL) {
		buf = (struct iwrl6432_buf_s *)sq_remfirst(&priv->doneq);
		msg.msgId = buf->msgId;
		msg.pData = (FAR void *)buf;
		ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO);
		if (ret != OK) {
			mmwavedbg("mq_send error...errno : %d\n", errno);
		}
	}
	return ret;
}

static void iwrl6432_request_cube(FAR struct iwrl6432_dev_s *priv, int ack_bit, bool purge)
{
	if (ack_bit == IWRL6432_ACK && !purge) {
		priv->req_num++;
		if (priv->req_num > IWRL6432_BUF_NUM) {
			priv->req_num = 1;
		}
	}
	if (purge) { // if purge is true, reset req_num to 0
		priv->req_num = 0;
	}
	uint8_t ack_tx_buf[ACK_TX_SIZE];
	make_ack(priv->req_num, ack_tx_buf, ack_bit);
	iwrl6432_spi_transfer(priv, ack_tx_buf, NULL, ACK_TX_SIZE);
}

static void iwrl6432_request_cube_data(FAR struct iwrl6432_dev_s *priv)
{
	struct iwrl6432_buf_s *rxbuf;
	uint16_t time_stamp;
	size_t size;
	struct timespec ts_start, ts_end;
	int elapsed_time;
	int ret;
	if (sq_empty(&priv->pendq)) {
		mmwavedbg("Error!!! queue is empty\n");
		return;
	}
	if (priv->config->ready_pin_status() == 1) { // if ready pin is high, do not send request
		ret = sem_tickwait(&priv->acksem, clock_systimer(), MSEC2TICK(8000));
		if (ret < 0) {
			int err = get_errno();
			if (err == ETIMEDOUT) {
				total_timeout_count++;
				mmwavedbg("#####################Timeout###################\n");
				return;
			}
			mmwavedbg("Error in waiting for ack semaphore\n");
			return;
		}
		iwrl6432_request_cube(priv, IWRL6432_NACK, false);
		return;
	}
	rxbuf = (struct iwrl6432_buf_s *)sq_remfirst(&priv->pendq);
	if (priv->req_num == 1) {
		size = CLOUD_TX_SIZE;
	} else if (priv->req_num == 8) {
		size = TRACK_DATA_SIZE;
	} else {
		size = IWRL6432_CUBE_DATA_SIZE;
	}
	clock_gettime(CLOCK_MONOTONIC, &ts_start);
	iwrl6432_spi_transfer(priv, NULL, rxbuf->data, size);
	clock_gettime(CLOCK_MONOTONIC, &ts_end);
	elapsed_time = (int)(ts_end.tv_sec - ts_start.tv_sec) * 1000 + (int)(ts_end.tv_nsec - ts_start.tv_nsec) / 1000000;
	if (rxbuf->data[6] == 1) {
		cloud_frame_id = (rxbuf->data[12] | rxbuf->data[13] << 8);
	}
	last_succeed_fid = rxbuf->data[12] | rxbuf->data[13] << 8;
	ret = sem_tickwait(&priv->acksem, clock_systimer(), MSEC2TICK(55));
	if (ret < 0) {
		int err = get_errno();
		if (err == ETIMEDOUT) {
			iwrl6432_semtake(&priv->exclsem, false);
			if (!priv->running) {
				iwrl6432_semgive(&priv->exclsem);
				return;
			}
			iwrl6432_semgive(&priv->exclsem);
			total_timeout_count++;
			mmwavedbg("#####################Timeout while waiting to send req_num:%d###################\n", priv->req_num + 1);
			struct iwrl6432_msg_s msg;
			msg.msgId = IWRL6432_MSG_TIMEOUT;
			if (mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO) != OK) {
				mmwavedbg("mq_send failed!!\n");
			}
			if (iwrl6432_stop(priv) != OK) {
				mmwavedbg("try to stop forcely but failed!!\n");
			}
			sem_destroy(&priv->datasem);
			sem_destroy(&priv->acksem);
			if (iwrl6432_init(priv) == OK) {
				msg.msgId = IWRL6432_MSG_READY_TO_USE;
				if (mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO) != OK) {
					mmwavedbg("mq_send failed!!\n");
				}
			} else {
				mmwavedbg("iwrl6432 init failed!!\n");
			}
			return;
		}
		mmwavedbg("Error in waiting for ack semaphore\n");
		return;
	}
	uint32_t length = (rxbuf->data[2]) | (rxbuf->data[3] << 8) | (rxbuf->data[4] << 16) | (rxbuf->data[5] << 24);
	uint8_t checksum = rxbuf->data[length + 5];
	uint8_t cksm = checksum_cal(rxbuf->data, 1, length + 5);
	if (checksum != cksm) {
		mmwavedbg("############################ Checksum Error ###########################\n");
		total_checksum_error++; // Increase checksum error count
		sq_addlast((sq_entry_t *)&rxbuf->entry, &priv->pendq);
		iwrl6432_request_cube(priv, IWRL6432_NACK, false); // Send NACK
		return;
	}
	if (cloud_frame_id != (rxbuf->data[12] | rxbuf->data[13] << 8)) {
		mmwavedbg("Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! frame id mismatch expected:%d received:%d\n", cloud_frame_id, (rxbuf->data[12] | rxbuf->data[13] << 8));
		total_frame_mismatch_count++;
		sq_addlast((sq_entry_t *)&rxbuf->entry, &priv->pendq);
		iwrl6432_request_cube(priv, IWRL6432_ACK, true); // Purge request
		up_mdelay(1);
		iwrl6432_request_cube(priv, IWRL6432_ACK, false); // Send ACK for cloud point
		return;
	}
	if (elapsed_time > INT_MAX) { // TODO this is temp value!
		rxbuf->msgId = IWRL6432_MSG_UNDERRUN;
	} else {
		rxbuf->msgId = IWRL6432_MSG_DEQUEUE;
	}
	/* add last to doneq then increase req_num */
	sq_addlast((sq_entry_t *)&rxbuf->entry, &priv->doneq);

	iwrl6432_semtake(&priv->exclsem, false);
	if (priv->running) { // if running is false, no need to send next request
		iwrl6432_semgive(&priv->exclsem);
		iwrl6432_request_cube(priv, IWRL6432_ACK, false);
	} else {
		iwrl6432_semgive(&priv->exclsem);
	}
}

static void iwrl6432_work_handler(void *arg)
{
	int ret;
	int sem_cnt;
	sem_t *sem = (sem_t *)arg;
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	iwrl6432_semtake(&priv->exclsem, false);
	if (!priv->enable) { // if forcefully paused, ignore interrupt
		iwrl6432_semgive(&priv->exclsem);
		return;
	}
	iwrl6432_semgive(&priv->exclsem);
	ret = sem_getvalue(sem, &sem_cnt);
	if (ret == OK && sem_cnt < 1) {
		sem_post(sem);
	}
}
/****************************************************************************
 * Name: iwrl6432_semgive
 * description:
 *  Handle iwrl6432's DataReady interrupt
 ****************************************************************************/
static void iwrl6432_interrupt_handler(void *arg, bool state)
{
	struct iwrl6432_dev_s *priv = (struct iwrl6432_dev_s *)arg;
	if (work_available(&priv->irq_work)) {
		if (!state) {
			work_queue(HPWORK, &priv->irq_work, iwrl6432_work_handler, &priv->datasem, 0);
		} else {
			work_queue(HPWORK, &priv->irq_work, iwrl6432_work_handler, &priv->acksem, 0);
		}
	}
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

static void iwrl6432_show(FAR struct iwrl6432_dev_s *priv, FAR uint16_t *buf)
{
	buf[0] = last_succeed_fid;
	buf[1] = total_checksum_error;
	buf[2] = total_timeout_count;
	buf[3] = total_frame_mismatch_count;
}

static int iwrl6432_enqueue_data(FAR struct iwrl6432_dev_s *priv, struct iwrl6432_buf_s *buf)
{
	mmwavevdbg("buf : %x\n", buf);
	sq_addlast((sq_entry_t *)&buf->entry, &priv->pendq);
	return OK;
}

static int iwrl6432_start(FAR struct iwrl6432_dev_s *priv)
{
	if (priv->mq == NULL) {
		mmwavedbg("mq is null...\n");
		return -ENOTTY;
	}
	priv->running = true;
	priv->req_num = 0;
	iwrl6432_request_cube(priv, IWRL6432_ACK, false);
	return OK;
}

static int iwrl6432_stop(FAR struct iwrl6432_dev_s *priv)
{
	int ret = OK;
	priv->running = false;
	sq_entry_t *tmp;
	/* Remove data from pendq  */
	while ((tmp = sq_remfirst(&priv->pendq)) != NULL) {
		mmwavevdbg("removed buffer from pendq: %x\n", tmp);
	}
	/* Remove data from doneq  */
	while ((tmp = sq_remfirst(&priv->doneq)) != NULL) {
		mmwavevdbg("removed buffer from doneq: %x\n", tmp);
	}
	ret = iwrl6432_flush(priv);
	if (ret != OK) {
		mmwavedbg("flush failed..\n");
	}

	priv->req_num = 0;

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
		*((int *)arg) = IWRL6432_CUBE_DATA_SIZE;
	} break;
	case SNIOC_GET_BUFNUM: {
		*((int *)arg) = IWRL6432_BUF_NUM;
	} break;
	case SNIOC_PREPARE: {
		/* Here we can alloc buffer & make iwrl6432 ready */
		ret = iwrl6432_prepare(priv);
	} break;
	case SNIOC_REGISTERMQ: {
		if (arg == 0) {
			mmwavedbg("Error : arg(mq) is null!!\n");
			ret = -EINVAL;
			goto error_with_sem;
		}
		priv->mq = (mqd_t)arg;
	} break;
	case SNIOC_START: {
		if (!priv->enable) {
			ret = ESHUTDOWN;
			mmwavedbg("Error : iwrl6432 disabled now!!\n");
			goto error_with_sem;
		}
		ret = iwrl6432_start(priv);
	} break;
	case SNIOC_STOP: {
		if (!priv->running) {
			ret = EPERM;
			mmwavedbg("Error : iwrl6432 is not running!!\n");
			goto error_with_sem;
		}
		ret = iwrl6432_stop(priv);
	} break;
	case SNIOC_SENDBUFFER: {
		if (arg == 0) {
			mmwavedbg("Error : arg(mq) is null!!\n");
			ret = -EINVAL;
			goto error_with_sem;
		}
		struct iwrl6432_buf_s *buf = (FAR struct iwrl6432_buf_s *)arg;
		ret = iwrl6432_enqueue_data(priv, buf);
	} break;
	case SNIOC_SHOW: {
		FAR uint16_t *buffer = (FAR uint16_t *)arg;
		iwrl6432_show(priv, buffer);
	} break;
	default: {
		mmwavedbg("invalid value : %d\n", cmd);
		ret = -EINVAL;
	} break;
	}
error_with_sem:
	iwrl6432_semgive(&priv->exclsem);
	return ret;
}

void iwrl6432_set_state(bool enable)
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
	if (!priv->enable) {
		priv->running = false;
		iwrl6432_stop(priv);
		msg.msgId = IWRL6432_MSG_STOP_FORCELY;
		ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO);
		if (ret != OK) {
			mmwavedbg("mq_send failed!!\n");
		}
	} else {
		msg.msgId = IWRL6432_MSG_READY_TO_USE;
		ret = mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO);
		if (ret != OK) {
			mmwavedbg("mq_send failed!!\n");
		}
	}
	iwrl6432_semgive(&priv->exclsem);
}

/****************************************************************************
 * Name:  iwrl6432_init
 *
 ****************************************************************************/
static int iwrl6432_init(FAR struct iwrl6432_dev_s *priv)
{
	int ret;
	uint8_t spiData[PARAMETER_TX_SIZE] = {0};
	make_param_packet(spiData);

	char *ack_str = (char *)kmm_malloc(33);
	if (!ack_str) {
		mmwavedbg("Failed to allocate memory for ack string\n");
		return ERROR;
	}

	for (int i = 0; i < IWRL6432_MAX_INIT_RETRY_COUNT; i++) { // retry init up to IWRL6432_MAX_INIT_RETRY_COUNT times until it succeeds.

		priv->config->reset();
		priv->enable = true;
		sem_init(&priv->acksem, 0, 0);
		sem_init(&priv->datasem, 0, 0);
		iwrl6432_spi_transfer(priv, spiData, NULL, PARAMETER_TX_SIZE);

		uint8_t param_ack[41];
		ret = sem_tickwait(&priv->datasem, clock_systimer(), MSEC2TICK(1000));
		if (ret < 0) {
			int err = get_errno();
			if (err == ETIMEDOUT) {
				mmwavedbg("#############Timeout during init(SPI_Ready stuck high) ##############\n");
				continue;
			}
		}
		iwrl6432_spi_transfer(priv, NULL, param_ack, 41);
		ret = sem_tickwait(&priv->acksem, clock_systimer(), MSEC2TICK(1000)); // wait for ack from sensor
		if (ret < 0) {
			int err = get_errno();
			if (err == ETIMEDOUT) {
				mmwavedbg("#############Timeout during init(SPI_READY stuck low)##############\n");
				continue;
			}
		}
		memcpy(ack_str, param_ack + 7, 32);
		ack_str[32] = '\0';
		mmwavevdbg("iwrl6432 init %s\n", ack_str);
		if (strncmp(ack_str, "Done", 4)) {
			continue;
		}
		if (ack_str) {
			kmm_free(ack_str);
			ack_str = NULL;
		}
		priv->req_num = 0;
		return OK;
	}
	if (ack_str) {
		kmm_free(ack_str);
		ack_str = NULL;
	}
	return ERROR;
}

static int iwrl6432_mq_thread(int argc, char **agrv)
{
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	struct iwrl6432_msg_s msg;
	size_t size;
	struct iwrl6432_buf_s *buf;
	int ret;
	bool enable;
	ret = iwrl6432_init(priv);
	if (ret != OK) {
		sem_destroy(&priv->datasem);
		sem_destroy(&priv->acksem);
		mmwavedbg("iwrl6432 init failed\n");
		return ret;
	}

	while (1) {
		ret = sem_tickwait(&priv->datasem, clock_systimer(), MSEC2TICK(400));
		if (ret < 0) {
			int err = get_errno();
			if (err == ETIMEDOUT) {
				iwrl6432_semtake(&priv->exclsem, false);
				if (!priv->running) {
					iwrl6432_semgive(&priv->exclsem);
					continue;
				}
				iwrl6432_semgive(&priv->exclsem);
				total_timeout_count++;
				mmwavedbg("##############Timeout while waiting to receive req_num:%d##################\n", priv->req_num);
				struct iwrl6432_msg_s msg;
				msg.msgId = IWRL6432_MSG_TIMEOUT;
				if (mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO) != OK) {
					mmwavedbg("mq_send failed!!\n");
				}
				if (iwrl6432_stop(priv) != OK) {
					mmwavedbg("try to stop forcely but failed!!\n");
				}
				sem_destroy(&priv->datasem);
				sem_destroy(&priv->acksem);
				if (iwrl6432_init(priv) == OK) {
					msg.msgId = IWRL6432_MSG_READY_TO_USE;
					if (mq_send(priv->mq, (FAR const char *)&msg, sizeof(msg), CONFIG_IWRL6432_SG_DEQUEUE_PRIO) != OK) {
						mmwavedbg("mq_send failed!!\n");
					}
				} else {
					mmwavedbg("iwrl6432 init failed!!\n");
					return ERROR;
				}
				continue;
			}
			mmwavedbg("Error in waiting for data semaphore\n");
			continue;
		}

		iwrl6432_request_cube_data(priv);
		/* Data is ready, here it will collect data from iwrl6432 through the SPI */
		if (work_available(&priv->work)) {
			/* TODO many module using HPWORK now, let's consider custom thread */
			ret = work_queue(HPWORK, &priv->work, iwrl6432_send_result, priv, 0);
			if (ret != 0) {
				mmwavedbg("Error, failed to queue work queue ret : %d errno : %d\n", ret, errno);
			}
		}
	}
	return OK;
}

int iwrl6432_register(FAR const char *devname, FAR struct iwrl6432_config_s *config, FAR struct spi_dev_s *spi)
{
	int ret;
	struct iwrl6432_dev_s *priv = &g_iwrl6432_priv;
	priv->spi = spi;
	priv->config = config;
	priv->crefs = 0;
	priv->enable = false;
	priv->running = false;
	last_succeed_fid = 0;
	total_checksum_error = 0;
	total_timeout_count = 0;
	total_frame_mismatch_count = 0;

	/* init semaphore */
	sem_init(&priv->exclsem, 0, 1);
	ret = priv->config->attach(iwrl6432_interrupt_handler, (FAR void *)priv);
	if (ret < 0) {
		sem_destroy(&priv->exclsem);
		mmwavedbg("iwrl6432 interrupt handler registration failed\n");
		return ret;
	}

	sq_init(&priv->pendq);
	sq_init(&priv->doneq);

	pid_t pid = kernel_thread("iwrl6432_mq_thread", 130, 5120, (main_t)iwrl6432_mq_thread, NULL);
	if (pid < 0) {
		mmwavedbg("iwrl6432_mq_thread thread creation failed\n");
		sem_destroy(&priv->exclsem);
		return -1;
	}

	ret = register_driver(devname, &g_iwrl6432_fileops, 0666, priv);
	if (ret < 0) {
		mmwavedbg("iwrl6432 Driver registration failed\n");
		sem_destroy(&priv->exclsem);
		sem_destroy(&priv->datasem);
		sem_destroy(&priv->acksem);
		task_delete(pid); // delete thread created above
		return ret;
	}

	mmwavevdbg("iwrl6432 Driver registered Successfully\n");
	return OK;
}
