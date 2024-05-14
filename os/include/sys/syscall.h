/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
 * include/sys/syscall.h
 * This file contains the system call numbers.
 *
 *   Copyright (C) 2011-2014 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_SYS_SYSCALL_H
#define __INCLUDE_SYS_SYSCALL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#ifndef __ASSEMBLY__
#include <stdint.h>
#endif

#ifdef CONFIG_LIB_SYSCALL

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Reserve the first system calls for platform-specific usage if so
 * configured.
 */

#ifndef CONFIG_SYS_RESERVED
#define CONFIG_SYS_RESERVED            (0)
#endif

/* System call numbers
 *
 * These first system calls are supported regardless of the TinyAra
 * configuration
 */

#define SYS__exit                      (CONFIG_SYS_RESERVED + 0)
#define SYS_exit                       (CONFIG_SYS_RESERVED + 1)
#define SYS_get_errno                  (CONFIG_SYS_RESERVED + 2)
#define SYS_getpid                     (CONFIG_SYS_RESERVED + 3)
#define SYS_sched_getparam             (CONFIG_SYS_RESERVED + 4)
#define SYS_sched_getscheduler         (CONFIG_SYS_RESERVED + 5)
#define SYS_sched_lock                 (CONFIG_SYS_RESERVED + 6)
#define SYS_sched_lockcount            (CONFIG_SYS_RESERVED + 7)
#define SYS_sched_rr_get_interval      (CONFIG_SYS_RESERVED + 8)
#define SYS_sched_setparam             (CONFIG_SYS_RESERVED + 9)
#define SYS_sched_setscheduler         (CONFIG_SYS_RESERVED + 10)
#define SYS_sched_unlock               (CONFIG_SYS_RESERVED + 11)
#define SYS_sched_yield                (CONFIG_SYS_RESERVED + 12)
#define SYS_set_errno                  (CONFIG_SYS_RESERVED + 13)

#ifdef CONFIG_SMP
#define SYS_sched_getaffinity          (CONFIG_SYS_RESERVED + 14)
#define SYS_sched_setaffinity          (CONFIG_SYS_RESERVED + 15)
#define SYS_sched_getcpu               (CONFIG_SYS_RESERVED + 16)
#define __SYS_sem                      (CONFIG_SYS_RESERVED + 17)
#else
#define __SYS_sem                      (CONFIG_SYS_RESERVED + 14)
#endif

/* Semaphores */

#define SYS_sem_destroy                (__SYS_sem + 0)
#define SYS_sem_post                   (__SYS_sem + 1)
#define SYS_sem_timedwait              (__SYS_sem + 2)
#define SYS_sem_trywait                (__SYS_sem + 3)
#define SYS_sem_wait                   (__SYS_sem + 4)

#ifdef CONFIG_PRIORITY_INHERITANCE
#define SYS_sem_setprotocol            (__SYS_sem + 5)
#define __SYS_named_sem                (__SYS_sem + 6)
#else
#define __SYS_named_sem                (__SYS_sem + 5)
#endif

/* Named semaphores */

#ifdef CONFIG_FS_NAMED_SEMAPHORES
#define SYS_sem_open                   __SYS_named_sem
#define SYS_sem_close                  (__SYS_named_sem + 1)
#define SYS_sem_unlink                 (__SYS_named_sem + 2)
#define __SYS_task_create              (__SYS_named_sem + 3)
#else
#define __SYS_task_create              (__SYS_named_sem)
#endif

/* Task creation APIs based on global entry points cannot be use with
 * address environments.
 */

#ifndef CONFIG_BUILD_KERNEL
#define SYS_task_create                __SYS_task_create
#define __SYS_task_delete              (__SYS_task_create + 1)

/* pgalloc() is only available with address environments with the page
 * allocator selected.  MMU support from the CPU is also required.
 */

#else
#define SYS_pgalloc                    __SYS_task_create
#define __SYS_task_delete              (__SYS_task_create + 1)
#endif

