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

static const int FOCUS_REQUEST_SUCCESS = 0;
static const int FOCUS_REQUEST_FAIL = -1;
static const int FOCUS_REQUEST_DELAY = -2;

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
	 * @brief Abandon Focus
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest to abandon
	 * @return return FOCUS_REQUEST_SUCCESS on Success, else return FOCUS_REQUEST_FAIL
	 * @since TizenRT v2.0
	 */
	int abandonFocus(std::shared_ptr<FocusRequest> focusRequest);
	/**
	 * @brief Request Focus
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest to request
	 * @return return FOCUS_REQUEST_SUCCESS on immediate GAIN and FOCUS_REQUEST_DELAY for GAIN later, else return FOCUS_REQUEST_FAIL
	 * @since TizenRT v2.0
	 */
	int requestFocus(std::shared_ptr<FocusRequest> focusRequest);

	/**
	 * @brief Request Transient Focus
	 * @details @b #include <media/FocusManager.h>
	 * param[in] focusRequest FocusRequest to request
	 * @return return FOCUS_REQUEST_SUCCESS on immediate GAIN and FOCUS_REQUEST_DELAY for GAIN later, else return FOCUS_REQUEST_FAIL
	 * @since TizenRT v2.0
	 */
	int requestFocusTransient(std::shared_ptr<FocusRequest> focusRequest);

	/**
	 * @brief Get current focussed stream info
	 * @details @b #include <media/FocusManager.h>
	 * @return return STREAM_FOCUS_STATE_ACQUIRED if stream has focus, else return STREAM_FOCUS_STATE_RELEASED
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

	FocusManager() = default;
	virtual ~FocusManager() = default;
	int insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, bool isTransientRequest);
	void removeFocusElement(std::shared_ptr<FocusRequest> focusRequest);
	std::list<std::shared_ptr<FocusRequester>> mFocusList;
	std::mutex mFocusLock;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
