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
 * @file media/DataSource.h
 * @brief Media DataSource APIs
 */

#ifndef __MEDIA_DATASOURCE_H
#define __MEDIA_DATASOURCE_H

#include <iostream>
#include <media/MediaTypes.h>

namespace media {

/**
 * @class 
 * @brief This class is audio data structure
 * @details @b #include <media/DataSource.h>
 * @since TizenRT v2.0
 */
class DataSource
{
public:
	/**
	 * @brief Constructs an empty DataSource.
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	DataSource();
	/**
	 * @brief Deconstructs an empty DataSource.
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	~DataSource();
	/**
	 * @brief Constructs a new object provide with audio configuration.
	 * @details @b #include <media/DataSource.h>
	 * param[in] channels   The channels that the channels of audio
	 * param[in] sampleRate The sampleRate that the sample rate of audio
	 * param[in] pcmFormat  The pcmFormat that the pcm format of audio
	 * @since TizenRT v2.0
	 */
	DataSource(unsigned int channels, unsigned int sampleRate, audio_format_type_t pcmFormat);
	/**
	 * @brief Copy constructs for DataSource.
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	DataSource(const DataSource&);
	/**
	 * @brief Operator= for DataSource.
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	DataSource& operator=(const DataSource&);

public:
	/**
	 * @brief open the stream data
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual bool open() = 0;
	/**
	 * @brief close the stream data
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual bool close() = 0;
	/**
	 * @brief Whether data is ready to be read.
	 * @details @b #include <media/DataSource.h>
	 * @since TizenRT v2.0
	 */
	virtual bool isPrepare() = 0;

	/**
	 * @brief Gets the channel count of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @return The channel count of the stream data.
	 * @since TizenRT v2.0
	 */
	unsigned int getChannels();
	/**
	 * @brief Gets the sample rate of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @return The sample rate of the stream data.
	 * @since TizenRT v2.0
	 */
	unsigned int getSampleRate();
	/**
	 * @brief Gets the pcm format of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @return The pcm format of the stream data.
	 * @since TizenRT v2.0
	 */
	audio_format_type_t getPcmFormat();

	/**
	 * @brief Sets the channel count of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @param[in] channels The channels that the channel count of stream data
	 * @since TizenRT v2.0
	 */
	void setChannels(unsigned int channels);
	/**
	 * @brief Sets the sample rate of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @param[in] sampleRate The sampleRate that the sample rate of stream data
	 * @since TizenRT v2.0
	 */
	void setSampleRate(unsigned int sampleRate);
	/**
	 * @brief Sets the pcm format of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @param[in] pcmFormat The pcmFormat that the pcm format of stream data
	 * @since TizenRT v2.0
	 */
	void setPcmFormat(audio_format_type_t pcmFormat);
	/**
	 * @brief Sets the audio type of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @param[in] audioType The audioType that the audio type of stream data
	 * @since TizenRT v2.0
	 */
	void setAudioType(audio_type_t audioType);
	/**
	 * @brief Gets the audio type of the stream data.
	 * @details @b #include <media/DataSource.h>
	 * @return The audio type of the stream data.
	 * @since TizenRT v2.0
	 */
	audio_type_t getAudioType();

private:
	unsigned char mChannels;
	unsigned int mSampleRate;
	audio_format_type_t mPcmFormat;
	audio_type_t mAudioType;
};
} // namespace media

#endif
/** @} */ // end of MEDIA group
