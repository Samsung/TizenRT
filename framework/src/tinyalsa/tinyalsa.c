/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
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
*
* Developed by:
*
*   Copyright (C) 2013 Ken Pettit. All rights reserved.
*   Author: Ken Pettit <pettitkd@gmail.com>
*
* With ongoing support:
*
*   Copyright (C) 2014 Gregory Nutt. All rights reserved.
*   Author: Greory Nutt <gnutt@nuttx.org>
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
/* tinyalsa.c
**
** Copyright 2011, The Android Open Source Project
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of The Android Open Source Project nor the names of
**       its contributors may be used to endorse or promote products derived
**       from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY The Android Open Source Project ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL The Android Open Source Project BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
** DAMAGE.
*/

#include <tinyara/config.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <limits.h>

#define __force
#define __bitwise
#define __user

#include <tinyara/audio/audio.h>
#include <tinyalsa/tinyalsa.h>

#define MILLI_TO_NANO	1000000

#ifndef CONFIG_AUDIO_NUM_BUFFERS
#define CONFIG_AUDIO_NUM_BUFFERS  8
#endif

#ifndef CONFIG_AUDIO_BUFSIZE
#define CONFIG_AUDIO_BUFSIZE  2048
#endif

#define PCM_ERROR_MAX 128

/** A PCM handle.
 * @ingroup libtinyalsa-pcm
 */
struct pcm {
	/** The PCM's file descriptor */
	int fd;
	/** Flags that were passed to @ref pcm_open */
	unsigned int flags;
	/** Whether the PCM is running or not */
	int running:1;
	/** Whether or not the PCM has been prepared */
	int prepared:1;
	/** Whether the PCM is in draining state */
	int draining:1;
	/** Size of the buffer */
	unsigned int buffer_size;
	/* Number of buffers */
	unsigned int buffer_cnt;
	/** Description of the last error that occured */
	char error[PCM_ERROR_MAX];
	/** Configuration that was passed to @ref pcm_open */
	struct pcm_config config;
	/** The subdevice corresponding to the PCM */
	unsigned int subdevice;
#ifdef CONFIG_AUDIO_MULTI_SESSION
	/* Session pointer to be managed by driver */
	void *session;
#endif
	/* Message queue for the playthread */
	mqd_t mq;
	/* Name of our message queue */
	char mqname[16];
	/* Pointers to apb buffers */
	struct ap_buffer_s **pBuffers;
	/* Index of the next apb to be queued during start of play or record */
	unsigned int buf_idx;
	/* Pointer to next apb for read / write */
	struct ap_buffer_s *next_buf;
	/* Size of data in bytes in the next buffer */
	unsigned int next_size;
	/* Offset of data in next buffer */
	unsigned int next_offset;
	/* Index of next buffer in ring for mmap operations */
	unsigned int mmap_idx;
};

int pcm_start(struct pcm *pcm);
int pcm_stop(struct pcm *pcm);

static int oops(struct pcm *pcm, int e, const char *fmt, ...)
{
	va_list ap;
	int sz;

	va_start(ap, fmt);
	vsnprintf(pcm->error, PCM_ERROR_MAX, fmt, ap);
	va_end(ap);
	sz = strlen(pcm->error);

	if (errno) {
		snprintf(pcm->error + sz, PCM_ERROR_MAX - sz, ": %s errno : %d", strerror(e), errno);
	}
	return -1;
}

/** Gets the buffer size of the PCM.
 * @param pcm A PCM handle.
 * @return The buffer size of the PCM.
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_get_buffer_size(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return 0;
	}

	return pcm->buffer_size;
}

/** Gets the channel count of the PCM.
 * @param pcm A PCM handle.
 * @return The channel count of the PCM.
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_get_channels(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return 0;
	}

	return pcm->config.channels;
}

/** Gets the PCM configuration.
 * @param pcm A PCM handle.
 * @return The PCM configuration.
 *  This function only returns NULL if
 *  @p pcm is NULL.
 * @ingroup libtinyalsa-pcm
 * */
const struct pcm_config *pcm_get_config(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return NULL;
	}
	return &pcm->config;
}

/** Gets the rate of the PCM.
 * The rate is given in frames per second.
 * @param pcm A PCM handle.
 * @return The rate of the PCM.
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_get_rate(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return 0;
	}

	return pcm->config.rate;
}

/** Gets the format of the PCM.
 * @param pcm A PCM handle.
 * @return The format of the PCM.
 * @ingroup libtinyalsa-pcm
 */
enum pcm_format pcm_get_format(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return PCM_FORMAT_NONE;
	}

	return pcm->config.format;
}

/** Gets the file descriptor of the PCM.
 * Useful for extending functionality of the PCM when needed.
 * @param pcm A PCM handle.
 * @return The file descriptor of the PCM.
 * @ingroup libtinyalsa-pcm
 */
int pcm_get_file_descriptor(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return -EINVAL;
	}

	return pcm->fd;
}

/** Gets the error message for the last error that occured.
 * If no error occured and this function is called, the results are undefined.
 * @param pcm A PCM handle.
 * @return The error message of the last error that occured.
 * @ingroup libtinyalsa-pcm
 */
const char *pcm_get_error(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return NULL;
	}

	return pcm->error;
}

/** Sets the PCM configuration.
 * @param pcm A PCM handle.
 * @param config The configuration to use for the
 *  PCM. This parameter may be NULL, in which case
 *  the default configuration is used.
 * @returns Zero on success, a negative errno value
 *  on failure.
 * @ingroup libtinyalsa-pcm
 * */
