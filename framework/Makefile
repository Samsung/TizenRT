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
CXXSRCS =
VPATH = .

# Framework Directories
# BUILDIRS is the list of top-level directories containing Make.defs files

BUILDIRS   := $(dir $(wildcard src/*/Make.defs))

define Add_Framework
  include $(1)Make.defs
endef

$(foreach BDIR, $(BUILDIRS), $(eval $(call Add_Framework,$(BDIR))))

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CPPOBJS = $(patsubst %.cpp,%$(OBJEXT),$(filter %.cpp,$(CXXSRCS)))
CXXOBJS = $(patsubst %.cxx,%$(OBJEXT),$(filter %.cxx,$(CXXSRCS)))

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS = $(AOBJS) $(COBJS) $(CPPOBJS) $(CXXOBJS)

BIN		= libframework$(LIBEXT)

all:	$(BIN)

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CPPOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cxx
	$(call COMPILEXX, $<, $@)

$(BIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(ASRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(CSRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, $(BIN))
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
