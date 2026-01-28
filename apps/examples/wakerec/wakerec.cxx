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
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
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
#include <audio/SoundManager.h>
#include <iostream>
#include <memory>
#include <functional>

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

media::voice::SpeechDetector *sd;

static const char *filePath = "/tmp/record.pcm";
uint8_t *gBuffer = NULL;
uint32_t bufferSize = 0;

static bool isRecording = false;
class WakeRec : public media::voice::SpeechDetectorListenerInterface,public FocusChangeListener,
				public media::MediaRecorderObserverInterface, public media::MediaPlayerObserverInterface,
				public std::enable_shared_from_this<WakeRec>
{
public:
	void onMicMute()
	{
		if (isRecording) {
			/* already paused by framework, not required to stop.*/
			mr.unprepare();
			mr.destroy();
			fclose(fp);
			isRecording = false;
			playRecordVoice();
		}
	}
private:
	MediaPlayer mp;
	MediaRecorder mr;
	shared_ptr<FocusRequest> mFocusRequest;
	FILE *fp;
	bool mPaused;

	void onRecordFinished(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####      onRecordFinished    ####\n");
		printf("##################################\n");
		mr.unprepare();
		mr.destroy();
		fclose(fp);
		isRecording = false;
		playRecordVoice();
	}

	void onRecordStopped(MediaRecorder& mediaRecorder, recorder_error_t errCode) override
	{
		printf("##################################\n");
		printf("####      onRecordStopped      ####\n");
		printf("##################################\n");
		mr.unprepare();
		mr.destroy();
		fclose(fp);
		isRecording = false;
		playRecordVoice();
	}

	void onRecordBufferDataReached(media::MediaRecorder &mediaRecorder, std::shared_ptr<unsigned char> data, size_t size) override
	{
		if (!isRecording) {
			fp = fopen(filePath, "ab");
			isRecording = true;
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
	}


	void onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override
	{
		printf("##################################\n");
		printf("####      onPlaybackError     ####\n");
		printf("##################################\n");
	}

	void onSpeechDetectionListener(media::voice::speech_detect_event_type_e event) override
	{
		printf("#### onSpeechDetectionListener\n");
		if (event == SPEECH_DETECT_KD) {
			/* take wakelock as soon as possible, and we hold it until we play recorded data */
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
			fclose(fp);
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
#ifdef PLAYER_SYNC_API_CALL
					auto &focusManager = FocusManager::getFocusManager();
					focusManager.abandonFocus(mFocusRequest);
				} else {
					mPaused = false;
					printf("player start succeeded\n");
#endif
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
#ifdef PLAYER_SYNC_API_CALL
					auto &focusManager = FocusManager::getFocusManager();
					focusManager.abandonFocus(mFocusRequest);
				} else {
					mPaused = false;
					printf("player start succeeded\n");
#endif
				}
			}
		} break;
		case FOCUS_LOSS: {
			res = mp.stop();
			if (res != PLAYER_OK) {
				printf("player stop failed res : %d\n", res);
			} else {
				mPaused = false;
				mp.unprepare();
				mp.destroy();
				printf("###################################\n");
				printf("#### Wait for wakeup triggered ####\n");
				printf("###################################\n");
				sd->startKeywordDetect();
			}
			auto &focusManager = FocusManager::getFocusManager();
			focusManager.abandonFocus(mFocusRequest); // call abandon focus when FOCUS_GAIN callback not required.
		} break;
		case FOCUS_LOSS_TRANSIENT: {
			res = mp.pause(); //it will be played again
			if (res != PLAYER_OK) {
				printf("player pause failed res : %d\n", res);
#ifdef PLAYER_SYNC_API_CALL
			}
			else {
				mPaused = true;
				printf("player pause succeeded\n");
#endif
			}
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

		mret = mr.start();
		if( mret != RECORDER_OK) {
			printf("#### [MR] start error!! ret : %d\n", mret);
		} else {
			printf("#### [MR] start succeeded.\n");
		}
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

static shared_ptr<WakeRec> wakerecObj = make_shared<WakeRec>();
static void WakerecVolumeChangedListener(stream_policy_t policy, int8_t volume)
{
	printf("WakerecVolumeChangedListener called with volume :%d, policy : %d\n",volume, policy);
	if (volume == AUDIO_DEVICE_MUTE_VALUE) {
		if (policy == STREAM_TYPE_VOICE_RECORD) {
			printf("Mic is muted\n");
			wakerecObj->onMicMute();
		} else {
			printf("Speaker is muted\n");
		}
	} else if (policy == STREAM_TYPE_VOICE_RECORD) {
		printf("Mic unmuted\n");
		if (volume != AUDIO_DEVICE_UNMUTE_VALUE) {
			printf("Input gain changed to %d\n", volume);
		}
	} else {
		printf("Speaker unmuted\n");
		printf("Speaker volume changed to %d\n", volume);
	}
}

extern "C" {
int wakerec_main(int argc, char *argv[])
{
	printf("wakerec_main Entry\n");
	if (argc != 2) {
		printf("invalid input\n");
		printf("Usage : wakerec [model]\n");
		printf("model 0 is 'hi bixby' model 1 is 'bixby'\n");
		return -1;
	}

	sd = media::voice::SpeechDetector::instance();
	int model = atoi(argv[1]);
	
	if (!sd->initKeywordDetect(16000, 1)) {
		printf("#### [SD] init failed.\n");
		return 0;
	}
	sd->changeKeywordModel(model);
	sd->addListener(wakerecObj);

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
	addVolumeStateChangedListener(WakerecVolumeChangedListener);
	sd->startKeywordDetect();

	while (1) {
		sleep(67);
	}
	delete[] gBuffer;
	gBuffer = NULL;
	return 0;
}
}
