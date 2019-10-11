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
#include <stdio.h>
#include <sys/types.h>
#include <debug.h>
#include "rbs.h"
#include "internal_defs.h"

#define RBS_OPTION_SET(rbsp, option)    (((rbstream_p)(rbsp))->options |= (option))
#define RBS_OPTION_CLR(rbsp, option)    (((rbstream_p)(rbsp))->options &= ~(option))
#define RBS_OPTION_CHK(rbsp, option)    ((((rbstream_p)(rbsp))->options & (option)) != 0)


rbstream_p rbs_open(rb_p rbp)
{
	medvdbg("[%s] rbp %p\n", __FUNCTION__, rbp);
	RETURN_VAL_IF_FAIL(rbp != NULL, NULL);

	rbstream_p rbsp = (rbstream_p) calloc(1, sizeof(rbstream_t));
	RETURN_VAL_IF_FAIL(rbsp != NULL, NULL);

	rbsp->rbp = rbp;
	rbsp->options = 0;
	rbsp->cur_pos = 0;
	rbsp->rd_size = 0;
	rbsp->wr_size = rb_used(rbp);   // Allow to preset data in ring-buffer.

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

	size_t len = size * nmemb;
	size_t offset = rbsp->cur_pos - rbsp->rd_size;
	// try to read data desired
	size_t read = rb_read_ext(rbsp->rbp, ptr, len, offset);
	// increase cur_pos
	rbsp->cur_pos += read;

	if (read < len) {
		// more data wanted
		size_t least = len - read;
		size_t avail = rb_avail(rbsp->rbp);
		if (least > avail) {
			// need to dequeue data
			if (RBS_OPTION_CHK(rbsp, OPTION_ALLOW_TO_DEQUEUE)) {
				// dequeue minimal data from ring-buffer
				offset = rbsp->cur_pos - rbsp->rd_size;
				size_t _len = MINIMUM(offset, (least - avail));
				rbsp->rd_size += rb_read(rbsp->rbp, NULL, _len);
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

		if ((size_t) offset > rbsp->wr_size) {
			// more data wanted
			size_t least = (size_t) offset - rbsp->wr_size;
			size_t avail = rb_avail(rbsp->rbp);
			if (least > avail) {
				// need to dequeue data
				RETURN_VAL_IF_FAIL((RBS_OPTION_CHK(rbsp, OPTION_ALLOW_TO_DEQUEUE)), ERROR);
				// dequeue all data from ring-buffer
				size_t len = rbsp->wr_size - rbsp->rd_size;
				rbsp->rd_size += rb_read(rbsp->rbp, NULL, len);
			}

			rbsp->cur_pos = (size_t)rbsp->wr_size;
			return least; // number of data wanted
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

	// seek
	int ret = rbs_seek(rbsp, offset, whence);
	RETURN_VAL_IF_FAIL(ret >= OK, ret);

	// dequeue data from ring-buffer
	size_t len = rbsp->cur_pos - rbsp->rd_size;
	size_t rlen = rb_read(rbsp->rbp, NULL, len);

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
		ret = RBS_OPTION_CHK(rbsp, OPTION_ALLOW_TO_DEQUEUE);
		if (value) {
			RBS_OPTION_SET(rbsp, option);
		} else {
			RBS_OPTION_CLR(rbsp, option);
		}
		break;
	}

	return ret;
}
