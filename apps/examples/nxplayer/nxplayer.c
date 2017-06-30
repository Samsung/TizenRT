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
 * apps/examples/nxplayer/nxplayer.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <debug.h>

#include <tinyara/audio/audio.h>
#include <apps/nxplayer.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define NXPLAYER_STATE_IDLE      0
#define NXPLAYER_STATE_PLAYING   1
#define NXPLAYER_STATE_PAUSED    2

#ifndef CONFIG_AUDIO_NUM_BUFFERS
#define CONFIG_AUDIO_NUM_BUFFERS  2
#endif

#ifndef CONFIG_AUDIO_BUFFER_NUMBYTES
#define CONFIG_AUDIO_BUFFER_NUMBYTES  8192
#endif

#ifndef CONFIG_EXAMPLES_NXPLAYER_MSG_PRIO
#define CONFIG_EXAMPLES_NXPLAYER_MSG_PRIO  1
#endif

#ifndef CONFIG_EXAMPLES_NXPLAYER_PLAYTHREAD_STACKSIZE
#define CONFIG_EXAMPLES_NXPLAYER_PLAYTHREAD_STACKSIZE    1500
#endif

/****************************************************************************
 * Private Type Declarations
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT
struct nxplayer_ext_fmt_s {
	const char *ext;
	uint16_t format;
	CODE int (*getsubformat)(FAR FILE *fd);
};
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef CONFIG_AUDIO_FORMAT_MIDI
int nxplayer_getmidisubformat(FAR FILE *fd);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT
static const struct nxplayer_ext_fmt_s g_known_ext[] = {
#ifdef CONFIG_AUDIO_FORMAT_AC3
	{"ac3", AUDIO_FMT_AC3, NULL},
#endif
#ifdef CONFIG_AUDIO_FORMAT_MP3
	{"mp3", AUDIO_FMT_MP3, NULL},
#endif
#ifdef CONFIG_AUDIO_FORMAT_DTS
	{"dts", AUDIO_FMT_DTS, NULL},
#endif
#ifdef CONFIG_AUDIO_FORMAT_WMA
	{"wma", AUDIO_FMT_WMA, NULL},
#endif
#ifdef CONFIG_AUDIO_FORMAT_PCM
	{"wav", AUDIO_FMT_PCM, NULL},
#endif
#ifdef CONFIG_AUDIO_FORMAT_MIDI
	{"mid", AUDIO_FMT_MIDI, nxplayer_getmidisubformat},
	{"midi", AUDIO_FMT_MIDI, nxplayer_getmidisubformat},
#endif
#ifdef CONFIG_AUDIO_FORMAT_OGG_VORBIS
	{"ogg", AUDIO_FMT_OGG_VORBIS, NULL}
#endif
};

static const int g_known_ext_count = sizeof(g_known_ext) / sizeof(struct nxplayer_ext_fmt_s);
#endif							/* CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nxplayer_opendevice
 *
 *   nxplayer_opendevice() either searches the Audio system for a device
 *   that is compatible with the specified audio format and opens it, or
 *   tries to open the prefered device if specified and validates that
 *   it supports the requested format.
 *
 * Return:
 *    OK        if compatible device opened (searched or preferred)
 *    -ENODEV   if no compatible device opened.
 *    -ENOENT   if preferred device couldn't be opened.
 *
 ****************************************************************************/

static int nxplayer_opendevice(FAR struct nxplayer_s *pPlayer, int format, int subfmt)
{
	/* If we have a preferred device, then open it */

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_PREFERRED_DEVICE
	if (pPlayer->prefdevice[0] != '\0') {
		/* Use the saved prefformat to test if the requested
		 * format is specified by the device
		 */

		if ((pPlayer->prefformat & (1 << (format - 1))) == 0 || (pPlayer->preftype & AUDIO_TYPE_OUTPUT) == 0) {
			/* Format not supported by the device */

			auddbg("ERROR: Format not supported by device: %d\n", format);
			return -ENODEV;
		}

		/* Device supports the format.  Open the device file. */

		pPlayer->devFd = open(pPlayer->prefdevice, O_RDWR);
		if (pPlayer->devFd == -1) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			auddbg("ERROR: Failed to open %s: %d\n", -errcode);
			UNUSED(errcode);
			return -ENOENT;
		}

		return OK;
	}
#endif

#if defined(CONFIG_EXAMPLES_NXPLAYER_INCLUDE_PREFERRED_DEVICE) && \
	defined(CONFIG_EXAMPLES_NXPLAYER_INCLUDE_DEVICE_SEARCH)

	else
#endif

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_DEVICE_SEARCH
	{
		struct audio_caps_s caps;
		FAR struct dirent *pDevice;
		FAR DIR *dirp;
		char path[64];
		uint8_t supported = true;
		uint8_t x;

		/* Search for a device in the audio device directory */

#ifdef CONFIG_AUDIO_CUSTOM_DEV_PATH
#ifdef CONFIG_AUDIO_DEV_ROOT
		dirp = opendir("/dev");
#else
		dirp = opendir(CONFIG_AUDIO_DEV_PATH);
#endif							/* CONFIG_AUDIO_DEV_ROOT */
#else
		dirp = opendir("/dev/audio");
#endif							/* CONFIG_AUDIO_CUSTOM_DEV_PATH */
		if (dirp == NULL) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			auddbg("ERROR: Failed to open /dev/audio: %d\n", -errcode);
			UNUSED(errcode);
			return -ENODEV;
		}

		while ((pDevice = readdir(dirp)) != NULL) {
			/* We found the next device.  Try to open it and
			 * get its audio capabilities.
			 */

#ifdef CONFIG_AUDIO_CUSTOM_DEV_PATH
#ifdef CONFIG_AUDIO_DEV_ROOT
			snprintf(path, sizeof(path), "/dev/%s", pDevice->d_name);
#else
			snprintf(path, sizeof(path), CONFIG_AUDIO_DEV_PATH "/%s", pDevice->d_name);
#endif							/* CONFIG_AUDIO_DEV_ROOT */
#else
			snprintf(path, sizeof(path), "/dev/audio/%s", pDevice->d_name);
#endif							/* CONFIG_AUDIO_CUSTOM_DEV_PATH */

			if ((pPlayer->devFd = open(path, O_RDWR)) != -1) {
				/* We have the device file open.  Now issue an AUDIO ioctls to
				 * get the capabilities
				 */

				caps.ac_len = sizeof(caps);
				caps.ac_type = AUDIO_TYPE_QUERY;
				caps.ac_subtype = AUDIO_TYPE_QUERY;

				if (ioctl(pPlayer->devFd, AUDIOIOC_GETCAPS, (unsigned long)&caps)
					== caps.ac_len) {
					/* Test if this device supports the format we want */

					if (((caps.ac_format.hw & (1 << (format - 1))) != 0) && (caps.ac_controls.b[0] & AUDIO_TYPE_OUTPUT)) {
						/* Do subformat detection */

						if (subfmt != AUDIO_FMT_UNDEF) {
							/* Prepare to get sub-formats for this main format */

							caps.ac_subtype = format;
							caps.ac_format.b[0] = 0;

							while (ioctl(pPlayer->devFd, AUDIOIOC_GETCAPS, (unsigned long)&caps) == caps.ac_len) {
								/* Check the next set of 4 controls to find the subformat */

								for (x = 0; x < sizeof(caps.ac_controls); x++) {
									if (caps.ac_controls.b[x] == subfmt) {
										/* Sub format supported! */

										break;
									} else if (caps.ac_controls.b[x] == AUDIO_SUBFMT_END) {
										/* Sub format not supported */

										supported = false;
										break;
									}
								}

								/* If we reached the end of the subformat list, then
								 * break out of the loop.
								 */

								if (x != sizeof(caps.ac_controls)) {
									break;
								}

								/* Increment ac_format.b[0] to get next set of subformats */

								caps.ac_format.b[0]++;
							}
						}

						/* Test if subformat needed and detected */

						if (supported) {
							/* Yes, it supports this format.  Use this device */

							closedir(dirp);
							return OK;
						}
					}
				}

				/* Not this device! */

				close(pPlayer->devFd);
			}
		}

		/* Close the directory */

		closedir(dirp);
	}
