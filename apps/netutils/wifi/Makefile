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

#VPATH		= %.c fsm/src/
# related to generation of fsm code
FSMOUTPATH = .
# name of the .fsm files that needes to be included
#
FSMS = p2p #staap
FSMGEN_SOURCES = $(FSMS:%=slsi_wifi_%_fsm_constants.c)
FSMGEN_PRIVATE_HEADERS = $(FSMS:%=slsi_wifi_%_fsm_definitions.h)
FSM16_SRCS = fsm/src/fsm16.c
FSM32_SRCS = fsm/src/fsm32.c
# WiFi Networking API

ASRCS		=
CSRCS		=

ifeq ($(CONFIG_NETUTILS_WIFI),y)
CSRCS		+= slsi_wifi_api.c \
		   slsi_wifi_p2p_api.c \
		   slsi_wifi_utils.c \
		   $(FSM32_SRCS) \
		   $(FSMGEN_PRIVATE_SRCS) \
		   $(FSMGEN_SOURCES)
endif

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS)
OBJS		= $(AOBJS) $(COBJS)


ifeq ($(CONFIG_WINDOWS_NATIVE),y)
   BIN = ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
   BIN = ..\\..\\libapps$(LIBEXT)
else
   BIN = ../../libapps$(LIBEXT)
endif
endif


CFLAGS += ${shell $(INCDIR) $(INCDIROPT) "$(CC)" $(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src \
			$(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src$(DELIM)utils \
			$(APPDIR)$(DELIM)wpa_supplicant$(DELIM)src$(DELIM)common \
			$(APPDIR)$(DELIM)netutils$(DELIM)wifi$(DELIM)fsm$(DELIM)src \
			$(APPDIR)$(DELIM)include$(DELIM)netutils$(DELIM)wifi}
ROOTDEPPATH	= --dep-path .


# Common build



.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built


AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

all: .built
.PHONY: context depend clean distclean fsmclean fsminfo fsmgen

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

install:

context:

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean: fsmclean
	$(call DELFILE, .built)
	$(call DELFILE, $(OBJS))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

context:




#$(FSMOUTPATH)/slsi_wifi_%_fsm.fsm: ./fsm/%.fsm
#	mkdir -p $(@D)
#	$(PERL) ../utils/filepp -w $(MAKEDEFINES) $< >$@


$(FSMOUTPATH)/slsi_wifi_%_fsm_constants.c $(FSMOUTPATH)/slsi_wifi_%_fsm_definitions.h : \
	fsm/slsi_wifi_%_fsm.fsm fsm/bin/fsmgen.pl \
		fsm/templates/Sparse32_Constants_C.tpl \
		fsm/templates/Sparse32_Definitions_H.tpl \
		fsm/templates/Sparse16_Constants_C.tpl \
		fsm/templates/Sparse16_Definitions_H.tpl
	mkdir -p $(@D)
	$(PERL) fsm/bin/fsmgen.pl -p $< $(FSMOUTPATH)


fsmclean:
	$(call DELFILE, $(FSMGEN_SOURCES))
	$(call DELFILE, $(FSMGEN_PRIVATE_HEADERS))
	$(call DELFILE, fsm/src/fsm32.o)

fsmgen: $(FSMGEN_SOURCE:%=$(FSMOUTPATH)/%) $(FSMGEN_PRIVATE_HEADERS:%=$(FSMOUTPATH)/%)

fsminfo:
	echo $(CSRCS)
	echo FSMGEN_SOURCE:
	echo $(FSMGEN_SOURCES)
	echo $(FSMGEN_PRIVATE_HEADERS)

-include Make.dep
.PHONY: preconfig
preconfig:
