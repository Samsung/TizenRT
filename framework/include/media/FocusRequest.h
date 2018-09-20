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
 * @file media/FocusRequest.h
 * @brief Media FocusRequest APIs
 */

#ifndef __MEDIA_FOCUSREQUEST_H
#define __MEDIA_FOCUSREQUEST_H

#include <memory>
#include <string>
#include <media/FocusChangeListener.h>

namespace media {
/**
 * @class 
 * @brief This class is focus request
 * @details @b #include <media/FocusRequest.h>
 * @since TizenRT v2.0
 */
class FocusRequest
{
public:
	/**
	 * @class 
	 * @brief This class is FocusRequest builder
	 * @details @b #include <media/FocusRequest.h>
	 * @since TizenRT v2.0
	 */
	class Builder
	{
	public:
		/**
		 * @brief constructor of FocusRequest::Builder
		 * @details @b #include <media/FocusRequest.h>
		 * @since TizenRT v2.0
		 */
		Builder();
		/**
		 * @brief set FocusChangeListener of FocusRequest
		 * @details @b #include <media/FocusRequest.h>
		 * param[in] listener shared_ptr of FocusChangeListener
	 	 * @return FocusRequest::Builder instance
		 * @since TizenRT v2.0
		 */
		Builder &setFocusChangeListener(std::shared_ptr<FocusChangeListener> listener);
		/**
		 * @brief build FocusRequest instance
		 * @details @b #include <media/FocusRequest.h>
	 	 * @return shared_ptr of FocusRequest
		 * @since TizenRT v2.0
		 */
		std::shared_ptr<FocusRequest> build();

	private:
		std::string mId;
		std::shared_ptr<FocusChangeListener> mListener;
	};

	/**
	 * @brief Get FocusRequest Id
	 * @details @b #include <media/FocusRequest.h>
	 * @return FocusRequest Id
	 * @since TizenRT v2.0
	 */
	std::string getId();
	/**
	 * @brief Get FocusChangeListener of FocusRequest
	 * @details @b #include <media/FocusRequest.h>
	 * @return shared_ptr of FocusChangeListener
	 * @since TizenRT v2.0
	 */
	std::shared_ptr<FocusChangeListener> getListener();

private:
	std::string mId;
	std::shared_ptr<FocusChangeListener> mListener;
};
} // namespace media
#endif
/** @} */ // end of MEDIA group
