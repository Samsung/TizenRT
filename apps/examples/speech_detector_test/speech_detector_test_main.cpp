/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
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

#include <stdio.h>
#include <semaphore.h>

#include <media/MediaRecorder.h>
#include <media/MediaTypes.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/BufferOutputDataSource.h>

#include <media/MediaPlayer.h>
#include <media/MediaPlayerObserverInterface.h>
#include <media/FileInputDataSource.h>

#include <media/voice/SpeechDetector.h>

static sem_t play_sem;
static sem_t record_sem;
bool isRecordWellDone;

const char *filePath = "/tmp/record.pcm";

class _Observer : public media::MediaPlayerObserverInterface, public std::enable_shared_from_this<_Observer>
{
public:
	void onPlaybackStarted(media::MediaPlayer &mediaPlayer)
	{
		printf("##################################\n");
		printf("####    onPlaybackStarted     ####\n");
		printf("##################################\n");
	}
	void onPlaybackFinished(media::MediaPlayer &mediaPlayer)
	{
		sem_post(&play_sem);
		printf("##################################\n");
		printf("####    onPlaybackFinished    ####\n");
		printf("##################################\n");
	}
	void onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
	{
		printf("##################################\n");
		printf("####      onPlaybackError     ####\n");
		printf("##################################\n");
	}
	void onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
	{
	}
	void onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
	{
	}
	void onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
	{
	}
	void onPlaybackPaused(media::MediaPlayer &mediaPlayer)
	{
	}
};

class BufferReceiver : public media::MediaRecorderObserverInterface, public std::enable_shared_from_this<BufferReceiver>
{
public:
	void onRecordStarted(media::MediaRecorder &mediaRecorder) override
	{
		printf("##################################\n");
		printf("####     onRecordStarted      ####\n");
		printf("##################################\n");
		fp = fopen(filePath, "w");
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
		printf("####     onRecordFinished     ####\n");
		printf("##################################\n");
		fclose(fp);
		isRecordWellDone = true;
		sem_post(&record_sem);
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
		fclose(fp);
		sem_post(&record_sem);
	}

	void onRecordBufferDataReached(media::MediaRecorder &mediaRecorder, std::shared_ptr<unsigned char> data, size_t size) override
	{
		static short result[8000];
		short *sdata = (short *)data.get();
		for (size_t i = 0; i < (size >> 1); i++) {
			result[i * 2] = sdata[i];
			result[i * 2 + 1] = sdata[i];
		}

		if (fp != NULL) {
			fwrite(result, sizeof(unsigned char), size << 1, fp);
		}

		auto sd = media::voice::SpeechDetector::instance();
		sd->detectEndPoint(sdata, size >> 1);
	}
private:
	FILE *fp;
};

void play_data()
{
	media::MediaPlayer mp;
	printf("##################################\n");
	printf("####       Let's Play         ####\n");
	printf("##################################\n");

	mp.create();
	auto source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(filePath)));
	source->setSampleRate(16000);
	source->setChannels(2);
	source->setPcmFormat(media::AUDIO_FORMAT_TYPE_S16_LE);
	mp.setObserver(std::make_shared<_Observer>());
	mp.setDataSource(std::move(source));
	mp.prepare();
	mp.start();

	sem_wait(&play_sem);

	printf("##################################\n");
	printf("####   Playback done!!        ####\n");
	printf("##################################\n");

	mp.unprepare();
	mp.destroy();
}

bool run()
{
	media::MediaRecorder mr;
	bool ret;

	auto sd = media::voice::SpeechDetector::instance();
	sd->initKeywordDetect(16000, 1);
	sd->initEndPointDetect(16000, 1);

	media::recorder_result_t mret = mr.create();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] create succeeded.\n");
	} else {
		printf("#### [MR] create failed.\n");
	}

	mret = mr.setDataSource(std::unique_ptr<media::stream::BufferOutputDataSource>(
		new media::stream::BufferOutputDataSource(1, 16000, media::AUDIO_FORMAT_TYPE_S16_LE)));
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] setDataSource succeeded.\n");
	} else {
		printf("#### [MR] setDataSource failed.\n");
	}

	mret = mr.setObserver(std::make_shared<BufferReceiver>());
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] setObserver succeeded.\n");
	} else {
		printf("#### [MR] setObserver failed.\n");
	}

	printf("###################################\n");
	printf("#### Wait for wakeup triggered ####\n");
	printf("###################################\n");

	ret = sd->startKeywordDetect(20);
	if (ret == false) {
		mr.unprepare();
		mr.destroy();
		sd->deinitKeywordDetect();
		sd->deinitEndPointDetect();
		return false;
	}

	sd->deinitKeywordDetect();
	isRecordWellDone = false;
	mret = mr.prepare();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] prepare succeeded.\n");
	} else {
		printf("#### [MR] prepare failed.\n");
	}

	mret = mr.start();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] start succeeded.\n");
	} else {
		printf("#### [MR] start failed.\n");
	}

	ret = sd->waitEndPoint(20);
	if (ret == false) {
		mr.stop();
		mr.unprepare();
		mr.destroy();
		sd->deinitKeywordDetect();
		sd->deinitEndPointDetect();
		return false;
	}

	mret = mr.stop();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] stop succeeded.\n");
	} else {
		printf("#### [MR] stop failed.\n");
	}
	sem_wait(&record_sem);

	mret = mr.unprepare();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] unprepare succeeded.\n");
	} else {
		printf("#### [MR] unprepare failed.\n");
	}
	mret = mr.destroy();
	if (mret == media::RECORDER_OK) {
		printf("#### [MR] destroy succeeded.\n");
	} else {
		printf("#### [MR] destroy failed.\n");
	}

	sd->deinitEndPointDetect();

	printf("##################################\n");
	printf("####      EPD Triggered       ####\n");
	printf("##################################\n");

	if (!isRecordWellDone) {
		return false;
	}

	return true;
}

extern "C" {
#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int speech_detector_test_main(int argc, char *argv[])
#endif
{
	sem_init(&play_sem, 0, 0);
	sem_init(&record_sem, 0, 0);
	while (true) {
		if (!run()) {
			continue;
		}
		play_data();
	}
	return 0;
}
}
