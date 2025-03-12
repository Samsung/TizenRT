###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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

-include $(TOPDIR)/Make.defs

DEPPATH = --dep-path .
VPATH = .
ASRCS =

ROOT_DIR = $(shell cd $(TOPDIR)/.. && pwd)
BUILD_DIR = $(ROOT_DIR)/build/tools/amebasmart/gnu_utility
BUILD_EXT_BIN = $(shell python $(ROOT_DIR)/build/tools/amebasmart/gnu_utility/rtk_data_binary.py)
RESOURCE_TARGET_FOLDER = $(ROOT_DIR)/resource/rtl8730e

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

include Make.defs
include component/soc/amebad2/fwlib/ram_common/Make.defs
include component/soc/amebad2/fwlib/ap_peripheral/Make.defs
include component/soc/amebad2/fwlib/ap_core/Make.defs
include component/soc/amebad2/fwlib/rom_common/Make.defs
include component/soc/amebad2/fwlib/ram_hp/Make.defs
include component/soc/amebad2/swlib/Make.defs

include component/usrcfg/rtl8730e/Make.defs
include component/lwip/api/Make.defs
include component/wifi/Make.defs
include component/wifi/inic/Make.defs
include component/wifi/common/Make.defs
include component/mbed/targets/hal/rtl8730e/Make.defs
include component/os/tizenrt/Make.defs
include component/os/os_wrapper/Make.defs

ifeq ($(CONFIG_AMEBASMART_WIFI),y)
include component/wifi/api/Make.defs
include component/wifi/wpa_supplicant/wpa_supplicant/Make.defs
include component/soc/amebad2/misc/Make.defs
endif

ifeq ($(CONFIG_AMEBASMART_BLE),y)
include component/bluetooth/api/Make.defs
include component/bluetooth/os/tizenrt/Make.defs
include component/bluetooth/example/utils/Make.defs
include component/bluetooth/driver/platform/amebad2/hci/Make.defs
include component/bluetooth/api/rtk_stack/Make.defs
include component/bluetooth/rtk_stack/platform/amebad2/src/Make.defs
include component/bluetooth/driver/hci/hci_transport/Make.defs
include component/bluetooth/driver/hci/Make.defs
include component/bluetooth/driver/hci/hci_process/Make.defs
include component/bluetooth/rtk_coex/Make.defs

ifeq ($(CONFIG_AMEBASMART_BLE_PERIPHERAL),y)
include component/bluetooth/example/ble_peripheral/Make.defs
endif
ifeq ($(CONFIG_AMEBASMART_BLE_CENTRAL),y)
include component/bluetooth/example/ble_central/Make.defs
include component/bluetooth/profile/client/Make.defs
endif
ifeq ($(CONFIG_AMEBASMART_BLE_SCATTERNET),y)
include component/bluetooth/example/ble_scatternet/Make.defs
endif
endif

ifeq ($(CONFIG_AMEBASMART_USBDEVICE),y)
include component/usb/common_new/Make.defs
include component/usb/device_new/cdc_acm/Make.defs
include component/usb/device_new/core/Make.defs
endif

include component/file_system/ftl/Make.defs
include component/file_system/kv/Make.defs
include component/file_system/vfs2.0/Make.defs

