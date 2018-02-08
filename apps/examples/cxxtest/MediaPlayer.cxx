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

	player_result_t MediaPlayer::create()  // sync call
	{
		unique_lock<mutex> lock(*cMtx);

		isRunning = true;
		worker = new thread(&MediaPlayer::worker_thread, this);
		enqueue([this]() {_create(); });
		cvStart.wait(lock);

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::destroy() // sync call
	{
		lock_guard<mutex> lock(*cMtx);

		enqueue([this]() {_destroy(); });
		worker->join();
		delete worker;
		worker = nullptr;

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::prepare()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_prepare(); });

		return PLAYER_OK;
	}

	player_result_t MediaPlayer::unprepare()
	{
		lock_guard<mutex> lock(*cMtx);
		enqueue([this]() {_unprepare(); });

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
		cvStart.notify_one();
	}

	void MediaPlayer::_destroy()
	{
		std::cout << "Player destroy" << std::endl;
		isRunning = false;
	}

	void MediaPlayer::_prepare()
	{

	}

	void MediaPlayer::_unprepare()
	{

	}

	void MediaPlayer::_start()
	{
		std::cout << "start playing" << std::endl;
	}

	void MediaPlayer::_stop()
	{
	}

	void MediaPlayer::_pause()
	{
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
