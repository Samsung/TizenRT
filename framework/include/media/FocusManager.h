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
#include <functional>
#include <map>
#include <media/FocusRequest.h>

namespace media {
/**
 * @brief Enumeration for stream focus state.
 */
enum stream_focus_state_e {
	STREAM_FOCUS_STATE_RELEASED, /**< Focus state for release */
	STREAM_FOCUS_STATE_ACQUIRED, /**< Focus state for acquisition */
	STREAM_FOCUS_STATE_DUCKED	/**< Focus state for ducking*/
};

/**
 * @brief Used to indicate change in audio focus state, or a request of audio focus.
*/
enum focus_state_e {
	/* Used to indicate no audio focus change */
	FOCUS_NONE = 0,
	/* Used to indicate a gain of audio focus, or a request of audio focus, of unknown duration. E.g. music playback */
	FOCUS_GAIN = 1,
	/* Used to indicate a temporary gain or request of audio focus, anticipated to last a short amount of time. E.g. event notification playback */
	FOCUS_GAIN_TRANSIENT = 2,
	/* Used to indicate a loss of audio focus of unknown duration */
	FOCUS_LOSS = 3,
	/* Used to indicate a temporary loss of audio focus, anticipated to last a short amount of time */
	FOCUS_LOSS_TRANSIENT = 4,
	/* Used to indicate a temporary gain or request of audio focus, anticipated to last a short amount of time, and where it is acceptable for other audio applications to keep playing in duck mode i.e. lowered volume. E.g. UI tap sounds */
	FOCUS_GAIN_TRANSIENT_MAY_DUCK = 5,
	/* Used to indicate a temporary loss of audio focus, anticipated to last a short amount of time, and where it is acceptable to keep playing in duck mode */
	FOCUS_LOSS_TRANSIENT_CAN_DUCK = 6
};

typedef enum stream_focus_state_e stream_focus_state_t;
typedef enum focus_state_e focus_state_t;

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
	using FocusLossListener = std::function<void()>;
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
	 * @param[in] focusRequest FocusRequest to request
	 * @return return FOCUS_REQUEST_SUCCESS on immediate GAIN and FOCUS_REQUEST_DELAY for GAIN later, else return FOCUS_REQUEST_FAIL
	 * @since TizenRT v2.0
	 */
	int requestFocusTransient(std::shared_ptr<FocusRequest> focusRequest);
	/**
	 * @brief Requests audio focus with a specific focus state, asynchronously.
	 * @details @b #include <media/FocusManager.h>
	 * @param[in] focusRequest FocusRequest to request
	 * @param[in] focusState The desired focus_state_t for this request. Valid focus states are
	 *                       FOCUS_GAIN, FOCUS_GAIN_TRANSIENT, & FOCUS_GAIN_TRANSIENT_MAY_DUCK.
	 * @return Returns FOCUS_REQUEST_SUCCESS if the request is successfully registered. The actual focus change result will be
	 *         communicated asynchronously via the FocusChangeListener.
	 *         Returns FOCUS_REQUEST_FAIL if the request is invalid.
	 */
	int requestFocus(std::shared_ptr<FocusRequest> focusRequest, focus_state_t focusState);
	/**
	 * @brief Retrieves the current focus state of a stream synchronously.
	 * @details @b #include <media/FocusManager.h>
	 * @param[in] streamId Id of the stream whose focus state is being queried.
	 * @return Current focus state of the stream:
	 *         - @c STREAM_FOCUS_STATE_ACQUIRED: The stream currently holds audio focus i.e. at the front.
	 *         - @c STREAM_FOCUS_STATE_DUCKED: The stream is ducked.
	 *         - @c STREAM_FOCUS_STATE_RELEASED: In all other cases.
	 */
	stream_focus_state_t getStreamFocusState(stream_info_id_t streamId);
	/**
	 * @brief Get current focussed player stream info
	 * @details @b #include <media/FocusManager.h>
	 * @return return current focused player stream_info structure. If list is empty, return {0, STREAM_TYPE_MEDIA}
	 */
	stream_info_t getCurrentStreamInfo(void);

private:
	/**
	 * @brief  It registers player focus loss listener. This function is called whenever player is started.
	 * @details @b #include <media/FocusManager.h>
	 */
	void registerPlayerFocusLossListener(FocusLossListener playerFocusLossCallback, stream_info_id_t id);
	/**
	 * @brief It unregisters player focus loss listener. This function is called whenever player is paused or stopped.
	 * @details @b #include <media/FocusManager.h>
	 * @return void
	*/
	void unregisterPlayerFocusLossListener(stream_info_id_t id);

	/*@ToDo: Find an alternative of using friend keyword*/
	friend class MediaPlayerImpl;

	class FocusRequester
	{
	public:
		FocusRequester(std::shared_ptr<stream_info_t> stream_info, std::shared_ptr<FocusChangeListener> listener, focus_state_t focusState);
		bool hasSameId(std::shared_ptr<FocusRequest> focusRequest);
		stream_info_t getStreamInfo(void);
		focus_state_t getFocusState(void);
		void notify(int focusChange);
		static bool compare(const FocusRequester a, const FocusRequester b);

	private:
		stream_info_id_t mId;
		stream_policy_t mPolicy;
		std::shared_ptr<FocusChangeListener> mListener;
		focus_state_t mFocusState;
	};

	FocusManager();
	virtual ~FocusManager() = default;
	void insertFocusElement(std::shared_ptr<FocusRequest> focusRequest, focus_state_t focusState);
	void removeFocusAndNotify(std::shared_ptr<FocusRequest> focusRequest);
	void removeFocusElement(std::shared_ptr<FocusRequest> focusRequest);
	void callFocusLossListener(stream_policy_t policy, std::shared_ptr<FocusRequester> duckedFocusRequester);
	std::list<std::shared_ptr<FocusRequester>> mPlayerFocusList;
	std::mutex mFocusLock;
	std::mutex mPlayerFocusListAccessLock;
	std::map<stream_info_id_t, FocusLossListener> mPlayerFocusLossListeners;
	/**
	 * @brief Holds a reference to the audio focus requester in focus list that is currently in a "ducked" state.
	 * @details "Ducking" refers to the scenario where a new audio focus request with the flag FOCUS_GAIN_TRANSIENT_MAY_DUCK is granted.
	 * Instead of completely pausing the current audio focus holder, its volume is lowered (ducked), allowing the new transient audio to
	 * be played simultaneously. It is maintained for managing audio focus transitions, such as restoring the ducked requester's state
	 * when the transient audio finishes or notifying it of a permanent focus loss.
	 *
	 * @note Position in Focus List:
	 * During ducking, main audio focus requester will be at the front & mDuckedFocusRequester will be somewhere back in list.
	 * When ducking ends, mDuckedFocusRequester is set to null.
	 */
	std::shared_ptr<FocusRequester> mDuckedFocusRequester;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
