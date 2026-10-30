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
# drivers/Makefile
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
############################################################################

-include $(TOPDIR)/Make.defs
DELIM ?= $(strip /)

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

DEPPATH = --dep-path .
ASRCS =
CSRCS =
DEPPATH = --dep-path .
VPATH = .

CSRCS = drivers_initialize.c

# Include support for various drivers.  Each Make.defs file will add its
# files to the source file list, add its DEPPATH info, and will add
# the appropriate paths to the VPATH variable

include analog/Make.defs
include audio/Make.defs
include bch/Make.defs
include crypto/Make.defs
include i2c/Make.defs
include i2s/Make.defs
include input/Make.defs
include ioexpander/Make.defs
include lcd/Make.defs
include loop/Make.defs
include misc/Make.defs
include mtd/Make.defs
include net/Make.defs
include note/Make.defs
include pipes/Make.defs
include power/Make.defs
include sensors/Make.defs
include serial/Make.defs
include spi/Make.defs
include syslog/Make.defs
include timers/Make.defs
include usbdev/Make.defs
include usbhost/Make.defs
include usbmisc/Make.defs
include usbmonitor/Make.defs
include video/Make.defs
include wireless/Make.defs

# TizenRT drivers that NuttX does not have

include bluetooth/Make.defs
include compression/Make.defs
include cpuload/Make.defs
include iotdev/Make.defs
include lwnl/Make.defs
include memory/Make.defs
include mipidsi/Make.defs
include os_api_test/Make.defs
include otp/Make.defs
include pm/Make.defs
include prodconfig/Make.defs
include seclink/Make.defs
include silent_reboot/Make.defs
include wifi_csi/Make.defs

ifeq ($(CONFIG_VIRTKEY),y)
  CSRCS += virtkey.c
endif

ifeq ($(CONFIG_SPECIFIC_DRIVERS),y)
-include platform/Make.defs
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libdrivers$(LIBEXT)

all: $(BIN)
.PHONY: depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

# Context hook used by the TizenRT top-level build.
context:
