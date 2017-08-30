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

#define MILLI_TO_NAMO	1000000

#ifndef CONFIG_AUDIO_NUM_BUFFERS
#define CONFIG_AUDIO_NUM_BUFFERS  2
#endif

#ifndef CONFIG_AUDIO_BUFFER_NUMBYTES
#define CONFIG_AUDIO_BUFFER_NUMBYTES  8192
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
	/** The number of underruns that have occured */
	int underruns;
	/** Size of the buffer */
	unsigned int buffer_size;
	/** The boundary for ring buffer pointers */
	unsigned int boundary;
	/** Description of the last error that occured */
	char error[PCM_ERROR_MAX];
	/** Configuration that was passed to @ref pcm_open */
	struct pcm_config config;
	unsigned int noirq_frames_per_msec;
	/** The delay of the PCM, in terms of frames */
	long pcm_delay;
	/** The subdevice corresponding to the PCM */
	unsigned int subdevice;

#ifdef CONFIG_AUDIO_MULTI_SESSION
	void *session;
#endif
	mqd_t mq;					/* Message queue for the playthread */
	char mqname[16];			/* Name of our message queue */
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	struct ap_buffer_info_s buf_info;
	struct ap_buffer_s **pBuffers;
#else
	struct ap_buffer_s *pBuffers[CONFIG_AUDIO_NUM_BUFFERS];
#endif
	unsigned int bufPtr;
	struct ap_buffer_s *nextBuf;
	unsigned int nextSize;
	unsigned int nextOffset;
};

static int oops(struct pcm *pcm, int e, const char *fmt, ...)
{
	va_list ap;
	int sz;

	va_start(ap, fmt);
	vsnprintf(pcm->error, PCM_ERROR_MAX, fmt, ap);
	va_end(ap);
	sz = strlen(pcm->error);

	if (errno) {
		snprintf(pcm->error + sz, PCM_ERROR_MAX - sz, ": %s", strerror(e));
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
	return pcm->buffer_size;
}

/** Gets the channel count of the PCM.
 * @param pcm A PCM handle.
 * @return The channel count of the PCM.
 * @ingroup libtinyalsa-pcm
 */
unsigned int pcm_get_channels(const struct pcm *pcm)
{
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
	return pcm->config.rate;
}

/** Gets the format of the PCM.
 * @param pcm A PCM handle.
 * @return The format of the PCM.
 * @ingroup libtinyalsa-pcm
 */
enum pcm_format pcm_get_format(const struct pcm *pcm)
{
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
int pcm_set_config(struct pcm *pcm, const struct pcm_config *config)
{
	struct audio_caps_desc_s cap_desc;
	int ret;

	if (pcm == NULL) {
		return -EFAULT;
	} else if (config == NULL) {
		config = &pcm->config;
		pcm->config.channels = 2;
		pcm->config.rate = 48000;
		pcm->config.format = PCM_FORMAT_S16_LE;
		pcm->config.start_threshold = 0;
		pcm->config.stop_threshold = 0;
		pcm->config.silence_threshold = 0;
	} else {
		pcm->config = *config;
	}

#ifdef CONFIG_AUDIO_MULTI_SESSION
	cap_desc.session = pcm->session;
#endif
	cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
	cap_desc.caps.ac_type = AUDIO_TYPE_OUTPUT;
	cap_desc.caps.ac_channels = config->channels;
	cap_desc.caps.ac_controls.hw[0] = config->rate;
	cap_desc.caps.ac_controls.b[2] = pcm_format_to_bits(config->format);

	ret = ioctl(pcm->fd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	if (ret < 0) {
		return oops(pcm, -errno, "AUDIOIOC_CONFIGURE ioctl failed");
	}

	return 0;
}

/** Gets the subdevice on which the pcm has been opened.
 * @param pcm A PCM handle.
 * @return The subdevice on which the pcm has been opened */
unsigned int pcm_get_subdevice(const struct pcm *pcm)
{
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
	return frames * pcm->config.channels * (pcm_format_to_bits(pcm->config.format) >> 3);
}

static int pcm_areas_copy(struct pcm *pcm, unsigned int pcm_offset, char *buf, unsigned int src_offset, unsigned int frames)
{
	int size_bytes = pcm_frames_to_bytes(pcm, frames);
	int pcm_offset_bytes = pcm_frames_to_bytes(pcm, pcm_offset);
	int src_offset_bytes = pcm_frames_to_bytes(pcm, src_offset);

	/* interleaved only atm */
	if (pcm->flags & PCM_IN) {
		memcpy(buf + src_offset_bytes, (char *)pcm->nextBuf + pcm_offset_bytes, size_bytes);
	} else {
		memcpy((char *)pcm->nextBuf + pcm_offset_bytes, buf + src_offset_bytes, size_bytes);
	}
	return 0;
}

static int pcm_mmap_transfer_areas(struct pcm *pcm, char *buf, unsigned int offset, unsigned int size)
{
	void *pcm_areas;
	int commit;
	unsigned int pcm_offset, frames, count = 0;

	while (size > 0) {
		frames = size;
		pcm_mmap_begin(pcm, &pcm_areas, &pcm_offset, &frames);
		pcm_areas_copy(pcm, pcm_offset, buf, offset, frames);
		commit = pcm_mmap_commit(pcm, pcm_offset, frames);
		if (commit < 0) {
			return oops(pcm, commit, "failed to commit %d frames\n", frames);
		}

		offset += commit;
		count += commit;
		size -= commit;
	}
	return count;
}

/** Returns available frames in pcm buffer and corresponding time stamp.
 * The clock is CLOCK_MONOTONIC if flag @ref PCM_MONOTONIC was specified in @ref pcm_open,
 * otherwise the clock is CLOCK_REALTIME.
 * For an input stream, frames available are frames ready for the application to read.
 * For an output stream, frames available are the number of empty frames available for the application to write.
 * Only available for PCMs opened with the @ref PCM_MMAP flag.
 * @param pcm A PCM handle.
 * @param avail The number of available frames
 * @param tstamp The timestamp
 * @return On success, zero is returned; on failure, negative one.
 */
int pcm_get_htimestamp(struct pcm *pcm, unsigned int *avail, struct timespec *tstamp)
{
	return -1;
}

/** Writes audio samples to PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_OUT flag.
 * This function is not valid for PCMs opened with the @ref PCM_MMAP flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param frame_count The number of frames occupied by the sample array.
 *  This value should not be greater than @ref TINYALSA_FRAMES_MAX
 *  or INT_MAX.
 * @return On success, this function returns the number of frames written; otherwise, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_writei(struct pcm *pcm, const void *data, unsigned int frame_count)
{
	int nbytes;
	struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb;
	struct audio_msg_s msg;
	unsigned int size;
	int prio;

	if (pcm->flags & PCM_IN) {
		return -EINVAL;
	}

	nbytes = pcm_frames_to_bytes(pcm, frame_count);

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if (nbytes > pcm->config.period_size) {
		nbytes = pcm->config.period_size;
	}

	if (pcm->bufPtr < pcm->config.period_count)
#else
	if (nbytes > CONFIG_AUDIO_BUFFER_NUMBYTES) {
		nbytes = CONFIG_AUDIO_BUFFER_NUMBYTES;
	}

	if (pcm->bufPtr < CONFIG_AUDIO_NUM_BUFFERS)
#endif
	{
		/* If we have empty buffers, fill them first */
		memcpy(pcm->pBuffers[pcm->bufPtr]->samp, data, nbytes);
		apb = pcm->pBuffers[pcm->bufPtr];
		pcm->bufPtr++;
	} else {
		/* We dont have any empty buffers. wait for deque message from kernel */
		size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
		if (size != sizeof(msg)) {
			/* Interrupted by a signal? What to do? */
			return oops(pcm, EINTR, "Interrupted while waiting for deque message from kernel");
		}
		if (msg.msgId == AUDIO_MSG_DEQUEUE) {
			apb = (struct ap_buffer_s *)msg.u.pPtr;
			memcpy(apb->samp, data, nbytes);
		} else {
			return oops(pcm, EINTR, "Recieved unexpected msg (id = %d) while waiting for deque message from kernel", msg.msgId);
		}
	}

	/* Buffer is ready. Enque it */
#ifdef CONFIG_AUDIO_MULTI_SESSION
	bufdesc.session = pcm->session;
#endif
	bufdesc.numbytes = apb->nbytes;
	bufdesc.u.pBuffer = apb;
	if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
		return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed");
	}
	/* If playback is not already started, start now! */
	if ((!pcm->running) && (pcm_start(pcm) < 0)) {
		return -errno;
	} else {
		pcm->running = 1;
	}

	return pcm_bytes_to_frames(pcm, nbytes);
}

