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

#include "core/OMDataType.h"
#include "core/memory/OMMemoryManager.h"
#include "core/train/OMTrainingHandler.h"
#include "train/losses_functions/MSE.h"
#include "train/losses_functions/CrossEntropy.h"
#include "train/losses_functions/SparseCrossEntropy.h"
#include "train/metrics/MSE.h"
#include "train/metrics/CrossEntropy.h"
#include "train/metrics/Accuracy.h"
#include "train/metrics/MAE.h"
#include "train/metrics/SparseCrossEntropyAccuracy.h"

using namespace onert_micro::core::train;
using namespace onert_micro::core;
using namespace onert_micro::train;
using namespace onert_micro;

/*
 * Calculate backpropagation error between calculated data in forward graph and target data
 */
OMStatus OMTrainingHandler::handleError(const OMConfig &config, OMRuntimeStorage &forward_storage,
                                        OMRuntimeStorage &backward_storage,
                                        OMRuntimeContext &context, uint32_t batch_num)
{
  auto forward_outputs = context.getCircleOutputs();
  // Go over all outputs
  for (uint32_t i = 0; i < forward_outputs->size(); ++i)
  {
    const auto forward_output_index = forward_outputs->operator[](i);
    const auto forward_output_tensor = context.getTensorByIndex(forward_output_index);

    OMRuntimeShape shape(forward_output_tensor);
    const auto flat_size = shape.flatSize();

    // Check type
    assert(forward_output_tensor->type() == circle::TensorType_FLOAT32 && "Unsupported type");
    if (forward_output_tensor->type() != circle::TensorType_FLOAT32)
      return UnsupportedType;

    // Get calculated data
    uint8_t *calculated_data = nullptr;
    OMStatus status = forward_storage.getDataByTensorIndex(&calculated_data, forward_output_index);
    assert(calculated_data != nullptr);

    OMLoss loss_type = config.training_context.loss;

    // Get target data
    auto data_type_size = sizeof(core::OMDataType(forward_output_tensor->type()));
    size_t offset = batch_num * data_type_size * flat_size;

    // Need to check loss type to control proper offset.
    if (loss_type == SPARSE_CROSS_ENTROPY)
    {
      offset = batch_num * data_type_size;
    }
    uint8_t *target_data = _training_storage.getTargetData(i) + offset;

    // Allocate data for error gradient for current calculated data and target data
    uint8_t *output_grad_data;
    core::memory::OMMemoryManager::allocateMemory(flat_size * data_type_size, &output_grad_data);
    // Save error gradient into backward storage associated with current output tensor index
    backward_storage.saveDataToTensorIndex(output_grad_data, forward_output_index);

    // Handle different loss types
    switch (loss_type)
    {
      case MSE:
      {
        losses_functions::MSE::calculateErrorBackpropagation(
          flat_size, reinterpret_cast<float *>(calculated_data),
          reinterpret_cast<float *>(target_data), reinterpret_cast<float *>(output_grad_data));
        break;
      }
      case CROSS_ENTROPY:
      {
        losses_functions::CrossEntropy::calculateErrorBackpropagation(
          flat_size, reinterpret_cast<float *>(calculated_data),
          reinterpret_cast<float *>(target_data), reinterpret_cast<float *>(output_grad_data));
        break;
      }
      case SPARSE_CROSS_ENTROPY:
      {
        losses_functions::SparseCrossEntropy::calculateErrorBackpropagation(
          flat_size, reinterpret_cast<float *>(calculated_data),
          reinterpret_cast<float *>(target_data), reinterpret_cast<float *>(output_grad_data));
        break;
      }
      default:
      {
        assert(false && "Unsupported loss type");
        return UnsupportedType;
      }
    }
  }

  return Ok;
}

/*
 * Update weights with current optimizer logic
 */
OMStatus OMTrainingHandler::updateWeights(const OMConfig &config, OMRuntimeContext &context,
                                          OMRuntimeStorage &storage)
{
  OMStatus status = Ok;

  // Chose optimizer type
  switch (config.training_context.optimizer)
  {
    case SGD:
    {
      auto *sgd_optimizer = _training_storage.getSGD();
      assert(sgd_optimizer != nullptr);
      if (sgd_optimizer == nullptr)
        return UnknownError;

      status = sgd_optimizer->updateWeights(config.training_context, context, storage,
                                            _training_storage.getTensorIndexToRankTypeTable());
      assert(status == Ok);
      // Reset values
#ifdef OM_MEMORY_ESTIMATE
      sgd_optimizer->reset(context, storage);
#else
      sgd_optimizer->reset();
#endif // OM_MEMORY_ESTIMATE
      break;
    }
    case ADAM:
    {
      auto *adam_optimizer = _training_storage.getAdam();
      assert(adam_optimizer != nullptr);
      if (adam_optimizer == nullptr)
        return UnknownError;
      status = adam_optimizer->updateWeights(config.training_context, context, storage,
                                             _training_storage.getTensorIndexToRankTypeTable());
      assert(status == Ok);
      // Reset values
#ifdef OM_MEMORY_ESTIMATE
      adam_optimizer->reset(context, storage);
#else
      adam_optimizer->reset();
#endif // OM_MEMORY_ESTIMATE
      break;
    }
    default:
    {
      assert(false && "Unsupported type");
      return UnsupportedType;
    }
  }

  return status;
}

