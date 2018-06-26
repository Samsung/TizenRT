#include <iostream>
#include <thread> // for debug: print thread id
#include <stdio.h>
#include <assert.h>

#include <media/streaming/StreamWriter.h>

using namespace std;

namespace media {
namespace stream {

StreamWriter::StreamWriter()
    : mStream(nullptr)
{
}

void StreamWriter::setStream(std::shared_ptr<Stream> stream)
{
    mStream = stream;
}

size_t StreamWriter::write(unsigned char *buf, size_t size, bool sync)
{
	assert(mStream);
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t wlen = 0;

	if (sync) {
		while (wlen < size) {
			// Write data into stream as much as possible
			wlen += mStream->write(buf + wlen, size - wlen);
			if (wlen < size) {
				// There's not enough space
				// Notify observer, shouldn't be blocked.
				mStream->notifyObserver(STREAM_OBSERVER_COMMAND_OVERRUN);
				// Reader may be waiting for more data, so it's necessary to notify after writing.
				mStream->getCondv().notify_one();
				// Then wait notification from reader.
				mStream->getCondv().wait(lock);
			}
		}
		assert(wlen == size);
	} else {
		wlen = mStream->write(buf, size);
	}

	// Reader may be waiting for more data, so it's necessary to notify after writing.
	mStream->getCondv().notify_one();

	return wlen;
}

size_t StreamWriter::sizeOfSpace()
{
	assert(mStream);
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t size = mStream->sizeOfSpace();
	return size;
}

size_t StreamWriter::sizeOfData()
{
	assert(mStream);
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	size_t size = mStream->sizeOfData();
	return size;
}

void StreamWriter::setEndOfStream()
{
	assert(mStream);
	std::unique_lock<std::mutex> lock(mStream->getMutex());

	// Set EOS flag in stream.
	mStream->setEndOfStream();

	// Reader may be waiting for more spaces, so it's necessary to notify EOS.
	mStream->getCondv().notify_one();
}

} // namespace media
} // namespace stream
