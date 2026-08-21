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
 * apps/examples/testcase/ostest/libc_memmem.c
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
#include <string.h>

#include "ostest.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int memmem_test(void)
{
  char *haystack = "hello";
  char *s;

  s = memmem(haystack, 5, "hel", 3);
  ASSERT(s == haystack);

  s = memmem(haystack, 5, "lo", 2);
  ASSERT(s == haystack + 3);

  s = memmem(haystack, 5, "hello", 5);
  ASSERT(s == haystack);

  /* Compare '\0' bytes at string ends. */

  s = memmem(haystack, 6, "o", 2);
  ASSERT(s == haystack + 4);

  /* Must not find needle that is right after end of haystack. */

  s = memmem("helloX", 5, "X", 1);
  ASSERT(s == NULL);

  /* Too long needle should return NULL. */

  s = memmem(haystack, 5, "hellohello", 10);
  ASSERT(s == NULL);

  /* Zero length needle is deemed to reside at start of haystack. */

  s = memmem(haystack, 5, "", 0);
  ASSERT(s == haystack);

  return OK;
}
