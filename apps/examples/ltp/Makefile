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
# apps/examples/ltp/Makefile
#
# Port of NuttX LTP integration to TizenRT.
# Extracts LTP v20230516 (only once), applies TizenRT-specific patches,
# discovers test files with main(), renames main() to unique entry points
# via -Dmain=xxx_main, and registers a single "ltprun" TASH command.
#
# Build flow:

#   1. make context  - extracts LTP source (only once), applies patches,
#                      generates ltp_test_registry.h for the ltprun runner
#   2. make          - compiles test files with main() renaming
#
# Note: On the first build, the LTP source is extracted during context.
# The test discovery happens at Makefile parse time, so on the very first
# build no tests are discovered. Run the build a second time to compile
# the tests. This is the same behavior as NuttX's LTP integration.

#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# LTP version - downloaded as tarball from GitHub during make context

LTP_VERSION     = 20230516

LTP_UNPACK       = ltp


TESTDIR          = $(LTP_UNPACK)/testcases/open_posix_testsuite

# Test directories to include in the build.
# Batch 1: functional/threads (4 tests) + sched_* query tests
LTP_TEST_SUBDIR  = $(TESTDIR)/functional/threads
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_get_priority_max
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_get_priority_min
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_getscheduler
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_yield
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_getparam
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_rr_get_interval
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_setparam
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sched_setscheduler
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_setschedparam
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_setschedpolicy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_getschedparam
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_setschedparam
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_setschedprio

# Basic pthread tests (non-sched) - categorized as "pthreads"
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_create
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_exit
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_join
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_detach
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cancel
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_once
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_self
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_lock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_unlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_trylock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_mutex_timedlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_broadcast
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_signal
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_timedwait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_wait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_cond_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_barrier_wait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_barrier_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_barrier_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_setstacksize
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_attr_getstacksize

LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_atfork
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_sigmask


# Stress tests
LTP_TEST_SUBDIR += $(TESTDIR)/stress/threads
LTP_TEST_SUBDIR += $(TESTDIR)/stress/semaphores
LTP_TEST_SUBDIR += $(TESTDIR)/stress/mqueues
LTP_TEST_SUBDIR += $(TESTDIR)/stress/signals

# Signal tests
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigaction
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigaddset
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigdelset
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigemptyset
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigfillset
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sighold
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigignore
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigismember
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigpause
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigpending
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigprocmask
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigqueue
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigrelse
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigset
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigsuspend
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigaltstack
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigwait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigwaitinfo
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sigtimedwait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/signal

# Timer/Clock tests
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/timer_create
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/timer_settime
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/timer_delete
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/timer_getoverrun
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/timer_gettime
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock_gettime
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock_settime
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock_getres
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock_nanosleep
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/nanosleep
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/clock_getcpuclockid
LTP_TEST_SUBDIR += $(TESTDIR)/functional/timers

# Message Queue tests
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_open
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_close
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_send
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_receive
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_timedsend
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_timedreceive
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_unlink
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_notify
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_getattr
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/mq_setattr
LTP_TEST_SUBDIR += $(TESTDIR)/functional/mqueues

# Semaphore tests (unnamed - always enabled)
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_post
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_wait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_timedwait
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_getvalue
LTP_TEST_SUBDIR += $(TESTDIR)/functional/semaphores
LTP_TEST_SUBDIR += $(TESTDIR)/stress/semaphores

# Named semaphore tests (conditional on CONFIG_FS_NAMED_SEMAPHORES)
ifeq ($(CONFIG_FS_NAMED_SEMAPHORES),y)
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_open
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_close
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/sem_unlink
endif

# Reader-Writer Lock tests
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_init
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_destroy
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_rdlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_wrlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_unlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_tryrdlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_trywrlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_timedrdlock
LTP_TEST_SUBDIR += $(TESTDIR)/conformance/interfaces/pthread_rwlock_timedwrlock

# Blacklist: patterns that cause build failures on TizenRT.






# Files matching these patterns (via grep) are excluded from the build.

BLACKWORDS += "pthread_mutexattr_setprioceiling"
BLACKWORDS += "pthread_mutexattr_getprioceiling"
BLACKWORDS += "pthread_getattr_np"
BLACKWORDS += "pthread_mutex_getprioceiling"
BLACKWORDS += "setpgid("

BLACKWORDS += "PTHREAD_SCOPE_PROCESS"
BLACKWORDS += "setpgrp"
BLACKWORDS += "threads_scenarii.c"
BLACKWORDS += "ucontext.h"
BLACKWORDS += "msync"
BLACKWORDS += "lfind"

# TizenRT does not define SIGPOLL (I/O polling signal). NuttX also blacklists this.
BLACKWORDS += "SIGPOLL"

# TizenRT does not implement sysconf() - declared in headers but no body
BLACKWORDS += "sysconf"

# TizenRT does not implement pthread_condattr_setpshared/setclock/getclock
BLACKWORDS += "pthread_condattr_setpshared"
BLACKWORDS += "pthread_condattr_setclock"
BLACKWORDS += "pthread_condattr_getclock"

# TizenRT does not implement pthread_mutex_timedlock
BLACKWORDS += "pthread_mutex_timedlock"

# TizenRT does not implement pthread_rwlockattr_setpshared/getpshared
BLACKWORDS += "pthread_rwlockattr_setpshared"
BLACKWORDS += "pthread_rwlockattr_getpshared"

# TizenRT does not implement pthread_rwlockattr_init/destroy (no rwlock attr support)
BLACKWORDS += "pthread_rwlockattr_init"
BLACKWORDS += "pthread_rwlockattr_destroy"



# TizenRT has pthread_cleanup_push/pop but only if CONFIG_PTHREAD_CLEANUP=y
# Blacklist these tests only when the feature is disabled
ifeq ($(CONFIG_PTHREAD_CLEANUP),)
BLACKWORDS += "pthread_cleanup_push"
BLACKWORDS += "pthread_cleanup_pop"
endif

# TizenRT has robust mutex APIs but only if CONFIG_PTHREAD_MUTEX_UNSAFE is NOT set
# When CONFIG_PTHREAD_MUTEX_UNSAFE=y, robust mutex support is compiled out
ifneq ($(CONFIG_PTHREAD_MUTEX_UNSAFE),)
BLACKWORDS += "pthread_mutexattr_setrobust"
BLACKWORDS += "pthread_mutexattr_getrobust"
BLACKWORDS += "pthread_mutex_consistent"
endif


# TizenRT does not implement pthread_attr_setdetachstate/getdetachstate
# Use "(" suffix to match function calls, not comments/headers
BLACKWORDS += "pthread_attr_setdetachstate("
BLACKWORDS += "pthread_attr_getdetachstate("

# TizenRT does not implement pthread_attr_setscope/getscope
BLACKWORDS += "pthread_attr_setscope("
BLACKWORDS += "pthread_attr_getscope("

# TizenRT does not implement pthread_attr_setstack/getstack
BLACKWORDS += "pthread_attr_setstack("
BLACKWORDS += "pthread_attr_getstack("



# TizenRT does not implement alarm()
BLACKWORDS += "alarm("

# TizenRT does not implement clock_nanosleep() - no syscall or libc stub
BLACKWORDS += "clock_nanosleep"

# TizenRT does not implement clock_getcpuclockid() - no CPU-time clock support
BLACKWORDS += "clock_getcpuclockid"

# TizenRT does not define CLOCK_PROCESS_CPUTIME_ID or CLOCK_THREAD_CPUTIME_ID
BLACKWORDS += "CLOCK_PROCESS_CPUTIME_ID"
BLACKWORDS += "CLOCK_THREAD_CPUTIME_ID"

# TizenRT does not define _POSIX_CPUTIME or _POSIX_THREAD_CPUTIME
BLACKWORDS += "_POSIX_CPUTIME"
BLACKWORDS += "_POSIX_THREAD_CPUTIME"

# TizenRT does not implement clock_settime() fully - it requires RTC hardware
# support. Tests that call clock_settime() may fail at runtime. However, the
# function is declared in headers, so we let them compile and they will be
# handled at runtime.

# pthread_mutex_lock/s-c1.c: 'bool' variable name fixed by patch 0023
# (was: conflicts with stdbool.h on TizenRT)

# pthread_cond_init/s-c.c uses setrlimit/RLIMIT_AS (Linux-specific, not on TizenRT)
BLACKSRCS += $(TESTDIR)/stress/threads/pthread_cond_init/s-c.c

# pthread_mutex_init/s-c.c uses setrlimit/RLIMIT_AS (Linux-specific, not on TizenRT)
BLACKSRCS += $(TESTDIR)/stress/threads/pthread_mutex_init/s-c.c

# signal/6-1.c and 7-1.c: errno lvalue bug fixed by patch 0021
# (was: assign directly to errno which is a macro on TizenRT)

