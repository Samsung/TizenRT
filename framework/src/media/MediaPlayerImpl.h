#ifndef __MEDIA_MEDIAPLAYERIMPL_H
#define __MEDIA_MEDIAPLAYERIMPL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <media/MediaPlayer.h>
#include <media/InputDataSource.h>
#include <media/MediaPlayerObserverInterface.h>

#include "PlayerObserverWorker.h"

namespace media {
class MediaPlayerImpl : public std::enable_shared_from_this<MediaPlayerImpl>
{
public:
	MediaPlayerImpl();
	~MediaPlayerImpl();

	player_result_t create();
	player_result_t destroy();
	player_result_t prepare();
	player_result_t unprepare();
	player_result_t start();
	player_result_t pause();
	player_result_t stop();

	int getVolume();
	player_result_t setVolume(int);

	player_result_t setDataSource(std::unique_ptr<stream::InputDataSource>);
	void setObserver(std::shared_ptr<MediaPlayerObserverInterface>);

	player_state_t getState();
	player_result_t seekTo(int);

	void notifySync();
	void notifyObserver(player_observer_command_t cmd);

	int playback(int size);

public:
	player_state_t mCurState;
	unsigned char* mBuffer;
	unsigned int mBufSize;
	static int mRefCnt;
	std::mutex mCmdMtx;
	std::condition_variable mSyncCv;
	int mCurVolume;
	std::shared_ptr<MediaPlayerObserverInterface> mPlayerObserver;
	std::shared_ptr<stream::InputDataSource> mInputDataSource;
	int mId;
};
} // namespace media
#endif
