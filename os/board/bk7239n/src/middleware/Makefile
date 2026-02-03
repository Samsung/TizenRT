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
# syscall/Makefile
#
#   Copyright (C) 2011-2013 Gregory Nutt. All rights reserved.
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
###########################################################################
-include $(TOPDIR)/.config
include $(TOPDIR)/tools/Config.mk
-include $(TOPDIR)/Make.defs


driver_path := driver
bk7239n_soc_path := soc
bk7239n_hal_path := hal


-include Make.defs

include soc/bk7239n/Make.defs
include $(driver_path)/sys_ctrl/Make.defs
include soc/bk7239n/$(bk7239n_hal_path)/Make.defs
include boards/bk7239n/vnd_cal/Make.defs


libname := middleware
libfile := lib$(libname)$(LIBEXT)
COBJS = $(CSRCS:.c=$(OBJEXT))
OBJS = $(COBJS)

ARCH_SRCDIR = $(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_SRCDIR = $(TOPDIR)/board
BOARD_DIR := ..

# Middleware includes (consolidated)
# SOC includes
CFLAGS += -Isoc/bk7239n/$(bk7239n_hal_path)
CFLAGS += -Isoc/common/soc/include
CFLAGS += -Isoc/bk7239n/$(bk7239n_soc_path)
CFLAGS += -Isoc/common/hal/include
# CFLAGS += -Isoc/bk7239n

# Driver includes (consolidated)
CFLAGS += -I$(driver_path)
CFLAGS += -I$(driver_path)/common
CFLAGS += -I$(driver_path)/include
CFLAGS += -I$(driver_path)/include/bk_private
CFLAGS += -I$(driver_path)/include/bk_private/legacy
CFLAGS += -I$(driver_path)/pmu
CFLAGS += -I$(driver_path)/sys_ctrl
CFLAGS += -I$(driver_path)/wdt
CFLAGS += -I$(driver_path)/reset_reason
CFLAGS += -I$(driver_path)/saradc
CFLAGS += -I$(driver_path)/security
CFLAGS += -I$(driver_path)/uart
CFLAGS += -I$(driver_path)/bk7239n

CFLAGS += -I$(driver_path)/gpio
CFLAGS += -I$(driver_path)/timer
CFLAGS += -I$(driver_path)/io_matrix/v2.0/
CFLAGS += -I$(driver_path)/flash
CFLAGS += -I$(driver_path)/deep_lv

# Architecture includes
CFLAGS += -Iarch/cm33/include

# Board specific includes
CFLAGS += -Iboards/bk7239n/partitions
CFLAGS += -Iboards/bk7239n/vnd_cal/include

# Local includes
CFLAGS += -I$(BOARD_DIR)/include
CFLAGS += -I$(BOARD_DIR)/include/driver
CFLAGS += -I$(BOARD_DIR)/include/driver/hal

COMPONENT_DIR = $(BOARD_DIR)/components
CMSIS_BASE = $(COMPONENT_DIR)/cmsis/CMSIS_5
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/armstar
CFLAGS += -I$(CMSIS_BASE)/CMSIS/NN/Include
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/bk7239xx/Include
CFLAGS += -I$(CMSIS_BASE)/Device/Beken/bk7239xx/Include/Template

CFLAGS += -I$(CMSIS_BASE)/CMSIS/Core/Include
CFLAGS += -I$(COMPONENT_DIR)/bk_rtos/include
CFLAGS += -I$(COMPONENT_DIR)/bk_common/include
CFLAGS += -I$(COMPONENT_DIR)/bk_ps/include
CFLAGS += -I$(COMPONENT_DIR)/bk_phy/include
CFLAGS += -I$(COMPONENT_DIR)/easy_flash
CFLAGS += -I$(COMPONENT_DIR)/temp_detect
CFLAGS += -I$(COMPONENT_DIR)/easy_flash/easy_flash_V4.X/inc
CFLAGS += -I$(COMPONENT_DIR)/bk_pm/include

ifeq ($(CONFIG_XIP_KERNEL), y)
tfm_firmware_dir_name := tfm_xip_build
else
tfm_firmware_dir_name := tfm_loadable_build
endif

# Add the TFM include directories to the build
# CFLAGS += -I$(COMPONENT_DIR)/tfm/tfm/interface/include
# CFLAGS += -I$(COMPONENT_DIR)/tfm/tfm/secure_fw/include
CFLAGS += -I$(COMPONENT_DIR)/tfm/$(tfm_firmware_dir_name)/tfm_install/interface/include
CFLAGS += -I$(TOPDIR)/../build/output/bin/tfm_build/generated/interface/include
CFLAGS += -I$(TOPDIR)/../build/output/bin/tfm_build/_build/

# Architecture and board includes
ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
#   CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/common
#   CFLAGS += -I$(BOARD_SRCDIR)/bk7239n/src
endif

all: $(libfile)
.PHONY: all depend
$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
$(libfile): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

# install: $(libfile)
# 	$(Q) cp $< $(LIBDIR)

.depend: Makefile $(CSRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(CSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(libfile))
	$(call DELFILE, $(OBJS))
	$(call DELFILE, *.o)
	$(call DELFILE, *$(OBJEXT))
	$(call CLEAN)
distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
-include Make.dep