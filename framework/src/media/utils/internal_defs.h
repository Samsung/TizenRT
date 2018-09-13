/******************************************************************
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

#ifndef DEFS_H
#define DEFS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RETURN_IF_FAIL(condition) \
	do { \
		if (!(condition)) { \
			return; \
		} \
	} while (0)

#define RETURN_VAL_IF_FAIL(condition, val) \
	do { \
		if (!(condition)) { \
			return val; \
		} \
	} while (0)

#define GOTO_IF_FAIL(condition, tag) \
	do { \
		if (!(condition)) { \
			goto tag; \
		} \
	} while (0)

#ifndef MINIMUM
#define MINIMUM(x, y) (((x) < (y)) ? (x) : (y))
#endif

// Size: 0, no data written or read
#define SIZE_ZERO (0)

#ifndef OK
#define OK (0)
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* DEFS_H */

