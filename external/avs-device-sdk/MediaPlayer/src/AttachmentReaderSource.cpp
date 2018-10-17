#include <cstring>

#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/AVS/Attachment/AttachmentReader.h>
#include <MediaPlayer/AttachmentReaderSource.h>

namespace alexaClientSDK {
namespace mediaPlayer {

using namespace avsCommon::avs::attachment;

/// String to identify log entries originating from this file.
static const std::string TAG("AttachmentReaderSource");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

std::unique_ptr<AttachmentReaderSource> AttachmentReaderSource::create(
    std::shared_ptr<AttachmentReader> reader) {
    std::unique_ptr<AttachmentReaderSource> result(new AttachmentReaderSource(reader));
    if (result->init()) {
        return result;
    }
    return nullptr;
};

AttachmentReaderSource::AttachmentReaderSource(std::shared_ptr<AttachmentReader> reader) :
        SourceInterface{"AttachmentReaderSource"},
        m_reader{reader} {};

AttachmentReaderSource::~AttachmentReaderSource() {

}

bool AttachmentReaderSource::init() {
    return true;
}

size_t AttachmentReaderSource::readData(uint16_t *buffer, size_t size) {
    auto status = AttachmentReader::ReadStatus::OK;
    auto len = m_reader->read(buffer, size, &status);

    ACSDK_DEBUG9(LX("read").d("size", len).d("status", static_cast<int>(status)));

    return len;
}

}  // namespace mediaPlayer
}  // namespace alexaClientSDK
