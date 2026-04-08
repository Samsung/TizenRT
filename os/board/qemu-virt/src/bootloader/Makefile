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
# Makefile for the QEMU S1 Bootloader

# TOPDIR is passed from the script that calls this makefile.
include $(TOPDIR)/os/.config
include $(TOPDIR)/os/tools/Config.mk
ARCH_FAMILY = $(patsubst "%",%,$(CONFIG_ARCH_FAMILY))
-include $(TOPDIR)/os/arch/$(CONFIG_ARCH)/src/$(ARCH_FAMILY)/Toolchain.defs

S1_BOOT_DIR = $(TOPDIR)/os/board/qemu-virt/src/bootloader
VPATH = $(S1_BOOT_DIR)

S1_LDSCRIPT = $(S1_BOOT_DIR)/s1_boot.ld
S1_ASRCS = $(S1_BOOT_DIR)/qemu_s1_boot_entry.S
S1_CSRCS = $(S1_BOOT_DIR)/qemu_s1_boot.c $(S1_BOOT_DIR)/s1_printf.c $(S1_BOOT_DIR)/s1_boot_crc.c $(S1_BOOT_DIR)/s1_utils.c

S1_AOBJS = $(notdir $(patsubst %.S,%.o,$(S1_ASRCS)))
S1_COBJS = $(notdir $(patsubst %.c,%.o,$(S1_CSRCS)))
S1_OBJS = $(S1_AOBJS) $(S1_COBJS)

S1_BIN = $(TOPDIR)/build/output/bin/s1_boot.bin

# Compiler and Linker Flags (simplified for bootloader)
S1_CFLAGS = -O0 -g -fno-common -ffunction-sections -fdata-sections -nostartfiles -nostdlib -mfloat-abi=soft
S1_CFLAGS += -I$(TOPDIR)/os/include -I$(TOPDIR)/os/arch/arm/include -I$(TOPDIR)/os/board/qemu-virt/include
S1_CFLAGS += -I$(TOPDIR)/os/arch/arm/src/armv7-a -I$(TOPDIR)/os/arch/arm/src/common
S1_AFLAGS = $(S1_CFLAGS) -D__ASSEMBLY__

S1_LDFLAGS = -T $(S1_LDSCRIPT) --gc-sections

# Build rules
all: $(S1_BIN)

$(S1_COBJS): %.o: %.c
	@echo "CC: $(notdir $<)"
	$(Q) $(CC) $(S1_CFLAGS) -c $< -o $@

$(S1_AOBJS): %.o: %.S
	@echo "AS: $(notdir $<)"
	$(Q) $(CC) $(S1_AFLAGS) -c $< -o $@

$(S1_BIN): $(S1_OBJS)
	@echo "LD: $(notdir $@)"
	$(Q) $(LD) --entry=_start $(S1_LDFLAGS) $(LIBPATHS) $(EXTRA_LIBPATHS) \
		-o $(@:.bin=.elf) $(S1_OBJS) -Map=$(@:.bin=.map) \
		--start-group $(EXTRA_LIBS) $(LIBGCC) --end-group
	$(Q) $(OBJCOPY) -O binary $(@:.bin=.elf) $@

clean:
	rm -f $(S1_OBJS) $(S1_BIN) $(S1_BIN:.bin=.elf) $(S1_BIN:.bin=.map)
