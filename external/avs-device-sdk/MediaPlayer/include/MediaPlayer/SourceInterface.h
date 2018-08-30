#ifndef ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_
#define ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_

#include <AVSCommon/Utils/RequiresShutdown.h>

namespace alexaClientSDK {
namespace mediaPlayer {

class SourceInterface : public avsCommon::utils::RequiresShutdown {
public:
    SourceInterface(const std::string& className) : RequiresShutdown(className) {
    }

    virtual ~SourceInterface() = default;

    virtual size_t readData(uint16_t *buffer, size_t size) = 0;
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_
