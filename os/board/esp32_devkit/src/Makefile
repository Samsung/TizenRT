############################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
############################################################################
############################################################################
#
#   Copyright (C) 2016 Gregory Nutt. All rights reserved.
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

SCRIPTDIR = $(TOPDIR)$(DELIM)..$(DELIM)build$(DELIM)configs$(DELIM)$(CONFIG_ESP32_DEVKIT_TYPE)$(DELIM)scripts
CONFIGFILE = $(TOPDIR)$(DELIM)include$(DELIM)tinyara$(DELIM)config.h

ASRCS =
CSRCS = esp32_boot.c esp32_bringup.c esp32_led.c
CSRCS += ../../common/partitions.c

ifeq ($(CONFIG_LIB_BOARDCTL),y)
CSRCS += esp32_appinit.c
endif

SCRIPTIN = $(SCRIPTDIR)$(DELIM)esp32.template
SCRIPTOUT = $(SCRIPTDIR)$(DELIM)esp32_out.ld

BOARD_CONTEXT = y
EXTRA_CLEAN =

COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ARCH_SRCDIR = $(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_DIR = $(TOPDIR)/board/$(CONFIG_ARCH_BOARD)
ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip/rom}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_DIR)/include}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(ARCH_SRCDIR)/chip/chip
  CFLAGS += -I$(ARCH_SRCDIR)/chip/rom
  CFLAGS += -I$(BOARD_DIR)/include
endif

all: depend context libboard$(LIBEXT) depend context
$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)
$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
libboard$(LIBEXT): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))
.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@
depend: .depend
clean:
	$(call DELFILE, libboard$(LIBEXT))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

#include $(TOPDIR)/configs/Board.mk

$(SCRIPTOUT): $(SCRIPTIN) $(CONFIGFILE)
	# $(call PREPROCESS, $(SCRIPTIN), $@)
	$(Q) $(CC) -isystem $(TOPDIR)/include -C -P -x c -E $(SCRIPTIN) -o $@

context: $(SCRIPTOUT)

-include Make.dep
