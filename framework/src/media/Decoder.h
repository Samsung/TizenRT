/* ****************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************/

#include <tinyara/config.h>
#include <stdio.h>

#ifdef CONFIG_AUDIO_CODEC
#include <audiocodec/streaming/player.h>
#endif

namespace media {

class Decoder
{
public:
	Decoder();
	Decoder(const Decoder *source);
	~Decoder();

public:
	size_t pushData(unsigned char *buf, size_t size);
	bool getFrame(unsigned char *buf, size_t *size, unsigned int *sampleRate, unsigned short *channels);
	bool empty();
	size_t getAvailSpace();

private:
#ifdef CONFIG_AUDIO_CODEC
	static int _configFunc(void *user_data, int audio_type, void *dec_ext);
	pv_player_t mPlayer;
#endif
};
} // namespace media
