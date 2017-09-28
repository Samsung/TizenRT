###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ASRCS =
CSRCS =
MAINSRC =

DEPPATH = --dep-path .
VPATH =

ifeq ($(CONFIG_LWM2M_WAKAAMA),y)
CFLAGS += -I$(TOPDIR)/../external/wakaama/core
include core/Make.defs
include core/er-coap-13/Make.defs
endif

ifeq ($(CONFIG_DM),y)
  include examples/shared/Make.defs
ifeq ($(CONFIG_LWM2M_CLIENT_MODE),y)
  include examples/client/Make.defs
endif
ifeq ($(CONFIG_LWM2M_SERVER_MODE),y)
  include examples/server/Make.defs
endif

  CFLAGS+=-I$(TOPDIR)/../framework/include/dm
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libexternal$(LIBEXT)
else
  BIN = ../libexternal$(LIBEXT)
endif
endif

all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

.depend: Makefile $(SRCS)
	@$(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
