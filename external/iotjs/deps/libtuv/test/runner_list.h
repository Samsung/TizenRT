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


#if !defined(__TUV_RAW__)
#define TEST_LIST_ALL(TE)                                                     \
  TE(idle_basic, 5000)                                                        \
  TE(active, 5000)                                                            \
  TE(timer_init, 5000)                                                        \
  TE(timer, 5000)                                                             \
  TE(timer_start_twice, 5000)                                                 \
  TE(timer_order, 5000)                                                       \
  TE(timer_run_once, 5000)                                                    \
  TE(timer_null_callback, 5000)                                               \
  TE(timer_again, 5000)                                                       \
  TE(timer_huge_timeout, 5000)                                                \
  TE(timer_huge_repeat, 5000)                                                 \
  \
  TE(async, 5000)                                                             \
  TE(condvar_2, 5000)                                                         \
  TE(condvar_3, 5000)                                                         \
  TE(cwd, 5000)                                                               \
  \
  TE(fs_file_noent, 5000)                                                     \
  TE(fs_file_sync, 5000)                                                      \
  TE(fs_file_async, 5000)                                                     \
  TE(fs_file_write_null_buffer, 5000)                                         \
  TE(fs_stat_missing_path, 5000)                                              \
  TE(fs_scandir_empty_dir, 5000)                                              \
  TE(fs_scandir_file, 5000)                                                   \
  TE(fs_open_dir, 5000)                                                       \
  TE(fs_file_open_append, 5000)                                               \
  TE(fs_read_file_eof, 5000)                                                  \
  TE(fs_async_dir, 5000)                                                      \
  \
  TE(error_message, 5000)                                                     \
  \
  TE(threadpool_queue_work_simple, 5000)                                      \
  TE(walk_handles, 5000)                                                      \
  \
  TE(tcp_open,5000)                                                           \
  TE(shutdown_eof,5000)                                                       \

#else
#define TEST_LIST_ALL(TE)                                                     \
  TE(idle_basic, 5000)                                                        \
  TE(active, 5000)                                                            \
  TE(timer_init, 5000)                                                        \
  TE(timer, 5000)                                                             \
  TE(timer_start_twice, 5000)                                                 \
  TE(timer_order, 5000)                                                       \
  TE(timer_run_once, 5000)                                                    \
  TE(timer_null_callback, 5000)                                               \
  TE(timer_again, 5000)                                                       \
  TE(timer_huge_timeout, 5000)                                                \
  TE(timer_huge_repeat, 5000)                                                 \
  \
  TE(async, 5000)                                                             \
  TE(condvar_1, 5000)                                                         \
  \
  TE(threadpool_queue_work_simple, 5000)                                      \
  TE(walk_handles, 5000)                                                      \
  TE(tcp_open,5000)                                                           \
  TE(shutdown_eof,5000)                                                       \


#endif

// shutdown_eof should be last of tcp test, it'll stop "echo_sevrer"

#if defined(__linux__) && defined(TUV_FEATURE_PIPE)
#define TEST_LIST_EXT_PIPE(TE)                                                \
  TE(pipe_bind_error_addrinuse, 5000)                                         \
  TE(pipe_bind_error_addrnotavail, 5000)                                      \
  TE(pipe_bind_error_inval, 5000)                                             \
  TE(pipe_listen_without_bind, 5000)                                          \
/*TE(pipe_close_stdout_read_stdin, 5000)*/                                    \
  TE(pipe_connect_bad_name, 5000)                                             \
/*TE(pipe_connect_to_file, 5000)*/                                            \
/*TE(pipe_connect_multiple, 5000)*/                                           \
/*TE(pipe_connect_on_prepare, 5000)*/                                         \
  TE(pipe_getsockname, 5000)                                                  \
  TE(pipe_getsockname_abstract, 5000)                                         \
  TE(pipe_getsockname_blocking, 5000)                                         \
  TE(pipe_pending_instances, 5000)                                            \
  TE(pipe_sendmsg, 5000)                                                      \
  TE(pipe_server_close, 5000)                                                 \
  TE(pipe_set_non_blocking, 5000)
