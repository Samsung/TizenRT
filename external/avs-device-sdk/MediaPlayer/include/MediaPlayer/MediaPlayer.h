/*
 * MediaPlayer.h
 *
 * Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

namespace alexaClientSDK {
namespace mediaPlayer {

typedef std::vector<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface::TagKeyValueType> VectorOfTags;

class MediaPlayer
        : public avsCommon::utils::mediaPlayer::MediaPlayerInterface
        , public avsCommon::sdkInterfaces::SpeakerInterface
        , public playlistParser::UrlContentToAttachmentConverter::ErrorObserverInterface
        , public avsCommon::utils::RequiresShutdown
        , public std::enable_shared_from_this<MediaPlayer> {
public:

    static std::shared_ptr<MediaPlayer> create(
        std::shared_ptr<avsCommon::sdkInterfaces::HTTPContentFetcherInterfaceFactoryInterface> contentFetcherFactory =
            nullptr,
        avsCommon::sdkInterfaces::SpeakerInterface::Type type =
            avsCommon::sdkInterfaces::SpeakerInterface::Type::AVS_SYNCED,
        std::string name = ""){ return nullptr; }

    ~MediaPlayer() { }

public: // MediaPlayerInterface
    virtual SourceId setSource(std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> attachmentReader) { return 0; }
    virtual SourceId setSource(
        const std::string& url,
        std::chrono::milliseconds offset = std::chrono::milliseconds::zero()) { return 0; }
    virtual SourceId setSource(std::shared_ptr<std::istream> stream, bool repeat) { return 0; }
    virtual bool play(SourceId id) { return false; }
    virtual bool stop(SourceId id) { return false; }
    virtual bool pause(SourceId id) { return false; }
    virtual bool resume(SourceId id) { return false; }
    virtual std::chrono::milliseconds getOffset(SourceId id) { return std::chrono::milliseconds::zero(); }
    virtual void setObserver(
        std::shared_ptr<avsCommon::utils::mediaPlayer::MediaPlayerObserverInterface> playerObserver) { return; }

    void doShutdown() override { }

private:
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_MEDIAPLAYER_H_
