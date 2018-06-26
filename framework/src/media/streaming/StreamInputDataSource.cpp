#include <thread>
#include <unistd.h>
#include <debug.h>

#include <media/streaming/StreamInputDataSource.h>

namespace media {
namespace stream {

StreamInputDataSource::StreamInputDataSource()
    : mIsWorkerAlive(false)
	, mStatus(BUFFER_STREAM_EMPTY)
{
    mStream = std::make_shared<Stream>();
    mStream->init(0x1000 << 6); // Specify ring buffer size in bytes

	mStreamReader = mStream->getReader();
	mStreamWriter = mStream->getWriter();
	setThreshold(1);
}

StreamInputDataSource::~StreamInputDataSource()
{
    //stop();
}

bool StreamInputDataSource::start()
{
	if (!mIsWorkerAlive) {
		createWriterWorker();
	}

	return true;
}

bool StreamInputDataSource::stop()
{
    if (mIsWorkerAlive) {
		// Stop worker firstly
        stopWriterWorker();
    }

	if (isPrepare()) {
		return close();
	}

	return true;
}

ssize_t StreamInputDataSource::read(unsigned char *buf, size_t size, bool sync)
{
    size_t rlen;

	if( !endOfStream() && mStream->sizeOfData() < size) {
		mStatus = BUFFER_STREAM_BUFFERING;
	}

	if (!endOfStream() && (mStatus == BUFFER_STREAM_BUFFERING || mStatus == BUFFER_STREAM_EMPTY)) {
		while (mStream->sizeOfData() < mThreshold) {
			usleep(1);
		}
		mStatus = BUFFER_STREAM_BUFFERED;
	} 
	rlen = mStreamReader->read(buf, size, sync);

	// Notify worker there's data read from stream (In other words, stream is writable now).
	mWakeupWriterWorker();

	return rlen;
}

int StreamInputDataSource::readAt(long offset, int origin, unsigned char* buf, size_t size)
{
	return -1;
}

void StreamInputDataSource::setObserver(std::shared_ptr<StreamObserverInterface> observer)
{
	std::unique_lock<std::mutex> lock(mStream->getMutex());
	mStream->setObserver(observer);
}

int StreamInputDataSource::getStreamId()
{
	return mStream->id();
}

void StreamInputDataSource::createWriterWorker()
{
    mIsWorkerAlive = true;
    mWorker = std::thread([this]() {
        while (mIsWorkerAlive) {
			// Sync with a reading/stopping operation
			mSleepWriterWorker();

			// Worker may be stoped
			if (!mIsWorkerAlive) {
				break;
			}

			if ((mStreamWriter->sizeOfSpace() > 0)
			 && (!endOfStream())) {
	            onStreamWritable();
			}
        }
    });
}

void StreamInputDataSource::stopWriterWorker()
{
	// Setup flag,
    mIsWorkerAlive = false;

	// then wakeup worker.
	mWakeupWriterWorker();

	// join thread
    mWorker.join();
}

void StreamInputDataSource::mSleepWriterWorker()
{
	std::unique_lock<std::mutex> lock(mSyncMtx);
	if (endOfStream() || mStreamWriter->sizeOfSpace() == 0) {
		mSyncCV.wait(lock);
	}
}

void StreamInputDataSource::mWakeupWriterWorker()
{
	std::unique_lock<std::mutex> lock(mSyncMtx);
	mSyncCV.notify_one();
}

void StreamInputDataSource::setThreshold(size_t sec)
{	
	mThreshold = (size_t)getChannels() * (size_t)getSampleRate() * (size_t)(getPcmFormat() + 1) * sec;
}

size_t StreamInputDataSource::getThreshold()
{
	return mThreshold;
}

} // namespace stream
} // namespace media
