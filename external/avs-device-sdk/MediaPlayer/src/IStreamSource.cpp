/*
 * Copyright 2017-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <cstring>

#include <AVSCommon/Utils/Logger/Logger.h>
#include <MediaPlayer/IStreamSource.h>

namespace alexaClientSDK {
namespace mediaPlayer {

/// String to identify log entries originating from this file.
static const std::string TAG("IStreamSource");

/**
 * Create a LogEntry using this file's TAG and the specified event string.
 *
 * @param The event string for this @c LogEntry.
 */
#define LX(event) alexaClientSDK::avsCommon::utils::logger::LogEntry(TAG, event)

std::unique_ptr<IStreamSource> IStreamSource::create(
    std::shared_ptr<std::istream> stream,
    bool repeat) {
    std::unique_ptr<IStreamSource> result(new IStreamSource(std::move(stream), repeat));
    if (result->init()) {
        return result;
    }
    return nullptr;
};

IStreamSource::IStreamSource(std::shared_ptr<std::istream> stream, bool repeat) :
        SourceInterface{"IStreamSource"},
        m_stream{stream},
        m_repeat{repeat} {};

IStreamSource::~IStreamSource() {

}

bool IStreamSource::init() {
    return true;
}

size_t IStreamSource::readData(uint16_t *buffer, size_t size) {
    m_stream->read(reinterpret_cast<std::istream::char_type*>(buffer), size);

    if (m_stream->bad()) {
        ACSDK_WARN(LX("readFailed").d("bad", m_stream->bad()).d("eof", m_stream->eof()));
        return 0;
    } else {
        ACSDK_DEBUG9(LX("read").d("size", size).d("pos", m_stream->tellg()).d("eof", m_stream->eof()));
        return m_stream->gcount();
    }
}

}  // namespace mediaPlayer
}  // namespace alexaClientSDK
