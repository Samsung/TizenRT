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
# apps/examples/iotivity/Makefile
#
#   Copyright (C) 2007-2012, 2014 Gregory Nutt. All rights reserved.
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

# iotivity built-in application info

APPNAME = iotivity
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = 16384



# TinyAra Iotivity

ifeq ($(CONFIG_IOTIVITY_SCONS_BUILD),y)
	IOTIVITY_RELEASE = 1.2-rel
	IOTIVITY_BASE_DIR = $(APPDIR)/../external/iotivity/iotivity_$(IOTIVITY_RELEASE)
else
	IOTIVITY_APP_DIR = $(APPDIR)/../external/iotivity/iotivity_1.1.0
	IOTIVITY_SECURITY_APP_DIR = $(IOTIVITY_APP_DIR)/resource/csdk/security/src
	IOTIVITY_CONNECTIVITY_ADAPTER_UTIL_DIR = $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/adapter_util
endif

ASRCS =
ifeq ($(CONFIG_IOTIVITY_SCONS_BUILD),y)
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include
else
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinycbor/tinycbor
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinycbor/tinycbor/src
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinydtls
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinydtls/aes
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinydtls/ecc
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/tinydtls/sha2
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/cjson
CFLAGS += -I$(IOTIVITY_APP_DIR)/extlibs/timer
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/oc_logger/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/c_common/ocrandom/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/c_common/oic_malloc/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/c_common/oic_string/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/c_common/oic_time/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/connectivity/common/inc
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/connectivity/inc
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/connectivity/inc/pkix
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/security/include
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/security/include/internal
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/stack/include/internal
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/stack/include/
ifeq ($(CONFIG_ROUTING_EP),y)
CFLAGS += -I$(IOTIVITY_APP_DIR)/resource/csdk/routing/include/
endif
ifeq ($(CONFIG_ENABLE_IOTIVITY_CLOUD),y)
CFLAGS += -I$(IOTIVITY_APP_DIR)/cloud/samples/client/
endif
endif #CONFIG_IOTIVITY_SCONS_BUILD

ifneq ($(CONFIG_IOTIVITY_SCONS_BUILD),y)

CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/tinycbor/tinycbor/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/cjson/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/timer/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/c_common/ocrandom/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/c_common/oic_malloc/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/c_common/oic_string/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/c_common/oic_time/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/common/src/*.c)

CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/ip_adapter/*.c)
ifeq ($(CONFIG_TCP_ADAPTER),y)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/tcp_adapter/*.c)
endif
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/ip_adapter/linux/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/logger/src/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/*.c)
CSRCS += $(IOTIVITY_CONNECTIVITY_ADAPTER_UTIL_DIR)/caadapterutils.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/secureresourcemanager.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/resourcemanager.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/aclresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/amaclresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/amsmgr.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/pstatresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/doxmresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/credresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/svcresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/pconfresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/dpairingresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/verresource.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/policyengine.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/psinterface.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/srmresourcestrings.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/srmutility.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/iotvticalendar.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/iotivitybase64.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/directpairing.c

#IOTIVITY CLOUD COMPILATION FILES

ifeq ($(CONFIG_ENABLE_IOTIVITY_CLOUD),y)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/cloud/samples/client/*.c)
endif

#IOTIVITY SECURITY COMPILATION FILES
#CONFIG_ENABLE_IOTIVITY_SECURED is aligned to SECURED flag in iotivity linux build
#CONFIG_WITH_X509 is aligned to DTLS_WITH_X509 flag in iotivity linux build


ifeq ($(CONFIG_ENABLE_IOTIVITY_SECURED),y)

CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/tinydtls/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/tinydtls/aes/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/tinydtls/ecc/*.c)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/extlibs/tinydtls/sha2/*.c)
CSRCS += $(IOTIVITY_CONNECTIVITY_ADAPTER_UTIL_DIR)/caadapternetdtls.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/oxmpincommon.c
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/pbkdf2.c
ifeq ($(CONFIG_WITH_X509),y)
CSRCS += $(IOTIVITY_SECURITY_APP_DIR)/crlresource.c
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/connectivity/src/adapter_util/pkix/*.c)
endif
endif
endif #CONFIG_IOTIVITY_SCONS_BUILD

ifneq ($(CONFIG_IOTIVITY_SCONS_BUILD),y)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/stack/src/*.c)
ifeq ($(CONFIG_ROUTING_EP),y)
CSRCS += $(wildcard $(IOTIVITY_APP_DIR)/resource/csdk/routing/src/*.c)
endif
endif #CONFIG_IOTIVITY_SCONS_BUILD

# Port TinyAra
CSRCS += port_tinyara.c

MAINSRC = iotivity_main.c

ifeq ($(CONFIG_ENABLE_IOTIVITY_CLOUD),y)
MAINSRC += iotivity_cloud_main.c
endif

ifeq ($(CONFIG_ENABLE_IOTIVITY_SECURED),y)
MAINSRC += iotivity_sec_main.c file_data.c
CFLAGS += -D__WITH_DTLS__
endif

CFLAGS += -DWITH_POSIX -DIP_ADAPTER
CFLAGS += -DWITH_BWT
ifeq ($(CONFIG_IOTIVITY_ROUTING),"EP")
CFLAGS += -DROUTING_EP
else
CFLAGS += -DROUTING_GW
endif
CFLAGS += -D__TINYARA__
CFLAGS += -std=c99
CFLAGS += -w

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

CONFIG_EXAMPLES_IOTIVITY_PROGNAME ?= iotivity$(EXEEXT)
PROGNAME = $(CONFIG_EXAMPLES_IOTIVITY_PROGNAME)

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

file_data.c: data/*
	$(Q) ./mkfsdata.pl

ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	@echo "LD: $(PROGNAME)"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif

context:

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

custom_clean:
	$(foreach OBJFILE, $(OBJS), rm -f $(OBJFILE))

clean: custom_clean
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
