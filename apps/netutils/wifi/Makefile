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
-include $(APPDIR)/Make.defs

# WiFi Networking API

ASRCS		=
CSRCS		=

ifeq ($(CONFIG_NETUTILS_WIFI),y)
CSRCS += \
	slsi_wifi_api.c \
	slsi_wifi_utils.c
endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
	BIN	= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
	BIN	= ..\\..\\libapps$(LIBEXT)
else
	BIN	= ../../libapps$(LIBEXT)
endif
endif

CFLAGS += ${shell $(INCDIR) $(INCDIROPT) "$(CC)" $(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src \
			$(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src$(DELIM)utils \
			$(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src$(DELIM)common \
			$(APPDIR)$(DELIM)include$(DELIM)netutils$(DELIM)wifi}

ROOTDEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

install:

context:

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

context:

-include Make.dep
.PHONY: preconfig
preconfig:
