############################################################################
# apps/examples/pthread_cleanup_test/Makefile
#
#   Copyright (C) 2026 Samsung Electronics. All rights reserved.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

# pthread_cleanup_test built-in application info

APPNAME = pthread_cleanup_test
FUNCNAME = pthread_cleanup_main
THREADEXEC = TASH_EXECMD_SYNC

# Pthread cleanup test sources

ASRCS =
CSRCS = test_basic.c test_cancel.c test_cancel_points.c test_stress.c
CSRCS += test_resources.c test_edge_cases.c
MAINSRC = pthread_cleanup_main.c

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

# For builtin apps (TASH commands), always include MAINOBJ in libapps.a
ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST),yy)
  OBJS += $(MAINOBJ)
else
  ifneq ($(CONFIG_BUILD_KERNEL),y)
    OBJS += $(MAINOBJ)
  endif
endif

ifneq ($(CONFIG_WINDOWS_NATIVE),y)
  ifeq ($(WINTOOL),y)
    BIN = $(APPDIR)\\libapps$(LIBEXT)
  else
    BIN = $(APPDIR)/libapps$(LIBEXT)
  endif
else
  BIN = $(APPDIR)\libapps$(LIBEXT)
endif

ifeq ($(WINTOOL),y)
  INSTALL_DIR = "${shell cygpath -w $(BIN_DIR)}"
else
  INSTALL_DIR = $(BIN_DIR)
endif

CONFIG_XYZ_PROGNAME ?= pthread_cleanup_test$(EXEEXT)
PROGNAME = $(CONFIG_XYZ_PROGNAME)

ROOTDEPPATH = --dep-path .

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

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_PTHREAD_CLEANUP_TEST),yy)
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
