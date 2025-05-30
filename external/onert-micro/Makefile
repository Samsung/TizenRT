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

CXXFLAGS += -Wno-shadow -Wno-pedantic -fno-permissive
CXXFLAGS += -pipe -std=c++14
CXXFLAGS += -fno-exceptions -fcheck-new -fno-rtti
CXXFLAGS += -ffunction-sections -fdata-sections
CXXFLAGS += -Os
# To use quantized models disable this flag
CXXFLAGS += -DDIS_QUANT
CXXFLAGS += -Wno-maybe-uninitialized
CXXFLAGS += -Wno-missing-field-initializers
CXXFLAGS += -Wno-type-limits -Wno-undef

CFLAGS += -Wno-strict-prototypes

ONERTMICRO_SRC_DIR = ./onert-micro/onert-micro/src
ONERTMICRO_INCLUDE_DIR = ./onert-micro/onert-micro/include
ONERTMICRO_PAL_MCU_DIR = ./onert-micro/onert-micro/include/pal/mcu
ONERTMICRO_PAL_COMMON_DIR = ./onert-micro/onert-micro/include/pal/common
FLATBUFFER_DIR = ./externals/flatbuffers
SCHEMA_DIR = ./externals/gen

CXXFLAGS += -I$(SCHEMA_DIR) -I$(ONERTMICRO_INCLUDE_DIR) -I$(ONERTMICRO_SRC_DIR) -I$(FLATBUFFER_DIR)
CXXFLAGS += -I$(ONERTMICRO_PAL_COMMON_DIR) -I$(ONERTMICRO_PAL_MCU_DIR)

# OLD
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/Interpreter.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/memory_managers/SimpleMemoryManager.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/loader/GraphLoader.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/KernelBuilder.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/loader/ModuleLoader.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Utils.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/RuntimeGraph.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/CircleMicroReader.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/CircleMicroReaderHelper.cpp
#
## Kernels
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Abs.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Add.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/AveragePool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/AveragePool2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/FullyConnected.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Conv2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Logistic.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Gather.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Exp.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/ExpandDims.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Fill.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Pack.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Reshape.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/ReduceCommon.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Less.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Mul.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/MaxPool2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Concatenation.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Shape.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Slice.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Sub.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Split.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/SplitV.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/StridedSlice.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Softmax.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Tanh.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/While.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/Transpose.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/kernels/UnidirectionalSequenceLSTM.cpp

# NEW

CXXSRCS += $(ONERTMICRO_SRC_DIR)/api/onert-micro.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/OMInterpreter.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/memory/OMMemoryManager.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/memory/OMRuntimeAllocator.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/OMCircleReader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/OMWeightOnlyFormatReader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMDataType.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMKernelType.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMRuntimeContext.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMRuntimeGraph.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMRuntimeModule.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMRuntimeStorage.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMShape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMUtils.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/reader/OMTrainingConfigFileReader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/OMKernelExecute.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/OMKernelExecutionBuilder.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/OMRuntimeKernel.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/OMUtils.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/OMUtils.cpp
#Execute Kernels

