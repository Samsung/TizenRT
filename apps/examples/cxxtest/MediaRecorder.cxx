#include "MediaRecorder.hpp"

namespace Media 
{
	MediaRecorder::MediaRecorder() : channels(2), sampleRate(16000), pcmFormat(0)
	{
		_init();
	}

	MediaRecorder::MediaRecorder(int _channels, int _sampleRate, int _pcmFormat)
	: channels(_channels), sampleRate(_sampleRate), pcmFormat(_pcmFormat)
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

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::destroy() // sync call
	{
		lock_guard<mutex> lock(*cMtx);

		isRunning = false;
		worker->join();
		delete worker;

		worker = nullptr;

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::prepare()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this](){_prepare(); });

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

	void MediaRecorder::setDataSource(DataSource dataSource)
	{
		lock_guard<mutex> lock(*cMtx);
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
	}

	void MediaRecorder::_create()
	{
		std::cout << "create Recorder" << std::endl;

		struct pcm_config config;
		memset(&config, 0, sizeof(struct pcm_config));
		config.channels = channels;
		config.rate = sampleRate;
		config.format = (pcm_format)pcmFormat;

		pcmIn = pcm_open(0, 0, PCM_IN, &config);		
		cvStart.notify_one();
	}

	void MediaRecorder::_start()
	{
		std::cout << "start recording" << std::endl;	
	}

	void MediaRecorder::_stop()
	{
		std::cout << "stop recording" << std::endl;
	}

	void MediaRecorder::_pause()
	{
		std::cout << "pause recording" << std::endl;
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
