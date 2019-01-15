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


set(FLAGS_COMMON
      "-fno-builtin"
      )

set(CMAKE_C_FLAGS_DEBUG     "-O0 -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE   "-O2 -DNDEBUG")

# unix common source files
set(UNIX_PATH "${SOURCE_ROOT}/unix")

# test include
set(TUV_TEST_INCDIRS "${TEST_ROOT}")

#
# { TUV_CHANGES@20161129:
#   It corresponds to uv.gyp's section "Not Windows i.e. POSIX" }
#
set(PLATFORM_SRCFILES
      ${INCLUDE_ROOT}/uv-unix.h
      ${INCLUDE_ROOT}/uv-linux.h
#     ${INCLUDE_ROOT}/uv-sunos.h
#     ${INCLUDE_ROOT}/uv-darwin.h
#     ${INCLUDE_ROOT}/uv-bsd.h
#     ${INCLUDE_ROOT}/uv-aix.h
      ${UNIX_PATH}/async.c
      ${UNIX_PATH}/atomic-ops.h
      ${UNIX_PATH}/core.c
#     ${UNIX_PATH}/dl.c
      ${UNIX_PATH}/fs.c
      ${UNIX_PATH}/getaddrinfo.c
#     ${UNIX_PATH}/getnameinfo.c
      ${UNIX_PATH}/internal.h
      ${UNIX_PATH}/loop.c
      ${UNIX_PATH}/loop-watcher.c
      ${UNIX_PATH}/poll.c
      ${UNIX_PATH}/process.c
#     ${UNIX_PATH}/proctitle.c
      ${UNIX_PATH}/spinlock.h
      ${UNIX_PATH}/stream.c
      ${UNIX_PATH}/tcp.c
      ${UNIX_PATH}/thread.c
      ${UNIX_PATH}/timer.c
#     ${UNIX_PATH}/tty.c
      ${UNIX_PATH}/udp.c
    )

set(TEST_MAINFILE "${TEST_ROOT}/runner_main.c")

set(TEST_UNITFILES
      "${TEST_ROOT}/test_idle.c"
      "${TEST_ROOT}/test_timer.c"
      "${TEST_ROOT}/test_timer_again.c"
      "${TEST_ROOT}/test_fs.c"
      "${TEST_ROOT}/test_cwd.c"
      "${TEST_ROOT}/test_error.c"
      "${TEST_ROOT}/test_tcp_open.c"
      "${TEST_ROOT}/test_shutdown_eof.c"
      "${TEST_ROOT}/echo_server.c"
      "${TEST_ROOT}/test_getaddrinfo.c"
      "${TEST_ROOT}/test_threadpool.c"
      "${TEST_ROOT}/test_condvar.c"
      "${TEST_ROOT}/test_active.c"
      "${TEST_ROOT}/test_walk_handles.c"
      "${TEST_ROOT}/test_async.c"
    )

# { TUV_CHANGES@20180803:
#   Made signal build time configurable }
if(TUV_FEATURE_PROCESS)
      set(TUV_FEATURE_PIPE ON)
      set(TUV_FEATURE_SIGNAL ON)
      set(FLAGS_COMMON "${FLAGS_COMMON}" "-DTUV_FEATURE_PROCESS=1")
      set(TEST_UNITFILES "${TEST_UNITFILES}"
            "${TEST_ROOT}/test_ipc.c"
            "${TEST_ROOT}/test_spawn.c")
endif()


# { TUV_CHANGES@20180724:
#   Made pipe build time configurable }
if(TUV_FEATURE_PIPE)
      set(PLATFORM_SRCFILES "${PLATFORM_SRCFILES}"
            ${UNIX_PATH}/pipe.c)
      set(FLAGS_COMMON "${FLAGS_COMMON}" "-DTUV_FEATURE_PIPE=1")
      set(TEST_UNITFILES "${TEST_UNITFILES}"
            "${TEST_ROOT}/test_pipe_bind_error.c"
            "${TEST_ROOT}/test_pipe_close_stdout_read_stdin.c"
            "${TEST_ROOT}/test_pipe_connect_error.c"
            "${TEST_ROOT}/test_pipe_connect_multiple.c"
            "${TEST_ROOT}/test_pipe_connect_prepare.c"
            "${TEST_ROOT}/test_pipe_getsockname.c"
            "${TEST_ROOT}/test_pipe_pending_instances.c"
            "${TEST_ROOT}/test_pipe_sendmsg.c"
            "${TEST_ROOT}/test_pipe_server_close.c"
            "${TEST_ROOT}/test_pipe_set_non_blocking.c"
           )
endif()

# { TUV_CHANGES@20180723:
#   Made signal build time configurable }
if(TUV_FEATURE_SIGNAL)
      set(PLATFORM_SRCFILES "${PLATFORM_SRCFILES}"
            ${UNIX_PATH}/signal.c)
      set(FLAGS_COMMON "${FLAGS_COMMON}" "-DTUV_FEATURE_SIGNAL=1")
      set(TEST_UNITFILES "${TEST_UNITFILES}" "${TEST_ROOT}/test_signal.c")
endif()

# configuration values
set(CONFIG_FILESYSTEM 1)
