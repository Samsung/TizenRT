############################################################################
# external/Makefile
#
#   Copyright (C) 2007, 2008, 2011-2015 Gregory Nutt. All rights reserved.
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


EXTDIR = ${shell pwd}
-include $(TOPDIR)/Make.defs

DELIM ?= $(strip /)

DEPPATH = --dep-path .
ASRCS =
CSRCS =
VPATH = .

ifeq ($(CONFIG_LWM2M_WAKAAMA),y)
include wakaama/core/Make.defs
include wakaama/core/er-coap-13/Make.defs	
endif

ifeq ($(CONFIG_DM),y)
ifeq ($(CONFIG_LWM2M_CLIENT_MODE),y)
   include wakaama/examples/shared/Make.defs
   include wakaama/examples/client/Make.defs
endif
ifeq ($(CONFIG_LWM2M_SERVER_MODE),y)
    include wakaama/examples/shared/Make.defs
    include wakaama/examples/server/Make.defs
endif
  CFLAGS+=-I$(TOPDIR)/../framework/include/dm
endif

ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR=$(EXTDIR)/iotivity/iotivity_$(IOTIVITY_RELEASE)
endif

ifeq ($(CONFIG_LIBTUV),y)
include libtuv/source/tinyara/Make.defs
include libtuv/source/unix/Make.defs
include libtuv/source/Make.defs
endif

ifeq ($(CONFIG_AWS_SDK),y)
include aws/Make.defs
include aws/src/Make.defs
endif

ifeq ($(CONFIG_ENABLE_IOTJS),y)

ifeq ($(IOTJS_ROOT_DIR),)
    IOTJS_ROOT_DIR = $(EXTDIR)/iotjs
endif

ifeq ($(CONFIG_DEBUG),y)
  IOTJS_BUILDTYPE = debug
  IOTJS_LIB_DIR = $(IOTJS_ROOT_DIR)/build/arm-tizenrt/debug/lib
else
  IOTJS_BUILDTYPE = release
  IOTJS_LIB_DIR = $(IOTJS_ROOT_DIR)/build/arm-tizenrt/release/lib
endif

endif


ifeq ($(CONFIG_NETUTILS_WIFI),y)
include slsi_wifi/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_DHCPD),y)
include dhcpd/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_XMLRPC),y)
include xmlrpc/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_NTPCLIENT),y)
include ntpclient/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_WEBSERVER),y)
include webserver/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_FTPC),y)
include ftpc/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_MDNS),y)
include mdns/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_WEBCLIENT),y)
include webclient/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_MQTT),y)
include mosquitto/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_FTPD),y)
include ftpd/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_DHCPC),y)
include dhcpc/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_WEBSOCKET),y)
include websocket/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_LIBCOAP),y)
include libcoap/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_TFTPC),y)
include tftpc/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_TELNETD),y)
include telnetd/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_SMTP),y)
include smtp/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_NETLIB),y)
include netutils/Make.defs
endif

ifeq ($(CONFIG_NETUTILS_JSON),y)
include json/Make.defs
endif

ifeq ($(CONFIG_NET_SECURITY_TLS),y)
include mbedtls/Make.defs
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cpp=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)

BIN	= libexternal$(LIBEXT)

all:	$(BIN)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

iotivity_build:
ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
	touch $(IOTIVITY_BASE_DIR)/iotivity.built
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/out
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/.sconsign.dblite
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/.sconf_temp
	$(call DELFILE, $(IOTIVITY_BASE_DIR)/config.log)
	$(Q) echo "Launching IoTivity Build"
	$(Q) TOPDIR="$(TOPDIR)" $(TOPDIR)/../external/iotivity/build_iotivity.sh
endif

iotjs_build:
ifeq ($(CONFIG_ENABLE_IOTJS),y)
	python $(IOTJS_ROOT_DIR)/tools/build.py --target-arch=arm --target-os=tizenrt --sysroot=../os --target-board=artik05x --jerry-heaplimit=$(CONFIG_IOTJS_JERRY_HEAP) --buildtype=$(IOTJS_BUILDTYPE) --no-init-submodule 
endif 

$(BIN): $(OBJS) iotivity_build iotjs_build
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(ASRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(CSRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

iotivity_clean:
ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
ifneq ("$(wildcard $(IOTIVITY_BASE_DIR)/iotivity.built)","")
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/iotivity.built
	$(Q) echo "Cleaning IoTivity Build"
	$(Q) TOPDIR="$(TOPDIR)" $(TOPDIR)/../external/iotivity/clean_iotivity.sh
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/out
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/extlibs/mbedtls/mbedtls
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/*.o
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/.sconsign.dblite
	$(Q) rm -rf $(IOTIVITY_BASE_DIR)/.sconf_temp
endif
endif

custom_clean:
	$(foreach OBJFILE, $(OBJS), rm -f $(OBJFILE))

iotjs_clean:
ifeq ($(CONFIG_ENABLE_IOTJS),y)
	$(Q) rm -rf $(IOTJS_ROOT_DIR)/build
endif

clean: custom_clean iotivity_clean iotjs_clean
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
