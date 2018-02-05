#include <media/MediaRecorder.hpp>

MediaRecorder::MediaRecorder()
{
	qMtx = new mutex();
	cMtx = new mutex();
	curState = RECORDER_STATE_NONE;
}

recorder_result_t MediaRecorder::create()  // sync call
{
	lock_guard<mutex> lock(*cMtx);

	isRunning = true;
	worker = new thread(&MediaRecorder::worker_thread, this);

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::destroy() // sync call
{
	lock_guard<mutex> lock(*cMtx);

	isRunning = false;
	worker->join();
	delete worker;

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::start()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_start(); });

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::resume()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_resume(); });

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::stop()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_stop(); });

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::pause()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_pause(); });

	return RECORDER_OK;
}

recorder_result_t MediaRecorder::getVolume() const
{
	lock_guard<mutex> lock(*cMtx);
	return RECORDER_OK;
}

void MediaRecorder::setVolume(int vol)
{
	lock_guard<mutex> lock(*cMtx);
	curVolume = vol;
}

void MediaRecorder::setDataSource(DataSource dataSource)
{
	lock_guard<mutex> lock(*cMtx);
}

void MediaRecorder::_start()
{
	std::cout << "start recording" << std::endl;
}

void MediaRecorder::_stop()
{
}

void MediaRecorder::_pause()
{
}

void MediaRecorder::_resume()
{
}

MediaRecorder::~MediaRecorder()
{
	delete qMtx;
	delete cMtx;
}

int MediaRecorder::worker_thread(void)
{
	while (isRunning)
	{
		unique_lock<mutex> lock(*qMtx);

		if (cmdQueue.empty())
		{
			cv.wait(lock);
		}

		std::function<void()> run = cmdQueue.front();
		cmdQueue.pop();

		run();
	}

	return 0;
}

