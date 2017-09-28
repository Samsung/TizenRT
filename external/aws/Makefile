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

AWS_SAMPLE_DIR = samples/TizenRT
AWS_CERT_DIR = certs
AWS_PLATFORM_DIR = platform/TizenRT
AWS_EXTLIB_DIR = external_libs

DEPPATH = --dep-path .
VPATH =

ASRCS =
CSRCS =
CXXSRCS =

include src/Make.defs
include $(AWS_SAMPLE_DIR)/subscribe_publish_sample/Make.defs
include $(AWS_SAMPLE_DIR)/subscribe_publish_library_sample/Make.defs
include $(AWS_SAMPLE_DIR)/shadow_sample_console_echo/Make.defs
include $(AWS_SAMPLE_DIR)/shadow_sample/Make.defs
include $(AWS_SAMPLE_DIR)/subscribe_publish_cpp_sample/Make.defs
include $(AWS_PLATFORM_DIR)/mbedtls/Make.defs
include $(AWS_PLATFORM_DIR)/common/Make.defs
include $(AWS_CERT_DIR)/Make.defs
include $(AWS_EXTLIB_DIR)/jsmn/Make.defs

CFLAGS += -Iinclude
CFLAGS += -Iexternal_libs/jsmn
CFLAGS += -I$(AWS_PLATFORM_DIR)/common
CFLAGS += -I$(AWS_CERT_DIR)
CFLAGS += -I$(AWS_PLATFORM_DIR)/mbedtls
CFLAGS += -I$(AWS_SAMPLE_DIR)/subscribe_publish_sample
CFLAGS += -I$(AWS_SAMPLE_DIR)/subscribe_publish_library_sample
CFLAGS += -I$(AWS_SAMPLE_DIR)/subscribe_publish_cpp_sample
CFLAGS += -I$(AWS_SAMPLE_DIR)/shadow_console_echo
CFLAGS += -I$(AWS_SAMPLE_DIR)/shadow_sample

# Enable AWS IoT Debugging Messages
#CFLAGS += -DENABLE_IOT_TRACE
#CFLAGS += -DENABLE_IOT_DEBUG
CFLAGS += -DENABLE_IOT_INFO
CFLAGS += -DENABLE_IOT_ERROR

CXXFLAGS += -Iinclude
CXXFLAGS += -Iexternal_libs/jsmn
CXXFLAGS += -I$(AWS_PLATFORM_DIR)/common
CXXFLAGS += -I$(AWS_CERT_DIR)
CXXFLAGS += -I$(AWS_PLATFORM_DIR)/mbedtls
CXXFLAGS += -I$(AWS_SAMPLE_DIR)/subscribe_publish_cpp_sample

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
CXXOBJS = $(CXXSRCS:.cpp=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(CXXSRCS)
OBJS = $(AOBJS) $(COBJS) $(CXXOBJS)

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libexternal$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libexternal$(LIBEXT)
else
  BIN = ../libexternal$(LIBEXT)
endif
endif

all: .built
.PHONY: .depend clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(CXXOBJS): %$(OBJEXT): %.cpp
	$(call COMPILEXX, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	$(Q) touch .built

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(ASRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(CSRCS) >Make.dep
	$(Q) $(MKDEP) $(DEPPATH) "$(CXX)" -- $(CXXFLAGS) -- $(CXXSRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
