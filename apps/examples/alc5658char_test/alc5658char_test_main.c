/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * apps/examples/alc5658char_test/alc5658char_test_main.c
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
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
#include <tinyara/audio/audio.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#include <fcntl.h>
#include <debug.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define BPSMPL	2

#define BUFFSIZE (1024 * 16)

#ifndef CONFIG_EXAMPLES_ALC5658CHAR_RXSTACKSIZE
#define CONFIG_EXAMPLES_ALC5658CHAR_RXSTACKSIZE 2048
#endif

#ifndef CONFIG_EXAMPLES_ALC5658CHAR_TXSTACKSIZE
#define CONFIG_EXAMPLES_ALC5658CHAR_TXSTACKSIZE 2048
#endif

#define LOOPBACK_CNT 200
#define RECORD_CNT 20

#define DEVICE_PATH "/dev/alc5658char0"
/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/
enum alctest_mode {
	MODE_LOOPBACK = 0,
	MODE_RECORD,
};

static struct ap_buffer_s *rx_apb;
sem_t tx_start_sem;
sem_t rx_done_sem;
void *buff;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Name: i2schar_receiver()
 *
 * Description:
 *   This is the entry point for the receiver thread.
 *
 ****************************************************************************/

pthread_addr_t alc5658char_receiver(void *arg)
{
	int fd;
	int pcm_fd;
	int i = 0;
	int errcode;
	int pos = 0;
	int nwritten;
	int mode = *(int *)arg;

	FAR struct audio_buf_desc_s bufdesc;
	char sign[4] = {'|', '/', '-', '\\'};

	/* Open the alc character device */
	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		errcode = errno;
		printf("failed to open file, %d\n", errcode);
		return NULL;
	}

	if (mode == MODE_LOOPBACK)
		i = LOOPBACK_CNT;
	else if (mode == MODE_RECORD)
		i = RECORD_CNT;

	while (i--) {
		printf("Now Recording.. ");
		printf("%c\r", sign[i%4]);

		/* Define buffer size, if not defined 16KB buffer will be used */
		bufdesc.numbytes = BUFFSIZE;
		bufdesc.u.ppBuffer = NULL;

		/* Initiate Audio IN, read audio data */
		ioctl(fd, AUDIOIOC_DEQUEUEBUFFER, (unsigned long)&bufdesc);

		rx_apb = bufdesc.u.pBuffer;

		if (mode == MODE_LOOPBACK) {
			sem_post(&rx_done_sem);
			sem_wait(&tx_start_sem);
		} else if (mode == MODE_RECORD) {
			memcpy((void *)buff + pos, rx_apb->samp, rx_apb->nbytes);
			pos += rx_apb->nbytes;
		}

		ioctl(fd, AUDIOIOC_FREEBUFFER, (unsigned long)&bufdesc);
	}
	printf("\n - END\n");

	if (mode == MODE_RECORD) {
		printf("Received Data Write Start..\n");

		pcm_fd = open("/mnt/test.pcm", O_RDWR | O_CREAT | O_TRUNC);
		if (pcm_fd < 0) {
			errcode = errno;
			printf("failed to open file, %d\n", errcode);
			return NULL;
		}

		nwritten = write(pcm_fd, buff, BUFFSIZE * RECORD_CNT);

		if (nwritten != (BUFFSIZE * RECORD_CNT))
			printf("File Write Fail\n");

		printf("File Write Done\n");
	}

	close(fd);

	return NULL;
}

pthread_addr_t alc5658char_transmitter(void *arg)
{
	int fd;
	int pcm_fd;
	int i = 0;
	int errcode;
	int pos = 0;
	int nwritten;
	int mode = *(int *)arg;
	FAR struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb;

	if (mode == MODE_RECORD) {
		printf("Read File Start..\n");

		pcm_fd = open("/mnt/test.pcm", O_RDONLY | O_NONBLOCK);
		if (pcm_fd < 0) {
			errcode = errno;
			printf("failed to open file, %d\n", errcode);
			return NULL;
		}

		nwritten = read(pcm_fd, buff, BUFFSIZE * RECORD_CNT);

		printf("File Read Done\n");
	}

	/* Open the alc character device */

	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		errcode = errno;
		printf("failed to open file, %d\n", errcode);
		return NULL;
	}

	if (mode == MODE_LOOPBACK)
		i = LOOPBACK_CNT;
	else if (mode == MODE_RECORD)
		i = RECORD_CNT;

	while (i--)	{
		if (mode == MODE_LOOPBACK)
			sem_wait(&rx_done_sem);

		/* Define APB size and allocate */
		bufdesc.numbytes = BUFFSIZE;
		bufdesc.u.ppBuffer = &apb;
		ioctl(fd, AUDIOIOC_ALLOCBUFFER, (unsigned long)&bufdesc);

		if (mode == MODE_LOOPBACK) {
			memcpy(apb->samp, rx_apb->samp, rx_apb->nbytes);
			sem_post(&tx_start_sem);

			bufdesc.u.pBuffer = apb;
			apb->nbytes = rx_apb->nbytes;
		} else if (mode == MODE_RECORD) {
			memcpy(apb->samp, (void *)buff + pos, rx_apb->nbytes);
			pos += rx_apb->nbytes;

			bufdesc.u.pBuffer = apb;
			apb->nbytes = apb->nmaxbytes;
		}

		/* Enqueue buffer to be sent */
		ioctl(fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc);
	}

	close(fd);

	return NULL;
}

