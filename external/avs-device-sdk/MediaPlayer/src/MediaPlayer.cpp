/******************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <cmath>
#include <cstring>
#include <unordered_map>

#include <assert.h>
#include <tinyara/config.h>

#include <AVSCommon/AVS/Attachment/AttachmentReader.h>
#include <AVSCommon/AVS/SpeakerConstants/SpeakerConstants.h>
#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/Utils/Memory/Memory.h>
#include <PlaylistParser/PlaylistParser.h>
#include <PlaylistParser/UrlContentToAttachmentConverter.h>

#include "media/HttpInputDataSource.h"
#include "MediaPlayer/AttachmentReaderSource.h"
#include "MediaPlayer/IStreamSource.h"
#include "MediaPlayer/MediaPlayer.h"

#ifdef AUDIO_SIMPLEX_MODE
#include "SampleApp/VoiceRecorder.h"
using namespace alexaClientSDK::sampleApp;
#endif

namespace alexaClientSDK {
namespace mediaPlayer {

using namespace avsCommon::avs::attachment;
using namespace avsCommon::avs::speakerConstants;
using namespace avsCommon::sdkInterfaces;
using namespace avsCommon::utils;
using namespace avsCommon::utils::mediaPlayer;
using namespace avsCommon::utils::memory;
using namespace avsCommon::utils::configuration;
using namespace media;


/// A link to @c MediaPlayerInterface::ERROR.
static const MediaPlayer::SourceId ERROR_SOURCE_ID = MediaPlayer::ERROR;

std::mutex MediaPlayer::m_mutex;

/// String to identify log entries originating from this file.
static const std::string TAG("MediaPlayer");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

std::shared_ptr<MediaPlayer> MediaPlayer::create(
	std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
	SpeakerInterface::Type type,
	std::string name) {
	ACSDK_DEBUG8(LX(__FUNCTION__));
	std::shared_ptr<MediaPlayer> mediaPlayer(new MediaPlayer(contentFetcherFactory, type, name));
	if (mediaPlayer->init()) {
		return mediaPlayer;
	} else {
		return nullptr;
	}
};

MediaPlayer::~MediaPlayer() {
	ACSDK_DEBUG8(LX(__FUNCTION__));
	delSource();
}


MediaPlayer::SourceId MediaPlayer::setSource(
	std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader,
	const avsCommon::utils::AudioFormat* audioFormat) {
	ACSDK_DEBUG8(LX(__FUNCTION__).m("AttachmentReader source").d("name", name()));
	if (audioFormat) {
		ACSDK_DEBUG9(LX(__FUNCTION__).d("encoding", audioFormat->encoding)
									.d("sampleRateHz", audioFormat->sampleRateHz)
									.d("sampleSizeInBits", audioFormat->sampleSizeInBits)
									.d("numChannels", audioFormat->numChannels)
									.d("dataSigned", audioFormat->dataSigned));
	}
	std::lock_guard<std::mutex> lock(m_mutex);
	m_op = MediaPlayer::Operate::SETSOURCE;

	delSource();
	return newSource(std::move(AttachmentReaderSource::create(reader)));
}

MediaPlayer::SourceId MediaPlayer::setSource(std::shared_ptr<std::istream> stream, bool repeat) {
	ACSDK_DEBUG8(LX(__FUNCTION__).m("istream source").d("repeat", repeat).d("name", name()));
	std::lock_guard<std::mutex> lock(m_mutex);
	m_op = MediaPlayer::Operate::SETSOURCE;

	delSource();
	return newSource(std::move(IStreamSource::create(stream, repeat)));
}

MediaPlayer::SourceId MediaPlayer::setSource(const std::string& url, std::chrono::milliseconds offset) {
	ACSDK_DEBUG8(LX(__FUNCTION__).m("url source").d("url", url).d("offset", offset.count()).d("name", name()));
	std::lock_guard<std::mutex> lock(m_mutex);
	m_op = MediaPlayer::Operate::SETSOURCE;

	std::unique_ptr<media::stream::HttpInputDataSource> src(new media::stream::HttpInputDataSource(url));
	delSource();
	return newSource(std::move(src));
}

bool MediaPlayer::play(MediaPlayer::SourceId id) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("SourceId", id));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_id != id) {
		ACSDK_ERROR(LX(__FUNCTION__).m("id not match"));
		return false;
	}

	if (m_state != MediaPlayer::State::READY) {
		ACSDK_ERROR(LX(__FUNCTION__).d("invalid state", (int)m_state));
		return false;
	}

	m_op = MediaPlayer::Operate::PLAY;

	auto ret = m_mp.start();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("reason", "media::MediaPlayer start failed"));
		return false;
	}

	m_state = MediaPlayer::State::PLAYING;

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerActivated();
#endif

	return true;
}

bool MediaPlayer::stop(MediaPlayer::SourceId id) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("SourceId", id));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_id != id) {
		ACSDK_ERROR(LX(__FUNCTION__).d("id", "not match"));
		return false;
	}

	if ((m_state != MediaPlayer::State::PLAYING) && (m_state != MediaPlayer::State::PAUSED)) {
		ACSDK_ERROR(LX(__FUNCTION__).d("invalid state", (int)m_state));
		return false;
	}

	m_op = MediaPlayer::Operate::STOP;

	auto ret = m_mp.stop();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("reason", "media::MediaPlayer stop failed"));
		return false;
	}

	m_state = MediaPlayer::State::STOPPED;
	return true;
}

bool MediaPlayer::pause(MediaPlayer::SourceId id) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("SourceId", id));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_id != id) {
		ACSDK_ERROR(LX(__FUNCTION__).d("id", "not match"));
		return false;
	}

	if (m_state != MediaPlayer::MediaPlayer::State::PLAYING) {
		ACSDK_ERROR(LX(__FUNCTION__).d("state", "isn't PLAYING"));
		return false;
	}

	m_op = MediaPlayer::Operate::PAUSE;

	auto ret = m_mp.pause();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("reason", "media::MediaPlayer pause failed"));
		return false;
	}

	m_state = MediaPlayer::State::PAUSED;
	return true;
}

bool MediaPlayer::resume(MediaPlayer::SourceId id) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("SourceId", id));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_id != id) {
		ACSDK_ERROR(LX(__FUNCTION__).d("id", "not match"));
		return false;
	}

	if (m_state != MediaPlayer::State::PAUSED) {
		ACSDK_ERROR(LX(__FUNCTION__).d("state", "isn't PAUSED"));
		return false;
	}

	m_op = MediaPlayer::Operate::RESUME;

	auto ret = m_mp.start();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("reason", "media::MediaPlayer start failed"));
		return false;
	}

	m_state = MediaPlayer::State::PLAYING;

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerActivated();
#endif
	return true;
}

uint64_t MediaPlayer::getNumBytesBuffered() {
	ACSDK_DEBUG8(LX(__FUNCTION__));
	std::lock_guard<std::mutex> lock(m_mutex);
	// unused
	return 0;
}

std::chrono::milliseconds MediaPlayer::getOffset(MediaPlayer::SourceId id) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("SourceId", id));
	std::lock_guard<std::mutex> lock(m_mutex);
	return std::chrono::milliseconds::zero();
}

void MediaPlayer::setObserver(std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> observer) {
	ACSDK_DEBUG8(LX(__FUNCTION__));
	std::lock_guard<std::mutex> lock(m_mutex);
	m_playerObserver = observer;
}

void MediaPlayer::onError()
{
	ACSDK_ERROR(LX(__FUNCTION__));
}

void MediaPlayer::doShutdown()
{
	ACSDK_DEBUG8(LX(__FUNCTION__));
}

bool MediaPlayer::setVolume(int8_t volume) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("volume", (int)volume));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (volume > AVS_SET_VOLUME_MAX ||
		volume < AVS_SET_VOLUME_MIN) {
		ACSDK_ERROR(LX(__FUNCTION__).d("invalid volume value", volume));
		return false;
	}

	m_speakerSettings.volume = volume;

	if (m_state != MediaPlayer::State::UNKNOWN) {
		auto ret = m_mp.setVolume(toLocalVolume(m_speakerSettings.volume));
		return (ret == media::PLAYER_OK);
	}
	return true;
}

bool MediaPlayer::adjustVolume(int8_t delta) {
	ACSDK_ERROR(LX(__FUNCTION__).d("name", name()).d("delta", (int)delta));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (delta > AVS_ADJUST_VOLUME_MAX || delta < AVS_ADJUST_VOLUME_MIN) {
		ACSDK_ERROR(LX(__FUNCTION__).d("invalid delta value", (int)delta));
		return false;
	}

	int newVolume = m_speakerSettings.volume + delta;
	newVolume = std::min(newVolume, (int)AVS_SET_VOLUME_MAX);
	newVolume = std::max(newVolume, (int)AVS_SET_VOLUME_MIN);

	m_speakerSettings.volume = newVolume;

	if (m_state != MediaPlayer::State::UNKNOWN) {
		auto ret = m_mp.setVolume(toLocalVolume(m_speakerSettings.volume));
		return (ret == media::PLAYER_OK);
	}
	return true;
}

bool MediaPlayer::setMute(bool mute) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("mute", mute));
	std::lock_guard<std::mutex> lock(m_mutex);

	m_speakerSettings.mute = mute;

	if (m_state != MediaPlayer::State::UNKNOWN) {
		int8_t volume = mute ? AVS_SET_VOLUME_MIN : m_speakerSettings.volume;
		auto ret = m_mp.setVolume(toLocalVolume(volume));
		return (ret == media::PLAYER_OK);
	}
	return true;
}

bool MediaPlayer::getSpeakerSettings(SpeakerInterface::SpeakerSettings* settings) {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()));
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!settings) {
		ACSDK_ERROR(LX(__FUNCTION__).d("Failed", "nullSettings"));
		return false;
	}

	settings->mute = m_speakerSettings.mute;
	settings->volume = m_speakerSettings.volume;
	ACSDK_DEBUG8(LX(__FUNCTION__).d("mute", settings->mute).d("volume", (int)settings->volume));
	return true;
}

SpeakerInterface::Type MediaPlayer::getSpeakerType() {
	ACSDK_DEBUG8(LX(__FUNCTION__).d("name", name()).d("m_speakerType", (int)m_speakerType));
	return m_speakerType;
}

uint8_t MediaPlayer::toLocalVolume(int8_t volume)
{
	uint8_t LOCAL_VOLUME_MAX = 10;
	m_mp.getMaxVolume(&LOCAL_VOLUME_MAX);
	return (volume * LOCAL_VOLUME_MAX / AVS_SET_VOLUME_MAX);
}

int8_t MediaPlayer::fromLocalVolume(uint8_t volume)
{
	uint8_t LOCAL_VOLUME_MAX = 10;
	m_mp.getMaxVolume(&LOCAL_VOLUME_MAX);
	return (volume * AVS_SET_VOLUME_MAX / LOCAL_VOLUME_MAX);
}

void MediaPlayer::onPlaybackStarted(media::MediaPlayer &mediaPlayer)
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));
	if (m_playerObserver) {
		if (m_op == MediaPlayer::Operate::RESUME) {
			ACSDK_DEBUG8(LX("==> call onPlaybackResumed"));
			m_playerObserver->onPlaybackResumed(m_id);
		} else {
			if (m_op != MediaPlayer::Operate::PLAY) {
				ACSDK_WARN(LX("onPlaybackStarted exception").d("m_op", (int)m_op).d("m_state", (int)m_state));
			}

			ACSDK_DEBUG8(LX("==> call onPlaybackStarted"));
			m_playerObserver->onPlaybackStarted(m_id);
		}

		m_op = MediaPlayer::Operate::NONE;
	}
}

void MediaPlayer::onPlaybackFinished(media::MediaPlayer &mediaPlayer)
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id).d("name", name()));

	{
		std::lock_guard<std::mutex> lock(m_mutex);
		ACSDK_DEBUG8(LX("onPlaybackFinishedCalled -unprepare start"));
		mediaPlayer.unprepare();
		m_state = MediaPlayer::State::IDLE;
		ACSDK_DEBUG8(LX("onPlaybackFinishedCalled -unprepare done"));
	}

	if (m_playerObserver) {
		m_playerObserver->onPlaybackFinished(m_id);
		if (m_op == MediaPlayer::Operate::PLAY ||
			m_op == MediaPlayer::Operate::RESUME) {
			m_op = MediaPlayer::Operate::NONE;
		}
	}

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerDeactivated();
#endif
}

void MediaPlayer::onPlaybackPaused(media::MediaPlayer &mediaPlayer)
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));
	if (m_playerObserver) {
		m_playerObserver->onPlaybackPaused(m_id);
	}

	if (m_op == MediaPlayer::Operate::PAUSE) {
		m_op = MediaPlayer::Operate::NONE;
	} else {
		ACSDK_WARN(LX("onPlaybackPaused exception").d("m_op", (int)m_op).d("m_state", (int)m_state));
	}

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerDeactivated();
#endif
}

void MediaPlayer::onPlaybackStopped(media::MediaPlayer &mediaPlayer)
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));

	if (m_op == MediaPlayer::Operate::STOP) {
		m_op = MediaPlayer::Operate::NONE;
		if (m_playerObserver) {
			m_playerObserver->onPlaybackStopped(m_id);
		}
	} else {
		ACSDK_WARN(LX("onPlaybackStopped exception").d("m_op", (int)m_op).d("m_state", (int)m_state));
	}

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerDeactivated();
#endif
}

void MediaPlayer::onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));
	if (m_playerObserver) {
		m_playerObserver->onPlaybackError(m_id, ErrorType::MEDIA_ERROR_UNKNOWN, "MEDIA_ERROR_UNKNOWN");
	}

	m_op = MediaPlayer::Operate::NONE;

#ifdef AUDIO_SIMPLEX_MODE
	onPlayerDeactivated();
#endif
}

void MediaPlayer::onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
	ACSDK_ERROR(LX(__FUNCTION__).d("sourceId", m_id));
}

void MediaPlayer::onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
	ACSDK_ERROR(LX(__FUNCTION__).d("sourceId", m_id));
}

void MediaPlayer::onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error)
{
	ACSDK_ERROR(LX(__FUNCTION__).d("sourceId", m_id));
}

bool MediaPlayer::delSource()
{
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));
	if (m_id != ERROR_SOURCE_ID && m_state != MediaPlayer::State::UNKNOWN) {
		if (m_state == MediaPlayer::State::PLAYING ||
			m_state == MediaPlayer::State::PAUSED) {
			m_mp.stop();
			m_state = MediaPlayer::State::STOPPED;
		}

		if (m_state == MediaPlayer::State::READY ||
			m_state == MediaPlayer::State::STOPPED) {
			m_mp.unprepare();
			m_state = MediaPlayer::State::IDLE;
		}

		if (m_state == MediaPlayer::State::IDLE) {
			m_mp.destroy();
			m_state = MediaPlayer::State::UNKNOWN;
		}

		m_id = ERROR_SOURCE_ID;
	}

	return true;
}

MediaPlayer::SourceId MediaPlayer::newSource(std::unique_ptr<media::stream::InputDataSource> source)
{
	ACSDK_DEBUG8(LX(__FUNCTION__));

	media::player_result_t ret;

	ret = m_mp.create();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("create", "failed"));
		return ERROR_SOURCE_ID;
	}

	ret = m_mp.setDataSource(std::move(source));
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("setDataSource", "failed"));
		m_mp.destroy();
		return ERROR_SOURCE_ID;
	}

	ret = m_mp.setObserver(shared_from_this());
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("setObserver", "failed"));
		m_mp.destroy();
		return ERROR_SOURCE_ID;
	}

	ret = m_mp.prepare();
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_ERROR(LX(__FUNCTION__).d("prepare", "failed"));
		m_mp.destroy();
		return ERROR_SOURCE_ID;
	}

	ret = m_mp.setVolume(toLocalVolume(m_speakerSettings.volume));
	if (ret != media::PLAYER_ERROR_NONE) {
		ACSDK_WARN(LX(__FUNCTION__).d("setVolume", "failed"));
	}

	m_state = MediaPlayer::State::READY;

	static SourceId s_sourceId = ERROR_SOURCE_ID + 1;
	if (s_sourceId == ERROR_SOURCE_ID) {
		s_sourceId = ERROR_SOURCE_ID + 1;
	}

	m_id = s_sourceId++;
	ACSDK_DEBUG8(LX(__FUNCTION__).d("sourceId", m_id));
	return m_id;
}

bool MediaPlayer::init() {
	m_id = ERROR_SOURCE_ID;
	m_speakerSettings.volume = avsCommon::avs::speakerConstants::AVS_VOLUME_DEFAULT;
	m_speakerSettings.mute = false;

	return true;
}

MediaPlayer::MediaPlayer(
	std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
	SpeakerInterface::Type type,
	std::string name) :
		RequiresShutdown(name),
		m_state(MediaPlayer::State::UNKNOWN),
		m_op(MediaPlayer::Operate::NONE),
		m_id(ERROR_SOURCE_ID),
		m_contentFetcherFactory(contentFetcherFactory),
		m_speakerType(type)
{
}

//-------------------------------------------------------

#ifdef AUDIO_SIMPLEX_MODE

bool MediaPlayer::sm_bPlayerActived = false;

bool MediaPlayer::isPlayerActive()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return sm_bPlayerActived;
}

void MediaPlayer::onPlayerActivated()
{
	sm_bPlayerActived = true;
}

void MediaPlayer::onPlayerDeactivated()
{
	sm_bPlayerActived = false;

	if (VoiceRecorder::isRecordingExpected()) {
		ACSDK_DEBUG9(LX(__FUNCTION__).m("startRecorder..."));
		VoiceRecorder::startRecorder();
	}
}
#endif

}  // namespace mediaPlayer
}  // namespace alexaClientSDK
