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

ifeq ($(CONFIG_COMPRESSED_BINARY),y)
COMPRESSION_TYPE = $(CONFIG_COMPRESSION_TYPE)
BLOCK_SIZE = $(CONFIG_COMPRESSION_BLOCK_SIZE)
else
COMPRESSION_TYPE = 0
BLOCK_SIZE = 0
endif

BOARDNAME=$(CONFIG_ARCH_BOARD)
APPDEFINE = ${shell $(TOPDIR)/tools/define.sh "$(CC)" __APP_BUILD__}

SRCS += $(USERSPACE).c

OBJS = $(SRCS:.c=$(OBJEXT))

prebuild:
	$(call DELFILE, $(USERSPACE)$(OBJEXT))

all: prebuild $(BIN)
.PHONY: prebuild clean install verify

$(OBJS): %$(OBJEXT): %.c
	@echo "CC: $<"
	$(Q) $(CC) $(APPDEFINE) -c $(CELFFLAGS) $< -o $@

$(BIN): $(OBJS)
	@echo "LD: $<"
ifeq ($(CONFIG_SUPPORT_COMMON_BINARY),y)
	$(Q) $(LD) $(LDELFFLAGS) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LIBGCC) --end-group
	$(Q) $(NM) -u $(BIN) | awk -F"U " '{print "--require-defined "$$2}' >> $(USER_BIN_DIR)/lib_symbols.txt
else
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LDLIBS) --end-group
endif

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(USER_BIN_DIR)/$(BIN))
	$(call DELFILE, $(TOPDIR)/../tools/fs/contents-smartfs/$(BOARDNAME)/base-files/bins/$(BIN)_$(BIN_VER))
	$(call CLEAN)

distclean: clean

install:
	$(Q) mkdir -p $(USER_BIN_DIR)
	$(Q) install $(BIN) $(USER_BIN_DIR)/$(BIN)
ifeq ($(CONFIG_ELF_EXCLUDE_SYMBOLS),y)
	$(Q) cp $(USER_BIN_DIR)/$(BIN) $(USER_BIN_DIR)/$(BIN)_dbg
	$(Q) $(OBJCOPY) --remove-section .comment $(USER_BIN_DIR)/$(BIN)
	$(Q) $(STRIP) -g $(USER_BIN_DIR)/$(BIN) -o $(USER_BIN_DIR)/$(BIN)
endif
	$(Q) $(TOPDIR)/tools/mkbinheader.py $(USER_BIN_DIR)/$(BIN) user $(BIN_TYPE) $(KERNEL_VER) $(BIN) $(BIN_VER) $(DYNAMIC_RAM_SIZE) $(STACKSIZE) $(PRIORITY) $(COMPRESSION_TYPE) $(BLOCK_SIZE) $(LOADING_PRIORITY)
	$(Q) $(TOPDIR)/tools/mkchecksum.py $(USER_BIN_DIR)/$(BIN)
	$(Q) mkdir -p $(TOPDIR)/../tools/fs/contents-smartfs/$(BOARDNAME)/base-files/bins
	$(Q) cp $(USER_BIN_DIR)/$(BIN) $(TOPDIR)/../tools/fs/contents-smartfs/$(BOARDNAME)/base-files/bins/$(BIN)_$(BIN_VER)

verify:
# If we support common binary, then the symbols in the common binary will appear as UNDEFINED
# in the application binary. So, verification is required only when we dont support common binary.
ifneq ($(CONFIG_SUPPORT_COMMON_BINARY),y)
	$(Q) if [ "`nm -u $(BIN) | wc -l`" != "0" ]; then \
		echo "Undefined Symbols"; \
		nm -u -l $(BIN); \
		rm $(BIN); \
		exit 1; \
	fi
endif
