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
 * External password backend
 * Copyright (c) 2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef EXT_PASSWORD_H
#define EXT_PASSWORD_H

struct ext_password_data;

#ifdef CONFIG_EXT_PASSWORD

struct ext_password_data * ext_password_init(const char *backend,
					     const char *params);
void ext_password_deinit(struct ext_password_data *data);

struct wpabuf * ext_password_get(struct ext_password_data *data,
				 const char *name);
void ext_password_free(struct wpabuf *pw);

#else /* CONFIG_EXT_PASSWORD */

#define ext_password_init(b, p)
#define ext_password_deinit(d)
#define ext_password_get(d, n)
#define ext_password_free(p)

#endif /* CONFIG_EXT_PASSWORD */

#endif /* EXT_PASSWORD_H */
