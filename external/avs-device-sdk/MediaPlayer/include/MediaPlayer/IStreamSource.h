#ifndef ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ISTREAMSOURCE_H_
#define ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ISTREAMSOURCE_H_

#include <iostream>
#include <memory>

#include <MediaPlayer/SourceInterface.h>

namespace alexaClientSDK {
namespace mediaPlayer {

class IStreamSource : public SourceInterface {
public:
    static std::unique_ptr<IStreamSource> create(
        std::shared_ptr<std::istream> stream,
        bool repeat);

    ~IStreamSource() override;

private:
    IStreamSource(std::shared_ptr<std::istream> stream, bool repeat);

    bool init();

    // SourceInterface
    virtual size_t readData(uint16_t *buffer, size_t size) override;

    // RequiresShutdown
    void doShutdown() override {};

private:
    std::shared_ptr<std::istream> m_stream;
    bool m_repeat;
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_ISTREAMSOURCE_H_
