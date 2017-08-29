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
 * apps/include/nxplayer/nxplayer.h
 *
 *   Copyright (C) 2013 Ken Pettit. All rights reserved.
 *   Author: Ken Pettit <pettitkd@gmail.com>
 *
 * With updates, enhancements, and modifications by:
 *
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

#ifndef __APPS_SYSTEM_NXPLAYER_NXPLAYER_H
#define __APPS_SYSTEM_NXPLAYER_NXPLAYER_H 1

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Type Declarations
 ****************************************************************************/
/* This structure describes the internal state of the NxPlayer */

struct nxplayer_s {
	int state;					/* Current player state */
	int devFd;					/* File descriptor of active device */
	mqd_t mq;					/* Message queue for the playthread */
	char mqname[16];			/* Name of our message queue */
	pthread_t playId;			/* Thread ID of the playthread */
	int crefs;					/* Number of references to the player */
	sem_t sem;					/* Thread sync semaphore */
	FILE *fileFd;				/* File descriptor of open file */
#ifdef CONFIG_NXPLAYER_INCLUDE_PREFERRED_DEVICE
	char prefdevice[CONFIG_NAME_MAX];	/* Preferred audio device */
	int prefformat;				/* Formats supported by preferred device */
	int preftype;				/* Types supported by preferred device */
#endif
#ifdef CONFIG_NXPLAYER_INCLUDE_MEDIADIR
	char mediadir[CONFIG_NAME_MAX];	/* Root media directory where media is located */
#endif
#ifdef CONFIG_AUDIO_MULTI_SESSION
	FAR void *session;			/* Session assigment from device */
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
	uint16_t volume;			/* Volume as a whole percentage (0-100) */
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
	uint16_t balance;			/* Balance as a whole % (0=left off, 100=right off) */
#endif
#endif
#ifndef CONFIG_AUDIO_EXCLUDE_TONE
	uint16_t treble;			/* Treble as a whole % */
	uint16_t bass;				/* Bass as a whole % */
#endif
};

typedef int (*nxplayer_func)(FAR struct nxplayer_s *pPlayer, char *pargs);

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: nxplayer_create
 *
 *   Allocates and Initializes a NxPlayer context that is passed to all
 *   nxplayer routines.  The player MUST be destroyed using the
 *   nxplayer_destroy() routine since the context is reference counted.
 *   The context can be used in a mode where the caller creates the
 *   context, starts a file playing, and then forgets about the context
 *   and it will self free.  This is because the nxplayer_playfile
 *   will also create a reference to the context, so the client calling
 *   nxplayer_destroy() won't actually de-allocate anything.  The freeing
 *   will occur after the playthread has completed.
 *
 *   Alternately, the caller can create the objec and hold on to it, then
 *   the context will persist until the original creator destroys it.
 *
 * Input Parameters:    None
 *
 * Returned Value:
 *   Pointer to created NxPlayer context or NULL if error.
 *
 ****************************************************************************/

FAR struct nxplayer_s *nxplayer_create(void);

/****************************************************************************
 * Name: nxplayer_release
 *
 *   Reduces the reference count to the player and if it reaches zero,
 *   frees all memory used by the context.
 *
 * Input Parameters:
 *   pPlayer    Pointer to the NxPlayer context
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void nxplayer_release(FAR struct nxplayer_s *pPlayer);

/****************************************************************************
 * Name: nxplayer_reference
 *
 *   Increments the reference count to the player.
 *
 * Input Parameters:
 *   pPlayer    Pointer to the NxPlayer context
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void nxplayer_reference(FAR struct nxplayer_s *pPlayer);

/****************************************************************************
 * Name: nxplayer_setdevice
 *
 *   Sets the preferred Audio device to use with the instance of the
 *   nxplayer.  Without a preferred device set, the nxplayer will search
 *   the audio subsystem to find a suitable device depending on the
 *   type of audio operation requested (i.e. an MP3 decoder device when
 *   playing an MP3 file, a WAV decoder device for a WAV file, etc.).
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   device    - Pointer to pathname of the preferred device
 *
 * Returned Value:
 *   OK if context initialized successfully, error code otherwise.
 *
 ****************************************************************************/

int nxplayer_setdevice(FAR struct nxplayer_s *pPlayer, FAR const char *device);

