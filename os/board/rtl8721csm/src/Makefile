###########################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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
include component/soc/realtek/amebad/app/monitor/ram/Make.defs
include component/soc/realtek/amebad/app/monitor/rom/Make.defs
include component/soc/realtek/amebad/fwlib/crypto/Make.defs
include component/soc/realtek/amebad/fwlib/ram_hp/Make.defs
include component/soc/realtek/amebad/fwlib/ram_common/Make.defs
include component/soc/realtek/amebad/fwlib/usrcfg/Make.defs
include component/common/drivers/wlan/realtek/src/core/option/Make.defs

ifeq ($(CONFIG_FTL_ENABLED),y)
include component/common/file_system/ftl/Make.defs
endif

ifeq ($(CONFIG_AMEBAD_BLE),y)
include component/common/bluetooth/realtek/sdk/board/amebad/src/Make.defs
include component/common/bluetooth/realtek/sdk/board/amebad/src/data_uart/Make.defs
include component/common/bluetooth/realtek/sdk/board/amebad/src/hci/Make.defs
include component/common/bluetooth/realtek/sdk/board/amebad/src/vendor_cmd/Make.defs
include component/common/bluetooth/realtek/sdk/board/common/os/tizenrt/Make.defs
include component/common/bluetooth/realtek/sdk/board/common/src/Make.defs
include component/common/bluetooth/realtek/sdk/src/ble/profile/client/Make.defs
include component/common/bluetooth/realtek/sdk/src/ble/profile/server/Make.defs
ifeq ($(CONFIG_AMEBAD_BLE_PERIPHERAL),y)
include component/common/bluetooth/realtek/sdk/example/ble_peripheral/Make.defs
endif
ifeq ($(CONFIG_AMEBAD_BLE_CENTRAL),y)
include component/common/bluetooth/realtek/sdk/example/ble_central/Make.defs
endif
ifeq ($(CONFIG_AMEBAD_BLE_SCATTERNET),y)
include component/common/bluetooth/realtek/sdk/example/ble_scatternet/Make.defs
endif
endif

ifeq ($(CONFIG_AMEBAD_WIFI),y)
include component/common/api/wifi/Make.defs
include component/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant/Make.defs
include component/common/api/Make.defs
include component/common/api/network/src/Make.defs
include component/os/os_dep/Make.defs
include component/os/tizenrt/Make.defs
include component/soc/realtek/amebad/misc/Make.defs
endif
include component/common/mbed/targets/hal/rtl8721d/Make.defs
ifeq ($(CONFIG_USBD_AUDIO),y)
include component/soc/realtek/amebad/fwlib/usb_otg/device/class/audio/src/Make.defs
endif
ifeq ($(CONFIG_USBD_CDC_ACM),y)
include component/soc/realtek/amebad/fwlib/usb_otg/device/class/cdc_acm/src/Make.defs
endif
ifeq ($(CONFIG_USBD_MSC),y)
include component/soc/realtek/amebad/fwlib/usb_otg/device/class/msc/src/Make.defs
endif

ARCH_SRCDIR = 	$(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_SRCDIR = 	$(TOPDIR)/board
DIR = 		$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/app/monitor
PROJ_DIR = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/project/realtek_amebaD_va0_example
COMPONENT_DIR =	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component
DIR_WIFI = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/common/api/wifi
ifeq ($(CONFIG_AMEBAD_BLE),y)
DIR_BT = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/common/bluetooth/realtek/sdk
endif
DIR_OSDEP = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os/os_dep
DIR_OS = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os
DIR_MBED = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/common/mbed/targets/hal/rtl8721d
DIR_CRYPTO = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/fwlib/crypto
DIR_FW = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/fwlib/ram_hp
DIR_COM = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/fwlib/ram_common
CFGDIR = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/fwlib/usrcfg
DIR_OTG = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/fwlib/usb_otg/device

CFLAGS += -I$(DIR)/../include
CFLAGS += -I$(TOPDIR)/board/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/swlib/include
CFLAGS += -I$(TOPDIR)/board/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/swlib/string
CFLAGS += -I$(TOPDIR)/board/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/app/monitor/include
CFLAGS += -I$(TOPDIR)/board/$(CONFIG_ARCH_BOARD)/src/component/soc/realtek/amebad/app/xmodem
CFLAGS += -I$(TOPDIR)/arch/$(CONFIG_ARCH)/src/armv8-m
CFLAGS += -I$(PROJ_DIR)/inc/inc_hp
CFLAGS += -I$(COMPONENT_DIR)/soc/realtek/amebad/cmsis
CFLAGS += -I$(COMPONENT_DIR)/common/mbed/hal
CFLAGS += -I$(COMPONENT_DIR)/common/mbed/hal_ext
CFLAGS += -I$(COMPONENT_DIR)/common/mbed/api
CFLAGS += -I$(COMPONENT_DIR)/common/mbed/targets/hal/rtl8721d
CFLAGS += -I$(COMPONENT_DIR)/common/api/platform
CFLAGS += -I$(COMPONENT_DIR)/common/api
CFLAGS += -I$(COMPONENT_DIR)/common/api/wifi/rtw_wpa_supplicant/wpa_supplicant
CFLAGS += -I$(COMPONENT_DIR)/common/drivers/wlan/realtek/include
CFLAGS += -I$(COMPONENT_DIR)/common/drivers/wlan/realtek/src/osdep
CFLAGS += -I$(COMPONENT_DIR)/common/drivers/wlan/realtek/wlan_ram_map/rom
CFLAGS += -I$(COMPONENT_DIR)/os/os_dep/include
CFLAGS += -I$(COMPONENT_DIR)/os/tizenrt
CFLAGS += -I$(DIR_WIFI)/rtw_wpa_supplicant/src
CFLAGS += -I$(COMPONENT_DIR)/soc/realtek/amebad/misc
CFALGS += -I$(DIR_CRYPTO)/../include
CFLAGS += -I$(DIR_FW)/../include
CFLAGS += -I$(TOPDIR)/net/netmgr
ifeq ($(CONFIG_AMEBAD_BLE),y)
CFLAGS += -I$(DIR_BT)/inc
CFLAGS += -I$(DIR_BT)/src/mcu/module/data_uart_cmd
CFLAGS += -I$(DIR_BT)/example/ble_peripheral
CFLAGS += -I$(DIR_BT)/example/ble_central
CFLAGS += -I$(DIR_BT)/example/ble_scatternet
endif

ifeq ($(CONFIG_USBD_MSC),y)
CFLAGS += -I$(BASEDIR)/component/common/drivers/sdio/realtek/sdio_host/inc
endif
CFLAGS += -I$(DIR_OTG)/core/inc
CFLAGS += -I$(DIR_OTG)/composite/inc

CFLAGS += -DARM_CORE_CM4

COBJS = $(CSRCS:.c=$(OBJEXT))
SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src
endif

all: libboard$(LIBEXT)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

libboard$(LIBEXT): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
#include $(TOPDIR)/configs/Board.mk