static int pcm_set_config(struct pcm *pcm, const struct pcm_config *config)
{
	struct audio_caps_desc_s cap_desc;
	struct ap_buffer_info_s buf_info;
	int ret;

	if (pcm == NULL) {
		return -EINVAL;
	} else if (config == NULL) {
		config = &pcm->config;
		pcm->config.channels = 2;
		pcm->config.rate = 16000;
		pcm->config.format = PCM_FORMAT_S16_LE;
		pcm->config.period_size = 1024;
		pcm->config.period_count = 8;
	} else {
		pcm->config = *config;
	}

#ifdef CONFIG_AUDIO_MULTI_SESSION
	cap_desc.session = pcm->session;
#endif
	cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
	if (pcm->flags & PCM_IN) {
		cap_desc.caps.ac_type = AUDIO_TYPE_INPUT;
	} else {
		cap_desc.caps.ac_type = AUDIO_TYPE_OUTPUT;
	}
	cap_desc.caps.ac_channels = config->channels;
	cap_desc.caps.ac_controls.hw[0] = config->rate;
	cap_desc.caps.ac_controls.b[2] = pcm_format_to_bits(config->format);

	ret = ioctl(pcm->fd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	if (ret < 0) {
		return oops(pcm, errno, "AUDIOIOC_CONFIGURE ioctl failed\n");
	}

	/* Gain buffer size and count through the ioctl */
	buf_info.buffer_size = config->period_size;
	buf_info.nbuffers = config->period_count;
	if (ioctl(pcm->fd, AUDIOIOC_GETBUFFERINFO, (unsigned long)&buf_info) < 0) {
		/* Driver doesn't report it's buffer size.  Use our default. */
		buf_info.buffer_size = CONFIG_AUDIO_BUFSIZE;
		buf_info.nbuffers = CONFIG_AUDIO_NUM_BUFFERS;
	}
	pcm->config.period_size = buf_info.buffer_size;
	pcm->config.period_count = buf_info.nbuffers;
	pcm->buffer_size = buf_info.buffer_size;
	pcm->buffer_cnt = buf_info.nbuffers;

	return 0;
}

/** Gets the subdevice on which the pcm has been opened.
 * @param pcm A PCM handle.
 * @return The subdevice on which the pcm has been opened */
unsigned int pcm_get_subdevice(const struct pcm *pcm)
{
	if (pcm == NULL) {
		return 0;
	}

	return pcm->subdevice;
}

/** Determines the number of bits occupied by a @ref pcm_format.
 * @param format A PCM format.
 * @return The number of bits associated with @p format
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_format_to_bits(enum pcm_format format)
{
	switch (format) {
	case PCM_FORMAT_S32_LE:
	case PCM_FORMAT_S32_BE:
	case PCM_FORMAT_S24_LE:
	case PCM_FORMAT_S24_BE:
		return 32;
	case PCM_FORMAT_S24_3LE:
	case PCM_FORMAT_S24_3BE:
		return 24;
	default:
	case PCM_FORMAT_S16_LE:
	case PCM_FORMAT_S16_BE:
		return 16;
	case PCM_FORMAT_S8:
		return 8;
	case PCM_FORMAT_NONE:
		return 0;
	};
}

/** Determines how many frames of a PCM can fit into a number of bytes.
 * @param pcm A PCM handle.
 * @param bytes The number of bytes.
 * @return The number of frames that may fit into @p bytes
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_bytes_to_frames(const struct pcm *pcm, unsigned int bytes)
{
	if (pcm == NULL || bytes == 0) {
		return 0;
	}

	return bytes / (pcm->config.channels * (pcm_format_to_bits(pcm->config.format) >> 3));
}

/** Determines how many bytes are occupied by a number of frames of a PCM.
 * @param pcm A PCM handle.
 * @param frames The number of frames of a PCM.
 * @return The bytes occupied by @p frames.
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_frames_to_bytes(const struct pcm *pcm, unsigned int frames)
{
	if (pcm == NULL || frames == 0) {
		return 0;
	}

	return frames * pcm->config.channels * (pcm_format_to_bits(pcm->config.format) >> 3);
}

static int pcm_areas_copy(struct pcm *pcm, unsigned int pcm_offset, void *pcm_buf, char *buf, unsigned int src_offset, unsigned int frames)
{
	int size_bytes = pcm_frames_to_bytes(pcm, frames);
	int pcm_offset_bytes = pcm_frames_to_bytes(pcm, pcm_offset);
	int src_offset_bytes = pcm_frames_to_bytes(pcm, src_offset);

	/* interleaved only atm */
	if (pcm->flags & PCM_IN) {
		memcpy(buf + src_offset_bytes, (char *)pcm_buf + pcm_offset_bytes, size_bytes);
	} else {
		memcpy((char *)pcm_buf + pcm_offset_bytes, buf + src_offset_bytes, size_bytes);
	}
	return 0;
}

static int pcm_mmap_transfer_areas(struct pcm *pcm, char *buf, unsigned int offset, unsigned int size)
{
	void *pcm_areas;
	unsigned int pcm_offset, frames, count = 0;
	int ret;

	while (size > 0) {
		frames = size;
		pcm_mmap_begin(pcm, &pcm_areas, &pcm_offset, &frames);
		pcm_areas_copy(pcm, pcm_offset, pcm_areas, buf, offset, frames);
		ret = pcm_mmap_commit(pcm, pcm_offset, frames);
		if (ret < 0) {
			return ret;
		}

		offset += frames;
		count += frames;
		size -= frames;
	}
	return count;
}

/** Writes audio samples to PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_OUT flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param frame_count The number of frames occupied by the sample array.
 *  This value should not be greater than @ref INT_MAX.
 * @return On success, this function returns the number of frames written; otherwise, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_writei(struct pcm *pcm, const void *data, unsigned int frame_count)
{
	unsigned int nbytes = 0, pending = 0, offset = 0;
	struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb;
	struct audio_msg_s msg;
	unsigned int size;
	int prio;

	if (pcm == NULL || data == NULL || frame_count == 0) {
		return -EINVAL;
	}

	if (!(pcm->flags & PCM_OUT)) {
		return -EINVAL;
	}

	if (frame_count > INT_MAX) {
		return -EINVAL;
	}

	pending = pcm_frames_to_bytes(pcm, frame_count);

	while (pending > 0) {
		nbytes = pending > pcm_frames_to_bytes(pcm, pcm->buffer_size) ? pcm_frames_to_bytes(pcm, pcm->buffer_size) : pending;
		if (pcm->buf_idx < pcm->buffer_cnt) {
			/* If we have empty buffers, fill them first */
			memcpy(pcm->pBuffers[pcm->buf_idx]->samp, (char *)data + offset, nbytes);
			apb = pcm->pBuffers[pcm->buf_idx];
			pcm->buf_idx++;
		} else {
			/* We dont have any empty buffers. wait for deque message from kernel */
			size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
			if (size != sizeof(msg)) {
				/* Interrupted by a signal? What to do? */
				return oops(pcm, EINTR, "Interrupted while waiting for deque message from kernel\n");
			}
			if (msg.msgId == AUDIO_MSG_DEQUEUE) {
				apb = (struct ap_buffer_s *)msg.u.pPtr;
				memcpy(apb->samp, (char *)data + offset, nbytes);
			} else if (msg.msgId == AUDIO_MSG_XRUN) {
				/* Underrun to be handled by client */
				return -EPIPE;
			} else {
				return oops(pcm, EINTR, "Recieved unexpected msg (id = %d) while waiting for deque message from kernel\n", msg.msgId);
			}
		}

		/* Buffer is ready. Enque it */
#ifdef CONFIG_AUDIO_MULTI_SESSION
		bufdesc.session = pcm->session;
#endif
		apb->nbytes = nbytes;
		apb->curbyte = 0;
		apb->flags = 0;
		bufdesc.numbytes = apb->nbytes;
		bufdesc.u.pBuffer = apb;
		if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
			return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed\n");
		}
		/* If playback is not already started, start now! */
		if ((!pcm->running) && (pcm_start(pcm) < 0)) {
			return -errno;
		} else {
			pcm->running = 1;
		}

		offset += nbytes;
		pending -= nbytes;
	}

	return pcm_bytes_to_frames(pcm, nbytes);
}

/** Reads audio samples from PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_IN flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param frame_count The number of frames occupied by the sample array.
 *  This value should not be greater than @ref INT_MAX.
 * @return On success, this function returns the number of frames written; otherwise, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_readi(struct pcm *pcm, void *data, unsigned int frame_count)
{
	int nbytes = 0, pending = 0, offset = 0;
	struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb;
	struct audio_msg_s msg;
	unsigned int size;
	int prio;
	struct timespec st_time;

	if (pcm == NULL || data == NULL || frame_count == 0) {
		return -EINVAL;
	}

	if (!(pcm->flags & PCM_IN)) {
		return -EINVAL;
	}

	if (frame_count > INT_MAX) {
		return -EINVAL;
	}

	pending = pcm_frames_to_bytes(pcm, frame_count);

	bufdesc.numbytes = pcm_frames_to_bytes(pcm, (pcm->buffer_size));

#ifdef CONFIG_AUDIO_MULTI_SESSION
	bufdesc.session = pcm->session;
#endif

	/* If device is not yet started, start now! */
	if ((!pcm->draining) && (!pcm->running) && (pcm_start(pcm) < 0)) {
		return -errno;
	}

	while (pending > 0) {

		/* Read data */
		if (pcm->next_buf != NULL) {
			/* We have a pending buffer with some data in it. */
			apb = pcm->next_buf;
			if (pending <= pcm->next_size) {
				/* User wants less data than we have in buffer.
				   Copy the requested amount of data to user buffer and return */
				memcpy((char *)data + offset, apb->samp + pcm->next_offset, pending);
				pcm->next_size -= pending;
				pcm->next_offset += pending;

				nbytes = pending;
			} else {
				/* User wants more data than we have in buffer.
				   Copy available data to user buffer, enqueue buffer and return */
				memcpy((char *)data + offset, apb->samp + pcm->next_offset, pcm->next_size);

				nbytes = pcm->next_size;
				pcm->next_size = 0;
				pcm->next_offset = 0;
				pcm->next_buf = NULL;

				apb->nbytes = 0;
				apb->curbyte = 0;
				apb->flags = 0;
				bufdesc.u.pBuffer = apb;
				if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
					return oops(pcm, errno, "failed to enque buffer after read\n");
				}
			}

		} else {
			/* We got here because we dont have any pending buffers */
			/* Wait for deque message from kernel */
			if (pcm->draining) {
				/* When we are in draining state, we will check for buffers already in the msg queue
				   If there is no buffer on the queue, we return immediately */
				clock_gettime(CLOCK_REALTIME, &st_time);
				size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
				if (size != sizeof(msg)) {
					pcm->draining = 0;
					pcm->next_size = 0;
					pcm->next_offset = 0;
					pcm->next_buf = NULL;
					return pcm_frames_to_bytes(pcm, frame_count) - pending;
				}
			} else {
				size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
				if (size != sizeof(msg)) {
					/* Interrupted by a signal? What to do? */
					return oops(pcm, EINTR, "Interrupted while waiting for deque message from kernel\n");
				}
			}
			if (msg.msgId == AUDIO_MSG_DEQUEUE) {
				apb = (struct ap_buffer_s *)msg.u.pPtr;
				if (pending <= apb->nbytes) {
					/* User wants less data than we have in buffer.
					   Copy the requested amount of data and keep the buffer as pending buffer */
					memcpy((char *)data + offset, apb->samp, pending);

					pcm->next_buf = apb;
					pcm->next_offset = pending;
					pcm->next_size = apb->nbytes - pending;

					nbytes = pending;
				} else {
					/* User wants more data than we have in buffer.
					   Copy available data to user buffer, enqueue buffer and return */
					memcpy((char *)data + offset, apb->samp, apb->nbytes);
					nbytes = apb->nbytes;

					if (!pcm->draining) {
						/* Dont enque the buffer since we are draining and
						   the pcm has already been stopped */
						apb->nbytes = 0;
						apb->curbyte = 0;
						apb->flags = 0;
						bufdesc.u.pBuffer = apb;
						if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
							return oops(pcm, errno, "failed to enque buffer after read\n");
						}
					}
				}
			} else if (msg.msgId == AUDIO_MSG_XRUN) {
				/* Underrun to be handled by client */
				return -EPIPE;
			} else {
				return oops(pcm, EINTR, "Recieved unexpected msg (id = %d) while waiting for deque message from kernel\n", msg.msgId);
			}
		}

		offset += nbytes;
		pending -= nbytes;
	}

	return pcm_bytes_to_frames(pcm, pcm_frames_to_bytes(pcm, frame_count) - pending);
}

/** Writes audio samples to PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_OUT flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param count The number of bytes occupied by the sample array.
 * @return On success, this function returns zero; otherwise, a negative number.
 * @deprecated
 * @ingroup libtinyalsa-pcm
 */
int pcm_write(struct pcm *pcm, const void *data, unsigned int count)
{
	return pcm_writei(pcm, data, pcm_bytes_to_frames(pcm, count)) > 0 ? 0 : -1;
}

/** Reads audio samples from PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_IN flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param count The number of bytes occupied by the sample array.
 * @return On success, this function returns zero; otherwise, a negative number.
 * @deprecated
 * @ingroup libtinyalsa-pcm
 */
int pcm_read(struct pcm *pcm, void *data, unsigned int count)
{
	return pcm_readi(pcm, data, pcm_bytes_to_frames(pcm, count)) > 0 ? 0 : -1;
}

/** Closes a PCM returned by @ref pcm_open.
 * @param pcm A PCM returned by @ref pcm_open.
 *  May not be NULL.
 * @return Always returns zero.
 * @ingroup libtinyalsa-pcm
 */
int pcm_close(struct pcm *pcm)
{
	int x;
	struct audio_buf_desc_s buf_desc;

	if (pcm == NULL) {
		return -EINVAL;
	}

	if (pcm->fd < 0) {
		free(pcm);
		return 0;
	}

	if (pcm->running) {
		pcm_stop(pcm);
	}

	ioctl(pcm->fd, AUDIOIOC_UNREGISTERMQ, (unsigned long)pcm->mq);
#ifdef CONFIG_AUDIO_MULTI_SESSION
	ioctl(pcm->fd, AUDIOIOC_RELEASE, (unsigned long)pcm->session);
#else
	ioctl(pcm->fd, AUDIOIOC_RELEASE, 0);
#endif

#ifdef CONFIG_AUDIO_MULTI_SESSION
	buf_desc.session = pcm->session;
#endif

	if (pcm->pBuffers != NULL) {
		for (x = 0; x < pcm->buffer_cnt; x++) {
			if (pcm->pBuffers[x] != NULL) {
				buf_desc.u.pBuffer = pcm->pBuffers[x];
				ioctl(pcm->fd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
			}
		}
		free(pcm->pBuffers);
	}

	mq_unlink(pcm->mqname);

	close(pcm->fd);

	free(pcm);
	return 0;
}

/** Opens a PCM by it's name.
 * @param name The name of the PCM.
 *  The name is given in the format: <i>hw</i>:<b>card</b>,<b>device</b>
 * @param flags Specify characteristics and functionality about the pcm.
 *  May be a bitwise AND of the following:
 *   - @ref PCM_IN
 *   - @ref PCM_OUT
 *   - @ref PCM_MMAP
 * @param config The hardware and software parameters to open the PCM with.
 * @returns A PCM structure.
 *  If an error occurs allocating memory for the PCM, NULL is returned.
 *  Otherwise, client code should check that the PCM opened properly by calling @ref pcm_is_ready.
 *  If @ref pcm_is_ready, check @ref pcm_get_error for more information.
 * @ingroup libtinyalsa-pcm
 */
struct pcm *pcm_open_by_name(const char *name, unsigned int flags, const struct pcm_config *config)
{
	unsigned int card, device;
	if (name == NULL) {
		return NULL;
	}
	if ((name[0] != 'h')
		|| (name[1] != 'w')
		|| (name[2] != ':')) {
		return NULL;
	} else if (sscanf(&name[3], "%u,%u", &card, &device) != 2) {
		return NULL;
	}
	return pcm_open(card, device, flags, config);
}

/** Opens a PCM.
 * @param card The card that the pcm belongs to.
 *  The default card is zero.
 * @param device The device that the pcm belongs to.
 *  The default device is zero.
 * @param flags Specify characteristics and functionality about the pcm.
 *  May be a bitwise AND of the following:
 *   - @ref PCM_IN
 *   - @ref PCM_OUT
 *   - @ref PCM_MMAP
 * @param config The hardware and software parameters to open the PCM with.
 * @returns A PCM structure.
 *  If an error occurs allocating memory for the PCM, NULL is returned.
 *  Otherwise, client code should check that the PCM opened properly by calling @ref pcm_is_ready.
 *  If @ref pcm_is_ready, check @ref pcm_get_error for more information.
 * @ingroup libtinyalsa-pcm
 */
struct pcm *pcm_open(unsigned int card, unsigned int device, unsigned int flags, const struct pcm_config *config)
{
	struct pcm *pcm;
	char fn[256];
	int ret = 0;
	struct mq_attr attr;
	struct audio_buf_desc_s buf_desc;
	int x;

	pcm = malloc(sizeof(struct pcm));
	if (!pcm) {
		return NULL;
	}
	memset(pcm, 0, sizeof(struct pcm));

	snprintf(fn, sizeof(fn), "/dev/audio/pcmC%uD%u%c", card, device, flags & PCM_IN ? 'c' : 'p');

	pcm->flags = flags;
	if (flags & PCM_IN) {
		pcm->fd = open(fn, O_RDONLY);
	} else {
		pcm->fd = open(fn, O_WRONLY);
	}

	if (pcm->fd < 0) {
		oops(pcm, errno, "cannot open device '%s'\n", fn);
		return pcm;
	}

	/* Try to reserve the device */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	ret = ioctl(pcm->fd, AUDIOIOC_RESERVE, (unsigned long)&pcm->session);
#else
	ret = ioctl(pcm->fd, AUDIOIOC_RESERVE, 0);
#endif
	if (ret < 0) {
		/* Device is busy or error */
		oops(pcm, errno, "Failed to reserve device\n");
		goto fail_close;
	}

	if (pcm_set_config(pcm, config) != 0) {
		goto fail_close;
	}

	/* Create a message queue */
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	snprintf(pcm->mqname, sizeof(pcm->mqname), "/tmp/%0lx", (unsigned long)((uintptr_t) pcm));

	pcm->mq = mq_open(pcm->mqname, O_RDWR | O_CREAT, 0644, &attr);
	if (pcm->mq == NULL) {
		/* Unable to open message queue! */
		oops(pcm, errno, "mq_open failed\n");
		goto fail_close;
	}

	/* Register our message queue with the audio device */
	ret = ioctl(pcm->fd, AUDIOIOC_REGISTERMQ, (unsigned long)pcm->mq);
	if (ret < 0) {
		oops(pcm, errno, "register mq failed\n");
		goto fail_after_mq;
	}
	
	/* Create array of pointers to buffers */
	pcm->pBuffers = (FAR struct ap_buffer_s **)malloc(pcm->buffer_cnt * sizeof(FAR void *));
	if (pcm->pBuffers == NULL) {
		/* Error allocating memory for buffer storage! */
		goto fail_after_mq;
	}

	/* Create our audio pipeline buffers to use for queueing up data */

	for (x = 0; x < pcm->buffer_cnt; x++) {
		pcm->pBuffers[x] = NULL;
		/* Fill in the buffer descriptor struct to issue an alloc request */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		buf_desc.session = pcm->session;
#endif
		buf_desc.numbytes = pcm_frames_to_bytes(pcm, (pcm->buffer_size));
		buf_desc.u.ppBuffer = &pcm->pBuffers[x];

		ret = ioctl(pcm->fd, AUDIOIOC_ALLOCBUFFER, (unsigned long)&buf_desc);

		if (ret != sizeof(buf_desc)) {
			/* Buffer alloc Operation not supported or error allocating! */
			oops(pcm, ENOMEM, "Could not allocate buffer %d\n", x);
			goto fail_cleanup_buffers;
		}
		pcm->pBuffers[x]->nbytes = 0;
		pcm->pBuffers[x]->curbyte = 0;
		pcm->pBuffers[x]->flags = 0;
	}

	pcm->prepared = 0;
	pcm->running = 0;
	pcm->draining = 0;
	pcm->buf_idx = 0;
	pcm->next_size = 0;
	pcm->next_offset = 0;
	pcm->next_buf = NULL;
	pcm->mmap_idx = 0;

	return pcm;

fail_cleanup_buffers:
	if (pcm->pBuffers != NULL) {
		for (x = 0; x < pcm->buffer_cnt; x++) {
			/* Fill in the buffer descriptor struct to issue a free request */
			if (pcm->pBuffers[x] != NULL) {
				buf_desc.u.pBuffer = pcm->pBuffers[x];
				ioctl(pcm->fd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
			}
		}
		/* Free the pointers to the buffers */
		free(pcm->pBuffers);
	}

	pcm->buf_idx = 0;

fail_after_mq:
	mq_close(pcm->mq);			/* Close the message queue */
	mq_unlink(pcm->mqname);		/* Unlink the message queue */

fail_close:
	close(pcm->fd);
	pcm->fd = -1;
	return pcm;
}

/** Checks if a PCM file has been opened without error.
 * @param pcm A PCM handle.
 *  May be NULL.
 * @return If a PCM's file descriptor is not valid or the pointer is NULL, it returns zero.
 *  Otherwise, the function returns one.
 * @ingroup libtinyalsa-pcm
 */
int pcm_is_ready(const struct pcm *pcm)
{
	if (pcm != NULL) {
		return pcm->fd >= 0;
	}
	return 0;
}

/** Links two PCMs.
 * After this function is called, the two PCMs will prepare, start and stop in sync (at the same time).
 * If an error occurs, the error message will be written to @p pcm1.
 * @param pcm1 A PCM handle.
 * @param pcm2 Another PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_link(struct pcm *pcm1, struct pcm *pcm2)
{
	return -1;
}

/** Unlinks a PCM.
 * @see @ref pcm_link
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_unlink(struct pcm *pcm)
{
	return -1;
}

/** Prepares a PCM, if it has not been prepared already.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_prepare(struct pcm *pcm)
{
	if (pcm == NULL) {
		return -EINVAL;
	}
	if (pcm->prepared) {
		return 0;
	}

#ifdef CONFIG_AUDIO_MULTI_SESSION
	if (ioctl(pcm->fd, AUDIOIOC_PREPARE, (unsigned long)pcm->session) < 0)
#else
	if (ioctl(pcm->fd, AUDIOIOC_PREPARE, 0) < 0)
#endif
	{
		return oops(pcm, errno, "cannot prepare pcm\n");
	}

	pcm->prepared = 1;
	return 0;
}

/** Starts a PCM.
 * If the PCM has not been prepared,
 * it is prepared in this function.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_start(struct pcm *pcm)
{
	struct audio_buf_desc_s bufdesc;

	if (pcm == NULL) {
		return -EINVAL;
	}

	if (pcm->running) {
		return 0;
	}

	int prepare_error = pcm_prepare(pcm);
	if (prepare_error) {
		return prepare_error;
	}

	/* If the device is opened for read and Our buffers are not enqued. Enque them now. */
	if (pcm->flags & PCM_IN) {
		if (pcm->draining) {
			/* Remove any pending messages from the message queue */
			struct audio_msg_s msg;
			unsigned int size;
			int prio;
			struct timespec st_time;
			do {
				clock_gettime(CLOCK_REALTIME, &st_time);
				size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
			} while (size > 0);
		}

#ifdef CONFIG_AUDIO_MULTI_SESSION
		bufdesc.session = pcm->session;
#endif
		bufdesc.numbytes = pcm_frames_to_bytes(pcm, pcm->buffer_size);
		for (pcm->buf_idx = 0; pcm->buf_idx < pcm->buffer_cnt; pcm->buf_idx++) {
			bufdesc.u.pBuffer = pcm->pBuffers[pcm->buf_idx];
			bufdesc.u.pBuffer->nbytes = 0;
			bufdesc.u.pBuffer->curbyte = 0;
			bufdesc.u.pBuffer->flags = 0;
			if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
				return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed\n");
			}
			pcm->pBuffers[pcm->buf_idx]->flags |= AUDIO_APB_MMAP_ENQUEUED;
		}
	} else {
		/* If device is opened for playback, we need atleast one buffer to be enqueued before
		starting the codec. Else we return error here */
		if (pcm->buf_idx == 0) {
			return oops(pcm, -EINVAL, "ERROR Trying to start PCM for playback without enqueuing buffers\n");
		}
	}
#ifdef CONFIG_AUDIO_MULTI_SESSION
	if (ioctl(pcm->fd, AUDIOIOC_START, (unsigned long)pcm->session) < 0)
#else
	if (ioctl(pcm->fd, AUDIOIOC_START, 0) < 0)
#endif
	{
		return oops(pcm, errno, "cannot start channel\n");
	}

	pcm->running = 1;
	pcm->draining = 0;

	return 0;
}

/** Stops a PCM.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_stop(struct pcm *pcm)
{
	if (pcm == NULL) {
		return -EINVAL;
	}

	if (!pcm->running) {
		return oops(pcm, EINVAL, "PCM already in stop state\n");
	}
#ifdef CONFIG_AUDIO_MULTI_SESSION
	if (ioctl(pcm->fd, AUDIOIOC_STOP, (unsigned long)pcm->session) < 0)
#else
	if (ioctl(pcm->fd, AUDIOIOC_STOP, 0) < 0)
#endif
		return oops(pcm, errno, "cannot stop channel\n");

	/* Remove any pending messages from the message queue */
	struct audio_msg_s msg;
	int size;
	int prio;
	struct timespec st_time;
	do {
		clock_gettime(CLOCK_REALTIME, &st_time);
		size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
	} while (size > 0);

	pcm->prepared = 0;
	pcm->running = 0;
	pcm->draining = 0;
	pcm->buf_idx = 0;
	pcm->next_size = 0;
	pcm->next_offset = 0;
	pcm->next_buf = NULL;
	pcm->mmap_idx = 0;

	return 0;
}

/** Stops a PCM. Any data present in the buffers will be dropped.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_drop(struct pcm *pcm)
{
	return pcm_stop(pcm);
}

/** Stop a PCM preserving pending frames.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_drain(struct pcm *pcm)
{
	if (pcm == NULL) {
		return -EINVAL;
	}

	if (!pcm->running) {
		return oops(pcm, EINVAL, "PCM is already stopped.\n");
	}

	if (pcm->flags & PCM_OUT) {
		struct audio_msg_s msg;
		unsigned int size;
		int prio;

		/* Playback case */
		/* Wait for all enqueued buffers to get dequeued. */
		while (pcm->buf_idx > 0) {
			/* Wait for deque message from kernel */
			size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
			if (size != sizeof(msg)) {
				/* Interrupted by a signal? What to do? */
				return oops(pcm, EINTR, "Interrupted while waiting for deque message from kernel\n");
			}
			if (msg.msgId == AUDIO_MSG_DEQUEUE) {
				pcm->buf_idx--;
			} else if (msg.msgId == AUDIO_MSG_XRUN) {
				/* Underrun to be handled by client */
				return -EPIPE;
			}
		}

		return pcm_stop(pcm);
	} else {
		struct audio_msg_s msg;
		int size;
		int prio;
		struct timespec st_time;
		/* Recording case */
		/* Stop the codec */
#ifdef CONFIG_AUDIO_MULTI_SESSION
		if (ioctl(pcm->fd, AUDIOIOC_STOP, (unsigned long)pcm->session) < 0) {
#else
		if (ioctl(pcm->fd, AUDIOIOC_STOP, 0) < 0) {
#endif
			return oops(pcm, errno, "cannot stop channel\n");
		}

		/* Remove any pending messages from the message queue */
		do {
			clock_gettime(CLOCK_REALTIME, &st_time);
			size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
		} while (size > 0);

		pcm->prepared = 0;
		pcm->running = 0;
		pcm->buf_idx = 0;
		/* We have stopped the codec, but the user can still call read API to
		   get the data that has already been recorded */
		pcm->draining = 1;

		return 0;
	}
}

