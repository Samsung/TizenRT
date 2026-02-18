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
#include "core/OMDataType.h"
#include "core/OMKernelData.h"
#include "core/memory/OMMemoryManager.h"

#include "execute/OMKernelExecutionBuilder.h"
#include "execute/OMUtils.h"
#include "execute/OMRuntimeKernel.h"

#include "PALSVDF.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::execute;

namespace
{

constexpr int inputTensorIdx = 0;
constexpr int weightsFeatureTensorIdx = 1;
constexpr int weightsTimeTensorIdx = 2;
constexpr int biasTensorIdx = 3;
constexpr int inputActivationStateTensorIdx =
  4; // This is a variable tensor, and will be modified by this op.
constexpr int outputTensorIdx = 0;

void prepareQuantParams(core::SVDFQuantParams &params, const circle::Tensor *input,
                        const circle::Tensor *weights_feature, const circle::Tensor *weights_time,
                        const circle::Tensor *activation_state, const circle::Tensor *output)
{
  assert(input->quantization() != nullptr);
  assert(output->quantization() != nullptr);
  assert(weights_feature->quantization() != nullptr);
  assert(weights_time->quantization() != nullptr);
  assert(activation_state->quantization() != nullptr);

  // Write zero points
  params.input_zero_point =
    static_cast<int32_t>(input->quantization()->zero_point()->operator[](0));
  params.output_zero_point =
    static_cast<int32_t>(output->quantization()->zero_point()->operator[](0));
  params.activation_state_zero_point =
    static_cast<int32_t>(activation_state->quantization()->zero_point()->operator[](0));

  // Calculate effective scales
  const float effective_scale_1 = (input->quantization()->scale()->operator[](0) *
                                   weights_feature->quantization()->scale()->operator[](0)) /
                                  (activation_state->quantization()->scale()->operator[](0));
  const float effective_scale_2 = (activation_state->quantization()->scale()->operator[](0) *
                                   weights_time->quantization()->scale()->operator[](0)) /
                                  (output->quantization()->scale()->operator[](0));

  execute::quantizeMultiplier(effective_scale_1, &params.effective_scale_1_a,
                              &params.effective_scale_1_b);
  execute::quantizeMultiplier(effective_scale_2, &params.effective_scale_2_a,
                              &params.effective_scale_2_b);
}

} // namespace

namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleSVDF(const OMExecuteArgs &execute_args)
{
  core::OMRuntimeContext &runtime_context = execute_args.runtime_context;
  core::OMRuntimeStorage &runtime_storage = execute_args.runtime_storage;
  uint16_t op_index = execute_args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *weights_feature;
  const circle::Tensor *weights_time;
  const circle::Tensor *bias;
  const circle::Tensor *activation_state;

  const circle::Tensor *output;

  uint8_t *input_data;
  uint8_t *weights_feature_data;
  uint8_t *weights_time_data;
  uint8_t *bias_data;
  uint8_t *activation_state_data;
  uint8_t *output_data;
  const circle::SVDFOptions *options = nullptr;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    OMStatus status = runtime_kernel.readKernel(op_index, runtime_context);
    if (status != Ok)
      return status;

    input = runtime_kernel.inputs[inputTensorIdx];
    weights_feature = runtime_kernel.inputs[weightsFeatureTensorIdx];
    weights_time = runtime_kernel.inputs[weightsTimeTensorIdx];
    bias = runtime_kernel.inputs[biasTensorIdx];
    activation_state = runtime_kernel.inputs[inputActivationStateTensorIdx];

    output = runtime_kernel.outputs[outputTensorIdx];

    assert(input != nullptr);
    assert(weights_feature != nullptr);
    assert(weights_time != nullptr);
    // bias can be nullptr
    assert(activation_state != nullptr);
    assert(output != nullptr);

    status = runtime_kernel.getDataFromStorage(op_index, runtime_storage, runtime_context);
    if (status != Ok)
      return status;

    input_data = runtime_kernel.inputs_data[inputTensorIdx];
    weights_feature_data = runtime_kernel.inputs_data[weightsFeatureTensorIdx];
    weights_time_data = runtime_kernel.inputs_data[weightsTimeTensorIdx];
    bias_data = runtime_kernel.inputs_data[biasTensorIdx];
    activation_state_data = runtime_kernel.inputs_data[inputActivationStateTensorIdx];
    output_data = runtime_kernel.outputs_data[outputTensorIdx];

    assert(input_data != nullptr);
    assert(weights_feature_data != nullptr);
    assert(weights_time_data != nullptr);
    // bias can be nullptr
    assert(output_data != nullptr);

    options = runtime_kernel.first_operator->builtin_options_as_SVDFOptions();
  }

  OMStatus status;
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

  const int num_units = num_filters / rank;
  const int memory_size = weights_time_shape.dims(1);

  const auto activation_state_size =
    activation_state_shape.flatSize() * sizeof(core::OMDataType(output->type()));
  status =
    core::memory::OMMemoryManager::allocateMemory(activation_state_size, &activation_state_data);
  if (status != Ok)
    return status;

  std::memset(activation_state_data, 0, activation_state_size);

  switch (input->type())
  {
#ifndef DIS_FLOAT
    case circle::TensorType_FLOAT32:
    {
      // Temporary buffer
      uint8_t *scratch_buffer;
      status = core::memory::OMMemoryManager::allocateMemory(
        batch_size * num_filters * sizeof(core::OMDataType(output->type())), &scratch_buffer);

      assert(status == Ok);
      if (status != Ok)
        return status;
      status = pal::SVDF(
        utils::castInputData<float>(input_data), utils::castInputData<float>(weights_feature_data),
        utils::castInputData<float>(weights_time_data), utils::castInputData<float>(bias_data),
        utils::castOutputData<float>(activation_state_data),
        utils::castOutputData<float>(scratch_buffer), utils::castOutputData<float>(output_data),
        rank, input_size, batch_size, num_filters, num_units, memory_size,
        options->fused_activation_function());

      status = core::memory::OMMemoryManager::deallocateMemory(scratch_buffer);
    }
    break;
#endif // DIS_FLOAT
#ifndef DIS_QUANT
    case circle::TensorType_INT8:
    {
      core::SVDFQuantParams params{};
      prepareQuantParams(params, input, weights_feature, weights_time, activation_state, output);

      params.rank = rank;

      status = pal::SVDF(
        params, utils::castInputData<int8_t>(input_data),
        utils::castInputData<int8_t>(weights_feature_data),
        utils::castInputData<int8_t>(weights_time_data), utils::castInputData<int32_t>(bias_data),
        utils::castOutputData<int8_t>(activation_state_data),
        utils::castOutputData<int8_t>(output_data), input_shape, weights_feature_shape,
        weights_time_shape, core::OMRuntimeShape(bias), output_shape);
    }
    break;
#endif // DIS_QUANT
    default:
    {
      status = UnsupportedActivation;
      assert(false && "Unsupported type.");
      break;
    }
  }

  status = core::memory::OMMemoryManager::deallocateMemory(activation_state_data);

  return status;
}

} // namespace execute
} // namespace onert_micro
