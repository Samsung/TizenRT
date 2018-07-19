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

#include <media/MediaRecorder.h>
#include <media/MediaRecorderObserverInterface.h>
#include <media/FileOutputDataSource.h>
#include "tc_common.h"

using namespace std;
using namespace media;
using namespace media::stream;

static unsigned int channels = 2;
static unsigned int sampleRate = 16000;
static audio_format_type_t pcmFormat = AUDIO_FORMAT_TYPE_S16_LE;
static const char *filePath = "/tmp/record";

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

	TC_ASSERT_EQ("utc_media_mediarecorder_create", mr.create(), RECORDER_ERROR);

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
	TC_ASSERT_EQ("utc_media_MediaRecorder_destroy", mr.destroy(), RECORDER_ERROR);
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_MediaRecorder_destroy", mr.destroy(), RECORDER_ERROR);
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
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr.setDataSource(std::move(nullptr)), RECORDER_ERROR);
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr2.setDataSource(std::move(dataSource)), RECORDER_ERROR);
	mr.destroy();

	dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_mediarecorder_setDataSource", mr.setDataSource(std::move(dataSource)), RECORDER_ERROR);
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setVolume_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_mediarecorder_setVolume", mr.setVolume(10), RECORDER_OK);
	mr.unprepare();
	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setVolume_n(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));
	TC_ASSERT_EQ("utc_media_mediarecorder_setVolume", mr.setVolume(10), RECORDER_ERROR);
	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	TC_ASSERT_EQ("utc_media_mediarecorder_setVolume", mr.setVolume(11), RECORDER_ERROR);
	mr.unprepare();
	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_getVolume_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

	mr.create();
	mr.setDataSource(std::move(dataSource));
	mr.prepare();
	for (int i = 0; i <= 10; ++i)
	{
		mr.setVolume(i);
		TC_ASSERT_EQ("utc_media_MediaRecorder_getVolume", mr.getVolume(), i);
	}

	mr.unprepare();
	mr.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_getVolume_n(void)
{
	MediaRecorder mr;
	TC_ASSERT_EQ("utc_media_MediaRecorder_getVolume", mr.getVolume(), -1);

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
	TC_ASSERT_EQ("utc_media_MediaRecorder_prepare", mr.prepare(), RECORDER_ERROR);
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
	TC_ASSERT_EQ("utc_media_MediaRecorder_unprepare", mr.unprepare(), RECORDER_ERROR);
	mr.destroy();

	TC_SUCCESS_RESULT();
}

class RecorderTest : public MediaRecorderObserverInterface
{
public:
	RecorderTest() {}
	~RecorderTest() {}

	void onRecordStarted(Id id) override
	{
		notifyStarted();
	}
	void onRecordPaused(Id id) override
	{
		notifyPaused();
	}
	void onRecordFinished(Id id) override
	{
		notifyFinished();
	}
	void onRecordError(Id id) override
	{
		notifyError();
	}

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

static void utc_media_MediaRecorder_start_p(void)
{
	MediaRecorder mr;
	unique_ptr<FileOutputDataSource> dataSource = unique_ptr<FileOutputDataSource>(new FileOutputDataSource(channels, sampleRate, pcmFormat, filePath));

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
	mr.create();

	TC_ASSERT_EQ("utc_media_MediaRecorder_setObserver", mr.setObserver(nullptr), RECORDER_OK);

	mr.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaRecorder_setObserver_n(void)
{
	MediaRecorder mr;

	TC_ASSERT_EQ("utc_media_MediaRecorder_setObserver", mr.setObserver(nullptr), RECORDER_ERROR);

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
	
	utc_media_MediaRecorder_prepare_p();
	utc_media_MediaRecorder_prepare_n();

	utc_media_MediaRecorder_unprepare_p();
	utc_media_MediaRecorder_unprepare_n();

	utc_media_MediaRecorder_start_p();
	utc_media_MediaRecorder_start_n();

	utc_media_MediaRecorder_stop_p();
	utc_media_MediaRecorder_stop_n();

	utc_media_MediaRecorder_pause_p();
	utc_media_MediaRecorder_pause_n();

	utc_media_MediaRecorder_setObserver_p();
	utc_media_MediaRecorder_setObserver_n();

	return 0;
}
