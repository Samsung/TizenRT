###########################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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
###########################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

CXXEXT ?= .cc
# Hello, Tensorflow Lite Micro! C++ Example

APPNAME    = hello_tfmicro
FUNCNAME   = $(APPNAME)_main
THREADEXEC = TASH_EXECMD_ASYNC

ifeq ($(CONFIG_EXTERNAL_TFMICRO),y)
	CXXFLAGS += -I$(TOPDIR)/../external/tfmicro
	CXXFLAGS += -I$(TOPDIR)/../external/tfmicro/third_party
	CXXFLAGS += -I$(TOPDIR)/../external/tfmicro/third_party/gemmlowp
endif

# Hello, Tensorflow Lite Micro! C++ Example
ASRCS		=
CSRCS		=
CXXSRCS		= constants.cc sine_model_data.cc
MAINSRC		= main.cc

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:$(CXXEXT)=$(OBJEXT))
ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
  MAINOBJ 	= $(MAINSRC:$(CXXEXT)=$(OBJEXT))
else
  MAINOBJ 	= $(MAINSRC:.c=$(OBJEXT))
endif

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS) $(MAINSRC)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
OBJS		+= $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

CONFIG_EXAMPLES_HELLO_TFMICRO_PROGNAME ?= hello_tfmicro$(EXEEXT)
PROGNAME	= $(CONFIG_EXAMPLES_HELLO_TFMICRO_PROGNAME)

ROOTDEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY:	clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)

ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
$(MAINOBJ): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)
else
$(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
endif

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

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_HELLO_TFMICRO),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(FUNCNAME),$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat

else
context:

endif

.depend: Makefile $(SRCS)
ifeq ($(filter %$(CXXEXT),$(SRCS)),)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
else
	@$(MKDEP) $(ROOTDEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
endif
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
