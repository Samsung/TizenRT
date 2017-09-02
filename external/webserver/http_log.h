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

#ifndef __http_log_h__
#define __http_log_h__

/*
 * For TinyAra
 */
#ifdef CONFIG_NETUTILS_WEBSERVER_LOGD
#define HTTP_LOGD(...) printf(__VA_ARGS__)
#else
#define HTTP_LOGD(...)
#endif

#ifdef CONFIG_NETUTILS_WEBSERVER_LOGE
#define HTTP_LOGE(...) printf(__VA_ARGS__)
#else
#define HTTP_LOGE(...)
#endif

#endif
