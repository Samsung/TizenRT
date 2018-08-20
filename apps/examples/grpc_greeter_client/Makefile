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
include $(APPDIR)/Make.defs

CXXEXT ?= .cc
PROTOEXT ?= .proto
GENCXXEXT ?= .pb.cc
SERVICECXXEXT ?= .grpc.pb.cc
MSG_PREFIX ?= .pb
SERVICE_PREFIX ?= .grpc.pb

# Greeter Client Example

APPNAME = grpc_client

FUNCNAME = greeter_client_main
THREADEXEC = TASH_EXECMD_ASYNC

CXXPROTO	= helloworld.proto
ASRCS		=
CSRCS		=
CXXFLAGS 	+= -I../../../external/protobuf/src/
CXXFLAGS 	+= -DGOOGLE_PROTOBUF_NO_RTTI -D__TizenRT__
MAINSRC		= greeter_client.cc

CXXSRCS		= $(CXXPROTO:$(PROTOEXT)=$(GENCXXEXT))
CXXSRCS2		= $(CXXPROTO:$(PROTOEXT)=$(SERVICECXXEXT))

AOBJS		= $(ASRCS:.S=$(OBJEXT))
COBJS		= $(CSRCS:.c=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:$(GENCXXEXT)=$(MSG_PREFIX)$(OBJEXT))
CXXOBJS2		= $(CXXSRCS2:$(SERVICECXXEXT)=$(SERVICE_PREFIX)$(OBJEXT))
ifeq ($(suffix $(MAINSRC)),$(CXXEXT))
MAINOBJ 	= $(MAINSRC:$(CXXEXT)=$(OBJEXT))
else
MAINOBJ 	= $(MAINSRC:.c=$(OBJEXT))
endif

SRCS		= $(ASRCS) $(CSRCS) $(CXXSRCS) $(CXXSRCS2) $(MAINSRC)
OBJS		= $(AOBJS) $(COBJS) $(CXXOBJS) $(CXXOBJS2)

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

CONFIG_EXAMPLES_GREETER_CLIENT_PROGNAME ?= grpc_client$(EXEEXT)
PROGNAME	= $(CONFIG_EXAMPLES_GREETER_CLIENT_PROGNAME)

ROOTDEPPATH	= --dep-path .

# Common build

VPATH		=

all: .built
.PHONY:	clean depend distclean

$(CXXSRCS): %$(GENCXXEXT): %$(PROTOEXT)
	protoc -I . --cpp_out=. $<

$(CXXSRCS2): %$(SERVICECXXEXT): %$(PROTOEXT)
	protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $<

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(MSG_PREFIX)$(OBJEXT): %$(GENCXXEXT)
	$(call COMPILEXX, $<, $@)

$(CXXOBJS2): %$(SERVICE_PREFIX)$(OBJEXT): %$(SERVICECXXEXT)
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

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_EXAMPLES_GREETER_CLIENT),yy)

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
	$(call DEL_GRPCFILES)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:
