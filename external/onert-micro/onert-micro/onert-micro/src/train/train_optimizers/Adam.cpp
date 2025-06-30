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
#include "train/train_optimizers/Adam.h"
#include "core/memory/OMMemoryManager.h"
#include "core/OMRuntimeShape.h"
#include "core/OMDataType.h"

#include <cmath>

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
void Adam::fullReset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage)
{
  for (auto &cur_tensor_index_data : _tensor_to_exponent_avg)
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
  _tensor_to_exponent_avg.clear();

  for (auto &cur_tensor_index_data : _tensor_to_exponent_avg_squares)
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
  _tensor_to_exponent_avg_squares.clear();

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
  auto tmp = core::memory::OMMemoryManager::cur_memory_allocated;
  _tensor_index_to_gradient.clear();
}

void Adam::reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage)
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

void Adam::fullReset()
{
  for (auto &cur_tensor_index_data : _tensor_to_exponent_avg)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;

    core::memory::OMMemoryManager::deallocateMemory(allocated_data);
  }
  _tensor_to_exponent_avg.clear();

  for (auto &cur_tensor_index_data : _tensor_to_exponent_avg_squares)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;

    core::memory::OMMemoryManager::deallocateMemory(allocated_data);
  }
  _tensor_to_exponent_avg_squares.clear();

  for (auto &cur_tensor_index_data : _tensor_index_to_gradient)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;

    core::memory::OMMemoryManager::deallocateMemory(allocated_data);
  }
  _tensor_index_to_gradient.clear();
}

void Adam::reset()
{
  for (auto &cur_tensor_index_data : _tensor_index_to_gradient)
  {
    uint8_t *allocated_data = cur_tensor_index_data.second;

    core::memory::OMMemoryManager::deallocateMemory(allocated_data);
  }
  _tensor_index_to_gradient.clear();
}

uint8_t *Adam::getExponentAvgDataByTensorIndex(uint16_t tensor_index)
{
  auto it = _tensor_to_exponent_avg.find(tensor_index);
  if (it == _tensor_to_exponent_avg.end())
    return nullptr;

  return it->second;
}

uint8_t *Adam::getExponentAvgSquaresDataByTensorIndex(uint16_t tensor_index)
{
  auto it = _tensor_to_exponent_avg_squares.find(tensor_index);
  if (it == _tensor_to_exponent_avg_squares.end())
    return nullptr;

  return it->second;
}

void Adam::setExponentAvgDataByTensorIndex(uint16_t tensor_index, uint8_t *data)
{
  assert(_tensor_to_exponent_avg.find(tensor_index) == _tensor_to_exponent_avg.end());
  assert(data != nullptr);

  _tensor_to_exponent_avg[tensor_index] = data;
}

void Adam::setExponentAvgSquaresDataByTensorIndex(uint16_t tensor_index, uint8_t *data)
{
  assert(_tensor_to_exponent_avg_squares.find(tensor_index) ==
         _tensor_to_exponent_avg_squares.end());
  assert(data != nullptr);

  _tensor_to_exponent_avg_squares[tensor_index] = data;
}

/*
 * Update internal states according to calculated gradients using Adam theory
 * grad(t) = grad(t - 1) + calculated_grad(t)
 */
OMStatus Adam::handle(core::OMRuntimeStorage &backward_storage, core::OMRuntimeContext &context,
                      core::OMRuntimeStorage &storage)
{
  auto &backward_tensor_to_data = backward_storage.getTensorIndexToData();

  // Check is allocated or not helper buffers
  if (_tensor_to_exponent_avg_squares.empty())
  {
    // If not - let's allocate it
    assert(_tensor_to_exponent_avg.empty() == true);
    // Goes over all calculated gradients
    // Warning: assume that backward storage at this moment contains only weighs gradients -
    // This should be done due to execution plan work
    for (auto &tensor_to_data : backward_tensor_to_data)
    {
      auto tensor_index = tensor_to_data.first;
      auto tensor = context.getTensorByIndex(tensor_index);
      auto num_elements = core::OMRuntimeShape(tensor).flatSize();

#ifndef DIS_DYN_SHAPES
      int32_t dynamic_tensor_size = storage.getDynamicRuntimeShape(tensor_index).flatSize();
      if (dynamic_tensor_size != 0)
        num_elements = dynamic_tensor_size;
#endif // DIS_DYN_SHAPES

      auto tensor_size = num_elements * sizeof(core::OMDataType(tensor->type()));

      // Allocate data for exponent calculation
      uint8_t *exponent_data = nullptr;
      OMStatus status = core::memory::OMMemoryManager::allocateMemory(tensor_size, &exponent_data);
      assert(status == Ok);
      if (status != Ok)
        return UnknownError;
      // Set to zeros
      std::memset(exponent_data, 0, tensor_size);
      _tensor_to_exponent_avg[tensor_to_data.first] = exponent_data;

      // Allocate data for exponent square calculation
      uint8_t *exponent_square_data = nullptr;
      status = core::memory::OMMemoryManager::allocateMemory(tensor_size, &exponent_square_data);
      assert(status == Ok);
      if (status != Ok)
        return UnknownError;
      // Set to zeros
      std::memset(exponent_square_data, 0, tensor_size);
      _tensor_to_exponent_avg_squares[tensor_to_data.first] = exponent_square_data;
    }
  }

  // Check is allocated or not helper buffer
  if (_tensor_index_to_gradient.empty())
  {
    // If not - let's just move it with calculations
    // Goes over all calculated gradients
    // Warning: assume that backward storage at this moment contains only weights gradients -
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
    // Warning: assume that backward storage at this moment contains only weighs gradients -
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
 * Update internal states according to calculated gradients using Adam theory
 * m(t) = beta_1 * m(t-1) + (1 - beta_1) * calculated_gradients(t)
 * v(t) = beta_2 * v(t-1) + (1 - beta_2) * (calculated_gradients(t)) ^ 2

 * Update weights according to Adam theory:
 * m`(t) = m(t) / (1 - (beta_1) ^ t)
 * v`(t) = v(t) / (1 - (beta_2) ^ t)
 *
 * w(t + 1) = w(t) - lambda * m`(t) / (sqrt(v` + epsilon))
 */
OMStatus Adam::updateWeights(
  const onert_micro::OMTrainingContext &training_config, core::OMRuntimeContext &context,
  core::OMRuntimeStorage &storage,
  std::unordered_map<uint16_t, core::OpTrainableRankType> &tensor_index_to_rank_type_map)
{
  assert(!_tensor_index_to_gradient.empty());
  for (auto &tensor_to_data : _tensor_index_to_gradient)
  {
    auto exponent_squares_it = _tensor_to_exponent_avg_squares.find(tensor_to_data.first);
    if (exponent_squares_it == _tensor_to_exponent_avg_squares.end())
      return UnknownError;

    auto exponent_it = _tensor_to_exponent_avg.find(tensor_to_data.first);
    if (exponent_it == _tensor_to_exponent_avg.end())
      return UnknownError;

    auto tensor = context.getTensorByIndex(tensor_to_data.first);
    core::OMRuntimeShape shape(tensor);

    auto original_d = shape.dims(0);

    auto num_elements = core::OMRuntimeShape(tensor).flatSize();

#ifndef DIS_DYN_SHAPES
    int32_t dynamic_tensor_size = storage.getDynamicRuntimeShape(tensor_to_data.first).flatSize();
    if (dynamic_tensor_size != 0)
      num_elements = dynamic_tensor_size;
#endif // DIS_DYN_SHAPES

    auto *exponent_data = reinterpret_cast<float *>(exponent_it->second);
    auto *exponent_square_data = reinterpret_cast<float *>(exponent_squares_it->second);
    auto *calculated_data = reinterpret_cast<float *>(tensor_to_data.second);
    float beta = training_config.beta;
    float beta_squares = training_config.beta_squares;
    auto batches = static_cast<float>(training_config.batch_size);
    for (uint32_t i = 0; i < num_elements; ++i)
    {
      const auto cur_val = calculated_data[i];
      exponent_data[i] = beta * exponent_data[i] + (1 - beta) * cur_val;
      exponent_square_data[i] =
        beta_squares * exponent_square_data[i] + (1 - beta_squares) * cur_val * cur_val;
    }

    uint8_t *weight_data = nullptr;
    if (context.getConstDataByTensorIndex(&weight_data, tensor_to_data.first) != Ok)
      return UnknownError;

    assert(weight_data != nullptr);
    if (weight_data == nullptr)
      return UnknownError;

    auto *f_weight_data = reinterpret_cast<float *>(weight_data);
    float lambda = training_config.learning_rate;
    auto num_step = static_cast<float>(training_config.num_step);
    float beta_in_pow_batch = std::pow(beta, num_step);
    float beta_square_in_pow_batch = std::pow(beta_squares, num_step);
    float epsilon = training_config.epsilon;

    assert((1.f - beta_in_pow_batch) != 0);
    assert((1.f - beta_square_in_pow_batch) != 0);
    auto train_it = tensor_index_to_rank_type_map.find(tensor_to_data.first);
    core::OpTrainableRankType rank = train_it == tensor_index_to_rank_type_map.end()
                                       ? core::OpTrainableRankType::ALL
                                       : core::OpTrainableRankType(train_it->second);
    auto depth_bounds = getUpLowerWeightTensorDepth(rank, original_d);

    for (uint32_t i = 0; i < num_elements; ++i)
    {
      float exponent_corrected = exponent_data[i] / (1.f - beta_in_pow_batch);
      float exponent_square_corrected = exponent_square_data[i] / (1.f - beta_square_in_pow_batch);
      f_weight_data[i + depth_bounds.first] -=
        lambda * (exponent_corrected / (std::sqrt(exponent_square_corrected + epsilon)));
    }
  }

  return Ok;
}
