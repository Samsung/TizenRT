/******************************************************************
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

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/voice/SpeechDetector.h
 * @brief Media SpeechDetector APIs
 */

#ifndef __MEDIA_SPEECH_DETECTOR_H
#define __MEDIA_SPEECH_DETECTOR_H

#include <functional>
#include <memory>

namespace media {
namespace voice {

/**
 * @class 
 * @brief This class is speech detector
 * @details @b #include <media/voice/SpeechDetector.h>
 * @since TizenRT v2.0
 */
class SpeechDetector
{
public:
	/**
	 * @brief Get SpeechDetector Instance
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * @return Instance of SpeechDetector
	 * @since TizenRT v2.0
	 */
	static SpeechDetector *instance();
	/**
	 * @brief Init KeywordDetector
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * param[in] samprate KeywordDetector sample rate
	 * param[in] channels KeywordDetector channels
	 * @return Return success if KeywordDetector is successfully init
	 * @since TizenRT v2.0
	 */
	virtual bool initKeywordDetect(uint32_t samprate, uint8_t channels) = 0;
	/**
	 * @brief Init EndPointDetector
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * param[in] samprate EndPointDetector sample rate
	 * param[in] channels EndPointDetector channels
	 * @return Return success if EndPointDetector is successfully init
	 * @since TizenRT v2.0
	 */
	virtual bool initEndPointDetect(uint32_t samprate, uint8_t channels) = 0;
	/**
	 * @brief Deinit KeywordDetector
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * @return Return success if KeywordDetector is successfully deinit
	 * @since TizenRT v2.0
	 */
	virtual bool deinitKeywordDetect() = 0;
	/**
	 * @brief Deinit EndPointDetector
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * @return Return success if EndPointDetector is successfully deinit
	 * @since TizenRT v2.0
	 */
	virtual bool deinitEndPointDetect() = 0;
	/**
	 * @brief Start Detecting Keyword
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * param[in] timeout Keyword detect timout in second
	 * @return Return success if Keyword is detected
	 * @since TizenRT v2.0
	 */
	virtual bool startKeywordDetect(int timeout) = 0;
	/**
	 * @brief Start Detecting EndPoint
	 * @details @b #include <media/voice/SpeechDetector.h>
	 * param[in] timeout EndPoint detect timout in second
	 * @return Return success if EndPoint is detected
	 * @since TizenRT v2.0
	 */
	virtual bool startEndPointDetect(int timeout) = 0;
protected:
	SpeechDetector() = default;
};

} // namespace voice
} // namespace media

#endif
/** @} */ // end of MEDIA group
