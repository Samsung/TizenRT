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

FRAMEWORK_LIB_DIR = ${shell pwd}
-include $(TOPDIR)/Make.defs

DELIM ?= $(strip /)

DEPPATH = --dep-path .
ASRCS =
CSRCS =
VPATH = .

CFLAGS += -I$(TOPDIR)/../framework/include/iotbus
include src$(DELIM)iotbus$(DELIM)Make.defs

ifeq ($(CONFIG_DM), y)
include src$(DELIM)dm$(DELIM)Make.defs
endif

ifeq ($(CONFIG_ARASTORAGE), y)
include src$(DELIM)arastorage$(DELIM)Make.defs
endif

ifeq ($(CONFIG_NETUTILS_MQTT), y)
include src$(DELIM)network$(DELIM)mqtt$(DELIM)Make.defs
endif

ifeq ($(CONFIG_WIFI_MANAGER), y)
include src$(DELIM)wifi_manager$(DELIM)Make.defs
endif

ifeq ($(CONFIG_ST_THINGS), y)
include src$(DELIM)st_things$(DELIM)Make.defs
endif

ifeq ($(CONFIG_AUDIO), y)
include src$(DELIM)tinyalsa$(DELIM)Make.defs
ifeq ($(CONFIG_AUDIO_MEDIAPLAYER), y)
include src$(DELIM)mediaplayer$(DELIM)Make.defs
endif
endif

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN		= libframework$(LIBEXT)

all:	$(BIN)

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
