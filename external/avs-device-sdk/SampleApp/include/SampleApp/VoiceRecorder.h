#ifndef ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_VOICE_RECORDER_H_
#define ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_VOICE_RECORDER_H_

#include <mutex>
#include <thread>

#include <AVSCommon/AVS/AudioInputStream.h>

#include <media/MediaRecorder.h>
#include <media/OutputDataSource.h>

#include <SampleApp/ConsolePrinter.h>

namespace alexaClientSDK {
namespace sampleApp {

/// This acts as a wrapper around PortAudio, a cross-platform open-source audio I/O library.
class VoiceRecorder {
public:
    class VoiceRecorderOutputDataSource : public media::stream::OutputDataSource {
    public:
        void setWriter(std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> writer) {
            m_writer = writer;
        }

        size_t write(unsigned char* buf, size_t size) override {
            ssize_t ret = m_writer->write(buf, size);
            if (ret <= 0) {
                ConsolePrinter::simplePrint("Failed to write to stream.");
                return 0;
            }

            return (size_t)ret;
        }

    private:
        std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> m_writer;
    };
public:
    /**
     * Creates a @c VoiceRecorder.
     *
     * @param stream The shared data stream to write to.
     * @return A unique_ptr to a @c VoiceRecorder if creation was successful and @c nullptr otherwise.
     */
    static std::unique_ptr<VoiceRecorder> create(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

    /**
     * Stops streaming from the microphone.
     *
     * @return Whether the stop was successful.
     */
    bool stopStreamingMicrophoneData();

    /**
     * Starts streaming from the microphone.
     *
     * @return Whether the start was successful.
     */
    bool startStreamingMicrophoneData();

    /**
     * Destructor.
     */
    ~VoiceRecorder();

private:
    /**
     * Constructor.
     *
     * @param stream The shared data stream to write to.
     */
    VoiceRecorder(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

    /// Initializes VoiceRecorder
    bool initialize();

    /// The stream of audio data.
    const std::shared_ptr<avsCommon::avs::AudioInputStream> m_audioInputStream;

    /// The writer that will be used to writer audio data into the sds.
    std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> m_writer;
};

}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_TINYALSAWRAPPER_H_
