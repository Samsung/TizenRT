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
###########################################################################
############################################################################
# libc/Makefile
#
#   Copyright (C) 2007-2014 Gregory Nutt. All rights reserved.
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

-include $(TOPDIR)/Make.defs

# CFLAGS

ifeq ($(CONFIG_BUILD_PROTECTED),y)
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  KDEFINE = ${shell $(TOPDIR)\tools\define.bat "$(CC)" __KERNEL__}
else
  KDEFINE = ${shell $(TOPDIR)/tools/define.sh "$(CC)" __KERNEL__}
endif
else
ifeq ($(CONFIG_BUILD_KERNEL),y)
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  KDEFINE = ${shell $(TOPDIR)\tools\define.bat "$(CC)" __KERNEL__}
else
  KDEFINE = ${shell $(TOPDIR)/tools/define.sh "$(CC)" __KERNEL__}
endif
endif
endif

# Sources and paths

ASRCS =
CSRCS =

DEPPATH := --dep-path .
VPATH := .

include aio/Make.defs
include assert/Make.defs
include audio/Make.defs
include builtin/Make.defs
include ctype/Make.defs
include dirent/Make.defs
include dlfcn/Make.defs
include errno/Make.defs
include eventfd/Make.defs
include fixedmath/Make.defs
include gdbstub/Make.defs
include grp/Make.defs
include hex2bin/Make.defs
include inttypes/Make.defs
include libgen/Make.defs
include locale/Make.defs
include lzf/Make.defs
include machine/Make.defs
include misc/Make.defs
include elf/Make.defs
include net/Make.defs
include netdb/Make.defs
include obstack/Make.defs
include pthread/Make.defs
include pwd/Make.defs
include queue/Make.defs
include regex/Make.defs
include sched/Make.defs
include search/Make.defs
include semaphore/Make.defs
include signal/Make.defs
include spawn/Make.defs
include stdio/Make.defs
include stdlib/Make.defs
include stream/Make.defs
include string/Make.defs
include symtab/Make.defs
include syslog/Make.defs
include termios/Make.defs
include time/Make.defs
include tls/Make.defs
include uio/Make.defs
include unistd/Make.defs
include userfs/Make.defs
include uuid/Make.defs
include wchar/Make.defs
include wctype/Make.defs
include wqueue/Make.defs
# REVISIT: Backslash causes problems in $(COBJS) target
DELIM := $(strip /)
BINDIR ?= bin

AOBJS = $(patsubst %.S, $(BINDIR)$(DELIM)%$(OBJEXT), $(ASRCS))
COBJS = $(patsubst %.c, $(BINDIR)$(DELIM)%$(OBJEXT), $(CSRCS))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

KBIN = libkc$(LIBEXT)
BIN ?= libc$(LIBEXT)

all: $(BIN)
.PHONY: clean distclean

$(AOBJS): $(BINDIR)$(DELIM)%$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)
	$(call MOVEOBJ,$(patsubst $(BINDIR)$(DELIM)%$(OBJEXT),%,$@),$(BINDIR))

$(COBJS): $(BINDIR)$(DELIM)%$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
	$(call MOVEOBJ,$(patsubst $(BINDIR)$(DELIM)%$(OBJEXT),%,$@),$(BINDIR))

# C library for the flat build

$(BIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

ifeq ($(CONFIG_LIBC_ZONEINFO_ROMFS),y)
	$(Q) $(MAKE) $(JOBS) -C zoneinfo all TOPDIR=$(TOPDIR)
endif

# C library for the kernel phase of the two-pass kernel build

ifneq ($(BIN),$(KBIN))
$(KBIN):
	$(Q) $(MAKE) $(JOBS) $(KBIN) BIN=$(KBIN) BINDIR=kbin TOPDIR=$(TOPDIR) EXTRADEFINES=$(EXTRADEFINES)
endif

# Context

bin kbin:
	$(Q) mkdir -p $@

context: bin kbin
ifeq ($(CONFIG_LIBC_ZONEINFO_ROMFS),y)
	$(Q) $(MAKE) $(JOBS) -C zoneinfo context TOPDIR=$(TOPDIR)
endif


CFLAGS += -I$(TOPDIR)/libs/libc
AFLAGS += -I$(TOPDIR)/libs/libc

# Dependencies

.depend: Makefile $(SRCS)
ifneq ($(CONFIG_BUILD_FLAT),y)
	$(Q) $(MKDEP) --obj-path bin --obj-suffix $(OBJEXT) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >bin/Make.dep
	$(Q) $(MKDEP) --obj-path kbin --obj-suffix $(OBJEXT) $(DEPPATH) "$(CC)" -- $(CFLAGS) $(KDEFINE) -- $(SRCS) >kbin/Make.dep
else
	$(Q) $(MKDEP) --obj-path bin --obj-suffix $(OBJEXT) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >bin/Make.dep
endif
	$(Q) touch $@

depend: .depend

# Clean most derived files, retaining the configuration

clean:
	$(call DELFILE, bin$(DELIM)*$(OBJEXT))
	$(call DELFILE, kbin$(DELIM)*$(OBJEXT))
	$(call DELFILE, $(BIN))
	$(call DELFILE, libc$(LIBEXT))
	$(call DELFILE, $(KBIN))
	$(call CLEAN)

# Deep clean -- removes all traces of the configuration

distclean: clean
	$(call DELDIR, bin)
	$(call DELDIR, kbin)
	$(Q) $(MAKE) -C zoneinfo distclean TOPDIR=$(TOPDIR)
	$(call DELFILE, bin/Make.dep)
	$(call DELFILE, kbin/Make.dep)
	$(call DELFILE, .depend)

-include bin/Make.dep
-include kbin/Make.dep

# Configuration-dependent ELF module linker script.
ifeq ($(CONFIG_LIBC_ELF),y)
elf/gnu-elf.ld: elf/gnu-elf.ld.in $(TOPDIR)/include/tinyara/config.h
	$(call PREPROCESS, $<, $@)
context: elf/gnu-elf.ld
endif

distclean_elf:
	$(call DELFILE, elf/gnu-elf.ld)
distclean: distclean_elf