/****************************************************************************
 * Name: nxplayer_playfile
 *
 *   Plays the specified media file (from the filesystem) using the
 *   Audio system.  If a preferred device has been set, that device
 *   will be used for the playback, otherwise the first suitable device
 *   found in the /dev/audio directory will be used.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   filename  - Pointer to pathname of the file to play
 *   filefmt   - Format of audio in filename if known, AUDIO_FMT_UNDEF
 *               to let nxplayer_playfile() determine automatically.
 *   subfmt    - Sub-Format of audio in filename if known, AUDIO_FMT_UNDEF
 *               to let nxplayer_playfile() determine automatically.
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

int nxplayer_playfile(FAR struct nxplayer_s *pPlayer, FAR const char *filename, int filefmt, int subfmt);

/****************************************************************************
 * Name: nxplayer_stop
 *
 *   Stops current playback.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_STOP
int nxplayer_stop(FAR struct nxplayer_s *pPlayer);
#endif

/****************************************************************************
 * Name: nxplayer_pause
 *
 *   Pauses current playback.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
int nxplayer_pause(FAR struct nxplayer_s *pPlayer);
#endif

/****************************************************************************
 * Name: nxplayer_resume
 *
 *   Resumes current playback.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_PAUSE_RESUME
int nxplayer_resume(FAR struct nxplayer_s *pPlayer);
#endif

/****************************************************************************
 * Name: nxplayer_fforward
 *
 *   Selects to fast forward in the audio data stream.  The fast forward
 *   operation can be cancelled by simply selected no sub-sampling with
 *   the AUDIO_SUBSAMPLE_NONE argument returning to normal 1x forward play.
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
int nxplayer_fforward(FAR struct nxplayer_s *pPlayer, uint8_t subsample);
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
int nxplayer_rewind(FAR struct nxplayer_s *pPlayer, uint8_t subsample);
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
int nxplayer_cancel_motion(FAR struct nxplayer_s *pPlayer, bool paused);
#endif

/****************************************************************************
 * Name: nxplayer_setvolume
 *
 *   Sets the playback volume.  The volume is represented in 1/10th of a
 *   percent increments, so the range is 0-1000.  A value of 10 would mean
 *   1%.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   volume    - Volume level to set in 1/10th percent increments
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
int nxplayer_setvolume(FAR struct nxplayer_s *pPlayer, uint16_t volume);
#endif

/****************************************************************************
 * Name: nxplayer_setbalance
 *
 *   Sets the playback balance.  The balance is represented in 1/10th of a
 *   percent increments, so the range is 0-1000.  A value of 10 would mean
 *   1%.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   balance   - Balance level to set in 1/10th percent increments
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_VOLUME
#ifndef CONFIG_AUDIO_EXCLUDE_BALANCE
int nxplayer_setbalance(FAR struct nxplayer_s *pPlayer, uint16_t balance);
#endif
#endif

/****************************************************************************
 * Name: nxplayer_setmediadir
 *
 *   Sets the root media directory for non-path qualified file searches.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   mediadir  - Pointer to pathname of the media directory
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void nxplayer_setmediadir(FAR struct nxplayer_s *pPlayer, FAR const char *mediadir);

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
int nxplayer_setequalization(FAR struct nxplayer_s *pPlayer, FAR uint8_t *equalization);
#endif

/****************************************************************************
 * Name: nxplayer_setbass
 *
 *   Sets the playback bass level.  The bass is represented in one percent
 *   increments, so the range is 0-100.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   bass      - Bass level to set in one percent increments
 *
 * Returned Value:
 *   OK if the bass level was set successfully
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
int nxplayer_setbass(FAR struct nxplayer_s *pPlayer, uint8_t bass);
#endif

/****************************************************************************
 * Name: nxplayer_settreble
 *
 *   Sets the playback treble level.  The bass is represented in one percent
 *   increments, so the range is 0-100.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *   treble    - Treble level to set in one percent increments
 *
 * Returned Value:
 *   OK if the treble level was set successfully
 *
 ****************************************************************************/

#ifndef CONFIG_AUDIO_EXCLUDE_TONE
int nxplayer_settreble(FAR struct nxplayer_s *pPlayer, uint8_t treble);
#endif

/****************************************************************************
 * Name: nxplayer_systemreset
 *
 *   Performs an audio system reset, including a hardware reset on all
 *   registered audio devices.
 *
 * Input Parameters:
 *   pPlayer   - Pointer to the context to initialize
 *
 * Returned Value:
 *   OK if file found, device found, and playback started.
 *
 ****************************************************************************/

#ifdef CONFIG_NXPLAYER_INCLUDE_SYSTEM_RESET
int nxplayer_systemreset(FAR struct nxplayer_s *pPlayer);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __APPS_SYSTEM_NXPLAYER_NXPLAYER_H */
