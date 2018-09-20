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

#include <memory>
#include <string.h>
#include <media/MediaPlayer.h>
#include <media/FileInputDataSource.h>
#include "tc_common.h"

static const char *dummyfilepath = "/mnt/fileinputdatasource.raw";

class EmptyObserver : public media::MediaPlayerObserverInterface
{
public:
	void onPlaybackStarted(media::MediaPlayer &mediaPlayer) override;
	void onPlaybackFinished(media::MediaPlayer &mediaPlayer) override;
	void onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	void onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	void onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	void onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	void onPlaybackPaused(media::MediaPlayer &mediaPlayer) override;
};

void EmptyObserver::onPlaybackStarted(media::MediaPlayer &mediaPlayer)
{
}

void EmptyObserver::onPlaybackFinished(media::MediaPlayer &mediaPlayer)
{
}

void EmptyObserver::onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
}

void EmptyObserver::onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
}

void EmptyObserver::onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
}

void EmptyObserver::onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
}

void EmptyObserver::onPlaybackPaused(media::MediaPlayer &mediaPlayer)
{
}

static void SetUp(void)
{
	FILE *fp = fopen(dummyfilepath, "w");

	if (fp != NULL) {
		int ret = fputs("dummydata", fp);
		if (ret != (int)strlen("dummydata")) {
			printf("fail to fputs\n");
		}
		fclose(fp);
	} else {
		printf("fail to open %s, errno : %d\n", dummyfilepath, get_errno());
	}
}

static void TearDown()
{
	int ret = remove(dummyfilepath);
	if (ret != 0) {
		printf("fail to remove %s, errno : %d\n", dummyfilepath, get_errno());
	}
}

