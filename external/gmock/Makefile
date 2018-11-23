###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

ASRCS		=
CSRCS		=
CXXSRCS 	=

GMOCK_DIR 	= ./
GTEST_DIR 	= ./gtest

CXXFLAGS += -D__TIZENRT__ -DHAVE_PTHREAD
CXXFLAGS += -I $(GMOCK_DIR) -I $(GMOCK_DIR)/include -I $(GMOCK_DIR)/include/gmock/internal
CXXFLAGS += -I $(GTEST_DIR) -I $(GTEST_DIR)/include -I $(GTEST_DIR)/include/gtest/internal
CXXFLAGS += -g -Wall -Wextra

#
# gmock 
#
CXXSRCS +=	gmock_main.cc	\
	gmock-all.cc			\
	gmock-cardinalities.cc	\
	gmock-internal-utils.cc\
	gmock-matchers.cc		\
	gmock-spec-builders.cc	\
	gmock.cc

#
# gtest
#
CXXSRCS +=	gtest-all.cc	\
	gtest_main.cc			\
	gtest.cc				\
	gtest-death-test.cc	\
	gtest-filepath.cc		\
	gtest-port.cc			\
	gtest-printers.cc		\
	gtest-test-part.cc		\
	gtest-typed-test.cc

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:.cc=$(OBJEXT))

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN		= ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\libexternal$(LIBEXT)
else
  BIN		= ../libexternal$(LIBEXT)
endif
endif

DEPPATH	= --dep-path . --dep-path $(GMOCK_DIR)/src --dep-path $(GTEST_DIR)/src

# Common build

VPATH	= :$(GMOCK_DIR)/src :$(GTEST_DIR)/src

all: .built
.PHONY: depend clean distclean chkcxx

chkcxx:
ifneq ($(CONFIG_HAVE_CXX),y)
	@echo ""
	@echo "In order to use this example, you toolchain must support must"
	@echo ""
	@echo "  (1) Explicitly select CONFIG_HAVE_CXX to build in C++ support"
	@echo "  (2) Define CXX, CXXFLAGS, and COMPILEXX in the Make.defs file"
	@echo "      of the configuration that you are using."
	@echo ""
	@exit 1
endif

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS) $(MAINOBJ): %$(OBJEXT): %.cc
	$(call COMPILEXX, $<, $@)

.built: chkcxx $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
