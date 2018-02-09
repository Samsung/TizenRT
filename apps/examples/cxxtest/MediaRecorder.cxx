#include "MediaRecorder.hpp"

namespace Media 
{
	MediaRecorder::MediaRecorder()
	{
		_init();
	}
	
	recorder_result_t MediaRecorder::create()  // sync call
	{
		unique_lock<mutex> lock(*cMtx);

		isRunning = true;
		worker = new thread(&MediaRecorder::worker_thread, this);
		enqueue([this](){_create(); });
		cvStart.wait(lock);

		curState = RECORDER_STATE_IDLE;

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::destroy() // sync call
	{
		lock_guard<mutex> lock(*cMtx);

		enqueue([this](){_destroy(); });
		worker->join();
		delete worker;
		worker = nullptr;

		curState = RECORDER_STATE_NONE;

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::prepare()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this](){_prepare(); });

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::unprepare()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_unprepare(); });

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::start()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this](){_start(); });

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::stop()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this](){_stop();});

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::pause()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this](){_pause(); });

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

	void MediaRecorder::setOutputDataSource(OutputDataSource *_outputDataSource)
	{
		lock_guard<mutex> lock(*cMtx);

		outputDataSource = _outputDataSource;
	}

	void MediaRecorder::_init()
	{
		qMtx = new mutex();
		cMtx = new mutex();		
		curState = RECORDER_STATE_NONE;
		worker = nullptr;
		pcmIn = nullptr;
	}

	void MediaRecorder::_prepare()
	{
		std::cout << "prepare recording" << std::endl;
		curState = RECORDER_STATE_READY;

		struct pcm_config config;
		memset(&config, 0, sizeof(struct pcm_config));
		config.channels = outputDataSource->getChannels();
		config.rate = outputDataSource->getSampleRate();
		config.format = (pcm_format)outputDataSource->getPcmFormat();

		//cardnum,  getDeviceCard
		pcmIn = pcm_open(0, 0, PCM_IN, &config);	
	}

	void MediaRecorder::_unprepare()
	{
		std::cout << "unprepare recording" << std::endl;
		curState = RECORDER_STATE_IDLE;
	}

	void MediaRecorder::_create()
	{
		std::cout << "create Recorder" << std::endl;	
		cvStart.notify_one();
	}

	void MediaRecorder::_destroy()
	{
		std::cout << "destroy Recorder" << std::endl;
		pcm_close(pcmIn);

		isRunning = false;
	}

	void MediaRecorder::_start()
	{
		std::cout << "start recording" << std::endl;
		curState = RECORDER_STATE_RECORDING;
	}

	void MediaRecorder::_stop()
	{
		std::cout << "stop recording" << std::endl;
		curState = RECORDER_STATE_READY;
	}

	void MediaRecorder::_pause()
	{
		std::cout << "pause recording" << std::endl;
		curState = RECORDER_STATE_PAUSED;
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
				cvQueue.wait(lock);
			}

			std::function<void()> run = cmdQueue.front();
			cmdQueue.pop();

			run();
		}

		return 0;
	}

}
