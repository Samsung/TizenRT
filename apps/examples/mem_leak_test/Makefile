############################################################################
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
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
############################################################################
############################################################################
# apps/examples/mem_leak_test/Makefile
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# Memory Leak Checker Test Suite built-in application info

APPNAME = mem_leak_test
FUNCNAME = $(APPNAME)_main
THREADEXEC = TASH_EXECMD_ASYNC

# Test source files

ASRCS =
CSRCS  = test_reference_cycle.c
CSRCS += test_mq_shared.c
CSRCS += test_workqueue_shared.c
CSRCS += test_mq_leak.c
CSRCS += test_work_cancel.c
CSRCS += test_normal_alloc_free.c
CSRCS += test_bss_pointer.c
CSRCS += test_stack_pointer.c
CSRCS += test_data_pointer.c
CSRCS += test_multilevel_ref.c
CSRCS += test_simple_orphan.c
CSRCS += test_triple_cycle.c
CSRCS += test_large_alloc.c
CSRCS += test_many_allocs.c
CSRCS += test_cross_task.c
CSRCS += test_kernel_to_app.c
MAINSRC = mem_leak_test_main.c


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

ifeq ($(WINTOOL),y)
  INSTALL_DIR = "${shell cygpath -w $(BIN_DIR)}"
else
  INSTALL_DIR = $(BIN_DIR)
endif

CONFIG_EXAMPLES_MEM_LEAK_TEST_PROGNAME ?= mem_leak_test$(EXEEXT)
PROGNAME = $(CONFIG_EXAMPLES_MEM_LEAK_TEST_PROGNAME)

ROOTDEPPATH = --dep-path .

CFLAGS += -I$(TOPDIR)/../apps/examples/mem_leak_test

# Common build

VPATH =

all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	@echo "LD: $(PROGNAME)"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_MEM_LEAK_TEST),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(FUNCNAME),$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat

else
context:

endif

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

-include Make.dep
.PHONY: preconfig
preconfig:
