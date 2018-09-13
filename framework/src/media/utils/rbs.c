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

/**
 * @file    rbs.c
 * @brief   Ring-buffer stream operation implementation based on ring-buffer.
 * @author  Zhou Xinhe
 * @date    2018-3-12
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <debug.h>
#include "rbs.h"
#include "internal_defs.h"

#define RBS_OPTION_SET(rbsp, option)    (((rbstream_p)(rbsp))->options |= (option))
#define RBS_OPTION_CLR(rbsp, option)    (((rbstream_p)(rbsp))->options &= ~(option))
#define RBS_OPTION_TEST(rbsp, option)   ((((rbstream_p)(rbsp))->options & (option)) != 0)

/**
 * @brief  Pull/Request more data from user, because there's not enough data in ring-buffer.
 *
 * @param  size : maximum size in bytes can be accepted, that also means free spaces in ring-buffer.
 * @param  least : minimum size in bytes must be inputed, user input-callback will be called continuously
 *         until the least size more data written to ring-buffer or there's no more data(callback return 0).
 * @param  stream : Pointer to the ring-buffer stream
 * @return the number of bytes transferred.
 */
static size_t _pull(size_t size, size_t least, rbstream_p rbsp);

rbstream_p rbs_open(rb_p rbp, rbstream_input_f input_func, void *data)
{
	medvdbg("[%s] rbp %p, input_func %p, data %p\n", __FUNCTION__, rbp, input_func, data);
	RETURN_VAL_IF_FAIL(rbp != NULL, NULL);

	rbstream_p rbsp = (rbstream_p) calloc(1, sizeof(rbstream_t));
	RETURN_VAL_IF_FAIL(rbsp != NULL, NULL);

	rbsp->rbp = rbp;
	rbsp->options = 0;
	rbsp->cur_pos = 0;
	rbsp->rd_size = 0;
	rbsp->wr_size = rb_used(rbp);   // Allow to preset data in ring-buffer.

	rbsp->data = data;
	rbsp->input_func = input_func;

	medvdbg("[%s] done, rbsp %p\n", __FUNCTION__, rbsp);
	return rbsp;
}

int rbs_close(rbstream_p rbsp)
{
	medvdbg("[%s] rbsp %p\n", __FUNCTION__, rbsp);

	RETURN_VAL_IF_FAIL(rbsp != NULL, ERROR);
	free(rbsp);
	return OK;
}

size_t rbs_read(void *ptr, size_t size, size_t nmemb, rbstream_p rbsp)
{
	medvdbg("[%s] ptr %p nmemb %lu\n", __FUNCTION__, ptr, nmemb);
	RETURN_VAL_IF_FAIL(ptr != NULL, SIZE_ZERO);
	RETURN_VAL_IF_FAIL(rbsp != NULL, SIZE_ZERO);

	// only size:1 supported
	assert(size == 1);

	size_t len = size * nmemb;
	size_t read = SIZE_ZERO;

	while (read < len) {
		void *_ptr = (void *) ((uint8_t *) ptr + read);
		size_t need = len - read;
		size_t offset = rbsp->cur_pos - rbsp->rd_size;
		// read data desired
		size_t rlen = rb_read_ext(rbsp->rbp, _ptr, need, offset);
		read += rlen;
		rbsp->cur_pos += rlen; // increase cur_pos

		if (read < len) {
			// need to read more data
			size_t least = len - read;
			size_t avail = rb_avail(rbsp->rbp);
			if (least > avail) {
				// need to dequeue data
				if (RBS_OPTION_TEST(rbsp, OPTION_ALLOW_TO_DEQUEUE)) {
					offset = rbsp->cur_pos - rbsp->rd_size;
					size_t _len = MINIMUM(offset, (least - avail));
					size_t _rlen = rb_read(rbsp->rbp, NULL, _len);
					assert(_rlen == _len);
					rbsp->rd_size += _rlen;
				}
			}

			// pull stream data, then it's available to read more.
			if (_pull(rb_avail(rbsp->rbp), least, rbsp) == SIZE_ZERO) {
				// pull data failed
				break;
			}
		}
	}

	medvdbg("[%s] done, read %lu\n", __FUNCTION__, read);
	return read;
}

size_t rbs_write(const void *ptr, size_t size, size_t nmemb, rbstream_p rbsp)
{
	medvdbg("[%s] ptr %p nmemb %lu\n", __FUNCTION__, ptr, nmemb);
	RETURN_VAL_IF_FAIL(ptr != NULL, SIZE_ZERO);
	RETURN_VAL_IF_FAIL(rbsp != NULL, SIZE_ZERO);

	// only size:1 supported
	assert(size == 1);

	size_t len = size * nmemb;
	size_t wlen;

	wlen = rb_write(rbsp->rbp, ptr, len);
	// increase wr_size
	rbsp->wr_size += wlen;

	medvdbg("[%s] done, wlen %lu\n", __FUNCTION__, wlen);
	return wlen;
}

