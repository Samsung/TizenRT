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
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
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
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <apps/shell/tash.h>

#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>
#include <tinyara/fs/ioctl.h>

/****************************************************************************
 * internal
 ****************************************************************************/
#define AI_AUDIO_DEVNODE		"/dev/audio/pcmC0D0c"
#define AI_PROCESS_MQ_NAME		"/adam_proc_mq"
#define AI_PROCESS_MQ_MAXMSG	16
#define AUTOKD_THREAD_STACK_SIZE	(8192)

#define AI_CAP_MQ_NAME "/adam_cap_mq"
#define AI_CAP_MQ_MAXMSG 32
static mqd_t g_cap_mq = (mqd_t)-1;

static volatile bool g_cap_active = false;
static int g_cap_target = 0;//10;
static int g_cap_count  = 0;

#define KD_SNAP_MAX   (3840 * 17)   // 65280 bytes
static uint8_t  g_kd_snap[KD_SNAP_MAX];
static uint32_t g_kd_snap_size = 0;
static pthread_mutex_t g_kd_lock = PTHREAD_MUTEX_INITIALIZER;

typedef enum {
    AI_SAVE_KD = 0,
    AI_SAVE_PCM,
} ai_save_mode_t;

typedef struct {
	unsigned int seconds;
	ai_save_mode_t mode;
} autokd_args_t;

typedef struct {
	int		fd;
	mqd_t	proc_mq;
	bool	opened;
	bool	proc_registered;

	/* capture buffer pool */
    struct ap_buffer_s **pbuffers;
    int     nbuffers;
    int     bufsize;
} adam_status_t;

static adam_status_t g_ai = {
	.fd					= -1,
	.proc_mq			= (mqd_t)-1,
	.opened				= false,
	.proc_registered 	= false,
	.pbuffers = NULL,
    .nbuffers = 0,
    .bufsize = 0,
};



static ai_save_mode_t g_save_mode = AI_SAVE_KD;

static int ai_wait_event(int timeout_sec, bool auto_kdget);
static int ai_open_cap_mq_and_register(void);
static int ai_prepare_and_enqueue_capture_buffers(void);
static void ai_dump_pcm_head(struct ap_buffer_s *apb, int head_bytes);
static int ai_handle_capture_mq_once(int timeout_sec);
static void ai_release_capture(void);

/****************************************************************************
 * timer
 ****************************************************************************/
static void make_abs_timeout(struct timespec *ts, int sec)
{
	clock_gettime(CLOCK_REALTIME, ts);
	ts->tv_sec += sec;
}

static int time_is_past(const struct timespec *end)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	if (now.tv_sec > end->tv_sec)
		return 1;
	if (now.tv_sec == end->tv_sec && now.tv_nsec >= end->tv_nsec)
		return 1;
	return 0;
}

/****************************************************************************
 * pthread worker
 ****************************************************************************/
static pthread_t g_autokd_tid;
static pthread_mutex_t g_autokd_lock = PTHREAD_MUTEX_INITIALIZER;
static bool g_autokd_running = false;
static bool g_autokd_stopreq = false;

