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

/*
 * MediaPlayer.h
 *
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#ifndef ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_MEDIAPLAYER_H_
#define ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_MEDIAPLAYER_H_

#include <AVSCommon/SDKInterfaces/SpeakerInterface.h>
#include <AVSCommon/Utils/MediaPlayer/MediaPlayerInterface.h>
#include <AVSCommon/Utils/MediaPlayer/MediaPlayerObserverInterface.h>
#include <AVSCommon/Utils/RequiresShutdown.h>
#include <PlaylistParser/include/PlaylistParser/UrlContentToAttachmentConverter.h>
#include "MediaPlayer/SourceInterface.h"

#include <media/MediaPlayer.h>

namespace alexaClientSDK {
namespace mediaPlayer {

typedef std::vector<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface::TagKeyValueType> VectorOfTags;

class MediaPlayer
		: public avsCommon::utils::mediaPlayer::MediaPlayerInterface
		, public avsCommon::sdkInterfaces::SpeakerInterface
		, public playlistParser::UrlContentToAttachmentConverter::ErrorObserverInterface
		, public media::MediaPlayerObserverInterface
		, public avsCommon::utils::RequiresShutdown
		, public std::enable_shared_from_this<MediaPlayer> {
public:

	static std::shared_ptr<MediaPlayer> create(
		std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory = nullptr,
		avsCommon::sdkInterfaces::SpeakerInterface::Type type = avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED,
		std::string name = "");

	~MediaPlayer();

public: // MediaPlayerInterface
	virtual SourceId setSource(std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> attachmentReader, const avsCommon::utils::AudioFormat* audioFormat) override;
	virtual SourceId setSource(const std::string& url, std::chrono::milliseconds offset = std::chrono::milliseconds::zero()) override;
	virtual SourceId setSource(std::shared_ptr<std::istream> stream, bool repeat) override;
	virtual bool play(SourceId id) override;
	virtual bool stop(SourceId id) override;
	virtual bool pause(SourceId id) override;
	virtual bool resume(SourceId id) override;
	uint64_t getNumBytesBuffered() override;
	virtual std::chrono::milliseconds getOffset(SourceId id) override;
	virtual void setObserver(std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> playerObserver) override;

public: // ErrorObserverInterface
	void onError() override;

public: // RequiresShutdown
	void doShutdown() override;

public: // SpeakerInterface
	bool setVolume(int8_t volume) override;
	bool adjustVolume(int8_t volume) override;
	bool setMute(bool mute) override;
	bool getSpeakerSettings(avsCommon::sdkInterfaces::SpeakerInterface::SpeakerSettings* settings) override;
	avsCommon::sdkInterfaces::SpeakerInterface::Type getSpeakerType() override;

public: // media::MediaPlayerObserverInterface
	virtual void onPlaybackStarted(media::MediaPlayer &mediaPlayer) override;
	virtual void onPlaybackFinished(media::MediaPlayer &mediaPlayer) override;
	virtual void onPlaybackPaused(media::MediaPlayer &mediaPlayer) override;
	virtual void onPlaybackStopped(media::MediaPlayer &mediaPlayer) override;
	virtual void onPlaybackError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	virtual void onStartError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	virtual void onStopError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
	virtual void onPauseError(media::MediaPlayer &mediaPlayer, media::player_error_t error) override;
private:
	/**
	 * Constructor.
	 */
	MediaPlayer(
		std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory,
		avsCommon::sdkInterfaces::SpeakerInterface::Type type,
		std::string name);

	bool init();
	bool delSource();
	SourceId newSource(std::unique_ptr<media::stream::InputDataSource> source);
	uint8_t toLocalVolume(int8_t volume);
	int8_t fromLocalVolume(uint8_t volume);

	enum class State {
		UNKNOWN,
		IDLE,
		READY,
		PLAYING,
		PAUSED,
		STOPPED,
	};

	enum class Operate {
		NONE,
		PLAY,
		STOP,
		PAUSE,
		RESUME,
		SETSOURCE,
	};

	State m_state;
	Operate m_op;
	SourceId m_id;
	media::MediaPlayer m_mp;
	std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> m_playerObserver;
	// Used to create objects that can fetch remote HTTP content.
	std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> m_contentFetcherFactory;
	avsCommon::sdkInterfaces::SpeakerInterface::Type m_speakerType;
	avsCommon::sdkInterfaces::SpeakerInterface::SpeakerSettings m_speakerSettings;
	static std::mutex m_mutex;

#ifdef AUDIO_SIMPLEX_MODE
public:
	static bool sm_bPlayerActived;
	static bool isPlayerActive();
	static void onPlayerActivated();
	static void onPlayerDeactivated();
#endif
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_MEDIAPLAYER_H_
