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

	player_result_t MediaPlayer::create(std::function<void(int, int)> _user_cb = nullptr)  // sync call
	{
		unique_lock<mutex> lock(*cMtx);

		user_cb = _user_cb;
		isRunning = true;
		worker = new thread(&MediaPlayer::worker_thread, this);
		enqueue([this]() {_create(); });
		cvSync.wait(lock);

		curState = PLAYER_STATE_IDLE;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::destroy() // sync call
	{
		lock_guard<mutex> lock(*cMtx);

		enqueue([this]() {_destroy(); });
		worker->join();
		delete worker;
		worker = nullptr;

		curState = PLAYER_STATE_NONE;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::prepare()
	{
		unique_lock<mutex> lock(*cMtx);
		enqueue([this]() {_prepare(); });
		cvSync.wait(lock);

		curState = PLAYER_STATE_READY;
		return PLAYER_OK;
	}

	player_result_t MediaPlayer::unprepare()
	{
		unique_lock<mutex> lock(*cMtx);
		enqueue([this]() {_unprepare(); });
		cvSync.wait(lock);

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

	void MediaPlayer::_create()
	{
		unique_lock<mutex> lock(*cMtx);
		std::cout << "Player create" << std::endl;
		cvSync.notify_one();
	}

	void MediaPlayer::_destroy()
	{
		std::cout << "Player destroy" << std::endl;
		isRunning = false;
	}

	void MediaPlayer::_prepare()
	{
		unique_lock<mutex> lock(*cMtx);
		std::cout << "Player prepare" << std::endl;
		cvSync.notify_one();
	}

	void MediaPlayer::_unprepare()
	{
		unique_lock<mutex> lock(*cMtx);
		std::cout << "Player unprepare" << std::endl;
		cvSync.notify_one();
	}

	void MediaPlayer::_start()
	{
		std::cout << "start playing" << std::endl;
		curState = PLAYER_STATE_PLAYING;
		if (user_cb) {
			user_cb(PLAYER_STATE_PLAYING, PLAYER_OK);
		}
	}

	void MediaPlayer::_stop()
	{
		curState = PLAYER_STATE_READY;
		if (user_cb) {
			user_cb(PLAYER_STATE_READY, PLAYER_OK);
		}
	}

	void MediaPlayer::_pause()
	{
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
