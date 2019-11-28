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

#include "bundle_util.h"
#include <string.h>
#include <codecs/base64.h>
#include <codecs/md5.h>

char *bundle_compute_checksum_for_string(checksum_type_t checksum_type, const char *str, ssize_t len)
{
	switch (checksum_type) {
	case CHECKSUM_MD5:
		return md5_hash((const uint8_t *)str, len);
	default:
		return NULL;
	}
}

char *bundle_base64_encode(const unsigned char *text, size_t len)
{
	return (char *)base64_encode(text, len, NULL, NULL);
}

unsigned char *bundle_base64_decode(const char *text, size_t *out_len)
{
	return (unsigned char *)base64_decode(text, strlen(text), NULL, out_len);
}
