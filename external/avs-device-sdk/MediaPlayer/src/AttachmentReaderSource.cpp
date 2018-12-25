#include <cstring>

#include <AVSCommon/Utils/Logger/Logger.h>
#include <AVSCommon/AVS/Attachment/AttachmentReader.h>
#include <MediaPlayer/AttachmentReaderSource.h>
#include <media/MediaTypes.h>

namespace alexaClientSDK {
namespace mediaPlayer {

using namespace avsCommon::avs::attachment;
using namespace media;
using namespace media::stream;

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
        m_reader{reader}
{
}

AttachmentReaderSource::~AttachmentReaderSource()
{
	close();
}

bool AttachmentReaderSource::init()
{
    return true;
}

bool AttachmentReaderSource::open()
{
	setAudioType(media::AUDIO_TYPE_MP3);
	setSampleRate(24000);
	setChannels(1);
	return true;
}

bool AttachmentReaderSource::close()
{
	return true;
}

bool AttachmentReaderSource::isPrepared()
{
	return (m_reader != nullptr);
}

ssize_t AttachmentReaderSource::read(unsigned char* buffer, size_t size)
{
	auto status = AttachmentReader::ReadStatus::OK;
    size_t len = m_reader->read(buffer, size, &status);

    switch (status) {
    case AttachmentReader::ReadStatus::CLOSED:
        if (len == 0) {
	        ACSDK_ERROR(LX("AttachmentReader::ReadStatus::CLOSED"));
            return -1;
        }
    // Fall through if some data was read.
    case AttachmentReader::ReadStatus::OK:
    case AttachmentReader::ReadStatus::OK_WOULDBLOCK:
    // Fall through to retry reading later.
    case AttachmentReader::ReadStatus::OK_TIMEDOUT:
        if (len > 0) {
			break;
        }
        return 0;
    case AttachmentReader::ReadStatus::ERROR_OVERRUN:
    case AttachmentReader::ReadStatus::ERROR_BYTES_LESS_THAN_WORD_SIZE:
    case AttachmentReader::ReadStatus::ERROR_INTERNAL:
        auto error = static_cast<int>(status);
        ACSDK_ERROR(LX("AttachmentReaderSource").d("reason", "readFailed").d("error", error));
        return -1;
	}

    ACSDK_DEBUG9(LX("read").d("size", len).d("status", static_cast<int>(status)));
    return len;
}

}  // namespace mediaPlayer
}  // namespace alexaClientSDK
