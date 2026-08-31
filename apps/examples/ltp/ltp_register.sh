#!/bin/sh
###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
# express or implied. See the License for the specific language
# governing permissions and limitations under the License.
#
###########################################################################
############################################################################
# apps/examples/ltp/ltp_register.sh

#
# Discovers LTP test files containing main(), filters out blacklisted
# tests, and generates:
#
#   1. ltp_test_registry.h  - test table used by ltp_runner.c (ltprun)
#   2. ltp_runner_main.mdat/.pdat - single TASH command for "ltprun"
#
# Individual per-test TASH commands are NOT registered. All tests are
# run via the single "ltprun" TASH command.
#
# This script is called from the Makefile during the context phase,
# after the LTP source has been extracted and patched.
#
############################################################################


set -e

TEST_SUBDIR="${LTP_TEST_SUBDIR}"
REGISTRY="${BUILTIN_REGISTRY}"
STACKSIZE="${STACKSIZE:-8192}"
PRIORITY="${PRIORITY:-100}"
EXECTYPE="${THREADEXEC:-TASH_EXECMD_ASYNC}"

# LTP_TEST_SUBDIR may contain multiple space-separated directories.
# Verify at least one exists.
found_dir=0
for d in ${TEST_SUBDIR}; do
    if [ -d "${d}" ]; then
        found_dir=1
    fi
done
if [ ${found_dir} -eq 0 ]; then
    echo "ltp_register: no test directories found in: ${TEST_SUBDIR}"
    exit 0
fi

# Find all .c files in the test subdirectories (unquoted for multiple paths)
ORIGS=$(find ${TEST_SUBDIR} -name "*.c" 2>/dev/null | sort)

if [ -z "${ORIGS}" ]; then
    echo "ltp_register: no .c files found in test directories"
    exit 0
fi

