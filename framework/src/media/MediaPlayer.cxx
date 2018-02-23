#include <stdlib.h>
#include <media/MediaPlayer.hpp>

namespace Media
{
	MediaPlayer::MediaPlayer()
	{
		qMtx = new mutex();
		cMtx = new mutex();
		curState = PLAYER_STATE_NONE;
		worker = nullptr;
	}

	player_result_t MediaPlayer::createWorker(unique_lock<mutex> &lock)
	{
		isRunning = true;
		worker = new thread(&MediaPlayer::worker_thread, this);
		if (worker == nullptr) {
			user_cb = nullptr;
			return PLAYER_ERROR;
		}

		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		curState = PLAYER_STATE_IDLE;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::create()
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState != PLAYER_STATE_NONE) {
			return PLAYER_ERROR;
		}

		user_cb = nullptr;
		return createWorker(lock);
	}

	player_result_t MediaPlayer::create(std::function<void(int, int)> &&_user_cb)
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState != PLAYER_STATE_NONE) {
			return PLAYER_ERROR;
		}

		user_cb = std::move(_user_cb);
		return createWorker(lock);
	}

	player_result_t MediaPlayer::create(std::function<void(int, int)> &_user_cb)
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState != PLAYER_STATE_NONE) {
			return PLAYER_ERROR;
		}

		user_cb = _user_cb;
		return createWorker(lock);
	}

	player_result_t MediaPlayer::destroy()
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState != PLAYER_STATE_IDLE) {
			return PLAYER_ERROR;
		}

		enqueue([this]() {isRunning = false; _sync(); });
		cvSync.wait(lock);

		worker->join();
		delete worker;
		worker = nullptr;

		curState = PLAYER_STATE_NONE;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::prepare()
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState != PLAYER_STATE_IDLE) {
			return PLAYER_ERROR;
		}

		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		pcmOut = pcm_open(0, 0, PCM_OUT, NULL);
		if (!pcm_is_ready(pcmOut)) {
			std::cout << "MediaPlayer::preare : pcm is not ready" << std::endl;
			return PLAYER_ERROR;
		}

		ifs.open("/mnt/record", ios::in);
		if (!ifs.is_open()) {
			std::cout << "MediaPlayer::prepare : open fail" << std::endl;
			return PLAYER_ERROR;
		}

		size = pcm_frames_to_bytes(pcmOut, pcm_get_buffer_size(pcmOut));
		buffer = (char *)malloc(size);
		if (!buffer) {
			std::cout << "MediaPlayer::prepare : buffer malloc fail" << std::endl;
			return PLAYER_ERROR;
		}

		curState = PLAYER_STATE_READY;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::unprepare()
	{
		unique_lock<mutex> lock(*cMtx);
		if (curState == PLAYER_STATE_NONE || curState == PLAYER_STATE_IDLE) {
			return PLAYER_ERROR;
		}

		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		free(buffer);
		buffer = nullptr;
		ifs.close();
		pcm_close(pcmOut);

		curState = PLAYER_STATE_IDLE;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::start()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_start(); });

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::stop()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_stop(); });

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::pause()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_pause(); });

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::getVolume() const
	{
		lock_guard<mutex> lock(*cMtx);
		return PLAYER_OK;
	}

	void MediaPlayer::setVolume(int vol)
	{
		lock_guard<mutex> lock(*cMtx);
		curVolume = vol;
	}

	void MediaPlayer::setDataSource(DataSource dataSource)
	{
		lock_guard<mutex> lock(*cMtx);
	}

	player_state_t MediaPlayer::getState()
	{
		return curState;
	}

	void MediaPlayer::_sync()
	{
		std::cout << "MediaPlayer Worker : SYNC" << std::endl;
		unique_lock<mutex> lock(*cMtx);
		cvSync.notify_one();
	}

	void MediaPlayer::_start()
	{
		std::cout << "MediaPlayer Worker : START" << std::endl;
		if (curState != PLAYER_STATE_READY) {
			if (user_cb) {
				user_cb(curState, PLAYER_ERROR);
			}
			return;
		}

		curState = PLAYER_STATE_PLAYING;
		if (user_cb) {
			user_cb(PLAYER_STATE_PLAYING, PLAYER_OK);
		}
	}

	void MediaPlayer::_stop()
	{
		std::cout << "MediaPlayer Worker : STOP" << std::endl;
		if (curState != PLAYER_STATE_PLAYING && curState != PLAYER_STATE_PAUSED) {
			if (user_cb) {
				user_cb(curState, PLAYER_ERROR);
			}
			return;
		}

		curState = PLAYER_STATE_READY;
		if (user_cb) {
			user_cb(PLAYER_STATE_READY, PLAYER_OK);
		}
	}

	void MediaPlayer::_pause()
	{
		std::cout << "MediaPlayer Worker : PAUSED" << std::endl;
		if (curState != PLAYER_STATE_PLAYING) {
			if (user_cb) {
				user_cb(curState, PLAYER_ERROR);
			}
			return;
		}

		curState = PLAYER_STATE_PAUSED;
		if (user_cb) {
			user_cb(PLAYER_STATE_PAUSED, PLAYER_OK);
		}
	}

	MediaPlayer::~MediaPlayer()
	{
		player_result_t ret;

		if (curState > PLAYER_STATE_IDLE) {
			ret = unprepare();
			if (ret != PLAYER_OK) {
				std::cout << "MediaPlayer::unprepare FAILED" << std::endl;
			}
		}

		if (curState == PLAYER_STATE_IDLE) {
			ret = destroy();
			if (ret != PLAYER_OK) {
				std::cout << "MediaPlayer::destroy FAILED" << std::endl;
			}
		}

		delete qMtx;
		delete cMtx;
	}

	int MediaPlayer::worker_thread(void)
	{
		while (isRunning) {
			unique_lock<mutex> lock(*qMtx);

			if (cmdQueue.empty()) {
				if (curState == PLAYER_STATE_PLAYING) {
					ifs.read(buffer, size);
					std::streamsize num_read = ifs.gcount();
					std::cout << "MediaPlayer Worker : num_read = " << num_read << std::endl;
					if (num_read > 0) {
						int ret;
						ret = pcm_writei(pcmOut, buffer, pcm_bytes_to_frames(pcmOut, num_read));
						if (ret == -EPIPE) {
							ret = pcm_prepare(pcmOut);
							if (ret != OK) {
								std::cout << "MediaPlayer Worker : pcm_prepare fail" << std::endl;
							}
							pcm_writei(pcmOut, buffer, pcm_bytes_to_frames(pcmOut, num_read));
						}
					} else {
						_stop();
					}
					continue;
				} else {
					std::cout << "MediaPlayer Worker : SLEEP" << std::endl;
					cvQueue.wait(lock);
					std::cout << "MediaPlayer Worker : WAKEUP" << std::endl;
				}
			}

			std::function<void()> run = cmdQueue.front();
			cmdQueue.pop();
			run();
		}

		return 0;
	}
}
