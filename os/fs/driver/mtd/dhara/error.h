/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
/* Dhara - NAND flash management layer
 * Copyright (C) 2013 Daniel Beer <dlbeer@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef DHARA_ERROR_H_
#define DHARA_ERROR_H_

typedef enum {
	DHARA_E_NONE = 0,
	DHARA_E_BAD_BLOCK,
	DHARA_E_ECC,
	DHARA_E_TOO_BAD,
	DHARA_E_RECOVER,
	DHARA_E_JOURNAL_FULL,
	DHARA_E_NOT_FOUND,
	DHARA_E_MAP_FULL,
	DHARA_E_CORRUPT_MAP,
	DHARA_E_MAX
} dhara_error_t;

/* Produce a human-readable error message. This function is kept in a
 * separate compilation unit and can be omitted to reduce binary size.
 */
const char *dhara_strerror(dhara_error_t err);

/* Save an error */
static inline void dhara_set_error(dhara_error_t *err, dhara_error_t v)
{
	if (err) {
		*err = v;
	}
}

#endif