#endif							/* CONFIG_EXAMPLES_NXPLAYER_INCLUDE_DEVICE_SEARCH */

	/* Device not found */

	auddbg("ERROR: Device not found\n");
	pPlayer->devFd = -1;
	return -ENODEV;
}

/****************************************************************************
 * Name: nxplayer_getmidisubformat
 *
 *   nxplayer_getmidisubformat() reads the MIDI header and determins the
 *   MIDI format of the file.
 *
 ****************************************************************************/

#ifdef CONFIG_AUDIO_FORMAT_MIDI
int nxplayer_getmidisubformat(FAR FILE *fd)
{
	char type[2];
	int ret;

	/* Seek to location 8 in the file (the format type) */

	fseek(fd, 8, SEEK_SET);
	fread(type, 1, 2, fd);

	/* Set return value based on type */

	switch (type[1]) {
	case 0:
		ret = AUDIO_SUBFMT_MIDI_0;
		break;

	case 1:
		ret = AUDIO_SUBFMT_MIDI_1;
		break;

	case 2:
		ret = AUDIO_SUBFMT_MIDI_2;
		break;
	}
	fseek(fd, 0, SEEK_SET);

	return ret;
}
#endif

/****************************************************************************
 * Name: nxplayer_fmtfromextension
 *
 *   nxplayer_fmtfromextension() tries to determine the file format based
 *   on the extension of the supplied filename.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT
static inline int nxplayer_fmtfromextension(FAR struct nxplayer_s *pPlayer,
                                            FAR const char *pFilename,
                                            FAR int *subfmt)
{
	const char *pExt;
	uint8_t x;
	uint8_t c;

	/* Find the file extension, if any */

	x = strlen(pFilename) - 1;
	while (x > 0) {
		/* Seach backward for the first '.' */

		if (pFilename[x] == '.') {
			/* First '.' found.  Now compare with known extensions */

			pExt = &pFilename[x + 1];
			for (c = 0; c < g_known_ext_count; c++) {
				/* Test for extension match */

				if (strcasecmp(pExt, g_known_ext[c].ext) == 0) {
					/* Test if we have a sub-format detection routine */

					if (subfmt && g_known_ext[c].getsubformat) {
						*subfmt = g_known_ext[c].getsubformat(pPlayer->fileFd);
					}

					/* Return the format for this extension */

					return g_known_ext[c].format;
				}
			}
		}

		/* Stop if we find a '/' */

		if (pFilename[x] == '/') {
			break;
		}

		x--;
	}

	return AUDIO_FMT_UNDEF;
}
#endif							/* CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT */

/****************************************************************************
 * Name: nxplayer_fmtfromheader
 *
 *   nxplayer_fmtfromheader() tries to determine the file format by checking
 *   the file header for known file types.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_HEADER
static int nxplayer_fmtfromheader(FAR struct nxplayer_s *pPlayer)
{
	return AUDIO_FMT_UNDEF;
}
#endif							/* CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_HEADER */

/****************************************************************************
 * Name: nxplayer_mediasearch
 *
 *   nxplayer_mediasearch() searches the subdirectories in the mediadir
 *   for the specified media file.  We borrow the caller's path stack
 *   variable (playfile) to conserve stack space.
 *
 ****************************************************************************/

#if defined(CONFIG_EXAMPLES_NXPLAYER_MEDIA_SEARCH) && defined(CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR)
static int nxplayer_mediasearch(FAR struct nxplayer_s *pPlayer,
                                FAR const char *pFilename,
                                FAR const char *path, int pathmax)
{
	return -ENOENT;
}
#endif

/****************************************************************************
 * Name: nxplayer_readbuffer
 *
 *  Read the next block of data from the media file into the specified
 *  buffer.
 *
 ****************************************************************************/

static int nxplayer_readbuffer(FAR struct nxplayer_s *pPlayer,
                               FAR struct ap_buffer_s* apb)
{
	/* Validate the file is still open.  It will be closed automatically when
	 * we encounter the end of file (or, perhaps, a read error that we cannot
	 * handle.
	 */

	if (pPlayer->fileFd == NULL) {
		/* Return -ENODATA to indicate that there is nothing more to read from
		 * the file.
		 */

		return -ENODATA;
	}

	/* Read data into the buffer. */

	apb->nbytes = fread(&apb->samp, 1, apb->nmaxbytes, pPlayer->fileFd);
	apb->curbyte = 0;
	apb->flags = 0;

	if (apb->nbytes < apb->nmaxbytes) {
#ifdef CONFIG_DEBUG
		int errcode = errno;
		int readerror = ferror(pPlayer->fileFd);

		audvdbg("Closing audio file, nbytes=%d readerr=%d\n", apb->nbytes, readerror);
#endif

		/* End of file or read error.. We are finished with this file in any
		 * event.
		 */

		fclose(pPlayer->fileFd);
		pPlayer->fileFd = NULL;

		/* Set a flag to indicate that this is the final buffer in the stream */

		apb->flags |= AUDIO_APB_FINAL;

#ifdef CONFIG_DEBUG
		/* Was this a file read error */

		if (apb->nbytes == 0 && readerror) {
			DEBUGASSERT(errcode > 0);
			auddbg("ERROR: fread failed: %d\n", errcode);
		}
#endif
	}

	/* Return OK to indicate that the buffer should be passed through to the
	 * audio device.  This does not necessarily indicate that data was read
	 * correctly.
	 */

	return OK;
}

