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
#include "PALMaximum.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleMaximum(const OMExecuteArgs &execute_args)
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
      // check that input and output dimensions are equal
      if (input_shape1 == input_shape2)
      {
        const int flat_size = input_shape1.flatSize();
        status = pal::Maximum(flat_size, utils::castInputData<float>(input_data1),
                              utils::castInputData<float>(input_data2),
                              utils::castOutputData<float>(output_data));
      }
      else
      {
        status =
          pal::BroadcastMaximum4DSlow(input_shape1, utils::castInputData<float>(input_data1),
                                      input_shape2, utils::castInputData<float>(input_data2),
                                      output_shape, utils::castOutputData<float>(output_data));
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
