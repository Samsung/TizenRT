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

#include "SampleApp/VoiceRecorder.h"
#ifdef AUDIO_SIMPLEX_MODE
#include "MediaPlayer/MediaPlayer.h"
#endif

#include <unistd.h>
#include <thread>

namespace alexaClientSDK {
namespace sampleApp {

using namespace avsCommon::avs;
using namespace media;
using namespace media::stream;

std::mutex VoiceRecorder::m_mutex;
VoiceRecorder::State VoiceRecorder::m_state = VoiceRecorder::State::IDLE;
media::MediaRecorder VoiceRecorder::m_mr;
std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> VoiceRecorder::g_writer = nullptr;

#ifdef AUDIO_SIMPLEX_MODE
bool VoiceRecorder::m_bListening = false;
#endif
std::unique_ptr<VoiceRecorder> VoiceRecorder::create(std::shared_ptr<AudioInputStream> stream)
{
	if (!stream) {
		ConsolePrinter::simplePrint("Invalid stream passed to VoiceRecorder");
		return nullptr;
	}

	std::unique_ptr<VoiceRecorder> voiceRecorder(new VoiceRecorder(stream));
	if (!voiceRecorder->initialize()) {
		ConsolePrinter::simplePrint("Failed to initialize VoiceRecorder");
		return nullptr;
	}

	ConsolePrinter::simplePrint("VoiceRecorder::create ok");
	return voiceRecorder;
}

VoiceRecorder::VoiceRecorder(std::shared_ptr<AudioInputStream> stream)
	: m_audioInputStream(stream)
{
}

VoiceRecorder::~VoiceRecorder()
{
	stopRecorder();

	unregisterRecorder();
}

bool VoiceRecorder::initialize()
{
	m_writer = m_audioInputStream->createWriter(AudioInputStream::Writer::Policy::NONBLOCKABLE);
	if (!m_writer) {
		ConsolePrinter::simplePrint("Failed to create stream writer");
		return false;
	}

	if (!registerRecorder(m_writer)) {
		return false;
	}

	ConsolePrinter::simplePrint("VoiceRecorderOutputDataSource:: initialize finished");
	return true;
}

bool VoiceRecorder::startStreamingMicrophoneData()
{
	ConsolePrinter::simplePrint("startStreamingMicrophoneData");
#ifndef AUDIO_SIMPLEX_MODE
	if (!startRecorder()) {
		ConsolePrinter::simplePrint("startStreamingMicrophoneData failed");
		return false;
	}
#endif
	ConsolePrinter::simplePrint("startStreamingMicrophoneData succeed");
	return true;
}

bool VoiceRecorder::stopStreamingMicrophoneData()
{
	ConsolePrinter::simplePrint("stopStreamingMicrophoneData");
#ifndef AUDIO_SIMPLEX_MODE
	if (!stopRecorder()) {
		ConsolePrinter::simplePrint("stopStreamingMicrophoneData failed");
		return false;
	}
#endif
	ConsolePrinter::simplePrint("stopStreamingMicrophoneData succeed");
	return true;
}

bool VoiceRecorder::registerRecorder(std::shared_ptr<avsCommon::avs::AudioInputStream::Writer> writer)
{
	ConsolePrinter::simplePrint("registerRecorder");
	std::lock_guard<std::mutex> lock(m_mutex);

	if (!writer) {
		ConsolePrinter::simplePrint("registerRecorderFailed: null writer");
		return false;
	}

	if (m_state == State::IDLE) {
		g_writer = writer;
		m_state = State::READY;
	} else {
		ConsolePrinter::simplePrint("registerRecorderFailed: invalid state");
		return false;
	}

	return true;
}

bool VoiceRecorder::unregisterRecorder()
{
	ConsolePrinter::simplePrint("unregisterRecorder");
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_state == State::READY) {
		g_writer = nullptr;
		m_state = State::IDLE;
	} else {
		ConsolePrinter::simplePrint("unregisterRecorderFailed: invalid state");
		return false;
	}

	return true;
}

bool VoiceRecorder::startRecorder()
{
	ConsolePrinter::simplePrint("startRecorder");
	std::lock_guard<std::mutex> lock(m_mutex);

#ifdef AUDIO_SIMPLEX_MODE
	if (alexaClientSDK::mediaPlayer::MediaPlayer::isPlayerActive()) {
		ConsolePrinter::simplePrint("startRecorder: start recording after player deactivated...");
		m_bListening = true;
		return true;
	} else if (m_bListening) {
		ConsolePrinter::simplePrint("startRecorder: player deactivated, now start recording...");
		m_bListening = false;
	}
#endif

	if (m_state == State::READY) {
		ConsolePrinter::simplePrint("startRecorder:: m_mr.create");
		auto ret = m_mr.create();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to create media recorder");
			return false;
		}

		auto src = std::move(std::unique_ptr<VoiceRecorderOutputDataSource>(new VoiceRecorderOutputDataSource));
		src->setWriter(g_writer);
		ConsolePrinter::simplePrint("startRecorder:: m_mr.setDataSource");
		ret = m_mr.setDataSource(std::move(src));
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to setDataSource for media recorder");
			m_mr.destroy();
			return false;
		}

		ConsolePrinter::simplePrint("startRecorder:: m_mr.prepare");
		ret = m_mr.prepare();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to prepare media recorder");
			m_mr.destroy();
			return false;
		}

		ConsolePrinter::simplePrint("startRecorder:: m_mr.start");
		ret = m_mr.start();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to start media recorder");
			m_mr.unprepare();
			m_mr.destroy();
			return false;
		}

		m_state = State::RECORDING;
	} else {
		ConsolePrinter::simplePrint("media recorder can not start");
		return false;
	}

	return true;
}

bool VoiceRecorder::stopRecorder()
{
	ConsolePrinter::simplePrint("stopRecorder");
	std::lock_guard<std::mutex> lock(m_mutex);

#ifdef AUDIO_SIMPLEX_MODE
	m_bListening = false;
#endif

	if (m_state == State::RECORDING) {
		ConsolePrinter::simplePrint("stopRecorder:: m_mr.stop");
		auto ret = m_mr.stop();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to stop media recorder");
			//return false;
		}

		ConsolePrinter::simplePrint("stopRecorder:: m_mr.unprepare");
		ret = m_mr.unprepare();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to unprepare media recorder");
			//return false;
		}

		ConsolePrinter::simplePrint("stopRecorder:: m_mr.destroy");
		ret = m_mr.destroy();
		if (ret != media::RECORDER_ERROR_NONE) {
			ConsolePrinter::simplePrint("Failed to destroy media recorder");
			return false;
		}

		m_state = State::READY;
	} else {
		ConsolePrinter::simplePrint("media recorder isn't running");
		return true;
	}

	return true;
}

#ifdef AUDIO_SIMPLEX_MODE
bool VoiceRecorder::isRecordingExpected()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_bListening;
}
#endif

}  // namespace sampleApp
}  // namespace alexaClientSDK
