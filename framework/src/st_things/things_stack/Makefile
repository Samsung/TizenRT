############################################################################
# apps/dawit/Makefile
#
#   Copyright (C) 2014 Gregory Nutt. All rights reserved.
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

THINGS_LIB_DIR = $(TOPDIR)/../framework/src/st_things
THINGS_STACK_DIR = $(TOPDIR)/../framework/src/st_things/things_stack

IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR = ../../../external/iotivity/iotivity_$(IOTIVITY_RELEASE)

SERVICE_NTFY_DIR = $(APPDIR)/../external/iotivity/iotivity_1.2-rel/service/notification

ASRCS =

CFLAGS += -DUSE_LOG_NO_NEWLINE
CFLAGS += -D__TIZENRT__
CFLAGS += -D__TINYARA__
CFLAGS += -D__ST_THINGS_RTOS__
CFLAGS += -std=c99
CFLAGS += -w

CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/common/inc
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include/internal
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/extlibs/mbedtls/mbedtls/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/oic_malloc/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/oic_string/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/ocrandom/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/oic_time/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_BASE_DIR)/service/notification/src/common/
CFLAGS += -I$(IOTIVITY_BASE_DIR)/service/notification/include/
CFLAGS += -I$(IOTIVITY_BASE_DIR)/service/notification/src/provider/
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/resource-directory/include/
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/include/
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include/internal/

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/inc
CFLAGS += -I$(THINGS_STACK_DIR)/src/common/cloud/inc

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/easy-setup/inc

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/framework/inc
CFLAGS += -I$(THINGS_STACK_DIR)/src/common/logging/inc
CFLAGS += -I$(THINGS_STACK_DIR)/src/common/memory/inc

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/security/inc

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/utils/inc

CFLAGS += -I$(THINGS_STACK_DIR)/src/common/stack
CFLAGS += -I$(THINGS_STACK_DIR)/inc
CFLAGS += -I$(THINGS_STACK_DIR)/src/common/security/ssCrypto/include


CFLAGS += -I$(SERVICE_NTFY_DIR)/src/common/
CFLAGS += -I$(SERVICE_NTFY_DIR)/src/provider/
CFLAGS += -I$(SERVICE_NTFY_DIR)/include/

ifeq ($(CONFIG_ENABLE_IOTIVITY_SECURED),y)
CFLAGS += -D__WITH_DTLS__
CFLAGS += -DOCF_RTOS_LOCAL_SECURED
CFLAGS += -D_SVR_ENC_
endif

#CFLAGS += -D_EXCLUDE_TEST_KEY_

ifeq ($(CONFIG_ENABLE_IOTIVITY_CLOUD),y)
CFLAGS += -DWITH_CLOUD
CFLAGS += -D__WITH_TLS__
CFLAGS +=-D__SECURED__
CFLAGS += -DTCP_ADAPTER
CFLAGS += -D__ST_THINGS_RTOS_CLOUD__
endif


CFLAGS += -DWITH_POSIX -DIP_ADAPTER
CFLAGS += -DWITH_BWT
ifeq ($(CONFIG_IOTIVITY_ROUTING),"EP")
CFLAGS += -DROUTING_EP
else
CFLAGS += -DROUTING_GW
endif

#------------------------------------------------------------------------------------#
#--------------------------------------------------------------------------
CSRCS += $(wildcard src/common/utils/*.c)
CSRCS += $(wildcard src/common/framework/*.c)
CSRCS += $(wildcard src/common/easy-setup/*.c)
CSRCS += $(wildcard src/common/cloud/*.c)
CSRCS += $(wildcard src/common/security/ssCrypto/src/*.c)

CSRCS += $(wildcard src/stack/*.c)
CSRCS += src/common/logging/thingslogger.c
CSRCS += src/common/memory/thingsmalloc.c

#CFLAGS += -DENABLE_PUSH_NOTY

CSRCS += port_tinyara.c

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\..\\..\\libapps$(LIBEXT)
else
  BIN = ../../../libapps$(LIBEXT)
endif
endif

ROOTDEPPATH = --dep-path .

# Common build

VPATH =

all: .built
$(info ************$(CFLAGS)**********)
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

install:

context:

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)


distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
