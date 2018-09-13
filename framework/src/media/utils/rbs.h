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

#ifndef _RBS_H_
#define _RBS_H_

#include "rb.h"
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This rbs.h provides APIs to operate ring buffer stream like a file stream.
   rbs_open  -> user provides ring-buffer object and register input-function
   rbs_close -> fclose
   rbs_read  -> fread, only item size 1 is supported
   rbs_write -> fwrite, only item size 1 is supported
   rbs_seek  -> fseek
   rbs_tell  -> ftell
   rbs_seek_ext -> if user seek_ext at pos1, then it's IMPOSSIBLE to seek(_ext) at
                   a forward pos2 (pos2 < pos1).
   rbs_tell_ext -> tell available minimum position to seek, not current read position).

   There's some limitations:
   rbs_seek(_ext) can only seek at position started from rd_size, and if seek at position
   after wr_size, more data will be requseted (user must push data during callback).
   Large ring-buffer size is better, problem maybe happen if the size is too small.
   It's thread safe for multi-threaded writing and one thread reading at same time,
   user can try to push data at any time as long as there is free space in ring-buffer.

                 [   RING             ~            BUFFER  ]
                 0         rd_idx          wr_idx
                 |           |                |
                  ------------------------------------------
 	             |...........#################.............|
                  ------------------------------------------

     ~~~~~~~~~~~~~~~~~~~~~~~~#################,,,,,,,,,,,,,,,,,,,,,,,
     |                       |       |        |                     |
POS: 0                     rd_size cur_pos wr_size              end of stream

'#' -- Valid Data in ring-buffer
'.' -- Free space in ring-buffer
'~' -- Data read/popped out from ring-buffer
',' -- will be pushed into ring-buffer when needed

*/

enum {
	OPTION_ALLOW_TO_DEQUEUE = 0x0001,    /* Allow to dequeue data form ring-buffer */
	OPTION_MAX,
};

typedef struct rbstream_s  rbstream_t;
typedef struct rbstream_s *rbstream_p;

typedef size_t(*rbstream_input_f)(void *data, rbstream_p stream);

struct rbstream_s {
	rb_p rbp;                       /* pointer to the ring-buffer object */
	volatile size_t wr_size;        /* total size written to ring-buffer */
	volatile size_t rd_size;        /* total size read from ring-buffer  */
	volatile size_t cur_pos;        /* current read position, range[rd_size,wr_size] */
	void *data;                     /* callback data for user */
	rbstream_input_f input_func;    /* callback function to request more data */
	int options;                    /*  */
};

/**
 * @brief  Opens the given ring-buffer and associates a stream with it.
 *
 * @param  rbp : Pointer to the ring-buffer object
 * @param  input_func: callback funtion for user to push data stream to ring-buffer
 * @param  data: ponter to user callback data
 * @return ring-buffer stream handle, NULL is returned when failed.
 */
rbstream_p rbs_open(rb_p rbp, rbstream_input_f input_func, void *data);

/**
 * @brief  Close ring-buffer stream returned by rbs_open().
 *
 * @param  stream : Pointer to the ring-buffer stream
 * @return 0 is returned on success. Otherwise, -1 is returned.
 */
int rbs_close(rbstream_p stream);

/**
 * @brief  Reads nmemb items of data, from the position seeked by rbs_seek(_ext)
 *
 * @param  ptr : Pointer to the location of items
 * @param  size : Size in bytes of each item. Currently we only support 1 now.
 * @param  nmemb : Number of items
 * @param  stream : Pointer to the ring-buffer stream
 * @return the number of bytes transferred.
 */
size_t rbs_read(void *ptr, size_t size, size_t nmemb, rbstream_p stream);

/**
 * @brief  Writes nmemb items of data. Always append data to ring-buffer end,
 *         that means data can't be written to the position seeked by rbs_seek()
 *
 * @param  ptr : Pointer to the location of items
 * @param  size : Size in bytes of each item. Currently we only support 1 now.
 * @param  nmemb : Number of items
 * @param  stream : Pointer to the ring-buffer stream
 * @return the number of bytes transferred.
 */
size_t rbs_write(const void *ptr, size_t size, size_t nmemb, rbstream_p stream);

/**
 * @brief  Sets the current read position indicator (cur_pos) for the stream.
 *
 * @param  stream : Pointer to the ring-buffer stream
 * @param  offset : Offset bytes to the position specified by whence.
 *         Anyway, the final absolute position should be greater than rd_size,
 *         And more data stream will be requested if there's not enough data
 *         in ring-buffer. If can not request enough data, it returns failure.
 * @param  whence : SEEK_SET, SEEK_CUR, or SEEK_END
 * @return 0 is returned on success. Otherwise, -1 is returned.
 */
int rbs_seek(rbstream_p stream, ssize_t offset, int whence);

/**
 * @brief  Sets the current read position indicator (cur_pos) for the stream.
 *         And update rd_size same with cur_pos
 *
 * @param  stream : Pointer to the ring-buffer stream
 * @param  offset : It's same as offset param in above rbs_seek(). The only
 *         difference is that, rd_size indicator will be changed to same cur_pos.
 *         It's means data has been popped out and can never been read again.
 * @param  whence : SEEK_SET, SEEK_CUR, or SEEK_END
 * @return 0 is returned on success. Otherwise, -1 is returned.
 */
int rbs_seek_ext(rbstream_p stream, ssize_t offset, int whence);

/**
 * @brief  Set options
 *
 * @param  stream : Pointer to the ring-buffer stream
 * @param  option : Refer option enum
 * @param  value : Detail option value
 * @return current option value.
 */
int rbs_ctrl(rbstream_p stream, int option, int value);

#ifdef __cplusplus
}
#endif
#endif