static void *autokd_worker(void *arg)
{
	autokd_args_t *a = (autokd_args_t *)arg;
	unsigned int seconds = a ? a->seconds : 0;
	ai_save_mode_t mode = a ? a->mode : AI_SAVE_KD;
	if (a)
		free(a);

	// 0. mode
	g_save_mode = mode;

	// 1. open
	g_ai.fd = open(AI_AUDIO_DEVNODE, O_RDWR);
	if (g_ai.fd < 0)
	{
		printf("open(%s) failed: %d (%s)\n", AI_AUDIO_DEVNODE, errno, strerror(errno));
		goto done;
	}
	g_ai.opened = true;

	// 2. message queue
	struct mq_attr attr;
	memset(&attr, 0x00, sizeof(attr));
	attr.mq_maxmsg = AI_PROCESS_MQ_MAXMSG;
	attr.mq_msgsize = sizeof(struct audio_msg_s);

	g_ai.proc_mq = mq_open(AI_PROCESS_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (g_ai.proc_mq == (mqd_t)-1)
	{
		printf("mq_open(%s) failed: %d (%s)\n", AI_PROCESS_MQ_NAME, errno, strerror(errno));
		close(g_ai.fd);
		g_ai.fd = -1;
		g_ai.opened = false;
		goto done;
	}

	// 3. register process mq
	if (ioctl(g_ai.fd, AUDIOIOC_REGISTERPROCESS, (unsigned long)g_ai.proc_mq) < 0) 
	{
		printf("ioctl(REGISTERPROCESS) failed: %d (%s)\n", errno, strerror(errno));
		mq_close(g_ai.proc_mq);
		mq_unlink(AI_PROCESS_MQ_NAME);
		g_ai.proc_mq = (mqd_t)-1;
		close(g_ai.fd);
		g_ai.fd = -1;
		g_ai.opened = false;
		goto done;
	}

	g_ai.proc_registered = true;

	// 4. change kd as 0
	if (ioctl(g_ai.fd, AUDIOIOC_CHANGEKD, (unsigned long)0) < 0)
	{
		printf("ioctl(AUDIOIOC_CHANGEKD) failed: %d (%s)\n", errno, strerror(errno));
		goto done;
	}

	// 5. start kd
	if (ioctl(g_ai.fd, AUDIOIOC_STARTPROCESS, (unsigned long)AUDIO_SD_KEYWORD_DETECT) < 0)
	{
		printf("ioctl(STARTPROCESS) failed: %d (%s)\n", errno, strerror(errno));
		goto done;
	}

	// 6. watch
	struct timespec end;
	if (seconds > 0)
	{
		clock_gettime(CLOCK_REALTIME, &end);
		end.tv_sec += seconds;
	}

	printf("ai watch: %d sec\n", seconds);

	while (1)
	{
		pthread_mutex_lock(&g_autokd_lock);
		bool stop_requested = g_autokd_stopreq;
		pthread_mutex_unlock(&g_autokd_lock);

		if (stop_requested)
			break;

		if (seconds > 0 && time_is_past(&end))
			break;

		int ret = ai_wait_event(1, true);
		if (ret < 0)
		{
			break;
			//goto done;
		}

		if (g_cap_active) {
			while (1) {
				int r = ai_handle_capture_mq_once(0);
				if (r == 0)
					continue;  // handled one
				break;         // 1: no msg, -1: error
			}
		}
	}

done:
	pthread_mutex_lock(&g_autokd_lock);
	g_autokd_running = false;
	g_autokd_stopreq = false;
	pthread_mutex_unlock(&g_autokd_lock);

/*
	// stop
	if (g_ai.opened)
	{
		ioctl(g_ai.fd, AUDIOIOC_STOPPROCESS, (unsigned long)AUDIO_SD_KEYWORD_DETECT);
	}

	// unregister
	if (g_ai.opened && g_ai.proc_registered)
	{
		(void)ioctl(g_ai.fd, AUDIOIOC_UNREGISTERPROCESS, 0);
		g_ai.proc_registered = false;
	}

	if (g_ai.proc_mq != (mqd_t)-1)
	{
		mq_close(g_ai.proc_mq);
		mq_unlink(AI_PROCESS_MQ_NAME);
		g_ai.proc_mq = (mqd_t)-1;
	}

	if (g_ai.fd >= 0)
	{
		close(g_ai.fd);
		g_ai.fd = -1;
	}
*/
	g_ai.opened = false;

	return NULL;
}

/****************************************************************************
 * capture
 ****************************************************************************/
static int ai_open_cap_mq_and_register(void)
{
    struct mq_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.mq_maxmsg  = AI_CAP_MQ_MAXMSG;
    attr.mq_msgsize = sizeof(struct audio_msg_s);

    mq_unlink(AI_CAP_MQ_NAME);
    g_cap_mq = mq_open(AI_CAP_MQ_NAME, O_RDWR | O_CREAT | O_NONBLOCK , 0666, &attr);
    if (g_cap_mq == (mqd_t)-1) {
        printf("mq_open(%s) failed: %d (%s)\n", AI_CAP_MQ_NAME, errno, strerror(errno));
        return -1;
    }

    if (ioctl(g_ai.fd, AUDIOIOC_REGISTERMQ, (unsigned long)g_cap_mq) < 0) {
        printf("ioctl(REGISTERMQ) failed: %d (%s)\n", errno, strerror(errno));
        mq_close(g_cap_mq);
        mq_unlink(AI_CAP_MQ_NAME);
        g_cap_mq = (mqd_t)-1;
        return -1;
    }

    return 0;
}

static int ai_prepare_and_enqueue_capture_buffers(void)
{
    struct ap_buffer_info_s info;
    memset(&info, 0, sizeof(info));

	if (g_ai.pbuffers || g_cap_mq != (mqd_t)-1) {
		ai_release_capture();
    }

    int ret = ioctl(g_ai.fd, AUDIOIOC_GETBUFFERINFO, (unsigned long)&info);
    if (ret != OK || info.nbuffers <= 0 || info.buffer_size <= 0) {
        printf("ioctl(AUDIOIOC_GETBUFFERINFO) failed: %d (%s)\n", errno, strerror(errno));
		goto fail;
    }
	else {
		printf("AUDIOIOC_GETBUFFERINFO n=%d, s=%d\n", info.nbuffers, info.buffer_size);
	}

    g_ai.nbuffers = info.nbuffers;
    g_ai.bufsize  = info.buffer_size;

    g_ai.pbuffers = (struct ap_buffer_s **)calloc(g_ai.nbuffers, sizeof(void *));
    if (!g_ai.pbuffers) {
        printf("calloc pbuffers failed\n");
        goto fail;
    }

    /* AUDIOIOC_ALLOCBUFFER */
    for (int i = 0; i < g_ai.nbuffers; i++) {
        struct audio_buf_desc_s desc;
        memset(&desc, 0, sizeof(desc));
        desc.numbytes  = g_ai.bufsize;

        desc.u.ppBuffer = &g_ai.pbuffers[i];

        ret = ioctl(g_ai.fd, AUDIOIOC_ALLOCBUFFER, (unsigned long)&desc);
        if (ret != sizeof(desc) || g_ai.pbuffers[i] == NULL) {
            printf("ALLOCBUFFER failed i=%d ret=%d errno=%d\n", i, ret, errno);
            goto fail;
        }
    }

    /* AUDIOIOC_ENQUEUEBUFFER */
    for (int i = 0; i < g_ai.nbuffers; i++) {
        struct ap_buffer_s *apb = g_ai.pbuffers[i];

        apb->nbytes = apb->nmaxbytes;

        struct audio_buf_desc_s q;
        memset(&q, 0, sizeof(q));
        q.numbytes  = apb->nbytes;
        q.u.pBuffer  = apb;

        ret = ioctl(g_ai.fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&q);
        if (ret < 0) {
            printf("ENQUEUEBUFFER failed i=%d errno=%d (%s)\n", i, errno, strerror(errno));
            goto fail;
        }
    }

    printf("capture buffers queued: nbuffers=%d bufsize=%d\n", g_ai.nbuffers, g_ai.bufsize);
    return 0;

fail:
    if (g_ai.pbuffers) {
        for (int i = 0; i < g_ai.nbuffers; i++) {
            if (g_ai.pbuffers[i]) {
                struct audio_buf_desc_s d;
                memset(&d, 0, sizeof(d));
                d.u.pBuffer = g_ai.pbuffers[i];
                (void)ioctl(g_ai.fd, AUDIOIOC_FREEBUFFER, (unsigned long)&d);
                g_ai.pbuffers[i] = NULL;
            }
        }
        free(g_ai.pbuffers);
        g_ai.pbuffers = NULL;
    }
    g_ai.nbuffers = 0;
    g_ai.bufsize  = 0;

    return -1;	
}

static void ai_release_capture(void)
{
    if (g_ai.fd >= 0 && g_cap_mq != (mqd_t)-1) {
        (void)ioctl(g_ai.fd, AUDIOIOC_UNREGISTERMQ, (unsigned long)g_cap_mq);
    }

    /* free allocated buffers */
    if (g_ai.pbuffers) {
        for (int i = 0; i < g_ai.nbuffers; i++) {
            if (g_ai.pbuffers[i]) {
                struct audio_buf_desc_s d = {0};
                d.u.pBuffer = g_ai.pbuffers[i];
                (void)ioctl(g_ai.fd, AUDIOIOC_FREEBUFFER, (unsigned long)&d);
                g_ai.pbuffers[i] = NULL;
            }
        }
        free(g_ai.pbuffers);
        g_ai.pbuffers = NULL;
    }
    g_ai.nbuffers = 0;
    g_ai.bufsize  = 0;

    /* close/unlink mq */
    if (g_cap_mq != (mqd_t)-1) {
        mq_close(g_cap_mq);
        mq_unlink(AI_CAP_MQ_NAME);
        g_cap_mq = (mqd_t)-1;
    }
}

static void ai_dump_pcm_head(struct ap_buffer_s *apb, int head_bytes)
{
    int n = head_bytes;
    if (n > (int)apb->nbytes) n = (int)apb->nbytes;
    printf("pcm: %u bytes head:", (unsigned)apb->nbytes);
    for (int i = 0; i < n; i++) {
        printf(" %02X", apb->samp[i]);
    }
    printf("\n");
}

static void ai_store_pcm_snapshot(struct ap_buffer_s *apb)
{
    if (!apb || apb->nbytes == 0)
        return;

    pthread_mutex_lock(&g_kd_lock);

    uint32_t remain = 0;
    uint32_t copy_sz = 0;

    if (g_kd_snap_size < KD_SNAP_MAX) {
        remain = KD_SNAP_MAX - g_kd_snap_size;
        copy_sz = apb->nbytes;

        if (copy_sz > remain) {
            copy_sz = remain;
        }

        if (copy_sz > 0) {
            memcpy(&g_kd_snap[g_kd_snap_size], apb->samp, copy_sz);
            g_kd_snap_size += copy_sz;
        }
    }

    pthread_mutex_unlock(&g_kd_lock);
/*
    if (copy_sz < (uint32_t)apb->nbytes) {
        printf("pcm snapshot full: appended %u / %u bytes\n", (unsigned)copy_sz, (unsigned)apb->nbytes);
    }
*/
}

static int ai_handle_capture_mq_once(int timeout_sec)
{
    if (g_cap_mq == (mqd_t)-1)
		return 1;

    struct audio_msg_s msg;
    int prio;
    ssize_t n;

    if (timeout_sec <= 0) {
        n = mq_receive(g_cap_mq, (char*)&msg, sizeof(msg), &prio);
    } else {
        struct timespec ts;
        make_abs_timeout(&ts, timeout_sec);
        n = mq_timedreceive(g_cap_mq, (char*)&msg, sizeof(msg), &prio, &ts);
        if (n < 0 && errno == ETIMEDOUT)
			return 1;
    }
    if (n < 0) {
		if (errno == EAGAIN)
			return 1;   // "no message"
        printf("cap mq recv failed: %d (%s)\n", errno, strerror(errno));
        return -1;
    }

    switch (msg.msgId) {
		case AUDIO_MSG_DEQUEUE: {
		    if (!g_cap_active) {
				break;
			}
			struct ap_buffer_s *apb = (struct ap_buffer_s *)msg.u.pPtr;

			// dump!!!
			ai_dump_pcm_head(apb, 16);

			//save!!
			if (g_save_mode == AI_SAVE_PCM) {
				ai_store_pcm_snapshot(apb);
			}
			g_cap_count++;
			//printf("AUDIO_MSG_DEQUEUE\n");

			if (g_cap_target > 0 && g_cap_count >= g_cap_target) {
				printf("Capture session done: %d chunks\n", g_cap_count);
				g_cap_active = false;
				
				// maybe kill
				ai_release_capture();
				break;
			}
			// again AUDIOIOC_ENQUEUEBUFFER
			apb->nbytes = apb->nmaxbytes;

			struct audio_buf_desc_s q = {0};
			q.numbytes = apb->nbytes;
			q.u.pBuffer = apb;

			if (ioctl(g_ai.fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&q) < 0) {
				printf("re-ENQUEUE failed: %d (%s)\n", errno, strerror(errno));
				g_cap_active = false;
				// maybe kill
				ai_release_capture();
				return -1;
			}
			break;
		}
		case AUDIO_MSG_COMPLETE:
			printf("cap: COMPLETE\n");
			break;

		default:
			break;
		}

    return 0;
}

/****************************************************************************
 * operation
 ****************************************************************************/
static int ai_auto(ai_save_mode_t mode, int seconds)
{
	if (g_ai.opened)
	{
		printf("already opened.\n");
		return 0;
	}

	pthread_mutex_lock(&g_autokd_lock);
	if (g_autokd_running)
	{
		pthread_mutex_unlock(&g_autokd_lock);
		printf("autokd is already running.\n");
		return ERROR;
	}

	g_autokd_running = true;
	g_autokd_stopreq = false;
	pthread_mutex_unlock(&g_autokd_lock);

	pthread_mutex_lock(&g_kd_lock);
	g_save_mode = mode;
	g_kd_snap_size = 0;
	memset(g_kd_snap, 0, sizeof(g_kd_snap));
	pthread_mutex_unlock(&g_kd_lock);


	autokd_args_t *arg = (autokd_args_t *)malloc(sizeof(autokd_args_t));
	if (!arg)
	{
		pthread_mutex_lock(&g_autokd_lock);
		g_autokd_running = false;
		pthread_mutex_unlock(&g_autokd_lock);
		printf("malloc fail.\n");
		return ERROR;
	}
	arg->seconds = (unsigned int)seconds;
	arg->mode = mode;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, AUTOKD_THREAD_STACK_SIZE);

	int ret = pthread_create(&g_autokd_tid, &attr, autokd_worker, arg);
	pthread_attr_destroy(&attr);

	if (ret != 0)
	{
		printf("pthread_create failed %d\n", ret);
		free(arg);
		pthread_mutex_lock(&g_autokd_lock);
		g_autokd_running = false;
		pthread_mutex_unlock(&g_autokd_lock);
		return ERROR;
	}

	return OK;
}

static int ai_stop_auto(void)
{
	pthread_mutex_lock(&g_autokd_lock);
	if (!g_autokd_running)
	{
		pthread_mutex_unlock(&g_autokd_lock);
		printf("ai stop: not running\n");
		return OK;
	}
	g_autokd_stopreq = true;
	pthread_mutex_unlock(&g_autokd_lock);

	/* join to ensure resources are released */
	pthread_join(g_autokd_tid, NULL);
	printf("ai stop: done\n");
	return OK;
}

/*
static int ai_open_and_registerprocess(void)
{
	if (g_ai.opened)
	{
		printf("already opened.\n");
		return 0;
	}

	g_ai.fd = open(AI_AUDIO_DEVNODE, O_RDWR);
	if (g_ai.fd < 0)
	{
		printf("open(%s) failed: %d (%s)\n", AI_AUDIO_DEVNODE, errno, strerror(errno));
		return -1;
	}
	g_ai.opened = true;

	struct mq_attr attr;
	memset(&attr, 0x00, sizeof(attr));
	attr.mq_maxmsg = AI_PROCESS_MQ_MAXMSG;
	attr.mq_msgsize = sizeof(struct audio_msg_s);

	mq_unlink(AI_PROCESS_MQ_NAME);

	// open
	g_ai.proc_mq = mq_open(AI_PROCESS_MQ_NAME, O_RDWR | O_CREAT, 0666, &attr);
	if (g_ai.proc_mq == (mqd_t)-1)
	{
		printf("mq_open(%s) failed: %d (%s)\n", AI_PROCESS_MQ_NAME, errno, strerror(errno));
		close(g_ai.fd);
		g_ai.fd = -1;
		g_ai.opened = false;
		return -1;
	}

	// register process mq
	if (ioctl(g_ai.fd, AUDIOIOC_REGISTERPROCESS, (unsigned long)g_ai.proc_mq) < 0) 
	{
		printf("ioctl(REGISTERPROCESS) failed: %d (%s)\n", errno, strerror(errno));
		mq_close(g_ai.proc_mq);
		mq_unlink(AI_PROCESS_MQ_NAME);
		g_ai.proc_mq = (mqd_t)-1;
		close(g_ai.fd);
		g_ai.fd = -1;
		g_ai.opened = false;
		return -1;
	}

	g_ai.proc_registered = true;
	printf("ai open ok: dev=%s, proc_mq=%s\n", AI_AUDIO_DEVNODE, AI_PROCESS_MQ_NAME);
	return 0;
}
*/
static void ai_close_all(void)
{
	// unregister
	if (g_ai.opened && g_ai.proc_registered)
	{
		(void)ioctl(g_ai.fd, AUDIOIOC_UNREGISTERPROCESS, 0);
		g_ai.proc_registered = false;
	}

	if (g_ai.proc_mq != (mqd_t)-1)
	{
		mq_close(g_ai.proc_mq);
		mq_unlink(AI_PROCESS_MQ_NAME);
		g_ai.proc_mq = (mqd_t)-1;
	}

	if (g_ai.fd >= 0)
	{
		close(g_ai.fd);
		g_ai.fd = -1;
	}

	g_ai.opened = false;
	printf("ai closed\n");
}

