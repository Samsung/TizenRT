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
 * apps/examples/testcase/ostest/dev_null.c
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
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "ostest.h"

/****************************************************************************
 * Private Data
 ****************************************************************************/

static FAR char buffer[1024];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int dev_null_test(void)
{
  int nbytes;
  int fd;

  fd = open("/dev/null", O_RDWR);
  if (fd < 0)
    {
      printf("dev_null: ERROR Failed to open /dev/null\n");
      ASSERT(false);
      return -1;
    }

  nbytes = read(fd, buffer, 1024);
  if (nbytes < 0)
    {
      printf("dev_null: ERROR Failed to read from /dev/null\n");
      ASSERT(false);
      close(fd);
      return -1;
    }

  printf("dev_null: Read %d bytes from /dev/null\n", nbytes);

  nbytes = write(fd, buffer, 1024);
  if (nbytes < 0)
    {
      printf("dev_null: ERROR Failed to write to /dev/null\n");
      ASSERT(false);
      close(fd);
      return -1;
    }

  printf("dev_null: Wrote %d bytes to /dev/null\n", nbytes);

  close(fd);
  return 0;
}
