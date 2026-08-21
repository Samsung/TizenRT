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
 * apps/examples/testcase/ostest/ostest_main.c
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

#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef CONFIG_TESTING_OSTEST_POWEROFF
#include <sys/boardctl.h>
#endif

#include <tinyara/init.h>

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define PRIORITY         100
#define NARGS              4
#define HALF_SECOND_USEC 500000L

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const char arg1[] = "Arg1";
static const char arg2[] = "Arg2";
static const char arg3[] = "Arg3";
static const char arg4[] = "Arg4";

static const char write_data1[] = "stdio_test: write fd=1\n";
static const char write_data2[] = "stdio_test: write fd=2\n";

#ifdef SDCC
/* I am not yet certain why SDCC does not like the following
 * initializer.  It involves some issues with 2- vs 3-byte
 * pointer types.
 */

static const char *g_argv[NARGS + 1];
#else
static const char *g_argv[NARGS + 1] =
{
  arg1, arg2, arg3, arg4, NULL
};
#endif

static struct mallinfo g_mmbefore;
static struct mallinfo g_mmprevious;
static struct mallinfo g_mmafter;

#ifndef CONFIG_DISABLE_ENVIRON
static const char g_var1_name[]    = "Variable1";
static const char g_var1_value[]   = "GoodValue1";
static const char g_var2_name[]    = "Variable2";
static const char g_var2_value[]   = "GoodValue2";
static const char g_var3_name[]    = "Variable3";
static const char g_var3_value[]   = "GoodValue3";

static const char g_bad_value1[]   = "BadValue1";
static const char g_bad_value2[]   = "BadValue2";

static const char g_putenv_value[] = "Variable1=BadValue3";
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: show_memory_usage
 ****************************************************************************/

static void show_memory_usage(struct mallinfo *mmbefore,
                              struct mallinfo *mmafter)
{
  printf("VARIABLE  BEFORE   AFTER\n");
  printf("======== ======== ========\n");
  printf("arena    %8x %8x\n", mmbefore->arena,    mmafter->arena);
  printf("ordblks  %8d %8d\n", mmbefore->ordblks,  mmafter->ordblks);
  printf("mxordblk %8x %8x\n", mmbefore->mxordblk, mmafter->mxordblk);
  printf("uordblks %8x %8x\n", mmbefore->uordblks, mmafter->uordblks);
  printf("fordblks %8x %8x\n", mmbefore->fordblks, mmafter->fordblks);
}

/****************************************************************************
 * Name: check_test_memory_usage
 ****************************************************************************/

static void check_test_memory_usage(void)
{
  /* Wait a little bit to let any threads terminate */

  usleep(HALF_SECOND_USEC);

  /* Get the current memory usage */

  g_mmafter = mallinfo();

  /* Show the change from the previous time */

  printf("\nEnd of test memory usage:\n");
  show_memory_usage(&g_mmprevious, &g_mmafter);

  /* Set up for the next test */

  g_mmprevious = g_mmafter;

  /* If so enabled, show the use of priority inheritance resources */

  dump_nfreeholders("user_main:");
}

/****************************************************************************
 * Name: show_variable
 ****************************************************************************/

#ifndef CONFIG_DISABLE_ENVIRON
static void show_variable(const char *var_name, const char *exptd_value,
                          bool var_valid)
{
  char *actual_value = getenv(var_name);
  if (actual_value)
    {
      if (var_valid)
        {
          if (strcmp(actual_value, exptd_value) == 0)
            {
              printf("show_variable: Variable=%s has value=%s\n",
                     var_name, exptd_value);
            }
          else
            {
              printf("show_variable: ERROR Variable=%s has the wrong "
                     "value\n",
                     var_name);
              ASSERT(false);
              printf("show_variable:       found=%s expected=%s\n",
                     actual_value, exptd_value);
            }
        }
      else
        {
          printf("show_variable: ERROR Variable=%s has a value when it "
                 "should not\n",
                 var_name);
          ASSERT(false);
          printf("show_variable:       value=%s\n",
                 actual_value);
        }
    }
  else if (var_valid)
    {
      printf("show_variable: ERROR Variable=%s has no value\n",
             var_name);
      ASSERT(false);
      printf("show_variable:       Should have had value=%s\n",
             exptd_value);
    }
  else
    {
      printf("show_variable: Variable=%s has no value\n", var_name);
    }
}

