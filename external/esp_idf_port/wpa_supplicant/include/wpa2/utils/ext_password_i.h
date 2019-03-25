/******************************************************************
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

/*
 * External password backend - internal definitions
 * Copyright (c) 2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EXT_PASSWORD_I_H
#define EXT_PASSWORD_I_H

#include "ext_password.h"

struct ext_password_backend {
	const char *name;
	void * (*init)(const char *params);
	void (*deinit)(void *ctx);
	struct wpabuf * (*get)(void *ctx, const char *name);
};

struct wpabuf * ext_password_alloc(size_t len);

#endif /* EXT_PASSWORD_I_H */
