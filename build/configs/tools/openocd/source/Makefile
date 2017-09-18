###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
# Author: Jihun Ahn <jhun.ahn@samsung.com>
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

include define.mk

WD = $(abspath $(shell pwd))

INSTALLDIR := $(WD)/..
PREFIXDIR := $(WD)/build
SRCDIR := $(WD)/libs

CHOST ?= $(shell gcc -dumpmachine)

define download
	$(Q) $(MAKE) -C $(SRCDIR) \
		url="$(url)" version="$(version)" pkg="$(@)" download
endef

define build
	$(call download)
	$(Q) $(MAKE) -C $(SRCDIR) -f Makefile \
		TOOL="$(TOOL)" ARCH="$(ARCH)" \
		PREFIXDIR="$(PREFIXDIR)" \
		CHOST="$(CHOST)" $(@)
endef

ifneq ($(findstring msys,$(CHOST))$(findstring mingw,$(CHOST)),)
TOOL ?= win
else
ifneq ($(findstring linux,$(CHOST)),)
TOOL ?= linux
else
ifneq ($(findstring darwin,$(CHOST)),)
TOOL ?= macos
else
TOOL :=
endif
endif
endif

ifneq ($(findstring x86_64,$(CHOST)),)
ARCH ?= 64
else
ARCH ?= 32
endif

all: openocd
.PHONY: context libusb libusb-compat-0.1 libconfuse libftdi hidapi openocd

context:
ifeq ($(shell which $(CHOST)-gcc),)
	$(eval CHOST = )
endif
	@echo CHECK: $(@)
	$(Q) ./check.sh || exit 1;

libusb: context
	$(eval url := https://github.com/libusb/libusb.git)
	$(eval version := v1.0.21-rc6)
	$(call build)

libusb-compat-0.1: context libusb
	$(eval url := https://github.com/libusb/libusb-compat-0.1.git)
	$(eval version := v0.1.6-rc2)
	$(call build)

libftdi: context libusb libconfuse
	$(eval url := http://repo.or.cz/libftdi.git)
	$(eval version := v1.4rc2)
	$(call build)

libconfuse: context
	$(eval url := https://github.com/martinh/libconfuse.git)
	$(eval version := v3.2.1)
	$(call build)

hidapi: context libusb
	$(eval url := https://github.com/signal11/hidapi.git)
	$(eval version := hidapi-0.8.0-rc1)
	$(call build)

openocd: context libusb libusb-compat-0.1 libftdi hidapi
	$(eval url := http://repo.or.cz/openocd.git)
	$(eval version := v0.10.0)
	$(call build)

#
# Make install targets
#

.install_macos:
	$(eval IDIR := $(INSTALLDIR)/$(TOOL))

.install_linux:
	$(eval IDIR := $(INSTALLDIR)/$(TOOL)$(ARCH))

.install_win:
	$(eval IDIR := $(INSTALLDIR)/$(TOOL)$(ARCH))
	$(eval EXT = .exe)

install: .install_$(TOOL)
ifeq ("$(wildcard $(IDIR))","")
	@echo IDIR: $(notdir $(IDIR))
	$(Q) if [ ! -e $(IDIR) ]; then mkdir -p $(IDIR); fi
endif
	$(eval FILES += $(PREFIXDIR)/bin/openocd$(EXT))
	$(eval FILES += $(PREFIXDIR)/bin/ftdi_eeprom$(EXT))
	$(foreach f,$(FILES),$(call INSTALL,$(f),$(IDIR)))

	$(call INSTALL,$(PREFIXDIR)/share/openocd/scripts,$(INSTALLDIR))

clean:
	@echo "CLEAN.."
	$(Q) $(MAKE) -C $(SRCDIR) $(@)

distclean: clean
	$(foreach f,$(filter-out *.gitignore,$(wildcard $(PREFIXDIR)/*)),$(call DELFILE, $f))
