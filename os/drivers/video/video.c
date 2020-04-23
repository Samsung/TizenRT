/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
 * drivers/video/video.c
 *
 *   Copyright 2018 Sony Semiconductor Solutions Corporation
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
 * 3. Neither the name of Sony Semiconductor Solutions Corporation nor
 *    the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
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

#include <sys/ioctl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <debug.h>

#include <tinyara/irq.h>
#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/kmalloc.h>

#include <arch/board/board.h>

#include <video/video.h>
#include <video/video_halif.h>

#include "video_framebuff.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define MAX_VIDEO_FILE_PATH     CONFIG_PATH_MAX

#define VIDEO_TRUE              (1)
#define VIDEO_FALSE             (0)

#define VIDEO_REMAINING_CAPNUM_INFINITY (-1)

/****************************************************************************
 * Private Types
 ****************************************************************************/

enum video_state_e {
	VIDEO_STATE_STREAMOFF = 0,	/* DMA trigger event is not received */
	VIDEO_STATE_STREAMON = 1,	/* DMA trigger event is received,
								 * but DMA is not operated.
								 */
	VIDEO_STATE_DMA = 2,		/* On DMA */
};

enum video_state_transition_cause {
	CAUSE_VIDEO_STOP = 0,		/* Stop  DMA event for video stream */
	CAUSE_VIDEO_START = 1,		/* Start DMA event for video stream */
	CAUSE_VIDEO_DQBUF = 2,		/* DQBUF timing    for video stream */
	CAUSE_STILL_STOP = 3,		/* Stop  DMA event for still stream */
	CAUSE_STILL_START = 4,		/* Start DMA event for still stream */
};

enum video_waitend_cause_e {
	VIDEO_WAITEND_CAUSE_DMADONE = 0,
	VIDEO_WAITEND_CAUSE_DQCANCEL = 1,
	VIDEO_WAITEND_CAUSE_STILLSTOP = 2,
};

struct video_wait_dma_s {
	FAR sem_t dqbuf_wait_flg;
	FAR vbuf_container_t *done_container;	/* Save container which dma done */
	enum video_waitend_cause_e waitend_cause;
};

typedef struct video_wait_dma_s video_wait_dma_t;

struct video_type_inf_s {
	sem_t lock_state;
	enum video_state_e state;
	int32_t remaining_capnum;
	video_wait_dma_t wait_dma;
	video_framebuff_t bufinf;
};

typedef struct video_type_inf_s video_type_inf_t;

struct video_upperhalf_s {
	FAR char *devpath;              /* parameter of video_initialize() */
	sem_t lock_open_num;
	uint8_t open_num;
	FAR struct pollfd *poll_wait;   /* poll(setup) information */
	video_type_inf_t video_inf;
	video_type_inf_t still_inf;
	FAR struct video_lowerhalf_s *dev;
};

typedef struct video_upperhalf_s video_upperhalf_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Character driver methods. */

static ssize_t video_open(FAR struct file *filep);
static ssize_t video_close(FAR struct file *filep);
static ssize_t video_ioctl(FAR struct file *filep, int cmd, unsigned long arg);
static ssize_t video_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup);

/* Common function */

static int video_lock(FAR sem_t *sem);
static int video_unlock(FAR sem_t *sem);
static FAR video_type_inf_t *get_video_type_inf(FAR video_upperhalf_t *priv, uint8_t type);
static enum video_state_e estimate_next_video_state(FAR video_upperhalf_t *priv, enum video_state_transition_cause cause);
static int change_video_state(FAR video_upperhalf_t *priv, enum video_state_e next_state);
static bool is_taking_still_picture(FAR video_upperhalf_t *priv);
static bool is_bufsize_sufficient(FAR video_upperhalf_t *priv, uint32_t bufsize);
static void cleanup_resources(FAR video_upperhalf_t *priv);
static bool is_sem_waited(FAR sem_t *sem);

/* internal function for each cmds of ioctl */

