#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <functional>
#include <iostream>

#include <media/InputDataSource.hpp>

using namespace std;

typedef enum player_state_e
{
	PLAYER_STATE_NONE,
	PLAYER_STATE_IDLE,
	PLAYER_STATE_READY,
	PLAYER_STATE_PLAYING,
	PLAYER_STATE_PAUSED
} play_state_t;

typedef enum player_result_e
{
	PLAYER_ERROR,
	PLAYER_OK
} play_result_t;

class MediaPlayer
{
public:
	MediaPlayer();
	~MediaPlayer();
	
	play_result_t create();
	play_result_t destroy();
	
	play_result_t prepare();
	play_result_t start();
	play_result_t pause();
	play_result_t resume();
	play_result_t stop();

	play_result_t getVolume() const;
	void setVolume(int vol);

	void setDataSource(DataSource dataSource);

private:
	template<typename _Callable, typename... _Args>
	void enqueue(_Callable&& __f, _Args&&... __args)
	{
		unique_lock<std::mutex> lock(*qMtx);
		std::function<void()> func = std::bind(std::forward<_Callable>(__f), std::forward<_Args>(__args)...);	
		cmdQueue.push(func);
		cv.notify_one();
	}
	
	thread *worker;
	int worker_thread();

	void _prepare();
	void _start();
	void _pause();
	void _resume();
	void _stop();



private:
	play_state_t curState;
	mutex *cMtx; // command mutex
	mutex *qMtx; // queue mutex

	std::condition_variable cv;
	std::queue<std::function<void()>> cmdQueue;

	bool isRunning;
	int curVolume;

};