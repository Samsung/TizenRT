#ifndef ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_
#define ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_

#include <AVSCommon/Utils/RequiresShutdown.h>
#include <media/InputDataSource.h>

namespace alexaClientSDK {
namespace mediaPlayer {

class SourceInterface : public avsCommon::utils::RequiresShutdown, public media::stream::InputDataSource {
public:
    SourceInterface(const std::string& className) : RequiresShutdown(className) {
    }

    virtual ~SourceInterface() = default;
};

}  // namespace mediaPlayer
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_MEDIAPLAYER_INCLUDE_MEDIAPLAYER_SOURCEINTERFACE_H_
