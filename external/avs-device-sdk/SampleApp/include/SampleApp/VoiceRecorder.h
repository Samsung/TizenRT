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

#ifndef ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_VOICE_RECORDER_H_
#define ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_VOICE_RECORDER_H_

#include <mutex>
#include <thread>
#include <stdio.h>

#include <AVSCommon/AVS/AudioInputStream.h>

#include <media/MediaRecorder.h>
#include <media/OutputDataSource.h>

#include <SampleApp/ConsolePrinter.h>

namespace alexaClientSDK {
namespace sampleApp {

class VoiceRecorder : public std::enable_shared_from_this<VoiceRecorder>
{
public:
	class VoiceRecorderOutputDataSource : public media::stream::OutputDataSource {
	public:
		VoiceRecorderOutputDataSource() {}

		~VoiceRecorderOutputDataSource() {
			close();
		}

		void setWriter(std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> writer) {
			ConsolePrinter::simplePrint("VoiceRecorderOutputDataSource:: setWriter");
			m_writer = writer;
		}

		ssize_t write(unsigned char* buf, size_t size) override {
			if (m_writer) {
				//ConsolePrinter::simplePrint("VoiceRecorderOutputDataSource:: write...");
				size_t frames = src_StereoToMono((short *)buf, size / sizeof(short));
				ssize_t ret = m_writer->write(buf, frames);
				if (ret <= 0) {
					ConsolePrinter::simplePrint("Failed to write to stream.");
					return -1;
				}
				return ret;
			} else {
				ConsolePrinter::simplePrint("VoiceRecorderOutputDataSource:: m_writer is nullptr!");
				return 0;
			}
		}

		bool open() override {
			return true;
		}

		bool close() override {
			return true;
		}

		bool isPrepare() override {
			return (m_writer != nullptr);
		}

	private:
		std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> m_writer;

		size_t src_StereoToMono(short *data, size_t len) {
			size_t index = 0;
			size_t channels = 2; // 2 channels stereo
			for (index = 0; index < (len / channels); index++) {
				// New Mono Sample = (L Sample + R Sample) / 2
				// Use the average value simpley.
				data[index] = (data[(index << 1) + 1] + data[index << 1]) >> 1;
			}

			return index;
		}
	};

public:
	/**
	 * Creates a @c VoiceRecorder.
	 *
	 * @param stream The shared data stream to write to.
	 * @return A unique_ptr to a @c VoiceRecorder if creation was successful and @c nullptr otherwise.
	 */
	static std::unique_ptr<VoiceRecorder> create(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

	/**
	 * Stops streaming from the microphone.
	 *
	 * @return Whether the stop was successful.
	 */
	bool stopStreamingMicrophoneData();

	/**
	 * Starts streaming from the microphone.
	 *
	 * @return Whether the start was successful.
	 */
	bool startStreamingMicrophoneData();

	/**
	 * Destructor.
	 */
	~VoiceRecorder();

private:
	/**
	 * Constructor.
	 *
	 * @param stream The shared data stream to write to.
	 */
	VoiceRecorder(std::shared_ptr<avsCommon::avs::AudioInputStream> stream);

	/// Initializes VoiceRecorder
	bool initialize();

	/// The stream of audio data.
	const std::shared_ptr<avsCommon::avs::AudioInputStream> m_audioInputStream;

	/// The writer that will be used to writer audio data into the sds.
	std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> m_writer;

	enum class State {
		IDLE,
		READY,
		RECORDING,
	};

	static std::mutex m_mutex;
	static State m_state;
	static media::MediaRecorder m_mr;
	static std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> g_writer;

#ifdef AUDIO_SIMPLEX_MODE
	static bool m_bListening;
#endif

public:
	static bool registerRecorder(std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> writer);
	static bool unregisterRecorder();
	static bool startRecorder();
	static bool stopRecorder();

#ifdef AUDIO_SIMPLEX_MODE
	static bool isRecordingExpected();
#endif
};

}  // namespace sampleApp
}  // namespace alexaClientSDK

#endif  // ALEXA_CLIENT_SDK_SAMPLEAPP_INCLUDE_SAMPLEAPP_TINYALSAWRAPPER_H_
