/* Copyright 2015 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef __uv__fs_header__
#define __uv__fs_header__

#ifndef __uv_header__
#error Please include with uv.h
#endif

#include <sys/stat.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------

struct uv_buf_s {
	char *base;
	size_t len;
};

//-----------------------------------------------------------------------------

typedef enum {
	UV_FS_UNKNOWN = -1,
	UV_FS_CUSTOM,
	UV_FS_OPEN,
	UV_FS_CLOSE,
	UV_FS_READ,
	UV_FS_WRITE,
	UV_FS_SENDFILE,
	UV_FS_STAT,
	UV_FS_LSTAT,
	UV_FS_FSTAT,
	UV_FS_FTRUNCATE,
	UV_FS_UTIME,
	UV_FS_FUTIME,
	UV_FS_ACCESS,
	UV_FS_CHMOD,
	UV_FS_FCHMOD,
	UV_FS_FSYNC,
	UV_FS_FDATASYNC,
	UV_FS_UNLINK,
	UV_FS_RMDIR,
	UV_FS_MKDIR,
	UV_FS_MKDTEMP,
	UV_FS_RENAME,
	UV_FS_SCANDIR,
	UV_FS_LINK,
	UV_FS_SYMLINK,
	UV_FS_READLINK,
	UV_FS_CHOWN,
	UV_FS_FCHOWN
} uv_fs_type;

/* uv_fs_t is a subclass of uv_req_t. */
struct uv_fs_s {
	UV_REQ_FIELDS uv_fs_type fs_type;
	uv_loop_t *loop;
	uv_fs_cb cb;
	ssize_t result;
	void *ptr;
	const char *path;
	uv_stat_t statbuf;		/* Stores the result of uv_fs_stat() and uv_fs_fstat(). */
	UV_FS_PRIVATE_FIELDS
};

void uv_fs_req_cleanup(uv_fs_t *req);
int uv_fs_close(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb);
int uv_fs_open(uv_loop_t *loop, uv_fs_t *req, const char *path, int flags, int mode, uv_fs_cb cb);
int uv_fs_read(uv_loop_t *loop, uv_fs_t *req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb);
int uv_fs_write(uv_loop_t *loop, uv_fs_t *req, uv_file file, const uv_buf_t bufs[], unsigned int nbufs, int64_t offset, uv_fs_cb cb);

int uv_fs_stat(uv_loop_t *loop, uv_fs_t *req, const char *path, uv_fs_cb cb);
int uv_fs_fstat(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb);

int uv_fs_fsync(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb);
int uv_fs_fdatasync(uv_loop_t *loop, uv_fs_t *req, uv_file file, uv_fs_cb cb);

int uv_fs_unlink(uv_loop_t *loop, uv_fs_t *req, const char *path, uv_fs_cb cb);
int uv_fs_ftruncate(uv_loop_t *loop, uv_fs_t *req, uv_file file, int64_t offset, uv_fs_cb cb);
int uv_fs_rename(uv_loop_t *loop, uv_fs_t *req, const char *path, const char *new_path, uv_fs_cb cb);

int uv_fs_futime(uv_loop_t *loop, uv_fs_t *req, uv_file file, double atime, double mtime, uv_fs_cb cb);
int uv_fs_utime(uv_loop_t *loop, uv_fs_t *req, const char *path, double atime, double mtime, uv_fs_cb cb);

#ifdef __cplusplus
}
#endif
#endif							// __uv__fs_header__
