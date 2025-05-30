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

#include "OMConfig.h"
#include "train/train_optimizers/SGD.h"
#include "core/memory/OMMemoryManager.h"
#include "core/OMRuntimeShape.h"
#include "core/OMDataType.h"

using namespace onert_micro;
using namespace onert_micro::train;
using namespace onert_micro::train::optimizers;

namespace
{
inline std::pair<uint32_t, uint32_t> getUpLowerWeightTensorDepth(core::OpTrainableRankType rank,
                                                                 const uint32_t output_depth)
{
  std::pair<uint32_t, uint32_t> result(0u, output_depth);

  switch (rank)
  {
    case core::ALL:
      break;
    case core::UP_1_2_PART:
      result.second = static_cast<uint32_t>(static_cast<float>(output_depth) / 2.f);
      break;
    case core::LOWER_1_2_PART:
      result.first = static_cast<uint32_t>(static_cast<float>(output_depth) / 2.f);
      break;
    default:
      assert("Unsupported type");
      break;
  }

  return result;
}
} // namespace

#ifdef OM_MEMORY_ESTIMATE

void SGD::reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage)
{
  for (auto &cur_tensor_index_data : _tensor_index_to_gradient)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;
    auto tensor_index = cur_tensor_index_data.first;

    auto tensor = context.getTensorByIndex(tensor_index);
    auto num_elements = core::OMRuntimeShape(tensor).flatSize();

#ifndef DIS_DYN_SHAPES
    int32_t dynamic_tensor_size = storage.getDynamicRuntimeShape(tensor_index).flatSize();
    if (dynamic_tensor_size != 0)
      num_elements = dynamic_tensor_size;
#endif // DIS_DYN_SHAPES

    auto tensor_size = num_elements * sizeof(core::OMDataType(tensor->type()));
    core::memory::OMMemoryManager::deallocateMemory(tensor_size, allocated_data);
  }
  _tensor_index_to_gradient.clear();
}

#endif // OM_MEMORY_ESTIMATE

void SGD::reset()
{
  for (auto &cur_tensor_index_data : _tensor_index_to_gradient)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;

    core::memory::OMMemoryManager::deallocateMemory(allocated_data);
  }
  _tensor_index_to_gradient.clear();
}

/*
 * Update internal states according to calculated gradients using Adam theory
 * grad(t) = grad(t - 1) + calculated_grad(t)
 */
OMStatus SGD::handle(core::OMRuntimeStorage &backward_storage, core::OMRuntimeContext &context,
                     core::OMRuntimeStorage &storage)
{
  auto &backward_tensor_to_data = backward_storage.getTensorIndexToData();
  // Check is allocated or not helper buffers
  if (_tensor_index_to_gradient.empty())
  {
    // If not - let's just move it with calculations
    // Goes over all calculated gradients
    // Warning: assume that backward storage at this moment contains only weigths gradients -
    // This should be done due to execution plan work
    for (auto &tensor_to_data : backward_tensor_to_data)
    {
      // Move data
      _tensor_index_to_gradient[tensor_to_data.first] = tensor_to_data.second;
      tensor_to_data.second = nullptr;
    }
    backward_tensor_to_data.clear();
  }
  else
  {
    // Goes over all calculated gradients
    // Warning: assume that backward storage at this moment contains only weigths gradients -
    // This should be done due to execution plan work
    for (auto &tensor_to_data : backward_tensor_to_data)
    {
      auto tensor = context.getTensorByIndex(tensor_to_data.first);
      auto num_elements = core::OMRuntimeShape(tensor).flatSize();

#ifndef DIS_DYN_SHAPES
      int32_t dynamic_tensor_size = storage.getDynamicRuntimeShape(tensor_to_data.first).flatSize();
      if (dynamic_tensor_size != 0)
        num_elements = dynamic_tensor_size;
#endif // DIS_DYN_SHAPES

      auto *grad_data = reinterpret_cast<float *>(_tensor_index_to_gradient[tensor_to_data.first]);
      auto *calculated_data = reinterpret_cast<float *>(tensor_to_data.second);

      for (uint32_t i = 0; i < num_elements; ++i)
      {
        grad_data[i] += calculated_data[i];
      }
    }
  }

  return Ok;
}

/*
 * Update weights according to Adam theory:
 *
 * w(t + 1) = w(t) - lambda * grad(t) / batch_size
 */
OMStatus SGD::updateWeights(
  const onert_micro::OMTrainingContext &training_config, core::OMRuntimeContext &context,
  core::OMRuntimeStorage &storage,
  std::unordered_map<uint16_t, core::OpTrainableRankType> &tensor_index_to_rank_type_map)
{
  assert(!_tensor_index_to_gradient.empty());
  if (_tensor_index_to_gradient.empty())
    return UnknownError;

  for (auto &tensor_to_data : _tensor_index_to_gradient)
  {
    auto tensor = context.getTensorByIndex(tensor_to_data.first);
    core::OMRuntimeShape shape(tensor);
    auto num_elements = shape.flatSize();

    auto original_d = shape.dims(0);

#ifndef DIS_DYN_SHAPES
    int32_t dynamic_tensor_size = storage.getDynamicRuntimeShape(tensor_to_data.first).flatSize();
    if (dynamic_tensor_size != 0)
      num_elements = dynamic_tensor_size;
#endif // DIS_DYN_SHAPES

    auto *grad_data = reinterpret_cast<float *>(tensor_to_data.second);
    uint8_t *weight_data = nullptr;
    if (context.getConstDataByTensorIndex(&weight_data, tensor_to_data.first) != Ok)
      return UnknownError;

    assert(weight_data != nullptr);
    if (weight_data == nullptr)
      return UnknownError;

    auto *f_weight_data = reinterpret_cast<float *>(weight_data);
    float lambda = training_config.learning_rate;
    const uint32_t batch_size = training_config.batch_size;
    auto train_it = tensor_index_to_rank_type_map.find(tensor_to_data.first);
    core::OpTrainableRankType rank = train_it == tensor_index_to_rank_type_map.end()
                                       ? core::OpTrainableRankType::ALL
                                       : core::OpTrainableRankType(train_it->second);
    auto depth_bounds = getUpLowerWeightTensorDepth(rank, original_d);

    assert(batch_size != 0);

    for (uint32_t i = 0; i < num_elements; ++i)
    {
      f_weight_data[i] -= (lambda * grad_data[i]) / (static_cast<float>(batch_size));
    }
  }
  return Ok;
}
