/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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

#ifndef __http_string_util_h__
#define __http_string_util_h__

#include <stdio.h>

int http_find_first_crlf(const char *src, int len, int start);
int http_separate_header(const char *src, int *method, char *url, int *httpver);
int http_separate_status_line(const char *src, int *httpver, int *status, char *phrase);
int http_separate_keyvalue(const char *src, char *key, char *value);

#endif
