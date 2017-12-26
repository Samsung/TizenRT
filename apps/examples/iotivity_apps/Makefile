###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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
# apps/examples/iotivity_apps/Makefile
#
#   Copyright (C) 2008, 2010-2013 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs
include $(APPDIR)/Make.defs

THREADEXEC = TASH_EXECMD_ASYNC

# built-in application info

# iotivitySimServ example
IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR = $(APPDIR)/../external/iotivity/iotivity_$(IOTIVITY_RELEASE)
IOTIVITY_TARGET_ARCH=${shell echo $(CONFIG_ARCH_FAMILY) | sed 's/"//g'}
ifeq ($(CONFIG_IOTIVITY_RELEASE),y)
IOTIVITY_OUT_DIR=$(IOTIVITY_BASE_DIR)/out/tizenrt/$(IOTIVITY_TARGET_ARCH)/release
else
IOTIVITY_OUT_DIR=$(IOTIVITY_BASE_DIR)/out/tizenrt/$(IOTIVITY_TARGET_ARCH)/debug
endif
IOTIVITY_CONFIG_DIR=$(IOTIVITY_OUT_DIR)/include/c_common

ifeq ($(CONFIG_EXAMPLES_IOTIVITY_SIMPLESERVER),y)
CSRCS += simpleServer.c
else
ifeq ($(CONFIG_EXAMPLES_IOTIVITY_SECURESERVER),y)
CSRCS += secureServer.c
else
ifeq ($(CONFIG_EXAMPLES_IOTIVITY_CLOUDSERVER),y)
CSRCS += cloudServer.c
endif
endif
endif

CSRCS += iotivity_common.c

CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/resource-directory/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/common/inc
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/build_common/tizenrt/compatibility
CFLAGS += -I$(APPDIR)/../external/mbedtls/include
CFLAGS += -I$(IOTIVITY_CONFIG_DIR)

CFLAGS += -DWITH_POSIX -DIP_ADAPTER
CFLAGS += -DWITH_BWT
ifeq ($(CONFIG_IOTIVITY_ROUTING),"EP")
CFLAGS += -DROUTING_EP
else
CFLAGS += -DROUTING_GW
endif
CFLAGS += -D__TIZENRT__
CFLAGS += -std=c99
CFLAGS += -w
CFLAGS += -D__WITH_DTLS__

ifeq ($(CONFIG_IOTIVITY_RELEASE_VERSION),"1.2-rel")
CSRCS += port_tinyara.c
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\..\\libapps$(LIBEXT)
else
  BIN = ../../libapps$(LIBEXT)
endif
endif

ifeq ($(WINTOOL),y)
  INSTALL_DIR = "${shell cygpath -w $(BIN_DIR)}"
else
  INSTALL_DIR = $(BIN_DIR)
endif

CONFIG_EXAMPLES_IOTIVITY_PROGNAME ?= iotivityApp$(EXEEXT)
PROGNAME = $(CONFIG_EXAMPLES_IOTIVITY_PROGNAME)


ROOTDEPPATH = --dep-path .

# Common build

VPATH = $(IOTIVITY_BASE_DIR)/extlibs/cjson

all: .built
.PHONY: clean depend distclean preconfig

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

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_IOTIVITY),yy)

# simpleServer
ifeq ($(CONFIG_EXAMPLES_IOTIVITY_SIMPLESERVER),y)
$(BUILTIN_REGISTRY)$(DELIM)simpleServer_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,simpleServer,simpleServer_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))
context: $(BUILTIN_REGISTRY)$(DELIM)simpleServer_main.bdat
endif

# secureServer
ifeq ($(CONFIG_EXAMPLES_IOTIVITY_SECURESERVER),y)
ifeq ($(findstring $(EXTRA_FILES),file_data.c),)
  EXTRA_FILES += file_data.c
endif
$(BUILTIN_REGISTRY)$(DELIM)secureServer_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,secureServer,secureServer_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))
context: $(BUILTIN_REGISTRY)$(DELIM)secureServer_main.bdat
endif

# cloudServer
ifeq ($(CONFIG_EXAMPLES_IOTIVITY_CLOUDSERVER),y)
ifeq ($(findstring $(EXTRA_FILES),file_data.c),)
  EXTRA_FILES += file_data.c
endif
$(BUILTIN_REGISTRY)$(DELIM)cloudServer_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,cloudServer,cloudServer_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))
context: $(BUILTIN_REGISTRY)$(DELIM)cloudServer_main.bdat
endif

else
context:

endif

IOTIVITY_CONFIG_FILE=$(IOTIVITY_CONFIG_DIR)/iotivity_config.h
EXTRA_FILES += $(IOTIVITY_CONFIG_FILE)

$(IOTIVITY_CONFIG_FILE):
	$(Q) if [ ! -e $(IOTIVITY_CONFIG_DIR) ]; then \
		mkdir -p $(IOTIVITY_CONFIG_DIR); \
	fi;
	$(Q) touch $(IOTIVITY_CONFIG_FILE)

file_data.c: mkfsdata.pl
	$(Q) ./mkfsdata.pl

.depend: Makefile $(SRCS) $(EXTRA_FILES)
	@$(MKDEP) $(ROOTDEPPATH) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	"$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call DELFILE, file_data.c)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
preconfig:
