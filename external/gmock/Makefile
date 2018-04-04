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
CXXSRCS +=	$(GMOCK_DIR)/src/gmock_main.cc	\
	$(GMOCK_DIR)/src/gmock-all.cc			\
	$(GMOCK_DIR)/src/gmock-cardinalities.cc	\
	$(GMOCK_DIR)/src/gmock-internal-utils.cc\
	$(GMOCK_DIR)/src/gmock-matchers.cc		\
	$(GMOCK_DIR)/src/gmock-spec-builders.cc	\
	$(GMOCK_DIR)/src/gmock.cc

#
# gtest
#
CXXSRCS +=	$(GTEST_DIR)/src/gtest-all.cc	\
	$(GTEST_DIR)/src/gtest_main.cc			\
	$(GTEST_DIR)/src/gtest.cc				\
	$(GTEST_DIR)/src/gtest-death-test.cc	\
	$(GTEST_DIR)/src/gtest-filepath.cc		\
	$(GTEST_DIR)/src/gtest-port.cc			\
	$(GTEST_DIR)/src/gtest-printers.cc		\
	$(GTEST_DIR)/src/gtest-test-part.cc		\
	$(GTEST_DIR)/src/gtest-typed-test.cc

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

DEPPATH	= --dep-path .

# Common build

VPATH		=

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
	find . -name "*.o" -exec rm -rf {} \; # Tobe fixed

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