static void show_environment(bool var1_valid, bool var2_valid,
                             bool var3_valid)
{
  show_variable(g_var1_name, g_var1_value, var1_valid);
  show_variable(g_var2_name, g_var2_value, var2_valid);
  show_variable(g_var3_name, g_var3_value, var3_valid);
}
#else
#  define show_environment()
#endif

/****************************************************************************
 * Name: user_main
 ****************************************************************************/

static int user_main(int argc, char *argv[])
{
  int i;

  /* Sample the memory usage now */

  usleep(HALF_SECOND_USEC);

  g_mmbefore = mallinfo();
  g_mmprevious = g_mmbefore;

  /* Run the two fork primitives first.  They exercise the lowest-level
   * machinery in the suite, so a fault takes the process down rather than
   * reporting a failure -- better to learn that in seconds.
   */

/* The simulator advertises vfork() but takes the process down when the test
 * runs it.  Excluded since apps ee7642793, "temporary remove vfork test on
 * arch/sim".
 */

#if defined(CONFIG_ARCH_HAVE_VFORK) && !defined(CONFIG_ARCH_SIM)
  printf("\nuser_main: vfork() test\n");
  vfork_test();
  check_test_memory_usage();
#endif

#ifdef CONFIG_ARCH_HAVE_FORK
  printf("\nuser_main: fork() test\n");
  fork_test();
  check_test_memory_usage();
#endif

  printf("\nuser_main: Begin argument test\n");
  printf("user_main: Started with argc=%d\n", argc);

  /* Verify passed arguments */

  if (argc != NARGS + 1)
    {
      printf("user_main: ERROR expected argc=%d got argc=%d\n",
             NARGS + 1, argc);
      ASSERT(false);
    }

  for (i = 0; i <= NARGS; i++)
    {
      printf("user_main: argv[%d]=\"%s\"\n", i, argv[i]);
    }

  for (i = 1; i <= NARGS; i++)
    {
      if (strcmp(argv[i], g_argv[i - 1]) != 0)
        {
          printf("user_main: ERROR argv[%d]:  "
                 "Expected \"%s\" found \"%s\"\n",
                 i, g_argv[i - 1], argv[i]);
          ASSERT(false);
        }
    }

  check_test_memory_usage();

  /* Test additional getopt(), getopt_long(), and getopt_long_only()
   * features.
   */

  printf("\nuser_main: getopt() test\n");
  getopt_test();
  check_test_memory_usage();

  /* Test misc libc functions. */

  printf("\nuser_main: libc tests\n");
  memmem_test();
  check_test_memory_usage();

  /* If retention of child status is enable, then suppress it for this task.
   * This task may produce many, many children (especially if
   * CONFIG_TESTING_OSTEST_LOOPS) and it does not harvest their exit status.
   * As a result, the test may fail inappropriately unless retention of
   * child exit status is disabled.
   *
   * So basically, this tests that child status can be disabled, but cannot
   * verify that status is retained correctly.
   */

#if defined(CONFIG_SCHED_HAVE_PARENT) && defined(CONFIG_SCHED_CHILD_STATUS) && \
    defined(CONFIG_ENABLE_ALL_SIGNALS)
    {
      struct sigaction sa;
      int ret;

      sa.sa_handler = SIG_IGN;
      sa.sa_flags = SA_NOCLDWAIT;
      ret = sigaction(SIGCHLD, &sa, NULL);
      if (ret < 0)
        {
          printf("user_main: ERROR: sigaction failed: %d\n", errno);
          ASSERT(false);
        }
    }
#endif

#ifndef CONFIG_DISABLE_ENVIRON
  /* Check environment variables */

  show_environment(true, true, true);

  unsetenv(g_var1_name);
  show_environment(false, true, true);
  check_test_memory_usage();

  clearenv();
  show_environment(false, false, false);
  check_test_memory_usage();
#endif

#if defined(CONFIG_TLS_NELEM) && CONFIG_TLS_NELEM > 0
  /* Test TLS */

  tls_test();
  check_test_memory_usage();
#endif

#ifdef CONFIG_SCHED_THREAD_LOCAL
  /* Test __thread/thread_local keyword */

  printf("\nuser_main: sched_thread_local test\n");
  sched_thread_local_test();
  check_test_memory_usage();
#endif

  /* Top of test loop */

#if CONFIG_TESTING_OSTEST_LOOPS > 1
  for (i = 0; i < CONFIG_TESTING_OSTEST_LOOPS; i++)
#elif CONFIG_TESTING_OSTEST_LOOPS == 0
  for (; ; )
#endif
    {
#ifndef CONFIG_STDIO_DISABLE_BUFFERING
      /* Checkout setvbuf */

      printf("\nuser_main: setvbuf test\n");
      setvbuf_test();
      check_test_memory_usage();
#endif

      /* Checkout /dev/null */

#ifdef CONFIG_DEV_NULL
      printf("\nuser_main: /dev/null test\n");
      dev_null_test();
      check_test_memory_usage();
#endif

#ifdef CONFIG_TESTING_OSTEST_AIO
      /* Check asynchronous I/O */

      printf("\nuser_main: AIO test\n");
      aio_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_ARCH_FPU) && !defined(CONFIG_TESTING_OSTEST_FPUTESTDISABLE) && \
    defined(CONFIG_BUILD_FLAT)
      /* Check that the FPU is properly supported during context switching */

      printf("\nuser_main: FPU test\n");
      fpu_test();
      check_test_memory_usage();
#endif

#ifndef CONFIG_BUILD_KERNEL
      /* Checkout task_restart() */

      printf("\nuser_main: task_restart test\n");
      restart_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_KERNEL)
      /* Check waitpid() and friends */

      printf("\nuser_main: waitpid test\n");
      waitpid_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_TESTING_OSTEST_MULTIUSER) && defined(CONFIG_SCHED_USER_IDENTITY)
      /* Multi-user identity and file permission regression tests */

      printf("\nuser_main: multi-user test\n");
      if (multiuser_test() != 0)
        {
          printf("user_main: ERROR multi-user test failed\n");
          ASSERT(false);
        }

      check_test_memory_usage();