/** Reads audio samples from PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_IN flag.
 * This function is not valid for PCMs opened with the @ref PCM_MMAP flag.
 * @param pcm A PCM handle.
 * @param data The audio sample array
 * @param frame_count The number of frames occupied by the sample array.
 *  This value should not be greater than @ref TINYALSA_FRAMES_MAX
 *  or INT_MAX.
 * @return On success, this function returns the number of frames written; otherwise, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_readi(struct pcm *pcm, void *data, unsigned int frame_count)
{
	int nbytes;
	struct audio_buf_desc_s bufdesc;
	struct ap_buffer_s *apb;
	struct audio_msg_s msg;
	unsigned int size;
	int prio;

	if (!(pcm->flags & PCM_IN)) {
		return -EINVAL;
	}

	nbytes = pcm_frames_to_bytes(pcm, frame_count);

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if (nbytes < pcm->config.period_size) {
		return -EINVAL;
	}
	bufdesc.numbytes = pcm->config.period_size;
#else
	if (nbytes < CONFIG_AUDIO_BUFFER_NUMBYTES) {
		return -EINVAL;
	}
	bufdesc.numbytes = CONFIG_AUDIO_BUFFER_NUMBYTES;
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
	bufdesc.session = pcm->session;
#endif

	/* If device is not yet started, start now! */
	if ((!pcm->running) && (pcm_start(pcm) < 0)) {
		return -errno;
	} else {
		pcm->running = 1;
	}

	/* Wait for deque message from kernel */
	size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
	if (size != sizeof(msg)) {
		/* Interrupted by a signal? What to do? */
		return oops(pcm, EINTR, "Interrupted while waiting for deque message from kernel");
	}
	if (msg.msgId == AUDIO_MSG_DEQUEUE) {
		apb = (struct ap_buffer_s *)msg.u.pPtr;
		/* Copy data to user buffer */
		memcpy(data, apb->samp, apb->nbytes);
		/* Enque buffer for next read opertion */
		bufdesc.u.pBuffer = apb;
		if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
			return oops(pcm, errno, "failed to enque buffer after read");
		}
	} else {
		return oops(pcm, EINTR, "Recieved unexpected msg (id = %d) while waiting for deque message from kernel", msg.msgId);
	}

	return pcm_bytes_to_frames(pcm, apb->nbytes);
}

/** Writes audio samples to PCM.
 * If the PCM has not been started, it is started in this function.
 * This function is only valid for PCMs opened with the @ref PCM_OUT flag.
 * This function is not valid for PCMs opened with the @ref PCM_MMAP flag.
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
 * This function is not valid for PCMs opened with the @ref PCM_MMAP flag.
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

static struct pcm bad_pcm = {
	.fd = -1,
};

/** Gets the hardware parameters of a PCM, without created a PCM handle.
 * @param card The card of the PCM.
 *  The default card is zero.
 * @param device The device of the PCM.
 *  The default device is zero.
 * @param flags Specifies whether the PCM is an input or output.
 *  May be one of the following:
 *   - @ref PCM_IN
 *   - @ref PCM_OUT
 * @return On success, the hardware parameters of the PCM; on failure, NULL.
 * @ingroup libtinyalsa-pcm
 */

#if 0
struct pcm_params *pcm_params_get(unsigned int card, unsigned int device, unsigned int flags)
{
	return NULL;
}

/** Frees the hardware parameters returned by @ref pcm_params_get.
 * @param pcm_params Hardware parameters of a PCM.
 *  May be NULL.
 * @ingroup libtinyalsa-pcm
 */
void pcm_params_free(struct pcm_params *pcm_params)
{
}

static int pcm_param_to_alsa(enum pcm_param param)
{
	return -1;
}

/** Gets a mask from a PCM's hardware parameters.
 * @param pcm_params A PCM's hardware parameters.
 * @param param The parameter to get.
 * @return If @p pcm_params is NULL or @p param is not a mask, NULL is returned.
 *  Otherwise, the mask associated with @p param is returned.
 * @ingroup libtinyalsa-pcm
 */
const struct pcm_mask *pcm_params_get_mask(const struct pcm_params *pcm_params, enum pcm_param param)
{
	return NULL;
}

/** Get the minimum of a specified PCM parameter.
 * @param pcm_params A PCM parameters structure.
 * @param param The specified parameter to get the minimum of.
 * @returns On success, the parameter minimum.
 *  On failure, zero.
 */
unsigned int pcm_params_get_min(const struct pcm_params *pcm_params, enum pcm_param param)
{
	return 0;
}

/** Get the maximum of a specified PCM parameter.
 * @param pcm_params A PCM parameters structure.
 * @param param The specified parameter to get the maximum of.
 * @returns On success, the parameter maximum.
 *  On failure, zero.
 */
unsigned int pcm_params_get_max(const struct pcm_params *pcm_params, enum pcm_param param)
{
	return 0;
}
#endif

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
		return oops(pcm, EINVAL, "pcm is null");
	}

	if (pcm == &bad_pcm) {
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
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if (pBuffers != NULL) {
		for (x = 0; x < pcm->config.period_count; x++) {
			if (pcm->pBuffers[x] != NULL) {
				buf_desc.u.pBuffer = pcm->pBuffers[x];
				ioctl(pPlayer->devFd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
			}
		}
		free(pBuffers);
	}
#else
	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++) {
		if (pcm->pBuffers[x] != NULL) {
			buf_desc.u.pBuffer = pcm->pBuffers[x];
			ioctl(pcm->fd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
		}
	}
#endif
	mq_close(pcm->mq);			/* Close the message queue */
	mq_unlink(pcm->mqname);

	if (pcm->fd >= 0) {
		close(pcm->fd);
	}
	pcm->prepared = 0;
	pcm->running = 0;
	pcm->buffer_size = 0;
	pcm->fd = -1;
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
 *   - @ref PCM_NOIRQ
 *   - @ref PCM_MONOTONIC
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
 *   - @ref PCM_NOIRQ
 *   - @ref PCM_MONOTONIC
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
	memset(pcm, 0, sizeof(struct pcm));

	if (!pcm) {
		return &bad_pcm;
	}

	snprintf(fn, sizeof(fn), "/dev/audio/pcmC%uD%u%c", card, device, flags & PCM_IN ? 'c' : 'p');

	pcm->flags = flags;
	pcm->fd = open(fn, O_RDWR);
	if (pcm->fd < 0) {
		oops(pcm, errno, "cannot open device '%s'", fn);
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
		oops(pcm, errno, "Failed to reserve device");
		ret = -errno;
		goto fail_close;
	}

	if (pcm_set_config(pcm, config) != 0) {
		goto fail_close;
	}

	/* Create a message queue for the playthread */
	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	snprintf(pcm->mqname, sizeof(pcm->mqname), "/tmp/%0lx", (unsigned long)((uintptr_t) pcm));

	pcm->mq = mq_open(pcm->mqname, O_RDWR | O_CREAT, 0644, &attr);
	if (pcm->mq == NULL) {
		/* Unable to open message queue! */
		ret = -errno;
		oops(pcm, errno, "mq_open failed");
		goto fail_close;
	}

	/* Register our message queue with the audio device */
	ioctl(pcm->fd, AUDIOIOC_REGISTERMQ, (unsigned long)pcm->mq);

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if ((ret = ioctl(pcm->fd, AUDIOIOC_GETBUFFERINFO, (unsigned long)&buf_info)) != OK) {
		/* Driver doesn't report it's buffer size.  Use our default. */
		buf_info.buffer_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
		buf_info.nbuffers = CONFIG_AUDIO_NUM_BUFFERS;
	}

	pcm->config.period_size = buf_info.buffer_size;
	pcm->config.period_count = buf_info.nbuffers;
	pcm->buffer_size = pcm->config.period_size;

	/* Create array of pointers to buffers */
	pcm->pBuffers = (FAR struct ap_buffer_s **)malloc(buf_info.nbuffers * sizeof(FAR void *));
	if (pcm->pBuffers == NULL) {
		/* Error allocating memory for buffer storage! */
		ret = -ENOMEM;
		goto fail_after_mq;
	}

	/* Create our audio pipeline buffers to use for queueing up data */

	for (x = 0; x < buf_info.nbuffers; x++) {
		pcm->pBuffers[x] = NULL;
	}

	for (x = 0; x < buf_info.nbuffers; x++)
#else							/* CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFER */

	pcm->config.period_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
	pcm->config.period_count = CONFIG_AUDIO_NUM_BUFFERS;
	pcm->buffer_size = pcm->config.period_size;

	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++) {
		pcm->pBuffers[x] = NULL;
	}

	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++)
