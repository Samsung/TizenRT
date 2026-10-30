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
############################################################################
# boards/arm/tiva/lm3s6965-ek/src/Makefile
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
############################################################################

-include $(TOPDIR)/Make.defs
ASRCS =
DEPPATH = --dep-path .
VPATH = .

CSRCS = lm_boot.c lm_leds.c lm_ethernet.c lm_ssi.c

ifeq ($(CONFIG_BOARDCTL),y)
CSRCS += lm_appinit.c
endif

ifeq ($(CONFIG_NX_LCDDRIVER),y)
CSRCS += lm_oled.c
endif

ifeq ($(CONFIG_BOARD_LATE_INITIALIZE),y)
  CSRCS += lm_bringup.c
else ifeq ($(CONFIG_BOARDCTL),y)
  CSRCS += lm_bringup.c
endif

ARCH_FAMILY = $(patsubst "%",%,$(CONFIG_ARCH_FAMILY))
CFLAGS += -I$(TOPDIR)/kernel -I$(TOPDIR)/arch/arm/src/common -I$(TOPDIR)/arch/arm/src/chip -I$(TOPDIR)/arch/arm/src/$(ARCH_FAMILY)
AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libboard$(LIBEXT)

all: $(BIN)
.PHONY: context depend clean distclean

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

context:
