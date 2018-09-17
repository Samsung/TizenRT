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

/**
 * @ingroup MEDIA
 * @{
 */

/**
 * @file media/MediaRecorderObserverInterface.h
 * @brief Media MediaRecorderObserverInterface APIs
 */

#ifndef __MEDIA_MEDIARECOREROBSERVERINTERFACE_H
#define __MEDIA_MEDIARECOREROBSERVERINTERFACE_H

#include <memory>
#include <media/MediaTypes.h>

namespace media {

class MediaRecorder;
enum recorder_error_e : int;
typedef enum recorder_error_e recorder_error_t;

/**
 * @class
 * @brief This class provides an interface to the user.
 * @details @b #include <media/MediaRecorderObserverInterface.h>
 * This class informs the user of the recording state of MediaRecorder
 * This class informs the user of the error state of MediaRecorder
 * @since TizenRT v2.0
 */

class MediaRecorderObserverInterface
{
public:
	/**
	 * @brief informs the user of the recording has begun.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordStarted(MediaRecorder& mediaRecorder) = 0;
	/**
	 * @brief informs the user of the recording has paused.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordPaused(MediaRecorder& mediaRecorder) = 0;
	/**
	 * @brief informs the user of the recording has finished.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordFinished(MediaRecorder& mediaRecorder) = 0;
	/**
	 * @brief informs the user of the error state of recorder start operation
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordStartError(MediaRecorder& mediaRecorder, recorder_error_t errCode) = 0;
	/**
	 * @brief informs the user of the error state of recorder pause operation
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordPauseError(MediaRecorder& mediaRecorder, recorder_error_t errCode) = 0;
	/**
	 * @brief informs the user of the error state of recorder stop operation
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordStopError(MediaRecorder& mediaRecorder, recorder_error_t errCode) = 0;
	/**
	 * @brief informs the user that data in the output-buffer reach the
	 * threshold, and it's possible to fetch data now.
	 * @remark user need to override this method when the recorder working
	 * with a BufferOutputDataSource.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordBufferDataReached(MediaRecorder& mediaRecorder, std::shared_ptr<unsigned char> data, size_t size) {}
	/**
	 * @brief informs the user the recorder buffer state: overrun.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordBufferOverrun(MediaRecorder& mediaRecorder) {}
	/**
	 * @brief informs the user the recorder buffer state: underrun.
	 * @details @b #include <media/MediaRecorderObserverInterface.h>
	 * @since TizenRT v2.0
	 */
	virtual void onRecordBufferUnderrun(MediaRecorder& mediaRecorder) {}
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
