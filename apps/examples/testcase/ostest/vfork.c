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
 * apps/examples/testcase/ostest/vfork.c
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: vfork_test
 *
 * Description:
 *   Verify the defining property of vfork():  the parent is suspended until
 *   the child _exit()s or exec()s.  The child does only what POSIX
 *   permits -- _exit(), not exit(), which would flush stdio in the parent's
 *   address space.  Since the child may not write memory, the observable is
 *   its exit status:  an unsuspended parent would reach waitpid() first.
 *
 ****************************************************************************/

int vfork_test(void)
{
  pid_t pid;

  printf("vfork_test: Started\n");

  pid = vfork();
  if (pid == 0)
    {
      /* The only thing a vfork() child may do is leave. */

      _exit(42);
    }
  else if (pid < 0)
    {
      printf("vfork_test: ERROR vfork() failed: %d\n", errno);
      ASSERT(false);
      return -1;
    }

  /* Reached only once the child has exited or exec'ed. */

#ifdef CONFIG_SCHED_WAITPID
    {
      int status = 0;
      pid_t ret;

      ret = waitpid(pid, &status, 0);

      /* Two answers are correct, and which one comes back is a property of
       * the configuration:  a retained status must be exit(42), and ECHILD
       * is equally good evidence -- it says the child was already gone when
       * we asked.  ostest_main() sets SA_NOCLDWAIT for the whole run, so
       * testing CONFIG_SCHED_CHILD_STATUS alone is not enough.
       */

      if (ret == pid)
        {
          if (!WIFEXITED(status) || WEXITSTATUS(status) != 42)
            {
              printf("vfork_test: ERROR Child %d status 0x%04x, expected "
                     "exit(42)\n", pid, status);
              ASSERT(false);
              return -1;
            }
        }
      else if (ret >= 0 || errno != ECHILD)
        {
          printf("vfork_test: ERROR waitpid() returned %d (%d), expected "
                 "the child's status or ECHILD\n", ret, errno);
          ASSERT(false);
          return -1;
        }
    }
#endif

  printf("vfork_test: Child %d ran and exited before the parent resumed\n",
         pid);
  return 0;
}
