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
#include "train/OMBackpropExecutionBuilder.h"
#include "execute/OMRuntimeKernel.h"
#include "PALFullyConnectedWeightGrad.h"
#include "PALFullyConnectedInputGrad.h"
#include "PALReluInputGrad.h"

using namespace onert_micro;
using namespace onert_micro::core;
using namespace onert_micro::train;

namespace
{

constexpr uint32_t inputTensorIdx = 0;
constexpr uint32_t weightTensorIdx = 1;
constexpr uint32_t biasTensorIdx = 2;

constexpr uint32_t outputTensorIdx = 0;

} // namespace

/*
 * - Calculate weight gradient (with bias)
 * - Calculate input gradient - Optional (not required if it is last op)
 */
OMStatus onert_micro::train::train_kernel_CircleFullyConnected(const OMBackpropExecuteArgs &args)
{
  core::OMRuntimeStorage &forward_storage = args.forward_storage;
  core::OMRuntimeStorage &backward_storage = args.backward_storage;
  core::OMRuntimeContext &context = args.backward_context;
  uint16_t op_index = args.kernel_index;

  const circle::Tensor *input;
  const circle::Tensor *weight;
  const circle::Tensor *output;

  int32_t weight_tensor_index = -1;

  uint8_t *input_data;
  uint8_t *dloss_dinput_data;

  uint8_t *weight_data;
  uint8_t *dloss_dweight_data;

  uint8_t *bias_data;
  uint8_t *dloss_dbias_data;

  uint8_t *output_data;
  uint8_t *dloss_doutput_data;

  const circle::FullyConnectedOptions *options;
  // Read kernel
  {
    execute::OMRuntimeKernel runtime_kernel;
    runtime_kernel.readKernel(op_index, context);

    input = runtime_kernel.inputs[inputTensorIdx];
    weight = runtime_kernel.inputs[weightTensorIdx];
    output = runtime_kernel.outputs[outputTensorIdx];
    assert(input != nullptr);
    assert(weight != nullptr);
    // Bias can be nullptr
    assert(output != nullptr);

    weight_tensor_index = runtime_kernel.inputs_index[weightTensorIdx];
    assert(weight_tensor_index != -1);

    // Read forward storage
    {
      runtime_kernel.getDataFromStorage(op_index, forward_storage, context);

      input_data = runtime_kernel.inputs_data[inputTensorIdx];
      weight_data = runtime_kernel.inputs_data[weightTensorIdx];
      bias_data = runtime_kernel.inputs_data[biasTensorIdx];
      output_data = runtime_kernel.outputs_data[outputTensorIdx];
      // Bias_data can be nullptr
      // Output_data can be nullptr
      // Input_data can be nullptr
      assert(weight_data != nullptr);
    }

    // Read backward storage
    {
      runtime_kernel.getDataFromStorage(op_index, backward_storage, context);

      dloss_dinput_data = runtime_kernel.inputs_data[inputTensorIdx];
      dloss_dweight_data = runtime_kernel.inputs_data[weightTensorIdx];
      dloss_dbias_data = runtime_kernel.inputs_data[biasTensorIdx];
      dloss_doutput_data = runtime_kernel.outputs_data[outputTensorIdx];
      // Bias_data and dloss_dinput_data can be nullptr
      // Note: dloss_dinput_data can be nullptr due to it can be last trainable node
      assert(dloss_dweight_data != nullptr);
      assert(dloss_doutput_data != nullptr);
    }

    options = runtime_kernel.first_operator->builtin_options_as_FullyConnectedOptions();
  }

  OMRuntimeShape input_shape(input);
  OMRuntimeShape output_shape(output);

  // 1. Handle activation functions
  switch (options->fused_activation_function())
  {
    case circle::ActivationFunctionType_NONE:
      // Do nothing
      break;
    case circle::ActivationFunctionType_RELU:
    {
      assert(output_data != nullptr);
      pal::ReluInputGrad(utils::castInputData<float>(output_data),
                         utils::castOutputData<float>(dloss_doutput_data), output_shape);
      break;
    }
    default:
    {
      assert(false && "Unsupported activation type");
      return UnsupportedType;
    }
  }

  if (args.is_trainable_layer)
  {
    // Check is only bias updating
    if (args.train_rank_type != ONLY_BIAS)
    {
      assert(input_data != nullptr); // FIX memory planner then

      // Get weight shape
      OMRuntimeShape weight_shape(weight);
      OMRuntimeShape dynamic_shapes = backward_storage.getDynamicRuntimeShape(weight_tensor_index);
      if (dynamic_shapes.flatSize() != 0)
        weight_shape = dynamic_shapes;

      // 2. Calculate weight gradient
      // Init weight grads with zeros
      const auto kDlossSizeInBytes = output_shape.dims(1) * input_shape.dims(1) * sizeof(float);
      for (int i = 0; i < kDlossSizeInBytes; i += sizeof(float))
        *static_cast<float *>(static_cast<void *>(dloss_dweight_data + i)) = 0;

      pal::FullyConnectedWeightGrad(
        core::utils::castInputData<float>(dloss_doutput_data), output_shape,
        core::utils::castInputData<float>(input_data), input_shape,
        core::utils::castOutputData<float>(dloss_dweight_data), weight_shape, args.train_rank_type);
    }

    // 3. Calculate bias gradient
    // Just copy dloss_doutput_data to dloss_dbias_data
    // TODO: introduce training inplace
    if (dloss_dbias_data)
    {
      assert(bias_data != nullptr);
      if (bias_data == nullptr)
        return UnknownError;

      std::memcpy(dloss_dbias_data, dloss_doutput_data,
                  sizeof(OMDataType(output->type())) *
                    output_shape.dims(output_shape.dimensionsCount() - 1));
    }
  }

  // 4. Calculate (if needed) input grad
  if (args.is_last_layer == false)
  {
    assert(dloss_dinput_data != nullptr);

    pal::FullyConnectedInputGrad(core::utils::castInputData<float>(dloss_doutput_data),
                                 output_shape, core::utils::castInputData<float>(weight_data),
                                 OMRuntimeShape(weight),
                                 core::utils::castOutputData<float>(dloss_dinput_data));
  }

  return Ok;
}