#endif							/* CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFER */
	{
		/* Fill in the buffer descriptor struct to issue an alloc request */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		buf_desc.session = pcm->session;
#endif
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
		buf_desc.numbytes = buf_info.buffer_size;
#else
		buf_desc.numbytes = CONFIG_AUDIO_BUFFER_NUMBYTES;
#endif
		buf_desc.u.ppBuffer = &pcm->pBuffers[x];

		ret = ioctl(pcm->fd, AUDIOIOC_ALLOCBUFFER, (unsigned long)&buf_desc);

		if (ret != sizeof(buf_desc)) {
			/* Buffer alloc Operation not supported or error allocating! */
			oops(pcm, -1, "Could not allocate buffer %d\n", x);
			goto fail_cleanup_buffers;
		}
	}

	pcm->underruns = 0;
	return pcm;

fail_cleanup_buffers:
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if (pcm->pBuffers != NULL) {
		for (x = 0; x < buf_info.nbuffers; x++) {
			/* Fill in the buffer descriptor struct to issue a free request */
			if (pcm->pBuffers[x] != NULL) {
				buf_desc.u.pBuffer = pBuffers[x];
				ioctl(pPlayer->devFd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
			}
		}
		/* Free the pointers to the buffers */
		free(pcm->pBuffers);
	}
#else
	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++) {
		/* Fill in the buffer descriptor struct to issue a free request */
		if (pcm->pBuffers[x] != NULL) {
			buf_desc.u.pBuffer = pcm->pBuffers[x];
			ioctl(pcm->fd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
		}
	}
#endif

	pcm->bufPtr = 0;

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
fail_after_mq:
	mq_close(pcm->mq);			/* Close the message queue */
	mq_unlink(pcm->mqname);		/* Unlink the message queue */
#endif
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
	if (pcm->prepared) {
		return 0;
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

	int prepare_error = pcm_prepare(pcm);
	if (prepare_error) {
		return prepare_error;
	}

	if (pcm->flags & PCM_IN) {
		/* If the device is opened for read and Our buffers are not enqued. Enque them now. */
#ifdef CONFIG_AUDIO_MULTI_SESSION
		bufdesc.session = pcm->session;
#endif
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
		bufdesc.numbytes = pcm->config.period_size;
		for (pcm->bufPtr = 0; pcm->bufPtr < buf_info.nbuffers; pcm->bufPtr++)
#else
		bufdesc.numbytes = CONFIG_AUDIO_BUFFER_NUMBYTES;
		for (pcm->bufPtr = 0; pcm->bufPtr < CONFIG_AUDIO_NUM_BUFFERS; pcm->bufPtr++)
#endif
		{
			bufdesc.u.pBuffer = pcm->pBuffers[pcm->bufPtr];
			if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
				return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed");
			}
		}
	}
#ifdef CONFIG_AUDIO_MULTI_SESSION
	if (ioctl(pcm->fd, AUDIOIOC_START, (unsigned long)pcm->session) < 0)
#else
	if (ioctl(pcm->fd, AUDIOIOC_START, 0) < 0)
#endif
	{
		return oops(pcm, errno, "cannot start channel");
	}

	pcm->nextSize = 0;
	pcm->nextOffset = 0;
	pcm->nextBuf = NULL;
	pcm->running = 1;
	return 0;
}

