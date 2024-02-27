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
CSRCS		=
CXXSRCS		=

CXXFLAGS += -Wno-shadow -Wno-pedantic -fno-permissive
CXXFLAGS += -pipe -std=c++11
CXXFLAGS += -fno-exceptions -fcheck-new -fno-rtti
CXXFLAGS += -ffunction-sections -fdata-sections
CXXFLAGS += -DTF_LITE_DISABLE_X86_NEON
CXXFLAGS += -DTF_LITE_USE_GLOBAL_CMATH_FUNCTIONS
CXXFLAGS += -Wno-maybe-uninitialized
CXXFLAGS += -Wno-missing-field-initializers
CXXFLAGS += -Wno-type-limits -Wno-undef

CFLAGS += -Wno-strict-prototypes
CFLAGS += -DTF_LITE_DISABLE_X86_NEON

TFMICRO_SRC_DIR = ./tensorflow/lite

CXXFLAGS += -g
CFLAGS   += -g
CXXFLAGS += -I$(TOPDIR)/../external/tfmicro
CXXFLAGS += -I$(TOPDIR)/../external/tfmicro/third_party
CXXFLAGS += -I$(TOPDIR)/../external/tfmicro/third_party/gemmlowp

CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/schema/ -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/core/ -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/kernels/ -name "*.cc")
#micro
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/ -maxdepth 1 -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/arena_allocator/ -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/memory_planner/ -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/testing/ -name "*.cc")
CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/tflite_bridge/ -name "*.cc")

#kernel files
# Enable this line to enable compile of all .cc files.
# CXXSRCS += $(shell find $(TFMICRO_SRC_DIR)/micro/kernels/ -maxdepth 1 -name "*.cc")

CXXSRCS += ./tensorflow/lite/micro/kernels/activations.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/activations_common.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/fully_connected_common.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/kernel_util.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/logistic.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/logistic_common.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/softmax_common.cc

ifeq ($(CONFIG_AMEBAD_CMSIS_NN),y)
CXXFLAGS += -DCMSIS_NN
CFLAGS += -DCMSIS_NN
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/add.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/conv.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/mul.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc
else
# CXXSRCS += ./tensorflow/lite/micro/kernels/add.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/conv.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/depthwise_conv.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/fully_connected.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/mul.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/pooling.cc
CXXSRCS += ./tensorflow/lite/micro/kernels/softmax.cc
# CXXSRCS += ./tensorflow/lite/micro/kernels/svdf.cc
endif #if CONFIG_AMEBAD_CMSIS_NN

CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-type-limits -Wno-undef

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
