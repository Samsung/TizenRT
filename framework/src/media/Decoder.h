#include <stdio.h>

#ifdef CONFIG_AUDIO_CODEC
#include <audiocodec/streaming/player.h>
#endif

namespace media {

class Decoder
{
public:
	Decoder();
	Decoder(const Decoder* source);
	~Decoder();

public:
	size_t pushData(unsigned char* buf, size_t size);
	bool getFrame(unsigned char* buf, size_t* size, unsigned int* sampleRate, unsigned short* channels);
	bool empty();
	size_t getDataSpace();

private:
#ifdef CONFIG_AUDIO_CODEC
	static int _configFunc(void* user_data, int audio_type, void* dec_ext);
	pv_player_p mPlayer;
#endif
};
} // namespace media