#endif

#if !defined(CONFIG_DISABLE_PTHREAD) && defined(CONFIG_BUILD_FLAT) && \
    defined(CONFIG_SCHED_WORKQUEUE)
      /* Check work queues */

      printf("\nuser_main: wqueue test\n");
      wqueue_test();
      check_test_memory_usage();
#endif

#ifndef CONFIG_DISABLE_PTHREAD
      /* Verify pthreads and pthread mutex */

      printf("\nuser_main: mutex test\n");
      mutex_test();
      check_test_memory_usage();

      printf("\nuser_main: timed mutex test\n");
      timedmutex_test();
      check_test_memory_usage();
#endif

#if !defined(CONFIG_DISABLE_PTHREAD) && defined(CONFIG_PTHREAD_MUTEX_TYPES)
      /* Verify recursive mutexes */

      printf("\nuser_main: recursive mutex test\n");
      recursive_mutex_test();
      check_test_memory_usage();
#endif

#if !defined(CONFIG_DISABLE_PTHREAD) && CONFIG_TLS_NELEM > 0
      /* Verify pthread-specific data */

      printf("\nuser_main: pthread-specific data test\n");
      specific_test();
      check_test_memory_usage();
#endif

#ifndef CONFIG_DISABLE_PTHREAD
      /* Verify pthread cancellation */

      printf("\nuser_main: cancel test\n");
      cancel_test();
      check_test_memory_usage();

