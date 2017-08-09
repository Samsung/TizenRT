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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/ringbuf.h>

inline void printBuf(char *buf, struct ringbuf *rbp)
{
	int i = 0;
	for (i = 0; i < rbp->bufsize; i++) {
		printf("%c", buf[i]);
	}
	printf("\n\n");
}

inline ssize_t ringbuf_write(FAR const char *buffer, size_t len, struct ringbuf *rbp)
{
	int chunklen = 0;
	if (rbp->index + len > rbp->bufsize) {
		if (!rbp->is_overwritable) {
			return rbp->index;
		}
		chunklen = rbp->bufsize - rbp->index;
		memcpy((void *)(rbp->buffer + rbp->index), (void *)buffer, chunklen);
		len = len - chunklen;
		rbp->index = 0;
		rbp->is_overwritten++;
	}
	memcpy((void *)(rbp->buffer + rbp->index), (void *)(buffer + chunklen), len);
	rbp->index += len;
	return rbp->index;
}

inline ssize_t ringbuf_read(char *buffer, size_t len, struct ringbuf *rbp)
{
	int chunklen = 0;
	if (rbp->is_overwritten == 0) {
		len = (len < rbp->index) ? len : rbp->index;
		memcpy((void *)buffer, (void *)(rbp->buffer), rbp->index);
	} else {
		len = rbp->bufsize;
		chunklen = rbp->bufsize - rbp->index;
		memcpy((void *)(buffer), (void *)(rbp->buffer + rbp->index), chunklen + 1);
		memcpy((void *)(buffer + chunklen), (void *)(rbp->buffer), rbp->index - 1);
	}
	rbp->is_overwritten = 0;
	rbp->index = 0;
	return len;
}
