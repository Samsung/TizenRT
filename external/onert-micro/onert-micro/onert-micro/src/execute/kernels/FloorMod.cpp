/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
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

#include "execute/OMKernelExecutionBuilder.h"
#include "OMStatus.h"
#include "execute/OMRuntimeKernel.h"
#include "execute/kernels/ReadKernelDataCommon.h"
#include "core/OMUtils.h"
#include "PALFloorMod.h"

using namespace onert_micro;
using namespace onert_micro::core;

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleFloorMod(const OMExecuteArgs &execute_args)
{
  uint8_t *input_data1;
  uint8_t *input_data2;
  uint8_t *output_data;

  core::OMRuntimeShape input_shape1;
  core::OMRuntimeShape input_shape2;
  core::OMRuntimeShape output_shape;

  circle::TensorType input1_type;

  OMStatus status =
    execute::readKernelDataTISO(execute_args, input_data1, input_data2, output_data, input_shape1,
                                input_shape2, output_shape, input1_type);

  switch (input1_type)
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      // Check the denominator
      for (int i = 0; i < input_shape2.flatSize(); ++i)
      {
        utils::checkCondition(core::utils::castInputData<float>(input_data2)[i] != 0);
      }
      // check that input and output dimensions are equal
      if (input_shape1 == input_shape2)
      {
        const int flat_size = input_shape1.flatSize();
        pal::FloorMod(flat_size, core::utils::castInputData<float>(input_data1),
                      core::utils::castInputData<float>(input_data2),
                      core::utils::castOutputData<float>(output_data));
      }
      else
      {
        pal::BroadcastFloorMod4DSlow(input_shape1, core::utils::castInputData<float>(input_data1),
                                     input_shape2, core::utils::castInputData<float>(input_data2),
                                     output_shape, core::utils::castOutputData<float>(output_data));
      }
    }
    break;
#endif // DIS_FLOAT
    default:
      assert(false && "Unsupported type.");
  }

  return status;
}

} // namespace execute
} // namespace onert_micro
