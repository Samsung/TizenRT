#include <stdio.h>
#include <assert.h>

#include <thread> // for debug: print thread id

#include <media/streaming/StreamReader.h>

namespace media {
namespace stream {

StreamReader::StreamReader()
    : mStream(nullptr)
{
}

void StreamReader::setStream(std::shared_ptr<Stream> stream)
{
    mStream = stream;
}

size_t StreamReader::read(unsigned char *buf, size_t size, bool sync)
{
	assert(mStream);

	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t rlen = 0;

	if (sync) {
		while (rlen < size) {
			// Read data from stream as much as possible
			rlen += mStream->read(buf + rlen, size - rlen);
			if (rlen < size) {
				// There's not enough data
				if (mStream->isEndOfStream()) {
					// End of stream, break reading
					break;
				}

				// Notify observer, shouldn't be blocked.
				mStream->notifyObserver(STREAM_OBSERVER_COMMAND_UNDERRUN);
				// Writer may be waiting for more spaces, so it's necessary to notify after reading.
				mStream->getCondv().notify_one();
				// Then wait notification from writer.
				mStream->getCondv().wait(lock);
			}
		}

		assert(rlen == size || mStream->isEndOfStream());
	} else {
		rlen = mStream->read(buf, size);
	}

	// Writer may be waiting for more spaces, so it's necessary to notify after reading.
	mStream->getCondv().notify_one();

	return rlen;
}

size_t StreamReader::sizeOfData()
{
	assert(mStream);
	std::unique_lock<std::mutex> lock(mStream->getMutex());
	return mStream->sizeOfData();
}

} // namespace media
} // namespace stream