#define SYS_task_delete                __SYS_task_delete
#define SYS_task_restart               (__SYS_task_delete + 1)
#ifdef CONFIG_CANCELLATION_POINTS
#define SYS_task_testcancel            (__SYS_task_delete + 2)
#define SYS_task_setcanceltype         (__SYS_task_delete + 3)
#define SYS_task_setcancelstate        (__SYS_task_delete + 4)
#define SYS_up_assert                  (__SYS_task_delete + 5)
#define __SYS_vfork                    (__SYS_task_delete + 6)
#else
#define SYS_task_setcancelstate        (__SYS_task_delete + 2)
#define SYS_up_assert                  (__SYS_task_delete + 3)
#define __SYS_vfork                    (__SYS_task_delete + 4)
#endif


/* The following can be individually enabled */

#if defined(CONFIG_ARCH_HAVE_VFORK) && defined(CONFIG_SCHED_WAITPID)
#define SYS_vfork                      __SYS_vfork
#define __SYS_atexit                   (__SYS_vfork + 1)
#else
#define __SYS_atexit                   __SYS_vfork
#endif

#ifdef CONFIG_SCHED_ATEXIT
#define SYS_atexit                     __SYS_atexit
#define __SYS_on_exit                  (__SYS_atexit + 1)
#else
#define __SYS_on_exit                  __SYS_atexit
#endif

#ifdef CONFIG_SCHED_ONEXIT
#define SYS_on_exit                    __SYS_on_exit
#define __SYS_waitpid                  (__SYS_on_exit + 1)
#else
#define __SYS_waitpid                  __SYS_on_exit
#endif

#ifdef CONFIG_SCHED_WAITPID
#define SYS_waitpid                    __SYS_waitpid
#ifdef CONFIG_SCHED_HAVE_PARENT
#define SYS_wait                       (__SYS_waitpid + 1)
#define SYS_waitid                     (__SYS_waitpid + 2)
#define __SYS_exec                     (__SYS_waitpid + 3)
#else
#define __SYS_exec                     (__SYS_waitpid + 1)
#endif
#else
#define __SYS_exec                     __SYS_waitpid
#endif

/* The following can only be defined if we are configured to execute
 * programs from a file system.
 */

#ifdef CONFIG_BINFMT_ENABLE
#ifndef CONFIG_BUILD_KERNEL
#define SYS_exec                       __SYS_exec
#define __SYS_execv                    (__SYS_exec + 1)
#else
#define __SYS_execv                    __SYS_exec
#endif
#ifdef CONFIG_LIBC_EXECFUNCS
#define SYS_execv                      __SYS_execv
#define __SYS_signals                  (__SYS_execv + 1)
#else
#define __SYS_signals                  __SYS_execv
#endif
#else
#define __SYS_signals                  __SYS_exec
#endif

/* The following are only defined is signals are supported in the TinyAra
 * configuration.
 */

#ifndef CONFIG_DISABLE_SIGNALS
#define SYS_kill                       (__SYS_signals + 0)
#define SYS_sigaction                  (__SYS_signals + 1)
#define SYS_sigpending                 (__SYS_signals + 2)
#define SYS_sigprocmask                (__SYS_signals + 3)
#define SYS_sigqueue                   (__SYS_signals + 4)
#define SYS_sigsuspend                 (__SYS_signals + 5)
#define SYS_sigtimedwait               (__SYS_signals + 6)
#define SYS_sigwaitinfo                (__SYS_signals + 7)
#define SYS_nanosleep                  (__SYS_signals + 8)
#define __SYS_clock                    (__SYS_signals + 9)
#else
#define __SYS_clock                    __SYS_signals
#endif

/* The following are only defined if the system clock is enabled in the
 * TinyAra configuration.
 */

#define SYS_clock                      (__SYS_clock + 0)
#define SYS_clock_getres               (__SYS_clock + 1)
#define SYS_clock_gettime              (__SYS_clock + 2)
#define SYS_clock_settime              (__SYS_clock + 3)
#define SYS_gettimeofday               (__SYS_clock + 4)
#define __SYS_timers                   (__SYS_clock + 5)

/* The following are defined only if POSIX timers are supported */

#ifndef CONFIG_DISABLE_POSIX_TIMERS
#define SYS_timer_create               (__SYS_timers + 0)
#define SYS_timer_delete               (__SYS_timers + 1)
#define SYS_timer_getoverrun           (__SYS_timers + 2)
#define SYS_timer_gettime              (__SYS_timers + 3)
#define SYS_timer_settime              (__SYS_timers + 4)
#define __SYS_descriptors              (__SYS_timers + 5)
#else
#define __SYS_descriptors              __SYS_timers
#endif

