/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2021 The TensorFlow Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"
#include "execute/kernels/ReadKernelDataCommon.h"
#include "PALGatherND.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

// NOTE: doesn't currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleGatherND(const OMExecuteArgs &execute_args)
{

  uint8_t *input_data;
  uint8_t *position_data;
  uint8_t *output_data;

  core::OMRuntimeShape input_shape;
  core::OMRuntimeShape position_shape;
  core::OMRuntimeShape output_shape;

  circle::TensorType inputType;

  OMStatus status =
    execute::readKernelDataTISO(execute_args, input_data, position_data, output_data, input_shape,
                                position_shape, output_shape, inputType);

  switch (inputType)
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      pal::GatherND<float, int32_t>(input_shape, utils::castInputData<float>(input_data),
                                    position_shape, utils::castInputData<int32_t>(position_data),
                                    utils::castOutputData<float>(output_data));
    }
    break;
#endif // DIS_FLOAT
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
    }
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
