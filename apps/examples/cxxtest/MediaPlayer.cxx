#include "MediaPlayer.hpp"

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
		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		if (curState != PLAYER_STATE_IDLE) {
			return PLAYER_ERROR;
		}

		curState = PLAYER_STATE_READY;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::unprepare()
	{
		unique_lock<mutex> lock(*cMtx);
		enqueue([this]() {_sync(); });
		cvSync.wait(lock);

		if (curState == PLAYER_STATE_NONE || curState == PLAYER_STATE_IDLE) {
			return PLAYER_ERROR;
		}

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
		delete qMtx;
		delete cMtx;
	}

	int MediaPlayer::worker_thread(void)
	{
		while (isRunning) {
			unique_lock<mutex> lock(*qMtx);

			if (cmdQueue.empty()) {
				cvQueue.wait(lock);
			}

			std::function<void()> run = cmdQueue.front();
			cmdQueue.pop();

			run();
		}

		return 0;
	}
}
