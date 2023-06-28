/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
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
 * syscall/syscall_names.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#if defined(CONFIG_LIB_SYSCALL) && defined(__KERNEL__)
#include <syscall.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/mount.h>
#include <sys/boardctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <poll.h>
#include <time.h>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <mqueue.h>
#include <assert.h>

/* Errno access is awkward. We need to generate get_errno() and set_errno()
 * interfaces to support the system calls, even though we don't use them
 * ourself.
 *
 * The "normal" pre-processor defintions for these functions is in errno.h
 * but we need the internal function prototypes in include/errno.h.
 */

#undef get_errno
#undef set_errno

#include <tinyara/errno.h>
#include <tinyara/clock.h>


/****************************************************************************
 * Public Data
 ****************************************************************************/

/* System call name tables.  This table is indexed by the system call numbers
 * defined above.  Given the system call number, this table provides the
 * name of the corresponding system function.
 */

const char *g_funcnames[SYS_nsyscalls] =
{
#  undef SYSCALL_LOOKUP1
#  define SYSCALL_LOOKUP1(f, n, p) #f
#  undef SYSCALL_LOOKUP
#  define SYSCALL_LOOKUP(f, n, p)  #f
#  include "syscall_lookup.h"
};
#endif /* CONFIG_LIB_SYSCALL && __KERNEL__ */
