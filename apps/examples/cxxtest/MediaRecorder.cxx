#include "MediaRecorder.hpp"

namespace Media 
{
	MediaRecorder::MediaRecorder()
	{
		_init();
	}
	
	recorder_result_t MediaRecorder::createWorker(unique_lock<mutex> &lock)
	{
		isRunning = true;		
		worker = new thread(&MediaRecorder::worker_thread, this);
		if (worker == nullptr) {
			user_cb = nullptr;
			return RECORDER_ERROR;
		}
		
		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		curState = RECORDER_STATE_IDLE;		
		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::create()
	{
		unique_lock<mutex> lock(*cMtx);

		std::cout << "create Recorder" << std::endl;

		if (curState != RECORDER_STATE_NONE) {
			return RECORDER_ERROR;
		}

		user_cb = nullptr;
		return createWorker(lock);
	}

	recorder_result_t MediaRecorder::create(std::function<void(int, int)> &&_user_cb)
	{
		unique_lock<mutex> lock(*cMtx);

		if (curState != RECORDER_STATE_NONE) {
			return RECORDER_ERROR;
		}

		user_cb = std::move(_user_cb);
		return createWorker(lock);
	}

	recorder_result_t MediaRecorder::create(std::function<void(int, int)> &_user_cb)
	{
		unique_lock<mutex> lock(*cMtx);

		if (curState != RECORDER_STATE_NONE) {
			return RECORDER_ERROR;
		}

		user_cb = _user_cb;
		return createWorker(lock);
	}

	recorder_result_t MediaRecorder::destroy() // sync call
	{
		unique_lock<mutex> lock(*cMtx);

		if (curState != RECORDER_STATE_IDLE) {
			return RECORDER_ERROR;
		}

		isRunning = false;
		enqueue([this]() {_sync(); });
		cvSync.wait(lock);
		
		worker->join();
		delete worker;
		worker = nullptr;

		pcm_close(pcmIn);
		curState = RECORDER_STATE_NONE;

		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::prepare()
	{
		unique_lock<mutex> lock(*cMtx);

		if (curState != RECORDER_STATE_IDLE) {
			return RECORDER_ERROR;
		}

		enqueue([this](){_sync(); });
		cvSync.wait(lock);

		struct pcm_config config;
		memset(&config, 0, sizeof(struct pcm_config));
		config.channels = recorderDataSource->getChannels();
		config.rate = recorderDataSource->getSampleRate();
		config.format = (pcm_format)recorderDataSource->getPcmFormat();

		//cardnum,  getDeviceCard
		pcmIn = pcm_open(0, 0, PCM_IN, &config);	
		if (!pcm_is_ready(pcmIn)) {
			return RECORDER_ERROR;
		}

		ofs.open(recorderDataSource->getDataPath(), std::fstream::out | std::fstream::trunc);

		if (!ofs.is_open())
		{
			std::cout << "file open failed" << std::endl;
			return RECORDER_ERROR;
		}

		buffSize = pcm_frames_to_bytes(pcmIn, pcm_get_buffer_size(pcmIn));
		buffer = new char[buffSize];
		if (!buffer)
		{
			std::cout << "alloc buffer failed" << std::endl;
			return RECORDER_ERROR;
		}
		
		curState = RECORDER_STATE_READY;
		return RECORDER_OK;
	}

	recorder_result_t MediaRecorder::unprepare()
	{
		unique_lock<mutex> lock(*cMtx);

		if (curState == RECORDER_STATE_NONE || curState == RECORDER_STATE_IDLE) {
			return RECORDER_ERROR;
		}

		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		if (buffer) {
			delete []buffer;
			buffer = nullptr;
		}
		
		buffSize = 0;
		
		if (ofs.is_open()) {
			ofs.close();
		}
		
		if (!pcmIn) {
			pcm_close(pcmIn);
		}
		
		curState = RECORDER_STATE_IDLE;
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

	void MediaRecorder::setDataSource(RecorderDataSource *_recorderDataSource)
	{
		lock_guard<mutex> lock(*cMtx);

		recorderDataSource = _recorderDataSource;
	}

	void MediaRecorder::_init()
	{
		qMtx = new mutex();
		cMtx = new mutex();		
		curState = RECORDER_STATE_NONE;
		worker = nullptr;
		pcmIn = nullptr;
		buffer = nullptr;
	}

	void MediaRecorder::_sync()
	{
		unique_lock<mutex> lock(*cMtx);
		std::cout << "__sync" << std::endl;		
		cvSync.notify_one();
	}

	void MediaRecorder::_start()
	{
		std::cout << "start recording" << std::endl;
				
		if (curState != RECORDER_STATE_READY) {
			if (user_cb) {
				user_cb(curState, RECORDER_ERROR);
			}
			return;
		}

		curState = RECORDER_STATE_RECORDING;
		if (user_cb) {
			user_cb(RECORDER_STATE_RECORDING, RECORDER_OK);
		}
		curState = RECORDER_STATE_RECORDING;
	}

	void MediaRecorder::_stop()
	{
		std::cout << "stop recording" << std::endl;		

		if (curState != RECORDER_STATE_RECORDING && curState != RECORDER_STATE_PAUSED) {
			if (user_cb) {
				user_cb(curState, RECORDER_ERROR);
			}
			return;
		}

		curState = RECORDER_STATE_READY;
		if (user_cb) {
			user_cb(RECORDER_STATE_READY, RECORDER_OK);
		}
	}

	void MediaRecorder::_pause()
	{
		std::cout << "pause recording" << std::endl;

		if (curState != RECORDER_STATE_RECORDING) {
			if (user_cb) {
				user_cb(curState, RECORDER_ERROR);
			}
			return;
		}

		curState = RECORDER_STATE_PAUSED;
		if (user_cb) {
			user_cb(RECORDER_STATE_PAUSED, RECORDER_OK);
		}
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

			if (cmdQueue.empty()) {
				if (curState == RECORDER_STATE_RECORDING) {
					int frames = 0, size = 0;
					frames = pcm_readi(pcmIn, buffer, pcm_bytes_to_frames(pcmIn, buffSize));
					
					if (frames == -EPIPE) {
						if (pcm_prepare(pcmIn) != OK) {
							_stop();
						}

						continue;
					}

					if (frames > 0) {
						size = pcm_frames_to_bytes(pcmIn, frames);	

						int ret = ofs.write(buffer, size);
						if (ret < 0) {
							std::cout << "write file failed" << std::endl;
						}
					}
				} else {
					std::cout << "wait Queue" << std::endl;
					cvQueue.wait(lock);
				}
			}

			std::function<void()> run = cmdQueue.front();
			cmdQueue.pop();

			run();
		}
		return 0;
	}
}