int rbs_seek(rbstream_p rbsp, ssize_t offset, int whence)
{
	medvdbg("[%s] offset %ld, whence %d\n", __FUNCTION__, offset, whence);
	RETURN_VAL_IF_FAIL(rbsp != NULL, ERROR);

	switch (whence) {
	case SEEK_SET:
		// checking underflow
		RETURN_VAL_IF_FAIL(((size_t) offset >= rbsp->rd_size), ERROR);

		while ((size_t) offset > rbsp->wr_size) {
			size_t least = (size_t) offset - rbsp->wr_size;
			size_t wlen;

			// pull stream data, then wr_size will be increased
			wlen = _pull(rb_avail(rbsp->rbp), least, rbsp);

			if ((size_t) offset > rbsp->wr_size) {
				// not enough
				if (rb_avail(rbsp->rbp) == SIZE_ZERO) {
					// ring-buffer is full
					RETURN_VAL_IF_FAIL((RBS_OPTION_TEST(rbsp, OPTION_ALLOW_TO_DEQUEUE)), ERROR); // overflow

					// dequeue minimal data from ring-buffer
					size_t len = rbsp->wr_size - rbsp->rd_size;
					least = (size_t) offset - rbsp->wr_size;
					len = MINIMUM(len, least);
					size_t rlen = rb_read(rbsp->rbp, NULL, len);
					assert(rlen == len);
					rbsp->rd_size += rlen;
				} else {
					RETURN_VAL_IF_FAIL((wlen != SIZE_ZERO), ERROR);    // EOS
				}

				// request more data
				continue;
			}
			// got enough data
			break;
		}

		rbsp->cur_pos = (size_t) offset;
		break;
	}

	return OK;
}

// seek and dequeue data from ring-buffer
int rbs_seek_ext(rbstream_p rbsp, ssize_t offset, int whence)
{
	medvdbg("[%s] offset %ld, whence %d\n", __FUNCTION__, offset, whence);
	assert(RBS_OPTION_TEST(rbsp, OPTION_ALLOW_TO_DEQUEUE));

	// seek
	int ret = rbs_seek(rbsp, offset, whence);
	RETURN_VAL_IF_FAIL(ret == OK, ret);

	// dequeue data from ring-buffer
	size_t len = rbsp->cur_pos - rbsp->rd_size;
	size_t rlen = rb_read(rbsp->rbp, NULL, len);

	assert(rlen == len);    // rbs_seek returned success!
	rbsp->rd_size += rlen;

	return ret;
}

int rbs_ctrl(rbstream_p rbsp, int option, int value)
{
	medvdbg("[%s] option %d value %d\n", __FUNCTION__, option, value);
	RETURN_VAL_IF_FAIL(rbsp != NULL, ERROR);

	int ret = OK;

	switch (option) {
	case OPTION_ALLOW_TO_DEQUEUE:
		ret = RBS_OPTION_TEST(rbsp, OPTION_ALLOW_TO_DEQUEUE);
		if (value) {
			RBS_OPTION_SET(rbsp, option);
		} else {
			RBS_OPTION_CLR(rbsp, option);
		}
		break;
	}

	return ret;
}

static size_t _pull(size_t size, size_t least, rbstream_p rbsp)
{
	RETURN_VAL_IF_FAIL(rbsp != NULL, SIZE_ZERO);
	RETURN_VAL_IF_FAIL(rbsp->input_func != NULL, SIZE_ZERO);

	medvdbg("[%s] size %lu, least %lu\n", __FUNCTION__, size, least);

	if (least > size) {
		// Large size ring-buffer is needed or you should dequeue read-data from ring-buffer in time.
		medvdbg("[%s] WARNING!!! least[%lu] > size[%lu]\n", __FUNCTION__, least, size);
		least = size;
		RETURN_VAL_IF_FAIL(least > SIZE_ZERO, SIZE_ZERO);   // ring-buffer is full.
	}

	size_t wlen = SIZE_ZERO;
	size_t len;
	do {
		len = (rbsp->input_func)(rbsp->data, rbsp);
		if (len == SIZE_ZERO) {
			medvdbg("[%s] WARNING!!! no more data, wlen[%lu], least[%lu], size[%lu]\n", __FUNCTION__, wlen, least, size);
			break;
		}

		wlen += len;
	} while (wlen < least);

	medvdbg("[%s] done, wlen %lu\n", __FUNCTION__, wlen);
	return wlen;
}

