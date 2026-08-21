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
 * apps/examples/testcase/ostest/getopt.c
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
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor DEFINITIONS
 ****************************************************************************/

#define SHORT_RESULT_A(n) \
 { \
    results[n].ret  = 'a'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
 }

#define SHORT_RESULT_B(n) \
 { \
    results[n].ret  = 'b'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
 }

#define SHORT_RESULT_C1(n) \
 { \
    results[n].ret  = 'c'; \
    results[n].flag = 0; \
    results[n].arg  = "Arg1"; \
 }

#define SHORT_RESULT_C2(n) \
 { \
    results[n].ret  = 'c'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
 }

#define SHORT_RESULT_D(n) \
 { \
    results[n].ret  = 'd'; \
    results[n].flag = 0; \
    results[n].arg  = "Arg2"; \
 }

#define SHORT_RESULT_X(n) \
 { \
    results[n].ret  = '?'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
 }

#define LONG_OPTION_A(n) \
  { \
    long_options[n].name    = "OptionA"; \
    long_options[n].has_arg = no_argument; \
    long_options[n].flag    = NULL; \
    long_options[n].val     = 'a'; \
  }

#define LONG_OPTION_B(n) \
  { \
    long_options[n].name    = "OptionB"; \
    long_options[n].has_arg = no_argument; \
    long_options[n].flag    = &g_flag; \
    long_options[n].val     = 'b'; \
  }

#define LONG_OPTION_C(n) \
  { \
    long_options[n].name    = "OptionC"; \
    long_options[n].has_arg = required_argument; \
    long_options[n].flag    = NULL; \
    long_options[n].val     = 'c'; \
  }

#define LONG_OPTION_D(n) \
  { \
    long_options[n].name    = "OptionD"; \
    long_options[n].has_arg = optional_argument; \
    long_options[n].flag    = &g_flag; \
    long_options[n].val     = 'd'; \
  }

#define LONG_OPTION_END(n) \
  { \
    long_options[n].name    = NULL; \
    long_options[n].has_arg = 0; \
    long_options[n].flag    = NULL; \
    long_options[n].val     = 0; \
  }

#define LONG_RESULT_A(n) \
  { \
    results[n].ret  = 'a'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
  }

#define LONG_RESULT_B(n) \
  { \
    results[n].ret  = OK; \
    results[n].flag = 'b'; \
    results[n].arg  = NULL; \
  }

#define LONG_RESULT_C(n) \
  { \
    results[n].ret  = 'c'; \
    results[n].flag = 0; \
    results[n].arg  = "Arg1"; \
  }

#define LONG_RESULT_D1(n) \
  { \
    results[n].ret  = OK; \
    results[n].flag = 'd'; \
    results[n].arg  = "Arg2"; \
  }

#define LONG_RESULT_D2(n) \
  { \
    results[n].ret  = OK; \
    results[n].flag = 'd'; \
    results[n].arg  = NULL; \
  }

