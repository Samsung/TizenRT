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
 * apps/examples/testcase/ostest/multiuser.c
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

#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <grp.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#if defined(CONFIG_LIBC_EXECFUNCS) && defined(CONFIG_SYSTEM_SUDO)
#  include <spawn.h>
#endif

#if !defined(CONFIG_DISABLE_MQUEUE)
#  include <mqueue.h>
#endif

#if defined(CONFIG_FS_NAMED_SEMAPHORES)
#  include <semaphore.h>
#endif

#if defined(CONFIG_FS_SHMFS)
#  include <sys/mman.h>
#endif

#include "ostest.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MU_UID1   1000
#define MU_GID1   1000
#define MU_UID2   2000
#define MU_GID2   2000
#define MU_GID3   3000

#define MU_PSEUDO_PERM   "/ostest_mu_perm"
#define MU_PSEUDO_SECRET "/ostest_mu_secret"
#define MU_PSEUDO_USER   "/ostest_mu_user"

#ifndef CONFIG_LIBC_TMPDIR
#  define CONFIG_LIBC_TMPDIR "/tmp"
#endif

#define MU_TMPFS_SECRET CONFIG_LIBC_TMPDIR "/ostest_mu_secret"

#define MU_CHILD_PRIORITY 100

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct mu_ctx_s
{
  int failures;
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void mu_fail(FAR struct mu_ctx_s *ctx, FAR const char *fmt, ...)
{
  va_list ap;

  ctx->failures++;
  printf("  FAIL: ");
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
  printf("\n");
}

static void mu_pass(FAR const char *fmt, ...)
{
  va_list ap;

  printf("  PASS: ");
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
  printf("\n");
}

static void mu_check_eq(FAR struct mu_ctx_s *ctx, FAR const char *label,
                        long got, long want)
{
  if (got == want)
    {
      mu_pass("%s == %ld", label, want);
    }
  else
    {
      mu_fail(ctx, "%s: expected %ld, got %ld", label, want, got);
    }
}

static int mu_restore_root(FAR struct mu_ctx_s *ctx)
{
  if (seteuid(0) != 0)
    {
      mu_fail(ctx, "seteuid(0) restore errno=%d", errno);
      return -1;
    }

  if (setegid(0) != 0)
    {
      mu_fail(ctx, "setegid(0) restore errno=%d", errno);
      return -1;
    }

  if (geteuid() != 0 || getegid() != 0)
    {
      mu_fail(ctx, "restore root effective credentials failed "
              "(euid=%d egid=%d)", geteuid(), getegid());
      return -1;
    }

  return 0;
}

static int mu_set_effective(FAR struct mu_ctx_s *ctx, uid_t uid, gid_t gid)
{
  /* seteuid/setegid allow arbitrary IDs only while euid is 0.  With
   * real UID 0, restore euid 0 before switching, matching
   * nsh_switch_credentials().
   */

  if (getuid() == 0 && (geteuid() != 0 || getegid() != 0))
    {
      if (seteuid(0) != 0)
        {
          mu_fail(ctx, "seteuid(0) before switch errno=%d", errno);
          return -1;
        }

      if (setegid(0) != 0)
        {
          mu_fail(ctx, "setegid(0) before switch errno=%d", errno);
          return -1;
        }
    }

  if (seteuid(uid) != 0)
    {
      mu_fail(ctx, "seteuid(%u) errno=%d", uid, errno);
      return -1;
    }

  if (setegid(gid) != 0)
    {
      mu_fail(ctx, "setegid(%u) errno=%d", gid, errno);
      return -1;
    }

  if (geteuid() != uid || getegid() != gid)
    {
      mu_fail(ctx, "effective credential switch failed "
              "(have euid=%d egid=%d)", geteuid(), getegid());
      return -1;
    }

  return 0;
}

static int mu_expect_ok(FAR struct mu_ctx_s *ctx, FAR const char *op,
                        int ret)
{
  if (ret == 0)
    {
      mu_pass("%s", op);
      return 0;
    }

  mu_fail(ctx, "%s (ret=%d errno=%d)", op, ret, errno);
  return -1;
}

static int mu_expect_denied(FAR struct mu_ctx_s *ctx, FAR const char *op,
                            int ret)
{
  int err = errno;

  if (ret != 0 && (err == EPERM || err == EACCES))
    {
      mu_pass("%s denied errno=%d", op, err);
      return 0;
    }

  if (ret == 0)
    {
      mu_fail(ctx, "%s succeeded (expected EPERM/EACCES, euid=%d)",
              op, geteuid());
    }
  else
    {
      mu_fail(ctx, "%s (ret=%d errno=%d, expected EPERM/EACCES)",
              op, ret, err);
    }

  return -1;
}

static int mu_verify_owner(FAR struct mu_ctx_s *ctx, FAR const char *path,
                           uid_t uid, gid_t gid)
{
  struct stat st;

  if (stat(path, &st) != 0)
    {
      mu_fail(ctx, "stat(%s) errno=%d", path, errno);
      return -1;
    }

  if (st.st_uid != uid || st.st_gid != gid)
    {
      mu_fail(ctx, "%s owner expected %u:%u got %u:%u",
              path, uid, gid, st.st_uid, st.st_gid);
      return -1;
    }

  mu_pass("%s owner %u:%u", path, uid, gid);
  return 0;
}

static void mu_verify_mode(FAR struct mu_ctx_s *ctx, FAR const char *path,
                           mode_t mode)
{
  struct stat st;

  if (stat(path, &st) != 0)
    {
      mu_fail(ctx, "stat(%s) after create errno=%d", path, errno);
    }
  else if ((st.st_mode & 0777) != mode)
    {
      mu_fail(ctx, "%s mode expected %04o got %04o", path,
              mode, st.st_mode & 0777);
    }
  else
    {
      mu_pass("%s mode %04o", path, mode);
    }
}

#if defined(CONFIG_SCHED_USER_IDENTITY)

static int multiuser_effective_test(FAR struct mu_ctx_s *ctx)
{
  int ret;

  printf("multiuser: effective UID/GID switching\n");

  mu_check_eq(ctx, "initial uid", getuid(), 0);
  mu_check_eq(ctx, "initial euid", geteuid(), 0);
  mu_check_eq(ctx, "initial gid", getgid(), 0);
  mu_check_eq(ctx, "initial egid", getegid(), 0);

  ret = seteuid(MU_UID1);
  if (mu_expect_ok(ctx, "root seteuid(1000)", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  mu_check_eq(ctx, "euid after seteuid(1000)", geteuid(), MU_UID1);

  ret = seteuid(0);
  if (mu_expect_ok(ctx, "root seteuid(0) restore", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  mu_check_eq(ctx, "euid restored to 0", geteuid(), 0);

  ret = setegid(MU_GID2);
  if (mu_expect_ok(ctx, "root setegid(2000)", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  mu_check_eq(ctx, "egid after setegid(2000)", getegid(), MU_GID2);

  ret = setegid(0);
  if (mu_expect_ok(ctx, "root setegid(0) restore", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  mu_check_eq(ctx, "egid restored to 0", getegid(), 0);

  return ctx->failures;
}

#if CONFIG_SCHED_NGROUPS > 0
static int multiuser_groups_test(FAR struct mu_ctx_s *ctx)
{
  gid_t list[NGROUPS_MAX];
  gid_t got[NGROUPS_MAX];
  int n;
  int ret;

  printf("multiuser: setgroups/getgroups supplementary IDs\n");

  list[0] = MU_GID2;
  list[1] = (gid_t)(MU_GID2 + 1);

  ret = setgroups(2, list);
  if (mu_expect_ok(ctx, "setgroups(2)", ret) != 0)
    {
      return ctx->failures;
    }

  n = getgroups(0, NULL);
  mu_check_eq(ctx, "getgroups(0) count", n, 2);

  n = getgroups(NGROUPS_MAX, got);
  mu_check_eq(ctx, "getgroups() count", n, 2);
  if (n >= 2)
    {
      mu_check_eq(ctx, "getgroups()[0]", got[0], list[0]);
      mu_check_eq(ctx, "getgroups()[1]", got[1], list[1]);
    }

  ret = setgroups(0, NULL);
  if (mu_expect_ok(ctx, "setgroups(0) clear", ret) != 0)
    {
      return ctx->failures;
    }

  n = getgroups(0, NULL);
  mu_check_eq(ctx, "getgroups after clear", n, 0);

  /* setgroups rejects size > NGROUPS_MAX (no silent truncate). */

  ret = setgroups(NGROUPS_MAX + 1, list);
  if (ret != 0 && errno == EINVAL)
    {
      mu_pass("setgroups(NGROUPS_MAX+1) rejected errno=EINVAL");
    }
  else
    {
      mu_fail(ctx, "setgroups(NGROUPS_MAX+1): ret=%d errno=%d "
              "(expected EINVAL)", ret, errno);
    }

#if defined(CONFIG_LIBC_GROUP_FILE)
  /* getgrouplist/initgroups fail (do not truncate) when membership >
   * NGROUPS_MAX.  Temporarily replace the group file for this check.
   */

    {
      char bak[128];
      FILE *fp;
      int i;
      int need;
      gid_t gbuf[NGROUPS_MAX];
      int ng;
      int saved_errno;

      snprintf(bak, sizeof(bak), "%s.bak", CONFIG_LIBC_GROUP_FILEPATH);
      unlink(bak);
      rename(CONFIG_LIBC_GROUP_FILEPATH, bak);

      fp = fopen(CONFIG_LIBC_GROUP_FILEPATH, "w");
      if (fp == NULL)
        {
          mu_fail(ctx, "fopen(%s) for overflow test",
                  CONFIG_LIBC_GROUP_FILEPATH);
          rename(bak, CONFIG_LIBC_GROUP_FILEPATH);
          return ctx->failures;
        }

      /* Primary MU_GID1 plus NGROUPS_MAX distinct supplementary gids. */

      for (i = 0; i < NGROUPS_MAX; i++)
        {
          fprintf(fp, "g%d:*:%d:mu_overflow\n", i, (int)(MU_GID2 + i));
        }

      fclose(fp);

      ng = NGROUPS_MAX;
      need = getgrouplist("mu_overflow", MU_GID1, gbuf, &ng);
      mu_check_eq(ctx, "getgrouplist overflow returns -1", need, -1);
      mu_check_eq(ctx, "getgrouplist reports required size", ng,
                  NGROUPS_MAX + 1);

      saved_errno = 0;
      ret = initgroups("mu_overflow", MU_GID1);
      if (ret == 0)
        {
          mu_fail(ctx, "initgroups should fail when groups > NGROUPS_MAX");
        }
      else
        {
          saved_errno = errno;
          mu_pass("initgroups fails when groups > NGROUPS_MAX (errno=%d)",
                  saved_errno);
        }

      unlink(CONFIG_LIBC_GROUP_FILEPATH);
      rename(bak, CONFIG_LIBC_GROUP_FILEPATH);
    }
#endif /* CONFIG_LIBC_GROUP_FILE */

  return ctx->failures;
}
#endif /* CONFIG_SCHED_NGROUPS > 0 */

/****************************************************************************
 * Name: multiuser_setres_order_test
 *
 * Description:
 *   Regression: setresgid requires euid==0.  Dropping uid before gid fails;
 *   gid-then-uid succeeds (NSH assume_identity order).
 *
 ****************************************************************************/

static int multiuser_setres_order_test(FAR struct mu_ctx_s *ctx)
{
  uid_t ruid;
  uid_t euid;
  uid_t suid;
  gid_t rgid;
  gid_t egid;
  gid_t sgid;
  int ret;

  printf("multiuser: setresuid/setresgid drop ordering\n");

  mu_restore_root(ctx);

  /* uid-first must fail once euid is no longer 0. */

  ret = setresuid(MU_UID1, MU_UID1, 0);
  if (mu_expect_ok(ctx, "setresuid(1000,1000,0) first", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = setresgid(MU_GID1, MU_GID1, 0);
  mu_expect_denied(ctx, "setresgid after uid drop (wrong order)", ret);

  ret = seteuid(0);
  if (mu_expect_ok(ctx, "seteuid(0) via suid after bad order", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = setresuid(0, 0, 0);
  mu_expect_ok(ctx, "setresuid(0,0,0) reset", ret);
  ret = setresgid(0, 0, 0);
  mu_expect_ok(ctx, "setresgid(0,0,0) reset", ret);

  /* gid-then-uid (correct order) must succeed. */

  ret = setresgid(MU_GID1, MU_GID1, 0);
  if (mu_expect_ok(ctx, "setresgid(1000,1000,0) first", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = setresuid(MU_UID1, MU_UID1, 0);
  if (mu_expect_ok(ctx, "setresuid(1000,1000,0) second", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  getresuid(&ruid, &euid, &suid);
  getresgid(&rgid, &egid, &sgid);
  mu_check_eq(ctx, "ruid after gid-then-uid", ruid, MU_UID1);
  mu_check_eq(ctx, "euid after gid-then-uid", euid, MU_UID1);
  mu_check_eq(ctx, "suid after gid-then-uid", suid, 0);
  mu_check_eq(ctx, "rgid after gid-then-uid", rgid, MU_GID1);
  mu_check_eq(ctx, "egid after gid-then-uid", egid, MU_GID1);
  mu_check_eq(ctx, "sgid after gid-then-uid", sgid, 0);

  mu_restore_root(ctx);
  setresuid(0, 0, 0);
  setresgid(0, 0, 0);
  return ctx->failures;
}

static int multiuser_resuid_test(FAR struct mu_ctx_s *ctx)
{
  uid_t ruid;
  uid_t euid;
  uid_t suid;
  gid_t rgid;
  gid_t egid;
  gid_t sgid;
  int ret;

  printf("multiuser: getresuid/getresgid and setreuid/setregid\n");

  ret = getresuid(&ruid, &euid, &suid);
  if (mu_expect_ok(ctx, "getresuid(initial)", ret) != 0)
    {
      return ctx->failures;
    }

  mu_check_eq(ctx, "initial real uid", ruid, 0);
  mu_check_eq(ctx, "initial eff uid", euid, 0);
  mu_check_eq(ctx, "initial saved uid", suid, 0);

  ret = getresgid(&rgid, &egid, &sgid);
  if (mu_expect_ok(ctx, "getresgid(initial)", ret) != 0)
    {
      return ctx->failures;
    }

  mu_check_eq(ctx, "initial real gid", rgid, 0);
  mu_check_eq(ctx, "initial eff gid", egid, 0);
  mu_check_eq(ctx, "initial saved gid", sgid, 0);

  ret = setreuid((uid_t)-1, MU_UID1);
  if (mu_expect_ok(ctx, "root setreuid(-1,1000)", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = getresuid(&ruid, &euid, &suid);
  mu_check_eq(ctx, "real uid after setreuid(-1,1000)", ruid, 0);
  mu_check_eq(ctx, "eff uid after setreuid(-1,1000)", euid, MU_UID1);
  mu_check_eq(ctx, "saved uid after setreuid(-1,1000)", suid, MU_UID1);

  ret = setreuid(MU_UID2, (uid_t)-1);
  mu_expect_denied(ctx, "non-root setreuid(2000,-1)", ret);

  ret = setreuid((uid_t)-1, 0);
  if (mu_expect_ok(ctx, "setreuid(-1,0) drop effective", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = setreuid(0, (uid_t)-1);
  if (mu_expect_ok(ctx, "root setreuid(0,-1) restore", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = getresuid(&ruid, &euid, &suid);
  mu_check_eq(ctx, "real uid restored", ruid, 0);
  mu_check_eq(ctx, "eff uid restored", euid, 0);
  mu_check_eq(ctx, "saved uid restored", suid, 0);

  ret = setregid((gid_t)-1, MU_GID1);
  if (mu_expect_ok(ctx, "root setregid(-1,1000)", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = getresgid(&rgid, &egid, &sgid);
  mu_check_eq(ctx, "real gid after setregid(-1,1000)", rgid, 0);
  mu_check_eq(ctx, "eff gid after setregid(-1,1000)", egid, MU_GID1);
  mu_check_eq(ctx, "saved gid after setregid(-1,1000)", sgid, MU_GID1);

  ret = setregid(MU_GID2, (gid_t)-1);
  mu_expect_denied(ctx, "non-root setregid(2000,-1)", ret);

  ret = setregid((gid_t)-1, 0);
  if (mu_expect_ok(ctx, "setregid(-1,0) drop effective", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  ret = setregid(0, (gid_t)-1);
  mu_expect_ok(ctx, "root setregid(0,-1) restore", ret);

  mu_restore_root(ctx);
  return ctx->failures;
}

#if defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_KERNEL)

static int mu_run_child(FAR struct mu_ctx_s *ctx, FAR const char *name,
                        main_t entry)
{
  pid_t pid;
  int status;

  pid = task_create(name, MU_CHILD_PRIORITY, STACKSIZE, entry, NULL);
  if (pid < 0)
    {
      mu_fail(ctx, "task_create(%s) failed", name);
      return ctx->failures;
    }

  if (waitpid(pid, &status, 0) != pid)
    {
      mu_fail(ctx, "waitpid(%s) failed errno=%d", name, errno);
      return ctx->failures;
    }

  if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
    {
      mu_fail(ctx, "%s child exited with status=%d", name, status);
    }
  else
    {
      mu_pass("%s child completed successfully", name);
    }

  return ctx->failures;
}

static int multiuser_resuid_child(int argc, FAR char *argv[])
{
  struct mu_ctx_s ctx =
  {
    0
  };

  uid_t ruid;
  uid_t euid;
  uid_t suid;
  int ret;

  (void)argc;
  (void)argv;

  ret = setreuid(MU_UID1, MU_UID1);
  if (mu_expect_ok(&ctx, "root setreuid(1000,1000)", ret) != 0)
    {
      return EXIT_FAILURE;
    }

  ret = getresuid(&ruid, &euid, &suid);
  if (mu_expect_ok(&ctx, "getresuid(after setreuid)", ret) != 0)
    {
      return EXIT_FAILURE;
    }

  mu_check_eq(&ctx, "real uid after setreuid", ruid, MU_UID1);
  mu_check_eq(&ctx, "eff uid after setreuid", euid, MU_UID1);
  mu_check_eq(&ctx, "saved uid after setreuid", suid, MU_UID1);

  printf("multiuser_resuid_child: %d failure(s)\n", ctx.failures);
  return ctx.failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int multiuser_resuid_child_test(FAR struct mu_ctx_s *ctx)
{
  printf("multiuser: setreuid(1000,1000) full assignment (child task)\n");

  mu_run_child(ctx, "mu_resuid", multiuser_resuid_child);
  mu_check_eq(ctx, "parent euid after resuid child", geteuid(), 0);
  mu_check_eq(ctx, "parent egid after resuid child", getegid(), 0);

  return ctx->failures;
}

static int multiuser_suid_child(int argc, FAR char *argv[])
{
  struct mu_ctx_s ctx =
  {
    0
  };

  int ret;

  (void)argc;
  (void)argv;

  printf("multiuser_suid_child: saved set-UID/GID semantics\n");

  ret = setuid(MU_UID1);
  if (mu_expect_ok(&ctx, "setuid(1000) as root", ret) != 0)
    {
      return EXIT_FAILURE;
    }

  mu_check_eq(&ctx, "uid after setuid(1000)", getuid(), MU_UID1);
  mu_check_eq(&ctx, "euid after setuid(1000)", geteuid(), MU_UID1);

  ret = seteuid(0);
  mu_expect_denied(&ctx, "non-root seteuid(0)", ret);
  mu_check_eq(&ctx, "euid unchanged after denied seteuid(0)", geteuid(),
              MU_UID1);

  ret = seteuid(MU_UID1);
  mu_expect_ok(&ctx, "non-root seteuid(1000)", ret);

  ret = setegid(MU_GID2);
  mu_expect_ok(&ctx, "root-group setegid(2000)", ret);
  mu_check_eq(&ctx, "egid after setegid(2000)", getegid(), MU_GID2);

  ret = setegid(0);
  mu_expect_ok(&ctx, "root-group setegid(0) restore", ret);

  ret = setgid(MU_GID3);
  mu_expect_ok(&ctx, "setgid(3000)", ret);
  mu_check_eq(&ctx, "gid after setgid(3000)", getgid(), MU_GID3);
  mu_check_eq(&ctx, "egid after setgid(3000)", getegid(), MU_GID3);

  ret = setegid(0);
  mu_expect_denied(&ctx, "non-root setegid(0)", ret);
  mu_check_eq(&ctx, "egid unchanged after denied setegid(0)", getegid(),
              MU_GID3);

  printf("multiuser_suid_child: %d failure(s)\n", ctx.failures);
  return ctx.failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int multiuser_suid_test(FAR struct mu_ctx_s *ctx)
{
  printf("multiuser: saved set-UID/GID semantics (child task)\n");

  mu_run_child(ctx, "mu_suid", multiuser_suid_child);
  mu_check_eq(ctx, "parent euid after child", geteuid(), 0);
  mu_check_eq(ctx, "parent egid after child", getegid(), 0);

  return ctx->failures;
}

#endif /* CONFIG_SCHED_WAITPID && !CONFIG_BUILD_KERNEL */

#if defined(CONFIG_FS_PERMISSION)

static int multiuser_open_secret(FAR struct mu_ctx_s *ctx,
                                 FAR const char *path, int expect_ok)
{
  int fd;

  fd = open(path, O_RDONLY);
  if (expect_ok)
    {
      if (fd < 0)
        {
          mu_fail(ctx, "open(%s) errno=%d (expected success)", path, errno);
          return -1;
        }

      close(fd);
      mu_pass("open(%s) allowed", path);
      return 0;
    }

  if (fd >= 0)
    {
      close(fd);
      mu_fail(ctx, "open(%s) succeeded (expected EACCES, euid=%d)",
              path, geteuid());
      return -1;
    }

  if (errno != EACCES)
    {
      mu_fail(ctx, "open(%s) errno=%d (expected EACCES)", path, errno);
      return -1;
    }

  mu_pass("open(%s) denied with EACCES", path);
  return 0;
}

#endif /* CONFIG_FS_PERMISSION */

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_PSEUDOFS_ATTRIBUTES) && \
    defined(CONFIG_PSEUDOFS_FILE)

static int multiuser_pseudofs_test(FAR struct mu_ctx_s *ctx)
{
  int fd;
  int ret;

  printf("multiuser: pseudoFS chmod/chown/open permissions\n");

  unlink(MU_PSEUDO_PERM);
  unlink(MU_PSEUDO_SECRET);
  unlink(MU_PSEUDO_USER);

  fd = open(MU_PSEUDO_PERM, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (fd < 0)
    {
      mu_fail(ctx, "create %s errno=%d", MU_PSEUDO_PERM, errno);
      goto out;
    }

  close(fd);
  mu_verify_owner(ctx, MU_PSEUDO_PERM, 0, 0);

  ret = chmod(MU_PSEUDO_PERM, 0600);
  mu_expect_ok(ctx, "root chmod(0600)", ret);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  ret = chmod(MU_PSEUDO_PERM, 0777);
  mu_expect_denied(ctx, "non-owner chmod(0777)", ret);

  ret = chown(MU_PSEUDO_PERM, 0, 0);
  mu_expect_denied(ctx, "non-root chown(0,0)", ret);

  if (mu_restore_root(ctx) != 0)
    {
      goto out;
    }

  ret = chown(MU_PSEUDO_PERM, MU_UID1, MU_GID1);
  mu_expect_ok(ctx, "root chown to 1000:1000", ret);
  mu_verify_owner(ctx, MU_PSEUDO_PERM, MU_UID1, MU_GID1);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  ret = chmod(MU_PSEUDO_PERM, 0777);
  mu_expect_ok(ctx, "owner chmod(0777)", ret);

  if (mu_restore_root(ctx) != 0)
    {
      goto out;
    }

  fd = open(MU_PSEUDO_SECRET, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd < 0)
    {
      mu_fail(ctx, "create %s errno=%d", MU_PSEUDO_SECRET, errno);
      goto out;
    }

  close(fd);
  mu_verify_owner(ctx, MU_PSEUDO_SECRET, 0, 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  multiuser_open_secret(ctx, MU_PSEUDO_SECRET, 0);

  if (mu_restore_root(ctx) != 0)
    {
      goto out;
    }

  multiuser_open_secret(ctx, MU_PSEUDO_SECRET, 1);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  fd = open(MU_PSEUDO_USER, O_CREAT | O_WRONLY, 0644);
  if (fd < 0)
    {
      mu_fail(ctx, "create %s as euid=%d errno=%d",
              MU_PSEUDO_USER, geteuid(), errno);
      goto out;
    }

  close(fd);
  mu_verify_owner(ctx, MU_PSEUDO_USER, MU_UID1, MU_GID1);

  if (mu_set_effective(ctx, MU_UID2, MU_GID2) != 0)
    {
      goto out;
    }

  multiuser_open_secret(ctx, MU_PSEUDO_USER, 1);

out:
  mu_restore_root(ctx);
  unlink(MU_PSEUDO_PERM);
  unlink(MU_PSEUDO_SECRET);
  unlink(MU_PSEUDO_USER);

  return ctx->failures;
}

#endif /* FS_PERMISSION && PSEUDOFS_ATTRIBUTES && PSEUDOFS_FILE */

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_FS_TMPFS)

static int multiuser_tmpfs_test(FAR struct mu_ctx_s *ctx)
{
  int fd;

  printf("multiuser: tmpfs open permission enforcement\n");

  unlink(MU_TMPFS_SECRET);

  fd = open(MU_TMPFS_SECRET, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fd < 0)
    {
      mu_fail(ctx, "create %s errno=%d", MU_TMPFS_SECRET, errno);
      goto out;
    }

  close(fd);
  mu_verify_owner(ctx, MU_TMPFS_SECRET, 0, 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  multiuser_open_secret(ctx, MU_TMPFS_SECRET, 0);

  if (mu_restore_root(ctx) != 0)
    {
      goto out;
    }

  multiuser_open_secret(ctx, MU_TMPFS_SECRET, 1);

out:
  mu_restore_root(ctx);
  unlink(MU_TMPFS_SECRET);

  return ctx->failures;
}

#endif /* CONFIG_FS_PERMISSION && CONFIG_FS_TMPFS */

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_PSEUDOFS_ATTRIBUTES)

#if !defined(CONFIG_DISABLE_MQUEUE)
#  define MU_MQ_NAME   "ostest_mu_mq"
#  define MU_MQ_PATH   CONFIG_FS_MQUEUE_VFS_PATH "/" MU_MQ_NAME
#endif

#if defined(CONFIG_FS_NAMED_SEMAPHORES)
#  define MU_SEM_NAME  "ostest_mu_sem"
#  define MU_SEM_PATH  CONFIG_FS_NAMED_SEMAPHORES_VFS_PATH "/" MU_SEM_NAME
#endif

#if defined(CONFIG_FS_SHMFS)
#  define MU_SHM_NAME  "ostest_mu_shm"
#  define MU_SHM_PATH  CONFIG_FS_SHMFS_VFS_PATH "/" MU_SHM_NAME
#endif

#if defined(CONFIG_PIPES)
#  define MU_FIFO_PATH "/ostest_mu_fifo"
#endif

#if !defined(CONFIG_DISABLE_MQUEUE)

static int multiuser_mqueue_test(FAR struct mu_ctx_s *ctx)
{
  mqd_t mq;
  struct mq_attr attr;

  printf("multiuser: message queue ownership and permissions\n");

  mu_restore_root(ctx);
  mq_unlink(MU_MQ_NAME);

  memset(&attr, 0, sizeof(attr));
  attr.mq_maxmsg  = 4;
  attr.mq_msgsize = CONFIG_MQ_MAXMSGSIZE;

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  mq = mq_open(MU_MQ_NAME, O_CREAT | O_RDWR | O_EXCL, 0600, &attr);
  if (mq == (mqd_t)-1)
    {
      mu_fail(ctx, "mq_open(create) errno=%d", errno);
      goto out;
    }

  mq_close(mq);
  mu_verify_owner(ctx, MU_MQ_PATH, MU_UID1, MU_GID1);
  mu_verify_mode(ctx, MU_MQ_PATH, 0600);

  if (mu_set_effective(ctx, MU_UID2, MU_GID2) != 0)
    {
      goto out;
    }

  mq = mq_open(MU_MQ_NAME, O_RDWR);
  mu_expect_denied(ctx, "mq_open(other user)", mq == (mqd_t)-1 ? -1 : 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  mq = mq_open(MU_MQ_NAME, O_RDWR);
  if (mq == (mqd_t)-1)
    {
      mu_fail(ctx, "mq_open(owner) errno=%d", errno);
    }
  else
    {
      mu_pass("mq_open(owner)");
      mq_close(mq);
    }

out:
  mu_restore_root(ctx);
  mq_unlink(MU_MQ_NAME);
  return ctx->failures;
}

#endif /* !CONFIG_DISABLE_MQUEUE */

#if defined(CONFIG_FS_NAMED_SEMAPHORES)

static int multiuser_sem_test(FAR struct mu_ctx_s *ctx)
{
  sem_t *sem;

  printf("multiuser: named semaphore ownership and permissions\n");

  mu_restore_root(ctx);
  sem_unlink(MU_SEM_NAME);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  sem = sem_open(MU_SEM_NAME, O_CREAT | O_EXCL, 0600, 1);
  if (sem == SEM_FAILED)
    {
      mu_fail(ctx, "sem_open(create) errno=%d", errno);
      goto out;
    }

  sem_close(sem);
  mu_verify_owner(ctx, MU_SEM_PATH, MU_UID1, MU_GID1);

  if (mu_set_effective(ctx, MU_UID2, MU_GID2) != 0)
    {
      goto out;
    }

  sem = sem_open(MU_SEM_NAME, 0);
  mu_expect_denied(ctx, "sem_open(other user)", sem == SEM_FAILED ? -1 : 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  sem = sem_open(MU_SEM_NAME, 0);
  if (sem == SEM_FAILED)
    {
      mu_fail(ctx, "sem_open(owner) errno=%d", errno);
    }
  else
    {
      mu_pass("sem_open(owner)");
      sem_close(sem);
    }

out:
  mu_restore_root(ctx);
  sem_unlink(MU_SEM_NAME);
  return ctx->failures;
}

#endif /* CONFIG_FS_NAMED_SEMAPHORES */

#if defined(CONFIG_FS_SHMFS)

static int multiuser_shm_test(FAR struct mu_ctx_s *ctx)
{
  int fd;

  printf("multiuser: shared memory ownership and permissions\n");

  mu_restore_root(ctx);
  shm_unlink(MU_SHM_NAME);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  fd = shm_open(MU_SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd < 0)
    {
      mu_fail(ctx, "shm_open(create) errno=%d", errno);
      goto out;
    }

  close(fd);
  mu_verify_owner(ctx, MU_SHM_PATH, MU_UID1, MU_GID1);

  if (mu_set_effective(ctx, MU_UID2, MU_GID2) != 0)
    {
      goto out;
    }

  fd = shm_open(MU_SHM_NAME, O_RDWR, 0);
  mu_expect_denied(ctx, "shm_open(other user)", fd < 0 ? -1 : 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  fd = shm_open(MU_SHM_NAME, O_RDWR, 0);
  if (fd < 0)
    {
      mu_fail(ctx, "shm_open(owner) errno=%d", errno);
    }
  else
    {
      mu_pass("shm_open(owner)");
      close(fd);
    }

out:
  mu_restore_root(ctx);
  shm_unlink(MU_SHM_NAME);
  return ctx->failures;
}

#endif /* CONFIG_FS_SHMFS */

#if defined(CONFIG_PIPES)

static int multiuser_fifo_test(FAR struct mu_ctx_s *ctx)
{
  int fd;

  printf("multiuser: FIFO ownership and permissions\n");

  mu_restore_root(ctx);
  unlink(MU_FIFO_PATH);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  if (mkfifo(MU_FIFO_PATH, 0600) != 0)
    {
      mu_fail(ctx, "mkfifo errno=%d", errno);
      goto out;
    }

  mu_verify_owner(ctx, MU_FIFO_PATH, MU_UID1, MU_GID1);

  if (mu_set_effective(ctx, MU_UID2, MU_GID2) != 0)
    {
      goto out;
    }

  fd = open(MU_FIFO_PATH, O_RDONLY | O_NONBLOCK);
  mu_expect_denied(ctx, "open fifo (other user)", fd < 0 ? -1 : 0);

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      goto out;
    }

  fd = open(MU_FIFO_PATH, O_RDONLY | O_NONBLOCK);
  if (fd < 0)
    {
      mu_fail(ctx, "open fifo (owner) errno=%d", errno);
    }
  else
    {
      mu_pass("open fifo (owner)");
      close(fd);
    }

out:
  mu_restore_root(ctx);
  unlink(MU_FIFO_PATH);
  return ctx->failures;
}

#endif /* CONFIG_PIPES */

static int multiuser_ipc_test(FAR struct mu_ctx_s *ctx)
{
#if !defined(CONFIG_DISABLE_MQUEUE)
  multiuser_mqueue_test(ctx);
#endif

#if defined(CONFIG_FS_NAMED_SEMAPHORES)
  multiuser_sem_test(ctx);
#endif

#if defined(CONFIG_FS_SHMFS)
  multiuser_shm_test(ctx);
#endif

#if defined(CONFIG_PIPES)
  multiuser_fifo_test(ctx);
#endif

  return ctx->failures;
}

#endif /* CONFIG_FS_PERMISSION && CONFIG_PSEUDOFS_ATTRIBUTES */

#if defined(CONFIG_LIBC_PASSWD_FILE) && defined(CONFIG_TESTING_OSTEST_MULTIUSER)

static int multiuser_passwd_install(FAR struct mu_ctx_s *ctx)
{
  int fd;
  ssize_t nwritten;
  static const char content[] =
    "root:x:0:0:/\n"
    "testuser:x:1000:1000:/home/testuser\n";

  fd = open(CONFIG_LIBC_PASSWD_FILEPATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    {
      mu_fail(ctx, "create %s errno=%d", CONFIG_LIBC_PASSWD_FILEPATH, errno);
      return -1;
    }

  nwritten = write(fd, content, sizeof(content) - 1);
  close(fd);
  if (nwritten != (ssize_t)(sizeof(content) - 1))
    {
      mu_fail(ctx, "write %s errno=%d", CONFIG_LIBC_PASSWD_FILEPATH, errno);
      unlink(CONFIG_LIBC_PASSWD_FILEPATH);
      return -1;
    }

  mu_pass("installed %s", CONFIG_LIBC_PASSWD_FILEPATH);
  return 0;
}

static void multiuser_passwd_remove(void)
{
  unlink(CONFIG_LIBC_PASSWD_FILEPATH);
}

static int multiuser_passwd_test(FAR struct mu_ctx_s *ctx)
{
  FAR struct passwd *pwd;

  printf("multiuser: passwd lookup after credential drop\n");

  if (multiuser_passwd_install(ctx) != 0)
    {
      return ctx->failures;
    }

  if (mu_set_effective(ctx, MU_UID1, MU_GID1) != 0)
    {
      multiuser_passwd_remove();
      return ctx->failures;
    }

  pwd = getpwnam("root");
  if (pwd == NULL)
    {
      mu_fail(ctx, "getpwnam(root) failed errno=%d after seteuid(%d)",
              errno, MU_UID1);
    }
  else
    {
      mu_pass("getpwnam(root) uid=%u", pwd->pw_uid);
    }

  pwd = getpwnam("testuser");
  if (pwd == NULL)
    {
      mu_fail(ctx, "getpwnam(testuser) failed errno=%d", errno);
    }
  else if (pwd->pw_uid != MU_UID1)
    {
      mu_fail(ctx, "getpwnam(testuser) uid=%u (expected %u)",
              pwd->pw_uid, MU_UID1);
    }
  else
    {
      mu_pass("getpwnam(testuser) uid=%u", pwd->pw_uid);
    }

  mu_restore_root(ctx);
  multiuser_passwd_remove();
  return ctx->failures;
}

#endif /* CONFIG_LIBC_PASSWD_FILE && CONFIG_TESTING_OSTEST_MULTIUSER */

#if defined(CONFIG_LIBC_EXECFUNCS) && defined(CONFIG_SYSTEM_SUDO) && \
    defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_KERNEL)

static int multiuser_sudo_setuid_test(FAR struct mu_ctx_s *ctx)
{
  posix_spawnattr_t attr;
  FAR char * const spawn_argv[] =
    {
      (FAR char *)"sudo", (FAR char *)"--probe", NULL
    };

  pid_t pid;
  int status;
  int ret;

  printf("multiuser: setuid sudo exec after hard credential drop\n");

  mu_restore_root(ctx);
  ret = setuid(MU_UID1);
  if (mu_expect_ok(ctx, "setuid(1000) before sudo exec", ret) != 0)
    {
      return ctx->failures;
    }

  mu_check_eq(ctx, "parent euid before sudo", geteuid(), MU_UID1);

  ret = posix_spawnattr_init(&attr);
  if (mu_expect_ok(ctx, "posix_spawnattr_init", ret) != 0)
    {
      return ctx->failures;
    }

  ret = posix_spawn(&pid, "sudo", NULL, &attr, spawn_argv, NULL);
  posix_spawnattr_destroy(&attr);
  if (mu_expect_ok(ctx, "posix_spawn(sudo --probe)", ret) != 0)
    {
      mu_restore_root(ctx);
      return ctx->failures;
    }

  if (waitpid(pid, &status, 0) != pid)
    {
      mu_fail(ctx, "waitpid(sudo) errno=%d", errno);
      mu_restore_root(ctx);
      return ctx->failures;
    }

  if (!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)
    {
      mu_fail(ctx, "sudo --probe status=%d", status);
    }
  else
    {
      mu_pass("sudo --probe after hard drop");
    }

  mu_check_eq(ctx, "parent euid after sudo", geteuid(), MU_UID1);
  mu_restore_root(ctx);
  return ctx->failures;
}

#endif /* CONFIG_LIBC_EXECFUNCS && CONFIG_SYSTEM_SUDO && CONFIG_SCHED_WAITPID */

#endif /* CONFIG_SCHED_USER_IDENTITY */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int multiuser_test(void)
{
  struct mu_ctx_s ctx =
  {
    0
  };

#if defined(CONFIG_SCHED_USER_IDENTITY)

  printf("multiuser_test: start\n");

  multiuser_effective_test(&ctx);
#if CONFIG_SCHED_NGROUPS > 0
  multiuser_groups_test(&ctx);
#else
  printf("multiuser: skipping supplementary groups test "
         "(need CONFIG_SCHED_NGROUPS > 0)\n");
#endif
  multiuser_setres_order_test(&ctx);
  multiuser_resuid_test(&ctx);

#if defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_KERNEL)
  multiuser_resuid_child_test(&ctx);
  multiuser_suid_test(&ctx);
#else
  printf("multiuser: skipping saved set-UID/GID child test "
         "(need CONFIG_SCHED_WAITPID)\n");
#endif

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_PSEUDOFS_ATTRIBUTES) && \
    defined(CONFIG_PSEUDOFS_FILE)
  multiuser_pseudofs_test(&ctx);
#else
  printf("multiuser: skipping pseudoFS permission tests "
         "(need FS_PERMISSION, PSEUDOFS_ATTRIBUTES, PSEUDOFS_FILE)\n");
#endif

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_FS_TMPFS)
  multiuser_tmpfs_test(&ctx);
#else
  printf("multiuser: skipping tmpfs permission tests "
         "(need FS_PERMISSION and FS_TMPFS)\n");
#endif

#if defined(CONFIG_FS_PERMISSION) && defined(CONFIG_PSEUDOFS_ATTRIBUTES)
  multiuser_ipc_test(&ctx);
#else
  printf("multiuser: skipping IPC/FIFO permission tests "
         "(need FS_PERMISSION and PSEUDOFS_ATTRIBUTES)\n");
#endif

#if defined(CONFIG_LIBC_PASSWD_FILE) && defined(CONFIG_TESTING_OSTEST_MULTIUSER)
  multiuser_passwd_test(&ctx);
#else
  printf("multiuser: skipping passwd lookup test "
         "(need LIBC_PASSWD_FILE and TESTING_OSTEST_MULTIUSER)\n");
#endif

#if defined(CONFIG_LIBC_EXECFUNCS) && defined(CONFIG_SYSTEM_SUDO) && \
    defined(CONFIG_SCHED_WAITPID) && !defined(CONFIG_BUILD_KERNEL)
  multiuser_sudo_setuid_test(&ctx);
#else
  printf("multiuser: skipping setuid sudo exec test "
         "(need LIBC_EXECFUNCS, SYSTEM_SUDO, SCHED_WAITPID)\n");
#endif

  mu_restore_root(&ctx);

  printf("multiuser_test: %d failure(s)\n", ctx.failures);

#else

  printf("multiuser_test: CONFIG_SCHED_USER_IDENTITY disabled\n");

#endif

  return ctx.failures;
}