/*
 * Update optimizer state
 *
 * WARNING: It is assumed that the backward_storage contains only calculated gradients (this
 * execution plane creator work).
 */
OMStatus OMTrainingHandler::updateOptimizerState(const OMConfig &config,
                                                 OMRuntimeStorage &backward_storage,
                                                 OMRuntimeContext &context)
{
  OMStatus status = Ok;

  switch (config.training_context.optimizer)
  {
    case SGD:
    {
      auto *sgd_optimizer = _training_storage.getSGD();
      assert(sgd_optimizer != nullptr);
      if (sgd_optimizer == nullptr)
        return UnknownError;

      sgd_optimizer->handle(backward_storage, context, backward_storage);
      break;
    }
    case ADAM:
    {
      auto *adam_optimizer = _training_storage.getAdam();
      assert(adam_optimizer != nullptr);
      if (adam_optimizer == nullptr)
        return UnknownError;

      adam_optimizer->handle(backward_storage, context, backward_storage);
      break;
    }
    default:
    {
      assert(false && "Unsupported type");
      return UnsupportedType;
    }
  }

  return status;
}

void OMTrainingHandler::reset() { _training_storage.reset(); }

#ifdef OM_MEMORY_ESTIMATE
void OMTrainingHandler::reset(core::OMRuntimeContext &context, core::OMRuntimeStorage &storage)
{
  _training_storage.reset(context, storage);
}
#endif // OM_MEMORY_ESTIMATE

/*
 * Evaluate metric according OMMetrics and save it into metric_val
 *
 * Warning: 1) assume that all metric_val for all OMMetrics types actually are float values.
 *          2) metric_val should be initialized with some value before calling this method due to
 *             after calculation for current batch_num (the sequence number of the current sample)
 *             this value is added to metric_val
 */
OMStatus OMTrainingHandler::evaluateMetric(OMMetrics metric, void *metric_val,
                                           OMRuntimeStorage &storage, OMRuntimeContext &context,
                                           uint32_t batch_num)
{
  // Go over all outputs and calculate metric
  auto forward_outputs = context.getCircleOutputs();
  for (uint32_t i = 0; i < forward_outputs->size(); ++i)
  {
    // Get output tensor
    const auto forward_output_index = forward_outputs->operator[](i);
    const auto forward_output_tensor = context.getTensorByIndex(forward_output_index);

    OMRuntimeShape shape(forward_output_tensor);
    const auto flat_size = shape.flatSize();

    // Check type
    assert(forward_output_tensor->type() == circle::TensorType_FLOAT32 && "Unsupported type");
    if (forward_output_tensor->type() != circle::TensorType_FLOAT32)
      return UnsupportedType;

    // Get calculated data
    uint8_t *calculated_data = nullptr;
    OMStatus status = storage.getDataByTensorIndex(&calculated_data, forward_output_index);
    assert(calculated_data != nullptr);

    // Get target data
    /** NOTE:
     * This offset will always return 0 if the MODEL OUTPUT is returning 1 value of prediction.
     * (forward_output->size() == length of output vector.)
     * one-hot: size == target_numbers
     * Sparse cross : size == 1
     */
    size_t offset = batch_num * sizeof(core::OMDataType(forward_output_tensor->type())) * flat_size;
    uint8_t *target_data = _training_storage.getTargetData(i) + offset;

    // Note: always cast it to float
    float *f_metric_val = reinterpret_cast<float *>(metric_val);
    switch (metric)
    {
      case MSE_METRICS:
      {
        // Note: sum up new calculated value for current sample
        *f_metric_val +=
          metrics::MSE::calculateValue(flat_size, reinterpret_cast<float *>(calculated_data),
                                       reinterpret_cast<float *>(target_data));
        break;
      }
      case MAE_METRICS:
      {
        // Note: sum up new calculated value for current sample
        *f_metric_val +=
          metrics::MAE::calculateValue(flat_size, reinterpret_cast<float *>(calculated_data),
                                       reinterpret_cast<float *>(target_data));
        break;
      }
      case CROSS_ENTROPY_METRICS:
      {
        // Note: sum up new calculated value for current sample
        *f_metric_val += metrics::CrossEntropy::calculateValue(
          flat_size, reinterpret_cast<float *>(calculated_data),
          reinterpret_cast<float *>(target_data));
        break;
      }
      case ACCURACY:
      {
        // Note: sum up new calculated value for current sample
        *f_metric_val +=
          metrics::Accuracy::calculateValue(flat_size, reinterpret_cast<float *>(calculated_data),
                                            reinterpret_cast<float *>(target_data));
        break;
      }
      case SPARSE_CROSS_ENTROPY_ACCURACY:
      {
        // Note: sum up new calculated value for current sample
        *f_metric_val += metrics::SparseCrossEntropyAccuracy::calculateValue(
          flat_size, reinterpret_cast<float *>(calculated_data),
          reinterpret_cast<float *>(target_data));
        break;
      }
      default:
      {
        assert(false && "Unsupported loss type");
        return UnsupportedType;
      }
    }
  }

  return Ok;
}