/** Application request to access a portion of direct (mmap) area
 * @param[in] pcm A PCM handle
 * @param[out] areas Returned mmap channel areas
 * @param[out] offset Returned mmap area offset in frames
 * @param[out] frames Mmap area portion size in frames
 * @returns On success, zero is returned. On failure, a negative number returned
 * @ingroup libtinyalsa-pcm
 */
int pcm_mmap_begin(struct pcm *pcm, void **areas, unsigned int *offset, unsigned int *frames)
{
	if (pcm == NULL || areas == NULL || offset == NULL || frames == NULL) {
		return -EINVAL;
	}

	if (!(pcm->flags & PCM_MMAP)) {
		return -EINVAL;
	}

	int nframes = 0;
	/* The next buffer to be used is pointed by mmap_idx */
	struct ap_buffer_s *apb = pcm->pBuffers[pcm->mmap_idx];

	/* If buffer is in enqueued state, return 0 */
	if (apb->flags & AUDIO_APB_MMAP_ENQUEUED) {
		*frames = 0;
		return 0;
	}

	*areas = apb->samp;

	if (pcm->flags & PCM_IN) {
		if (!pcm->running) {
			return -EINVAL;
		}

		*offset = pcm_bytes_to_frames(pcm, apb->curbyte);
		nframes = pcm_bytes_to_frames(pcm, apb->nbytes - apb->curbyte);
	} else {
		*offset = 0;
		nframes = pcm_bytes_to_frames(pcm, apb->nmaxbytes);
	}

	if (*frames > nframes) {
		*frames = nframes;
	}

	return 0;
}

