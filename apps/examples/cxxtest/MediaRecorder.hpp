#ifndef __MEIDAFRAMEWORK_MEDIARECORDER_HPP
#define __MEIDAFRAMEWORK_MEDIARECORDER_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>

#include <tinyalsa/tinyalsa.h>
#include "OutputDataSource.hpp"

using namespace std;

namespace Media 
{
	typedef enum recorder_state_e
	{
		RECORDER_STATE_NONE,
		RECORDER_STATE_IDLE,
		RECORDER_STATE_READY,
		RECORDER_STATE_RECORDING,
		RECORDER_STATE_PAUSED
	} recorder_state_t;

	typedef enum recorder_result_e
	{
		RECORDER_ERROR,
		RECORDER_OK
	} recorder_result_t;

	class MediaRecorder
	{
	public:
		MediaRecorder();
		MediaRecorder(int, int, int );
		~MediaRecorder();

		recorder_result_t create();
		recorder_result_t destroy();
		
		recorder_result_t prepare();
		recorder_result_t unprepare();
		recorder_result_t start();
		recorder_result_t pause();
		recorder_result_t stop();

		recorder_result_t getVolume() const;
		void setVolume(int vol);

		void setOutputDataSource(OutputDataSource *_outputDataSource);

	private:

		template<typename _Callable, typename... _Args>
		void enqueue(_Callable&& __f, _Args&&... __args)
		{
			unique_lock<std::mutex> lock(*qMtx);
			std::function<void()> func = std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...);
			cmdQueue.push(func);
			cvQueue.notify_one();
		}

		thread *worker;
		int worker_thread();

		void _init();
		void _prepare();
		void _unprepare();
		void _create();
		void _destroy();
		void _start();
		void _pause();
		void _stop();

	private:
		struct pcm *pcmIn;
		recorder_state_t curState;
		mutex *cMtx; // command mutex
		mutex *qMtx; // queue mutex

		std::condition_variable cvQueue;
		std::condition_variable cvStart;
		std::queue<std::function<void()>> cmdQueue;

		bool isRunning;
		int curVolume;

		OutputDataSource *outputDataSource;
	};
}

#endif
