###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
############################################################################
# apps/examples/aws/Makefile
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

VPATH = .
ROOTDEPPATH = --dep-path .

# built-in application info

APPNAME = aws_sample
THREADEXEC = TASH_EXECMD_SYNC

# AWS example

IOT_CLIENT_DIR = .
PLATFORM_DIR = $(IOT_CLIENT_DIR)/platform/TizenRT
PLATFORM_COMMON_DIR = $(PLATFORM_DIR)/common
PLATFORM_CERT_DIR = $(PLATFORM_DIR)/cert
SAMPLE_DIR = $(IOT_CLIENT_DIR)/samples/TizenRT

include src$(DELIM)Make.defs

ASRCS =
CSRCS += $(SAMPLE_DIR)/subscribe_publish_sample/subscribe_publish_sample.c \
		 $(SAMPLE_DIR)/subscribe_publish_library_sample/subscribe_publish_library_sample.c \
		 $(SAMPLE_DIR)/shadow_sample_console_echo/shadow_console_echo.c \
		 $(SAMPLE_DIR)/shadow_sample/shadow_sample.c
CSRCS += $(PLATFORM_DIR)/mbedtls/network_mbedtls_wrapper.c
CSRCS += $(PLATFORM_COMMON_DIR)/timer.c
CSRCS += $(PLATFORM_CERT_DIR)/certData.c
CSRCS += $(IOT_CLIENT_DIR)/external_libs/jsmn/jsmn.c

MAINSRC = $(SAMPLE_DIR)/aws_sample_main.c

CXXSRCS += $(SAMPLE_DIR)/subscribe_publish_cpp_sample/subscribe_publish_cpp_sample.cpp

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cpp=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)


CFLAGS += -I$(TOPDIR)/include

CFLAGS += -I$(IOT_CLIENT_DIR)/include
CFLAGS += -I$(IOT_CLIENT_DIR)/external_libs/jsmn
CFLAGS += -I$(PLATFORM_COMMON_DIR)
CFLAGS += -I$(PLATFORM_CERT_DIR)
CFLAGS += -I$(PLATFORM_DIR)/mbedtls
CFLAGS += -I$(SAMPLE_DIR)/subscribe_publish_sample
CFLAGS += -I$(SAMPLE_DIR)/subscribe_publish_library_sample
CFLAGS += -I$(SAMPLE_DIR)/subscribe_publish_cpp_sample
CFLAGS += -I$(SAMPLE_DIR)/shadow_console_echo
CFLAGS += -I$(SAMPLE_DIR)/shadow_sample

# Enable AWS IoT Debugging Messages
#CFLAGS += -DENABLE_IOT_TRACE
#CFLAGS += -DENABLE_IOT_DEBUG
CFLAGS += -DENABLE_IOT_INFO
CFLAGS += -DENABLE_IOT_ERROR


CXXFLAGS += -I$(IOT_CLIENT_DIR)/include
CXXFLAGS += -I$(IOT_CLIENT_DIR)/external_libs/jsmn
CXXFLAGS += -I$(PLATFORM_COMMON_DIR)
CXXFLAGS += -I$(PLATFORM_CERT_DIR)
CXXFLAGS += -I$(PLATFORM_DIR)/mbedtls
CXXFLAGS += -I$(SAMPLE_DIR)/subscribe_publish_cpp_sample

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

CONFIG_AWS_SAMPLE_PROGNAME ?= aws_sample$(EXEEXT)
PROGNAME = $(CONFIG_AWS_SAMPLE_PROGNAME)

# Common build


all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built


ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif


ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_HELLO),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(call REGISTER,$(APPNAME),$(APPNAME)_main,$(THREADEXEC))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat

else
context:

endif

.depend: Makefile $(CSRCS) $(CXXSRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(CSRCS) >Make.dep
	@$(MKDEP) $(ROOTDEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

.PHONY: clean
clean:
	$(call DELFILE, .built)
	$(foreach OBJFILE, $(OBJS), $(call DELFILE, $(OBJFILE)))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:

