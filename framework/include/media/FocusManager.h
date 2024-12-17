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
 * @file media/FocusManager.h
 * @brief Media FocusManager APIs
 */

#ifndef __MEDIA_FOCUSMANAGER_H
#define __MEDIA_FOCUSMANAGER_H

#include <list>
#include <memory>
#include <mutex>
#include <string>

#include <media/FocusRequest.h>

namespace media {

static const int FOCUS_REQUEST_SUCCESS = 0; /* Request accepted, onFocusChange will be called with appropriate values */
static const int FOCUS_REQUEST_GAINED = 1; /* Focus already gained. */
static const int FOCUS_REQUEST_FAIL = -1; /* Requested operation failed */
/*@ToDo: change FOCUS_REQUEST_DELAY value to positive number as it is not a failure case.*/
static const int FOCUS_REQUEST_DELAY = -2; /* Requested operation delayed. onFocusChange will be called with appropriate values */

/**
 * @class 
 * @brief This class is focus manager
 * @details @b #include <media/FocusManager.h>
 * @since TizenRT v2.0
 */
class FocusManager
{
public:
	/**
	 * @brief Get FocusManager Instance
	 * @details @b #include <media/FocusManager.h>
	 * @return Instance of FocusManager
	 * @since TizenRT v2.0
	 */
	static FocusManager &getFocusManager();
	/**
	 * @brief Abandon focus by passing it's Focus Request variable as argument. This is asynchronous function call. Focus will be revoked and request wll be removed from queue.
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest to abandon
	 * @return FOCUS_REQUEST_SUCCESS: successful request and there is no callback for abandon focus.
	 * FOCUS_REQUEST_FAIL: parameter is not valid. Request is ignored.
	 * @since TizenRT v2.0
	 */
	int abandonFocus(std::shared_ptr<FocusRequest> focusRequest);
	/**
	 * @brief Requests focus by passing it's Focus Request variable as argument. This is asynchronous function call. Focus gained / lost is shared in onFocusChange callback. An application must have gained focus before calling MediaPlayer API. MediaPlayer API without focus gain may return error.
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest object corresponding to a media type
	 * @return FOCUS_REQUEST_GAINED: if requesting application already has gained focus. No callback will be called in this case.
	 * FOCUS_REQUEST_FAIL: parameter is not valid. Request is ignored. Callback function will not be called.
	 * FOCUS_REQUEST_SUCCESS: successful request and result will be shared via callback function. FOCUS_REQUEST_SUCCESS does not mean focus is gained and application must wait for callback function before proceeding further.
	 * @since TizenRT v2.0
	 */
	int requestFocus(std::shared_ptr<FocusRequest> focusRequest);

	/**
	 * @brief Requests Transient focus by passing it's Focus Request variable as argument. This is asynchronous function call. Focus gained / lost is shared in onFocusChange callback. An application must have gained focus before calling MediaPlayer API. MediaPlayer API without focus gained may return error.
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest object corresponding to a media type
	* @return FOCUS_REQUEST_GAINED: if requesting application already has gained focus. No callback will be called in this case.
	 * FOCUS_REQUEST_FAIL: parameter is not valid. Request is ignored. Callback function will not be called.
	 * FOCUS_REQUEST_SUCCESS: successful request and result will be shared via callback function. FOCUS_REQUEST_SUCCESS does not mean focus is gained and application must wait for callback function before proceeding further.
	 * @since TizenRT v2.0
	 */
	int requestFocusTransient(std::shared_ptr<FocusRequest> focusRequest);

	/**
	 * @brief Get current focussed stream info
	 * @details @b #include <media/FocusManager.h>
	 * @return STREAM_FOCUS_STATE_ACQUIRED: if stream has focus.
	 * STREAM_FOCUS_STATE_RELEASED: if stream does not have focus.
	 */
	stream_info_t getCurrentStreamInfo(void);

private:
	class FocusRequester
	{
	public:
		FocusRequester(std::shared_ptr<stream_info_t> stream_info, std::shared_ptr<FocusChangeListener> listener);
		bool hasSameId(std::shared_ptr<FocusRequest> focusRequest);
		stream_info_t getStreamInfo(void);
		void notify(int focusChange);

		static bool compare(const FocusRequester a, const FocusRequester b);

	private:
		stream_info_id_t mId;
		stream_policy_t mPolicy;
		std::shared_ptr<FocusChangeListener> mListener;
	};

	FocusManager();
	virtual ~FocusManager() = default;
	void insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, bool isTransientRequest);
	void removeFocusAndNotify(std::shared_ptr<FocusRequest> focusRequest);
	void removeFocusElement(std::shared_ptr<FocusRequest> focusRequest);
	std::list<std::shared_ptr<FocusRequester>> mFocusList;
	std::mutex mFocusLock;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