#else
#define TEST_LIST_EXT_PIPE(TE)
#endif

#if defined(__linux__) && defined(TUV_FEATURE_SIGNAL)
#define TEST_LIST_EXT_SIGNAL(TE)                                              \
  TE(we_get_signal, 5000)                                                     \
  TE(we_get_signals, 5000)
#else
#define TEST_LIST_EXT_SIGNAL(TE)
#endif

#if defined(__linux__) && defined(TUV_FEATURE_PROCESS)
#define TEST_LIST_EXT_PROCESS(TE)                                             \
/*TE(ipc_listen_before_write, 5000)*/                                         \
/*TE(ipc_listen_after_write, 5000)*/                                          \
/*TE(ipc_tcp_connection, 5000)*/                                              \
/*TE(spawn_fails_check_for_waitpid_cleanup, 5000) */                          \
/*TE(spawn_exit_code, 5000) */                                                \
  TE(spawn_stdout, 5000)                                                      \
/*TE(spawn_stdout_to_file, 5000) */                                           \
/*TE(spawn_stdout_and_stderr_to_file, 5000) */                                \
/*TE(spawn_stdout_and_stderr_to_file2, 5000) */                               \
/*TE(spawn_stdout_and_stderr_to_file_swap, 5000) */                           \
/*TE(spawn_stdin, 5000) */                                                    \
/*TE(spawn_stdio_greater_than_3, 5000) */                                     \
/*TE(spawn_ignored_stdio, 5000) */                                            \
/*TE(spawn_and_kill, 5000) */                                                 \
/*TE(spawn_preserve_env, 5000) */                                             \
/*TE(spawn_detached, 5000) */                                                 \
/*TE(spawn_and_kill_with_std, 5000) */                                        \
/*TE(spawn_and_ping, 5000) */                                                 \
/*TE(spawn_same_stdout_stderr, 5000) */                                       \
/*TE(spawn_closed_process_io, 5000) */                                        \
/*TE(spawn_fails, 5000) */                                                    \
/*TE(kill, 5000) */                                                           \
/*TE(spawn_setuid_fails, 5000) */                                             \
/*TE(spawn_setgid_fails, 5000) */                                             \
/*TE(spawn_setuid_fails, 5000) */                                             \
/*TE(spawn_setgid_fails, 5000) */                                             \
/*TE(spawn_auto_unref, 5000) */                                               \
/*TE(spawn_fs_open, 5000) */                                                  \
/*TE(closed_fd_events, 5000) */                                               \
/*TE(spawn_reads_child_path, 5000) */                                         \
/*TE(spawn_inherit_streams, 5000) */
#else
#define TEST_LIST_EXT_PROCESS(TE)
#endif

#if defined(__linux__)
#define TEST_LIST_EXT(TE)                                                     \
  TE(condvar_1, 5000)                                                         \
  TE(condvar_4, 5000)                                                         \
  \
  TE(fs_file_nametoolong, 5000)                                               \
  TE(fs_fstat, 5000)                                                          \
  TE(fs_utime, 5000)                                                          \
  TE(fs_futime, 5000)                                                         \
  \
  TE(getaddrinfo_basic, 5000)                                                 \
  TE(getaddrinfo_basic_sync, 5000)                                            \
  TE(getaddrinfo_concurrent, 5000)                                            \
  \
  TEST_LIST_EXT_PIPE(TE) \
  TEST_LIST_EXT_PROCESS(TE) \
  TEST_LIST_EXT_SIGNAL(TE)

#elif defined(__TUV_RAW__)
#define TEST_LIST_EXT(TE)                                                     \

#else
#define TEST_LIST_EXT(TE)                                                     \

#endif

#if !defined(__TUV_RAW__)
#define HELPER_LIST_ALL(TE)                                                   \
  TE(tcp4_echo_server, tcp4_echo_server)                                      \

#else
#define HELPER_LIST_ALL(TE)                                                   \

#endif
