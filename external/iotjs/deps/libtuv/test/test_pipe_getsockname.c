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

#include "uv.h"
#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__linux__)
  #include <sys/socket.h>
  #include <sys/un.h>
#endif

#ifndef _WIN32
# include <unistd.h>  /* close */
#else
# include <fcntl.h>
#endif

static uv_pipe_t pipe_client;
static uv_pipe_t pipe_server;
static uv_connect_t connect_req;

static int pipe_close_cb_called = 0;
static int pipe_client_connect_cb_called = 0;


static void pipe_close_cb(uv_handle_t* handle) {
  TUV_ASSERT(handle == (uv_handle_t*) &pipe_client ||
         handle == (uv_handle_t*) &pipe_server);
  pipe_close_cb_called++;
}


static void pipe_client_connect_cb(uv_connect_t* req, int status) {
  char buf[1024];
  size_t len;
  int r;

  TUV_ASSERT(req == &connect_req);
  TUV_ASSERT(status == 0);

  len = sizeof buf;
  r = uv_pipe_getpeername(&pipe_client, buf, &len);
  TUV_ASSERT(r == 0);

  TUV_ASSERT(buf[len - 1] != 0);
  TUV_ASSERT(memcmp(buf, TEST_PIPENAME, len) == 0);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_client, buf, &len);
  TUV_ASSERT(r == 0 && len == 0);

  pipe_client_connect_cb_called++;


  uv_close((uv_handle_t*) &pipe_client, pipe_close_cb);
  uv_close((uv_handle_t*) &pipe_server, pipe_close_cb);
}


static void pipe_server_connection_cb(uv_stream_t* handle, int status) {
  /* This function *may* be called, depending on whether accept or the
   * connection callback is called first.
   */
  TUV_ASSERT(status == 0);
}


TEST_IMPL(pipe_getsockname) {
  uv_loop_t* loop;
  char buf[1024];
  size_t len;
  int r;
  pipe_close_cb_called = 0;

  loop = uv_default_loop();
  TUV_ASSERT(loop != NULL);

  r = uv_pipe_init(loop, &pipe_server, 0);
  TUV_ASSERT(r == 0);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_server, buf, &len);
  TUV_ASSERT(r == UV_EBADF);

  len = sizeof buf;
  r = uv_pipe_getpeername(&pipe_server, buf, &len);
  TUV_ASSERT(r == UV_EBADF);

  r = uv_pipe_bind(&pipe_server, TEST_PIPENAME);
  TUV_ASSERT(r == 0);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_server, buf, &len);
  TUV_ASSERT(r == 0);

  TUV_ASSERT(buf[len - 1] != 0);
  TUV_ASSERT(buf[len] == '\0');
  TUV_ASSERT(memcmp(buf, TEST_PIPENAME, len) == 0);

  len = sizeof buf;
  r = uv_pipe_getpeername(&pipe_server, buf, &len);
  TUV_ASSERT(r == UV_ENOTCONN);

  r = uv_listen((uv_stream_t*) &pipe_server, 0, pipe_server_connection_cb);
  TUV_ASSERT(r == 0);

  r = uv_pipe_init(loop, &pipe_client, 0);
  TUV_ASSERT(r == 0);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_client, buf, &len);
  TUV_ASSERT(r == UV_EBADF);

  len = sizeof buf;
  r = uv_pipe_getpeername(&pipe_client, buf, &len);
  TUV_ASSERT(r == UV_EBADF);

  uv_pipe_connect(&connect_req, &pipe_client, TEST_PIPENAME, pipe_client_connect_cb);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_client, buf, &len);
  TUV_ASSERT(r == 0 && len == 0);

  len = sizeof buf;
  r = uv_pipe_getpeername(&pipe_client, buf, &len);
  TUV_ASSERT(r == 0);

  TUV_ASSERT(buf[len - 1] != 0);
  TUV_ASSERT(memcmp(buf, TEST_PIPENAME, len) == 0);

  r = uv_run(loop, UV_RUN_DEFAULT);
  TUV_ASSERT(r == 0);
  TUV_ASSERT(pipe_client_connect_cb_called == 1);
  TUV_ASSERT(pipe_close_cb_called == 2);

  return 0;
}


TEST_IMPL(pipe_getsockname_abstract) {
#if defined(__linux__)
  char buf[1024];
  size_t len;
  int r;
  int sock;
  struct sockaddr_un sun;
  socklen_t sun_len;
  char abstract_pipe[] = "\0test-pipe";
  pipe_close_cb_called = 0;

  sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  TUV_ASSERT(sock != -1);

  sun_len = sizeof sun;
  memset(&sun, 0, sun_len);
  sun.sun_family = AF_UNIX;
  memcpy(sun.sun_path, abstract_pipe, sizeof abstract_pipe);

  r = bind(sock, (struct sockaddr*)&sun, sun_len);
  TUV_ASSERT(r == 0);

  r = uv_pipe_init(uv_default_loop(), &pipe_server, 0);
  TUV_ASSERT(r == 0);
  r = uv_pipe_open(&pipe_server, sock);
  TUV_ASSERT(r == 0);

  len = sizeof buf;
  r = uv_pipe_getsockname(&pipe_server, buf, &len);
  TUV_ASSERT(r == 0);

  TUV_ASSERT(memcmp(buf, abstract_pipe, sizeof abstract_pipe) == 0);

  uv_close((uv_handle_t*)&pipe_server, pipe_close_cb);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  close(sock);

  TUV_ASSERT(pipe_close_cb_called == 1);
  return 0;
#else
  return 0;
#endif
}

TEST_IMPL(pipe_getsockname_blocking) {
#ifdef _WIN32
  HANDLE readh, writeh;
  int readfd;
  char buf1[1024], buf2[1024];
  size_t len1, len2;
  int r;

  r = CreatePipe(&readh, &writeh, NULL, 65536);
  TUV_ASSERT(r != 0);

  r = uv_pipe_init(uv_default_loop(), &pipe_client, 0);
  TUV_ASSERT(r == 0);
  readfd = _open_osfhandle((intptr_t)readh, _O_RDONLY);
  TUV_ASSERT(r != -1);
  r = uv_pipe_open(&pipe_client, readfd);
  TUV_ASSERT(r == 0);
  r = uv_read_start((uv_stream_t*)&pipe_client, NULL, NULL);
  TUV_ASSERT(r == 0);
  Sleep(100);
  r = uv_read_stop((uv_stream_t*)&pipe_client);
  TUV_ASSERT(r == 0);

  len1 = sizeof buf1;
  r = uv_pipe_getsockname(&pipe_client, buf1, &len1);
  TUV_ASSERT(r == 0);
  TUV_ASSERT(len1 == 0);  /* It's an annonymous pipe. */

  r = uv_read_start((uv_stream_t*)&pipe_client, NULL, NULL);
  TUV_ASSERT(r == 0);
  Sleep(100);

  len2 = sizeof buf2;
  r = uv_pipe_getsockname(&pipe_client, buf2, &len2);
  TUV_ASSERT(r == 0);
  TUV_ASSERT(len2 == 0);  /* It's an annonymous pipe. */

  r = uv_read_stop((uv_stream_t*)&pipe_client);
  TUV_ASSERT(r == 0);

  TUV_ASSERT(len1 == len2);
  TUV_ASSERT(memcmp(buf1, buf2, len1) == 0);

  pipe_close_cb_called = 0;
  uv_close((uv_handle_t*)&pipe_client, pipe_close_cb);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  TUV_ASSERT(pipe_close_cb_called == 1);

  CloseHandle(writeh);
#endif

  return 0;
}
