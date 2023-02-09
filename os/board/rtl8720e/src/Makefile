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
include component/soc/amebalite/app/monitor/ram/Make.defs
include component/soc/amebalite/fwlib/ram_km4/Make.defs
include component/soc/amebalite/fwlib/ram_common/Make.defs
include component/soc/amebalite/fwlib/usrcfg/Make.defs
include component/lwip/api/Make.defs
include component/wifi/Make.defs
include component/wifi/inic/Make.defs
include component/wifi/driver/src/core/option/Make.defs
include component/wifi/driver/src/osdep/freertos/Make.defs
include component/ssl/mbedtls_ram_map/rom/Make.defs
include component/mbed/targets/hal/rtl8720e/Make.defs
include component/os/os_dep/Make.defs
include component/os/tizenrt/Make.defs

ifeq ($(CONFIG_AMEBALITE_WIFI),y)
include component/wifi/api/Make.defs
include component/wifi/promisc/Make.defs
include component/wifi/wpa_supplicant/wpa_supplicant/Make.defs
include component/soc/amebalite/misc/Make.defs
endif

ifeq ($(CONFIG_AMEBALITE_BLE),y)
include component/bluetooth/api/Make.defs
include component/bluetooth/api/ipc/Make.defs
include component/bluetooth/os/tizenrt/Make.defs
include component/bluetooth/example/utils/Make.defs
ifeq ($(CONFIG_AMEBALITE_BLE_PERIPHERAL),y)
include component/bluetooth/example/ble_peripheral/Make.defs
endif
ifeq ($(CONFIG_AMEBALITE_BLE_CENTRAL),y)
include component/bluetooth/example/ble_central/Make.defs
include component/bluetooth/profile/client/Make.defs
endif
ifeq ($(CONFIG_AMEBALITE_BLE_SCATTERNET),y)
include component/bluetooth/example/ble_scatternet/Make.defs
endif
endif

ARCH_SRCDIR = 	$(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_SRCDIR = 	$(TOPDIR)/board
PROJ_DIR = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/project/realtek_amebaLite_va0_example
COMPONENT_DIR = $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component
DIR_WIFI = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/wifi
DIR_OSDEP = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os/os_dep
DIR_OS = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/os
DIR_MBED = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/mbed/targets/hal/rtl8720e
ifeq ($(CONFIG_AMEBALITE_BLE),y)
DIR_BT = 	$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src/component/bluetooth
endif

TARGETDIR = 	$(COMPONENT_DIR)/soc/amebalite

CFLAGS += -I$(DIR)/../include
CFLAGS += -I$(TOPDIR)/arch/$(CONFIG_ARCH)/src/armv8-m
CFLAGS += -I$(PROJ_DIR)/inc/inc_km4
CFLAGS += -I$(TARGETDIR)/bootloader
CFLAGS += -I$(TARGETDIR)/cmsis
CFLAGS += -I$(TARGETDIR)/cmsis-dsp/Include
CFLAGS += -I$(TARGETDIR)/cmsis-dsp/PrivateInclude
CFLAGS += -I$(TARGETDIR)/fwlib/include
CFLAGS += -I$(TARGETDIR)/app/monitor/include
CFLAGS += -I$(TARGETDIR)/app/xmodem
CFLAGS += -I$(TARGETDIR)/swlib
CFLAGS += -I$(TARGETDIR)/misc
CFLAGS += -I$(COMPONENT_DIR)/stdlib

CFLAGS += -I$(COMPONENT_DIR)/wifi
CFLAGS += -I$(COMPONENT_DIR)/wifi/api
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/include
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/osdep
CFLAGS += -I$(COMPONENT_DIR)/wifi/inic
CFLAGS += -I$(COMPONENT_DIR)/wifi/wifi_config
CFLAGS += -I$(COMPONENT_DIR)/wifi/wifi_fw/amebalite/include
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/include
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/hal/phydm
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/hal/phy_g6/bb
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/hal/rtl8720e
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/osdep
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/hal
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/hci
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/phl
CFLAGS += -I$(COMPONENT_DIR)/wifi/driver/src/core/option
CFLAGS += -I$(COMPONENT_DIR)/wifi/wpa_supplicant/src/crypto
CFLAGS += -I$(COMPONENT_DIR)/lwip/api
CFLAGS += -I$(COMPONENT_DIR)/ssl/mbedtls_ram_map/rom

CFLAGS += -I$(COMPONENT_DIR)/os/tizenrt
CFLAGS += -I$(DIR_MBED)
CFLAGS += -I$(DIR_OSDEP)/include
CFLAGS += -I$(DIR_WIFI)/wpa_supplicant/src
CFLAGS += -I$(COMPONENT_DIR)/mbed/api
CFLAGS += -I$(COMPONENT_DIR)/mbed/hal
CFLAGS += -I$(COMPONENT_DIR)/mbed/hal_ext
CFLAGS += -I$(COMPONENT_DIR)/mbed/targets/hal/rtl8720e
CFLAGS += -I$(COMPONENT_DIR)/os/os_dep/include
CFLAGS += -I$(TOPDIR)/net/netmgr

ifeq ($(CONFIG_AMEBALITE_BLE),y)
CFLAGS += -I$(DIR_BT)/inc
CFLAGS += -I$(DIR_BT)/profile/client
endif

ifeq ($(CONFIG_FTL_ENABLED),y)
CFLAGS += -I$(COMPONENT_DIR)/filesystem/ftl
endif

#CFLAGS += -DARM_CORE_CM4

COBJS = $(CSRCS:.c=$(OBJEXT))
SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/armv8-m}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(COMPONENT_DIR)/soc/amebalite/cmsis}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(COMPONENT_DIR)/soc/amebalite/cmsis
  CFLAGS += -I$(BOARD_SRCDIR)/common
  CFLAGS += -I$(ARCH_SRCDIR)/armv8-m
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