# Blacklist patterns - files containing these are excluded
# Use a newline-separated list for POSIX sh compatibility
BLACKWORDS="pthread_mutexattr_setprioceiling
pthread_mutexattr_getprioceiling
pthread_getattr_np
pthread_mutex_getprioceiling
setpgid(
PTHREAD_SCOPE_PROCESS
setpgrp
threads_scenarii.c
ucontext.h
msync
lfind
_POSIX_SPORADIC_SERVER
sched_setscheduler/19-
sched_setparam/25-
sys/shm.h
shmget
shmat
fork(
getuid
setuid
geteuid
seteuid
setgid
setpwent
getpwent
endpwent
sigaltstack
SS_ONSTACK
SS_DISABLE
stack_t
SIGPOLL
sysconf
pthread_condattr_setpshared
pthread_condattr_setclock
pthread_condattr_getclock
pthread_mutex_timedlock
pthread_rwlockattr_setpshared
pthread_rwlockattr_getpshared
pthread_rwlockattr_init
pthread_rwlockattr_destroy

pthread_attr_setdetachstate(
pthread_attr_getdetachstate(
pthread_attr_setscope(

pthread_attr_getscope(
pthread_attr_setstack(
pthread_attr_getstack(
clock_nanosleep
clock_getcpuclockid
CLOCK_PROCESS_CPUTIME_ID
CLOCK_THREAD_CPUTIME_ID
_POSIX_CPUTIME
_POSIX_THREAD_CPUTIME
alarm("



# Explicitly blacklisted source files (by path suffix).


# TizenRT does not support SCHED_OTHER policy (only SCHED_FIFO and SCHED_RR).
# Cannot use "SCHED_OTHER" as a content grep pattern because it would also
# exclude passing tests and error-handling tests that merely reference
# SCHED_OTHER as an invalid policy.
# Note: The following files were previously blacklisted but are now fixed by patches:
# - stress/threads/pthread_mutex_lock/s-c1.c  (patch 0023: bool->b variable name)
# - conformance/interfaces/signal/6-1.c         (patch 0021: errno lvalue fix)
# - conformance/interfaces/signal/7-1.c         (patch 0021: errno lvalue fix)
# - conformance/interfaces/mq_timedsend/12-1.c (patch 0022: errno lvalue in macro)
BLACKSRCS="conformance/interfaces/pthread_attr_setschedpolicy/1-1.c
conformance/interfaces/pthread_attr_setschedpolicy/5-1.c
conformance/interfaces/sched_get_priority_max/1-4.c
conformance/interfaces/sched_get_priority_min/1-4.c
stress/threads/pthread_cond_init/s-c.c
stress/threads/pthread_mutex_init/s-c.c
conformance/interfaces/pthread_mutex_init/speculative/5-2.c
conformance/interfaces/pthread_create/1-6.c
conformance/interfaces/pthread_attr_setscope/1-1.c
conformance/interfaces/pthread_attr_setscope/5-1.c
conformance/interfaces/sem_post/8-1.c"




# pthread_attr_setschedpolicy/2-1.c requires set_affinity_single() which
# is only implemented for SMP builds (see patch 0015). Blacklist on non-SMP.
if [ "${CONFIG_SMP}" != "y" ]; then
    BLACKSRCS="${BLACKSRCS}
conformance/interfaces/pthread_attr_setschedpolicy/2-1.c"
fi






# TizenRT uses CONFIG_DISABLE_PTHREAD=n to enable pthread (inverse convention).
# If pthread is disabled, blacklist all pthread tests.
if [ "${CONFIG_DISABLE_PTHREAD}" = "y" ]; then
    BLACKWORDS="${BLACKWORDS}
pthread"
fi

# Message queue tests - blacklist if MQ_MAXMSGSIZE < 64
# (matches Makefile conditional and NuttX)
if [ -z "${CONFIG_MQ_MAXMSGSIZE}" ] || [ "${CONFIG_MQ_MAXMSGSIZE}" -lt 64 ] 2>/dev/null; then
    BLACKWORDS="${BLACKWORDS}
mq_close
mq_getattr
mq_notify
mq_open
mq_receive
mq_send
mq_setattr
mq_timedreceive
mq_timedsend
mq_unlink"
fi

# Named semaphore tests - blacklist if CONFIG_FS_NAMED_SEMAPHORES is not set
# (matches Makefile conditional and NuttX)
if [ "${CONFIG_FS_NAMED_SEMAPHORES}" != "y" ]; then
    BLACKWORDS="${BLACKWORDS}
sem_open
sem_close
sem_unlink"
fi

# TizenRT has pthread_cleanup_push/pop but only if CONFIG_PTHREAD_CLEANUP=y
# Blacklist these tests only when the feature is disabled
if [ "${CONFIG_PTHREAD_CLEANUP}" != "y" ]; then
    BLACKWORDS="${BLACKWORDS}
pthread_cleanup_push
pthread_cleanup_pop"
fi

# TizenRT has robust mutex APIs but only if CONFIG_PTHREAD_MUTEX_UNSAFE is NOT set
# When CONFIG_PTHREAD_MUTEX_UNSAFE=y, robust mutex support is compiled out
if [ "${CONFIG_PTHREAD_MUTEX_UNSAFE}" = "y" ]; then
    BLACKWORDS="${BLACKWORDS}
pthread_mutexattr_setrobust
pthread_mutexattr_getrobust
pthread_mutex_consistent"
fi




# Filter out blacklisted files

FILTERED=""
for f in ${ORIGS}; do
    blacklisted=0
    # Check each blacklist word
    echo "${BLACKWORDS}" | while IFS= read -r word; do
        [ -z "${word}" ] && continue
        if grep -q "${word}" "${f}" 2>/dev/null; then
            echo "BLACKLISTED:${f}"
        fi
    done > /tmp/ltp_blacklist_tmp

    if grep -q "BLACKLISTED:${f}" /tmp/ltp_blacklist_tmp 2>/dev/null; then
        blacklisted=1
    fi
    rm -f /tmp/ltp_blacklist_tmp
    # Check against explicitly blacklisted source files (BLACKSRCS)
    if [ ${blacklisted} -eq 0 ]; then
        echo "${BLACKSRCS}" | while IFS= read -r pat; do
            [ -z "${pat}" ] && continue
            case "${f}" in
                *${pat}) echo "BLACKLISTED:${f}" ;;
            esac
        done > /tmp/ltp_blacksrc_tmp
        if grep -q "BLACKLISTED:${f}" /tmp/ltp_blacksrc_tmp 2>/dev/null; then
            blacklisted=1
        fi
        rm -f /tmp/ltp_blacksrc_tmp
    fi
    if [ ${blacklisted} -eq 0 ]; then
        if [ -z "${FILTERED}" ]; then
            FILTERED="${f}"
        else
            FILTERED="${FILTERED} ${f}"
        fi
    fi
done

# Find files containing main()
MAINCSRCS=""
for f in ${FILTERED}; do
    if grep -q "main(" "${f}" 2>/dev/null; then
        if [ -z "${MAINCSRCS}" ]; then
            MAINCSRCS="${f}"
        else
            MAINCSRCS="${MAINCSRCS} ${f}"
        fi
    fi
done

if [ -z "${MAINCSRCS}" ]; then
    echo "ltp_register: no test files with main() found"
    exit 0
fi

# Count tests
test_count=$(echo ${MAINCSRCS} | wc -w)
echo "ltp_register: found ${test_count} test files"

# NOTE: We no longer register individual TASH commands for each test.
# All tests are run via the single "ltprun" TASH command (ltp_runner_main).
# The test table is compiled into ltp_test_registry.h below.


# --- Generate ltp_test_registry.h for ltp_runner ---
# This header contains a memory-optimized test table used by the
# ltprun command to execute tests by category or individually.
REGISTRY_H="${CURDIR}/ltp_test_registry.h"

# Function to map file paths to OS-specific categories.
# This provides high-level grouping (scheduler, pthreads, signals, etc.)
# instead of fine-grained per-interface categories.
#
# The function receives the FULL file path so it can detect stress tests
# by their path (e.g., .../stress/threads/...) rather than just the
# immediate directory name.
#
# To add new API categories (e.g., timers, mqueues, AIO), add new
# pattern cases below. See LTP_Porting_Guide.md for details.
get_os_category() {
    file_path="$1"
    dir_name=$(echo "${file_path}" | awk -F "[/]" '{print $(NF-1)}')

    # Check full path for stress tests first (highest priority)
    case "${file_path}" in
        */stress/*)   echo "stress"; return ;;
        */stress.*)   echo "stress"; return ;;
    esac

    # Check directory name for other categories
    case "${dir_name}" in
        *multi*)             echo "stress" ;;
        condvar)             echo "condvar" ;;
        sem*|semaphores)     echo "semaphores" ;;
        sig*|signal)         echo "signals" ;;
        sched*)              echo "scheduler" ;;
        pthread*sched*)      echo "scheduler" ;;
        pthread*)            echo "pthreads" ;;
        speculative)         echo "scheduler" ;;
        timer*|clock*|nanosleep|timers|clocks)  echo "timers" ;;
        mq_*|mqueues)       echo "mqueues" ;;
        *)                   echo "pthreads" ;;  # default fallback


    esac
}


