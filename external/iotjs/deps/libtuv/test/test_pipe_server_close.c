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

#include <string.h>
#include <errno.h>

#include "uv.h"
#include "runner.h"


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
  TUV_ASSERT(req == &connect_req);
  TUV_ASSERT(status == 0);

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


TEST_IMPL(pipe_server_close) {
  uv_loop_t* loop;
  int r;

  loop = uv_default_loop();
  TUV_ASSERT(loop != NULL);

  r = uv_pipe_init(loop, &pipe_server, 0);
  TUV_ASSERT(r == 0);

  r = uv_pipe_bind(&pipe_server, TEST_PIPENAME);
  TUV_ASSERT(r == 0);

  r = uv_listen((uv_stream_t*) &pipe_server, 0, pipe_server_connection_cb);
  TUV_ASSERT(r == 0);

  r = uv_pipe_init(loop, &pipe_client, 0);
  TUV_ASSERT(r == 0);

  uv_pipe_connect(&connect_req, &pipe_client, TEST_PIPENAME, pipe_client_connect_cb);

  r = uv_run(loop, UV_RUN_DEFAULT);
  TUV_ASSERT(r == 0);
  TUV_ASSERT(pipe_client_connect_cb_called == 1);
  TUV_ASSERT(pipe_close_cb_called == 2);

  return 0;
}
