#include <iostream>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include <media/streaming/BufferStreamInputDataSource.h>
#include <pthread.h>

#include <curl/curl.h>

namespace media {
namespace stream {


size_t recv_callback( void *source , size_t size , size_t nmemb , void *userData ){
	std::cout << "[jjs]recv_callback "<<size*nmemb <<std::endl;
	BufferStreamInputDataSource* stream = static_cast<BufferStreamInputDataSource*>(userData);

	unsigned char* data = static_cast< unsigned char* >( source );
	size_t bufferSize = size * nmemb ;
	stream->addBufferStreamData(data, bufferSize);

	return ( bufferSize ) ;

} // showSize()

BufferStreamInputDataSource::BufferStreamInputDataSource(const std::string& httpAddr)
    : StreamInputDataSource()
	, mIpAddr(httpAddr)
	, mIsPrepare(false)
{
	mIsEndOfStream = false;
	start();
}

BufferStreamInputDataSource::~BufferStreamInputDataSource()
{
	stop();
}

bool BufferStreamInputDataSource::open()
{
	mWorker = std::thread([this]() {
		CURL *mCurl;
		mCurl = curl_easy_init();
		curl_easy_setopt(mCurl, CURLOPT_URL, mIpAddr.c_str());
		curl_easy_setopt(mCurl, CURLOPT_USERAGENT, "curl/7.35.0");
		curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, recv_callback);
		curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(mCurl, CURLOPT_BUFFERSIZE, 4096);
        int ret = curl_easy_perform(mCurl);
		this->setEndOfStream(true);
	 });

	mIsPrepare = true;

	return true;
}

size_t BufferStreamInputDataSource::addBufferStreamData(unsigned char *buf, size_t size, bool isEndOfStream = false)
{
	std::unique_lock<std::mutex> lock(mMutex);

	size_t smax = mStreamWriter->sizeOfSpace();
	while (smax < size){
		mSyncCV.wait(lock);
		smax = mStreamWriter->sizeOfSpace();
	}
	size_t wlen = mStreamWriter->write(buf, size);
	assert(wlen == size);
	mIsEndOfStream = isEndOfStream;

	return wlen;
}

bool BufferStreamInputDataSource::close()
{
}

void BufferStreamInputDataSource::onStreamWritable()
{
//	std::cout << "[jjs]onStreamWritable"<<std::endl;
	assert(mStreamWriter->sizeOfSpace() > 0);
	mSyncCV.notify_one();
}

bool BufferStreamInputDataSource::isPrepare()
{
	return mIsPrepare;
}

bool BufferStreamInputDataSource::endOfStream()
{
	return (!isPrepare() || mIsEndOfStream);
}

void BufferStreamInputDataSource::setEndOfStream(bool isEnd)
{
	mIsEndOfStream = isEnd;
}


} // namespace stream
} // namespace media
