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
include $(APPDIR)/Make.defs

CXXEXT ?= .cc
# Protocol Buffers Test

APPNAME    = protobuf_test
FUNCNAME   = gtest_main
THREADEXEC = TASH_EXECMD_ASYNC

GMOCK_DIR = $(TOPDIR)/../external/gmock
GTEST_DIR = $(TOPDIR)/../external/gmock/gtest
USER_DIR  = ../../../external/protobuf/src

CXXFLAGS += -D__TIZENRT__ -DHAVE_PTHREAD -DGOOGLE_PROTOBUF_NO_RTTI -g -Wall -Wno-sign-compare
CXXFLAGS += -I $(USER_DIR)  -I $(USER_DIR)/google/protobuf
CXXFLAGS += -I $(GMOCK_DIR) -I $(GMOCK_DIR)/include -I $(GMOCK_DIR)/include/gmock/internal
CXXFLAGS += -I $(GTEST_DIR) -I $(GTEST_DIR)/include -I $(GTEST_DIR)/include/gtest/internal

# Protocol Buffers Test
ASRCS	  =
CSRCS		=
CXXSRCS	=
MAINSRC	= gtest_main.cc

# protobuf-test
CXXSRCS +=	  $(USER_DIR)/google/protobuf/any_test.cc\
  $(USER_DIR)/google/protobuf/arenastring_unittest.cc\
  $(USER_DIR)/google/protobuf/arena_unittest.cc\
  $(USER_DIR)/google/protobuf/descriptor_database_unittest.cc\
  $(USER_DIR)/google/protobuf/descriptor_unittest.cc\
  $(USER_DIR)/google/protobuf/drop_unknown_fields_test.cc\
  $(USER_DIR)/google/protobuf/dynamic_message_unittest.cc\
  $(USER_DIR)/google/protobuf/extension_set_unittest.cc\
  $(USER_DIR)/google/protobuf/generated_message_reflection_unittest.cc\
  $(USER_DIR)/google/protobuf/map_field_test.cc\
  $(USER_DIR)/google/protobuf/map_test.cc\
  $(USER_DIR)/google/protobuf/message_unittest.cc\
  $(USER_DIR)/google/protobuf/no_field_presence_test.cc\
  $(USER_DIR)/google/protobuf/preserve_unknown_enum_test.cc\
  $(USER_DIR)/google/protobuf/reflection_ops_unittest.cc\
  $(USER_DIR)/google/protobuf/repeated_field_reflection_unittest.cc\
  $(USER_DIR)/google/protobuf/repeated_field_unittest.cc\
  $(USER_DIR)/google/protobuf/unknown_field_set_unittest.cc\
  $(USER_DIR)/google/protobuf/well_known_types_unittest.cc\
  $(USER_DIR)/google/protobuf/wire_format_unittest.cc\
  $(USER_DIR)/google/protobuf/arena_test_util.cc\
  $(USER_DIR)/google/protobuf/map_test_util.cc\
  $(USER_DIR)/google/protobuf/test_util.cc\
  $(USER_DIR)/google/protobuf/testing/googletest.cc\
  $(USER_DIR)/google/protobuf/testing/file.cc\
  $(USER_DIR)/google/protobuf/any_test.pb.cc\
  $(USER_DIR)/google/protobuf/map_proto2_unittest.pb.cc\
  $(USER_DIR)/google/protobuf/map_unittest.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_arena.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_custom_options.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_drop_unknown_fields.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_import.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_import_public.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_lazy_dependencies.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_lazy_dependencies_custom_option.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_lazy_dependencies_enum.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_mset.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_mset_wire_format.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_no_arena_import.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_no_arena.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_no_field_presence.pb.cc\
  $(USER_DIR)/google/protobuf/unittest.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_preserve_unknown_enum.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_preserve_unknown_enum2.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_proto3_arena.pb.cc\
  $(USER_DIR)/google/protobuf/unittest_well_known_types.pb.cc\

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:$(CXXEXT)=$(OBJEXT))
ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
  MAINOBJ 	= $(MAINSRC:$(CXXEXT)=$(OBJEXT))
else
  MAINOBJ 	= $(MAINSRC:.c=$(OBJEXT))
endif

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS) $(MAINSRC)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
OBJS		+= $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
BIN		= ..\..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN		= ..\\..\\libapps$(LIBEXT)
else
  BIN		= ../../libapps$(LIBEXT)
endif
endif

CONFIG_EXAMPLES_GTEST_PROGNAME ?= gtest$(EXEEXT)
PROGNAME	= $(CONFIG_EXAMPLES_GTEST_PROGNAME)

ROOTDEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY:	clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)

ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
$(MAINOBJ): %$(OBJEXT): %$(CXXEXT)
	$(call COMPILEXX, $<, $@)
else
$(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)
endif

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	@echo "LD: $(PROGNAME)"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_GTEST),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(FUNCNAME),$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(FUNCNAME).bdat

else
context:

endif

.depend: Makefile $(SRCS)
ifeq ($(filter %$(CXXEXT),$(SRCS)),)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
else
	@$(MKDEP) $(ROOTDEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(SRCS) >Make.dep
endif
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:
