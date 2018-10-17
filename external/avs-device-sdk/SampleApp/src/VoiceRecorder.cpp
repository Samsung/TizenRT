//#include <tinyalsa/asoundlib.h>

#include <unistd.h>

#include "SampleApp/VoiceRecorder.h"

namespace alexaClientSDK {
namespace sampleApp {

using avsCommon::avs::AudioInputStream;

std::unique_ptr<VoiceRecorder> VoiceRecorder::create(
    std::shared_ptr<AudioInputStream> stream) {
    if (!stream) {
        ConsolePrinter::simplePrint("Invalid stream passed to VoiceRecorder");
        return nullptr;
    }
    std::unique_ptr<VoiceRecorder> voiceRecorder(new VoiceRecorder(stream));
    if (!voiceRecorder->initialize()) {
        ConsolePrinter::simplePrint("Failed to initialize VoiceRecorder");
        return nullptr;
    }
    return voiceRecorder;
}

VoiceRecorder::VoiceRecorder(std::shared_ptr<AudioInputStream> stream) :
        m_audioInputStream{stream} {
}

VoiceRecorder::~VoiceRecorder() {

}

bool VoiceRecorder::initialize() {
    m_writer = m_audioInputStream->createWriter(AudioInputStream::Writer::Policy::NONBLOCKABLE);
    if (!m_writer) {
        ConsolePrinter::simplePrint("Failed to create stream writer");
        return false;
    }

    return true;
}

bool VoiceRecorder::startStreamingMicrophoneData() {
    return true;
}

bool VoiceRecorder::stopStreamingMicrophoneData() {
    return true;
}

}  // namespace sampleApp
}  // namespace alexaClientSDK