ARCH_SRCDIR = 	$(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_SRCDIR = 	$(TOPDIR)/board
PROJ_DIR = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/project/realtek_amebaD2_va0_example
COMPONENT_DIR = $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component
DIR_WIFI = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/wifi
DIR_OSDEP = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os/os_wrapper
DIR_OS = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os
DIR_MBED = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/mbed/targets/hal/rtl8730e
ifeq ($(CONFIG_AMEBASMART_BLE),y)
DIR_BT = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/bluetooth
endif
ifeq ($(CONFIG_AMEBASMART_USBDEVICE),y)
DIR_USB = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/usb
endif

TARGETDIR = 	$(COMPONENT_DIR)/soc/amebad2

CFLAGS += -I$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/include
CFLAGS += -I$(TOPDIR)/arch/$(CONFIG_ARCH)/src/armv7-a
CFLAGS += -I$(PROJ_DIR)/inc/inc_ap
CFLAGS += -I$(TARGETDIR)/bootloader
CFLAGS += -I$(TARGETDIR)/fwlib/include
CFLAGS += -I$(TARGETDIR)/fwlib/ap_core
CFLAGS += -I$(TARGETDIR)/app/monitor/include
CFLAGS += -I$(TARGETDIR)/swlib
CFLAGS += -I$(TARGETDIR)/misc
CFLAGS += -I$(COMPONENT_DIR)/stdlib
CFLAGS += -I$(COMPONENT_DIR)/usrcfg/rtl8730e/include

CFLAGS += -I$(COMPONENT_DIR)/wifi/api
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/include
CFLAGS += -I$(COMPONENT_DIR)/wifi/inic
CFLAGS += -I$(COMPONENT_DIR)/wifi/wpa_supplicant/src/crypto
CFLAGS += -I$(COMPONENT_DIR)/lwip/api

CFLAGS += -I$(COMPONENT_DIR)/os/os_wrapper/include
CFLAGS += -I$(COMPONENT_DIR)/os/tizenrt
CFLAGS += -I$(DIR_MBED)
CFLAGS += -I$(DIR_OSDEP)/include
CFLAGS += -I$(DIR_WIFI)/wpa_supplicant/wpa_lite
CFLAGS += -I$(COMPONENT_DIR)/mbed/api
CFLAGS += -I$(COMPONENT_DIR)/mbed/hal
CFLAGS += -I$(COMPONENT_DIR)/mbed/hal_ext
CFLAGS += -I$(COMPONENT_DIR)/mbed/targets/hal/rtl8730e
CFLAGS += -I$(TOPDIR)/net/netmgr
# Temporary keep it, need further check whether the usb needs the filesystem
CFLAGS += -I$(COMPONENT_DIR)/file_system/vfs2.0
CFLAGS += -I$(COMPONENT_DIR)/file_system/ftl
CFLAGS += -I$(COMPONENT_DIR)/file_system/kv

ifeq ($(CONFIG_AMEBASMART_BLE),y)
CFLAGS += -I$(DIR_BT)/inc
CFLAGS += -I$(DIR_BT)/profile/client
CFLAGS += -I$(DIR_BT)/driver/inc
CFLAGS += -I$(DIR_BT)/driver/inc/hci
CFLAGS += -I$(DIR_BT)/api/rtk_stack
CFLAGS += -I$(DIR_BT)/rtk_stack/inc
CFLAGS += -I$(DIR_BT)/rtk_stack/platform/amebad2/inc
endif

COBJS = $(CSRCS:.c=$(OBJEXT))
SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(COMPONENT_DIR)/soc/amebad2/cmsis}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(COMPONENT_DIR)/soc/amebad2/cmsis
  CFLAGS += -I$(BOARD_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src
  CFLAGS += -I$(TOPDIR)/arch/$(CONFIG_ARCH)/src/armv7-a
  CFLAGS += -I$(TARGETDIR)/bootloader
  CFLAGS += -I$(TOPDIR)/se/ameba
  
endif

all: libboard$(LIBEXT)

$(AOBJS): %$(OBJEXT): %$(ASMEXT)
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

libboard$(LIBEXT): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))
ifeq ($(CONFIG_RTK_DATA_BINARY_TO_EXT_FLASH),y)
	$(call build_ext_bin)
else
	@echo "========== Current config does not support external flash! =========="
endif

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

define build_ext_bin
	$(Q) $(shell python $(BUILD_DIR)/rtk_data_binary.py)
	@if [ -d $(RESOURCE_TARGET_FOLDER) ]; then echo "$(RESOURCE_TARGET_FOLDER) exist"; else $(MKDIR) -p $(RESOURCE_TARGET_FOLDER); fi
	$(Q) mv -f $(BUILD_DIR)/rtk_ext_flash_data.bin $(RESOURCE_TARGET_FOLDER)/
	@echo "========== Current config support external flash, binary created and moved to path /resource/rtl8730e/rtk_ext_flash_data.bin! =========="
endef

clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)
ifeq ($(CONFIG_RTK_DATA_BINARY_TO_EXT_FLASH),y)
	$(call DELDIR, $(RESOURCE_TARGET_FOLDER))
endif

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
#include $(TOPDIR)/configs/Board.mk