# mq_timedsend/12-1.c: errno lvalue bug in error_and_exit macro fixed by patch 0022



# pthread_mutex_init/speculative/5-2.c uses <sys/utsname.h> and uname()
# (not available in TizenRT)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_mutex_init/speculative/5-2.c

# pthread_create/1-6.c uses setitimer() and ITIMER_PROF
# (not available in TizenRT - no interval timer support)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_create/1-6.c

# pthread_attr_setscope/1-1.c and 5-1.c use PTHREAD_SCOPE_SYSTEM
# (not defined in TizenRT - only PTHREAD_SCOPE_PROCESS is supported)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_attr_setscope/1-1.c
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_attr_setscope/5-1.c










# Sporadic server tests - TizenRT does not support _POSIX_SPORADIC_SERVER
BLACKWORDS += "_POSIX_SPORADIC_SERVER"
BLACKWORDS += "sched_setscheduler/19-"
BLACKWORDS += "sched_setparam/25-"

# Shared memory tests - TizenRT does not have sys/shm.h
BLACKWORDS += "sys/shm.h"
BLACKWORDS += "shmget"
BLACKWORDS += "shmat"

# TizenRT does not support fork() - only vfork() is available
BLACKWORDS += "fork("

# TizenRT does not support user identity functions (single-user RTOS)
BLACKWORDS += "getuid"
BLACKWORDS += "setuid"
BLACKWORDS += "geteuid"
BLACKWORDS += "seteuid"
BLACKWORDS += "setgid"

# TizenRT does not have password database
BLACKWORDS += "setpwent"
BLACKWORDS += "getpwent"
BLACKWORDS += "endpwent"

# TizenRT does not support sigaltstack (alternate signal stack)
BLACKWORDS += "sigaltstack"
BLACKWORDS += "SS_ONSTACK"
BLACKWORDS += "SS_DISABLE"
BLACKWORDS += "stack_t"


# TizenRT does not support SCHED_OTHER policy (only SCHED_FIFO and SCHED_RR).
# Blacklist specific test files that require SCHED_OTHER support.
# (Cannot use "SCHED_OTHER" as a content grep pattern because it would
#  also exclude passing tests and error-handling tests that merely
#  reference SCHED_OTHER as an invalid policy.)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_attr_setschedpolicy/1-1.c
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_attr_setschedpolicy/5-1.c
BLACKSRCS += $(TESTDIR)/conformance/interfaces/sched_get_priority_max/1-4.c
BLACKSRCS += $(TESTDIR)/conformance/interfaces/sched_get_priority_min/1-4.c

# pthread_attr_setschedpolicy/2-1.c requires set_affinity_single() which
# is only implemented for SMP builds (see patch 0015). Blacklist on non-SMP.
ifeq ($(CONFIG_SMP),)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/pthread_attr_setschedpolicy/2-1.c
endif



# SCHED_RR tests - blacklist if RR_INTERVAL is not configured
ifneq ($(shell expr $(CONFIG_RR_INTERVAL) \> 0),1)
  BLACKWORDS += "SCHED_RR"
endif


# TizenRT uses CONFIG_DISABLE_PTHREAD=n to enable pthread (inverse convention).
# If pthread is disabled, blacklist all pthread tests.
ifeq ($(CONFIG_DISABLE_PTHREAD),y)
  BLACKWORDS += "pthread"
endif

# Message queue tests - blacklist if MQ_MAXMSGSIZE < 64
# (matches NuttX conditional: ifneq ($(shell expr $(CONFIG_MQ_MAXMSGSIZE) \>= 64),1))
ifneq ($(shell expr $(CONFIG_MQ_MAXMSGSIZE) \>= 64),1)
  BLACKWORDS += "mq_close"
  BLACKWORDS += "mq_getattr"
  BLACKWORDS += "mq_notify"
  BLACKWORDS += "mq_open"
  BLACKWORDS += "mq_receive"
  BLACKWORDS += "mq_send"
  BLACKWORDS += "mq_setattr"
  BLACKWORDS += "mq_timedreceive"
  BLACKWORDS += "mq_timedsend"
  BLACKWORDS += "mq_unlink"
endif

# Named semaphore tests - blacklist if CONFIG_FS_NAMED_SEMAPHORES is not set
# (matches NuttX conditional: ifeq ($(CONFIG_FS_NAMED_SEMAPHORES),))
ifeq ($(CONFIG_FS_NAMED_SEMAPHORES),)
  BLACKWORDS += "sem_open"
  BLACKWORDS += "sem_close"
  BLACKWORDS += "sem_unlink"