/** Application indicates the completion of access to mmap area
 * @param[in] pcm A PCM handle
 * @param[in] offset Area offset in frames. This must be same as the offset returned by pcm_mmap_begin
 * @param[in] frames Mmap area portion size in frames that application wishes to commit
 * @returns On success, zero is returned. On failure, a negative number returned
 * @ingroup libtinyalsa-pcm
 */
int pcm_mmap_commit(struct pcm *pcm, unsigned int offset, unsigned int frames)
{
	if (pcm == NULL) {
		return -EINVAL;
	}

	if (!(pcm->flags & PCM_MMAP)) {
		return -EINVAL;
	}

	struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb = pcm->pBuffers[pcm->mmap_idx];

	if (pcm->flags & PCM_OUT) {
		/* In case of playback, we will always enqueue the buffer when
		application calls commit, even if the buffer is not completely filled */
		bufdesc.numbytes = pcm_frames_to_bytes(pcm, frames);
		apb->nbytes = bufdesc.numbytes;
		apb->curbyte = 0;
	} else {
		/* In record case, we will enqueue the buffer only after the application
		reads all data present in buffer */
		if (pcm_frames_to_bytes(pcm, offset + frames) < apb->nbytes) {
			/* Application has not completely read the buffer. So, we only
			update the offset in the buffer. The application will get the same buffer
			when it calls mmap_begin() function again */
			apb->curbyte = pcm_frames_to_bytes(pcm, offset + frames);
			return 0;
		} else {
			bufdesc.numbytes = pcm_frames_to_bytes(pcm, apb->nmaxbytes);
			apb->nbytes = 0;
			apb->curbyte = 0;
		}
	}

	if (pcm->draining) {
		return 0;
	}

	/* Enqueue the buffer and set the apb flag */
#ifdef CONFIG_AUDIO_MULTI_SESSION
	bufdesc.session = pcm->session;
#endif
	bufdesc.u.pBuffer = apb;
	apb->flags = 0;
	if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
		return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed\n");
	}
	apb->flags |= AUDIO_APB_MMAP_ENQUEUED;

	/* Move the mmap_idx to tbe next position so that when
	subsequent calls to mmap_begin will use the next buffer */
	pcm->mmap_idx++;
	if (pcm->mmap_idx == pcm->buffer_cnt) {
		pcm->mmap_idx = 0;
	}

	/* Update buf_idx. It will be used during pcm_drain */
	if (pcm->buf_idx < pcm->buffer_cnt) {
		pcm->buf_idx++;
	}

	return 0;
}

/** Returns the number of frames ready to be written or read
 * @param[in] pcm A PCM handle
 * @returns On success, positive number is returned. On failure, a negative number returned
 * @ingroup libtinyalsa-pcm
 */
int pcm_avail_update(struct pcm *pcm)
{
	if (pcm == NULL) {
		return -EINVAL;
	}

	if (!(pcm->flags & PCM_MMAP)) {
		return -EINVAL;
	}

	int count = 0;
	int i;

	/* We will count the number of bytes available in all the buffers which have not been enqueued */
	if (pcm->flags & PCM_OUT) {
		for (i = 0; i < pcm->buffer_cnt; i++) {
			count += pcm->pBuffers[i]->flags & AUDIO_APB_MMAP_ENQUEUED ? 0 : (pcm->pBuffers[i]->nmaxbytes);
		}
	} else {
		if (!pcm->running && !pcm->draining) {
			return 0;
		}
		for (i = 0; i < pcm->buffer_cnt; i++) {
			count += pcm->pBuffers[i]->flags & AUDIO_APB_MMAP_ENQUEUED ? 0 : (pcm->pBuffers[i]->nbytes);
		}
	}

	return pcm_bytes_to_frames(pcm, count);
}

