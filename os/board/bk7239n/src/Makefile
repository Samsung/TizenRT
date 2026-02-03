###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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
#
#   Copyright (C) 2020 Gregory Nutt. All rights reserved.
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
include $(TOPDIR)/tools/Config.mk
-include $(TOPDIR)/Make.defs

DEPPATH = --dep-path .
VPATH = .
ASRCS =

DEPPATH += --dep-path $(TOPDIR)/board/common
VPATH += :$(TOPDIR)/board/common

ifeq ($(CONFIG_FLASH_PARTITION),y)
CSRCS += partitions.c
endif

ifeq ($(CONFIG_BOARD_CRASHDUMP),y)
CSRCS += crashdump.c
endif

# boardctl support
ifeq ($(CONFIG_LIB_BOARDCTL),y)
DEPPATH += --dep-path $(TOPDIR)/arch
VPATH += :$(TOPDIR)/arch
CSRCS += boardctl.c
endif


ARCH_NAME:=$(subst ",,$(CONFIG_ARCH))
ARCH_BOARD_NAME:=$(subst ",,$(CONFIG_ARCH_BOARD))

ARCH_SRCDIR = $(TOPDIR)/arch/$(ARCH_NAME)/src
BOARD_SRCDIR = $(TOPDIR)/board
DIR = 
PROJ_DIR = 
COMPONENT_DIR = components
MIDDLEWARE_DIR = middleware

submodules := middleware
# submodules += components/wpa_supplicant


include Make.defs
include components/bk_pm/Make.defs
include components/bk_startup/Make.defs
include components/bk_system/Make.defs
include components/bk_rtos/Make.defs
include components/bk_init/Make.defs
include components/os_source/Make.defs
include components/temp_detect/Make.defs
include components/bk_ate/Make.defs
include components/bk_cli/Make.defs
include components/cmsis/CMSIS_5/Device/Beken/bk7239xx/Source/Make.defs
include components/bk_wifi/src/Make.defs
include components/bk_phy/src/Make.defs
include components/bk_event/Make.defs
include components/wpa_supplicant-2.10/bk_patch/Make.defs
include components/wpa_supplicant-2.10/hostapd/Make.defs
include components/wpa_supplicant-2.10/src/ap/Make.defs
include components/wpa_supplicant-2.10/src/common/Make.defs
include components/wpa_supplicant-2.10/src/crypto/Make.defs
include components/wpa_supplicant-2.10/src/drivers/Make.defs
include components/wpa_supplicant-2.10/src/l2_packet/Make.defs
include components/wpa_supplicant-2.10/src/rsn_supp/Make.defs
include components/wpa_supplicant-2.10/src/utils/Make.defs
include components/wpa_supplicant-2.10/wpa_supplicant/Make.defs
include bsp_adapter/src/net/Make.defs
include components/bk_common/Make.defs
include components/easy_flash/Make.defs

ifeq ($(CONFIG_PSA_MBEDTLS),y)
include components/psa_mbedtls/Make.defs
endif

ifeq ($(CONFIG_TFM),y)
include components/tfm/Make.defs
endif
CFLAGS += -DBK_SUPPLICANT=1
CFLAGS += -DBK_MAC=1



ifeq ($(CONFIG_ARMINO_BLE),y)
include components/bk_bluetooth/Make.defs
include bsp_adapter/src/ble/Make.defs
endif

ifeq ($(CONFIG_ARMINO_WIFI),y)

endif




DIR_OSDEP = $(BOARD_SRCDIR)/$(ARCH_BOARD_NAME)/src/components/os/os_dep
DIR_OS = $(BOARD_SRCDIR)/$(ARCH_BOARD_NAME)/src/components/os
DIR_MBED = 
DIR_CRYPTO = 

# =============================================================================
# Optimized Include Paths - Grouped by functionality to reduce CFLAG length
# =============================================================================

# Core TizenRT includes
CFLAGS += -I$(BOARD_SRCDIR)/$(ARCH_BOARD_NAME)/src/include
CFLAGS += -Iinclude
CFLAGS += -I$(TOPDIR)/arch/$(ARCH_NAME)/src/armv8-m


# CMSIS includes (consolidated)
CMSIS_BASE = $(COMPONENT_DIR)/cmsis/CMSIS_5
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/bk7239xx/Include
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/bk7239xx/Include/Template
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/armstar
CFLAGS += -I$(CMSIS_BASE)/CMSIS/Core/Include
CFLAGS += -I$(CMSIS_BASE)/CMSIS/NN/Include

# BK Components includes (consolidated)
CFLAGS += -I$(COMPONENT_DIR)/bk_pm/include
CFLAGS += -I$(COMPONENT_DIR)/bk_ps/include
CFLAGS += -I$(COMPONENT_DIR)/bk_rtos/include
CFLAGS += -I$(COMPONENT_DIR)/bk_common/include
CFLAGS += -I$(COMPONENT_DIR)/temp_detect
CFLAGS += -I$(COMPONENT_DIR)/bk_init/include
CFLAGS += -I$(COMPONENT_DIR)/bk_wifi/include/bk_private
CFLAGS += -I$(COMPONENT_DIR)/bk_wifi/include
CFLAGS += -I$(COMPONENT_DIR)/bk_startup/tizen
CFLAGS += -I$(COMPONENT_DIR)/bk_phy/include

# OS Source includes (consolidated)
CFLAGS += -I$(COMPONENT_DIR)/os_source/tizen/include

# Easy Flash includes (consolidated)
CFLAGS += -I$(COMPONENT_DIR)/easy_flash/
CFLAGS += -I$(COMPONENT_DIR)/easy_flash/easy_flash_V4.X/inc/


# Additional component includes (removed duplicates)
CFLAGS += -I$(COMPONENT_DIR)/bk_event
CFLAGS += -Ibsp_adapter/include

ifeq ($(CONFIG_PSA_MBEDTLS),y)
CFLAGS += -I$(COMPONENT_DIR)/psa_mbedtls/mbedtls/include
CFLAGS += -I$(COMPONENT_DIR)/psa_mbedtls/mbedtls/library
CFLAGS += -I$(COMPONENT_DIR)/psa_mbedtls/mbedtls_port/inc
CFLAGS += -I$(COMPONENT_DIR)/psa_mbedtls/mbedtls_port/configs
endif


# Middleware includes (consolidated)
# SOC includes
CFLAGS += -I$(MIDDLEWARE_DIR)/soc/bk7239n/hal
CFLAGS += -I$(MIDDLEWARE_DIR)/soc/common/soc/include
CFLAGS += -I$(MIDDLEWARE_DIR)/soc/bk7239n/soc
CFLAGS += -I$(MIDDLEWARE_DIR)/soc/common/hal/include
# CFLAGS += -I$(MIDDLEWARE_DIR)/soc/bk7239n

# Driver includes (consolidated)
CFLAGS += -I$(MIDDLEWARE_DIR)/driver
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/common
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/include
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/include/bk_private
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/include/bk_private/legacy
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/pmu
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/sys_ctrl
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/wdt
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/reset_reason
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/saradc
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/security
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/uart
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/bk7239n

CFLAGS += -I$(MIDDLEWARE_DIR)/driver/gpio
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/timer
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/io_matrix/v2.0/
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/flash
CFLAGS += -I$(MIDDLEWARE_DIR)/driver/deep_lv

# Architecture includes
CFLAGS += -I$(MIDDLEWARE_DIR)/arch/cm33/include

# Board specific includes
CFLAGS += -I$(MIDDLEWARE_DIR)/boards/bk7239n/partitions
CFLAGS += -I$(MIDDLEWARE_DIR)/boards/bk7239n/vnd_cal/include




# Local includes
CFLAGS += -Iinclude/driver
CFLAGS += -Iinclude/driver/hal

# Network includes
CFLAGS += -I$(TOPDIR)/net/netmgr



COBJS = $(CSRCS:.c=$(OBJEXT))
SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

# Architecture and board includes
ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
#   CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(ARCH_BOARD_NAME)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/common
endif



all: libboard$(LIBEXT)
.PHONY: properties tfm modules submodules_depend secure_deployment py_libs

modules: tfm
	$(Q) set -e; for module in $(submodules); do \
		$(MAKE) -C $$module TOPDIR="$(TOPDIR)"; \
	done

ifeq ($(CONFIG_XIP_KERNEL), y)
tfm_firmware_dir := $(abspath components/tfm/tfm_xip_build)
else
tfm_firmware_dir := $(abspath components/tfm//tfm_loadable_build)
endif

security_h := $(tfm_firmware_dir)/_build/security.h
middleware_path := $(abspath ./middleware/boards/bk7239n)

PRE_HANDLE_SCRIPT := $(abspath $(TOPDIR)/../build/tools/armino/misc/tfm_prehandle.py)
TOOL_PATH := $(abspath $(TOPDIR)/../build/tools/armino)
PY_LIBS_DIR := $(TOOL_PATH)/bk_py_libs
PY_LIBS_TGZ := $(TOOL_PATH)/bk_py_libs.tgz

# Flash encrypt enable: read from config if available, otherwise default to 0
# This parameter controls whether to modify security.csv to set flash_aes_type to FIXED
FLASH_ENCRYPT_ENABLE := $(if $(CONFIG_FLASH_ENCRYPT),1,0)

# py_libs target: ensure Python environment is ready
py_libs:
	@if [ ! -d $(PY_LIBS_DIR) ]; then \
		if [ -f $(PY_LIBS_TGZ) ]; then \
			tar -zxvf $(PY_LIBS_TGZ) -C $(TOOL_PATH) || true; \
			find $(PY_LIBS_DIR) -type d -exec chmod a+wx {} \; 2>/dev/null || true; \
		else \
			echo "[WARNING] $(PY_LIBS_TGZ) not found, Python libraries may be missing"; \
		fi \
	fi

$(security_h): py_libs
	$(Q) mkdir -p $(tfm_firmware_dir)/_build
	$(Q) PYTHONPATH=$(PY_LIBS_DIR):$$PYTHONPATH python3 -B $(PRE_HANDLE_SCRIPT) $(middleware_path) $(tfm_firmware_dir)/_build $(FLASH_ENCRYPT_ENABLE)

ifeq ($(CONFIG_TFM),y)
ifeq ($(wildcard components/tfm/tfm/CMakeLists.txt),)
tfm_prebuild: $(security_h)
else
tfm_prebuild: 
endif
else
tfm_prebuild: 
endif

tfm: tfm_prebuild
ifeq ($(CONFIG_TFM),y)
ifneq ($(wildcard components/tfm/tfm/CMakeLists.txt),)
	$(Q) $(MAKE) -C components/tfm TOPDIR="$(TOPDIR)"
endif
	$(Q) if [ -d "$(tfm_firmware_dir)" ]; then \
		cp -rf "$(tfm_firmware_dir)" "$(TOPDIR)/../build/output/bin/tfm_build"; \
	else \
		echo "tfm_firmware_dir $(tfm_firmware_dir) directory not found, please run 'make tfm' first	"; \
	fi
endif

secure_deployment:
ifeq ($(CONFIG_SECURE_DEPLOYMENT),y)
	$(Q) cd components/secure_deployment && ./build.sh
endif

properties_file := properties/Makefile
properties:
ifneq ($(wildcard $(properties_file)),)
	echo "properties file exist"
	$(Q) $(MAKE) -C properties TOPDIR="$(TOPDIR)" BSPDIR="$(CURDIR)"
endif

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

libboard$(LIBEXT): $(OBJS) tfm properties modules secure_deployment
	$(call ARCHIVE, $@, $(OBJS))

submodules_depend:
	$(Q) for module in $(submodules); do \
		$(MAKE) -C $$module TOPDIR="$(TOPDIR)" depend; \
	done
	ifneq ($(wildcard $(properties_file)),)
	$(Q) $(MAKE) -C properties TOPDIR="$(TOPDIR)" depend
	endif

CDEPFLAGS := $(subst ",\",$(CFLAGS))
.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CDEPFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend submodules_depend

clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)
	$(call DELFILE, $(OBJS))
	$(Q) $(MAKE) -C components/tfm TOPDIR="$(TOPDIR)" clean
ifneq ($(wildcard $(properties_file)),)
	$(Q) $(MAKE) -C properties TOPDIR="$(TOPDIR)" clean
endif
	$(Q) for module in $(submodules); do \
		$(MAKE) -C $$module TOPDIR="$(TOPDIR)" clean; \
	done

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(Q) rm -rf $(PY_LIBS_DIR)

-include Make.dep
#include $(TOPDIR)/configs/Board.mk
