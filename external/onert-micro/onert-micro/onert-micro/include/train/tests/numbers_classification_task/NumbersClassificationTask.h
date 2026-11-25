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

#ifndef ONERT_MICRO_TRAIN_TESTS_NUMBERS_CLASSIFICATION_TASK_H
#define ONERT_MICRO_TRAIN_TESTS_NUMBERS_CLASSIFICATION_TASK_H

#include "train/tests/OMTestTrainBase.h"
#include "train/tests/models/numbers_classification_model.h"
#include "train/tests/numbers_classification_task/data/train_input.h"
#include "train/tests/numbers_classification_task/data/train_target.h"

#include <vector>
#include <cstring>

namespace onert_micro
{
namespace train
{
namespace test
{

namespace
{

// Note: use only one dataset - train will be also for test
const size_t NUM_SAMPLES = 20;

} // namespace

template <typename T, typename U = T> class NumbersClassificationTask : public OMTestTrainBase<T, U>
{
public:
  NumbersClassificationTask()
  {
    // Set model
    _train_model_ptr.resize(models::numbers_classification_model_size);
    std::memcpy(_train_model_ptr.data(), models::numbers_classification_model,
                models::numbers_classification_model_size);
  }

  // Get ptr to trained model
  char *getModelPtr() final { return _train_model_ptr.data(); }

  // Return model size
  size_t getModelSize() final { return _train_model_ptr.size(); }

  // Return num train samples
  size_t getTrainNumSamples() final { return NUM_SAMPLES; };

  // Return num test samples
  size_t getTestNumSamples() final { return NUM_SAMPLES; }

  // Read train input data with cur size and cur offset
  std::vector<T> readTrainInputData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::numbers_classification_task_input_data + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }
  // Read train target data with cur size and cur offset
  std::vector<U> readTrainTargetData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::numbers_classification_task_target_data + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

  // Read test input data with cur size and cur offset
  // Note: use only one dataset - train will be also for test
  std::vector<T> readTestInputData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::numbers_classification_task_input_data + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

  // Read test target data with cur size and cur offset
  // Note: use only one dataset - train will be also for test
  std::vector<T> readTestTargetData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::numbers_classification_task_target_data + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

private:
  std::vector<char> _train_model_ptr = {};
};

} // namespace test
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TESTS_NUMBERS_CLASSIFICATION_TASK_H
