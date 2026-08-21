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
 * apps/examples/testcase/ostest/setvbuf.c
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
#include <stdio.h>
#include <stdbool.h>

#ifndef CONFIG_STDIO_DISABLE_BUFFERING

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * setvbuf_test
 ****************************************************************************/

int setvbuf_test(void)
{
  FILE *stream;
  char buffer[64];
  int ret;

  printf("setvbuf_test: Test NO buffering\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, NULL, _IONBF, 0);
  if (ret < 0)
    {
      printf("setvbuf_test ERROR: "
             "setvbuf(stream, NULL, _IONBF, 0) failed\n");
      ASSERT(false);
    }

  fprintf(stream, "setvbuf_test: Using NO buffering\r\n");

  /* Close the stream.  setvbuf() may only be called AFTER the stream has
   * been opened and BEFORE any operations have been performed on the
   * stream.
   */

  fclose(stream);

  printf("setvbuf_test: Test default FULL buffering\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, NULL, _IOFBF, 0);
  if (ret < 0)
    {
      printf("ssetvbuf_test ERROR: "
             "setvbuf(stream, NULL, _IOFBF, 0) failed\n");
      ASSERT(false);
    }

  fprintf(stream, "setvbuf_test: Using default FULL buffering\r\n");

  /* Close the stream.  setvbuf() may only be called AFTER the stream has
   * been opened and BEFORE any operations have been performed on the
   * stream.
   */

  fclose(stream);

  printf("setvbuf_test: Test FULL buffering, buffer size 64\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, NULL, _IOFBF, 64);
  if (ret < 0)
    {
      printf("ssetvbuf_test ERROR: "
             "setvbuf(stream, NULL, _IOFBF, 64) failed\n");
      ASSERT(false);
    }

  fprintf(stream, "setvbuf_test: Using FULL buffering, buffer size 64\r\n");

  /* Close the stream.  setvbuf() may only be called AFTER the stream has
   * been opened and BEFORE any operations have been performed on the
   * stream.
   */

  fclose(stream);

  printf("setvbuf_test: Test FULL buffering, pre-allocated buffer\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, buffer, _IOFBF, 64);
  if (ret < 0)
    {
      printf("ssetvbuf_test ERROR: "
             "setvbuf(stream, buffer, _IOFBF, 64) failed\n");
      ASSERT(false);
    }

  fprintf(stream,
          "setvbuf_test: Using FULL buffering, pre-allocated buffer\r\n");

  /* Close the stream.  setvbuf() may only be called AFTER the stream has
   * been opened and BEFORE any operations have been performed on the
   * stream.
   */

  fclose(stream);

  printf("setvbuf_test: Test LINE buffering, buffer size 64\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, NULL, _IOLBF, 64);
  if (ret < 0)
    {
      printf("setvbuf_test ERROR: "
             "setvbuf(stream, NULL, _IOLBF, 64) failed\n");
      ASSERT(false);
    }

  fprintf(stream, "setvbuf_test: Using LINE buffering, buffer size 64\r\n");

  /* Close the stream.  setvbuf() may only be called AFTER the stream has
   * been opened and BEFORE any operations have been performed on the
   * stream.
   */

  fclose(stream);

  printf("setvbuf_test: Test FULL buffering, pre-allocated buffer\n");

  stream = fopen("/dev/console", "w");
  if (stream == NULL)
    {
      printf("setvbuf_test ERROR: fopen(dev/console, rw) failed\n");
      ASSERT(false);
      return ERROR;
    }

  ret = setvbuf(stream, buffer, _IOLBF, 64);
  if (ret < 0)
    {
      printf("setvbuf_test ERROR: "
             "setvbuf(stream, buffer, _IOLBF, 64) failed\n");
      ASSERT(false);
    }

  fprintf(stream,
          "setvbuf_test: Using FULL buffering, pre-allocated buffer\r\n");

  fclose(stream);
  return 0;
}

#endif /* CONFIG_STDIO_DISABLE_BUFFERING */
