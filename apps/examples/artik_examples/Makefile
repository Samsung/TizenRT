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
# apps/examples/artik_examples/Makefile
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

APPNAME = artik_examples
PRIORITY = SCHED_PRIORITY_DEFAULT
STACKSIZE = 2048
THREADEXEC = TASH_EXECMD_ASYNC

ASRCS =
ARTIK_SDK_DIR = $(TOPDIR)/../external/artik-sdk
API_DIR = api
CFLAGS += -D__TIZENRT__
CFLAGS += -Wno-error=strict-prototypes
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/base
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/systemio
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/connectivity
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/wifi
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/lwm2m
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/mqtt
CFLAGS += -I$(ARTIK_SDK_DIR)/inc/coap
CFLAGS += -I$(TOPDIR)/net
CFLAGS += -I$(APPDIR)/include
CFLAGS += -I$(APPDIR)/include/netutils
CSRCS += command.c
CSRCS += wifi-auto.c

ifeq ($(CONFIG_ARTIK_IDE_INTERGRATE),y)
CSRCS += ._main.c examples-api.c
endif

ifeq ($(CONFIG_EXAMPLES_ARTIK_ADC),y)
  TASH_APPS += adc
  MAINSRC += adc-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_CLOUD), y)
  THREADEXEC_cloud = TASH_EXECMD_SYNC
  TASH_APPS += cloud
  MAINSRC += cloud-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_GPIO), y)
  TASH_APPS += gpio
  MAINSRC += gpio-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_HTTP), y)
  TASH_APPS += http
  MAINSRC += http-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_MODULE), y)
  TASH_APPS += sdk
  MAINSRC += module-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_PWM), y)
  THREADEXEC_pwm = TASH_EXECMD_SYNC
  TASH_APPS += pwm
  MAINSRC += pwm-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_SECURITY), y)
  TASH_APPS += see
  MAINSRC += security-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_WEBSOCKET), y)
  TASH_APPS += websocket
  MAINSRC += websocket-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_WIFI), y)
  THREADEXEC_wifictl = TASH_EXECMD_SYNC
  TASH_APPS += wifictl
  MAINSRC += wifi-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_MQTT), y)
  THREADEXEC_mqtt = TASH_EXECMD_SYNC
  TASH_APPS += mqtt
  MAINSRC += mqtt-api.c
endif
ifeq ($(CONFIG_EXAMPLES_ARTIK_COAP), y)
  THREADEXEC_coap = TASH_EXECMD_SYNC
  TASH_APPS += coap
  MAINSRC += coap-api.c
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

ifeq ($(CONFIG_BUILTIN_APPS),y)
define REGISTER_FOREACH
$(BUILTIN)$(DELIM)$2.bdat: $(DEFCONFIG) Makefile
	$(call REGISTER,$1,$2,$3)
endef

$(foreach var,$(TASH_APPS),$(eval $(call REGISTER_FOREACH,$(var),$(var)_main,$(if $(THREADEXEC_$(var)), $(THREADEXEC_$(var)), $(THREADEXEC)))))

TASH_APPS_TARGET=$(addprefix $(BUILTIN)$(DELIM),$(addsuffix _main.bdat,$(TASH_APPS)))
context: $(TASH_APPS_TARGET)

else
context:
endif

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call DELFILE, $(wildcard $(API_DIR)/*.o))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
-include Make.dep
.PHONY: preconfig
	preconfig:
