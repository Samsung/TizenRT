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

#include "execute/OMTestUtils.h"

#include "OMInterpreter.h"

using ::testing::FloatNear;
using ::testing::Matcher;

using namespace onert_micro;

Matcher<std::vector<float>> execute::testing::FloatArrayNear(const std::vector<float> &values,
                                                             float max_abs_error)
{
  std::vector<Matcher<float>> matchers;
  matchers.reserve(values.size());
  for (const float v : values)
  {
    matchers.emplace_back(FloatNear(v, max_abs_error));
  }
  return ElementsAreArray(matchers);
}

void execute::testing::checkNEGSISOKernel(onert_micro::test_model::NegTestDataBase *test_data_base)
{
  onert_micro::OMInterpreter interpreter;
  onert_micro::OMConfig config;

  interpreter.importModel(reinterpret_cast<const char *>(test_data_base->get_model_ptr()), config);
}
