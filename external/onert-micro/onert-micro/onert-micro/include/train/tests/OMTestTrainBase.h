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

#ifndef ONERT_MICRO_TRAIN_TESTS_TEST_TRAIN_BASE_H
#define ONERT_MICRO_TRAIN_TESTS_TEST_TRAIN_BASE_H

#include <vector>
#include <cstddef>

namespace onert_micro
{
namespace train
{
namespace test
{

template <typename T, typename U = T> class OMTestTrainBase
{
public:
  virtual ~OMTestTrainBase() = default;

  // Get ptr to trained model
  virtual char *getModelPtr() = 0;

  // Return model size
  virtual size_t getModelSize() = 0;

  // Return num train samples
  virtual size_t getTrainNumSamples() = 0;

  // Return num test samples
  virtual size_t getTestNumSamples() = 0;

  // Read train input data with cur size and cur offset
  virtual std::vector<T> readTrainInputData(size_t size, size_t offset) = 0;

  // Read train target data with cur size and cur offset
  virtual std::vector<U> readTrainTargetData(size_t size, size_t offset) = 0;

  // Read test input data with cur size and cur offset
  virtual std::vector<T> readTestInputData(size_t size, size_t offset) = 0;

  // Read test target data with cur size and cur offset
  virtual std::vector<T> readTestTargetData(size_t size, size_t offset) = 0;
};

} // namespace test
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TESTS_TEST_TRAIN_BASE_H
