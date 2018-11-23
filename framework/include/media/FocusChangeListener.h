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
 * @file media/FocusChangeListener.h
 * @brief Media FocusChangeListener APIs
 */

#ifndef __MEDIA_FOCUSCHANGELISTENER_H
#define __MEDIA_FOCUSCHANGELISTENER_H

namespace media {
static const int FOCUS_NONE = 0;
static const int FOCUS_GAIN = 1;
static const int FOCUS_LOSS = -1;

/**
 * @class 
 * @brief This class is callback interface of FocusRequest
 * @details @b #include <media/FocusChangeListener.h>
 * @since TizenRT v2.0
 */
class FocusChangeListener
{
public:
	/**
	 * @brief Called when a focus is changed
	 * @details @b #include <media/FocusChangeListener.h>
	 * @since TizenRT v2.0
	 */
	virtual void onFocusChange(int focusChange) {};
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
