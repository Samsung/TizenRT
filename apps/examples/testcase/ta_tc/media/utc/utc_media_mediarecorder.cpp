/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in comrliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or imrlied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <sys/stat.h>
#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/FileOutputDataSource.h>
#include "tc_common.h"

#define RECORD_DURATION 2

using namespace std;
using namespace media;
using namespace media::stream;

static unsigned int channels = 2;
static unsigned int sampleRate = 16000;
static audio_format_type_t pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
static const char *filePath = "/tmp/record";
static const char *filePath_opus = "/tmp/record_opus.opus";
static const char *filePath_wav = "/tmp/record_wav.wav";

static void utc_media_MediaRecorder_create_p(void)
{
	MediaRecorder mr;
	TC_ASSERT_EQ("utc_media_mediarecorder_create", mr.create(), RECORDER_OK);

	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_create_n(void)
{
	MediaRecorder mr;
	mr.create();

	TC_ASSERT_EQ("utc_media_mediarecorder_create", mr.create(), RECORDER_ERROR_INVALID_STATE);

	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_destroy_p(void)
{
	MediaRecorder mr;
	mr.create();
	TC_ASSERT_EQ("utc_media_MediaRecorder_destroy", mr.destroy(), RECORDER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_destroy_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_MediaRecorder_destroy", mr.destroy(), RECORDER_ERROR_NOT_ALIVE);
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_MediaRecorder_destroy", mr.destroy(), RECORDER_ERROR_INVALID_STATE);
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setDataSource_p(void)
{
	MediaRecorder mr;
	mr.create();
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr.setDataSource(std::move(dataSource)), RECORDER_OK);
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setDataSource_n(void)
{
	MediaRecorder mr;
	MediaRecorder mr2;

	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	mr.create();
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr.setDataSource(std::move(nullptr)), RECORDER_ERROR_INVALID_PARAM);
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr2.setDataSource(std::move(dataSource)), RECORDER_ERROR_INVALID_STATE);
	mr.destroy();
	dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr.setDataSource(std::move(dataSource)), RECORDER_ERROR_NOT_ALIVE);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setVolume_p(void)
{
	uint8_t volume;
	uint8_t prev;
	MediaRecorder mr;
	mr.create();
	mr.getVolume(&prev);

	if (mr.setVolume(prev) == RECORDER_ERROR_DEVICE_NOT_SUPPORTED) {
		printf("device does not support volume control\n");
		TC_ASSERT_NEQ_CLEANUP("utc_media_mediarecorder_setVolume", mr.setVolume(prev + 1), RECORDER_OK, goto cleanup);
	} else {
		TC_ASSERT_EQ_CLEANUP("utc_media_mediarecorder_setVolume", mr.setVolume(10), RECORDER_OK, goto cleanup);
		mr.getVolume(&volume);
		TC_ASSERT_EQ_CLEANUP("utc_media_mediarecorder_setVolume", volume, 10, goto cleanup);
	}

cleanup:
	mr.setVolume(prev);
	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setVolume_n(void)
{
	uint8_t prev, volume;
	MediaRecorder mr;
	mr.create();
	mr.getVolume(&prev);

	if (mr.setVolume(prev) == RECORDER_ERROR_DEVICE_NOT_SUPPORTED) {
		mr.setVolume(prev + 1);
		mr.getVolume(&volume);
		TC_ASSERT_NEQ_CLEANUP("utc_media_mediarecorder_setVolume", prev + 1, volume, goto cleanup);
	} else {
		TC_ASSERT_EQ_CLEANUP("utc_media_mediarecorder_setVolume", mr.setVolume(11), RECORDER_OK, goto cleanup);
	}

	TC_SUCCESS_RESULT();
cleanup:
	mr.setVolume(prev);
	mr.destroy();
}

static void utc_media_MediaRecorder_getVolume_p(void)
{
	uint8_t volume, prev;
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.create();
	mr.getVolume(&prev);

	for (int i = 0; i <= 10; ++i) {
		if (mr.setVolume(i) == RECORDER_ERROR_DEVICE_NOT_SUPPORTED) {
			mr.getVolume(&volume);
			TC_ASSERT_NEQ_CLEANUP("utc_media_MediaRecorder_getVolume", volume, 0, goto cleanup);
			break;
		} else {
			mr.setVolume(i);
			mr.getVolume(&volume);
			TC_ASSERT_EQ_CLEANUP("utc_media_MediaRecorder_getVolume", volume, i, goto cleanup);
		}
	}

	TC_SUCCESS_RESULT();
cleanup:
	mr.setVolume(prev);
	mr.destroy();
}

static void utc_media_MediaRecorder_getVolume_n(void)
{
	MediaRecorder mr;
	uint8_t volume;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_MediaRecorder_getVolume", mr.getVolume(&volume), RECORDER_ERROR_NOT_ALIVE);
	mr.create();
	TC_ASSERT_EQ_CLEANUP("utc_media_MediaRecorder_getVolume", mr.getVolume(nullptr), RECORDER_ERROR_INVALID_PARAM, mr.destroy());
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_getMaxVolume_p(void)
{
	uint8_t volume;
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.create();

	mr.getMaxVolume(&volume);
	TC_ASSERT_EQ_CLEANUP("utc_media_MediaRecorder_getMaxVolume", volume, 10, mr.destroy());

	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_getMaxVolume_n(void)
{
	/* getMaxVolume without create */
	{
		uint8_t volume;
		media::MediaRecorder mr;
		TC_ASSERT_EQ("utc_media_MediaRecorder_getMaxVolume", mr.getMaxVolume(&volume), media::RECORDER_ERROR_NOT_ALIVE);
	}

	/* getMaxVolume with nullptr */
	{
		media::MediaRecorder mr;
		mr.create();
		TC_ASSERT_EQ_CLEANUP("utc_media_MediaRecorder_getMaxVolume", mr.getMaxVolume(nullptr), media::RECORDER_ERROR_INVALID_PARAM, mr.destroy());
		mr.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_prepare_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.create();
	mr.setDataSource(std::move(dataSource));
	TC_ASSERT_EQ("utc_media_MediaRecorder_prepare", mr.prepare(), RECORDER_OK);
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_prepare_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	mr.prepare();
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_MediaRecorder_prepare", mr.prepare(), RECORDER_ERROR_INVALID_STATE);
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_unprepare_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_MediaRecorder_unprepare", mr.unprepare(), RECORDER_OK);
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_unprepare_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	mr.unprepare();
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	mr.unprepare();
	TC_ASSERT_EQ("utc_media_MediaRecorder_unprepare", mr.unprepare(), RECORDER_ERROR_INVALID_STATE);
	mr.destroy();

	TC_SUCCESS_RESULT();
}

class RecorderTest : public MediaRecorderObserverInterface
{
public:
	RecorderTest() {}
	~RecorderTest() {}

	void onRecordStarted(MediaRecorder& mediaRecorder) override
	{
		notifyStarted();
	}
	void onRecordPaused(MediaRecorder& mediaRecorder) override
	{
		notifyPaused();
	}
	void onRecordFinished(MediaRecorder& mediaRecorder) override
	{
		notifyFinished();
	}
	void onRecordStartError(MediaRecorder& mediaRecorder, recorder_error_t ret) override
	{
		notifyError();
	}
	void onRecordPauseError(MediaRecorder& mediaRecorder, recorder_error_t ret) override
	{
		notifyError();
	}
	void onRecordStopError(MediaRecorder& mediaRecorder, recorder_error_t ret) override
	{
		notifyError();
	}

	/** Wait until start() excuted, so if this function is not called, MediaRecorder.start() is failed
	  * so we do not add TC_ASSERT for start, pause, finished, error(observer) **/
	void waitStarted()
	{
		std::unique_lock<std::mutex> lock(mtx);
		cvStarted.wait(lock);
	}

	void waitPaused()
	{
		std::unique_lock<std::mutex> lock(mtx);
		cvPaused.wait(lock);
	}

	void waitFinished()
	{
		std::unique_lock<std::mutex> lock(mtx);
		cvFinished.wait(lock);
	}

	void waitError()
	{
		std::unique_lock<std::mutex> lock(mtx);
		cvError.wait(lock);
	}

	void notifyStarted()
	{
		cvStarted.notify_one();
	}

	void notifyPaused()
	{
		cvPaused.notify_one();
	}

	void notifyFinished()
	{
		cvFinished.notify_one();
	}

	void notifyError()
	{
		cvError.notify_one();
	}

private:
	std::mutex mtx;
	std::condition_variable cvStarted;
	std::condition_variable cvPaused;
	std::condition_variable cvFinished;
	std::condition_variable cvError;
};

static long get_file_size()
{
	struct stat st;
	long ret;

	ret = stat(filePath, &st);
	if (ret == OK) {
		printf("size : %d\n", st.st_size);
		return st.st_size;
	}
	return 0;
}

static void utc_media_MediaRecorder_setDuration_p(void)
{
	{
		long size;
		MediaRecorder mr;
		unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

		auto observer = std::make_shared<RecorderTest>();
		mr.create();
		mr.setObserver(observer);
		mr.setDataSource(std::move(dataSource));

		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setFileSize(0), RECORDER_OK);
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setFileSize(-1), RECORDER_OK);

		size = channels * sampleRate * RECORD_DURATION * 2;
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setFileSize(size), RECORDER_OK);
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.prepare(), RECORDER_OK);
		mr.start();
		sleep(RECORD_DURATION + 1);
		mr.stop();
		mr.unprepare();
		mr.destroy();

		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", get_file_size(), size);
	}

	{
		long size;
		MediaRecorder mr;
		unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

		auto observer = std::make_shared<RecorderTest>();
		mr.create();
		mr.setObserver(observer);
		mr.setDataSource(std::move(dataSource));

		/* setDuration with 0. It means infinite time */
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setDuration(0), RECORDER_OK);

		/* setDuration with positive number */
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setDuration(RECORD_DURATION), RECORDER_OK);
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.prepare(), RECORDER_OK);
		mr.start();
		sleep(RECORD_DURATION + 1);
		mr.stop();
		mr.unprepare();
		mr.destroy();

		size = channels * sampleRate * RECORD_DURATION;

		switch (pcmFormat) {
		case AUDIO_FORMAT_TYPE_S8:
			size *= 1;
			break;
		case AUDIO_FORMAT_TYPE_S16_LE:
			size *= 2;
			break;
		case AUDIO_FORMAT_TYPE_S32_LE:
			size *= 4;
			break;
		default:
			break;
		}
		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", get_file_size(), size);
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setDuration_n(void)
{
	{
		long size;
		MediaRecorder mr;
		unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
		auto observer = std::make_shared<RecorderTest>();

		size = channels * sampleRate * RECORD_DURATION * 2;
		TC_ASSERT_NEQ("utc_media_MediaRecorder_setDuration", mr.setFileSize(size), RECORDER_OK);
		mr.create();
		mr.setObserver(observer);
		mr.setDataSource(std::move(dataSource));

		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setFileSize(size), RECORDER_OK);
		mr.destroy();

		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", get_file_size(), size);
	}

	/* setDuration before create */
	{
		MediaRecorder mr;
		mr.setDuration(RECORD_DURATION);
	}

	/* setDuration at invalid state */
	{
		MediaRecorder mr;
		auto dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

		auto observer = std::make_shared<RecorderTest>();
		mr.create();
		mr.setObserver(observer);
		mr.setDataSource(std::move(dataSource));
		mr.prepare();

		TC_ASSERT_EQ("utc_media_MediaRecorder_setDuration", mr.setDuration(RECORD_DURATION), RECORDER_ERROR_INVALID_STATE);
		mr.start();
		observer->waitStarted();
		mr.stop();
		mr.unprepare();
		mr.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_start_p(void)
{
	constexpr int channels[] = {1, 2};
	constexpr int samprates[] = {41000, 32000, 20000, 16000, 14000, 8000, 6000};

	for (auto channel : channels) {
		for (auto samprate : samprates) {
			MediaRecorder mr;
			auto dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channel, samprate, pcmFormat, filePath));

			auto observer = std::make_shared<RecorderTest>();
			mr.create();
			mr.setObserver(observer);
			mr.setDataSource(std::move(dataSource));
			mr.prepare();

			mr.start();
			observer->waitStarted();
			mr.stop();
			mr.unprepare();
			mr.destroy();
		}
	}

	for (int i = 0; i < 2; ++i) {
		MediaRecorder mr;
		auto dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource( (i == 0 ? filePath_opus : filePath_wav) ));

		auto observer = std::make_shared<RecorderTest>();
		mr.create();
		mr.setObserver(observer);
		mr.setDataSource(std::move(dataSource));
		mr.prepare();

		mr.start();
		observer->waitStarted();
		mr.stop();
		mr.unprepare();
		mr.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_start_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.start();
	auto observer = std::make_shared<RecorderTest>();
	mr.create();
	mr.setObserver(observer);
	mr.setDataSource(std::move(dataSource));
	mr.start();
	observer->waitError();
	mr.stop();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_stop_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	auto observer = std::make_shared<RecorderTest>();
	mr.create();
	mr.setObserver(observer);
	mr.setDataSource(std::move(dataSource));
	mr.prepare();

	mr.start();
	mr.stop();
	observer->waitFinished();
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_stop_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.stop();
	auto observer = std::make_shared<RecorderTest>();
	mr.create();
	mr.setObserver(observer);
	mr.setDataSource(std::move(dataSource));
	mr.stop();
	observer->waitError();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_pause_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	auto observer = std::make_shared<RecorderTest>();
	mr.create();
	mr.setObserver(observer);
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	mr.start();
	mr.pause();
	observer->waitPaused();
	mr.start();
	mr.pause();
	observer->waitPaused();
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_pause_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.pause();
	auto observer = std::make_shared<RecorderTest>();
	mr.create();
	mr.setObserver(observer);
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	mr.pause();
	observer->waitError();
	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setObserver_p(void)
{
	MediaRecorder mr;
	auto observer = std::make_shared<RecorderTest>();
	mr.create();

	TC_ASSERT_EQ("utc_media_MediaRecorder_setObserver", mr.setObserver(observer), RECORDER_OK);

	/* setObserver twice */
	TC_ASSERT_EQ("utc_media_MediaRecorder_setObserver", mr.setObserver(observer), RECORDER_OK);

	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setObserver_n(void)
{
	MediaRecorder mr;

	TC_ASSERT_EQ("utc_media_MediaRecorder_setObserver", mr.setObserver(nullptr), RECORDER_ERROR_NOT_ALIVE);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_isRecording_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	mr.start();

	TC_ASSERT_EQ("utc_media_MediaRecorder_isRecording", mr.isRecording(), true);

	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_isRecording_n(void)
{
	/* isRecording before create */
	{
		MediaRecorder mr;

		TC_ASSERT_EQ("utc_media_MediaRecorder_isRecording", mr.isRecording(), false);
	}

	/* isRecording without start */
	{
		MediaRecorder mr;
		unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
		mr.create();
		mr.setDataSource(std::move(dataSource));
		mr.prepare();

		TC_ASSERT_EQ("utc_media_MediaRecorder_isRecording", mr.isRecording(), false);

		mr.unprepare();
		mr.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_operator_equal_p(void)
{
	MediaRecorder mr;
	bool isSame = mr == mr;
	
	TC_ASSERT("utc_media_MediaRecorder_operator_equal", isSame);
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_operator_equal_n(void)
{
	MediaRecorder mr1, mr2;
	bool isSame = mr1 == mr2;
	
	TC_ASSERT("utc_media_MediaRecorder_operator_equal", !isSame);	
	TC_SUCCESS_RESULT();
}

int utc_media_mediarecorder_main(void)
{
	utc_media_MediaRecorder_create_p();
	utc_media_MediaRecorder_create_n();

	utc_media_MediaRecorder_destroy_p();
	utc_media_MediaRecorder_destroy_n();

	utc_media_MediaRecorder_setDataSource_p();
	utc_media_MediaRecorder_setDataSource_n();

	utc_media_MediaRecorder_setVolume_p();
	utc_media_MediaRecorder_setVolume_n();

	utc_media_MediaRecorder_getVolume_p();
	utc_media_MediaRecorder_getVolume_n();

	utc_media_MediaRecorder_getMaxVolume_p();
	utc_media_MediaRecorder_getMaxVolume_n();

	utc_media_MediaRecorder_prepare_p();
	utc_media_MediaRecorder_prepare_n();

	utc_media_MediaRecorder_unprepare_p();
	utc_media_MediaRecorder_unprepare_n();

	utc_media_MediaRecorder_setDuration_p();
	utc_media_MediaRecorder_setDuration_n();

	utc_media_MediaRecorder_start_p();
	utc_media_MediaRecorder_start_n();

	utc_media_MediaRecorder_stop_p();
	utc_media_MediaRecorder_stop_n();

	utc_media_MediaRecorder_pause_p();
	utc_media_MediaRecorder_pause_n();

	utc_media_MediaRecorder_setObserver_p();
	utc_media_MediaRecorder_setObserver_n();

	utc_media_MediaRecorder_isRecording_p();
	utc_media_MediaRecorder_isRecording_n();

	utc_media_MediaRecorder_operator_equal_p();
	utc_media_MediaRecorder_operator_equal_n();
	return 0;
}
