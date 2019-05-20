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
VPATH = .

CFLAGS += -I $(TOPDIR)/include

ifeq ($(CONFIG_TASK_MANAGER),y)
CFLAGS += -I $(TOPDIR)/kernel
endif

# Include support for various drivers.  Each Make.defs file will add its
# files to the source file list, add its DEPPATH info, and will add
# the appropriate paths to the VPATH variable

include analog$(DELIM)Make.defs
include audio$(DELIM)Make.defs
include bch$(DELIM)Make.defs
include fota$(DELIM)Make.defs
include gpio$(DELIM)Make.defs
include heapinfo$(DELIM)Make.defs
include i2c$(DELIM)Make.defs
include iotdev$(DELIM)Make.defs
include lwnl${DELIM}Make.defs
include net$(DELIM)Make.defs
include pipes$(DELIM)Make.defs
include power$(DELIM)Make.defs
include seclink$(DELIM)Make.defs
include sensors$(DELIM)Make.defs
include serial$(DELIM)Make.defs
include spi$(DELIM)Make.defs
include syslog$(DELIM)Make.defs
include task_manager$(DELIM)Make.defs
include testcase$(DELIM)Make.defs
include ttrace$(DELIM)Make.defs
include usbdev$(DELIM)Make.defs
include usbhost$(DELIM)Make.defs
include wireless$(DELIM)Make.defs

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
  CSRCS += dev_null.c dev_zero.c

ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)
  CSRCS += loop.c
ifeq ($(CONFIG_DRVR_WRITEBUFFER),y)
  CSRCS += rwbuffer.c
else
ifeq ($(CONFIG_DRVR_READAHEAD),y)
  CSRCS += rwbuffer.c
endif
endif
endif

ifeq ($(CONFIG_CAN),y)
  CSRCS += can.c
endif

ifeq ($(CONFIG_DEV_URANDOM),y)
  CSRCS += dev_urandom.c
endif

ifeq ($(CONFIG_PWM),y)
  CSRCS += pwm.c
endif

ifeq ($(CONFIG_WATCHDOG),y)
  CSRCS += watchdog.c
endif

ifeq ($(CONFIG_TIMER),y)
  CSRCS += timer.c
endif

ifeq ($(CONFIG_RTC),y)
  CSRCS += rtc.c
endif
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libdrivers$(LIBEXT)

all: $(BIN)

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