static int video_reqbufs(FAR video_upperhalf_t *priv, FAR struct v4l2_requestbuffers *reqbufs);
static int video_qbuf(FAR video_upperhalf_t *priv, FAR struct v4l2_buffer *buf);
static int video_dqbuf(FAR video_upperhalf_t *priv, FAR struct v4l2_buffer *buf);
static int video_cancel_dqbuf(FAR video_upperhalf_t *priv, enum v4l2_buf_type type);
static int video_enum_fmt(FAR video_upperhalf_t *priv, FAR struct v4l2_fmtdesc *fmt);
static int video_enum_framesizes(FAR video_upperhalf_t *priv, FAR struct v4l2_frmsizeenum *frmsize);
static int video_set_fmt(FAR video_upperhalf_t *priv, FAR struct v4l2_format *fmt);
static int video_enum_frameintervals(FAR video_upperhalf_t *priv, FAR struct v4l2_frmivalenum *frmival);
static int video_set_parm(FAR video_upperhalf_t *priv, FAR struct v4l2_streamparm *parm);
static int video_streamon(FAR video_upperhalf_t *priv, FAR enum v4l2_buf_type *type);
static int video_streamoff(FAR video_upperhalf_t *priv, FAR enum v4l2_buf_type *type);
static int video_do_halfpush(FAR video_upperhalf_t *priv, bool enable);
static int video_takepict_start(FAR video_upperhalf_t *priv, int32_t capture_num);
static int video_takepict_stop(FAR video_upperhalf_t *priv, bool halfpush);
static int video_queryctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_queryctrl *ctrl);
static int video_query_ext_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_query_ext_ctrl *ctrl);
static int video_querymenu(FAR video_upperhalf_t *priv, FAR struct v4l2_querymenu *menu);
static int video_get_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_control *ctrl);
static int video_set_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_control *ctrl);
static int video_get_ext_ctrls(FAR video_upperhalf_t *priv, FAR struct v4l2_ext_controls *ctrls);
static int video_set_ext_ctrls(FAR video_upperhalf_t *priv, FAR struct v4l2_ext_controls *ctrls);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_video_fops = {
	video_open,              /* open */
	video_close,             /* close */
	0,                       /* read */
	0,                       /* write */
	0,                       /* seek */
	video_ioctl,             /* ioctl */
#ifndef CONFIG_DISABLE_POLL
	video_poll,              /* poll */
#endif
	0                        /* unlink */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int video_lock(FAR sem_t *sem)
{
	int ret;
	int l_errno;

	if (sem == NULL) {
		return -EINVAL;
	}

	while (1) {
		ret = sem_wait(sem);
		if (ret == ERROR) {
			l_errno = errno;
			if (l_errno == EINTR) {
				continue;
			}

			videodbg("sem_wait() failed:%d\n", l_errno);
		}

		break;
	}

	return ret;
}

static int video_unlock(FAR sem_t *sem)
{
	if (sem == NULL) {
		return -EINVAL;
	}

	sem_post(sem);

	return OK;
}

static FAR video_type_inf_t *get_video_type_inf(FAR video_upperhalf_t *priv, uint8_t type)
{
	FAR video_type_inf_t *type_inf;

	switch (type) {
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
		type_inf = &priv->video_inf;
		break;

	case V4L2_BUF_TYPE_STILL_CAPTURE:
		type_inf = &priv->still_inf;
		break;

	default:					/* Error case */
		type_inf = NULL;
		break;
	}

	return type_inf;
}

static enum video_state_e estimate_next_video_state(FAR video_upperhalf_t *priv, enum video_state_transition_cause cause)
{
	enum video_state_e current_state = priv->video_inf.state;

	switch (cause) {
	case CAUSE_VIDEO_STOP:
		return VIDEO_STATE_STREAMOFF;

	case CAUSE_VIDEO_START:
		if (is_taking_still_picture(priv)) {
			return VIDEO_STATE_STREAMON;
		} else {
			return VIDEO_STATE_DMA;
		}
	case CAUSE_STILL_STOP:
		if (current_state == VIDEO_STATE_STREAMON) {
			return VIDEO_STATE_DMA;
		}
		break;
	case CAUSE_STILL_START:
		if (current_state == VIDEO_STATE_DMA) {
			return VIDEO_STATE_STREAMON;
		}
		break;
	case CAUSE_VIDEO_DQBUF:
		if ((current_state == VIDEO_STATE_STREAMON) && !is_taking_still_picture(priv)) {
			return VIDEO_STATE_DMA;
		}
	}

	return current_state;
}

static int change_video_state(FAR video_upperhalf_t *priv, enum video_state_e next_state)
{
	int ret;
	enum video_state_e current_state = priv->video_inf.state;
	enum video_state_e updated_next_state = next_state;
	FAR vbuf_container_t *dma_container;
	FAR struct video_devops_s *video_devops = priv->dev->ops;

	if ((current_state != VIDEO_STATE_DMA) && (next_state == VIDEO_STATE_DMA)) {
		dma_container = video_framebuff_get_dma_container(&priv->video_inf.bufinf);
		if (dma_container) {
			ret = video_devops->set_buf(priv->dev, V4L2_BUF_TYPE_VIDEO_CAPTURE, dma_container->buf.m.userptr, dma_container->buf.length);
			if (ret != OK) {
				videodbg("Unable to set buffer for state change.\n");
				return ret;
			}
		} else {
			updated_next_state = VIDEO_STATE_STREAMON;
		}
	} else {
		if ((current_state == VIDEO_STATE_DMA) && (next_state != VIDEO_STATE_DMA)) {
			video_devops->cancel_dma(priv->dev);
		}
	}

	priv->video_inf.state = updated_next_state;
	return OK;
}

static bool is_taking_still_picture(FAR video_upperhalf_t *priv)
{
	return ((priv->still_inf.state == VIDEO_STATE_STREAMON) || (priv->still_inf.state == VIDEO_STATE_DMA));
}

static bool is_bufsize_sufficient(FAR video_upperhalf_t *priv, uint32_t bufsize)
{
	/* Depend on format, frame size, and JPEG compression quality */

	return true;
}

static void initialize_streamresources(FAR video_type_inf_t *type_inf)
{
	memset(type_inf, 0, sizeof(video_type_inf_t));
	type_inf->remaining_capnum = VIDEO_REMAINING_CAPNUM_INFINITY;
	sem_init(&type_inf->lock_state, 0, 1);
	sem_init(&type_inf->wait_dma.dqbuf_wait_flg, 0, 0);
	video_framebuff_init(&type_inf->bufinf);
}

static void initialize_resources(FAR video_upperhalf_t *priv)
{
	initialize_streamresources(&priv->video_inf);
	initialize_streamresources(&priv->still_inf);
}

static void cleanup_streamresources(FAR video_type_inf_t *type_inf)
{
	video_framebuff_uninit(&type_inf->bufinf);
	sem_destroy(&type_inf->wait_dma.dqbuf_wait_flg);
	sem_destroy(&type_inf->lock_state);
	memset(type_inf, 0, sizeof(video_type_inf_t));
	type_inf->remaining_capnum = VIDEO_REMAINING_CAPNUM_INFINITY;
}

static void cleanup_resources(FAR video_upperhalf_t *priv)
{
	/* clean up resource */
	FAR struct video_devops_s *video_devops = priv->dev->ops;

	if ((priv->video_inf.state == VIDEO_STATE_DMA) || (priv->still_inf.state == VIDEO_STATE_DMA)) {
		/* If in DMA, stop */

		video_devops->cancel_dma(priv->dev);
	}

	cleanup_streamresources(&priv->video_inf);
	cleanup_streamresources(&priv->still_inf);
}

static bool is_sem_waited(FAR sem_t *sem)
{
	int ret = false;
	int semcount;

	ret = sem_getvalue(sem, &semcount);
	if ((ret == OK) && (semcount < 0)) {
		ret = true;
	}

	return ret;
}

static ssize_t video_open(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR video_upperhalf_t *priv = (FAR video_upperhalf_t *) inode->i_private;
	FAR struct video_devops_s *video_devops = priv->dev->ops;
	ssize_t ret = OK;

	video_lock(&priv->lock_open_num);
	if (priv->open_num == 0) {
		/* Only in first execution, open device */

		ret = video_devops->open(priv->dev);
		if (ret == OK) {
			initialize_resources(priv);
		}
	}

	/* In second or later execution, ret is initial value(=OK) */

	if (ret == OK) {
		priv->open_num++;
	}
	video_unlock(&priv->lock_open_num);

	return ret;
}

static ssize_t video_close(FAR struct file *filep)
{
	FAR struct inode *inode = filep->f_inode;
	FAR video_upperhalf_t *priv = (FAR video_upperhalf_t *) inode->i_private;
	FAR struct video_devops_s *video_devops = priv->dev->ops;
	ssize_t ret = ERROR;

	video_lock(&priv->lock_open_num);
	if (priv->open_num == 0) {
		video_unlock(&priv->lock_open_num);
		return OK;
	}

	priv->open_num--;

	if (priv->open_num == 0) {
		cleanup_resources(priv);
		video_devops->close(priv->dev);
	}
	video_unlock(&priv->lock_open_num);

	return ret;
}

static int video_reqbufs(FAR video_upperhalf_t *priv, FAR struct v4l2_requestbuffers *reqbufs)
{
	int ret = OK;
	FAR video_type_inf_t *type_inf;
	irqstate_t flags;

	if ((priv == NULL) || (reqbufs == NULL)) {
		return -EINVAL;
	}

	type_inf = get_video_type_inf(priv, reqbufs->type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	flags = enter_critical_section();

	if (type_inf->state == VIDEO_STATE_DMA) {
		/* In DMA, REQBUFS is not permitted */

		ret = -EPERM;
	} else {
		video_framebuff_change_mode(&type_inf->bufinf, reqbufs->mode);

		ret = video_framebuff_realloc_container(&type_inf->bufinf, reqbufs->count);
	}

	leave_critical_section(flags);

	return ret;
}

static int video_qbuf(FAR video_upperhalf_t *priv, FAR struct v4l2_buffer *buf)
{
	int ret = OK;
	FAR video_type_inf_t *type_inf;
	FAR vbuf_container_t *container;
	enum video_state_e next_video_state;
	irqstate_t flags;
	FAR struct video_devops_s *video_devops;

	if ((priv == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	type_inf = get_video_type_inf(priv, buf->type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	if (!is_bufsize_sufficient(priv, buf->length)) {
		return -EINVAL;
	}

	container = video_framebuff_get_container(&type_inf->bufinf);
	if (container == NULL) {
		return -ENOMEM;
	}

	memcpy(&container->buf, buf, sizeof(struct v4l2_buffer));
	video_framebuff_queue_container(&type_inf->bufinf, container);

	video_lock(&type_inf->lock_state);
	flags = enter_critical_section();
	if (type_inf->state != VIDEO_STATE_STREAMON) {
		leave_critical_section(flags);
		goto streamoff;
	}
	leave_critical_section(flags);

	if (buf->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		video_lock(&priv->still_inf.lock_state);
		next_video_state = estimate_next_video_state(priv, CAUSE_VIDEO_START);
		ret = change_video_state(priv, next_video_state);
		video_unlock(&priv->still_inf.lock_state);
		if (ret < 0) {
			goto streamoff;
		}
	} else {
		container = video_framebuff_get_dma_container(&type_inf->bufinf);
		if (container) {
			ret = video_devops->set_buf(priv->dev, buf->type, container->buf.m.userptr, container->buf.length);
			if (ret < 0) {
				goto streamoff;
			}
			type_inf->state = VIDEO_STATE_DMA;
		}
	}

streamoff:
	video_unlock(&type_inf->lock_state);

	return ret;
}

static int video_dqbuf(FAR video_upperhalf_t *priv, FAR struct v4l2_buffer *buf)
{
	int ret = OK;
	irqstate_t flags;
	FAR video_type_inf_t *type_inf;
	FAR vbuf_container_t *container;
	sem_t *dqbuf_wait_flg;
	enum video_state_e next_video_state;

	if ((priv == NULL) || (buf == NULL)) {
		return -EINVAL;
	}

	type_inf = get_video_type_inf(priv, buf->type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	container = video_framebuff_dq_valid_container(&type_inf->bufinf);
	if (container == NULL) {
		/* Not yet done DMA. Wait done */

		dqbuf_wait_flg = &type_inf->wait_dma.dqbuf_wait_flg;

		/* Loop until semaphore is unlocked by DMA done or DQCANCEL */

		do {
			if (buf->type == V4L2_BUF_TYPE_VIDEO_CAPTURE) {
				/* If start DMA condition is satisfied, start DMA */

				flags = enter_critical_section();
				next_video_state = estimate_next_video_state(priv, CAUSE_VIDEO_DQBUF);
				ret = change_video_state(priv, next_video_state);
				leave_critical_section(flags);
				if (ret < 0) {
					return ret;
				}
			}

			sem_wait(dqbuf_wait_flg);
		} while (type_inf->wait_dma.waitend_cause == VIDEO_WAITEND_CAUSE_STILLSTOP);

		container = type_inf->wait_dma.done_container;

		if (container == NULL) {
			/* Waking up without DMA data means abort.
			 * Therefore, Check cause.
			 */

			if (type_inf->wait_dma.waitend_cause == VIDEO_WAITEND_CAUSE_DQCANCEL) {
				return -ECANCELED;
			}
		}

		type_inf->wait_dma.done_container = NULL;
	}

	/* On DMA failure */
	if (container->buf.flags == V4L2_BUF_FLAG_ERROR) {
		video_framebuff_free_container(&type_inf->bufinf, container);
		return -EIO;
	}

	memcpy(buf, &container->buf, sizeof(struct v4l2_buffer));

	video_framebuff_free_container(&type_inf->bufinf, container);

	return OK;
}

static int video_cancel_dqbuf(FAR video_upperhalf_t *priv, enum v4l2_buf_type type)
{
	FAR video_type_inf_t *type_inf;

	type_inf = get_video_type_inf(priv, type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	if (!is_sem_waited(&type_inf->wait_dma.dqbuf_wait_flg)) {
		/* In not waiting DQBUF case, return OK */

		return OK;
	}

	type_inf->wait_dma.waitend_cause = VIDEO_WAITEND_CAUSE_DQCANCEL;

	/* If DMA is done before sem_post, cause is overwritten */

	sem_post(&type_inf->wait_dma.dqbuf_wait_flg);

	return OK;
}

static int video_enum_fmt(FAR video_upperhalf_t *priv, FAR struct v4l2_fmtdesc *fmt)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->get_range_of_fmt == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->get_range_of_fmt(priv->dev, fmt);

	return ret;
}

static int video_enum_framesizes(FAR video_upperhalf_t *priv, FAR struct v4l2_frmsizeenum *frmsize)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->get_range_of_framesize == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->get_range_of_framesize(priv->dev, frmsize);

	return ret;
}

static int video_try_fmt(FAR video_upperhalf_t *priv, FAR struct v4l2_format *fmt)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->try_format == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->try_format(priv->dev, fmt);

	return ret;
}

static int video_set_fmt(FAR video_upperhalf_t *priv, FAR struct v4l2_format *fmt)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	if (priv->video_inf.state == VIDEO_STATE_STREAMON || priv->still_inf.state == VIDEO_STATE_STREAMON) {
		videodbg("Device currently streaming, unable to change parameters.\n");
		return -EBUSY;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->set_format == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->set_format(priv->dev, fmt);

	return ret;
}

static int video_enum_frameintervals(FAR video_upperhalf_t *priv, FAR struct v4l2_frmivalenum *frmival)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->get_range_of_frameinterval == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->get_range_of_frameinterval(priv->dev, frmival);

	return ret;
}

static int video_set_parm(FAR video_upperhalf_t *priv, FAR struct v4l2_streamparm *parm)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	if (priv->video_inf.state == VIDEO_STATE_STREAMON || priv->still_inf.state == VIDEO_STATE_STREAMON) {
		videodbg("Device currently streaming, unable to change parameters.\n");
		return -EBUSY;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->set_frameinterval == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->set_frameinterval(priv->dev, parm);

	return ret;
}

static int video_streamon(FAR video_upperhalf_t *priv, FAR enum v4l2_buf_type *type)
{
	FAR video_type_inf_t *type_inf;
	enum video_state_e next_video_state;
	int ret = OK;

	if ((priv == NULL) || (type == NULL)) {
		return -EINVAL;
	}

	type_inf = get_video_type_inf(priv, *type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	if (*type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		/* No procedure for VIDIOC_STREAMON(STILL_CAPTURE) */

		return OK;
	}

	video_lock(&type_inf->lock_state);

	if (type_inf->state != VIDEO_STATE_STREAMOFF) {
		ret = -EPERM;
	} else {
		next_video_state = estimate_next_video_state(priv, CAUSE_VIDEO_START);
		ret = change_video_state(priv, next_video_state);
	}

	video_unlock(&type_inf->lock_state);

	return ret;
}

static int video_streamoff(FAR video_upperhalf_t *priv, FAR enum v4l2_buf_type *type)
{
	FAR video_type_inf_t *type_inf;
	enum video_state_e next_video_state;
	irqstate_t flags;
	int ret = OK;

	if ((priv == NULL) || (type == NULL)) {
		return -EINVAL;
	}

	type_inf = get_video_type_inf(priv, *type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	if (*type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
		/* No procedure for VIDIOC_STREAMOFF(STILL_CAPTURE) */

		return OK;
	}

	flags = enter_critical_section();

	if (type_inf->state == VIDEO_STATE_STREAMOFF) {
		ret = -EPERM;
	} else {
		next_video_state = estimate_next_video_state(priv, CAUSE_VIDEO_STOP);
		ret = change_video_state(priv, next_video_state);
	}

	leave_critical_section(flags);

	return ret;
}

static int video_do_halfpush(FAR video_upperhalf_t *priv, bool enable)
{
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->do_halfpush == NULL)) {
		return -ENOSYS;
	}

	return video_devops->do_halfpush(priv->dev, enable);
}

static int video_takepict_start(FAR video_upperhalf_t *priv, int32_t capture_num)
{
	irqstate_t flags;
	enum video_state_e next_video_state;
	FAR vbuf_container_t *dma_container;
	FAR struct video_devops_s *video_devops;
	int ret = OK;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	video_lock(&priv->still_inf.lock_state);

	if (priv->still_inf.state != VIDEO_STATE_STREAMOFF) {
		ret = -EPERM;
	} else {
		if (capture_num > 0) {
			priv->still_inf.remaining_capnum = capture_num;
		} else {
			priv->still_inf.remaining_capnum = VIDEO_REMAINING_CAPNUM_INFINITY;
		}

		/* Control video stream prior to still stream */

		flags = enter_critical_section();

		next_video_state = estimate_next_video_state(priv, CAUSE_STILL_START);
		ret = change_video_state(priv, next_video_state);

		leave_critical_section(flags);

		if (ret < 0) {
			goto error;
		}
		dma_container = video_framebuff_get_dma_container(&priv->still_inf.bufinf);
		if (dma_container) {
			/* Start video stream DMA */
			ret = video_devops->set_buf(priv->dev, V4L2_BUF_TYPE_STILL_CAPTURE, dma_container->buf.m.userptr, dma_container->buf.length);
			if (ret < 0) {
				goto error;
			}
			priv->still_inf.state = VIDEO_STATE_DMA;
		} else {
			priv->still_inf.state = VIDEO_STATE_STREAMON;
		}
	}
error:
	video_unlock(&priv->still_inf.lock_state);

	return ret;
}

static int video_takepict_stop(FAR video_upperhalf_t *priv, bool halfpush)
{
	int ret = OK;
	irqstate_t flags;
	enum video_state_e next_video_state;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	video_lock(&priv->still_inf.lock_state);

	if ((priv->still_inf.state == VIDEO_STATE_STREAMOFF) && (priv->still_inf.remaining_capnum == VIDEO_REMAINING_CAPNUM_INFINITY)) {
		ret = -EPERM;
	} else {
		flags = enter_critical_section();
		if (priv->still_inf.state == VIDEO_STATE_DMA) {
			video_devops->cancel_dma(priv->dev);
		}
		leave_critical_section(flags);

		priv->still_inf.state = VIDEO_STATE_STREAMOFF;
		priv->still_inf.remaining_capnum = VIDEO_REMAINING_CAPNUM_INFINITY;

		/* Control video stream */

		video_lock(&priv->video_inf.lock_state);
		next_video_state = estimate_next_video_state(priv, CAUSE_STILL_STOP);
		ret = change_video_state(priv, next_video_state);
		video_unlock(&priv->video_inf.lock_state);
	}

	video_unlock(&priv->still_inf.lock_state);

	return ret;
}

static int video_queryctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_queryctrl *ctrl)
{
	int ret;
	struct v4l2_query_ext_ctrl ext_ctrl;

	if ((ctrl == NULL) || (priv == NULL)) {
		return -EINVAL;
	}

	/* Replace to VIDIOC_QUERY_EXT_CTRL format */

	ext_ctrl.ctrl_class = ctrl->ctrl_class;
	ext_ctrl.id = ctrl->id;

	ret = video_query_ext_ctrl(priv, &ext_ctrl);

	if (ret != OK) {
		return ret;
	}

	if ((ext_ctrl.type == V4L2_CTRL_TYPE_INTEGER64) || (ext_ctrl.type == V4L2_CTRL_TYPE_U8) || (ext_ctrl.type == V4L2_CTRL_TYPE_U16) || (ext_ctrl.type == V4L2_CTRL_TYPE_U32)) {
		/* Unsupported type in VIDIOC_QUERYCTRL */

		return -EINVAL;
	}

	/* Replace gotten value to VIDIOC_QUERYCTRL */

	ctrl->type = ext_ctrl.type;
	ctrl->minimum = ext_ctrl.minimum;
	ctrl->maximum = ext_ctrl.maximum;
	ctrl->step = ext_ctrl.step;
	ctrl->default_value = ext_ctrl.default_value;
	ctrl->flags = ext_ctrl.flags;
	strncpy(ctrl->name, ext_ctrl.name, sizeof(ctrl->name));

	return OK;
}

static int video_query_ext_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_query_ext_ctrl *ctrl)
{
	int ret;

	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->get_range_of_ctrlvalue == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->get_range_of_ctrlvalue(priv->dev, ctrl);

	return ret;
}

static int video_querymenu(FAR video_upperhalf_t *priv, FAR struct v4l2_querymenu *menu)
{
	int ret;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	if ((video_devops == NULL) || (video_devops->get_menu_of_ctrlvalue == NULL)) {
		return -ENOSYS;
	}

	ret = video_devops->get_menu_of_ctrlvalue(priv->dev, menu);

	return ret;
}

static int video_get_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_control *ctrl)
{
	int ret;
	struct v4l2_ext_controls ext_controls;
	struct v4l2_ext_control control;

	if (ctrl == NULL) {
		return -EINVAL;
	}

	/* Replace to VIDIOC_G_EXT_CTRLS format */

	control.id = ctrl->id;

	ext_controls.ctrl_class = V4L2_CTRL_CLASS_USER;
	ext_controls.count = 1;
	ext_controls.controls = &control;

	/* Execute VIDIOC_G_EXT_CTRLS */

	ret = video_get_ext_ctrls(priv, &ext_controls);

	if (ret == OK) {
		/* Replace gotten value to VIDIOC_G_CTRL parameter */

		ctrl->value = control.value;
	}

	return ret;
}