/****************************************************************************
 * Name: nxplayer_enqueuebuffer
 *
 * Description:
 *   Enqueue the audio buffer in the downstream device.  Normally we are
 *   called with a buffer of data to be enqueued in the audio stream.
 *
 *   Be we may also receive an empty length buffer (with only the
 *   AUDIO_APB_FINAL set) in the event of certin read error occurs or in the
 *   event that the file was an exact multiple of the nmaxbytes size of the
 *   audio buffer.  In that latter case, we have an end of file with no bytes
 *   read.
 *
 *   These infrequent zero length buffers have to be passed through because
 *   the include the AUDIO_APB_FINAL flag that is needed to terminate the
 *   audio stream.
 *
 ****************************************************************************/

static int nxplayer_enqueuebuffer(FAR struct nxplayer_s *pPlayer,
                                  FAR struct ap_buffer_s* apb)
{
	struct audio_buf_desc_s bufdesc;
	int ret;

	/* Now enqueue the buffer with the audio device.  If the number of
	 * bytes in the file happens to be an exact multiple of the audio
	 * buffer size, then we will receive the last buffer size = 0.  We
	 * encode this buffer also so the audio system knows its the end of
	 * the file and can do proper clean-up.
	 */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	bufdesc.session = pPlayer->session;
#endif
	bufdesc.numbytes = apb->nbytes;
	bufdesc.u.pBuffer = apb;

	ret = ioctl(pPlayer->devFd, AUDIOIOC_ENQUEUEBUFFER, (unsigned long)&bufdesc);
	if (ret < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		auddbg("ERROR: AUDIOIOC_ENQUEUEBUFFER ioctl failed: %d\n", errcode);
		return -errcode;
	}

	/* Return OK to indicate that we successfully read data from the file
	 * (and we are not yet at the end of file)
	 */

	return OK;
}

/****************************************************************************
 * Name: nxplayer_thread_playthread
 *
 *  This is the thread that reads the audio file file and enqueues /
 *  dequeues buffers to the selected and opened audio device.
 *
 ****************************************************************************/

static void *nxplayer_playthread(pthread_addr_t pvarg)
{
	struct nxplayer_s *pPlayer = (struct nxplayer_s *)pvarg;
	struct audio_msg_s msg;
	struct audio_buf_desc_s buf_desc;
	ssize_t size;
	bool running = true;
	bool streaming = true;
	bool failed = false;
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	struct ap_buffer_info_s buf_info;
	FAR struct ap_buffer_s **pBuffers;
#else
	FAR struct ap_buffer_s *pBuffers[CONFIG_AUDIO_NUM_BUFFERS];
#endif
#ifdef CONFIG_DEBUG
	int outstanding = 0;
#endif
	int prio;
	int x;
	int ret;

	audvdbg("Entry\n");

	/* Query the audio device for it's preferred buffer size / qty */

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if ((ret = ioctl(pPlayer->devFd, AUDIOIOC_GETBUFFERINFO, (unsigned long)&buf_info)) != OK) {
		/* Driver doesn't report it's buffer size.  Use our default. */

		buf_info.buffer_size = CONFIG_AUDIO_BUFFER_NUMBYTES;
		buf_info.nbuffers = CONFIG_AUDIO_NUM_BUFFERS;
	}

	/* Create array of pointers to buffers */

	pBuffers = (FAR struct ap_buffer_s **)malloc(buf_info.nbuffers * sizeof(FAR void *));
	if (pBuffers == NULL) {
		/* Error allocating memory for buffer storage! */

		ret = -ENOMEM;
		running = false;
		goto err_out;
	}

	/* Create our audio pipeline buffers to use for queueing up data */

	for (x = 0; x < buf_info.nbuffers; x++) {
		pBuffers[x] = NULL;
	}

	for (x = 0; x < buf_info.nbuffers; x++)
#else							/* CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFER */

	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++) {
		pBuffers[x] = NULL;
	}

	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++)
#endif							/* CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFER */
	{
		/* Fill in the buffer descriptor struct to issue an alloc request */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		buf_desc.session = pPlayer->session;
#endif
#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
		buf_desc.numbytes = buf_info.buffer_size;
#else
		buf_desc.numbytes = CONFIG_AUDIO_BUFFER_NUMBYTES;
#endif
		buf_desc.u.ppBuffer = &pBuffers[x];

		ret = ioctl(pPlayer->devFd, AUDIOIOC_ALLOCBUFFER, (unsigned long)&buf_desc);
		if (ret != sizeof(buf_desc)) {
			/* Buffer alloc Operation not supported or error allocating! */

			auddbg("ERROR: Could not allocate buffer %d\n", x);
			running = false;
			goto err_out;
		}
	}

	/* Fill up the pipeline with enqueued buffers */

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	for (x = 0; x < buf_info.nbuffers; x++)
#else
	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++)
#endif
	{
		/* Read the next buffer of data */

		ret = nxplayer_readbuffer(pPlayer, pBuffers[x]);
		if (ret != OK) {
			/* nxplayer_readbuffer will return an error if there is no further
			 * data to be read from the file.  This can happen normally if the
			 * file is very small (less than will fit in
			 * CONFIG_AUDIO_NUM_BUFFERS) or if an error occurs trying to read
			 * from the file.
			 */

			/* We are no longer streaming data from the file */

			streaming = false;

			if (x == 0) {
				/* No buffers read?  Should never really happen.  Even in the
				 * case of a read failure, one empty buffer containing the
				 * AUDIO_APB_FINAL indication will be returned.
				 */

				running = false;
			}
		}

		/* Enqueue buffer by sending it to the audio driver */

		else {
			ret = nxplayer_enqueuebuffer(pPlayer, pBuffers[x]);
			if (ret != OK) {
				/* Failed to enqueue the buffer.  The driver is not happy with
				 * the buffer.  Perhaps a decoder has detected something that it
				 * does not like in the stream and has stopped streaming.  This
				 * would happen normally if we send a file in the incorrect format
				 * to an audio decoder.
				 *
				 * We must stop streaming as gracefully as possible.  Close the
				 * file so that no further data is read.
				 */

				fclose(pPlayer->fileFd);
				pPlayer->fileFd = NULL;

				/* We are no longer streaming data from the file.  Be we will
				 * need to wait for any outstanding buffers to be recovered.  We
				 * also still expect the audio driver to send a AUDIO_MSG_COMPLETE
				 * message after all queued buffers have been returned.
				 */

				streaming = false;
				failed = true;
				break;
			}
#ifdef CONFIG_DEBUG
			else {
				/* The audio driver has one more buffer */

				outstanding++;
			}
#endif
		}
	}

	audvdbg("%d buffers queued, running=%d streaming=%d\n", x, running, streaming);

	/* Start the audio device */

	if (running && !failed) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		ret = ioctl(pPlayer->devFd, AUDIOIOC_START, (unsigned long)pPlayer->session);
#else
		ret = ioctl(pPlayer->devFd, AUDIOIOC_START, 0);
