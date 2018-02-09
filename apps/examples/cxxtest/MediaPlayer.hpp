#ifndef __MEIDAFRAMEWORK_MEDIAPLAYER_HPP
#define __MEIDAFRAMEWORK_MEDIAPLAYER_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>

#include "InputDataSource.hpp"
#include <tinyalsa/tinyalsa.h>

using namespace std;

namespace Media 
{
	typedef enum player_state_e
	{
		PLAYER_STATE_NONE,
		PLAYER_STATE_IDLE,
		PLAYER_STATE_READY,
		PLAYER_STATE_PLAYING,
		PLAYER_STATE_PAUSED
	} player_state_t;

	typedef enum player_result_e
	{
		PLAYER_ERROR,
		PLAYER_OK
	} player_result_t;

	class MediaPlayer
	{
	public:
		MediaPlayer();
		~MediaPlayer();

		player_result_t create();
		player_result_t destroy();

		player_result_t prepare();
		player_result_t unprepare();
		player_result_t start();
		player_result_t pause();
		player_result_t stop();

		player_result_t getVolume() const;
		void setVolume(int vol);

		void setDataSource(DataSource dataSource);

		player_state_t getState();

	private:
		template<typename _Callable, typename... _Args>
		void enqueue(_Callable&& __f, _Args&&... __args) {
			unique_lock<std::mutex> lock(*qMtx);
			std::function<void()> func = std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...);	
			cmdQueue.push(func);
			cvQueue.notify_one();
		}

		thread *worker;
		int worker_thread();

		void _create();
		void _destroy();
		void _prepare();
		void _unprepare();
		void _start();
		void _pause();
		void _stop();

	private:
		player_state_t curState;
		mutex *cMtx; // command mutex
		mutex *qMtx; // queue mutex

		std::condition_variable cvQueue;
		std::condition_variable cvSync;
		std::queue<std::function<void()>> cmdQueue;

		bool isRunning;
		int curVolume;

	};
}
#endif