static void utc_media_MediaPlayer_create_p(void)
{
	media::MediaPlayer mp;
	TC_ASSERT_EQ("utc_media_MediaPlayer_create", mp.create(), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_create_n(void)
{
	media::MediaPlayer mp;
	mp.create();

	TC_ASSERT_NEQ("utc_media_MediaPlayer_create", mp.create(), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_destroy_p(void)
{
	media::MediaPlayer mp;
	auto observer = std::make_shared<EmptyObserver>();
	mp.create();
	mp.setObserver(observer);

	TC_ASSERT_EQ("utc_media_MediaPlayer_destroy", mp.destroy(), media::PLAYER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_destroy_n(void)
{
	/* destroy without create */
	{
		media::MediaPlayer mp;

		TC_ASSERT_NEQ("utc_media_MediaPlayer_destroy", mp.destroy(), media::PLAYER_OK);
	}

	/* destroy At invalid time */
	{
		media::MediaPlayer mp;
		std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
		mp.create();
		mp.setDataSource(std::move(source));
		mp.prepare();

		TC_ASSERT_NEQ("utc_media_MediaPlayer_destroy", mp.destroy(), media::PLAYER_OK);

		mp.unprepare();
		mp.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setDataSource_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	source->setSampleRate(20000);
	source->setChannels(2);
	mp.create();

	TC_ASSERT_EQ("utc_media_MediaPlayer_setDataSource", mp.setDataSource(std::move(source)), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setDataSource_n(void)
{
	/* setDataSource without create */
	{
		media::MediaPlayer mp;
		std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
		source->setSampleRate(20000);
		source->setChannels(2);

		TC_ASSERT_NEQ("setDataSource", mp.setDataSource(std::move(source)), media::PLAYER_OK);
	}

	/* setDataSource with nullptr */
	{
		media::MediaPlayer mp;
		mp.create();

		TC_ASSERT_NEQ("utc_media_MediaPlayer_setDataSource", mp.setDataSource(nullptr), media::PLAYER_OK);

		mp.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setObserver_p(void)
{
	media::MediaPlayer mp;
	auto observer = std::make_shared<EmptyObserver>();
	mp.create();

	TC_ASSERT_EQ("utc_media_MediaPlayer_setObserver", mp.setObserver(observer), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setObserver_n(void)
{
	media::MediaPlayer mp;

	TC_ASSERT_NEQ("utc_media_MediaPlayer_setObserver", mp.setObserver(nullptr), media::PLAYER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_prepare_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	mp.create();
	mp.setDataSource(std::move(source));

	TC_ASSERT_EQ("utc_media_MediaPlayer_prepare", mp.prepare(), media::PLAYER_OK);

	mp.unprepare();
	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_prepare_n(void)
{
	/* prepare without create */
	{
		media::MediaPlayer mp;

		TC_ASSERT_NEQ("utc_media_MediaPlayer_prepare", mp.prepare(), media::PLAYER_OK);
	}

	/* prepare twice */
	{
		media::MediaPlayer mp;
		std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
		mp.create();
		mp.setDataSource(std::move(source));
		mp.prepare();

		TC_ASSERT_NEQ("utc_media_MediaPlayer_prepare", mp.prepare(), media::PLAYER_OK);

		mp.unprepare();
		mp.destroy();
	}

	/* prepare without setting datasource */
	{
		media::MediaPlayer mp;
		mp.create();

		TC_ASSERT_NEQ("utc_media_MediaPlayer_prepare", mp.prepare(), media::PLAYER_OK);

		mp.destroy();
	}

	/* prepare with invalid datasource */
	{
		media::MediaPlayer mp;
		std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource("non-exist-file")));
		mp.create();
		mp.setDataSource(std::move(source));

		TC_ASSERT_NEQ("utc_media_MediaPlayer_prepare", mp.prepare(), media::PLAYER_OK);

		mp.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_unprepare_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	mp.create();
	mp.setDataSource(std::move(source));
	mp.prepare();

	TC_ASSERT_EQ("utc_media_MediaPlayer_unprepare", mp.unprepare(), media::PLAYER_OK);

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_unprepare_n(void)
{
	/* unprepare without create */
	{
		media::MediaPlayer mp;

		TC_ASSERT_NEQ("utc_media_MediaPlayer_unprepare", mp.unprepare(), media::PLAYER_OK);
	}

	/* unprepare twice */
	{
		media::MediaPlayer mp;
		std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
		mp.create();
		mp.setDataSource(std::move(source));
		mp.unprepare();

		TC_ASSERT_NEQ("utc_media_MediaPlayer_unprepare", mp.unprepare(), media::PLAYER_OK);

		mp.unprepare();
		mp.destroy();
	}

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_start_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	mp.create();
	mp.setDataSource(std::move(source));
	mp.prepare();

	TC_ASSERT_EQ("utc_media_MediaPlayer_start", mp.start(), media::PLAYER_OK);

	mp.unprepare();
	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_start_n(void)
{
	media::MediaPlayer mp;

	TC_ASSERT_NEQ("utc_media_MediaPlayer_start", mp.start(), media::PLAYER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_pause_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	mp.create();
	mp.setDataSource(std::move(source));
	mp.prepare();
	mp.start();

	TC_ASSERT_EQ("utc_media_MediaPlayer_pause", mp.pause(), media::PLAYER_OK);

	mp.unprepare();
	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_pause_n(void)
{
	media::MediaPlayer mp;

	TC_ASSERT_NEQ("utc_media_MediaPlayer_pause", mp.pause(), media::PLAYER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_stop_p(void)
{
	media::MediaPlayer mp;
	std::unique_ptr<media::stream::FileInputDataSource> source = std::move(std::unique_ptr<media::stream::FileInputDataSource>(new media::stream::FileInputDataSource(dummyfilepath)));
	mp.create();
	mp.setDataSource(std::move(source));
	mp.prepare();
	mp.start();

	TC_ASSERT_EQ("utc_media_MediaPlayer_stop", mp.stop(), media::PLAYER_OK);

	mp.unprepare();
	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_stop_n(void)
{
	media::MediaPlayer mp;

	TC_ASSERT_NEQ("utc_media_MediaPlayer_stop", mp.stop(), media::PLAYER_OK);

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_getVolume_p(void)
{
	uint8_t volume;
	media::MediaPlayer mp;

	mp.create();

	for (int i = 0; i <= 10; ++i) {
		if (mp.setVolume(i) == media::PLAYER_ERROR_DEVICE_NOT_SUPPORTED) {
			mp.getVolume(&volume);
			TC_ASSERT_NEQ_CLEANUP("utc_media_MediaPlayer_getVolume", volume, 0, mp.destroy());
			break;
		} else {
			mp.setVolume(i);
			mp.getVolume(&volume);
			TC_ASSERT_EQ_CLEANUP("utc_media_MediaPlayer_getVolume", volume, i, mp.destroy());
		}
	}

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_getVolume_n(void)
{
	media::MediaPlayer mp;
	uint8_t volume;

	TC_ASSERT_EQ("utc_media_MediaPlayer_getVolume", mp.getVolume(&volume), media::PLAYER_ERROR_NOT_ALIVE);
	mp.create();
	TC_ASSERT_EQ_CLEANUP("utc_media_MediaPlayer_getVolume", mp.getVolume(nullptr), media::PLAYER_ERROR_INVALID_PARAMETER, mp.destroy());
	mp.destroy();

	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_getMaxVolume_p(void)
{
	uint8_t volume;
	media::MediaPlayer mp;

	mp.create();

	mp.getMaxVolume(&volume);
	TC_ASSERT_EQ_CLEANUP("utc_media_MediaPlayer_getMaxVolume", volume, 10, mp.destroy());

	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setVolume_p(void)
{
	uint8_t prev, volume;
	media::MediaPlayer mp;
	mp.create();
	mp.getVolume(&prev);

  if (mp.setVolume(prev) == media::PLAYER_ERROR_DEVICE_NOT_SUPPORTED) {
		printf("device does not support volume control\n");
		TC_ASSERT_NEQ_CLEANUP("utc_media_mediaPlayer_setVolume", mp.setVolume(prev + 1), media::PLAYER_OK, mp.destroy());
	} else {
		TC_ASSERT_EQ_CLEANUP("utc_media_mediaPlayer_setVolume", mp.setVolume(10), media::PLAYER_OK, mp.destroy());
		mp.getVolume(&volume);
		TC_ASSERT_EQ_CLEANUP("utc_media_mediaPlayer_setVolume", volume, 10, mp.destroy());
	}
	mp.destroy();
	TC_SUCCESS_RESULT();
}

static void utc_media_MediaPlayer_setVolume_n(void)
{
	uint8_t prev, volume;
	media::MediaPlayer mp;
	mp.create();
	mp.getVolume(&prev);

	if (mp.setVolume(prev) == media::PLAYER_ERROR_DEVICE_NOT_SUPPORTED) {
		mp.setVolume(prev + 1);
		mp.getVolume(&volume);
		TC_ASSERT_NEQ_CLEANUP("utc_media_mediaPlayer_setVolume", prev + 1, volume, mp.destroy());		
	} else {
		TC_ASSERT_EQ_CLEANUP("utc_media_mediaPlayer_setVolume", mp.setVolume(11), media::PLAYER_OK, mp.destroy());
	}
	mp.destroy();
	TC_SUCCESS_RESULT();
}

int utc_media_MediaPlayer_main(void)
{
	SetUp();

	utc_media_MediaPlayer_create_p();
	utc_media_MediaPlayer_create_n();

	utc_media_MediaPlayer_destroy_p();
	utc_media_MediaPlayer_destroy_n();

	utc_media_MediaPlayer_setDataSource_p();
	utc_media_MediaPlayer_setDataSource_n();

	utc_media_MediaPlayer_setObserver_p();
	utc_media_MediaPlayer_setObserver_n();

	utc_media_MediaPlayer_prepare_p();
	utc_media_MediaPlayer_prepare_n();

	utc_media_MediaPlayer_unprepare_p();
	utc_media_MediaPlayer_unprepare_n();

	utc_media_MediaPlayer_start_p();
	utc_media_MediaPlayer_start_n();

	utc_media_MediaPlayer_pause_p();
	utc_media_MediaPlayer_pause_n();

	utc_media_MediaPlayer_stop_p();
	utc_media_MediaPlayer_stop_n();

	utc_media_MediaPlayer_getVolume_p();
	utc_media_MediaPlayer_getVolume_n();

	utc_media_MediaPlayer_getMaxVolume_p();

	utc_media_MediaPlayer_setVolume_p();
	utc_media_MediaPlayer_setVolume_n();

	TearDown();

	return 0;
}
