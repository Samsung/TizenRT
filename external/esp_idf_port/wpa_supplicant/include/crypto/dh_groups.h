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
 * Diffie-Hellman groups
 * Copyright (c) 2007, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef DH_GROUPS_H
#define DH_GROUPS_H

struct dh_group {
	int id;
	const u8 *generator;
	size_t generator_len;
	const u8 *prime;
	size_t prime_len;
};

const struct dh_group * dh_groups_get(int id);
struct wpabuf * dh_init(const struct dh_group *dh, struct wpabuf **priv);
struct wpabuf * dh_derive_shared(const struct wpabuf *peer_public,
				 const struct wpabuf *own_private,
				 const struct dh_group *dh);

#endif /* DH_GROUPS_H */