#ifndef CONFIG_PTHREAD_MUTEX_UNSAFE
      printf("\nuser_main: robust test\n");
      robust_test();
      check_test_memory_usage();
#endif
#endif

#ifndef CONFIG_DISABLE_PTHREAD
      /* Verify pthreads and semaphores */

      printf("\nuser_main: semaphore test\n");
      sem_test();
      check_test_memory_usage();

      printf("\nuser_main: timed semaphore test\n");
      semtimed_test();
      check_test_memory_usage();

#ifdef CONFIG_FS_NAMED_SEMAPHORES
      printf("\nuser_main: Named semaphore test\n");
      nsem_test();
      check_test_memory_usage();

#endif
#endif

#ifndef CONFIG_DISABLE_PTHREAD
      /* Verify pthreads and condition variables */

      printf("\nuser_main: condition variable test\n");
#ifdef CONFIG_PRIORITY_INHERITANCE
      printf("\n           Skipping, "
             "Test logic incompatible with priority inheritance\n");
#else
      cond_test();
      check_test_memory_usage();
#endif

#ifdef CONFIG_SCHED_WAITPID
      /* Verify pthread_exit() and pthread_self() */

      printf("\nuser_main: pthread_exit() test\n");
      pthread_exit_test();
      check_test_memory_usage();
#endif

      /* Verify pthreads rwlock interfaces */

      printf("\nuser_main: pthread_rwlock test\n");
      pthread_rwlock_test();
      check_test_memory_usage();

      printf("\nuser_main: pthread_rwlock_cancel test\n");
      pthread_rwlock_cancel_test();
      check_test_memory_usage();

#if CONFIG_TLS_NCLEANUP > 0
      /* Verify pthread cancellation cleanup handlers */

      printf("\nuser_main: pthread_cleanup test\n");
      pthread_cleanup_test();
      check_test_memory_usage();
#endif

      /* Verify pthreads and condition variable timed waits */

      printf("\nuser_main: timed wait test\n");
      timedwait_test();
      check_test_memory_usage();
#endif /* !CONFIG_DISABLE_PTHREAD */

#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_PTHREAD)
      /* Verify pthreads and message queues */

      printf("\nuser_main: timed message queue test\n");
      timedmqueue_test();
      check_test_memory_usage();
#endif

#ifndef CONFIG_DISABLE_ALL_SIGNALS
      /* Verify that we can modify the signal mask */

      printf("\nuser_main: sigprocmask test\n");
      sigprocmask_test();
      check_test_memory_usage();

#if !defined(CONFIG_DISABLE_MQUEUE) && !defined(CONFIG_DISABLE_PTHREAD)
      /* Verify pthreads and message queues */

      printf("\nuser_main: message queue test\n");
      mqueue_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_SIG_SIGSTOP_ACTION) && defined(CONFIG_SIG_SIGKILL_ACTION) && \
    !defined(CONFIG_BUILD_KERNEL)
      printf("\nuser_main: signal action test\n");
      suspend_test();
      check_test_memory_usage();
#endif
#endif /* !CONFIG_DISABLE_ALL_SIGNALS */

#ifdef CONFIG_ENABLE_ALL_SIGNALS
      /* Verify signal handlers */

      printf("\nuser_main: signal handler test\n");
      sighand_test();
      check_test_memory_usage();

      printf("\nuser_main: nested signal handler test\n");
      signest_test();
      check_test_memory_usage();

#ifndef CONFIG_DISABLE_POSIX_TIMERS
      /* Verify posix timers (with SIGEV_SIGNAL) */

      printf("\nuser_main: POSIX timer test\n");
      timer_test();
      check_test_memory_usage();
#endif
#endif

#ifdef CONFIG_BUILD_FLAT
      printf("\nuser_main: spinlock test\n");
      spinlock_test();
      check_test_memory_usage();

      printf("\nuser_main: wdog test\n");
      wdog_test();
      check_test_memory_usage();

      /* Verify hrtimer */

