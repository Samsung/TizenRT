############################################################################
# apps/examples/ocf_mylight/Makefile
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

# built-in application info

APPNAME = ocf_mylight
THREADEXEC = TASH_EXECMD_SYNC

# Example
ASRCS =
CSRCS = ocf_mylight_device.c \
	ocf_mylight_light.c \
	ocf_mylight_verbose.c \
	ocf_mylight_platform.c \
	ocf_mylight_security.c \
	ocf_mylight_userinput.c \
	ocf_mylight_notify.c \
	ocf_mylight_default_db.c \
	ocf_mylight_wifi.c \
	ocf_mylight_ntp.c \
	ocf_mylight_crudn.c \
	ocf_mylight_setup.c

ifeq ($(CONFIG_LIBC_LOCALTIME),y)
CSRCS += ocf_mylight_tz.c
endif
ifeq ($(CONFIG_EXAMPLES_OCFMYLIGHT_DIMMER),y)
CSRCS += ocf_mylight_dimm.c
endif

MAINSRC = ocf_mylight_main.c

ifeq ($(CONFIG_IOTIVITY_RELEASE_VERSION_1_2),y)
IOTIVITY_BASE_DIR = $(APPDIR)/../external/iotivity/iotivity_1.2-rel
else
ifeq ($(CONFIG_IOTIVITY_RELEASE_VERSION_1_3),y)
IOTIVITY_BASE_DIR = $(APPDIR)/../external/iotivity/iotivity_1.3-rel
endif
endif

IOTIVITY_TARGET_ARCH=${shell echo $(CONFIG_ARCH_FAMILY) | sed 's/"//g'}
ifeq ($(CONFIG_IOTIVITY_RELEASE),y)
IOTIVITY_OUT_DIR=$(IOTIVITY_BASE_DIR)/out/tizenrt/$(IOTIVITY_TARGET_ARCH)/release
else
IOTIVITY_OUT_DIR=$(IOTIVITY_BASE_DIR)/out/tizenrt/$(IOTIVITY_TARGET_ARCH)/debug
endif
IOTIVITY_CONFIG_DIR=$(IOTIVITY_OUT_DIR)/include/c_common

CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/api
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/common/inc
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/logger/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/extlibs/mbedtls/mbedtls/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/build_common/tizenrt/compatibility
CFLAGS += -I$(IOTIVITY_CONFIG_DIR)

# json2cbor
CSRCS += json2cbor.c
CFLAGS += -I$(IOTIVITY_BASE_DIR)/extlibs/tinycbor/tinycbor/src
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/oic_malloc/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/oic_string/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/c_common/ocrandom/include
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/stack/include/internal
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/security/include/internal
CFLAGS += -I$(IOTIVITY_BASE_DIR)/resource/csdk/connectivity/lib/libcoap-4.1.1/include
CFLAGS += -DTB_LOG -DMULTIPLE_OWNER -DINLINE_API=inline
CFLAGS += -DPRIi64=\"lld\"

CFLAGS += -DWITH_POSIX -DIP_ADAPTER
CFLAGS += -DWITH_BWT
ifeq ($(CONFIG_IOTIVITY_ROUTING),"EP")
CFLAGS += -DROUTING_EP
else
CFLAGS += -DROUTING_GW
endif

CFLAGS += -D__TINYARA__
CFLAGS += -D__TIZENRT__
CFLAGS += -D__WITH_DTLS__

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

CONFIG_EXAMPLES_OCFMYLIGHT_PROGNAME ?= ocf_mylight$(EXEEXT)
PROGNAME = $(CONFIG_EXAMPLES_OCFMYLIGHT_PROGNAME)

ROOTDEPPATH = --dep-path .

# Common build

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

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_OCFMYLIGHT),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(APPNAME)_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat

else
context:

endif

IOTIVITY_CONFIG_FILE=$(IOTIVITY_CONFIG_DIR)/iotivity_config.h
EXTRA_FILES += $(IOTIVITY_CONFIG_FILE) ocf_mylight_default_db.c json2cbor.c
ifeq ($(CONFIG_LIBC_LOCALTIME),y)
EXTRA_FILES += ocf_mylight_tz.c
endif

$(IOTIVITY_CONFIG_FILE):
	$(Q) if [ ! -e $(IOTIVITY_CONFIG_DIR) ]; then \
		mkdir -p $(IOTIVITY_CONFIG_DIR); \
	fi;
	$(Q) touch $(IOTIVITY_CONFIG_FILE)

ocf_mylight_default_db.c: ./json/generate_c.sh $(TOPDIR)/include/tinyara/config.h
	$(Q) ./json/generate_c.sh $(shell pwd)/json/

ifeq ($(CONFIG_LIBC_LOCALTIME),y)
ocf_mylight_tz.c: ./tz/generate_c.sh
	$(Q) ./tz/generate_c.sh $(shell pwd)/tz/
endif

json2cbor.c:
	$(Q) cp $(IOTIVITY_BASE_DIR)/resource/csdk/security/tool/json2cbor.c .
	$(Q) patch -p1 json2cbor.c < json2cbor.patch

.depend: Makefile $(SRCS) $(EXTRA_FILES)
	@$(MKDEP) $(ROOTDEPPATH) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	"$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)
	@rm -f ocf_mylight_default_db.c
	@rm -f json2cbor.c
ifeq ($(CONFIG_LIBC_LOCALTIME),y)
	@rm -f ocf_mylight_tz.c
endif

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
preconfig:
