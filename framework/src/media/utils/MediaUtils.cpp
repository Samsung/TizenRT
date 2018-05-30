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

#include "MediaUtils.h"
#include <debug.h>

namespace media {
namespace utils {

void toLowerString(std::string &str)
{
	for (char& c : str) {
		if ('A' <= c && c <= 'Z') {
			c += ('a' - 'A');
		}
	}
}

void toUpperString(std::string &str)
{
	for (char& c : str) {
		if ('a' <= c && c <= 'z') {
			c -= ('a' - 'A');
		}
	}
}

audio_type_t getAudioTypeFromPath(std::string datapath)
{
	std::string basename = datapath.substr(datapath.find_last_of("/") + 1);
	std::string extension;

	if (basename.find(".") == std::string::npos) {
		extension = "";
	} else {
		extension = basename.substr(basename.find_last_of(".") + 1);
	}

	toLowerString(extension);

	if (extension.compare("mp3") == 0) {
		medvdbg("audio type : mp3\n");
		return AUDIO_TYPE_MP3;
	} else if ((extension.compare("aac") == 0) || (extension.compare("mp4") == 0)) {
		medvdbg("audio type : aac\n");
		return AUDIO_TYPE_AAC;
	} else if ((extension.compare("opus") == 0)) {
		medvdbg("audio type : opus\n");
		return AUDIO_TYPE_OPUS;
	} else if ((extension.compare("flac") == 0)) {
		medvdbg("audio type : flac\n");
		return AUDIO_TYPE_FLAC;
	} else if ((extension.compare("") == 0) || (extension.compare("pcm") == 0) || (extension.compare("raw") == 0)) {
		medvdbg("audio type : pcm\n");
		return AUDIO_TYPE_PCM;
	} else {
		medvdbg("audio type : unknown\n");
		return AUDIO_TYPE_INVALID;
	}
}
} // namespace util
} // namespace media
