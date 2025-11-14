###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
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
CXXSRCS		=

CXXFLAGS += -fno-permissive
CXXFLAGS += -Os
# To use quantized models disable this flag
# CXXFLAGS += -DDIS_QUANT
# Temporary solution uncomment below line to resolve compilation errors for rtl8721csm 
# CXXFLAGS += -DCONFIG_WCHAR_BUILTIN -std=c++14

ONERTMICRO_SRC_DIR = ./onert-micro/luci-interpreter/src
ONERTMICRO_INCLUDE_DIR = ./onert-micro/luci-interpreter/include
ONERTMICRO_PAL_MCU_DIR = ./onert-micro/luci-interpreter/pal/mcu
ONERTMICRO_PAL_CMSIS_NN_DIR = $(TOPDIR)/../external/onert-micro/onert-micro/luci-interpreter/pal/cmsisnn
ONERTMICRO_PAL_COMMON_DIR = $(TOPDIR)/../external/onert-micro/onert-micro/luci-interpreter/pal/common
FLATBUFFER_DIR = $(TOPDIR)/../external/onert-micro
SCHEMA_DIR = $(TOPDIR)/../external/onert-micro/externals/gen

CXXFLAGS += -I$(SCHEMA_DIR) -I$(ONERTMICRO_INCLUDE_DIR) -I$(ONERTMICRO_SRC_DIR) -I$(FLATBUFFER_DIR)
CXXFLAGS += -I$(ONERTMICRO_PAL_COMMON_DIR)

ifeq ($(CONFIG_EXTERNAL_CMSIS_NN),y)
CXXFLAGS += -I$(ONERTMICRO_PAL_CMSIS_NN_DIR)
else
CXXFLAGS += -I$(ONERTMICRO_PAL_MCU_DIR)
endif

ifeq ($(CONFIG_EXTERNAL_CMSIS_NN),y)
CXXFLAGS += -I$(ONERTMICRO_PAL_CMSIS_NN_DIR)
CXXFLAGS += -I$(TOPDIR)/../external/include/cmsis_nn
else
CXXFLAGS += -I$(ONERTMICRO_PAL_MCU_DIR)
endif # CONFIG_EXTERNAL_CMSIS_NN

CXXSRCS += $(ONERTMICRO_SRC_DIR)/Interpreter.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/memory_managers/SimpleMemoryManager.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/loader/GraphLoader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/KernelBuilder.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/loader/ModuleLoader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Utils.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/RuntimeGraph.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/CircleMicroReader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/CircleMicroReaderHelper.cpp

# Kernels
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Abs.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Add.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/AveragePool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/FullyConnected.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Dequantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Conv2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/ConvolutionCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Logistic.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Gather.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Exp.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/ExpandDims.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Fill.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Pack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Reshape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/ReduceCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Less.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Mul.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/MaxPool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Concatenation.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Shape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Slice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Sub.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Split.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/SplitV.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Quantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/StridedSlice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Softmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Tanh.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Transpose.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/While.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/UnidirectionalSequenceLSTM.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Unpack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Mean.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/SquaredDifference.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Rsqrt.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Sum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/L2Normalize.cpp

AOBJS		= $(ASRCS:.S=$(OBJEXT))
CXXOBJS		= $(CXXSRCS:.cpp=$(OBJEXT))

SRCS		= $(ASRCS) $(CXXSRCS)
OBJS		= $(AOBJS) $(CXXOBJS)

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

$(CXXOBJS) $(MAINOBJ): %$(OBJEXT): %.cpp
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
	$(foreach FILE, $(CXXOBJS), $(call DELFILE, $(FILE)))

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