#  ifdef CONFIG_HRTIMER
      printf("\nuser_main: hrtimer test\n");
      hrtimer_test();
      check_test_memory_usage();
#  endif
#endif

#if !defined(CONFIG_DISABLE_POSIX_TIMERS) && \
    defined(CONFIG_SIG_EVTHREAD)
      /* Verify posix timers (with SIGEV_THREAD) */

      printf("\nuser_main: SIGEV_THREAD timer test\n");
      sigev_thread_test();
      check_test_memory_usage();
#endif

#if !defined(CONFIG_DISABLE_PTHREAD) && CONFIG_RR_INTERVAL > 0
      /* Verify round robin scheduling */

      printf("\nuser_main: round-robin scheduler test\n");
      rr_test();
      check_test_memory_usage();
#endif

#if !defined(CONFIG_DISABLE_PTHREAD) && defined(CONFIG_SCHED_SPORADIC)
      /* Verify sporadic scheduling */

      printf("\nuser_main: sporadic scheduler test\n");
      sporadic_test();
      check_test_memory_usage();

      printf("\nuser_main: Dual sporadic thread test\n");
      sporadic2_test();
      check_test_memory_usage();
#endif

#ifndef CONFIG_DISABLE_PTHREAD
      /* Verify pthread barriers */

      printf("\nuser_main: barrier test\n");
      barrier_test();
      check_test_memory_usage();
#endif

#ifdef CONFIG_ARCH_SETJMP_H
      /* Verify setjmp/longjmp */

      printf("\nuser_main: setjmp test\n");
      setjmp_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_PRIORITY_INHERITANCE) && !defined(CONFIG_DISABLE_PTHREAD)
      /* Verify priority inheritance */

      printf("\nuser_main: priority inheritance test\n");
      priority_inheritance();
      check_test_memory_usage();
#endif /* CONFIG_PRIORITY_INHERITANCE && !CONFIG_DISABLE_PTHREAD */

#ifndef CONFIG_DISABLE_PTHREAD
      printf("\nuser_main: scheduler lock test\n");
      sched_lock_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_SMP) && defined(CONFIG_BUILD_FLAT)
      printf("\nuser_main: smp call test\n");
      smp_call_test();
#endif

#if defined(CONFIG_SCHED_EVENTS) && defined(CONFIG_BUILD_FLAT)
      /* Verify nxevent */

      printf("\nuser_main: nxevent test\n");
      nxevent_test();
      check_test_memory_usage();
#endif

#if defined(CONFIG_ARCH_PERF_EVENTS) && \
    !defined(CONFIG_ARCH_PERF_EVENTS_USER_ACCESS) && defined(CONFIG_BUILD_FLAT)
      /* Verify performance event time counter */

      printf("\nuser_main: performance event time counter test\n");
      perf_gettime_test();
      check_test_memory_usage();
#endif

      /* Compare memory usage at time ostest_main started until
       * user_main exits.  These should not be identical, but should
       * be similar enough that we can detect any serious OS memory
       * leaks.
       */

      usleep(HALF_SECOND_USEC);

      g_mmafter = mallinfo();

      printf("\nFinal memory usage:\n");
      show_memory_usage(&g_mmbefore, &g_mmafter);
    }

  printf("user_main: Exiting\n");
  return 0;
}

/****************************************************************************
 * Name: stdio_test
 ****************************************************************************/

