/* ****************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#ifndef __MEDIA_DEMUXER_H
#define __MEDIA_DEMUXER_H

#include <tinyara/config.h>
#include <stdio.h>
#include <memory>
#include <media/MediaTypes.h>

namespace media {

/* Container format is enumerated in audio type enumeration */
typedef audio_type_t container_type_t;

/* Demuxer Error Codes */
enum demuxer_error_e : int {
	DEMUXER_ERROR_OUT_OF_MEMORY = -5,
	DEMUXER_ERROR_SYNC_FAILED = -4,
	DEMUXER_ERROR_WANT_DATA = -3,
	DEMUXER_ERROR_NOT_READY = -2,
	DEMUXER_ERROR_UNKNOWN = -1,
	DEMUXER_ERROR_NONE = 0 /* No Error */
};

typedef enum demuxer_error_e demuxer_error_t;

class Demuxer
{
public:
	/**
	 * @brief Create an instance of a derived demuxer class
	 * @param[in] containerType: container type to specify corresponding demuxer
	 * @return shared pointer of the demuxer instance,
	 *         nullptr means create failed.
	 * Note: This create() method should be the only way to create a new demuxer!
	 */
	static std::shared_ptr<Demuxer> create(container_type_t containerType);
	/**
	 * @brief Delete default constructor
	 */
	Demuxer() = delete;
	/**
	 * @brief Constructor & Destructor
	 */
	Demuxer(container_type_t containerType);
	virtual ~Demuxer();
	/**
	 * @brief Get container type of current demuxer
	 * @return container type same as the given type when creating demuxer
	 */
	container_type_t getContainerType(void) { return mContainerType; }
	/**
	 * @brief Set user param
	 * Not in use now!
	 */
	void setParam(void *param) { mParam = param; }
	/**
	 * @brief Get user param
	 * Not in use now!
	 */
	void *getParam(void) { return mParam; }

public:
	/**
	 * @brief Initialize demuxer, e.g. allocate resources
	 *        Derived class should implement it
	 * @return true on success, false on failure.
	 */
	virtual bool initialize(void) = 0;
	/**
	 * @brief Push stream data into demuxer buffer
	 *        Derived class should implement it
	 * @param[in] buf: pointer to the buffer of stream data for input
	 * @param[in] size: length in bytes of the data in buffer
	 * @return number of bytes of data accepted by demuxer on success
	 *         (it may be less than `size` when demuxer buffer is full),
	 *         negative value (see demuxer_error_t) on failure.
	 */
	virtual ssize_t pushData(unsigned char *buf, size_t size) = 0;
	/**
	 * @brief Pull audio elementary stream data from demuxer
	 *        Derived class should implement it
	 * @param[out] buf: pointer to the buffer for output
	 * @param[in] size: capability in bytes of the given buffer
	 * @return number of bytes of data saved in `buf` on success,
	 *         negative value (see demuxer_error_t) on failure,
	 *         DEMUXER_ERROR_WANT_DATA means demuxer expect more input data.
	 */
	virtual ssize_t pullData(unsigned char *buf, size_t size, void *param = nullptr) = 0;
	/**
	 * @brief Get size in bytes that demuxer can accept equivalent input stream data surely
	 *        Derived class should implement it
	 * @return number of bytes of space
	 */
	virtual size_t getAvailSpace(void) = 0;
	/**
	 * @brief Preparse demuxer with inputted stream data. (e.g. pre parsing)
	 *        Derived class should implement it
	 * @return 0 on success, negative value (see demuxer_error_t) on failure.
	 *         DEMUXER_ERROR_WANT_DATA means demuxer expect more input data.
	 */
	virtual int prepare(void) = 0;
	/**
	 * @brief Check if demuxer is ready (prepare succeed)
	 *        Derived class should implement it
	 * @return true means ready, false means not ready.
	 *         in false case, prepare() would be called again.
	 */
	virtual bool isReady(void) = 0;
	/**
	 * @brief Get audio type of the audio elementary stream
	 *        Derived class should implement it
	 * @param[in] param: parameter of derived demuxer related, default nullptr.
	 * @return audio (codec) type of the audio elementary stream.
	 *         in case AUDIO_TYPE_INVALID, demuxer can't give this information,
	 *         it's necessary to pull and parse elementary stream manually.
	 */
	virtual audio_type_t getAudioType(void *param = nullptr) = 0;

private:
	// container type
	container_type_t mContainerType;
	// user parameter
	void *mParam;
};

} // namespace media

#endif /* __MEDIA_DEMUXER_H */
