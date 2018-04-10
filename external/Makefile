############################################################################
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
############################################################################
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
VPATH = .
ASRCS =
CSRCS =
CXXSRCS =

# External Directories

# BUILDIRS is the list of top-level directories containing Make.defs files

BUILDIRS   := $(dir $(wildcard */Make.defs))

# CONFIGURED_EXT is the external directories that should be built in
#   the current configuration.

CONFIGURED_EXT =

define Add_EXTLIB
  include $(1)Make.defs
endef

$(foreach BDIR, $(BUILDIRS), $(eval $(call Add_EXTLIB,$(BDIR))))

BIN	= libexternal$(LIBEXT)

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cpp=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)

# Build targets

all: $(BIN)
.PHONY: context .depdirs depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

define SDIR_template
$(1)_$(2):
	$(Q) $(MAKE) -C $(1) $(2) TOPDIR="$(TOPDIR)" EXTDIR="$(EXTDIR)"

endef

$(foreach SDIR, $(CONFIGURED_EXT), $(eval $(call SDIR_template,$(SDIR),all)))
$(foreach SDIR, $(CONFIGURED_EXT), $(eval $(call SDIR_template,$(SDIR),depend)))
$(foreach SDIR, $(BUILDIRS), $(eval $(call SDIR_template,$(SDIR),clean)))
$(foreach SDIR, $(BUILDIRS), $(eval $(call SDIR_template,$(SDIR),distclean)))

$(BIN): $(OBJS) $(foreach SDIR, $(CONFIGURED_EXT), $(SDIR)_all) iotivity_build iotjs_build
	$(call ARCHIVE, $@, $(OBJS))

.depdirs: $(foreach SDIR, $(CONFIGURED_EXT), $(SDIR)_depend)

.depend: Makefile .depdirs
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(ASRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(CSRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean: $(foreach SDIR, $(BUILDIRS), $(SDIR)_clean) iotivity_clean iotjs_clean
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: $(foreach SDIR, $(BUILDIRS), $(SDIR)_distclean)
	$(call DELFILE, .depend)
	$(call DELFILE, Make.dep)
	$(call DELFILE, $(BIN))
	$(call CLEAN)

ifeq ($(CONFIG_ENABLE_IOTIVITY),y)
IOTIVITY_RELEASE=${shell echo $(CONFIG_IOTIVITY_RELEASE_VERSION) | sed 's/"//g'}
IOTIVITY_BASE_DIR=$(EXTDIR)/iotivity/iotivity_$(IOTIVITY_RELEASE)
endif

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

ifeq ($(CONFIG_ENABLE_IOTJS),y)
IOTJS_ROOT_DIR ?= $(EXTDIR)/iotjs
IOTJS_BUILD_OPTION ?=
ifeq ($(CONFIG_DEBUG),y)
  IOTJS_BUILDTYPE = debug
  IOTJS_LIB_DIR = $(IOTJS_ROOT_DIR)/build/arm-tizenrt/debug/lib
else
  IOTJS_BUILDTYPE = release
  IOTJS_LIB_DIR = $(IOTJS_ROOT_DIR)/build/arm-tizenrt/release/lib
endif
endif

iotjs_build:
ifeq ($(CONFIG_ENABLE_IOTJS),y)
	python $(IOTJS_ROOT_DIR)/tools/build.py --target-arch=$(CONFIG_ARCH) --target-os=tizenrt \
	--sysroot=$(TOPDIR) --target-board=$(CONFIG_ARCH_BOARD) --jerry-heaplimit=$(CONFIG_IOTJS_JERRY_HEAP) \
	--buildtype=$(IOTJS_BUILDTYPE) --no-init-submodule $(IOTJS_BUILD_OPTION)
endif

iotjs_clean:
ifeq ($(CONFIG_ENABLE_IOTJS),y)
	$(Q) rm -rf $(IOTJS_ROOT_DIR)/build
endif

-include Make.dep
