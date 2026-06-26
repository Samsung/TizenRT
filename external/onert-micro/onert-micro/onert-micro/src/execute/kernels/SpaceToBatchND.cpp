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

#include "execute/kernels/SpacesBatchesNDCommon.h"
#include "PALSpaceToBatchND.h"

using namespace onert_micro;
using namespace onert_micro::execute;

namespace onert_micro
{
namespace execute
{

OMStatus
execute_kernel_CircleSpaceToBatchND(const onert_micro::execute::OMExecuteArgs &execute_args)
{
  auto batch_to_space_float_lambda =
    [](const core::OMRuntimeShape &input1_shape, const float *input1_data,
       const core::OMRuntimeShape &input2_shape, const int32_t *block_shape_data,
       const core::OMRuntimeShape &input3_shape, const int32_t *crops_data,
       const core::OMRuntimeShape &output_shape, float *output_data) {
      return pal::SpaceToBatchND<float>(input1_shape, input1_data, input2_shape, block_shape_data,
                                        input3_shape, crops_data, output_shape, output_data);
    };

  return execute_spaces_batches_nd_common(execute_args, batch_to_space_float_lambda);
}

} // namespace execute
} // namespace onert_micro
