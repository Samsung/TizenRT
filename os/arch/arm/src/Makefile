###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
############################################################################
############################################################################
# arch/arm/src/Makefile
#
#   Copyright (C) 2007-2009, 2011-2012, 2014 Gregory Nutt. All rights reserved.
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
-include chip/Make.defs

ifeq ($(CONFIG_ARCH_CORTEXM3),y)   # Cortex-M3 is ARMv7-M
ARCH_SUBDIR = armv7-m
else
ifeq ($(CONFIG_ARCH_CORTEXM4),y)   # Cortex-M4 is ARMv7E-M
ARCH_SUBDIR = armv7-m
else
ifeq ($(CONFIG_ARCH_CORTEXM7),y)   # Cortex-M7 is ARMv7E-M
ARCH_SUBDIR = armv7-m
else
ifeq ($(CONFIG_ARCH_CORTEXR4),y)   # Cortex-R4 is ARMv7-R
ARCH_SUBDIR = armv7-r
else
ifeq ($(CONFIG_ARCH_CORTEXM33),y)   # Cortex-M33 is ARMv8-M
ARCH_SUBDIR = armv8-m
else
ifeq ($(CONFIG_ARCH_CORTEXM55),y)   # Cortex-M55 is ARMv8-M
ARCH_SUBDIR = armv8-m
else
ifeq ($(CONFIG_ARCH_ARMV7A_FAMILY),y)
ARCH_SUBDIR = armv7-a
else
ARCH_SUBDIR = arm
endif
endif
endif
endif
endif
endif
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  ARCH_SRCDIR = $(TOPDIR)\arch\$(CONFIG_ARCH)\src
  TINYARA = "$(TOPDIR)\$(OUTBIN_DIR)\tinyara$(EXEEXT)"
  CFLAGS += -I$(ARCH_SRCDIR)\chip
  CFLAGS += -I$(ARCH_SRCDIR)\common
  CFLAGS += -I$(ARCH_SRCDIR)\$(ARCH_SUBDIR)
  CFLAGS += -I$(TOPDIR)\kernel
else
  ARCH_SRCDIR = $(TOPDIR)/arch/$(CONFIG_ARCH)/src
ifeq ($(WINTOOL),y)
  TINYARA = "${shell cygpath -w $(TOPDIR)/$(OUTBIN_DIR)/tinyara$(EXEEXT)}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/$(ARCH_SUBDIR)}"
  CFLAGS += -I "${shell cygpath -w $(TOPDIR)/kernel}"
else
  TINYARA = "$(TOPDIR)/$(OUTBIN_DIR)/tinyara$(EXEEXT)"
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(ARCH_SRCDIR)/$(ARCH_SUBDIR)
  CFLAGS += -I$(TOPDIR)/kernel
endif
endif

# The "head" object

HEAD_OBJ = $(HEAD_ASRC:.S=$(OBJEXT))
STARTUP_OBJS ?= $(HEAD_OBJ)

# Flat build or kernel-mode objects

ASRCS = $(CHIP_ASRCS) $(CMN_ASRCS)
AOBJS = $(ASRCS:.S=$(OBJEXT))

CSRCS = $(CHIP_CSRCS) $(CMN_CSRCS)
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

# User-mode objects

UASRCS = $(CHIP_UASRCS) $(CMN_UASRCS)
UAOBJS = $(UASRCS:.S=$(OBJEXT))

UCSRCS = $(CHIP_UCSRCS) $(CMN_UCSRCS)
UCOBJS = $(UCSRCS:.c=$(OBJEXT))

USRCS = $(UASRCS) $(UCSRCS)
UOBJS = $(UAOBJS) $(UCOBJS)

KBIN = libkarch$(LIBEXT)
UBIN = libuarch$(LIBEXT)
BIN  = libarch$(LIBEXT)

LDFLAGS += $(ARCHSCRIPT)

# Override in Make.defs if linker is not 'ld'

LDSTARTGROUP ?= --start-group
LDENDGROUP ?= --end-group

EXTRA_LIBS ?=
EXTRA_LIBPATHS ?=
LINKLIBS ?=

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BOARDMAKE = $(if $(wildcard .\board\Makefile),y,)
  LIBPATHS += -L"$(TOPDIR)\$(LIBRARIES_DIR)"
ifeq ($(BOARDMAKE),y)
  LIBPATHS += -L"$(TOPDIR)\arch\$(CONFIG_ARCH)\src\board"
endif

else
  BOARDMAKE = $(if $(wildcard ./board/Makefile),y,)

ifeq ($(WINTOOL),y)
  LIBPATHS += -L"${shell cygpath -w "$(TOPDIR)/$(LIBRARIES_DIR)"}"
ifeq ($(BOARDMAKE),y)
  LIBPATHS += -L"${shell cygpath -w "$(TOPDIR)/arch/$(CONFIG_ARCH)/src/board"}"
endif
else
  LIBPATHS += -L"$(TOPDIR)/$(LIBRARIES_DIR)"
ifeq ($(BOARDMAKE),y)
  LIBPATHS += -L"$(TOPDIR)/arch/$(CONFIG_ARCH)/src/board"
endif
endif
endif

LDLIBS = $(patsubst %.a,%,$(patsubst lib%,-l%,$(LINKLIBS)))
ifeq ($(BOARDMAKE),y)
  LDLIBS += -lboard
endif

LIBGCC = "${shell "$(CC)" $(ARCHCPUFLAGS) -print-libgcc-file-name}"
GCC_LIBDIR := ${shell dirname $(LIBGCC)}

VPATH += chip
VPATH += common
VPATH += $(ARCH_SUBDIR)

all: $(HEAD_OBJ) $(BIN)

.PHONY: board$(DELIM)libboard$(LIBEXT)

$(AOBJS) $(UAOBJS) $(HEAD_OBJ): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(UCOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN) $(KBIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

$(UBIN): $(UOBJS)
	$(call ARCHIVE, $@, $(UOBJS))

board$(DELIM)libboard$(LIBEXT):
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" libboard$(LIBEXT) EXTRADEFINES=$(EXTRADEFINES)

$(OUTBIN_DIR)/tinyara$(EXEEXT): $(HEAD_OBJ) board/libboard$(LIBEXT)
	$(Q) echo "LD: tinyara"
	$(Q) $(LD) --entry=__start $(LDFLAGS) $(LIBPATHS) $(EXTRA_LIBPATHS) \
		-o $(TINYARA) $(HEAD_OBJ) $(EXTRA_OBJS) \
		--start-group $(LDLIBS) $(EXTRA_LIBS) $(LIBGCC) --end-group -Map $(TOPDIR)/../build/output/bin/tinyara.map
ifneq ($(CONFIG_WINDOWS_NATIVE),y)
	$(Q) $(NM) -l $(TINYARA) | \
	grep -v '\(compiled\)\|\(\$(OBJEXT)$$\)\|\( [aUw] \)\|\(\.\.ng$$\)\|\(LASH[RL]DI\)' | \
	sort > $(TOPDIR)/$(OUTBIN_DIR)/System.map
endif

# This is part of the top-level export target
# Note that there may not be a head object if layout is handled
# by the linker configuration.

export_startup: board/libboard$(LIBEXT) $(STARTUP_OBJS)
ifneq ($(STARTUP_OBJS),)
	$(Q) if [ -d "$(EXPORT_DIR)/startup" ]; then \
		cp -f $(STARTUP_OBJS) "$(EXPORT_DIR)/startup/."; \
	 else \
		echo "$(EXPORT_DIR)/startup does not exist"; \
	exit 1; \
	fi
endif

# Dependencies

.depend: Makefile chip/Make.defs $(SRCS)
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" depend
endif
	$(Q) $(MKDEP) $(patsubst %,--dep-path %,$(subst :, ,$(VPATH))) \
	 "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" clean
endif
	$(call DELFILE, $(KBIN))
	$(call DELFILE, $(UBIN))
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
ifeq ($(BOARDMAKE),y)
	$(Q) $(MAKE) -C board TOPDIR="$(TOPDIR)" distclean
endif
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
