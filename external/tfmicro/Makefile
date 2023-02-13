###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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

OBJEXT		?= .o
ASRCS		=
CSRCS		=
CXXSRCS		=

FLATBUFFER_INC = third_party/flatbuffers/include
GEMMLOWP_INC   = third_party/gemmlowp

CXXFLAGS += -Wno-shadow -Wno-pedantic -fno-permissive
CXXFLAGS += -pipe -std=c++11
CXXFLAGS += -fno-exceptions -fcheck-new -fno-rtti
CXXFLAGS += -ffunction-sections -fdata-sections
CXXFLAGS += -DTF_LITE_DISABLE_X86_NEON
CXXFLAGS += -DTF_LITE_STATIC_MEMORY -DNDEBUG -g
CXXFLAGS += -I$(FLATBUFFER_INC) -I$(GEMMLOWP_INC) -I.

CXXFLAGS += -Wno-maybe-uninitialized
CXXFLAGS += -Wno-missing-field-initializers
CXXFLAGS += -Wno-type-limits -Wno-undef

CFLAGS += -Wno-strict-prototypes
CFLAGS += -DTF_LITE_DISABLE_X86_NEON
CFLAGS += -DTF_LITE_STATIC_MEMORY -DNDEBUG -g
CFLAGS += -I$(FLATBUFFER_INC) -I$(GEMMLOWP_INC) -I.

CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-type-limits -Wno-undef

CSRCS += tensorflow/lite/c/common.c
CXXSRCS += tensorflow/lite/micro/kernels/concatenation.cc
CXXSRCS += tensorflow/lite/micro/kernels/conv.cc
CXXSRCS += tensorflow/lite/micro/kernels/depthwise_conv.cc
CXXSRCS += tensorflow/lite/micro/kernels/softmax.cc
CXXSRCS += tensorflow/lite/micro/kernels/all_ops_resolver.cc
CXXSRCS += tensorflow/lite/micro/kernels/round.cc
CXXSRCS += tensorflow/lite/micro/kernels/strided_slice.cc
CXXSRCS += tensorflow/lite/micro/kernels/prelu.cc
CXXSRCS += tensorflow/lite/micro/kernels/split.cc
CXXSRCS += tensorflow/lite/micro/kernels/fully_connected.cc
CXXSRCS += tensorflow/lite/micro/kernels/pad.cc
CXXSRCS += tensorflow/lite/micro/kernels/mul.cc
CXXSRCS += tensorflow/lite/micro/kernels/dequantize.cc
CXXSRCS += tensorflow/lite/micro/kernels/unpack.cc
CXXSRCS += tensorflow/lite/micro/kernels/arg_min_max.cc
CXXSRCS += tensorflow/lite/micro/kernels/reshape.cc
CXXSRCS += tensorflow/lite/micro/kernels/activations.cc
CXXSRCS += tensorflow/lite/micro/kernels/logistic.cc
CXXSRCS += tensorflow/lite/micro/kernels/ceil.cc
CXXSRCS += tensorflow/lite/micro/kernels/elementwise.cc
CXXSRCS += tensorflow/lite/micro/kernels/pooling.cc
CXXSRCS += tensorflow/lite/micro/kernels/floor.cc
CXXSRCS += tensorflow/lite/micro/kernels/svdf.cc
CXXSRCS += tensorflow/lite/micro/kernels/maximum_minimum.cc
CXXSRCS += tensorflow/lite/micro/kernels/pack.cc
CXXSRCS += tensorflow/lite/micro/kernels/comparisons.cc
CXXSRCS += tensorflow/lite/micro/kernels/reduce.cc
CXXSRCS += tensorflow/lite/micro/kernels/quantize.cc
CXXSRCS += tensorflow/lite/micro/kernels/logical.cc
CXXSRCS += tensorflow/lite/micro/kernels/neg.cc
CXXSRCS += tensorflow/lite/micro/kernels/add.cc
CXXSRCS += tensorflow/lite/micro/test_helpers.cc
CXXSRCS += tensorflow/lite/micro/debug_log.cc
CXXSRCS += tensorflow/lite/micro/simple_memory_allocator.cc
CXXSRCS += tensorflow/lite/micro/micro_error_reporter.cc
CXXSRCS += tensorflow/lite/micro/micro_optional_debug_tools.cc
CXXSRCS += tensorflow/lite/micro/debug_log_numbers.cc
CXXSRCS += tensorflow/lite/micro/micro_interpreter.cc
CXXSRCS += tensorflow/lite/micro/memory_planner/greedy_memory_planner.cc
CXXSRCS += tensorflow/lite/micro/memory_planner/linear_memory_planner.cc
CXXSRCS += tensorflow/lite/micro/micro_utils.cc
CXXSRCS += tensorflow/lite/micro/micro_allocator.cc
CXXSRCS += tensorflow/lite/micro/memory_helpers.cc
CXXSRCS += tensorflow/lite/kernels/kernel_util.cc
CXXSRCS += tensorflow/lite/kernels/internal/quantization_util.cc
CXXSRCS += tensorflow/lite/core/api/error_reporter.cc
CXXSRCS += tensorflow/lite/core/api/tensor_utils.cc
CXXSRCS += tensorflow/lite/core/api/flatbuffer_conversions.cc
CXXSRCS += tensorflow/lite/core/api/op_resolver.cc

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
	$(foreach FILE, $(COBJS), $(call DELFILE, $(FILE)))
	$(foreach FILE, $(CXXOBJS), $(call DELFILE, $(FILE)))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