static void stdio_test(void)
{
  /* Verify that we can communicate */

  write(1, write_data1, sizeof(write_data1)-1);
  printf("stdio_test: Standard I/O Check: printf\n");

  write(2, write_data2, sizeof(write_data2)-1);
#ifdef CONFIG_FILE_STREAM
  fprintf(stderr, "stdio_test: Standard I/O Check: fprintf to stderr\n");
#endif
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * ostest_main
 ****************************************************************************/

int main(int argc, FAR char **argv)
{
#ifdef CONFIG_TESTING_OSTEST_WAITRESULT
  int ostest_result = ERROR;
#else
  int ostest_result = OK;
#endif

#ifndef CONFIG_BUILD_KERNEL
  int result;
#else
  struct sched_param param;
  posix_spawnattr_t attr;
  FAR const char * const arg[7] =
  {
    argv[0], "user_main", arg1, arg2, arg3, arg4, NULL
  };

  pid_t result;
  int status;

  /* Use posix_spawn API to spawn the new task for tests, should re-enter
   * the main entry in new task, the second arg as to identify the real
   * entry point of the test tasks.
   */

  if (argc >= 2 && strcmp(argv[1], "user_main") == 0)
    {
      return user_main(argc - 1 , &argv[1]);
    }
#endif

  /* Verify that stdio works first */

  stdio_test();

#ifdef SDCC
  /* I am not yet certain why SDCC does not like the following initializers.
   * It involves some issues with 2- vs 3-byte pointer types.
   */

  g_argv[0] = arg1;
  g_argv[1] = arg2;
  g_argv[2] = arg3;
  g_argv[3] = arg4;
  g_argv[4] = NULL;
#endif

  /* Set up some environment variables */

#ifndef CONFIG_DISABLE_ENVIRON
  printf("ostest_main: putenv(%s)\n", g_putenv_value);
  putenv(g_putenv_value);                   /* Varaible1=BadValue3 */
  printf("ostest_main: setenv(%s, %s, TRUE)\n", g_var1_name, g_var1_value);
  setenv(g_var1_name, g_var1_value, TRUE);  /* Variable1=GoodValue1 */

  printf("ostest_main: setenv(%s, %s, FALSE)\n", g_var2_name, g_bad_value1);
  setenv(g_var2_name, g_bad_value1, FALSE); /* Variable2=BadValue1 */
  printf("ostest_main: setenv(%s, %s, TRUE)\n", g_var2_name, g_var2_value);
  setenv(g_var2_name, g_var2_value, TRUE);  /* Variable2=GoodValue2 */

  printf("ostest_main: setenv(%s, %s, FALSE)\n", g_var3_name, g_var3_value);
  setenv(g_var3_name, g_var3_value, FALSE); /* Variable3=GoodValue3 */
  printf("ostest_main: setenv(%s, %s, FALSE)\n", g_var3_name, g_bad_value2);
  setenv(g_var3_name, g_bad_value2, FALSE); /* Variable3=GoodValue3 */
  show_environment(true, true, true);
#endif

  /* Verify that we can spawn a new task */

#ifdef CONFIG_BUILD_KERNEL
  posix_spawnattr_init(&attr);
  param.sched_priority = PRIORITY;
  posix_spawnattr_setschedparam(&attr, &param);
  posix_spawnattr_setstacksize(&attr, STACKSIZE);
  status = posix_spawn(&result, arg[0], NULL, &attr,
                       (char * const *)arg, NULL);
  if (status != 0)
#else
  result = task_create("ostest", PRIORITY, STACKSIZE, user_main,
                       (FAR char * const *)g_argv);
  if (result == ERROR)
#endif
    {
      printf("ostest_main: ERROR Failed to start user_main\n");
      ASSERT(false);
      ostest_result = ERROR;
    }
  else
    {
      printf("ostest_main: Started user_main at PID=%d\n", result);

#ifdef CONFIG_TESTING_OSTEST_WAITRESULT
      /* Wait for the test to complete to get the test result */

      if (waitpid(result, &ostest_result, 0) != result)
        {
          printf("ostest_main: ERROR Failed to wait for user_main to "
                 "terminate\n");
          ASSERT(false);
          ostest_result = ERROR;
        }
#endif
    }

  printf("ostest_main: Exiting with status %d\n", ostest_result);

#ifdef CONFIG_TESTING_OSTEST_POWEROFF
  /* Power down, providing the test result.  This is really only an
   * interesting case when used with the NuttX simulator.  In that case,
   * test management logic can received the result of the test.
   */

  boardctl(BOARDIOC_POWEROFF, ostest_result);
#endif

  return ostest_result;
}