# Build the fixed OS-specific category list (order must match
# the indices used in ltp_runner.c help text)
CATS="scheduler pthreads signals condvar stress timers mqueues semaphores"




# Generate category table
cat_idx=0
cat > "${REGISTRY_H}" << 'HEADER'

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
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language
 * governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * apps/examples/ltp/ltp_test_registry.h
 *
 * Auto-generated by ltp_register.sh during make context.
 * DO NOT EDIT MANUALLY — edit the Makefile/ltp_register.sh blacklist instead.
 *
 * Memory-optimized test registry for the ltprun test runner.
 * Uses uint8_t category index instead of string pointers to save RAM.
 *
 ****************************************************************************/


#ifndef __LTP_TEST_REGISTRY_H
#define __LTP_TEST_REGISTRY_H

#include <stdint.h>

typedef int (*ltp_test_func_t)(int argc, char *argv[]);

typedef struct {
    const char *test_name;       /* Short TASH name, e.g. "ltp_t1" */
    uint8_t category_id;         /* Index into g_ltp_categories[] */
    ltp_test_func_t test_func;   /* Function pointer to test entry */
} ltp_test_entry_t;

HEADER

echo "" >> "${REGISTRY_H}"
echo "/* Category name table (stored once, referenced by index) */" >> "${REGISTRY_H}"
echo "static const char *g_ltp_categories[] = {" >> "${REGISTRY_H}"
for c in ${CATS}; do
    echo "    \"${c}\"," >> "${REGISTRY_H}"
    cat_idx=$((cat_idx + 1))