/* The following are defined if either file or socket descriptor are
 * enabled.
 */

#ifndef CONFIG_NET
#undef CONFIG_NSOCKET_DESCRIPTORS
#define CONFIG_NSOCKET_DESCRIPTORS 0
#endif

#if CONFIG_NFILE_DESCRIPTORS > 0 || CONFIG_NSOCKET_DESCRIPTORS > 0
#define SYS_close                      (__SYS_descriptors + 0)
#ifdef CONFIG_LIBC_IOCTL_VARIADIC
#define SYS_fs_ioctl                   (__SYS_descriptors + 1)
#else
#define SYS_ioctl                      (__SYS_descriptors + 1)
#endif
#define SYS_read                       (__SYS_descriptors + 2)
#define SYS_write                      (__SYS_descriptors + 3)
#define SYS_pread                      (__SYS_descriptors + 4)
#define SYS_pwrite                     (__SYS_descriptors + 5)
#ifdef CONFIG_FS_AIO
#define SYS_aio_read                   (__SYS_descriptors + 6)
#define SYS_aio_write                  (__SYS_descriptors + 7)
#define SYS_aio_fsync                  (__SYS_descriptors + 8)
#define SYS_aio_cancel                 (__SYS_descriptors + 9)
#define __SYS_poll                     (__SYS_descriptors + 10)
#else
#define __SYS_poll                     (__SYS_descriptors + 6)
#endif
#ifndef CONFIG_DISABLE_POLL
#define SYS_poll                       __SYS_poll
#define SYS_select                     (__SYS_poll + 1)
#define __SYS_boardctl                 (__SYS_poll + 2)
#else
#define __SYS_boardctl                 __SYS_poll
#endif
#else
#define __SYS_boardctl                 __SYS_descriptors
#endif

/* Board support */
#ifdef CONFIG_LIB_BOARDCTL
#define SYS_boardctl                  __SYS_boardctl
#define __SYS_filedesc                (__SYS_boardctl + 1)
#else
#define __SYS_filedesc                __SYS_boardctl
#endif

/* The following are defined if file descriptors are enabled */

#if CONFIG_NFILE_DESCRIPTORS > 0
#define SYS_closedir                   (__SYS_filedesc + 0)
#define SYS_dup                        (__SYS_filedesc + 1)
#define SYS_dup2                       (__SYS_filedesc + 2)
#define SYS_fcntl                      (__SYS_filedesc + 3)
#define SYS_fstat                      (__SYS_filedesc + 4)
#define SYS_fstatfs                    (__SYS_filedesc + 5)
#define SYS_lseek                      (__SYS_filedesc + 6)
#if defined(CONFIG_PIPES)
#define SYS_mkfifo                     (__SYS_filedesc + 7)
#define __SYS_mmap                     (__SYS_filedesc + 8)
#else
#define __SYS_mmap                     (__SYS_filedesc + 7)
#endif
#define SYS_mmap                       (__SYS_mmap + 0)
#define SYS_open                       (__SYS_mmap + 1)
#define SYS_opendir                    (__SYS_mmap + 2)
#if defined(CONFIG_PIPES)
#define SYS_pipe                       (__SYS_mmap + 3)
#define __SYS_readdir                  (__SYS_mmap + 4)
#else
#define __SYS_readdir                  (__SYS_mmap + 3)
#endif
#define SYS_readdir                    (__SYS_readdir + 0)
#define SYS_rewinddir                  (__SYS_readdir + 1)
#define SYS_seekdir                    (__SYS_readdir + 2)
#define SYS_stat                       (__SYS_readdir + 3)
#define SYS_statfs                     (__SYS_readdir + 4)

#if CONFIG_NFILE_STREAMS > 0
#define SYS_fs_fdopen                  (__SYS_readdir + 5)
#define SYS_sched_getstreams           (__SYS_readdir + 6)
#define __SYS_mountpoint               (__SYS_readdir + 7)
#else
#define __SYS_mountpoint               (__SYS_readdir + 5)
#endif

#if !defined(CONFIG_DISABLE_MOUNTPOINT)
#define SYS_fsync                      (__SYS_mountpoint + 0)
#define SYS_mkdir                      (__SYS_mountpoint + 1)
#define SYS_mount                      (__SYS_mountpoint + 2)
#define SYS_rename                     (__SYS_mountpoint + 3)
#define SYS_rmdir                      (__SYS_mountpoint + 4)
#define SYS_umount                     (__SYS_mountpoint + 5)
#define SYS_unlink                     (__SYS_mountpoint + 6)
#define SYS_ftruncate                  (__SYS_mountpoint + 7)
#define __SYS_shm                      (__SYS_mountpoint + 8)
#else
#define __SYS_shm                      __SYS_mountpoint
#endif

#else
#define __SYS_shm                      __SYS_filedesc
#endif

/* Shared memory interfaces */

#ifdef CONFIG_MM_SHM
#define SYS_shmget                     (__SYS_shm + 0)
#define SYS_shmat                      (__SYS_shm + 1)
#define SYS_shmctl                     (__SYS_shm + 2)
#define SYS_shmdt                      (__SYS_shm + 3)
#define __SYS_pthread                  (__SYS_shm + 4)
#else
#define __SYS_pthread                  __SYS_shm
#endif

/* The following are defined if pthreads are enabled */

#ifndef CONFIG_DISABLE_PTHREAD
#define SYS_pthread_cancel             (__SYS_pthread + 0)
#define SYS_pthread_cond_broadcast     (__SYS_pthread + 1)
#define SYS_pthread_cond_signal        (__SYS_pthread + 2)
#define SYS_pthread_cond_wait          (__SYS_pthread + 3)
#define SYS_pthread_create             (__SYS_pthread + 4)
#define SYS_pthread_detach             (__SYS_pthread + 5)
#define SYS_pthread_exit               (__SYS_pthread + 6)
#define SYS_pthread_getschedparam      (__SYS_pthread + 7)
#define SYS_pthread_getspecific        (__SYS_pthread + 8)
#define SYS_pthread_join               (__SYS_pthread + 9)
#define SYS_pthread_tryjoin_np         (__SYS_pthread + 10)
#define SYS_pthread_key_create         (__SYS_pthread + 11)
#define SYS_pthread_key_delete         (__SYS_pthread + 12)
#define SYS_pthread_mutex_destroy      (__SYS_pthread + 13)
#define SYS_pthread_mutex_init         (__SYS_pthread + 14)
#define SYS_pthread_mutex_lock         (__SYS_pthread + 15)
#define SYS_pthread_mutex_trylock      (__SYS_pthread + 16)
#define SYS_pthread_mutex_unlock       (__SYS_pthread + 17)

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
#define SYS_pthread_mutex_consistent   (__SYS_pthread + 18)
#define __SYS_pthread_setaffinity_np   (__SYS_pthread + 19)
#else
#define __SYS_pthread_setaffinity_np   (__SYS_pthread + 18)
#endif

#ifdef CONFIG_SMP
#define SYS_pthread_setaffinity_np     (__SYS_pthread_setaffinity_np + 0)	
#define SYS_pthread_getaffinity_np     (__SYS_pthread_setaffinity_np + 1)	
#define __SYS_pthread_setschedparam    (__SYS_pthread_setaffinity_np + 2)
#else
#define __SYS_pthread_setschedparam    (__SYS_pthread_setaffinity_np + 0)
#endif

#define SYS_pthread_setschedparam      (__SYS_pthread_setschedparam + 0)
#define SYS_pthread_setschedprio       (__SYS_pthread_setschedparam + 1)
#define SYS_pthread_setspecific        (__SYS_pthread_setschedparam + 2)

#ifndef CONFIG_DISABLE_SIGNAL
#define SYS_pthread_cond_timedwait     (__SYS_pthread_setschedparam + 3)
#define SYS_pthread_kill               (__SYS_pthread_setschedparam + 4)
#define SYS_pthread_sigmask            (__SYS_pthread_setschedparam + 5)
#define __SYS_mqueue                   (__SYS_pthread_setschedparam + 6)
#else
#define __SYS_mqueue                   (__SYS_pthread_setschedparam + 3)
#endif

#else
#define __SYS_mqueue                   __SYS_pthread
#endif

/* The following are defined only if message queues are enabled */

#ifndef CONFIG_DISABLE_MQUEUE
#define SYS_mq_close                   (__SYS_mqueue + 0)
#define SYS_mq_getattr                 (__SYS_mqueue + 1)
#define SYS_mq_notify                  (__SYS_mqueue + 2)
#define SYS_mq_open                    (__SYS_mqueue + 3)
#define SYS_mq_receive                 (__SYS_mqueue + 4)
#define SYS_mq_send                    (__SYS_mqueue + 5)
#define SYS_mq_setattr                 (__SYS_mqueue + 6)
#define SYS_mq_timedreceive            (__SYS_mqueue + 7)
#define SYS_mq_timedsend               (__SYS_mqueue + 8)
#define SYS_mq_unlink                  (__SYS_mqueue + 9)
#define __SYS_environ                  (__SYS_mqueue + 10)
#else
#define __SYS_environ                  __SYS_mqueue
#endif

/* The following are defined only if environment variables are supported */

#ifndef CONFIG_DISABLE_ENVIRON
#define SYS_clearenv                   (__SYS_environ + 0)
#define SYS_getenv                     (__SYS_environ + 1)
#define SYS_putenv                     (__SYS_environ + 2)
#define SYS_setenv                     (__SYS_environ + 3)
#define SYS_unsetenv                   (__SYS_environ + 4)
#define SYS_get_environ_ptr            (__SYS_environ + 5)
#define __SYS_network                  (__SYS_environ + 6)
#else
#define __SYS_network                  __SYS_environ
#endif

/* The following are defined only if networking AND sockets are supported */

#if CONFIG_NSOCKET_DESCRIPTORS > 0 && defined(CONFIG_NET)
#define SYS_accept                     (__SYS_network + 0)
#define SYS_bind                       (__SYS_network + 1)
#define SYS_connect                    (__SYS_network + 2)
#define SYS_getpeername                (__SYS_network + 3)
#define SYS_getsockname                (__SYS_network + 4)
#define SYS_getsockopt                 (__SYS_network + 5)
#define SYS_listen                     (__SYS_network + 6)
#define SYS_recv                       (__SYS_network + 7)
#define SYS_recvfrom                   (__SYS_network + 8)
#define SYS_recvmsg                    (__SYS_network + 9)
#define SYS_send                       (__SYS_network + 10)
#define SYS_sendmsg                    (__SYS_network + 11)
#define SYS_sendto                     (__SYS_network + 12)
#define SYS_setsockopt                 (__SYS_network + 13)
#define SYS_shutdown                   (__SYS_network + 14)
#define SYS_socket                     (__SYS_network + 15)
#define __SYS_prctl                    (__SYS_network + 16)
#else
#define __SYS_prctl                    __SYS_network
#endif

#define SYS_prctl                      __SYS_prctl

#define SYS_fin_wait                   SYS_prctl + 1

#define SYS_maxsyscall                 (SYS_fin_wait + 1)

/* Note that the reported number of system calls does *NOT* include the
 * architecture-specific system calls.  If the "real" total is required,
 * use SYS_maxsyscall.
 */

#define SYS_nsyscalls                  (SYS_maxsyscall - CONFIG_SYS_RESERVED)

/****************************************************************************
 * Public Type Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#ifdef __KERNEL__

/* Function lookup tables.  This table is indexed by the system call numbers
 * defined above.  Given the system call number, this table provides the
 * address of the corresponding system function.
 *
 * This table is only available during the kernel phase of a kernel build.
 */

EXTERN const uintptr_t g_funclookup[SYS_nsyscalls];

/* Given the system call number, the corresponding entry in this table
 * provides the address of the stub function.
 *
 * This table is only available during the kernel phase of a kernel build.
 */

EXTERN const uintptr_t g_stublookup[SYS_nsyscalls];

#endif

/* Given the system call number, the corresponding entry in this table
 * provides the number of parameters needed by the function.
 */

EXTERN const uint8_t g_funcnparms[SYS_nsyscalls];
EXTERN const char *g_funcnames[SYS_nsyscalls];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif							/* __ASSEMBLY__ */
#endif							/* CONFIG_LIB_SYSCALL */
#endif							/* __INCLUDE_SYS_SYSCALL_H */