void alc5658char_test_help(FAR const char *program)
{
	printf("================== %s commands ==================\n", program);
	printf("%s loopback : Loopback test using headphone jack\n", program);
	printf("%s record   : Record and Play test using headphone jack\n", program);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int alc5658char_test_main(int argc, char *argv[])
#endif
{
	pthread_attr_t attr;
	pthread_addr_t result;
	pthread_t transmitter;
	pthread_t receiver;
	int ret;
	int fd;
	int errcode;
	int mode;
	struct audio_caps_s caps;

	if (argc > 2) {
		alc5658char_test_help(argv[0]);
		return -EINVAL;
	}

	if (!strcmp(argv[1], "loopback")) {
		mode = MODE_LOOPBACK;
	} else if (!strcmp(argv[1], "record")) {
		mode = MODE_RECORD;
	} else {
		alc5658char_test_help(argv[0]);
		return -EINVAL;
	}

	/* Open the alc character device */
	fd = open(DEVICE_PATH, O_RDWR);
	if (fd < 0) {
		errcode = errno;
		printf("failed to open file, %d\n", errcode);
		return -ENODEV;
	}

	if (mode == MODE_LOOPBACK) {
		sem_init(&tx_start_sem, 0, 0);
		sem_init(&rx_done_sem, 0, 0);
	}

	printf("HELLO!!! %s\n", argv[0]);

	/* Input will be the same since it is the same I2S channel */
	caps.ac_len = sizeof(struct audio_caps_s);
	caps.ac_type = AUDIO_TYPE_OUTPUT;
	caps.ac_channels = 2;

	caps.ac_controls.hw[0] = AUDIO_SAMP_RATE_48K;
	caps.ac_controls.b[2] = 8 * BPSMPL;

	ret = ioctl(fd, AUDIOIOC_CONFIGURE, &caps);

	if (mode == MODE_RECORD)	{
		buff = malloc(BUFFSIZE * RECORD_CNT);
		if (!buff) {
			printf("Alloc failed!!! Please Decrease BUFFSIZE\n");
			return -ENOMEM;
		}
	}

	/* Start the receiver thread */
	sched_lock();
	pthread_attr_init(&attr);

	/* Set the receiver stack size */
	(void)pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_ALC5658CHAR_RXSTACKSIZE);

	/* Start the receiver */

	ret = pthread_create(&receiver, &attr, alc5658char_receiver, (void *)&mode);
	if (ret != OK) {
		sched_unlock();
		printf("%s: ERROR: failed to Start receiver thread: %d\n", __func__, ret);
		goto err;
	}

	pthread_setname_np(receiver, "receiver");
	sched_unlock();

	ioctl(fd, AUDIOIOC_START, 0);

	/* Set Volume */
	caps.ac_type = AUDIO_TYPE_FEATURE;

	caps.ac_format.hw = AUDIO_FU_VOLUME;
	caps.ac_controls.hw[0] = 800;
	ret = ioctl(fd, AUDIOIOC_CONFIGURE, &caps);

	caps.ac_format.hw = AUDIO_FU_MUTE;
	caps.ac_controls.b[0] = false;
	ret = ioctl(fd, AUDIOIOC_CONFIGURE, &caps);

	caps.ac_format.hw = AUDIO_FU_BALANCE;
	caps.ac_controls.hw[0] = -500;
	ret = ioctl(fd, AUDIOIOC_CONFIGURE, &caps);

	/* Values -16 ~ 0 ~ 53 equal to -12dB ~ 0dB ~ 39.75 */
	caps.ac_format.hw = AUDIO_FU_MICGAIN;
	caps.ac_controls.hw[0] = 0;
	ret = ioctl(fd, AUDIOIOC_CONFIGURE, &caps);

	if (mode == MODE_RECORD) {
		ret = pthread_join(receiver, &result);
		if (ret != OK) {
			printf("%s: ERROR: pthread_join failed: %d\n", __func__, ret);
			goto err;
		}
	}

	/* Start the transmitter thread */
	sched_lock();
	pthread_attr_init(&attr);

	/* Set the transmitter stack size */
	(void)pthread_attr_setstacksize(&attr, CONFIG_EXAMPLES_ALC5658CHAR_TXSTACKSIZE);

	ret = pthread_create(&transmitter, &attr, alc5658char_transmitter, (void *)&mode);
	if (ret != OK) {
		sched_unlock();
		printf("%s: ERROR: failed to Start transmitter thread: %d\n", __func__, ret);
		printf("%s: Waiting for the receiver thread\n", __func__);
		(void)pthread_join(receiver, &result);
		goto err;
	}

	pthread_setname_np(transmitter, "transmitter");
	sched_unlock();

	ret = pthread_join(transmitter, &result);
	if (ret != OK) {
		printf("%s: ERROR: pthread_join failed: %d\n", __func__, ret);
		goto err;
	}

	if (mode == MODE_LOOPBACK) {
		ret = pthread_join(receiver, &result);
		if (ret != OK) {
			printf("%s: ERROR: pthread_join failed: %d\n", __func__, ret);
			goto err;
		}
	}

	sem_destroy(&tx_start_sem);
	sem_destroy(&rx_done_sem);

	ioctl(fd, AUDIOIOC_STOP, 0);
	close(fd);
	if (buff != NULL)
		free(buff);

	return EXIT_SUCCESS;

err:
	sem_destroy(&tx_start_sem);
	sem_destroy(&rx_done_sem);

	ioctl(fd, AUDIOIOC_STOP, 0);
	close(fd);
	if (buff != NULL)
		free(buff);
	return EXIT_FAILURE;
}