/** Stops a PCM.
 * @param pcm A PCM handle.
 * @return On success, zero; on failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
int pcm_stop(struct pcm *pcm)
{
#ifdef CONFIG_AUDIO_MULTI_SESSION
	if (ioctl(pcm->fd, AUDIOIOC_STOP, (unsigned long)pcm->session) < 0)
#else
	if (ioctl(pcm->fd, AUDIOIOC_STOP, 0) < 0)
#endif
		return oops(pcm, errno, "cannot stop channel");

	pcm->prepared = 0;
	pcm->running = 0;
	pcm->bufPtr = 0;
	pcm->nextSize = 0;
	pcm->nextOffset = 0;
	pcm->nextBuf = NULL;

	return 0;
}

static inline int pcm_mmap_avail(struct pcm *pcm)
{
	return pcm_bytes_to_frames(pcm, pcm->nextSize);
}

int pcm_mmap_begin(struct pcm *pcm, void **areas, unsigned int *offset, unsigned int *frames)
{
	int nframes = pcm_bytes_to_frames(pcm, pcm->nextSize);

	/* If data is not available, return -1 */
	if (nframes == 0) {
		*frames = 0;
		return -1;
	}

	*areas = pcm->nextBuf->samp;
	*offset = pcm->nextOffset;

	if (*frames < nframes) {
		pcm->nextSize -= pcm_frames_to_bytes(pcm, *frames);
		pcm->nextOffset += pcm_frames_to_bytes(pcm, *frames);
	} else {
		*frames = pcm_bytes_to_frames(pcm, pcm->nextSize);
		pcm->nextSize = 0;
		pcm->nextOffset = 0;
	}

	return 0;
}

int pcm_mmap_commit(struct pcm *pcm, unsigned int offset, unsigned int frames)
{
	/* not used */
	(void)offset;

	struct audio_buf_desc_s bufdesc;

	if (pcm->nextSize == 0) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		bufdesc.session = pcm->session;
#endif
		bufdesc.numbytes = pcm->nextBuf->nmaxbytes;
		bufdesc.u.pBuffer = pcm->nextBuf;
		if (ioctl(pcm->fd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc) < 0) {
			return oops(pcm, errno, "AUDIOIOC_ENQUEUEBUFFER ioctl failed");
		}
		pcm->nextBuf = NULL;
	}

	return frames;
}

int pcm_avail_update(struct pcm *pcm)
{
	return pcm_mmap_avail(pcm);
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

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if ((pcm->flags & PCM_OUT) && pcm->bufPtr < pcm->config.period_count)
#else
	if ((pcm->flags & PCM_OUT) && pcm->bufPtr < CONFIG_AUDIO_NUM_BUFFERS)
#endif
	{
		/* In playback scenario, if audio buffers are available, return immediately */
		pcm->nextBuf = pcm->pBuffers[pcm->bufPtr];
		pcm->nextSize = pcm->pBuffers[pcm->bufPtr]->nmaxbytes;
		pcm->bufPtr++;
		return 1;
	} else {
		/* We dont have any empty buffers. wait for deque message from kernel */
		if (timeout > 0) {
			clock_gettime(CLOCK_REALTIME, &st_time);
			st_time.tv_nsec += timeout * MILLI_TO_NANO;
			size = mq_timedreceive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio, &st_time);
		} else {
			size = mq_receive(pcm->mq, (FAR char *)&msg, sizeof(msg), &prio);
		}

		if (size != sizeof(msg)) {
			if (errno == ETIMEDOUT) {
				oops(pcm, errno, "TIMEOUT while watiting for deque message from kernel");
				return 0;
			} else {
				return oops(pcm, errno, "Interrupted while waiting for deque message from kernel");
			}
		}
		if (msg.msgId == AUDIO_MSG_DEQUEUE) {
			apb = (struct ap_buffer_s *)msg.u.pPtr;
			pcm->nextBuf = apb;
			pcm->nextSize = apb->nbytes;
			return 1;
		} else {
			return oops(pcm, EINTR, "Recieved unexpected msg (id = %d) while waiting for deque message from kernel", msg.msgId);
		}
	}
}

int pcm_mmap_transfer(struct pcm *pcm, const void *buffer, unsigned int bytes)
{
	int err = 0, frames, avail;
	unsigned int offset = 0, count;

	if (bytes == 0) {
		return 0;
	}

	count = pcm_bytes_to_frames(pcm, bytes);

	while (count > 0) {

		/* get the available space for writing new frames */
		avail = pcm_avail_update(pcm);
		if (avail < 0) {
			return oops(pcm, 1, "cannot determine available mmap frames");
		}

		/* start the audio if we reach the threshold */
		if (!pcm->running) {
			if (pcm_start(pcm) < 0) {
				return oops(pcm, errno, "start error: avail 0x%x\n", avail);
			}
		}

		/* sleep until we have space to write new frames */
		if (pcm->running && (unsigned int)avail == 0) {
			int time = -1;

			/* We will not support NOIRQ flag presently */
#if 0
			if (pcm->flags & PCM_NOIRQ)
				time = (pcm->buffer_size - avail - pcm->mmap_control->avail_min)
					   / pcm->noirq_frames_per_msec;
#endif
			err = pcm_wait(pcm, time);
			if (err < 0) {
				pcm->prepared = 0;
				pcm->running = 0;
				return oops(pcm, errno, "wait error: avail 0x%x\n", avail);
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

		/* copy frames from buffer */
		frames = pcm_mmap_transfer_areas(pcm, (void *)buffer, offset, frames);
		if (frames < 0) {
			return oops(pcm, 1, "write error: avail 0x%x\n", avail);
		}

		offset += frames;
		count -= frames;
	}

	return 0;
}

int pcm_mmap_write(struct pcm *pcm, const void *data, unsigned int count)
{
	if ((~pcm->flags) & (PCM_OUT | PCM_MMAP)) {
		return -ENOSYS;
	}

	return pcm_mmap_transfer(pcm, (void *)data, count);
}

int pcm_mmap_read(struct pcm *pcm, void *data, unsigned int count)
{
	if ((~pcm->flags) & (PCM_IN | PCM_MMAP)) {
		return -ENOSYS;
	}

	return pcm_mmap_transfer(pcm, data, count);
}

#if 0
/** Gets the delay of the PCM, in terms of frames.
 * @param pcm A PCM handle.
 * @returns On success, the delay of the PCM.
 *  On failure, a negative number.
 * @ingroup libtinyalsa-pcm
 */
long pcm_get_delay(struct pcm *pcm)
{
	return -1;
}
#endif