#define LONG_RESULT_X(n) \
  { \
    results[n].ret  = '?'; \
    results[n].flag = 0; \
    results[n].arg  = NULL; \
  }

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct result_s
{
  int ret;
  int flag;
  FAR char *arg;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static int g_flag;
static const char *g_optstring = "abc::d:";

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int getopt_short_test(int noptions, int argc, FAR char **argv,
                             FAR const char *optstring,
                             FAR const struct result_s *expected)
{
  int ndx;
  int ret;

  optarg = NULL;

  for (ndx = 0;
       (ret = getopt(argc, argv, optstring)) != ERROR && ndx < noptions;
       ndx++)
    {
      /* optind may index through argc (to the NULL argv entry) since it is
       * required to always point to the next command line argument.
       */

      if (optind < 1 || optind > argc)
        {
          printf("ERROR: optind=%d\n", optind);
          ASSERT(false);
        }

      /* Parse until getopt(), but do not process anything if ndx exceeds
       * noptions.
       */

      if (ndx < noptions)
        {
          if (expected[ndx].ret != ret)
            {
              printf("ERROR: arg %d:  ret=%d (expected %d)\n",
                     ndx + 1, ret, expected[ndx].ret);
              ASSERT(false);
            }

          if ((expected[ndx].arg == NULL &&
               optarg != NULL) ||
              (expected[ndx].arg != NULL &&
               strcmp(expected[ndx].arg, optarg) != 0))
            {
              printf("ERROR: arg %d:  optarg=%s (expected %s)\n",
                     ndx + 1, optarg == NULL ? "null" : optarg,
                     expected[ndx].arg == NULL ? "null" :
                     expected[ndx].arg);
              ASSERT(false);
            }
        }

      optarg = NULL;
    }

  /* Verify the number of options.  Some tests have and extra value at the
   * end of the command line after the options.
   */

  if (ndx != noptions && ndx != noptions + 1)
    {
      printf("ERROR: ndx=%d (expected %d)\n", ndx, noptions);
      ASSERT(false);
    }

  return OK;
}

static int getopt_long_test(int noptions, int argc, FAR char **argv,
                            FAR const char *optstring,
                            FAR const struct option *longopts,
                            FAR int *longindex,
                            FAR const struct result_s *expected)
{
  int ndx;
  int ret;

  optarg = NULL;
  g_flag = 0;

  for (ndx = 0;
       (ret = getopt_long(argc, argv, optstring, longopts,
                          longindex)) != ERROR;
       ndx++)
    {
      /* optind may index through argc (to the NULL argv entry) since it is
       * required to always point to the next command line argument.
       */

      if (optind < 1 || optind > argc)
        {
          printf("ERROR: optind=%d\n", optind);
          ASSERT(false);
        }

      /* Parse until getop_long(), but do not process anything if ndx exceeds
       * noptions.
       */

      if (ndx < noptions)
        {
          if (expected[ndx].ret != ret)
            {
              printf("ERROR: arg %d:  ret=%d (expected %d)\n",
                     ndx + 1, ret, expected[ndx].ret);
              ASSERT(false);
            }

          if (expected[ndx].flag != g_flag)
            {
              printf("ERROR: arg %d;  flag=%d (expected %d)\n",
                     ndx + 1, expected[ndx].flag, g_flag);
              ASSERT(false);
            }

          if ((expected[ndx].arg == NULL &&
               optarg != NULL) ||
              (expected[ndx].arg != NULL &&
               strcmp(expected[ndx].arg, optarg) != 0))
            {
              printf("ERROR: arg %d:  optarg=%s (expected %s)\n",
                     ndx + 1, optarg == NULL ? "null" : optarg,
                     expected[ndx].arg == NULL ? "null" :
                     expected[ndx].arg);
              ASSERT(false);
            }
        }

      optarg = NULL;
      g_flag = 0;
    }

  /* Verify the number of options.  Some tests have and extra value at the
   * end of the command line after the options.
   */

  if (ndx != noptions && ndx != noptions + 1)
    {
      printf("ERROR: ndx=%d (expected %d)\n", ndx, noptions);
      ASSERT(false);
    }

  return OK;
}

static int getopt_longonly_test(int noptions, int argc, FAR char **argv,
                                FAR const char *optstring,
                                FAR const struct option *longopts,
                                FAR int *longindex,
                                FAR const struct result_s *expected)
{
  int ndx;
  int ret;

  optarg = NULL;
  g_flag = 0;

  for (ndx = 0;
       (ret = getopt_long_only(argc, argv, optstring, longopts,
                               longindex)) != ERROR;
       ndx++)
    {
      /* optind may index through argc (to the NULL argv entry) since it is
       * required to always point to the next command line argument.
       */

      if (optind < 1 || optind > argc)
        {
          printf("ERROR: optind=%d\n", optind);
          ASSERT(false);
        }

      /* Parse until getop_long(), but do not process anything if ndx exceeds
       * noptions.
       */

      if (ndx < noptions)
        {
          if (expected[ndx].ret != ret)
            {
              printf("ERROR: arg %d:  ret=%d (expected %d)\n",
                     ndx + 1, ret, expected[ndx].ret);
              ASSERT(false);
            }

          if (expected[ndx].flag != g_flag)
            {
              printf("ERROR: arg %d;  flag=%d (expected %d)\n",
                     ndx + 1, expected[ndx].flag, g_flag);
              ASSERT(false);
            }

          if ((expected[ndx].arg == NULL &&
               optarg != NULL) ||
              (expected[ndx].arg != NULL &&
               strcmp(expected[ndx].arg, optarg) != 0))
            {
              printf("ERROR: arg %d:  optarg=%s (expected %s)\n",
                     ndx + 1, optarg == NULL ? "null" : optarg,
                     expected[ndx].arg == NULL ? "null" :
                     expected[ndx].arg);
              ASSERT(false);
            }
        }

      optarg = NULL;
      g_flag = 0;
    }

  /* Verify the number of options.  Some tests have and extra value at the
   * end of the command line after the options.
   */

  if (ndx != noptions && ndx != noptions + 1)
    {
      printf("ERROR: ndx=%d (expected %d)\n", ndx, noptions);
      ASSERT(false);
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int getopt_test(void)
{
  struct option long_options[5];
  struct result_s results[5];
  FAR char *argv[10];

  printf("getopt():  Simple test\n");

  argv[0] = NULL;
  argv[1] = "-a";
  argv[2] = "-b";
  argv[3] = "-c";
  argv[4] = "Arg1";
  argv[5] = "-d";
  argv[6] = "Arg2";
  argv[7] = "NoOption";
  argv[8] = NULL;

  SHORT_RESULT_A(0);
  SHORT_RESULT_B(1);
  SHORT_RESULT_C1(2);
  SHORT_RESULT_D(3);

  getopt_short_test(4, 8, argv, g_optstring, results);

  printf("getopt():  Invalid argument\n");

  argv[0] = NULL;
  argv[1] = "-a";
  argv[2] = "-b";
  argv[3] = "-c";
  argv[4] = "Arg1";
  argv[5] = "-d";
  argv[6] = "Arg2";
  argv[7] = "-x";
  argv[8] = "NoOption";
  argv[9] = NULL;

  SHORT_RESULT_A(0);
  SHORT_RESULT_B(1);
  SHORT_RESULT_C1(2);
  SHORT_RESULT_D(3);
  SHORT_RESULT_X(4);

  getopt_short_test(5, 9, argv, g_optstring, results);

  argv[8] = NULL;
  getopt_short_test(5, 8, argv, g_optstring, results);

  printf("getopt():  Missing optional argument\n");

  argv[0] = NULL;
  argv[1] = "-a";
  argv[2] = "-b";
  argv[3] = "-c";
  argv[4] = "-d";
  argv[5] = "Arg2";
  argv[6] = "NoOption";
  argv[7] = NULL;

  SHORT_RESULT_A(0);
  SHORT_RESULT_B(1);
  SHORT_RESULT_C2(2);
  SHORT_RESULT_D(3);

  getopt_short_test(4, 7, argv, g_optstring, results);

  printf("getopt_long():  Simple test\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "--OptionB";
  argv[3] = "--OptionC";
  argv[4] = "Arg1";
  argv[5] = "--OptionD";
  argv[6] = "Arg2";
  argv[7] = "NoOption";
  argv[8] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_B(1);
  LONG_OPTION_C(2);
  LONG_OPTION_D(3);
  LONG_OPTION_END(4)

  LONG_RESULT_A(0);
  LONG_RESULT_B(1);
  LONG_RESULT_C(2);
  LONG_RESULT_D1(3);

  getopt_long_test(4, 8, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long():  No short options\n");

  getopt_long_test(4, 8, argv, NULL, long_options, NULL,
                   results);

  printf("getopt_long():  Argument for --option=argument\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "--OptionB";
  argv[3] = "--OptionC=Arg1";
  argv[4] = "--OptionD=Arg2";
  argv[5] = "NoOption";
  argv[6] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_B(1);
  LONG_OPTION_C(2);
  LONG_OPTION_D(3);
  LONG_OPTION_END(4)

  LONG_RESULT_A(0);
  LONG_RESULT_B(1);
  LONG_RESULT_C(2);
  LONG_RESULT_D1(3);

  getopt_long_test(4, 6, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long():  Invalid long option\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "--OptionB";
  argv[3] = "--OptionC";
  argv[4] = "Arg1";
  argv[5] = "--OptionD";
  argv[6] = "Arg2";
  argv[7] = "--OptionX";
  argv[8] = "NoOption";
  argv[9] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_B(1);
  LONG_OPTION_C(2);
  LONG_OPTION_D(3);
  LONG_OPTION_END(4)

  LONG_RESULT_A(0);
  LONG_RESULT_B(1);
  LONG_RESULT_C(2);
  LONG_RESULT_D1(3);
  LONG_RESULT_X(4);

  getopt_long_test(5, 9, argv, g_optstring, long_options, NULL,
                   results);

  argv[8] = NULL;
  getopt_long_test(5, 8, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long():  Mixed long and short options\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "-b";
  argv[3] = "--OptionC";
  argv[4] = "Arg1";
  argv[5] = "-d";
  argv[6] = "Arg2";
  argv[7] = "NoOption";
  argv[8] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_C(1);
  LONG_OPTION_END(2)

  LONG_RESULT_A(0);
  SHORT_RESULT_B(1);
  LONG_RESULT_C(2);
  SHORT_RESULT_D(3);

  getopt_long_test(4, 8, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long():  Invalid short option\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "--OptionB";
  argv[3] = "--OptionC";
  argv[4] = "Arg1";
  argv[5] = "--OptionD";
  argv[6] = "Arg2";
  argv[7] = "-x";
  argv[8] = "NoOption";
  argv[9] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_B(1);
  LONG_OPTION_C(2);
  LONG_OPTION_D(3);
  LONG_OPTION_END(4)

  LONG_RESULT_A(0);
  LONG_RESULT_B(1);
  LONG_RESULT_C(2);
  LONG_RESULT_D1(3);
  SHORT_RESULT_X(4);

  getopt_long_test(5, 9, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long():  Missing optional arguments\n");

  argv[0] = NULL;
  argv[1] = "--OptionA";
  argv[2] = "--OptionB";
  argv[3] = "-c";
  argv[4] = "--OptionD";
  argv[5] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_B(1);
  LONG_OPTION_D(2);
  LONG_OPTION_END(3)

  LONG_RESULT_A(0);
  LONG_RESULT_B(1);
  SHORT_RESULT_C2(2);
  LONG_RESULT_D2(3);

  getopt_long_test(4, 6, argv, g_optstring, long_options, NULL,
                   results);

  printf("getopt_long_only():  Mixed long and short options\n");

  argv[0] = NULL;
  argv[1] = "-a";
  argv[2] = "--OptionB";
  argv[3] = "-c";
  argv[4] = "Arg1";
  argv[5] = "--OptionD";
  argv[6] = "Arg2";
  argv[7] = "NoOption";
  argv[8] = NULL;

  LONG_OPTION_B(0);
  LONG_OPTION_D(1);
  LONG_OPTION_END(2)

  SHORT_RESULT_A(0);
  LONG_RESULT_B(1);
  SHORT_RESULT_C1(2);
  LONG_RESULT_D1(3);

  getopt_longonly_test(4, 8, argv, g_optstring, long_options, NULL,
                       results);

  printf("getopt_long_only():  Single hyphen long options\n");

  argv[0] = NULL;
  argv[1] = "-OptionA";
  argv[2] = "-b";
  argv[3] = "--OptionC";
  argv[4] = "Arg1";
  argv[5] = "-d";
  argv[6] = "Arg2";
  argv[7] = "NoOption";
  argv[8] = NULL;

  LONG_OPTION_A(0);
  LONG_OPTION_C(1);
  LONG_OPTION_END(2)

  LONG_RESULT_A(0);
  SHORT_RESULT_B(1);
  LONG_RESULT_C(2);
  SHORT_RESULT_D(3);

  getopt_longonly_test(4, 8, argv, g_optstring, long_options, NULL,
                       results);
  return OK;
}
