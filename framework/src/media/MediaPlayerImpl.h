#ifndef __MEDIA_MEDIAPLAYERIMPL_H
#define __MEDIA_MEDIAPLAYERIMPL_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include <media/MediaPlayer.h>
#include <media/InputDataSource.h>
#include <media/MediaPlayerObserverInterface.h>

#include "PlayerObserverWorker.h"
#include "audio/audio_manager.h"

namespace media {
class PlayerAudioManger
{
public:
	audio_manager_result_t set_audio_volume(uint16_t volume) { return ::set_audio_volume(volume); }
	int get_audio_volume(void) { return ::get_audio_volume(); }
	audio_manager_result_t set_audio_stream_out(int channels, int sample_rate, int format) {
		return ::set_audio_stream_out(channels, sample_rate, format);
	}
	int get_output_frame_count(void) { return ::get_output_frame_count(); }
	int get_output_frames_byte_size(int frames) { return ::get_output_frames_byte_size(frames); }
	int get_output_bytes_frame_count(unsigned int bytes) { return ::get_output_bytes_frame_count(bytes); }
	int start_audio_stream_out(void *data, int frames) { return ::start_audio_stream_out(data, frames); }
	audio_manager_result_t pause_audio_stream_out(void) { return ::pause_audio_stream_out(); }
	audio_manager_result_t stop_audio_stream_out(void) { return ::stop_audio_stream_out(); }
	audio_manager_result_t reset_audio_stream_out(void) { return ::reset_audio_stream_out(); }
	audio_manager_result_t destroy_audio_stream_out(void) { return ::destroy_audio_stream_out(); }
};

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

	void setDataSource(std::unique_ptr<stream::InputDataSource>);
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

protected:
	PlayerAudioManger mAudioManager;
};
} // namespace media
#endif