static int ai_save_dump(void)
{
    pthread_mutex_lock(&g_kd_lock);

    if (g_kd_snap_size == 0) {
        printf("snapshot empty\n");
        pthread_mutex_unlock(&g_kd_lock);
        return 0;
    }

	printf("snapshot(%s): %u bytes\n", (g_save_mode == AI_SAVE_PCM) ? "pcm" : "kd", (unsigned)g_kd_snap_size);

	for (uint32_t i = 0; i + 1 < g_kd_snap_size; i += 2) {
		uint16_t v;
		if (g_save_mode == AI_SAVE_PCM) {
			v = ((uint16_t)g_kd_snap[i + 1] << 8) | g_kd_snap[i];
		} else {
			v = ((uint16_t)g_kd_snap[i] << 8) | g_kd_snap[i + 1];
		}
		printf("0x%04X,", v);
	}
	printf("\n");

    pthread_mutex_unlock(&g_kd_lock);
    return 0;
}

/*
static int ai_start_process(uint8_t mode)
{
	if (!g_ai.opened)
	{
		printf("ai not opened. run: ai open\n");
		return -1;
	}

	// start
	if (ioctl(g_ai.fd, AUDIOIOC_STARTPROCESS, (unsigned long)mode) < 0)
	{
		printf("ioctl(STARTPROCESS) failed: %d (%s)\n", errno, strerror(errno));
		return -1;
	}

	printf("ai startprocess ok (mode=%u)\n", (unsigned)mode);
	return 0;
}
*/

