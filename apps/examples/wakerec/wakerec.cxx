/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

//***************************************************************************
// Included Files
//***************************************************************************

#include <tinyara/config.h>
#include <cstdio>
#include <debug.h>
#include <tinyara/init.h>
#include <tinyara/pm/pm.h>
#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileInputDataSource.h>
#include <media/FileOutputDataSource.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>
#include <media/voice/SpeechDetector.h>
#include <media/voice/SpeechDetectorListenerInterface.h>
#include <media/FocusManager.h>
#include <media/stream_info.h>
#include <iostream>
#include <memory>
#include <functional>
#include <tinyara/pm/pm.h>

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

media::voice::SpeechDetector *sd;

static const char *filePath = "/tmp/record.pcm";
uint8_t *gBuffer = NULL;
uint32_t bufferSize = 0;

static bool isRecording = true;

static void playRecordVoice(void);
static void startRecord(void);

class WakeRec : public media::voice::SpeechDetectorListenerInterface,public FocusChangeListener,
				public media::MediaRecorderObserverInterface, public media::MediaPlayerObserverInterface,
				public std::enable_shared_from_this<WakeRec>
{
private:
	MediaPlayer mp;
	MediaRecorder mr;
	shared_ptr<FocusRequest> mFocusRequest;
	FILE *fp;
	bool mPaused;

	void onRecordStarted(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####     onRecordStarted      ####\n");
		printf("##################################\n");
	}
	void onRecordPaused(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####      onRecordPaused      ####\n");
		printf("##################################\n");
	}
	void onRecordFinished(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####      onRecordFinished    ####\n");
		printf("##################################\n");
		mr.unprepare();
		mr.destroy();
		fclose(fp);
		playRecordVoice();
	}
	void onRecordStartError(media::MediaRecorder &mediaRecorder, media::recorder_error_t errCode) override
	{
		printf("#### onRecordStartError!! errCode : %d\n", errCode);
	}
	void onRecordPauseError(media::MediaRecorder &mediaRecorder, media::recorder_error_t errCode) override
	{
		printf("#### onRecordPauseError!! errCode : %d\n", errCode);
	}
	void onRecordStopError(media::MediaRecorder &mediaRecorder, media::recorder_error_t errCode) override
	{
		printf("#### onRecordStopError!! errCode : %d\n", errCode);
	}

	void onRecordStopped(media::MediaRecorder &mediaRecorder, media::recorder_error_t errCode) override
	{
		printf("##################################\n");
		printf("####      onRecordStopped     ####\n");
		printf("##################################\n");

		if (errCode == RECORDER_ERROR_DEVICE_DEAD) {
			printf("####      Mic is unreachable     ####\n");
			mr.unprepare();
			mr.destroy();
			fclose(fp);
			playRecordVoice();
		}
	}

	void onRecordBufferDataReached(media::MediaRecorder &mediaRecorder, std::shared_ptr<unsigned char> data, size_t size) override
	{
		if (!isRecording) {
			return;
		}
		printf("###########################################\n");
		printf("####      onRecordBufferDataReached    ####\n");
		printf("###########################################\n");

		short *sdata = (short *)data.get();
		if (fp != NULL) {
			int sz_written = fwrite(sdata, sizeof(short), size / 2, fp);
			printf("\n********Size written to file= %d *********\n", sz_written);
		}
	}
	
	void onPlaybackStarted(media::MediaPlayer &mediaPlayer) override
	{
		printf("##################################\n");
		printf("####    onPlaybackStarted     ####\n");
		printf("##################################\n");
		mPaused = false;
	}
	void onPlaybackFinished(media::MediaPlayer &mediaPlayer) override
	{
		printf("##################################\n");
		printf("####    onPlaybackFinished    ####\n");
		printf("##################################\n");

		mp.unprepare();
		mp.destroy();
		mPaused = false;
		auto &focusManager = FocusManager::getFocusManager();
		focusManager.abandonFocus(mFocusRequest);

		printf("##################################\n");
		printf("####   Playback done!!        ####\n");
		printf("##################################\n");

		printf("###################################\n");
		printf("#### Wait for wakeup triggered ####\n");
		printf("###################################\n");

		sd->startKeywordDetect();
		/* Now that we finished playback, we can go to sleep */
		sleep(3); //for test, add sleep.

		pm_resume(PM_IDLE_DOMAIN);
	}

	void onPlaybackStopped(media::MediaPlayer &mediaPlayer) override
	{
		mPaused = false;
		mp.unprepare();
		mp.destroy();
		printf("###################################\n");
		printf("#### Wait for wakeup triggered ####\n");
		printf("###################################\n");
		sd->startKeywordDetect();
		pm_resume(PM_IDLE_DOMAIN);
	}

	void onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override
	{
		printf("##################################\n");
		printf("####      onPlaybackError     ####\n");
		printf("##################################\n");
	}

	void onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override
	{
	}

	void onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override
	{
	}

	void onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override
	{
	}

	void onPlaybackPaused(media::MediaPlayer &mediaPlayer) override
	{
		mPaused = true;
	}

	void onSpeechDetectionListener(media::voice::speech_detect_event_type_e event) override
	{
		printf("#### onSpeechDetectionListener\n");
		if (event == SPEECH_DETECT_KD) {
			/* take wakelock as soon as possible, and we hold it until we play recorded data */
			pm_suspend(PM_IDLE_DOMAIN);
			printf("Event SPEECH_DETECT_KD\n");
			printf("#### [SD] keyword detected.\n");
			fp = fopen(filePath, "wb");
			if (fp == NULL) {
				printf("FILE OPEN FAILED\n");
				return;
			}

			if (gBuffer) {
				if (sd->getKeywordData(gBuffer) == true) {
					/* consume buffer */
					fwrite(gBuffer, 1, bufferSize, fp);	
					printf("KD data extraction OK\n");
				} else {
					printf("kd data extraction failed\n");
				}
			}
			sd->stopKeywordDetect();
			startRecord();
		} else if (event == SPEECH_DETECT_EPD) {
			// do nothing
		} else if (event == SPEECH_DETECT_NONE) {
			printf("Event SPEECH_DETECT_NO_EPD\n");
		} else {
			printf("Event not valid\n");
		}
	}

	void onFocusChange(int focusChange) override
	{
		player_result_t res;
		printf("focusChange : %d mPaused : %d\n", focusChange, mPaused);
		switch (focusChange) {
		case FOCUS_GAIN:
		case FOCUS_GAIN_TRANSIENT: {
			if (mPaused) {
				printf("it was paused, just start playback now\n");
				res = mp.start();
				if (res != PLAYER_OK) {
					printf("player start failed res : %d\n", res);
					auto &focusManager = FocusManager::getFocusManager();
					focusManager.abandonFocus(mFocusRequest);
				}
			} else {
				auto source = std::move(unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(filePath)));
				source->setSampleRate(16000);
				source->setChannels(1);
				source->setPcmFormat(media::AUDIO_FORMAT_TYPE_S16_LE);
				mp.setDataSource(std::move(source));
				mp.prepare();
				mp.setVolume(10);
				res = mp.start();
				if (res != PLAYER_OK) {
					printf("player start failed res : %d\n", res);
					auto &focusManager = FocusManager::getFocusManager();
					focusManager.abandonFocus(mFocusRequest);
				}
			}
		} break;
		case FOCUS_LOSS: {
			mp.stop();
			auto &focusManager = FocusManager::getFocusManager();
			focusManager.abandonFocus(mFocusRequest); // call abandon focus when FOCUS_GAIN callback not required.
		} break;
		case FOCUS_LOSS_TRANSIENT: {
			mp.pause(); //it will be played again
		} break;
		default: {
		} break;
		}
	}

	void startRecord(void)
	{
		media::recorder_result_t mret = mr.create();
		if (mret == media::RECORDER_OK) {
			printf("#### [MR] create succeeded.\n");
		} else {
			printf("#### [MR] create failed.\n");
			return;
		}

		mret = mr.setDataSource(std::unique_ptr<media::stream::BufferOutputDataSource>(
			new media::stream::BufferOutputDataSource(1, 16000, media::AUDIO_FORMAT_TYPE_S16_LE)));
		if (mret == media::RECORDER_OK) {
			printf("#### [MR] setDataSource succeeded.\n");
		} else {
			printf("#### [MR] setDataSource failed.\n");
			return;
		}

		mret = mr.setObserver(shared_from_this());
		if (mret == media::RECORDER_OK) {
			printf("#### [MR] setObserver succeeded.\n");
		} else {
			printf("#### [MR] setObserver failed.\n");
			return;
		}

		if (mr.setDuration(7) == RECORDER_ERROR_NONE && mr.prepare() == RECORDER_ERROR_NONE) {
			printf("#### [MR] prepare succeeded.\n");
		} else {
			printf("#### [MR] prepare failed.\n");
			return;
		}

		mr.start();
	}

	void playRecordVoice(void)
	{
		mp.create();
		mp.setObserver(shared_from_this());
		stream_info_t *info;
		stream_info_create(STREAM_TYPE_MEDIA, &info);
		auto deleter = [](stream_info_t *ptr) { stream_info_destroy(ptr); };
		auto stream_info = std::shared_ptr<stream_info_t>(info, deleter);
		mFocusRequest = FocusRequest::Builder()
							.setStreamInfo(stream_info)
							.setFocusChangeListener(shared_from_this())
							.build();
		mp.setStreamInfo(stream_info);
		auto &focusManager = FocusManager::getFocusManager();
		printf("mp : %x request focus!!\n", &mp);
		focusManager.requestFocus(mFocusRequest);
	}
};

extern "C" {
int wakerec_main(int argc, char *argv[])
{
	printf("wakerec_main Entry\n");
	sd = media::voice::SpeechDetector::instance();
	if (!sd->initKeywordDetect(16000, 1)) {
		printf("#### [SD] init failed.\n");
		return 0;
	}

	sd->addListener(std::make_shared<WakeRec>());

	printf("###################################\n");
	printf("#### Wait for wakeup triggered ####\n");
	printf("###################################\n");
	
	if (sd->getKeywordBufferSize(&bufferSize) == true) {
		printf("KD buffer size %d\n", bufferSize);
		gBuffer = new uint8_t[bufferSize];
		if (!gBuffer) {
			printf("memory allocation failed\n");
		}
	}
	sd->startKeywordDetect();
	/* similar to wake lock, we release wake lock as we started our thread */
	pm_resume(PM_IDLE_DOMAIN);

	while (1) {
		sleep(67);
	}
	delete[] gBuffer;
	gBuffer = NULL;
	return 0;
}
}

