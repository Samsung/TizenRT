/****************************************************************************
 *
 * Copyright 2026 Samsung Electronics All Rights Reserved.
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
 * apps/examples/testcase/ostest/fork.c
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FORK_HEAPSIZE   256
#define FORK_PARENTMARK 0x5a
#define FORK_CHILDMARK  0xa5

/* Distinct values written through a pointer to a stack local, to check that
 * the child's stack is at the same virtual address as the parent's.
 */

#define FORK_STACKPARENT 0x1234
#define FORK_STACKCHILD  0x5678

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* .data and .bss, which a fork() child must get its own copy of */

static volatile int  g_forkdata = 1;
static volatile int  g_forkbss;
static FAR unsigned char *g_forkheap;

/* Pointer to a local in fork_test()'s frame, taken before fork().  In .data
 * rather than on the stack:  the compiler could rematerialise a local
 * pointer from the current stack pointer, which would test nothing.
 */

static FAR volatile int *g_forkstackptr;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fork_child
 *
 * Description:
 *   Everything here is forbidden to a vfork() child and permitted to a
 *   fork() child.  Returns the child's exit status.
 *
 ****************************************************************************/

static int fork_child(void)
{
  FAR char *scratch;

  /* The parent must see none of this. */

  g_forkdata = 2;
  g_forkbss  = 2;
  memset(g_forkheap, FORK_CHILDMARK, FORK_HEAPSIZE);

  /* A vfork() child may not call these; a fork() child may. */

  scratch = malloc(64);
  if (scratch == NULL)
    {
      printf("fork_test: ERROR Child could not malloc()\n");
      return 1;
    }

  strlcpy(scratch, "child", 64);
  printf("fork_test: Child running independently (%s)\n", scratch);
  free(scratch);

  /* Give the parent time to make its own writes, so that if the two shared
   * memory we would see the parent's values below rather than our own.
   */

  usleep(200 * 1000);

  if (g_forkdata != 2 || g_forkbss != 2)
    {
      printf("fork_test: ERROR Child saw the parent's writes: "
             "data=%d bss=%d\n", g_forkdata, g_forkbss);
      return 1;
    }

  if (g_forkheap[0] != FORK_CHILDMARK ||
      g_forkheap[FORK_HEAPSIZE - 1] != FORK_CHILDMARK)
    {
      printf("fork_test: ERROR Child saw the parent's heap writes\n");
      return 1;
    }

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fork_test
 *
 * Description:
 *   Verify the defining property of POSIX fork():  the child gets its own
 *   copy of the parent's memory, in both directions.  That is the exact
 *   opposite of the sharing vfork() gives.  Also verifies that none of
 *   vfork()'s restrictions apply.
 *
 ****************************************************************************/

int fork_test(void)
{
  pid_t pid;
  int status = 0;
  int ret = 0;

  /* volatile so it is really in memory and the compare is not folded. */

  volatile int stackvar = FORK_STACKPARENT;

  printf("fork_test: Started\n");

  /* Publish its address before forking.  See g_forkstackptr. */

  g_forkstackptr = &stackvar;

  g_forkdata = 1;
  g_forkbss  = 1;

  g_forkheap = malloc(FORK_HEAPSIZE);
  if (g_forkheap == NULL)
    {
      printf("fork_test: ERROR Failed to allocate the heap probe\n");
      ASSERT(false);
      return -1;
    }

  memset(g_forkheap, FORK_PARENTMARK, FORK_HEAPSIZE);

  pid = fork();
  if (pid == 0)
    {
      /* The child's stack is at the parent's virtual addresses, so this
       * pointer names the child's own live local.  A relocated stack would
       * write into the copy of the parent's instead.
       */

      *g_forkstackptr = FORK_STACKCHILD;
      if (stackvar != FORK_STACKCHILD)
        {
          printf("fork_test: ERROR Child stack was relocated: wrote %d "
                 "through %p, local at %p reads %d\n",
                 FORK_STACKCHILD, g_forkstackptr, &stackvar, stackvar);
          _exit(1);
        }

      /* Returns from fork_child() and from this branch -- both illegal
       * for a vfork() child.
       */

      _exit(fork_child());
    }
  else if (pid < 0)
    {
      printf("fork_test: ERROR fork() failed: %d\n", errno);
      free(g_forkheap);
      ASSERT(false);
      return -1;
    }

  /* Parent runs concurrently:  write now, check isolation afterwards. */

  g_forkdata = 3;
  g_forkbss  = 3;
  memset(g_forkheap, FORK_PARENTMARK, FORK_HEAPSIZE);

#ifdef CONFIG_SCHED_WAITPID
  /* Wait for the child to be done before comparing memory.  waitpid() blocks
   * on a child that is still alive whether or not its exit status will be
   * retained, so this synchronises either way; ECHILD simply means the child
   * had already finished, which is just as good.  It is not a failure:
   * ostest_main() sets SA_NOCLDWAIT on SIGCHLD for the whole run, so an
   * exited child's status is not kept even where CONFIG_SCHED_CHILD_STATUS
   * is enabled.
   */

  if (waitpid(pid, &status, 0) != pid && errno != ECHILD)
    {
      printf("fork_test: ERROR waitpid() failed: %d\n", errno);
      free(g_forkheap);
      ASSERT(false);
      return -1;
    }
#else
  sleep(1);
#endif

  if (g_forkdata != 3 || g_forkbss != 3)
    {
      printf("fork_test: ERROR Parent saw the child's writes: "
             "data=%d bss=%d (expected 3, 3)\n", g_forkdata, g_forkbss);
      ret = -1;
    }

  if (g_forkheap[0] != FORK_PARENTMARK ||
      g_forkheap[FORK_HEAPSIZE - 1] != FORK_PARENTMARK)
    {
      printf("fork_test: ERROR Parent saw the child's heap writes\n");
      ret = -1;
    }

  /* The child wrote to the same stack address; the parent must not see it */

  if (stackvar != FORK_STACKPARENT)
    {
      printf("fork_test: ERROR Parent saw the child's stack write: "
             "%d (expected %d)\n", stackvar, FORK_STACKPARENT);
      ret = -1;
    }

#ifdef CONFIG_SCHED_WAITPID
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    {
      printf("fork_test: ERROR Child reported failure, status 0x%04x\n",
             status);
      ret = -1;
    }
#endif

  free(g_forkheap);
  g_forkheap = NULL;

  if (ret < 0)
    {
      ASSERT(false);
      return ret;
    }

  printf("fork_test: Parent and child had independent memory\n");
  return 0;
}