/*
static int ai_stop_process(uint8_t mode)
{
	if (!g_ai.opened)
	{
		printf("ai not opened. run: ai open\n");
		return -1;
	}

	// stop
	if (ioctl(g_ai.fd, AUDIOIOC_STOPPROCESS, (unsigned long)mode) < 0)
	{
		printf("ioctl(STOPPROCESS) failed: %d (%s)\n", errno, strerror(errno));
		return -1;
	}

	printf("ai stopprocess ok (mode=%u)\n", (unsigned)mode);
	return 0;
}
*/
static int ai_kd_getdata_dump(uint32_t dump_head_bytes, bool save_to_snap)
{
	if (!g_ai.opened)
	{
		printf("ai not opened\n");
		return -1;
	}

	// get kd
	uint32_t sz = 0;
	if (ioctl(g_ai.fd, AUDIOIOC_GETKDBUFSIZE, (unsigned long)(uintptr_t)&sz) < 0)
	{
		printf("ioctl(GETKDBUFSIZE) failed: %d (%s)\n", errno, strerror(errno));
		return -1;
	}
	printf("GETKDBUFSIZE sz=%d\n", sz);

	if (sz == 0)
	{
		printf("kd buf size is 0 (no keyword data?)\n");
		pthread_mutex_lock(&g_kd_lock);
		if (save_to_snap) {
			g_kd_snap_size = 0;
		}
        pthread_mutex_unlock(&g_kd_lock);
		return 0;
	}

	if (sz > KD_SNAP_MAX) {
        printf("kd buf size too large: %u\n", (unsigned)sz);
        sz = KD_SNAP_MAX;
    }

	// get kd
	pthread_mutex_lock(&g_kd_lock);
	if (ioctl(g_ai.fd, AUDIOIOC_GETKDDATA, (unsigned long)(uintptr_t)g_kd_snap) < 0) {
		pthread_mutex_unlock(&g_kd_lock);
		return -1;
	}
	g_kd_snap_size = sz;
	pthread_mutex_unlock(&g_kd_lock);

	printf("kd data: %u bytes, head:", (unsigned)sz);
	uint32_t n = (dump_head_bytes == 0) ? 16 : dump_head_bytes;
	if (n > sz) n = sz;
	for (uint32_t i = 0; i < n; i++) {
		printf(" %02X", g_kd_snap[i]);
	}
	printf("\n");

	if (save_to_snap == false) {
		pthread_mutex_lock(&g_kd_lock);
		g_kd_snap_size = 0;
		pthread_mutex_unlock(&g_kd_lock);
	}

	return 0;
}

