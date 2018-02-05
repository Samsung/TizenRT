#include <media/MediaPlayer.hpp>

MediaPlayer::MediaPlayer()
{
	qMtx = new mutex();
	cMtx = new mutex();
	curState = PLAYER_STATE_NONE;
}

play_result_t MediaPlayer::create()  // sync call
{
	lock_guard<mutex> lock(*cMtx);
	
	isRunning = true;
	worker = new thread(&MediaPlayer::worker_thread, this);
	
	return PLAYER_OK;
}

play_result_t MediaPlayer::destroy() // sync call
{
	lock_guard<mutex> lock(*cMtx);

	isRunning = false;
	worker->join();
	delete worker;

	return PLAYER_OK;	
}

play_result_t MediaPlayer::prepare()
{	
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_prepare(); });

	return PLAYER_OK;
}

play_result_t MediaPlayer::start()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_start(); });
	
	return PLAYER_OK;
}

play_result_t MediaPlayer::resume()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_resume(); });

	return PLAYER_OK;
}

play_result_t MediaPlayer::stop()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_stop(); });	

	return PLAYER_OK;
}

play_result_t MediaPlayer::pause()
{
	lock_guard<mutex> lock(*cMtx);
	//enqueue([this](){_pause(); });

	return PLAYER_OK;
}

play_result_t MediaPlayer::getVolume() const
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

void MediaPlayer::_prepare()
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

void MediaPlayer::_resume()
{
}

MediaPlayer::~MediaPlayer()
{
	delete qMtx;
	delete cMtx;
}

int MediaPlayer::worker_thread(void)
{
	while (isRunning)
	{ 
		unique_lock<mutex> lock(*qMtx);

		if (cmdQueue.empty())
		{
			cv.wait(lock);
		}

		std::function<void()> run = cmdQueue.front();
		cmdQueue.pop();

		run();
	}	

	return 0;
}