/** Waits for frames to be available for read or write operations.
 * @param pcm A PCM handle.
 * @param timeout The maximum amount of time to wait for, in terms of milliseconds.
 * @returns If frames became available, one is returned.
 *  If a timeout occured, zero is returned.
 *  If an error occured, a negative number is returned.
 * @ingroup libtinyalsa-pcm
 */
	int pcm_wait(struct pcm *pcm, int timeout)
	{
		struct ap_buffer_s *apb;
		struct audio_msg_s msg;
		unsigned int size;
		int prio;
		struct timespec st_time;
		int count = 0;
	
		if (pcm == NULL) {
			return -EINVAL;
		}
	
		if (!(pcm->flags & PCM_MMAP)) {
			return -EINVAL;
		}
	
		/* If there are some messages in the queue, empty them */
		while (1) {
			clock_gettime(CLOCK_REALTIME, &st_time);
			size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
			if (size != sizeof(msg)) {
				break;
			}
			/* If you find a dequeue message in the queue,
			update the apb flags and curbyte and increment the count*/
			if (msg.msgId == AUDIO_MSG_DEQUEUE) {
				apb = (struct ap_buffer_s *)msg.u.pPtr;
				apb->flags &= ~AUDIO_APB_MMAP_ENQUEUED;
				apb->curbyte = 0;
				count++;
			} else if (msg.msgId == AUDIO_MSG_XRUN) {
				/* Underrun to be handled by client */
				return -EPIPE;
			}
		}
		/* If we got some buffer from the queue, return positive result */
		if (count > 0) {
			return 1;
		}
	
		/* If we are in draining state, and the queue is empty, reset the draining state
		We will return positive result for now. But next call to pcm_wait will return error */
		if (count == 0 && pcm->draining) {
			pcm->draining = 0;
			return 1;
		}
	
		/* If PCM is opened for recording, then start it */
		if ((pcm->flags & PCM_IN) && !pcm->running && !pcm->draining) {
			if (pcm_start(pcm) < 0) {
				return oops(pcm, -1, "Failed to start PCM\n");
			}
		}
	
		/* Check if some buffer is already available */
		/* This is put here to handle corner case of open for write -> wait() */
		if (pcm_avail_update(pcm) == pcm->buffer_size) {
			audvdbg("avail update %d buffer_size %d\n", pcm_avail_update(pcm), pcm->buffer_size);
			return 1;
		}
		int cnt = 0;
		while (cnt < pcm->buffer_cnt - 1) {
			/* If there were no buffers in the queue, wait for codec to put a buffer on the queue */
			if (timeout > 0) {
				/* Use the timeout given by application */
				clock_gettime(CLOCK_REALTIME, &st_time);
				st_time.tv_nsec += timeout * MILLI_TO_NANO;
				size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
			} else {
				/* Application did not give a timeout value
				The behavior here depends on whether mq was opened with O_NONBLOCK
				This might block until a msg becomes available on the queue */
				size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
			}
			if (size != sizeof(msg)) {
				if (errno == ETIMEDOUT) {
					oops(pcm, errno, "TIMEOUT while watiting for deque message from kernel\n");
					return 0;
				} else {
					return oops(pcm, errno, "Interrupted while waiting for deque message from kernel\n");
				}
			}
			if (msg.msgId == AUDIO_MSG_DEQUEUE) {
				/* If you find a dequeue message in the queue,
				update the apb flags and curbyte and increment the count*/
				apb = (struct ap_buffer_s *)msg.u.pPtr;
				apb->flags &= ~AUDIO_APB_MMAP_ENQUEUED;
				apb->curbyte = 0;
				cnt++;
			} else if (msg.msgId == AUDIO_MSG_XRUN) {
				/* Underrun to be handled by client */
				return -EPIPE;
			} else {
				break;
			}
		}
		if (cnt == pcm->buffer_cnt -1) {
			return 1;
		}
	
		return oops(pcm, EINTR, "Received unexpected msg (id = %d) while waiting for deque message from kernel\n", msg.msgId);
	}

int pcm_mmap_transfer(struct pcm *pcm, const void *buffer, unsigned int bytes)
{
	int err = 0, frames, avail;
	unsigned int offset = 0, count;

	/* No data to be transferred */
	if (bytes == 0) {
		return 0;
	}

	count = pcm_bytes_to_frames(pcm, bytes);
	while (count > 0) {
		/* get the available space for writing new frames */
		avail = pcm_avail_update(pcm);
		if (avail < 0) {
			return oops(pcm, ENOMEM, "cannot determine available mmap frames\n");
		}

		/* sleep until we have space to write new frames */
		if ((unsigned int)avail == 0) {
			err = pcm_wait(pcm, -1);
			if (err < 0) {
				return err;
			}
			continue;
		}

		frames = count;
		if (frames > avail) {
			frames = avail;
		}

		if (!frames) {
			break;
		}

		/* copy frames to/from buffer */
		frames = pcm_mmap_transfer_areas(pcm, (void *)buffer, offset, frames);
		if (frames < 0) {
			return oops(pcm, ENOMEM, "write error: avail 0x%x\n", avail);
		}

		offset += frames;
		count -= frames;

		/* Start the PCM, if required */
		if (pcm->flags & PCM_OUT && !pcm->running) {
			if (pcm_start(pcm) < 0) {
				return oops(pcm, errno, "start error\n");
			}
		}
	}

	return offset;
}

/** Write data to the codec using mmap audio buffer
 * @param[in] pcm A PCM handle
 * @param[in] data Pointer to the buffer containing data to be written
 * @param[in] count Size of the data in bytes
 * @returns On success, written number of frames returned. On failure, a negative number returned
 * @ingroup libtinyalsa-pcm
 */
int pcm_mmap_write(struct pcm *pcm, const void *data, unsigned int count)
{
	if (pcm == NULL || data == NULL) {
		return -EINVAL;
	}
	if ((~pcm->flags) & (PCM_OUT | PCM_MMAP)) {
		return -ENOSYS;
	}

	return pcm_mmap_transfer(pcm, (void *)data, count);
}

/** Read data from codec using mmap audio buffers
 * @param[in] pcm A PCM handle
 * @param[in] data Pointer to the buffer to read into
 * @param[in] count Size of the buffer in bytes
 * @returns On success, read number of frames returned. On failure, a negative number returned
 * @ingroup libtinyalsa-pcm
 */
int pcm_mmap_read(struct pcm *pcm, void *data, unsigned int count)
{
	if (pcm == NULL || data == NULL) {
		return -EINVAL;
	}
	if ((~pcm->flags) & (PCM_IN | PCM_MMAP)) {
		return -ENOSYS;
	}

	return pcm_mmap_transfer(pcm, data, count);
}
