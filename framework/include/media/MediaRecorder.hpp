#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>

#include <media/InputDataSource.hpp>

using namespace std;

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
	~MediaRecorder();

	recorder_result_t create();
	recorder_result_t destroy();

	//recorder_result_t prepare();
	recorder_result_t start();
	recorder_result_t pause();
	recorder_result_t resume();
	recorder_result_t stop();

	recorder_result_t getVolume() const;
	void setVolume(int vol);

	void setDataSource(DataSource dataSource);

private:
/*
	template<typename _Callable, typename... _Args>
	void enqueue(_Callable&& __f, _Args&&... __args)
	{
		unique_lock<std::mutex> lock(*qMtx);
		std::function<void()> func = std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...);
		cmdQueue.push(func);
		cv.notify_one();
	};*/

	thread *worker;
	int worker_thread();

	//void _prepare();
	void _start();
	void _pause();
	void _resume();
	void _stop();

private:
	recorder_state_t curState;
	mutex *cMtx; // command mutex
	mutex *qMtx; // queue mutex

	std::condition_variable cv;
	std::queue<std::function<void()>> cmdQueue;

	bool isRunning;
	int curVolume;

};
