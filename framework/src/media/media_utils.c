/****************************************************************************
 *
 * Copyright 2017 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include <media/media_utils.h>

media_format_t media_parse_header(int fd)
{
	return MEDIA_FORMAT_PCM;
}

media_format_t media_get_file_format(const char *path)
{
	return MEDIA_FORMAT_PCM;
}

int media_generate_header(media_format_t format, int sample_rate, bool is_stereo, int pcm_format)
{
	return MEDIA_OK;
}
