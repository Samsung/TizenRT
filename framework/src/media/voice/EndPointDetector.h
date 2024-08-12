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
#ifndef __MEDIA_ENDPOINT_DETECTOR_H
#define __MEDIA_ENDPOINT_DETECTOR_H

#include <functional>

#include <media/voice/SpeechDetector.h>
#include "../audio/audio_manager.h"

namespace media {
namespace voice {

typedef void (*EPDResultListener)(audio_device_process_unit_subtype_e event);

enum EPD_STATE{
	EPD_STATE_IDLE = 0,
	EPD_STATE_IN_PROGRESS = 1,
	EPD_STATE_STOP = 2
};

class EndPointDetector
{
public:
	virtual bool init(uint32_t samprate, uint8_t channels) = 0;
	virtual void deinit() = 0;
	virtual bool startEndPointDetect(int timeout)
	{
		return false;
	}

	virtual bool startEndPointDetect(void)
	{
		return false;
	}
	virtual bool stopEndPointDetect(void) = 0;
	virtual bool detectEndPoint(std::shared_ptr<unsigned char> sample, int size)
	{
		return false;
	}
	virtual bool detectEndPoint(void)
	{
		return false;
	}
	virtual bool waitEndPoint(int timeout) = 0;
	virtual void registerEPDResultListener(EPDResultListener epdResultCallback) = 0;
	virtual EPD_STATE getEPDState(void)
	{
		return mEPDState;
	}

protected:
	volatile EPD_STATE mEPDState;
};

} // namespace voice
} // namespace media

#endif
