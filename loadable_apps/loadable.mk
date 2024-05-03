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

SRCS += $(USERSPACE).c

ifeq ($(CONFIG_LIBCXX_EXCEPTION),y)
ifeq ($(CONFIG_SUPPORT_COMMON_BINARY),y)
SRCS += $(TOPDIR)/../apps/platform/gnu/gnu_unwind_find_exidx.c
endif
endif

OBJS = $(SRCS:.c=$(OBJEXT))
CPPOBJS = $(patsubst %.cpp,%$(OBJEXT),$(filter %.cpp,$(CXXSRCS)))

prebuild:
	$(call DELFILE, $(USERSPACE)$(OBJEXT))

all: prebuild $(BIN)
.PHONY: prebuild clean install

$(OBJS): %$(OBJEXT): %.c
	@echo "CC: $<"
	$(Q) $(CC) $(APPDEFINE) -c $(CELFFLAGS) $< -o $@

$(CPPOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

$(BIN): $(OBJS) $(CPPOBJS)
	@echo "LD: $<"
ifeq ($(CONFIG_SUPPORT_COMMON_BINARY),y)
	$(Q) $(LD) $(LDELFENTRY) $(LDELFFLAGS) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LIBGCC) $(LIBSUPXX) --end-group
	$(Q) $(NM) -u $(BIN) | awk -F"[Uw] " '{print "--require-defined "$$2}' >> $(USER_BIN_DIR)/lib_symbols.txt
else
	$(Q) $(LD) $(LDELFENTRY) $(LDELFFLAGS) $(LDLIBPATH) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LIBSUPXX) $(LDLIBS) --end-group
endif

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(USER_BIN_DIR)/$(BIN))
	$(call CLEAN)

distclean: clean

install:
	$(Q) install $(BIN) $(USER_BIN_DIR)/$(BIN)
