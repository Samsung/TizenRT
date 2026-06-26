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

#ifndef ONERT_MICRO_EXECUTE_TESTUTILS_H
#define ONERT_MICRO_EXECUTE_TESTUTILS_H

#include "test_models/TestDataBase.h"
#include "OMInterpreter.h"

#include <type_traits>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace onert_micro
{
namespace execute
{
namespace testing
{

// Array version of `::testing::FloatNear` matcher.
::testing::Matcher<std::vector<float>> FloatArrayNear(const std::vector<float> &values,
                                                      float max_abs_error = 1.0e-5f);

template <typename T, typename U = T>
std::vector<U> checkKernel(uint32_t num_inputs,
                           onert_micro::test_model::TestDataBase<T, U> *test_data_base)
{
  onert_micro::OMInterpreter interpreter;
  onert_micro::OMConfig config;

  interpreter.importModel(reinterpret_cast<const char *>(test_data_base->get_model_ptr()), config);

  assert(num_inputs == interpreter.getNumberOfInputs());

  interpreter.reset();
  interpreter.allocateInputs();

  for (uint32_t i = 0; i < num_inputs; ++i)
  {
    T *input_data = reinterpret_cast<T *>(interpreter.getInputDataAt(i));

    // Set input data
    {
      std::copy(test_data_base->get_input_data_by_index(i).begin(),
                test_data_base->get_input_data_by_index(i).end(), input_data);
    }
  }

  interpreter.run(config);

  U *output_data = reinterpret_cast<U *>(interpreter.getOutputDataAt(0));
  const size_t num_elements = interpreter.getOutputSizeAt(0);
  std::vector<U> output_data_vector(output_data, output_data + num_elements);
  return output_data_vector;
}

void checkNEGSISOKernel(onert_micro::test_model::NegTestDataBase *test_data_base);

} // namespace testing
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_TESTUTILS_H
