############################################################################
# boards/arm/imx6/sabre-6quad/src/Makefile
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

include $(TOPDIR)/Make.defs

DEPPATH = --dep-path .

include Make.defs

ARCH_SRCDIR = 	$(TOPDIR)/arch/$(CONFIG_ARCH)/src
BOARD_SRCDIR = 	$(TOPDIR)/board
ifeq ($(WINTOOL),y)
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/chip}"
  CFLAGS += -I "${shell cygpath -w $(ARCH_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/common}"
  CFLAGS += -I "${shell cygpath -w $(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src}"
else
  CFLAGS += -I$(ARCH_SRCDIR)/chip
  CFLAGS += -I$(ARCH_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/common
  CFLAGS += -I$(BOARD_SRCDIR)/$(CONFIG_ARCH_BOARD)/src
  CFLAGS += -I$(TOPDIR)/arch/$(CONFIG_ARCH)/src/armv7-a
endif

# boardctl support
ifeq ($(CONFIG_LIB_BOARDCTL),y)
DEPPATH += --dep-path $(TOPDIR)/arch
VPATH += :$(TOPDIR)/arch
CSRCS += boardctl.c
endif

all: libboard$(LIBEXT)

COBJS = $(CSRCS:.c=$(OBJEXT))
SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(ZDSVERSION),)
$(ASRCS) $(HEAD_ASRC): %$(ASMEXT): %.S
	$(Q) $(CPP) $(CPPFLAGS) $(call CONVERT_PATH,$<) -o $@.tmp
	$(Q) cat $@.tmp | sed -e "s/^#/;/g" > $@
	$(Q) rm $@.tmp
endif

$(AOBJS): %$(OBJEXT): %$(ASMEXT)
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(LINKOBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(LINKOBJS): %$(OBJEXT): %.cxx
	$(call COMPILEXX, $<, $@)

libboard$(LIBEXT): $(OBJS) $(CXXOBJS)
	$(call ARCHIVE, $@, $(OBJS) $(CXXOBJS))

.depend: Makefile $(SRCS) $(CXXSRCS) $(RCSRCS) $(TOPDIR)$(DELIM).config
ifneq ($(ZDSVERSION),)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
else
	$(Q) $(MKDEP) $(DEPPATH) $(CC) -- $(CFLAGS) -- $(SRCS) >Make.dep
endif
ifneq ($(CXXSRCS),)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >>Make.dep
endif
ifneq ($(RCSRCS),)
	$(Q) $(MKDEP) $(DEPPATH) "$(CPP)" --obj-path . -- $(CPPFLAGS) -- $(RCSRCS) >>Make.dep
endif
	$(Q) touch $@

depend: .depend

context::

clean::
	$(call DELFILE, libboard$(LIBEXT))
	$(call DELFILE, $(ETCSRC))
	$(call DELDIR, $(ETCDIR))
	$(call CLEAN)

distclean:: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