#endif

		if (ret < 0) {
			/* Error starting the audio stream!  We need to continue running
			 * in order to recover the audio buffers that have already been
			 * queued.
			 */

			failed = true;
		}
	}

	if (running && !failed) {
		/* Indicate we are playing a file */

		pPlayer->state = NXPLAYER_STATE_PLAYING;

		/* Set initial parameters such as volume, bass, etc.
		 * REVISIT:  Shouldn't this actually be done BEFORE we start playing?
		 */

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
		(void)nxplayer_setvolume(pPlayer, pPlayer->volume);
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
		nxplayer_setbalance(pPlayer, pPlayer->balance);
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
		nxplayer_setbass(pPlayer, pPlayer->bass);
		nxplayer_settreble(pPlayer, pPlayer->treble);
#endif
	}

	/* Loop until we specifically break.  running == true means that we are
	 * still looping waiting for the playback to complete.  All of the file
	 * data may have been sent (if streaming == false), but the playback is
	 * not complete until we get the AUDIO_MSG_COMPLETE (or AUDIO_MSG_STOP)
	 * message
	 *
	 * The normal protocol for streaming errors detected by the audio driver
	 * is as follows:
	 *
	 * (1) The audio driver will indicated the error by returning a negated
	 *     error value when the next buffer is enqueued.  The upper level
	 *     then knows that this buffer was not queue.
	 * (2) The audio driver must return all queued buffers using the
	 *     AUDIO_MSG_DEQUEUE message, and
	 * (3) Terminate playing by sending the AUDIO_MSG_COMPLETE message.
	 */

	audvdbg("%s\n", running ? "Playing..." : "Not runnning");
	while (running) {
		/* Wait for a signal either from the Audio driver that it needs
		 * additional buffer data, or from a user-space signal to pause,
		 * stop, etc.
		 */

		size = mq_receive(pPlayer->mq, (FAR char *)&msg, sizeof(msg), &prio);

		/* Validate a message was received */

		if (size != sizeof(msg)) {
			/* Interrupted by a signal? What to do? */

			continue;
		}

		/* Perform operation based on message id */

		switch (msg.msgId) {
		/* An audio buffer is being dequeued by the driver */

		case AUDIO_MSG_DEQUEUE:
#ifdef CONFIG_DEBUG
			/* Make sure that we believe that the audio driver has at
			 * least one buffer.
			 */

			DEBUGASSERT(msg.u.pPtr && outstanding > 0);
			outstanding--;
#endif

			/* Read data from the file directly into this buffer and
			 * re-enqueue it.  streaming == true means that we have
			 * not yet hit the end-of-file.
			 */

			if (streaming) {
				/* Read the next buffer of data */

				ret = nxplayer_readbuffer(pPlayer, msg.u.pPtr);
				if (ret != OK) {
					/* Out of data.  Stay in the loop until the device sends
					 * us a COMPLETE message, but stop trying to play more
					 * data.
					 */

					streaming = false;
				}

				/* Enqueue buffer by sending it to the audio driver */

				else {
					ret = nxplayer_enqueuebuffer(pPlayer, msg.u.pPtr);
					if (ret != OK) {
						/* There is some issue from the audio driver.
						 * Perhaps a problem in the file format?
						 *
						 * We must stop streaming as gracefully as possible.
						 * Close the file so that no further data is read.
						 */

						fclose(pPlayer->fileFd);
						pPlayer->fileFd = NULL;

						/* Stop streaming and wait for buffers to be
						 * returned and to receive the AUDIO_MSG_COMPLETE
						 * indication.
						 */

						streaming = false;
						failed = true;
					}
#ifdef CONFIG_DEBUG
					else {
						/* The audio driver has one more buffer */

						outstanding++;
					}
#endif
				}
			}
			break;

		/* Someone wants to stop the playback. */

		case AUDIO_MSG_STOP:
			/* Send a stop message to the device */

			audvdbg("Stopping! outstanding=%d\n", outstanding);

#ifdef CONFIG_AUDIO_MULTI_SESSION
			ioctl(pPlayer->devFd, AUDIOIOC_STOP, (unsigned long)pPlayer->session);
#else
			ioctl(pPlayer->devFd, AUDIOIOC_STOP, 0);
#endif
			/* Stay in the running loop (without sending more data).
			 * we will need to recover our audio buffers.  We will
			 * loop until AUDIO_MSG_COMPLETE is received.
			 */

			streaming = false;
			break;

		/* Message indicating the playback is complete */

		case AUDIO_MSG_COMPLETE:
			audvdbg("Play complete.  outstanding=%d\n", outstanding);
			DEBUGASSERT(outstanding == 0);
			running = false;
			break;

		/* Unknown / unsupported message ID */

		default:
			break;
		}
	}

	/* Release our audio buffers and unregister / release the device */

err_out:
	audvdbg("Clean-up and exit\n");

	/* Unregister the message queue and release the session */

	ioctl(pPlayer->devFd, AUDIOIOC_UNREGISTERMQ, (unsigned long)pPlayer->mq);
#ifdef CONFIG_AUDIO_MULTI_SESSION
	ioctl(pPlayer->devFd, AUDIOIOC_RELEASE, (unsigned long)pPlayer->session);
#else
	ioctl(pPlayer->devFd, AUDIOIOC_RELEASE, 0);
#endif

	/* Cleanup */

	while (sem_wait(&pPlayer->sem) < 0) ;

