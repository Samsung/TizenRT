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
 * @defgroup MEDIA MEDIA
 * @brief Provides APIs for Media Framework 
 * @{
 */

/**
 * @file media/MediaRecorder.h
 * @brief Media MediaRecorder APIs
 */

#ifndef __MEDIA_MEDIARECORDER_H
#define __MEDIA_MEDIARECORDER_H

#include <memory>
#include <media/OutputDataSource.h>
#include <media/MediaRecorderObserverInterface.h>

namespace media {

class MediaRecorderImpl;

/**
 * @brief result of call the apis
 * @details @b #include <media/MediaRecorder.h>
 * @since TizenRT v2.0
 */
enum recorder_error_e : int {
	/** MediaRecorder Error case */
	RECORDER_ERROR_DEVICE_SUSPENDED = -10,
	RECORDER_ERROR_NOT_ALIVE,
	RECORDER_ERROR_INVALID_STATE,
	RECORDER_ERROR_INVALID_OPERATION,
	RECORDER_ERROR_INVALID_PARAM,
	RECORDER_ERROR_INTERNAL_OPERATION_FAILED,
	RECORDER_ERROR_FILE_OPEN_FAILED,
	RECORDER_ERROR_OUT_OF_MEMORY,
	RECORDER_ERROR_DEVICE_NOT_SUPPORTED,
	RECORDER_ERROR_DEVICE_DEAD,
	/** MediaRecorder Success case */
	RECORDER_ERROR_NONE = 0
};

typedef enum recorder_error_e recorder_error_t;
const int RECORDER_OK = RECORDER_ERROR_NONE;
typedef int recorder_result_t;

/**
 * @class 
 * @brief This class implements the MediaRecorder capability agent.
 * @details @b #include <media/MediaRecorder.h>
 * @since TizenRT v2.0
 */
class MediaRecorder
{
public:
	/**
	 * @brief Construct an empty MediaRecorder.
	 * @details @b #include <media/MediaRecorder.h>
	 * @since TizenRT v2.0
	 */
	MediaRecorder();
	
	/**
	 * @brief Deconstruct an empty MediaRecorder.
	 * @details @b #include <media/MediaRecorder.h>
	 * @since TizenRT v2.0
	 */
	~MediaRecorder();
	
	/**
	 * @brief Create MediaRecorder for capturing
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The result of the create operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t create();
	
	/**
	 * @brief Destroy MediaRecorder
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The result of the destroy operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t destroy();
	
	/**
	 * @brief Allocate and prepare resources related to the recorder, it should be called before start
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The result of the prepare operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t prepare();
	
	/**
	 * @brief Release allocated resources related to the recorder.
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The result of the unpreapre operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t unprepare();

	/**
	 * @brief Reset allocated resources related to the recorder, after the API, recorder is in state RECORDER_STATE_IDLE.
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The result of the reset operation
	 * @since TizenLite v5.0
	 */
	recorder_result_t reset();
	
	/**
	 * @brief Start recording.
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a asynchronous API
	 * Order to MediaRecordWorker begin recording through the queue
	 * @return The result of the unpreapre operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t start();
	
	/**
	 * @brief Pause recording.
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a asynchronous API
	 * Order to MediaRecordWorker pause recording through the queue
	 * @return The result of the pause operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t pause();
	
	/**
	 * @brief Stop recording.
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a asynchronous API
	 * Order to MediaRecordWorker stop recording through the queue
	 * @return The result of the stop operation
	 * @since TizenRT v2.0
	 */
	recorder_result_t stop();
	
	/**
	 * @brief Get the current volume
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The value of current MIC volume
	 * @since TizenRT v2.0
	 */
	recorder_result_t getVolume(uint8_t *vol);
	
	/**
	 * @brief Get the Maximum input gain
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return The value of the maximum MIC gain
	 * @since TizenRT v2.0
	 */
	recorder_result_t getMaxVolume(uint8_t *vol);

	/**
	 * @brief Set the volume adjusted
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @param[in] vol The vol that the value of mic volume
	 * @return The result of setting the mic volume
	 * @since TizenRT v2.0
	 */
	recorder_result_t setVolume(uint8_t vol);
	
	/**
	 * @brief Set the DataSource of output data
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @param[in] dataSource The dataSource that the config of output data
	 * @return The result of setting the datasource
	 * @since TizenRT v2.0
	 */
	recorder_result_t setDataSource(std::unique_ptr<stream::OutputDataSource> dataSource);
	
	/**
	 * @brief Set the observer of MediaRecorder
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * It sets the user's function
	 * @param[in] observer The callback to be set for Media Recorder Observer.
	 * @return The result of setting the observer
	 * @since TizenRT v2.0
	 */
	recorder_result_t setObserver(std::shared_ptr<MediaRecorderObserverInterface> observer);

	/**
	 * @brief Set limitation of recording time by given value(second), will be stopped when it reaches that.
	 * This should be called after setDataSource but before prepare
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * It sets the user's function
 	 * @param[in] Max duration(second), No limitation If zero or negative.
	 * @return The result of setting the duration
	 * @since TizenRT v2.0
	 */
	recorder_result_t setDuration(int second);

	/**
	 * @brief Limit the size of the file to be recoreded.
	 * This should be called after setDataSource but before prepare
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous api
	 * It sets the user's function
	 * @param[in] Max size(byte), No limitation If zero or negative.
	 * @return The result of setting the size
	 * @since TizenRT v2.0
	 */
	recorder_result_t setFileSize(int byte);

	/**
	 * @brief MediaRecorder operator==
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * Compares the MediaRecorder objects for equality
	 * @return The result of the compare operation for MediaRecorder object
	 * @since TizenRT v2.0
	 */
	bool operator==(const MediaRecorder& rhs);

	/**
	 * @brief Get whether the Recorder is Recording
	 * @details @b #include <media/MediaRecorder.h>
	 * This function is a synchronous API
	 * @return true if Recorder is Recording
	 * @since TizenRT v2.1 PRE
	 */
	bool isRecording();

private:
	std::shared_ptr<MediaRecorderImpl> mPMrImpl;
	uint64_t mId;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
