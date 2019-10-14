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

#include <tinyara/config.h>
#include <debug.h>
#include "Demuxer.h"
#ifdef CONFIG_CONTAINER_MPEG2TS
#include "demux/mpeg2ts/TSDemuxer.h"
#endif

namespace media {

std::shared_ptr<Demuxer> Demuxer::create(container_type_t containerType)
{
	std::shared_ptr<Demuxer> demuxer;

	switch (containerType) {
#ifdef CONFIG_CONTAINER_MPEG2TS
	case AUDIO_TYPE_MP2T:
		demuxer = std::make_shared<TSDemuxer>();
		if (demuxer && demuxer->initialize()) {
			return demuxer;
		}
		break;
#endif
	default:
		break;
	}

	meddbg("create demuxer instance of type %d failed!\n", containerType);
	return nullptr;
}

Demuxer::Demuxer(container_type_t containerType)
	: mContainerType(containerType)
	, mParam(nullptr)
{
}

Demuxer::~Demuxer()
{
}

} // namespace media