#ifdef CONFIG_AUDIO_DRIVER_SPECIFIC_BUFFERS
	if (pBuffers != NULL) {
		audvdbg("Freeing buffers\n");
		for (x = 0; x < buf_info.nbuffers; x++) {
			/* Fill in the buffer descriptor struct to issue a free request */

			if (pBuffers[x] != NULL) {
				buf_desc.u.pBuffer = pBuffers[x];
				ioctl(pPlayer->devFd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
			}
		}

		/* Free the pointers to the buffers */

		free(pBuffers);
	}
#else
	audvdbg("Freeing buffers\n");
	for (x = 0; x < CONFIG_AUDIO_NUM_BUFFERS; x++) {
		/* Fill in the buffer descriptor struct to issue a free request */

		if (pBuffers[x] != NULL) {
			buf_desc.u.pBuffer = pBuffers[x];
			ioctl(pPlayer->devFd, AUDIOIOC_FREEBUFFER, (unsigned long)&buf_desc);
		}
	}
#endif

	/* Close the files */

	if (pPlayer->fileFd != NULL) {
		fclose(pPlayer->fileFd);	/* Close the file */
		pPlayer->fileFd = NULL;	/* Clear out the FD */
	}

	close(pPlayer->devFd);		/* Close the device */
	pPlayer->devFd = -1;		/* Mark device as closed */
	mq_close(pPlayer->mq);		/* Close the message queue */
	mq_unlink(pPlayer->mqname);	/* Unlink the message queue */
	pPlayer->state = NXPLAYER_STATE_IDLE;	/* Go to IDLE */

	sem_post(&pPlayer->sem);	/* Release the semaphore */

	/* The playthread is done with the context.  Release it, which may
	 * actually cause the context to be freed if the creator has already
	 * abandoned (released) the context too.
	 */

	nxplayer_release(pPlayer);

	audvdbg("Exit\n");

	return NULL;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nxplayer_setvolume
 *
 *   nxplayer_setvolume() sets the volume.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
int nxplayer_setvolume(FAR struct nxplayer_s *pPlayer, uint16_t volume)
{
	struct audio_caps_desc_s cap_desc;
	int ret;

	/* Thread sync using the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) ;

	/* If we are currently playing, then we need to post a message to
	 * the playthread to perform the volume change operation.  If we
	 * are not playing, then just store the volume setting and it will
	 * be applied before the next playback begins.
	 */

	if (pPlayer->state == NXPLAYER_STATE_PLAYING) {
		/* Send a CONFIGURE ioctl to the device to set the volume */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		cap_desc.session = pPlayer->session;
#endif
		cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
		cap_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
		cap_desc.caps.ac_format.hw = AUDIO_FU_VOLUME;
		cap_desc.caps.ac_controls.hw[0] = volume;
		ret = ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
		if (ret < 0) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			auddbg("ERROR: AUDIOIOC_CONFIGURE ioctl failed: %d\n", errcode);
			return -errcode;
		}
	}

	/* Store the volume setting */

	pPlayer->volume = volume;
	sem_post(&pPlayer->sem);

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_VOLUME */

/****************************************************************************
 * Name: nxplayer_setequalization
 *
 *   Sets the level on each band of an equalizer.  Each band setting is
 *   represented in one percent increments, so the range is 0-100.
 *
 * Input Parameters:
 *   pPlayer      - Pointer to the context to initialize
 *   equalization - Pointer to array of equalizer settings of size
 *                  CONFIG_AUDIO_EQUALIZER_NBANDS bytes.  Each byte
 *                  represents the setting for one band in the range of
 *                  0-100.
 *
 * Returned Value:
 *   OK if equalization was set correctly.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_EQUALIZER
int nxplayer_setequalization(FAR struct nxplayer_s *pPlayer,
                             FAR uint8_t *equalization)
{
#warning Missing logic
	return -ENOSYS;
}
#endif

/****************************************************************************
 * Name: nxplayer_setbass
 *
 *   nxplayer_setbass() sets the bass level and range.
 *
 * Input:
 *   pPlayer  - Pointer to the nxplayer context
 *   level    - Bass level in percentage (0-100)
 *   range    - Bass range in percentage (0-100)
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
int nxplayer_setbass(FAR struct nxplayer_s *pPlayer, uint8_t level)
{
	struct audio_caps_desc_s cap_desc;

	/* Thread sync using the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) ;

	/* If we are currently playing, then we need to post a message to
	 * the playthread to perform the volume change operation.  If we
	 * are not playing, then just store the bass setting and it will
	 * be applied before the next playback begins.
	 */

	if (pPlayer->state == NXPLAYER_STATE_PLAYING) {
		/* Send a CONFIGURE ioctl to the device to set the volume */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		cap_desc.session = pPlayer->session;
#endif
		cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
		cap_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
		cap_desc.caps.ac_format.hw = AUDIO_FU_BASS;
		cap_desc.caps.ac_controls.b[0] = level;
		ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	}

	/* Store the volume setting */

	pPlayer->bass = level;

	sem_post(&pPlayer->sem);

	return -ENOENT;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/****************************************************************************
 * Name: nxplayer_settreble
 *
 *   nxplayer_settreble() sets the treble level and range.
 *
 * Input:
 *   pPlayer  - Pointer to the nxplayer context
 *   level    - Treble level in percentage (0-100)
 *   range    - Treble range in percentage (0-100)
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
int nxplayer_settreble(FAR struct nxplayer_s *pPlayer, uint8_t level)
{
	struct audio_caps_desc_s cap_desc;

	/* Thread sync using the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) ;

	/* If we are currently playing, then we need to post a message to
	 * the playthread to perform the volume change operation.  If we
	 * are not playing, then just store the treble setting and it will
	 * be applied before the next playback begins.
	 */

	if (pPlayer->state == NXPLAYER_STATE_PLAYING) {
		/* Send a CONFIGURE ioctl to the device to set the volume */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		cap_desc.session = pPlayer->session;
#endif
		cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
		cap_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
		cap_desc.caps.ac_format.hw = AUDIO_FU_TREBLE;
		cap_desc.caps.ac_controls.b[0] = level;
		ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	}

	/* Store the volume setting */

	pPlayer->treble = level;

	sem_post(&pPlayer->sem);

	return -ENOENT;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_TONE */

/****************************************************************************
 * Name: nxplayer_setbalance
 *
 *   nxplayer_setbalance() sets the volume.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
int nxplayer_setbalance(FAR struct nxplayer_s *pPlayer, uint16_t balance)
{
	struct audio_caps_desc_s cap_desc;

	/* Thread sync using the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) ;

	/* If we are currently playing, then we need to post a message to
	 * the playthread to perform the volume change operation.  If we
	 * are not playing, then just store the volume setting and it will
	 * be applied before the next playback begins.
	 */

	if (pPlayer->state == NXPLAYER_STATE_PLAYING) {
		/* Send a CONFIGURE ioctl to the device to set the volume */

#ifdef CONFIG_AUDIO_MULTI_SESSION
		cap_desc.session = pPlayer->session;
#endif
		cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
		cap_desc.caps.ac_type = AUDIO_TYPE_FEATURE;
		cap_desc.caps.ac_format.hw = AUDIO_FU_BALANCE;
		cap_desc.caps.ac_controls.hw[0] = balance;
		ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	}

	/* Store the volume setting */

	pPlayer->balance = balance;

	sem_post(&pPlayer->sem);

	return -ENOENT;
}
#endif

/****************************************************************************
 * Name: nxplayer_pause
 *
 *   nxplayer_pause() pauses playback without cancelling it.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
int nxplayer_pause(FAR struct nxplayer_s *pPlayer)
{
	int ret = OK;

	if (pPlayer->state == NXPLAYER_STATE_PLAYING) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		ret = ioctl(pPlayer->devFd, AUDIOIOC_PAUSE, (unsigned long)pPlayer->session);
#else
		ret = ioctl(pPlayer->devFd, AUDIOIOC_PAUSE, 0);
#endif
		if (ret == OK) {
			pPlayer->state = NXPLAYER_STATE_PAUSED;
		}
	}

	return ret;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: nxplayer_resume
 *
 *   nxplayer_resume() resumes playback after a pause operation.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
int nxplayer_resume(FAR struct nxplayer_s *pPlayer)
{
	int ret = OK;

	if (pPlayer->state == NXPLAYER_STATE_PAUSED) {
#ifdef CONFIG_AUDIO_MULTI_SESSION
		ret = ioctl(pPlayer->devFd, AUDIOIOC_RESUME, (unsigned long)pPlayer->session);
#else
		ret = ioctl(pPlayer->devFd, AUDIOIOC_RESUME, 0);
#endif
		if (ret == OK) {
			pPlayer->state = NXPLAYER_STATE_PLAYING;
		}
	}

	return ret;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME */

/****************************************************************************
 * Name: nxplayer_fforward
 *
 *   Selects to fast forward in the audio data stream.  The fast forward
 *   operation can be cancelled by simply selected no sub-sampling with
 *   the AUDIO_SUBSAMPLE_NONE argument returning to normal 1x forward play.
 *   This function may be called multiple times to change fast forward rate.
 *
 *   The preferred way to cancel a fast forward operation is via
 *   nxplayer_cancel_motion() that provides the option to also return to
 *   paused, non-playing state.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   subsample - Identifies the fast forward rate (in terms of sub-sampling,
 *               but does not explicitly require sub-sampling).  See
 *               AUDIO_SUBSAMPLE_* definitions.
 *
 * Returned Value:
 *   OK if fast forward operation successful.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_FFORWARD
int nxplayer_fforward(FAR struct nxplayer_s *pPlayer, uint8_t subsample)
{
	struct audio_caps_desc_s cap_desc;
	int ret;

	DEBUGASSERT(pPlayer && subsample >= AUDIO_SUBSAMPLE_NONE && subsample <= AUDIO_SUBSAMPLE_MAX);

	/* Send a CONFIGURE ioctl to the device to set the forward rate */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	cap_desc.session = pPlayer->session;
#endif
	cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
	cap_desc.caps.ac_type = AUDIO_TYPE_PROCESSING;
	cap_desc.caps.ac_format.hw = AUDIO_PU_SUBSAMPLE_FORWARD;
	cap_desc.caps.ac_controls.b[0] = subsample;

	ret = ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	if (ret < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		auddbg("ERROR: ioctl AUDIOIOC_CONFIGURE failed: %d\n", errcode);
		ret = -errcode;
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: nxplayer_rewind
 *
 *   Selects to rewind in the audio data stream.  The rewind operation must
 *   be cancelled with nxplayer_cancel_motion.  This function may be called
 *   multiple times to change rewind rate.
 *
 *   NOTE that cancellation of the rewind operation differs from
 *   cancellation of the fast forward operation because we must both restore
 *   the sub-sampling rate to 1x and also return to forward play.
 *   AUDIO_SUBSAMPLE_NONE is not a valid argument to this function.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   subsample - Identifies the rewind rate (in terms of sub-sampling, but
 *               does not explicitly require sub-sampling).  See
 *               AUDIO_SUBSAMPLE_* definitions.
 *
 * Returned Value:
 *   OK if rewind operation successfully initiated.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_REWIND
int nxplayer_rewind(FAR struct nxplayer_s *pPlayer, uint8_t subsample)
{
	struct audio_caps_desc_s cap_desc;
	int ret;

	DEBUGASSERT(pPlayer && subsample >= AUDIO_SUBSAMPLE_MIN && subsample <= AUDIO_SUBSAMPLE_MAX);

	/* Send a CONFIGURE ioctl to the device to set the forward rate */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	cap_desc.session = pPlayer->session;
#endif
	cap_desc.caps.ac_len = sizeof(struct audio_caps_s);
	cap_desc.caps.ac_type = AUDIO_TYPE_PROCESSING;
	cap_desc.caps.ac_format.hw = AUDIO_PU_SUBSAMPLE_REWIND;
	cap_desc.caps.ac_controls.b[0] = subsample;

	ret = ioctl(pPlayer->devFd, AUDIOIOC_CONFIGURE, (unsigned long)&cap_desc);
	if (ret < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		auddbg("ERROR: ioctl AUDIOIOC_CONFIGURE failed: %d\n", errcode);
		ret = -errcode;
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: nxplayer_cancel_motion
 *
 *   Cancel a rewind or fast forward operation and return to either the
 *   paused state or to the normal, forward play state.
 *
 * Input Parameters:
 *   pPlayer - Pointer to the context to initialize
 *   paused  - True: return to the paused state, False: return to the 1X
 *             forward play state.
 *
 * Returned Value:
 *   OK if rewind operation successfully cancelled.
 *
 ****************************************************************************/

#if !defined(CONFIG_AUDIO_EXCLUDE_FFORWARD) || !defined(CONFIG_AUDIO_EXCLUDE_REWIND)
int nxplayer_cancel_motion(FAR struct nxplayer_s *pPlayer, bool paused)
{
	int ret;

	/* I think this is equivalent to calling nxplayer_fforward with the
	 * argument AUDIO_SUBSAMPLE_NONE:  Forward motion with no sub-sampling.
	 *
	 * REVISIT: There is no way at present to cancel sub-sampling and return
	 * to pause atomically.
	 */

	ret = nxplayer_fforward(pPlayer, AUDIO_SUBSAMPLE_NONE);
	if (ret < 0) {
		auddbg("ERROR: nxplayer_fforward failed: %d\n", ret);
		return ret;
	}
#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
	if (paused) {
		ret = nxplayer_pause(pPlayer);
		if (ret < 0) {
			auddbg("ERROR: nxplayer_pause failed: %d\n", ret);
			return ret;
		}
	}
#endif

	return OK;
}
#endif

/****************************************************************************
 * Name: nxplayer_setdevice
 *
 *   nxplayer_setdevice() sets the perferred audio device to use with the
 *   provided nxplayer context.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_PREFERRED_DEVICE
int nxplayer_setdevice(FAR struct nxplayer_s *pPlayer, FAR const char *pDevice)
{
	int tempFd;
	struct audio_caps_s caps;

	DEBUGASSERT(pPlayer != NULL);
	DEBUGASSERT(pDevice != NULL);

	/* Try to open the device */

	tempFd = open(pDevice, O_RDWR);
	if (tempFd == -1) {
		/* Error opening the device */

		return -ENOENT;
	}

	/* Validate it's an Audio device by issuing an AUDIOIOC_GETCAPS ioctl */

	caps.ac_len = sizeof(caps);
	caps.ac_type = AUDIO_TYPE_QUERY;
	caps.ac_subtype = AUDIO_TYPE_QUERY;
	if (ioctl(tempFd, AUDIOIOC_GETCAPS, (unsigned long)&caps) != caps.ac_len) {
		/* Not an Audio device! */

		close(tempFd);
		return -ENODEV;
	}

	/* Close the file */

	close(tempFd);

	/* Save the path and format capabilities of the preferred device */

	strncpy(pPlayer->prefdevice, pDevice, sizeof(pPlayer->prefdevice));
	pPlayer->prefformat = caps.ac_format.b[0] | (caps.ac_format.b[1] << 8);
	pPlayer->preftype = caps.ac_controls.b[0];

	return OK;
}
#endif							/* CONFIG_EXAMPLES_NXPLAYER_INCLUDE_PREFERRED_DEVICE */

/****************************************************************************
 * Name: nxplayer_stop
 *
 *   nxplayer_stop() stops the current playback and closes the file and
 *   the associated device.
 *
 * Input:
 *   pPlayer    Pointer to the initialized MPlayer context
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
int nxplayer_stop(FAR struct nxplayer_s *pPlayer)
{
	struct audio_msg_s term_msg;
	FAR void *value;

	DEBUGASSERT(pPlayer != NULL);

	/* Validate we are not in IDLE state */

	sem_wait(&pPlayer->sem);	/* Get the semaphore */
	if (pPlayer->state == NXPLAYER_STATE_IDLE) {
		sem_post(&pPlayer->sem);	/* Release the semaphore */
		return OK;
	}

	sem_post(&pPlayer->sem);

	/* Notify the playback thread that it needs to cancel the playback */

	term_msg.msgId = AUDIO_MSG_STOP;
	term_msg.u.data = 0;
	mq_send(pPlayer->mq, (FAR const char *)&term_msg, sizeof(term_msg), CONFIG_EXAMPLES_NXPLAYER_MSG_PRIO);

	/* Join the thread.  The thread will do all the cleanup. */

	pthread_join(pPlayer->playId, &value);
	pPlayer->playId = 0;

	return OK;
}
#endif							/* CONFIG_AUDIO_EXCLUDE_STOP */

/****************************************************************************
 * Name: nxplayer_playfile
 *
 *   nxplayer_playfile() tries to play the specified file using the Audio
 *   system.  If a preferred device is specified, it will try to use that
 *   device otherwise it will perform a search of the Audio device files
 *   to find a suitable device.
 *
 * Input:
 *   pPlayer    Pointer to the initialized MPlayer context
 *   pFilename  Pointer to the filename to play
 *   filefmt    Format of the file or AUD_FMT_UNDEF if unknown / to be
 *              determined by nxplayer_playfile()
 *
 * Returns:
 *   OK         File is being played
 *   -EBUSY     The media device is busy
 *   -ENOSYS    The media file is an unsupported type
 *   -ENODEV    No audio device suitable to play the media type
 *   -ENOENT    The media file was not found
 *
 ****************************************************************************/

int nxplayer_playfile(FAR struct nxplayer_s *pPlayer,
                      FAR const char *pFilename, int filefmt, int subfmt)
{
	struct mq_attr attr;
	struct sched_param sparam;
	pthread_attr_t tattr;
	void *value;
#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR
	char path[128];
#endif
	int tmpsubfmt = AUDIO_FMT_UNDEF;
	int ret;

	DEBUGASSERT(pPlayer != NULL);
	DEBUGASSERT(pFilename != NULL);

	if (pPlayer->state != NXPLAYER_STATE_IDLE) {
		return -EBUSY;
	}

	audvdbg("==============================\n");
	audvdbg("Playing file %s\n", pFilename);
	audvdbg("==============================\n");

	/* Test that the specified file exists */

	if ((pPlayer->fileFd = fopen(pFilename, "r")) == NULL) {
		/* File not found.  Test if its in the mediadir */

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR
		snprintf(path, sizeof(path), "%s/%s", pPlayer->mediadir, pFilename);

		if ((pPlayer->fileFd = fopen(path, "r")) == NULL) {
#ifdef CONFIG_EXAMPLES_NXPLAYER_MEDIA_SEARCH
			/* File not found in the media dir.  Do a search */

			if (nxplayer_mediasearch(pPlayer, pFilename, path, sizeof(path)) != OK) {
				auddbg("ERROR: Could not find file\n");
				return -ENOENT;
			}
#else
			auddbg("ERROR: Could not open %s or %s\n", pFilename, path);
			return -ENOENT;
#endif							/* CONFIG_EXAMPLES_NXPLAYER_MEDIA_SEARCH */
		}
#else							/* CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR */

		auddbg("ERROR: Could not open %s\n", pFilename);
		return -ENOENT;
#endif							/* CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR */
	}
#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_EXT
	/* Try to determine the format of audio file based on the extension */

	if (filefmt == AUDIO_FMT_UNDEF) {
		filefmt = nxplayer_fmtfromextension(pPlayer, pFilename, &tmpsubfmt);
	}
#endif

#ifdef CONFIG_EXAMPLES_NXPLAYER_FMT_FROM_HEADER
	/* If type not identified, then test for known header types */

	if (filefmt == AUDIO_FMT_UNDEF) {
		filefmt = nxplayer_fmtfromheader(pPlayer, &subfmt, &tmpsubfmt);
	}
#endif

	/* Test if we determined the file format */

	if (filefmt == AUDIO_FMT_UNDEF) {
		/* Hmmm, it's some unknown / unsupported type */

		auddbg("ERROR: Unsupported format: %d \n", filefmt);
		ret = -ENOSYS;
		goto err_out_nodev;
	}

	/* Test if we have a sub format assignment from above */

	if (subfmt == AUDIO_FMT_UNDEF) {
		subfmt = tmpsubfmt;
	}

	/* Try to open the device */

	ret = nxplayer_opendevice(pPlayer, filefmt, subfmt);
	if (ret < 0) {
		/* Error opening the device */

		auddbg("ERROR: nxplayer_opendevice failed: %d\n", ret);
		goto err_out_nodev;
	}

	/* Try to reserve the device */

#ifdef CONFIG_AUDIO_MULTI_SESSION
	ret = ioctl(pPlayer->devFd, AUDIOIOC_RESERVE, (unsigned long)&pPlayer->session);
#else
	ret = ioctl(pPlayer->devFd, AUDIOIOC_RESERVE, 0);
#endif
	if (ret < 0) {
		/* Device is busy or error */

		auddbg("ERROR: Failed to reserve device: %d\n", ret);
		ret = -errno;
		goto err_out;
	}

	/* Create a message queue for the playthread */

	attr.mq_maxmsg = 16;
	attr.mq_msgsize = sizeof(struct audio_msg_s);
	attr.mq_curmsgs = 0;
	attr.mq_flags = 0;

	snprintf(pPlayer->mqname, sizeof(pPlayer->mqname), "/tmp/%0lx", (unsigned long)((uintptr_t) pPlayer));

	pPlayer->mq = mq_open(pPlayer->mqname, O_RDWR | O_CREAT, 0644, &attr);
	if (pPlayer->mq == NULL) {
		/* Unable to open message queue! */

		ret = -errno;
		auddbg("ERROR: mq_open failed: %d\n", ret);
		goto err_out;
	}

	/* Register our message queue with the audio device */

	ioctl(pPlayer->devFd, AUDIOIOC_REGISTERMQ, (unsigned long)pPlayer->mq);

	/* Check if there was a previous thread and join it if there was
	 * to perform clean-up.
	 */

	if (pPlayer->playId != 0) {
		pthread_join(pPlayer->playId, &value);
	}

	/* Start the playfile thread to stream the media file to the
	 * audio device.
	 */

	pthread_attr_init(&tattr);
	sparam.sched_priority = sched_get_priority_max(SCHED_FIFO) - 9;
	(void)pthread_attr_setschedparam(&tattr, &sparam);
	(void)pthread_attr_setstacksize(&tattr, CONFIG_EXAMPLES_NXPLAYER_PLAYTHREAD_STACKSIZE);

	/* Add a reference count to the player for the thread and start the
	 * thread.  We increment for the thread to avoid thread start-up
	 * race conditions.
	 */

	nxplayer_reference(pPlayer);
	ret = pthread_create(&pPlayer->playId, &tattr, nxplayer_playthread, (pthread_addr_t) pPlayer);
	if (ret != OK) {
		auddbg("ERROR: Failed to create playthread: %d\n", ret);
		goto err_out;
	}

	/* Name the thread */

	pthread_setname_np(pPlayer->playId, "playthread");
	return OK;

err_out:
	close(pPlayer->devFd);
	pPlayer->devFd = -1;

err_out_nodev:
	if (pPlayer->fileFd != NULL) {
		fclose(pPlayer->fileFd);
		pPlayer->fileFd = NULL;
	}

	return ret;
}

/****************************************************************************
 * Name: nxplayer_setmediadir
 *
 *   nxplayer_setmediadir() sets the root path for media searches.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR
void nxplayer_setmediadir(FAR struct nxplayer_s *pPlayer,
						  FAR const char *mediadir)
{
	strncpy(pPlayer->mediadir, mediadir, sizeof(pPlayer->mediadir));
}
#endif

/****************************************************************************
 * Name: nxplayer_create
 *
 *   nxplayer_create() allocates and initializes a nxplayer context for
 *   use by further nxplayer operations.  This routine must be called before
 *   to perform the create for proper reference counting.
 *
 * Input Parameters:  None
 *
 * Returned values:
 *   Pointer to the created context or NULL if there was an error.
 *
 ****************************************************************************/

FAR struct nxplayer_s *nxplayer_create(void)
{
	FAR struct nxplayer_s *pPlayer;

	/* Allocate the memory */

	pPlayer = (FAR struct nxplayer_s *)malloc(sizeof(struct nxplayer_s));
	if (pPlayer == NULL) {
		return NULL;
	}

	/* Initialize the context data */

	pPlayer->state = NXPLAYER_STATE_IDLE;
	pPlayer->devFd = -1;
	pPlayer->fileFd = NULL;
#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_PREFERRED_DEVICE
	pPlayer->prefdevice[0] = '\0';
	pPlayer->prefformat = 0;
	pPlayer->preftype = 0;
#endif
	pPlayer->mq = NULL;
	pPlayer->playId = 0;
	pPlayer->crefs = 1;

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
	pPlayer->bass = 50;
	pPlayer->treble = 50;
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
	pPlayer->balance = 500;
#endif

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	pPlayer->volume = 400;
#endif

#ifdef CONFIG_AUDIO_MULTI_SESSION
	pPlayer->session = NULL;
#endif

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_MEDIADIR
	strncpy(pPlayer->mediadir, CONFIG_EXAMPLES_NXPLAYER_DEFAULT_MEDIADIR, sizeof(pPlayer->mediadir));
#endif
	sem_init(&pPlayer->sem, 0, 1);

	return pPlayer;
}

/****************************************************************************
 * Name: nxplayer_release
 *
 *   nxplayer_release() reduces the reference count by one and if it
 *   reaches zero, frees the context.
 *
 * Input Parameters:
 *   pPlayer    Pointer to the NxPlayer context
 *
 * Returned values:    None
 *
 ****************************************************************************/

void nxplayer_release(FAR struct nxplayer_s *pPlayer)
{
	int refcount;
	FAR void *value;

	/* Grab the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		if (errcode != EINTR) {
			auddbg("ERROR: sem_wait failed: %d\n", errcode);
			return;
		}
	}

	/* Check if there was a previous thread and join it if there was */

	if (pPlayer->playId != 0) {
		sem_post(&pPlayer->sem);
		pthread_join(pPlayer->playId, &value);
		pPlayer->playId = 0;

		while (sem_wait(&pPlayer->sem) < 0) {
			int errcode = errno;
			DEBUGASSERT(errcode > 0);

			if (errcode != -EINTR) {
				auddbg("ERROR: sem_wait failed: %d\n", errcode);
				return;
			}
		}
	}

	/* Reduce the reference count */

	refcount = pPlayer->crefs--;
	sem_post(&pPlayer->sem);

	/* If the ref count *was* one, then free the context */

	if (refcount == 1) {
		free(pPlayer);
	}
}

/****************************************************************************
 * Name: nxplayer_reference
 *
 *   nxplayer_reference() increments the reference count by one.
 *
 * Input Parameters:
 *   pPlayer    Pointer to the NxPlayer context
 *
 * Returned values:    None
 *
 ****************************************************************************/

void nxplayer_reference(FAR struct nxplayer_s *pPlayer)
{
	/* Grab the semaphore */

	while (sem_wait(&pPlayer->sem) < 0) {
		int errcode = errno;
		DEBUGASSERT(errcode > 0);

		if (errcode != -EINTR) {
			auddbg("ERROR: sem_wait failed: %d\n", errcode);
			return;
		}
	}

	/* Increment the reference count */

	pPlayer->crefs++;
	sem_post(&pPlayer->sem);
}

/****************************************************************************
 * Name: nxplayer_systemreset
 *
 *   nxplayer_systemreset() performs a HW reset on all registered
 *   audio devices.
 *
 ****************************************************************************/

#ifdef CONFIG_EXAMPLES_NXPLAYER_INCLUDE_SYSTEM_RESET
int nxplayer_systemreset(FAR struct nxplayer_s *pPlayer)
{
	struct dirent *pDevice;
	DIR *dirp;
	char path[64];

	/* Search for a device in the audio device directory */

#ifdef CONFIG_AUDIO_CUSTOM_DEV_PATH
#ifdef CONFIG_AUDIO_DEV_ROOT
	dirp = opendir("/dev");
#else
	dirp = opendir(CONFIG_AUDIO_DEV_PATH);
#endif
#else
	dirp = opendir("/dev/audio");
#endif
	if (dirp == NULL) {
		return -ENODEV;
	}

	while ((pDevice = readdir(dirp)) != NULL) {
		/* We found the next device.  Try to open it and
		 * get its audio capabilities.
		 */

#ifdef CONFIG_AUDIO_CUSTOM_DEV_PATH
#ifdef CONFIG_AUDIO_DEV_ROOT
		snprintf(path, sizeof(path), "/dev/%s", pDevice->d_name);
#else
		snprintf(path, sizeof(path), CONFIG_AUDIO_DEV_PATH "/%s", pDevice->d_name);
#endif
#else
		snprintf(path, sizeof(path), "/dev/audio/%s", pDevice->d_name);
#endif
		if ((pPlayer->devFd = open(path, O_RDWR)) != -1) {
			/* We have the device file open.  Now issue an
			 * AUDIO ioctls to perform a HW reset
			 */

			ioctl(pPlayer->devFd, AUDIOIOC_HWRESET, 0);

			/* Now close the device */

			close(pPlayer->devFd);
		}

	}

	pPlayer->devFd = -1;
	closedir(dirp);
	return OK;
}
#endif							/* CONFIG_EXAMPLES_NXPLAYER_INCLUDE_SYSTEM_RESET */