static int video_set_ctrl(FAR video_upperhalf_t *priv, FAR struct v4l2_control *ctrl)
{
	int ret;
	struct v4l2_ext_controls ext_controls;
	struct v4l2_ext_control control;

	if (ctrl == NULL) {
		return -EINVAL;
	}

	/* Replace to VIDIOC_S_EXT_CTRLS format */

	control.id = ctrl->id;
	control.value = ctrl->value;

	ext_controls.ctrl_class = V4L2_CTRL_CLASS_USER;
	ext_controls.count = 1;
	ext_controls.controls = &control;

	/* Execute VIDIOC_S_EXT_CTRLS */

	ret = video_set_ext_ctrls(priv, &ext_controls);

	return ret;
}

static int video_get_ext_ctrls(FAR video_upperhalf_t *priv, FAR struct v4l2_ext_controls *ctrls)
{
	int ret = OK;
	int cnt;
	FAR struct v4l2_ext_control *control;
	FAR struct video_devops_s *video_devops;

	if ((priv == NULL) || (ctrls == NULL)) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	for (cnt = 0, control = ctrls->controls; cnt < ctrls->count; cnt++, control++) {
		ret = video_devops->get_ctrlvalue(priv->dev, ctrls->ctrl_class, control);

		if (ret < 0) {
			/* Set cnt in that error occured */

			ctrls->error_idx = cnt;
			return ret;
		}
	}

	return ret;
}