static int ai_wait_event(int timeout_sec, bool auto_kdget)
{
	if (g_ai.proc_mq == (mqd_t)-1)
	{
		printf("process mq not ready. run: ai open\n");
		return -1;
	}

	int prio = 0;
	struct audio_msg_s msg;
	ssize_t n;

	if (timeout_sec <= 0)
	{
		n = mq_receive(g_ai.proc_mq, (char *)&msg, sizeof(msg), &prio);
	}
	else
	{
		struct timespec ts;
		make_abs_timeout(&ts, timeout_sec);

		n = mq_timedreceive(g_ai.proc_mq, (char *)&msg, sizeof(msg), &prio, &ts);
		if (n < 0)
		{
			if (errno == ETIMEDOUT)
			{
				//printf(".\n");
				return 1; /* timeout */
			}
			printf("mq_receive failed: %d (%s)\n", errno, strerror(errno));
			return -1;
		}
	}

	if (msg.msgId == 11) {
		printf("ai event: msgId=%d prio=%u\n", (int)msg.msgId, prio);

		if (ioctl(g_ai.fd, AUDIOIOC_START, 0) < 0) {
			printf("ioctl(AUDIOIOC_START) failed: %d (%s)\n", errno, strerror(errno));
		}
		else
			printf("AUDIOIOC_START\n");

		/* KD snapshot */
		if (auto_kdget) {
			bool save_kd = (g_save_mode == AI_SAVE_KD);
			(void)ai_kd_getdata_dump(16, save_kd);
		}

		if (!g_cap_active) {
			g_cap_active = true;
			g_cap_count = 0;

			/* start capture (mq open + alloc + enqueue) */
			if (ai_prepare_and_enqueue_capture_buffers() < 0 || ai_open_cap_mq_and_register() < 0) {
				printf("capture start failed\n");
				g_cap_active = false;
    			ai_release_capture();
			}
		}
		return 0;
	}
	return 0;
}
/*
static int ai_watch(int seconds)
{
	if (!g_ai.opened || g_ai.proc_mq == (mqd_t)-1)
	{
		printf("ai not ready. run: ai open\n");
		return -1;
	}

	if (seconds <= 0)
		seconds = 10;

	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	end.tv_sec += seconds;

	printf("ai watch: %d sec (auto kdget)\n", seconds);

	while (!time_is_past(&end))
	{
		// 1-second timed wait to allow exit on overall timeout
		int ret = ai_wait_event(1, true);
		if (ret < 0)
		{
			return ret; // error
		}
		// ret==1 means per-wait timeout; just continue
	}

	printf("ai watch done\n");
	return 0;
}
*/