endif

# sem_post/8-1.c uses fork() (not available on TizenRT)
BLACKSRCS += $(TESTDIR)/conformance/interfaces/sem_post/8-1.c




# Pattern to find files containing main()
MAINWORDS += "main("

# --- Test discovery and filtering (at parse time) ---
# This logic mirrors NuttX's approach. Only runs if LTP source exists.

ifneq ($(wildcard $(TESTDIR)),)

# Step 1: Find all .c files in the test subdirectory
LTP_ORIGS := $(shell find $(LTP_TEST_SUBDIR) -name "*.c")

# Step 2: Build blacklist by grepping for blacklisted words
ifneq ($(LTP_ORIGS),)
$(foreach word, $(BLACKWORDS), $(eval BLACKLIST += $(shell grep -lr $(word) $(LTP_ORIGS))))
endif

# Also remove explicitly blacklisted source files (BLACKSRCS)
BLACKLIST += $(BLACKSRCS)

# Remove blacklisted files from the list
LTP_ORIGS := $(filter-out $(BLACKLIST), $(LTP_ORIGS))

# Step 3: Find files containing main()
ifneq ($(LTP_ORIGS),)
$(foreach word, $(MAINWORDS), $(eval LTP_MAINCSRCS += $(shell grep -lr $(word) $(LTP_ORIGS))))
endif

# Non-main source files (compiled into libapps as support code)
LTP_CSRCS := $(filter-out $(LTP_MAINCSRCS), $(LTP_ORIGS))

endif # wildcard $(TESTDIR)

# Build configuration

STACKSIZE     = $(CONFIG_EXAMPLES_LTP_STACKSIZE)
PRIORITY      = $(CONFIG_EXAMPLES_LTP_PRIORITY)
THREADEXEC    = TASH_EXECMD_ASYNC

# Source files
# ltp_runner.c is the test runner application (ltprun command).
# It is compiled as a support file (CSRCS), not via the main-renaming rule,
# because its entry point is ltp_runner_main() (not main()).
# It includes the auto-generated ltp_test_registry.h
ASRCS =
CSRCS = $(LTP_CSRCS) ltp_runner.c
MAINSRC = $(LTP_MAINCSRCS)



AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = $(APPDIR)\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = $(APPDIR)\\libapps$(LIBEXT)
else
  BIN = $(APPDIR)/libapps$(LIBEXT)
endif
endif

ROOTDEPPATH = --dep-path . --dep-path $(LTP_UNPACK)

VPATH = $(LTP_UNPACK)

# Include paths for LTP headers
CFLAGS += -I$(CURDIR)
CFLAGS += -I$(TESTDIR)/include
CFLAGS += -I$(TESTDIR)/../../include

# Force-include config.h for TizenRT-specific feature definitions
# (e.g. SIGTERM definition, feature detection macros)
CFLAGS += -include $(CURDIR)/config.h

# Define __TIZENRT__ for conditional compilation in LTP tests
CFLAGS += -D__TIZENRT__
ifeq ($(CONFIG_SMP),y)
CFLAGS += -DCONFIG_SMP
endif
# Relax warning checks - LTP tests generate many expected warnings
CFLAGS += -Wno-strict-prototypes -Wno-return-type -Wno-format -Wno-uninitialized
CFLAGS += -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value
CFLAGS += -Wno-int-conversion -Wno-shadow
CFLAGS += -Wno-incompatible-pointer-types -Wno-overflow -Wno-int-to-pointer-cast
CFLAGS += -Wno-shift-count-overflow

all: .built
.PHONY: clean depend distclean context

# --- main() renaming mechanism ---
# For each test source file, compile with -Dmain=<funcname> to rename
# the main() function to a unique entry point.
# This is the same approach used by NuttX (apps/Application.mk lines 352-356).
#
# The function name is derived from the file path:
#   ltp/.../condvar/pthread_cond_wait_1.c
#   -> ltp_condvar_pthread_cond_wait_1_main
#
# We use a shell command in the recipe to compute the function name,
# since Make functions cannot easily process $< at recipe execution time.

$(MAINOBJ): %$(OBJEXT): %.c
	$(Q) dir=$$(basename $$(dirname $<)); \
	file=$$(basename $< .c); \
	funcname=ltp_$$(echo $$dir | sed 's/-/_/g')_$$(echo $$file | sed 's/-/_/g')_main; \
	echo "CC: $< (main=$$funcname)"; \
	$(CC) -c $(CFLAGS) -Dmain=$$funcname $< -o $@

