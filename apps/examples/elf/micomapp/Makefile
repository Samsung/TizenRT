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
############################################################################
# apps/examples/elf/micomapp/Makefile
#
#   Copyright (C) 2012, 2014, 2016 Gregory Nutt. All rights reserved.
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
-include $(TOPDIR)/Make.defs

OBJEXT ?= .o
DELIM ?= /

ifeq ($(WINTOOL),y)
TINYARALIB = "${shell cygpath -w $(TOPDIR)$(DELIM)lib}"
else
TINYARALIB = "$(TOPDIR)$(DELIM)../build/output/libraries"
endif

ifeq ($(CONFIG_EXAMPLES_ELF_FULLYLINKED),y)
LDELFFLAGS += -Bstatic
LDLIBPATH += -L $(TINYARALIB)
LDLIBS += -lproxies

ifeq ($(CONFIG_BUILD_PROTECTED),y)
LDLIBS += -luc
else
LDLIBS += -lc
endif

ifeq ($(CONFIG_BUILD_PROTECTED),y)
LDLIBS += -lumm
else
LDLIBS += -lmm
endif

LIBGCC = "${shell "$(CC)" $(ARCHCFLAGS) -print-libgcc-file-name}"
LDLIBS += $(LIBGCC)
endif

BIN = micom
BIN_CHECK = $(shell nm -u $(PWD)/$(BIN)/$(BIN) | wc -l)
# need to update
BIN_TYPE = ELF
DYNAMIC_RAM_SIZE = 20480
BIN_VER = 20190421
KERNEL_VER = 2.0

SRCS = micomapp.c
OBJS = $(SRCS:.c=$(OBJEXT))

all: $(BIN)
.PHONY: clean install verify

$(OBJS): %$(OBJEXT): %.c
	@echo "CC: $<"
	$(Q) $(CC) -c $(CELFFLAGS) $< -o $@

$(BIN): $(OBJS)
	@echo "LD: $<"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $@ $(ARCHCRT0OBJ) $^ --start-group $(LDLIBS) --end-group

clean:
	$(call DELFILE, $(BIN))
	$(call DELFILE, $(USER_BIN_DIR)/$(BIN))
	$(call CLEAN)

install:
	$(Q) mkdir -p $(USER_BIN_DIR)
	$(Q) install $(BIN) $(USER_BIN_DIR)/$(BIN)
	$(Q) $(TOPDIR)/tools/mkbinheader.py $(USER_BIN_DIR)/$(BIN) $(BIN_TYPE) $(KERNEL_VER) $(BIN) $(BIN_VER) $(DYNAMIC_RAM_SIZE)
	$(Q) $(TOPDIR)/tools/mkchecksum.py $(USER_BIN_DIR)/$(BIN)

verify:
ifneq ($(BIN_CHECK),0)
	@echo "Undefined Symbols"
	$(Q) $(NM) -u -l $(BIN)
	$(call DELFILE, $(SYSTEM_BIN_DIR)/$(BIN))
	$(call DELFILE, $(BIN))
	$(call CLEAN)
	exit 1
endif
