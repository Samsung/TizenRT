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
############################################################################
# external/protobuf/Makefile
#
#   Copyright (C) 2012 Gregory Nutt. All rights reserved.
#   Author: Gregory Nutt <gnutt@nuttx.org>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

-include $(TOPDIR)/.config
-include $(TOPDIR)/Make.defs

ASRCS		=
CSRCS		=
CXXSRCS =

#CXXFLAGS += -Wno-unused-function -Wno-unused-but-set-variable -g -Os
#CXXFLAGS += -fno-strict-aliasing -fno-strength-reduce -fomit-frame-pointer
#CXXFLAGS += -D__KERNEL__ -pipe -ffunction-sections -fdata-sections
CXXFLAGS += -D__TIZENRT__
CXXFLAGS += -DGOOGLE_PROTOBUF_NO_RTTI -DHAVE_PTHREAD
CXXFLAGS += -I ./src

#
# protobuf
#
CXXSRCS +=	src/google/protobuf/any.cc\
	src/google/protobuf/any.pb.cc\
	src/google/protobuf/api.pb.cc\
	src/google/protobuf/compiler/importer.cc\
	src/google/protobuf/compiler/parser.cc\
	src/google/protobuf/descriptor.cc\
	src/google/protobuf/descriptor.pb.cc\
	src/google/protobuf/descriptor_database.cc\
	src/google/protobuf/duration.pb.cc\
	src/google/protobuf/dynamic_message.cc\
	src/google/protobuf/empty.pb.cc\
	src/google/protobuf/extension_set_heavy.cc\
	src/google/protobuf/field_mask.pb.cc\
	src/google/protobuf/generated_message_reflection.cc\
	src/google/protobuf/generated_message_table_driven.cc\
	src/google/protobuf/io/gzip_stream.cc\
	src/google/protobuf/io/printer.cc\
	src/google/protobuf/io/strtod.cc\
	src/google/protobuf/io/tokenizer.cc\
	src/google/protobuf/io/zero_copy_stream_impl.cc\
	src/google/protobuf/map_field.cc\
	src/google/protobuf/message.cc\
	src/google/protobuf/reflection_ops.cc\
	src/google/protobuf/service.cc\
	src/google/protobuf/source_context.pb.cc\
	src/google/protobuf/struct.pb.cc\
	src/google/protobuf/stubs/mathlimits.cc\
	src/google/protobuf/stubs/substitute.cc\
	src/google/protobuf/text_format.cc\
	src/google/protobuf/timestamp.pb.cc\
	src/google/protobuf/type.pb.cc\
	src/google/protobuf/unknown_field_set.cc\
	src/google/protobuf/util/delimited_message_util.cc\
	src/google/protobuf/util/field_comparator.cc\
	src/google/protobuf/util/field_mask_util.cc\
	src/google/protobuf/util/internal/datapiece.cc\
	src/google/protobuf/util/internal/default_value_objectwriter.cc\
	src/google/protobuf/util/internal/error_listener.cc\
	src/google/protobuf/util/internal/field_mask_utility.cc\
	src/google/protobuf/util/internal/json_escaping.cc\
	src/google/protobuf/util/internal/json_objectwriter.cc\
	src/google/protobuf/util/internal/json_stream_parser.cc\
	src/google/protobuf/util/internal/object_writer.cc\
	src/google/protobuf/util/internal/proto_writer.cc\
	src/google/protobuf/util/internal/protostream_objectsource.cc\
	src/google/protobuf/util/internal/protostream_objectwriter.cc\
	src/google/protobuf/util/internal/type_info.cc\
	src/google/protobuf/util/internal/type_info_test_helper.cc\
	src/google/protobuf/util/internal/utility.cc\
	src/google/protobuf/util/json_util.cc\
	src/google/protobuf/util/message_differencer.cc\
	src/google/protobuf/util/time_util.cc\
	src/google/protobuf/util/type_resolver_util.cc\
	src/google/protobuf/wire_format.cc\
	src/google/protobuf/wrappers.pb.cc

#
# protobuf_lite
#
CXXSRCS +=	src/google/protobuf/arena.cc\
	src/google/protobuf/arenastring.cc\
	src/google/protobuf/extension_set.cc\
	src/google/protobuf/generated_message_table_driven_lite.cc\
	src/google/protobuf/generated_message_util.cc\
	src/google/protobuf/io/coded_stream.cc\
	src/google/protobuf/io/zero_copy_stream.cc\
	src/google/protobuf/io/zero_copy_stream_impl_lite.cc\
	src/google/protobuf/message_lite.cc\
	src/google/protobuf/repeated_field.cc\
	src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc\
	src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc\
	src/google/protobuf/stubs/bytestream.cc\
	src/google/protobuf/stubs/common.cc\
	src/google/protobuf/stubs/int128.cc\
	src/google/protobuf/stubs/io_win32.cc\
	src/google/protobuf/stubs/once.cc\
	src/google/protobuf/stubs/status.cc\
	src/google/protobuf/stubs/statusor.cc\
	src/google/protobuf/stubs/stringpiece.cc\
	src/google/protobuf/stubs/stringprintf.cc\
	src/google/protobuf/stubs/structurally_valid.cc\
	src/google/protobuf/stubs/strutil.cc\
	src/google/protobuf/stubs/time.cc\
	src/google/protobuf/wire_format_lite.cc

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
	$(Q) find . -name "*.o" -exec rm -rf {} \; # Tobe fixed

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep

