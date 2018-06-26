#include <cstring>
#include <stdio.h>
#include <assert.h>

#include <thread> // for debug: print thread id

#include <media/streaming/Stream.h>
#include <media/streaming/StreamReader.h>
#include <media/streaming/StreamWriter.h>

#include "../utils/rb.h"

namespace media {
namespace stream {

Stream::Stream()
	: mObserver(nullptr), mRingBuf(nullptr), mEOS(false)
{
    mReader = std::move(std::unique_ptr<StreamReader>(new StreamReader()));
    mWriter = std::move(std::unique_ptr<StreamWriter>(new StreamWriter()));

	static int streamId = 1;
	mId = streamId++;
}

Stream::~Stream()
{
	delete mRingBuf;
	mRingBuf = nullptr;
}

void Stream::init(size_t size)
{
	assert(!mRingBuf);
	mRingBuf = new rb_t;

	assert(mRingBuf);
	rb_init(mRingBuf, size);

    mReader->setStream(shared_from_this());
    mWriter->setStream(shared_from_this());
}

int Stream::id()
{
	return mId;
}

std::unique_ptr<StreamReader> Stream::getReader()
{
    return std::move(mReader);
}

std::unique_ptr<StreamWriter> Stream::getWriter()
{
    return std::move(mWriter);
}

size_t Stream::read(unsigned char *buf, size_t size)
{
	assert(mRingBuf);
	return rb_read(mRingBuf, buf, size);
}

size_t Stream::write(unsigned char *buf, size_t size)
{
	assert(mRingBuf);
	return rb_write(mRingBuf, buf, size);
}

size_t Stream::sizeOfSpace()
{
	assert(mRingBuf);
	return rb_avail(mRingBuf);
}

size_t Stream::sizeOfData()
{
	assert(mRingBuf);
	return rb_used(mRingBuf);
}

void Stream::setEndOfStream()
{
	mEOS = true;
}

bool Stream::isEndOfStream()
{
	return mEOS;
}

void Stream::setObserver(std::shared_ptr<StreamObserverInterface> observer)
{
	mObserver = observer;
}

void Stream::notifyObserver(stream_observer_command_t cmd)
{
	if (mObserver) {
		// FIXME: How about notify observer in a worker thread ?
		switch (cmd) {
		case STREAM_OBSERVER_COMMAND_OVERRUN:
			mObserver->onStreamOverrun(mId);
			break;

		case STREAM_OBSERVER_COMMAND_UNDERRUN:
			mObserver->onStreamUnderrun(mId);
			break;
		}
	}
}

} // namespace media
} // namespace stream