static int video_set_ext_ctrls(FAR video_upperhalf_t *priv, FAR struct v4l2_ext_controls *ctrls)
{
	int ret;
	int cnt;
	FAR struct v4l2_ext_control *control;
	FAR struct video_devops_s *video_devops;

	if ((priv == NULL) || (ctrls == NULL)) {
		return -EINVAL;
	}

	if (priv->video_inf.state == VIDEO_STATE_STREAMON || priv->still_inf.state == VIDEO_STATE_STREAMON) {
		videodbg("Device currently streaming, unable to change parameters.\n");
		return -EBUSY;
	}

	video_devops = priv->dev->ops;

	for (cnt = 0, control = ctrls->controls; cnt < ctrls->count; cnt++, control++) {
		ret = video_devops->set_ctrlvalue(priv->dev, ctrls->ctrl_class, control);

		if (ret < 0) {
			/* Set cnt in that error occured */

			ctrls->error_idx = cnt;
			return ret;
		}
	}

	ret = video_devops->refresh(priv->dev);
	if (ret < 0) {
		ctrls->error_idx = cnt;
	}

	return ret;
}

/****************************************************************************
 * Name: video_ioctl
 *
 * Description:
 *   Standard character driver ioctl method.
 *
 ****************************************************************************/

static ssize_t video_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
	FAR struct inode *inode = filep->f_inode;
	FAR video_upperhalf_t *priv = (FAR video_upperhalf_t *) inode->i_private;
	ssize_t ret = OK;

	switch (cmd) {
	case VIDIOC_REQBUFS:
		ret = video_reqbufs(priv, (FAR struct v4l2_requestbuffers *)arg);
		break;
	case VIDIOC_QBUF:
		ret = video_qbuf(priv, (FAR struct v4l2_buffer *)arg);
		break;
	case VIDIOC_DQBUF:
		ret = video_dqbuf(priv, (FAR struct v4l2_buffer *)arg);
		break;
	case VIDIOC_CANCEL_DQBUF:
		ret = video_cancel_dqbuf(priv, (FAR enum v4l2_buf_type)arg);
		break;
	case VIDIOC_STREAMON:
		ret = video_streamon(priv, (FAR enum v4l2_buf_type *)arg);
		break;
	case VIDIOC_STREAMOFF:
		ret = video_streamoff(priv, (FAR enum v4l2_buf_type *)arg);
		break;
	case VIDIOC_DO_HALFPUSH:
		ret = video_do_halfpush(priv, arg);
		break;
	case VIDIOC_TAKEPICT_START:
		ret = video_takepict_start(priv, (int32_t) arg);
		break;
	case VIDIOC_TAKEPICT_STOP:
		ret = video_takepict_stop(priv, arg);
		break;
	case VIDIOC_ENUM_FMT:
		ret = video_enum_fmt(priv, (FAR struct v4l2_fmtdesc *)arg);
		break;
	case VIDIOC_ENUM_FRAMESIZES:
		ret = video_enum_framesizes(priv, (FAR struct v4l2_frmsizeenum *)arg);
		break;
	case VIDIOC_TRY_FMT:
		ret = video_try_fmt(priv, (FAR struct v4l2_format *)arg);
		break;
	case VIDIOC_S_FMT:
		ret = video_set_fmt(priv, (FAR struct v4l2_format *)arg);
		break;
	case VIDIOC_ENUM_FRAMEINTERVALS:
		ret = video_enum_frameintervals(priv, (FAR struct v4l2_frmivalenum *)arg);
		break;
	case VIDIOC_S_PARM:
		ret = video_set_parm(priv, (FAR struct v4l2_streamparm *)arg);
		break;
	case VIDIOC_QUERYCTRL:
		ret = video_queryctrl(priv, (FAR struct v4l2_queryctrl *)arg);
		break;
	case VIDIOC_QUERY_EXT_CTRL:
		ret = video_query_ext_ctrl(priv, (FAR struct v4l2_query_ext_ctrl *)arg);
		break;
	case VIDIOC_QUERYMENU:
		ret = video_querymenu(priv, (FAR struct v4l2_querymenu *)arg);
		break;
	case VIDIOC_G_CTRL:
		ret = video_get_ctrl(priv, (FAR struct v4l2_control *)arg);
		break;
	case VIDIOC_S_CTRL:
		ret = video_set_ctrl(priv, (FAR struct v4l2_control *)arg);
		break;
	case VIDIOC_G_EXT_CTRLS:
		ret = video_get_ext_ctrls(priv, (FAR struct v4l2_ext_controls *)arg);
		break;
	case VIDIOC_S_EXT_CTRLS:
		ret = video_set_ext_ctrls(priv, (FAR struct v4l2_ext_controls *)arg);
		break;
	default:
		videodbg("Unrecognized cmd: %d\n", cmd);
		ret = -ENOTTY;
		break;
	}

	return ret;
}

