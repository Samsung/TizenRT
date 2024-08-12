/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
#include <wifi_manager/wifi_manager.h>

#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileInputDataSource.h>
#include <media/FileOutputDataSource.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>
#include <media/voice/SpeechDetector.h>
#include <media/voice/SpeechDetectorListenerInterface.h>

#include <iostream>
#include <memory>

using namespace std;
using namespace media;
using namespace media::stream;
using namespace media::voice;

media::voice::SpeechDetector *sd;

media::MediaPlayer mp;
media::MediaRecorder mr;

static const char *filePath = "";
FILE *fp;

static bool isRecording = true;

static void playRecordVoice(void);
static void startRecord(void);

class _Observer : public media::MediaPlayerObserverInterface, public std::enable_shared_from_this<_Observer>
{
	void onPlaybackStarted(media::MediaPlayer &mediaPlayer) override
	{
		printf("##################################\n");
		printf("####    onPlaybackStarted     ####\n");
		printf("##################################\n");
	}
	void onPlaybackFinished(media::MediaPlayer &mediaPlayer) override
	{
		printf("##################################\n");
		printf("####    onPlaybackFinished    ####\n");
		printf("##################################\n");

		mp.unprepare();
		mp.destroy();

		printf("##################################\n");
		printf("####   Playback done!!        ####\n");
		printf("##################################\n");

		printf("###################################\n");
		printf("#### Wait for wakeup triggered ####\n");
		printf("###################################\n");

		sd->startKeywordDetect();
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
	}
};

class BufferReceiver : public media::MediaRecorderObserverInterface, public std::enable_shared_from_this<BufferReceiver>
{
	void onRecordStarted(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####     onRecordStarted      ####\n");
		printf("##################################\n");

		filePath = "/tmp/record.pcm";
		fp = fopen(filePath, "wb");
		if (fp == NULL) {
			printf("FILE OPEN FAILED\n");
			return;
		}
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
};

class SpeechDetectorListener : public media::voice::SpeechDetectorListenerInterface, public std::enable_shared_from_this<SpeechDetectorListener>
{
public:
	void onSpeechDetectionListener(media::voice::speech_detect_event_type_e event) override
	{
		printf("#### onSpeechDetectionListener\n");
		if (event == SPEECH_DETECT_KD) {
			printf("Event SPEECH_DETECT_KD\n");
			printf("#### [SD] keyword detected.\n");
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
};

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

	mret = mr.setObserver(std::make_shared<BufferReceiver>());
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
	auto source = std::move(unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(filePath)));
	source->setSampleRate(16000);
	source->setChannels(1);
	source->setPcmFormat(media::AUDIO_FORMAT_TYPE_S16_LE);
	mp.setObserver(std::make_shared<_Observer>());
	mp.setDataSource(std::move(source));
	mp.prepare();
	mp.setVolume(5);
	mp.start();
}

extern "C" {
int wakerec_main(int argc, char *argv[])
{
	printf("wakerec_main Entry\n");
	sd = media::voice::SpeechDetector::instance();
	if (!sd->initKeywordDetect(16000, 1)) {
		printf("#### [SD] init failed.\n");
		return 0;
	}

	sd->addListener(std::make_shared<SpeechDetectorListener>());

	printf("###################################\n");
	printf("#### Wait for wakeup triggered ####\n");
	printf("###################################\n");

	sd->startKeywordDetect();

	while (1) {
		sleep(67);
	}

	return 0;
}
}

