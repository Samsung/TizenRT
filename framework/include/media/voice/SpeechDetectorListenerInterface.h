/* ****************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/voice/SpeechDetectorListenerInterface.h
 * @brief Media Voice SpeechDetectorListenerInterface APIs
 */

#ifndef __MEDIA_SPEECHDETECTORLISTENERINTERFACE_H
#define __MEDIA_SPEECHDETECTORLISTENERINTERFACE_H

#include <memory>

namespace media {
namespace voice {

enum speech_detect_event_type_e {
	/* All these value need to be aligned with enum audio_device_process_unit_subtype_e */
	SPEECH_DETECT_NONE = 0,
	SPEECH_DETECT_EPD = 1,
	SPEECH_DETECT_SPD = 2,
	SPEECH_DETECT_KD = 3,
	SPEECH_DETECT_LOCAL = 4,
	SPEECH_DETECT_LOCAL0 = 5,
	SPEECH_DETECT_LOCAL1 = 6,
	SPEECH_DETECT_LOCAL2 = 7,
	SPEECH_DETECT_LOCAL3 = 8,
	SPEECH_DETECT_LOCAL4 = 9,
	SPEECH_DETECT_LOCAL5 = 10,
	SPEECH_DETECT_LOCAL6 = 11,
	SPEECH_DETECT_LOCAL7 = 12
};

/**
 * @class
 * @brief This class provides an interface to the user.
 * @details @b #include <media/SpeechDetectorListenerInterface.h>
 */

class SpeechDetectorListenerInterface
{
public:
	virtual void onSpeechDetectionListener(speech_detect_event_type_e event) = 0;
};
} // namespace voice
} // namespace media

#endif
/** @} */ // end of MEDIA group
