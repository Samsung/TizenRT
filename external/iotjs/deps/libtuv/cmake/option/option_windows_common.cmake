# Copyright 2015-2017 Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


#set(CMAKE_C_FLAGS_DEBUG     "-O0 -g -DDEBUG")
#set(CMAKE_C_FLAGS_RELEASE   "-O2 -DNDEBUG")

# unix common source files
set(WIN_PATH "${SOURCE_ROOT}/win")

# test include
set(TUV_TEST_INCDIRS "${TEST_ROOT}")

#
# { TUV_CHANGES@20161129:
#   It corresponds to uv.gyp's section "Windows" }
#
set(PLATFORM_SRCFILES
      ${INCLUDE_ROOT}/uv-win.h
      ${WIN_PATH}/async.c
      ${WIN_PATH}/atomicops-inl.h
      ${WIN_PATH}/core.c
      ${WIN_PATH}/detect-wakeup.c
      ${WIN_PATH}/dl.c
      ${WIN_PATH}/error.c
      ${WIN_PATH}/fs.c
      ${WIN_PATH}/fs-event.c
      ${WIN_PATH}/getaddrinfo.c
      ${WIN_PATH}/getnameinfo.c
      ${WIN_PATH}/handle.c
      ${WIN_PATH}/handle-inl.h
      ${WIN_PATH}/internal.h
      ${WIN_PATH}/loop-watcher.c
      ${WIN_PATH}/pipe.c
      ${WIN_PATH}/thread.c
      ${WIN_PATH}/poll.c
      ${WIN_PATH}/process.c
      ${WIN_PATH}/process-stdio.c
      ${WIN_PATH}/req.c
      ${WIN_PATH}/req-inl.h
      ${WIN_PATH}/signal.c
      ${WIN_PATH}/snprintf.c
      ${WIN_PATH}/stream.c
      ${WIN_PATH}/stream-inl.h
      ${WIN_PATH}/tcp.c
      ${WIN_PATH}/tty.c
      ${WIN_PATH}/timer.c
      ${WIN_PATH}/udp.c
      ${WIN_PATH}/util.c
      ${WIN_PATH}/winapi.c
      ${WIN_PATH}/winapi.h
      ${WIN_PATH}/winsock.c
      ${WIN_PATH}/winsock.h
    )

set(TEST_MAINFILE "${TEST_ROOT}/runner_main.c")

set(TEST_UNITFILES
      ${TEST_ROOT}/test_idle.c
      ${TEST_ROOT}/test_timer.c
      ${TEST_ROOT}/test_timer_again.c
      ${TEST_ROOT}/test_fs.c
      ${TEST_ROOT}/test_cwd.c
      ${TEST_ROOT}/test_error.c
      ${TEST_ROOT}/test_tcp_open.c
      ${TEST_ROOT}/test_shutdown_eof.c
      ${TEST_ROOT}/echo_server.c
      ${TEST_ROOT}/test_getaddrinfo.c
      ${TEST_ROOT}/test_threadpool.c
      ${TEST_ROOT}/test_condvar.c
      ${TEST_ROOT}/test_active.c
      ${TEST_ROOT}/test_walk_handles.c
      ${TEST_ROOT}/test_async.c
      )

# configuration values
set(CONFIG_FILESYSTEM 1)
