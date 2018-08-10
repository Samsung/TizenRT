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
#ifndef FMWUP_UTIL_HTTP_H_
#define FMWUP_UTIL_HTTP_H_

#include <tinyara/config.h>
#include <protocols/webserver/http_keyvalue_list.h>

#define FOTA_CHECK_URL "https://apis.samsungiotcloud.com/v1/stdm/fota/target"
#define KEY_URL "url"
#define KEY_BIN_URL "binaryUrl"
#define KEY_BIN_SIZE "binarySize"
#define KEY_BIN_VERSION "version"
#define REQUEST_URL_LEN 256
#define TOKEN_URL_LEN 256
#define DOWNLOAD_URL_LEN 512
#define NEW_VERSION_LEN 16

//char * g_token_URL = NULL;
//char * g_download_url = NULL;
static bool g_new_firmware = false;
static bool g_new_download = false;

int fmwup_http_download_file(const char *download_url);

int fmwup_update(void);

int fmwup_http_check_firmware(void);

#endif
