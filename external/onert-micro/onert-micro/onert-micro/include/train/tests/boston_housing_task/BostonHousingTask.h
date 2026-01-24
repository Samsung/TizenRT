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

#ifndef ONERT_MICRO_TRAIN_TESTS_BOSTON_HOUSING_TASK_H
#define ONERT_MICRO_TRAIN_TESTS_BOSTON_HOUSING_TASK_H

#include "train/tests/OMTestTrainBase.h"
#include "train/tests/models/boston_housing.h"
#include "train/tests/boston_housing_task/data/test_target.h"
#include "train/tests/boston_housing_task/data/test_input.h"
#include "train/tests/boston_housing_task/data/train_target.h"
#include "train/tests/boston_housing_task/data/train_input.h"

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

const size_t NUM_TRAIN_SAMPLES = 404;
const size_t NUM_TEST_SAMPLES = 102;

} // namespace

template <typename T, typename U = T> class BostonHousingTask : public OMTestTrainBase<T, U>
{
public:
  BostonHousingTask()
  {
    // Set model
    _train_model_ptr.resize(models::boston_housing_model_size);
    std::memcpy(_train_model_ptr.data(), models::boston_housing_model,
                models::boston_housing_model_size);
  }

  // Get ptr to trained model
  char *getModelPtr() final { return _train_model_ptr.data(); }

  // Return model size
  size_t getModelSize() final { return _train_model_ptr.size(); }

  // Return num train samples
  size_t getTrainNumSamples() final { return NUM_TRAIN_SAMPLES; };

  // Return num test samples
  size_t getTestNumSamples() final { return NUM_TEST_SAMPLES; }

  // Read train input data with cur size and cur offset
  std::vector<T> readTrainInputData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::train_input + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }
  // Read train target data with cur size and cur offset
  std::vector<U> readTrainTargetData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::train_target + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

  // Read test input data with cur size and cur offset
  std::vector<T> readTestInputData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::test_input + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

  // Read test target data with cur size and cur offset
  std::vector<T> readTestTargetData(size_t size, size_t offset) final
  {
    std::vector<T> result(size);

    auto *cur_ptr = data::test_target + offset;

    std::memcpy(result.data(), cur_ptr, size);
    return result;
  }

private:
  std::vector<char> _train_model_ptr = {};
};

} // namespace test
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TESTS_BOSTON_HOUSING_TASK_H
