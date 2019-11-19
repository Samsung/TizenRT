/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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

#include <stddef.h>

enum checksum_type_e {
	CHECKSUM_MD5
};
typedef enum checksum_type_e checksum_type_t;

char *bundle_compute_checksum_for_string(checksum_type_t checksum_type, const char *str, ssize_t len);
char *bundle_base64_encode(const unsigned char *text, size_t len);
unsigned char *bundle_base64_decode(const char *text, size_t *out_len);
