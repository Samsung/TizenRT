###########################################################################
#
# Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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
# apps/platform/Makefile
#
#   Copyright (C) 2013-2014 Gregory Nutt. All rights reserved.
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
include $(APPDIR)/Make.defs

# Platform-specific Appliction Support
# Tool stuff

ifeq ($(DIRLINK),)
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
DIRLINK = $(TOPDIR)/tools/link.bat
DIRUNLINK = $(TOPDIR)/tools/unlink.bat
else
DIRLINK = $(TOPDIR)/tools/link.sh
DIRUNLINK = $(TOPDIR)/tools/unlink.sh
endif
endif

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# Directories

GNUDIR      = $(APPDIR)/platform/gnu
PLATFORMDIR = $(APPDIR)/platform/board
DUMMYDIR    = $(APPDIR)/platform/dummy
BOARDDIR    = $(APPDIR)/platform/$(CONFIG_ARCH_BOARD)
LINKDIR     = $(if $(wildcard $(BOARDDIR)/Make.defs),$(BOARDDIR),$(DUMMYDIR))

VPATH       = board
DEPPATH     = --dep-path $(PLATFORMDIR)

# Binaries

ASRCS =
CSRCS =

-include $(GNUDIR)/Make.defs
-include $(PLATFORMDIR)/Make.defs

AOBJS = $(patsubst %.S, bin/%$(OBJEXT), $(ASRCS))
COBJS = $(patsubst %.c, bin/%$(OBJEXT), $(CSRCS))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
else
  BIN = ../libapps$(LIBEXT)
endif
endif

# Build targets

all: .built
.PHONY: context .depend depend clean distclean

ifneq ($(ASRCS),)
$(AOBJS): bin/%$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)
endif

ifneq ($(CSRCS),)
$(COBJS): bin/%$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
endif

.built: $(OBJS)
ifneq ($(SRCS),)
	$(call ARCHIVE, $(BIN), $(OBJS))
endif
	$(Q) touch .built

# Build context setup

$(PLATFORMDIR): $(TOPDIR)/.config
	@echo "LN: platform/board to $(LINKDIR)"
	$(Q) $(DIRUNLINK) $(PLATFORMDIR)
	$(Q) $(DIRLINK) $(LINKDIR) $(PLATFORMDIR)

install:

context: $(PLATFORMDIR)

# Dependencies

.depend: Makefile $(SRCS) $(PLATFORMDIR)
	$(Q) $(MKDEP) --obj-path bin --obj-suffix  $(OBJEXT) --dep-path $(PLATFORMDIR) --dep-path $(GNUDIR) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

# Clean targets

clean:
	$(call DELFILE, .built)
	$(Q) $(MAKE) -C bin TOPDIR="$(TOPDIR)" APPDIR="$(APPDIR)" clean

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)
	$(Q) $(DIRUNLINK) $(PLATFORMDIR)

-include Make.dep
.PHONY: preconfig
preconfig:
