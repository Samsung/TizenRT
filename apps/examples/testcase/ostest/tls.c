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
 * apps/examples/testcase/ostest/tls.c
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
#include <tinyara/tls.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "ostest.h"

#if defined(CONFIG_TLS_NELEM) && CONFIG_TLS_NELEM > 0

/****************************************************************************
 * Preprocessor Definitions
 ****************************************************************************/

#define copy_tls_info(a, b) memcpy(a, b, sizeof(struct tls_info_s))

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct tls_info_s g_save_info;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void get_tls_info(FAR struct tls_info_s *info)
{
  copy_tls_info(info, tls_get_info());
}

static void put_tls_info(FAR const struct tls_info_s *info)
{
  copy_tls_info(tls_get_info(), info);
}

static void set_tls_info(uintptr_t value)
{
  FAR struct tls_info_s *info = tls_get_info();
  int i;

  for (i = 0; i < CONFIG_TLS_NELEM; i++)
    {
      info->tl_elem[i] = value;
    }
}

static bool verify_tls_info(uintptr_t value)
{
  FAR struct tls_info_s *info = tls_get_info();
  bool fail = false;
  int i;

  for (i = 0; i < CONFIG_TLS_NELEM; i++)
    {
      if (info->tl_elem[i] != value)
        {
          printf("tls: ERROR Element %d: Set %lx / read %lx\n",
                 i, (unsigned long)value,
                 (unsigned long)info->tl_elem[i]);
          ASSERT(false);
          fail = true;
        }
    }

  return fail;
}

static void do_tls_test(uintptr_t value)
{
  set_tls_info(value);
  if (!verify_tls_info(value))
    {
      printf("tls: Successfully set %lx\n", (unsigned long)value);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void tls_test(void)
{
  get_tls_info(&g_save_info);
  do_tls_test(0);
  do_tls_test(0xffffffff);
  do_tls_test(0x55555555);
  do_tls_test(0xaaaaaaaa);
  put_tls_info(&g_save_info);
}

#endif /* CONFIG_TLS_NELEM > 0 */