/*
static int parse_process_mode(const char *s, uint8_t *out_mode)
{
	if (!s || !out_mode)
		return -1;

	if (!strcmp(s, "kd"))
	{
		*out_mode = (uint8_t)AUDIO_SD_KEYWORD_DETECT;
		return 0;
	}
	if (!strcmp(s, "local"))
	{
		*out_mode = (uint8_t)AUDIO_SD_LOCAL;
		return 0;
	}
	if (!strcmp(s, "aec"))
	{
		*out_mode = (uint8_t)AUDIO_SD_AEC;
		return 0;
	}

	return -1;
}
*/

/****************************************************************************
 * TASH command
 ****************************************************************************/
static int cmd_ai(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage:\n");
		printf("  ai auto kd|pcm <sec>\n");
		printf("  ai save\n");
		//printf("  ai open\n");
		printf("  ai close\n");
		printf("  ai stop\n");
		//printf("  ai start kd|local|aec\n");
		//printf("  ai stop  kd|local|aec\n");
		//printf("  ai wait <sec>          (wait 1 event)\n");
		//printf("  ai kdget               (pull keyword buffer)\n");
		//printf("  ai watch <sec>         (loop wait + auto kdget)\n");
		return 0;
	}

	const char *sub = argv[1];

	if (!strcmp(sub, "auto"))
	{
	    if (argc < 3) {
			printf("usage: ai auto kd [sec]\n");
			printf("       ai auto pcm [sec]\n");
			return -1;
		}
		ai_save_mode_t mode;
		int sec = 0;

		if (!strcmp(argv[2], "kd")) {
			mode = AI_SAVE_KD;
		} else if (!strcmp(argv[2], "pcm")) {
			mode = AI_SAVE_PCM;
		} else {
			printf("unknown auto mode: %s\n", argv[2]);
			return -1;
		}

	    if (argc >= 4) {
			sec = atoi(argv[3]);
		}
		return ai_auto(mode, sec);
	}

	if (!strcmp(sub, "save"))
	{
		return ai_save_dump();
	}

	if (!strcmp(sub, "stop"))
	{
		return ai_stop_auto();
	}
/*
	if (!strcmp(sub, "open"))
	{
		return ai_open_and_registerprocess();
	}
*/
	if (!strcmp(sub, "close"))
	{
		ai_close_all();
		return 0;
	}
/*
	if (!strcmp(sub, "start"))
	{
		if (argc < 3)
		{
			printf("ai start kd|local|aec\n");
			return -1;
		}
		uint8_t mode;
		if (parse_process_mode(argv[2], &mode) < 0)
		{
			printf("unknown mode: %s\n", argv[2]);
			return -1;
		}
		return ai_start_process(mode);
	}

	if (!strcmp(sub, "stop"))
	{
		if (argc < 3)
		{
			printf("ai stop kd|local|aec\n");
			return -1;
		}
		uint8_t mode;
		if (parse_process_mode(argv[2], &mode) < 0)
		{
			printf("unknown mode: %s\n", argv[2]);
			return -1;
		}
		return ai_stop_process(mode);
	}

	if (!strcmp(sub, "wait"))
	{
		int sec = (argc >= 3) ? atoi(argv[2]) : 10;
		return ai_wait_event(sec, false);
	}

	if (!strcmp(sub, "kdget"))
	{
		return ai_kd_getdata_dump(16, true);
	}

	if (!strcmp(sub, "watch"))
	{
		int sec = (argc >= 3) ? atoi(argv[2]) : 10;
		return ai_watch(sec);
	}
*/
	printf("unknown subcmd: %s\n", sub);
	return -1;
}

static tash_cmdlist_t g_ai_cmds[] = {
	{ "ai", cmd_ai, TASH_EXECMD_ASYNC },
	{ NULL, NULL, 0 }
};
/****************************************************************************
 * hello_main
 ****************************************************************************/

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int adam_main(int argc, char *argv[])
#endif
{
	printf("ADAM!!\n");
	tash_cmdlist_install(g_ai_cmds);

	return 0;
}
