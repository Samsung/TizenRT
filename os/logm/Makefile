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

-include $(TOPDIR)/Make.defs

ASRCS =

ifeq ($(CONFIG_LOGM),y)
CSRCS += logm_start.c logm_process.c logm.c
CSRCS += logm_get.c logm_set.c
ifeq ($(CONFIG_TASH),y)
CSRCS += logm_tashcmds.c
endif
ifeq ($(CONFIG_LOGM_TEST),y)
CSRCS += logm_test.c
endif
endif

DEPPATH = --dep-path .
VPATH = .

COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = liblogm$(LIBEXT)

all: $(BIN)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $(notdir $@))

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $(notdir $@))

$(BIN):	$(OBJS)
	$(call ARCHIVE, $@, $(notdir $(OBJS)))

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
