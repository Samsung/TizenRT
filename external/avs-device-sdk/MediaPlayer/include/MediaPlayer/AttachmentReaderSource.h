#ifndef ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ATTACHMENTREADERSOURCE_H_
#define ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ATTACHMENTREADERSOURCE_H_

#include <iostream>
#include <memory>

#include <MediaPlayer/SourceInterface.h>

namespace alexaClientSDK {
namespace mediaPlayer {

class AttachmentReaderSource : public SourceInterface {
public:
    static std::unique_ptr<AttachmentReaderSource> create(
        std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader);

    ~AttachmentReaderSource() override;

private:
    AttachmentReaderSource(std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> reader);

    bool init();

    // SourceInterface
    virtual size_t readData(uint16_t *buffer, size_t size) override;

    // RequiresShutdown
    void doShutdown() override {};

private:
    std::shared_ptr<avsCommon::avs::attachment::AttachmentReader> m_reader;
    bool m_repeat;
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ISTREAMSOURCE_H_