# Compile support (non-main) source files normally
$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

.built: $(OBJS) $(MAINOBJ)
	$(call ARCHIVE, $(BIN), $(OBJS) $(MAINOBJ))
	@touch .built

# --- LTP source download, patching and registry generation ---
# The LTP source is downloaded automatically from within this Makefile
# during the context target. This runs inside the Docker build container.
# (dbuild.sh clears proxy env vars so that curl/wget can access the internet
# directly from inside Docker.)
#
# Patches 0017-0018 fix LTP scheduler test issues on TizenRT:
#   0017: pthread_setschedparam/5-1.c - fix deadlock and CPU starvation:
#         - Add usleep(50) in worker loop to prevent CPU starvation
#         - Add sem_post() to wake up blocked sendsig threads so they exit
#   0018: Add "Test PASSED" + fflush(stdout) completion prints to
#         pthread_setschedparam/1-2.c and pthread_setschedprio/1-1.c
# These patches are applied automatically by the patches/*.patch glob above.

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_LTP),yy)
context:
	$(Q) if [ ! -d "$(LTP_UNPACK)/testcases" ]; then \
		echo "LTP source not found, downloading v$(LTP_VERSION)..."; \
		curl -kL --connect-timeout 30 --retry 3 -o /tmp/ltp-$(LTP_VERSION).tar.gz \
			https://github.com/linux-test-project/ltp/archive/refs/tags/$(LTP_VERSION).tar.gz; \
		tar -xzf /tmp/ltp-$(LTP_VERSION).tar.gz -C $(CURDIR); \
		mv $(CURDIR)/ltp-$(LTP_VERSION) $(CURDIR)/$(LTP_UNPACK); \
		rm -f /tmp/ltp-$(LTP_VERSION).tar.gz; \
		echo "LTP source downloaded and extracted."; \
	fi
	$(Q) if [ ! -f .ltp_patched ] || [ -n "$$(find patches -name '*.patch' -newer .ltp_patched -print -quit)" ]; then \
		echo "Applying LTP patches..."; \
		for p in patches/*.patch; do patch -N -d $(LTP_UNPACK) -p1 < $$p || true; done; \
		touch .ltp_patched; \
	else \
		echo "LTP source already patched, skipping..."; \
	fi
	$(Q) if [ ! -f .ltp_registry_generated ] || [ .ltp_patched -nt .ltp_registry_generated ]; then \
		echo "Generating LTP test registry..."; \
		CURDIR="$(CURDIR)" \
			TESTDIR="$(CURDIR)/$(TESTDIR)" \
			LTP_TEST_SUBDIR="$(addprefix $(CURDIR)/,$(LTP_TEST_SUBDIR))" \
			BUILTIN_REGISTRY="$(BUILTIN_REGISTRY)" \
			STACKSIZE="$(STACKSIZE)" \
			PRIORITY="$(PRIORITY)" \
			THREADEXEC="$(THREADEXEC)" \
			CONFIG_DISABLE_PTHREAD="$(CONFIG_DISABLE_PTHREAD)" \
			CONFIG_SMP="$(CONFIG_SMP)" \
			CONFIG_MQ_MAXMSGSIZE="$(CONFIG_MQ_MAXMSGSIZE)" \
			CONFIG_FS_NAMED_SEMAPHORES="$(CONFIG_FS_NAMED_SEMAPHORES)" \
			CONFIG_PTHREAD_CLEANUP="$(CONFIG_PTHREAD_CLEANUP)" \
			CONFIG_PTHREAD_MUTEX_UNSAFE="$(CONFIG_PTHREAD_MUTEX_UNSAFE)" \
			sh $(CURDIR)/ltp_register.sh; \
	else \
		echo "LTP test registry already generated, skipping..."; \
		echo "Registering ltprun TASH command..."; \
		mkdir -p $(BUILTIN_REGISTRY); \
		echo "{ \"ltprun\", ltp_runner_main, $(THREADEXEC), $(PRIORITY), $(STACKSIZE) }," > $(BUILTIN_REGISTRY)/ltp_runner_main.mdat; \
		echo "int ltp_runner_main(int argc, char *argv[]);" > $(BUILTIN_REGISTRY)/ltp_runner_main.pdat; \
	fi
else
context:
endif






install:

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(Q) rm -rf $(LTP_UNPACK)
	$(Q) rm -f .ltp_patched .ltp_registry_generated





-include Make.dep
.PHONY: preconfig
preconfig:
