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

#include "OMStatus.h"

#include "core/OMUtils.h"
#include "core/OMKernelData.h"

#include "import/OMKernelConfigureBuilder.h"

#include "execute/OMRuntimeKernel.h"
#include "execute/OMUtils.h"

using namespace onert_micro;
using namespace onert_micro::core;

namespace
{

constexpr int inputTensorIdx = 0;
constexpr int weightsFeatureTensorIdx = 1;
constexpr int weightsTimeTensorIdx = 2;
constexpr int biasTensorIdx = 3;
constexpr int inputActivationStateTensorIdx =
  4; // This is a variable tensor, and will be modified by this op.
constexpr int outputTensorIdx = 0;

} // namespace

namespace onert_micro
{
namespace import
{

OMStatus configure_kernel_CircleSVDF(const OMConfigureArgs &config_args)
{
  // Validate Tensor Inputs (dtype depends on quantization):
  // [0] = Input, {2, batch_size, input_size}
  // [1] = Weights Feature, {2, num_filters, input_size}
  // [2] = Weights Time, {2, num_filters, memory_size}
  // [3] = Bias (optional), {1, num_units}
  // [4] = Activation State (variable), {2, batch_size, memory_size * num_filters}

  OMRuntimeContext &runtime_context = config_args.runtime_context;
  uint16_t op_index = config_args.kernel_index;

  execute::OMRuntimeKernel runtime_kernel;
  runtime_kernel.readKernel(op_index, runtime_context);

  const circle::Tensor *input = runtime_kernel.inputs[inputTensorIdx];
  const circle::Tensor *weights_feature = runtime_kernel.inputs[weightsFeatureTensorIdx];
  const circle::Tensor *weights_time = runtime_kernel.inputs[weightsTimeTensorIdx];
  const circle::Tensor *bias = runtime_kernel.inputs[biasTensorIdx];
  const circle::Tensor *activation_state = runtime_kernel.inputs[inputActivationStateTensorIdx];

  const circle::Tensor *output = runtime_kernel.outputs[outputTensorIdx];

  assert(input != nullptr);
  assert(weights_feature != nullptr);
  assert(weights_time != nullptr);
  // bias can be nullptr
  assert(activation_state != nullptr);
  assert(output != nullptr);

  OMStatus status = Ok;

  const auto *options = runtime_kernel.first_operator->builtin_options_as_SVDFOptions();
  status = utils::checkCondition(options != nullptr);
  if (status != Ok)
    return status;

  OMRuntimeShape input_shape(input);
  OMRuntimeShape weights_feature_shape(weights_feature);
  OMRuntimeShape weights_time_shape(weights_time);
  OMRuntimeShape activation_state_shape(activation_state);
  OMRuntimeShape output_shape(output);

  // Define input constants based on input tensor definition above:
  const int rank = options->rank();
  const int input_size = input_shape.dims(1);
  const int batch_size = input_shape.dims(0);
  const int num_filters = weights_feature_shape.dims(0);
  status = utils::checkCondition(num_filters % rank == 0);
  if (status != Ok)
    return status;

  const int num_units = num_filters / rank;
  const int memory_size = weights_time_shape.dims(1);

  status = utils::checkCondition(input_shape.dimensionsCount() == 2);
  if (status != Ok)
    return status;
  // Validate Tensor Output:
  // [0] = float/int8_t, {2, batch_size, num_units}
  status =
    utils::checkCondition(output_shape.dimensionsCount() == 2 and
                          output_shape.dims(0) == batch_size and output_shape.dims(1) == num_units);
  if (status != Ok)
    return status;

  // Validate Weights Feature Input Tensor
  status = utils::checkCondition(weights_feature_shape.dimensionsCount() == 2 and
                                 weights_feature_shape.dims(1) == input_size);
  if (status != Ok)
    return status;

  // Validate Weights Time Input Tensor:
  status = utils::checkCondition(weights_time_shape.dimensionsCount() == 2 and
                                 weights_time_shape.dims(0) == num_filters and
                                 weights_time_shape.dims(1) == memory_size);
  if (status != Ok)
    return status;

  // Validate Optional Bias Input Tensor:
  if (bias != nullptr)
  {
    status = utils::checkCondition(OMRuntimeShape(bias).dims(0) == num_units);
    if (status != Ok)
      return status;
  }

  // Validate Activation State Input Tensor:
  status = utils::checkCondition(activation_state_shape.dimensionsCount() == 2 and
                                 activation_state_shape.dims(0) == batch_size and
                                 activation_state_shape.dims(1) == memory_size * num_filters);
  if (status != Ok)
    return status;

  if (input->type() == circle::TensorType_FLOAT32)
  {
    status = utils::checkCondition(weights_feature->type() == circle::TensorType_FLOAT32 and
                                   weights_time->type() == circle::TensorType_FLOAT32 and
                                   activation_state->type() == circle::TensorType_FLOAT32 and
                                   output->type() == circle::TensorType_FLOAT32);
    if (status != Ok)
      return status;
    if (bias)
    {
      status = utils::checkCondition(bias->type() == circle::TensorType_FLOAT32);
      if (status != Ok)
        return status;
    }
  }

  return status;
}

} // namespace import
} // namespace onert_micro
