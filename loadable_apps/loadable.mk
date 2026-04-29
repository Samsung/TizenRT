###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
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
include $(TOPDIR)/.config

LINKLIBS = $(patsubst $(LIBRARIES_DIR)/%, %, $(USERLIBS))
LDLIBS = $(patsubst %.a, %, $(patsubst lib%,-l%,$(LINKLIBS)))

TINYARALIB = "$(TOPDIR)/../build/output/libraries"
USERSPACE = $(TOPDIR)/userspace/up_userspace

LDELFFLAGS += -Bstatic
LDLIBPATH += -L $(TINYARALIB)

LIBGCC = "${shell "$(CC)" $(ARCHCPUFLAGS) -print-libgcc-file-name}"
LDLIBS += $(LIBGCC)

OBJCOPY = $(CROSSDEV)objcopy

APPDEFINE = ${shell $(TOPDIR)/tools/define.sh "$(CC)" __APP_BUILD__}

APP_OBJS = $(SRCS:.c=$(OBJEXT))
USERSPACE_OBJ = $(USERSPACE)$(OBJEXT)
OBJS = $(APP_OBJS) $(USERSPACE_OBJ)

prebuild:
	$(call DELFILE, $(USERSPACE_OBJ))

all: prebuild postbuild
.PHONY: prebuild postbuild clean install distclean

$(USERSPACE_OBJ):
	$(Q) $(MAKE) -C $(TOPDIR)/userspace TOPDIR="$(TOPDIR)" APPDEFINE="$(APPDEFINE)" CELFFLAGS="$(CELFFLAGS)" app_obj

$(APP_OBJS): %$(OBJEXT): %.c
	@echo "CC: $<"
	$(Q) $(CC) $(APPDEFINE) -c $(CELFFLAGS) $< -o $@

ifeq ($(CONFIG_ELF),y)
$(BIN): $(OBJS)
	@echo "LD: $<"
ifeq ($(CONFIG_SUPPORT_COMMON_BINARY),y)
	$(Q) $(LD) $(LDELFFLAGS) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LIBGCC) --end-group
	$(Q) $(NM) -u $(BIN) | awk -F"U " '{print "--require-defined "$$2}' >> $(USER_BIN_DIR)/lib_symbols.txt
else
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LDLIBS) $(LIBSUPXX) --end-group
endif
endif

ifeq ($(CONFIG_XIP_ELF),y)
MEM_VARS := $(shell python $(TOPDIR)/tools/bin_mem_layout.py --binary-name $(BIN) --ota-index 0)
$(foreach v,$(MEM_VARS),$(eval $(v)))
$(BIN)_LD_DEFS := \
	--defsym __FLASH_START_ADDRESS__=$(FLASH_ADD) \
	--defsym __FLASH_SIZE__=$(FLASH_SIZE) \
	--defsym __RAM_START_ADDRESS__=$(RAM_ADD) \
	--defsym __RAM_SIZE__=$(RAM_SIZE)

$(BIN): $(OBJS)
	$(Q) $(LD) -T $(TOPDIR)/../build/configs/$(CONFIG_ARCH_BOARD)/scripts/xipelf/userspace_all.ld -e main -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LIBGCC) $(LIBSUPXX) --end-group -R $(USER_BIN_DIR)/$(CONFIG_COMMON_BINARY_NAME) $($(BIN)_LD_DEFS)

undefsym : $(OBJS)
	$(Q) $(LD) $(LDELFFLAGS) -o $(USER_BIN_DIR)/$(BIN).relelf $(ARCHCRT0OBJ) $^ --start-group $(LIBGCC) --end-group
	$(Q) $(NM) -u $(USER_BIN_DIR)/$(BIN).relelf | grep -v "w " | awk -F"U " '{print "--require-defined "$$2}' >> $(USER_BIN_DIR)/lib_symbols.txt
endif

BUILDDIR = build

$(BUILDDIR):
	$(Q) mkdir -p $(BUILDDIR)

postbuild: $(BIN) | $(BUILDDIR)
	$(Q) mv $(BIN) $(BUILDDIR)/

clean:
	$(call DELFILE, $(BUILDDIR)/$(BIN))
	$(call DELFILE, $(USER_BIN_DIR)/$(BIN))
	$(call CLEAN)
	$(Q) rm -rf $(BUILDDIR)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

install:
	$(Q) install $(BUILDDIR)/$(BIN) $(USER_BIN_DIR)/$(BIN)