## common kernels
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/PoolingCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReshapeCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/MathCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMConfigureTISOKernel.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMConfigureSISOKernel.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMPoolingCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMPadCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMFloorCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMArgCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/helpers/OMSpacesBatchesNDCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SpacesBatchesNDCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReadKernelDataCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReluCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ArgCommon.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ConvolutionCommon.cpp
#/*CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Abs.cpp*/
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Add.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Cast.cpp
#/*CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Concatenation.cpp*/
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Conv2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Div.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ExpandDims.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/FullyConnected.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Gather.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Greater.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/GRU.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Logistic.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/MaxPool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Mul.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Neg.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Pack.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReduceProd.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReduceSum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Reshape.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Shape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Softmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/StridedSlice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Sub.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SVDF.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Transpose.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/UnidirectionalSequenceLSTM.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Unpack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ZerosLike.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Abs.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Abs.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/AddN.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/AddN.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ArgMax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ArgMax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ArgMin.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ArgMin.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Concatenation.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Concatenation.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/BatchToSpaceND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/BatchToSpaceND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Ceil.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Ceil.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Cos.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Cos.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Cast.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Cast.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Div.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Div.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Dequantize.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Dequantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Logistic.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Logistic.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Log.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Log.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Gather.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Gather.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/GatherND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/GatherND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Exp.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Exp.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Greater.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Greater.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/GreaterEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/GreaterEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ExpandDims.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ExpandDims.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Elu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Elu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Equal.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Equal.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Fill.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Fill.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Floor.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Floor.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/FloorDiv.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/FloorDiv.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/FloorMod.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/FloorMod.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Pack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Pack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Pad.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Pad.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Relu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Relu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ReduceProd.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/ReduceProd.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Round.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Round.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Less.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Less.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/LessEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/LessEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/LeakyRelu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/LeakyRelu.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/LogSoftmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/LogSoftmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Maximum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Maximum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Mean.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Mean.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Minimum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Minimum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Shape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Shape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/NotEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/NotEqual.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Sin.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Sin.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SquaredDifference.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SquaredDifference.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Slice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Slice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Split.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Split.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SpaceToBatchND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SpaceToBatchND.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SplitV.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SplitV.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Square.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Square.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Sqrt.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Sqrt.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SpaceToDepth.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SpaceToDepth.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Quantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Quantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Tanh.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Tanh.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Transpose.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Transpose.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/TransposeConv.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/TransposeConv.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Sum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Sum.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/While.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/While.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Rsqrt.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Rsqrt.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/OMExecutionPlanCreator.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/OMKernelConfiguration.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/OMKernelConfigureBuilder.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/OMDynamicShapesHandler.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/DepthwiseConv2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/DepthwiseConv2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Dequantize.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Dequantize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Relu6.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/Relu6.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/L2Normalize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/L2Normalize.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/L2Pool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/L2Pool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SelectV2.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/execute/kernels/SelectV2.cpp

# Import Kernels
#/*CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Abs.cpp*/
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Add.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Cast.cpp
#/*CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Concatenation.cpp*/
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Conv2D.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Div.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ExpandDims.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/FullyConnected.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Gather.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Greater.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/GRU.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Logistic.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/MaxPool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Mul.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Neg.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Pack.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ReduceProd.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Reshape.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Shape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Softmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/StridedSlice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Sub.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/SVDF.cpp
#CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Transpose.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/UnidirectionalSequenceLSTM.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/Unpack.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/import/kernels/ZerosLike.cpp

CXXSRCS += $(ONERTMICRO_SRC_DIR)/optimize/pass/FindInplaceOpPass.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/optimize/OMOptimizer.cpp

# TRAINING
CXXSRCS += $(ONERTMICRO_SRC_DIR)/OMTrainingInterpreter.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/OMTrainingRuntimeModule.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/train/OMTrainingHandler.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/train/OMTrainingStorage.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/train/OMCheckpointSaver.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/core/train/OMCheckpointLoader.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/OMBackpropExecute.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/OMBackpropExecutionBuilder.cpp
#Train kernels
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/Conv2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/FullyConnected.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/MaxPool2D.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/Reshape.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/Softmax.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/StridedSlice.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/kernels/GRU.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/losses_functions/CrossEntropy.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/losses_functions/SparseCrossEntropy.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/losses_functions/MSE.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/metrics/Accuracy.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/metrics/CrossEntropy.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/metrics/SparseCrossEntropyAccuracy.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/metrics/MAE.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/metrics/MSE.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/train_optimizers/Adam.cpp
CXXSRCS += $(ONERTMICRO_SRC_DIR)/train/train_optimizers/SGD.cpp

CFLAGS += -Wno-maybe-uninitialized
CFLAGS += -Wno-missing-field-initializers
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-type-limits -Wno-undef

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
