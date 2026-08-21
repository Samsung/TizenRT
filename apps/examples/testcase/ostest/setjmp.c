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
 * apps/examples/testcase/ostest/setjmp.c
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
#include <setjmp.h>
#include <stdio.h>
#include <stdbool.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void jump_with_retval(jmp_buf buf, int ret)
{
  volatile bool did_jump = false;
  int value;

  if ((value = setjmp(buf)) == 0)
    {
      printf("setjmp_test: Try jump\n");

      if (did_jump)
        {
          ASSERT(!"setjmp retutns zero after calling longjmp");
        }

      did_jump = true;
      printf("setjmp_test: About to jump, longjmp with ret val: %d\n", ret);
      longjmp(buf, ret);

      /* Unreachable */

      ASSERT(0);
    }
  else
    {
      /* If we provide 0 as the return value to longjmp()
       * we expect it substitute to 1 for us.
       */

      if (ret == 0)
        {
          ret = 1;
        }

      ASSERT(value == ret);
      printf("setjmp_test: Jump succeed\n");
    }
}

void setjmp_test(void)
{
  jmp_buf buf;

  printf("setjmp_test: Initializing jmp_buf\n");

  jump_with_retval(buf, 123);

  /* Pls ref to:
   * the longjmp should never make setjmp returns 0
   * https://pubs.opengroup.org/onlinepubs/009604599/functions/longjmp.html
   */

  jump_with_retval(buf, 0);
}
