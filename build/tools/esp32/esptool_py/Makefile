###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###########################################################################

# Component support for esptool.py. Doesn't do much by itself,
# components have their own flash targets that can use these variables.

include $(CURRENTDIR)/../../../os/.config

ESPPORT ?= $(CONFIG_ESPTOOLPY_PORT)
ESPBAUD ?= $(CONFIG_ESPTOOLPY_BAUD)
ESPFLASHMODE ?= $(CONFIG_ESPTOOLPY_FLASHMODE)
ESPFLASHFREQ ?= $(CONFIG_ESPTOOLPY_FLASHFREQ)
ESPFLASHSIZE ?= $(CONFIG_ESPTOOLPY_FLASHSIZE)

CONFIG_ESPTOOLPY_COMPRESSED ?=

PYTHON ?= python
MAKECMDGOALS ?= all

define prereq_if_explicit
$(filter $(1),$(MAKECMDGOALS))
endef

BUILD_DIR_BASE = $(CURRENTDIR)/../../output/bin
BIN_NAME = tinyara

APP_ELF:=$(BUILD_DIR_BASE)/$(BIN_NAME).elf
APP_MAP:=$(APP_ELF:.elf=.map)
APP_BIN:=$(APP_ELF:.elf=.bin)
ROMFS_IMG:=$(BUILD_DIR_BASE)/romfs.img
ROMFS_OFFSET = $(shell esptool_py/romfs_flash_address.sh)

# two commands that can be used from other components
# to invoke esptool.py (with or without serial port args)
#
ESPTOOLPY_SRC := $(CURRENTDIR)/esptool_py/esptool/esptool.py
ESPTOOLPY := $(PYTHON) $(ESPTOOLPY_SRC) --chip esp32
ESPTOOLPY_SERIAL := $(ESPTOOLPY) --port $(ESPPORT) --baud $(ESPBAUD) --before $(CONFIG_ESPTOOLPY_BEFORE) --after $(CONFIG_ESPTOOLPY_AFTER)

ESPTOOL_FLASH_OPTIONS := --flash_mode $(ESPFLASHMODE) --flash_freq $(ESPFLASHFREQ) --flash_size $(ESPFLASHSIZE)
ifdef CONFIG_ESPTOOLPY_FLASHSIZE_DETECT
ESPTOOL_WRITE_FLASH_OPTIONS := --flash_mode $(ESPFLASHMODE) --flash_freq $(ESPFLASHFREQ) --flash_size detect
else
ESPTOOL_WRITE_FLASH_OPTIONS := $(ESPTOOL_FLASH_OPTIONS)
endif

ESPTOOL_ELF2IMAGE_OPTIONS :=

ESPTOOLPY_WRITE_FLASH=$(ESPTOOLPY_SERIAL) write_flash $(if $(CONFIG_ESPTOOLPY_COMPRESSED),-z,-u) $(ESPTOOL_WRITE_FLASH_OPTIONS)

ESPTOOL_ALL_FLASH_ARGS += $(APP_OFFSET) $(APP_BIN)

# non-secure boot (or bootloader), both these files are the same
APP_BIN_UNSIGNED ?= $(APP_BIN)

$(APP_BIN_UNSIGNED): $(APP_ELF) $(ESPTOOLPY_SRC)
	$(ESPTOOLPY) elf2image $(ESPTOOL_FLASH_OPTIONS) $(ESPTOOL_ELF2IMAGE_OPTIONS) -o $@ $<


ESPTOOL_ALL_FLASH_ARGS += $(ESPTOOL_BOOTLOADER_FLASH_ARGS)

ALL:flash

OS:app-flash

ROMFS:romfs

romfs: $(ROMFS_IMG) $(ESPTOOLPY_SRC)
ifneq ($(ROMFS_OFFSET),-1)
	@echo "Flashing romfs image to serial port $(ESPPORT), offset $(ROMFS_OFFSET)..."
	$(ESPTOOLPY_WRITE_FLASH) $(ROMFS_OFFSET) $(ROMFS_IMG)
endif

BOOTLOADER:
	@echo "Flashing bootloader to serial port $(ESPPORT) (app at offset $(BOOTLOADER_OFFSET))..."
	$(ESPTOOLPY_WRITE_FLASH) $(ESPTOOL_BOOTLOADER_FLASH_ARGS)

flash: $(APP_BIN) $(ESPTOOLPY_SRC) $(call prereq_if_explicit,erase_flash) partition_table_get_info
	@echo "Flashing binaries to serial port $(ESPPORT) (app at offset $(APP_OFFSET))..."
	$(ESPTOOLPY_WRITE_FLASH) $(ESPTOOL_ALL_FLASH_ARGS)

app-flash: $(APP_BIN) $(ESPTOOLPY_SRC) $(call prereq_if_explicit,erase_flash) partition_table_get_info
	@echo "Flashing app to serial port $(ESPPORT), offset $(APP_OFFSET)..."
	$(ESPTOOLPY_WRITE_FLASH) $(APP_OFFSET) $(APP_BIN)

# Submodules normally added in component.mk, but can be added
# at the project level as long as qualified path
COMPONENT_SUBMODULES += $(COMPONENT_PATH)/esptool

BOOTLOADER_OFFSET := 0x1000
BOOTLOADER_BIN := $(CURRENTDIR)/bootloader/bootloader.bin


erase_flash:
	@echo "Erasing entire flash..."
	$(ESPTOOLPY_SERIAL) erase_flash

MONITORBAUD ?= $(CONFIG_MONITOR_BAUD)

MONITOR_PYTHON := $(PYTHON)

ifeq ("$(OS)","Windows_NT")
# miniterm and idf_monitor both need a Windows Console PTY in order
# to correctly handle user input
MONITOR_PYTHON := winpty $(PYTHON)
endif

# note: if you want to run miniterm from command line, can simply run
# miniterm.py on the console. The '$(PYTHON) -m serial.tools.miniterm'
# is to allow for the $(PYTHON) variable overriding the python path.
simple_monitor: $(call prereq_if_explicit,%flash)
	$(MONITOR_PYTHON) -m serial.tools.miniterm --rts 0 --dtr 0 --raw $(ESPPORT) $(MONITORBAUD)

PRINT_FILTER ?=

MONITOR_OPTS := --baud $(MONITORBAUD) --port $(ESPPORT) --toolchain-prefix $(CONFIG_TOOLPREFIX) --make "$(MAKE)" --print_filter "$(PRINT_FILTER)"

monitor: $(call prereq_if_explicit,%flash)
	$(summary) MONITOR
	[ -f $(APP_ELF) ] || echo "*** 'make monitor' target requires an app to be compiled and flashed first."
	[ -f $(APP_ELF) ] || echo "*** Run 'make flash monitor' to build, flash and monitor"
	[ -f $(APP_ELF) ] || echo "*** Or alternatively 'make simple_monitor' to view the serial port as-is."
	[ -f $(APP_ELF) ] || exit 1
	$(MONITOR_PYTHON) $(IDF_PATH)/tools/idf_monitor.py $(MONITOR_OPTS) $(APP_ELF)

.PHONY: erase_flash