static int video_poll_setup(FAR video_upperhalf_t *priv, FAR struct pollfd *fds)
{
	if ((fds->events & POLLIN) == 0) {
		return -EDEADLK;
	}

	/* TODO: If data exists, get and sem_post */
	/*       If no data, wait dma             */

	return OK;
}

static int video_poll_teardown(FAR video_upperhalf_t *priv, FAR struct pollfd *fds)
{
	/* TODO: Delete poll wait information */

	return OK;
}

static int video_poll(FAR struct file *filep, FAR struct pollfd *fds, bool setup)
{
	FAR struct inode *inode = filep->f_inode;
	FAR video_upperhalf_t *priv = inode->i_private;

	if (setup) {
		return video_poll_setup(priv, fds);
	} else {
		return video_poll_teardown(priv, fds);
	}

	return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
FAR void *video_register(const char *devpath, FAR struct video_lowerhalf_s *dev)
{
	FAR video_upperhalf_t *priv;
	int ret;
	size_t allocsize;

	/* Input devpath Error Check */
	if (!devpath) {
		return NULL;
	}

	allocsize = strnlen(devpath, MAX_VIDEO_FILE_PATH - 1 /* Space for '\0' */);
	if ((allocsize < 2) || (devpath[0] != '/') || ((allocsize == (MAX_VIDEO_FILE_PATH - 1)) && (devpath[MAX_VIDEO_FILE_PATH] != '\0'))) {
		return NULL;
	}

	/* Initialize video device structure */

	priv = (FAR video_upperhalf_t *)kmm_malloc(sizeof(video_upperhalf_t));
	if (!priv) {
		videodbg("Failed to allocate instance\n");
		return NULL;
	}

	memset(priv, 0, sizeof(video_upperhalf_t));

	/* Save device path */

	priv->devpath = (FAR char *)kmm_malloc(allocsize + 1);
	if (!priv->devpath) {
		kmm_free(priv);
		return NULL;
	}
	memcpy(priv->devpath, devpath, allocsize);
	priv->devpath[allocsize] = '\0';

	/* Initialize semaphore */

	sem_init(&priv->lock_open_num, 0, 1);

	/* Initialize lower half reference */
	priv->dev = dev;

	/* Register the character driver */

	ret = register_driver(priv->devpath, &g_video_fops, 0666, priv);
	if (ret < 0) {
		videodbg("Failed to register driver: %d\n", ret);
		kmm_free(priv->devpath);
		kmm_free(priv);
		return NULL;
	}

	return (FAR void *)priv;
}

int video_unregister(FAR void *arg)
{
	int ret = OK;
	FAR video_upperhalf_t *priv = (video_upperhalf_t *) arg;

	if (!priv) {
		ret = -ENODEV;
	} else {
		sem_destroy(&priv->lock_open_num);

		unregister_driver((const char *)priv->devpath);

		kmm_free(priv->devpath);
		kmm_free(priv);
	}

	return ret;
}

int video_common_notify_dma_done(uint8_t err_code, uint32_t buf_type, uint32_t datasize, FAR void *arg)
{
	FAR video_upperhalf_t *priv = (FAR video_upperhalf_t *)arg;
	FAR video_type_inf_t *type_inf;
	FAR vbuf_container_t *container = NULL;
	FAR struct video_devops_s *video_devops;

	if (priv == NULL) {
		return -EINVAL;
	}

	video_devops = priv->dev->ops;

	type_inf = get_video_type_inf(priv, buf_type);
	if (type_inf == NULL) {
		return -EINVAL;
	}

	if (err_code == 0) {
		type_inf->bufinf.vbuf_dma->buf.flags = 0;
		if (type_inf->remaining_capnum > 0) {
			type_inf->remaining_capnum--;
		}
	} else {
		type_inf->bufinf.vbuf_dma->buf.flags = V4L2_BUF_FLAG_ERROR;
	}

	type_inf->bufinf.vbuf_dma->buf.bytesused = datasize;
	video_framebuff_dma_done(&type_inf->bufinf);

	if (is_sem_waited(&type_inf->wait_dma.dqbuf_wait_flg)) {
		/* If waiting DMA done in DQBUF,
		 * get/save container and unlock wait
		 */

		type_inf->wait_dma.done_container = video_framebuff_pop_curr_container(&type_inf->bufinf);
		type_inf->wait_dma.waitend_cause = VIDEO_WAITEND_CAUSE_DMADONE;
		sem_post(&type_inf->wait_dma.dqbuf_wait_flg);

		/* TODO:  in poll wait, unlock wait */
	}

	if (type_inf->remaining_capnum == 0) {
		video_devops->cancel_dma(priv->dev);
		type_inf->state = VIDEO_STATE_STREAMOFF;

		/* If stop still stream, notify it to video stream */

		if ((buf_type == V4L2_BUF_TYPE_STILL_CAPTURE) && is_sem_waited(&priv->video_inf.wait_dma.dqbuf_wait_flg)) {
			priv->video_inf.wait_dma.waitend_cause = VIDEO_WAITEND_CAUSE_STILLSTOP;
			sem_post(&priv->video_inf.wait_dma.dqbuf_wait_flg);
		}
	} else {
		container = video_framebuff_get_dma_container(&type_inf->bufinf);
		if (!container) {
			video_devops->cancel_dma(priv->dev);
			type_inf->state = VIDEO_STATE_STREAMON;
		} else {
			video_devops->set_buf(priv->dev, buf_type, container->buf.m.userptr, container->buf.length);
		}
	}

	return OK;
}