done
echo "    NULL" >> "${REGISTRY_H}"
echo "};" >> "${REGISTRY_H}"
echo "" >> "${REGISTRY_H}"
echo "#define LTP_CATEGORY_COUNT ${cat_idx}" >> "${REGISTRY_H}"
echo "" >> "${REGISTRY_H}"

# Generate forward declarations for all test functions
echo "/* Forward declarations of test entry points */" >> "${REGISTRY_H}"
idx=1
for f in ${MAINCSRCS}; do
    funcname=$(echo "${f}" | awk -F "[/]" '{print "ltp_"$(NF-1)"_"$NF}' | sed 's/\.c$//' | sed 's/-/_/g')_main
    echo "extern int ${funcname}(int argc, char *argv[]);" >> "${REGISTRY_H}"
    idx=$((idx + 1))
done
echo "" >> "${REGISTRY_H}"

# Generate test table

idx=1
echo "/* Test table (auto-generated) */" >> "${REGISTRY_H}"
echo "static const ltp_test_entry_t g_ltp_tests[] = {" >> "${REGISTRY_H}"
for f in ${MAINCSRCS}; do
    funcname=$(echo "${f}" | awk -F "[/]" '{print "ltp_"$(NF-1)"_"$NF}' | sed 's/\.c$//' | sed 's/-/_/g')_main
    cmdname="ltp_t${idx}"

    # Map full file path to OS-specific category
    cat_name=$(get_os_category "${f}")


    # Find category index in CATS list
    cidx=0
    for c in ${CATS}; do
        if [ "${c}" = "${cat_name}" ]; then
            break
        fi
        cidx=$((cidx + 1))
    done

    echo "    {\"${cmdname}\", ${cidx}, ${funcname}}," >> "${REGISTRY_H}"

    idx=$((idx + 1))
done
echo "};" >> "${REGISTRY_H}"
echo "" >> "${REGISTRY_H}"
echo "#define LTP_TEST_COUNT ${test_count}" >> "${REGISTRY_H}"
echo "" >> "${REGISTRY_H}"
echo "#endif /* __LTP_TEST_REGISTRY_H */" >> "${REGISTRY_H}"

echo "ltp_register: generated ltp_test_registry.h (${test_count} tests, ${cat_idx} categories)"

# --- Register the ltprun test runner as a TASH command ---
# This provides a single entry point to run all LTP tests by category,
# individually, or all at once.
echo "{ \"ltprun\", ltp_runner_main, ${EXECTYPE}, ${PRIORITY}, ${STACKSIZE} }," > "${REGISTRY}/ltp_runner_main.mdat"
echo "int ltp_runner_main(int argc, char *argv[]);" > "${REGISTRY}/ltp_runner_main.pdat"
echo "  Registered: ltprun -> ltp_runner_main"

echo "ltp_register: done."

# Touch marker file so Makefile skips re-registration on subsequent builds
touch "${CURDIR}/.ltp_registry_generated"



