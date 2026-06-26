/*
 * Copyright (c) 2024 Samsung Electronics Co., Ltd. All Rights Reserved
 * Copyright 2017 The TensorFlow Authors. All Rights Reserved.
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
#include "test_models/minimum/FloatMinimumKernel.h"
#include "test_models/minimum/NegMinimumKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{
using namespace testing;

class MinimumTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(MinimumTest, Float_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    onert_micro::test_model::TestDataFloatMinimum test_data_kernel(is_with_broadcast);

    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float, float>(2, &test_data_kernel);

    EXPECT_THAT(output_data_vector,
                FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    onert_micro::test_model::TestDataFloatMinimum test_data_kernel(is_with_broadcast);

    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float, float>(2, &test_data_kernel);

    EXPECT_THAT(output_data_vector,
                FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }
}

TEST_F(MinimumTest, Wrong_Input1_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput1WrongTypeMinimum test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(MinimumTest, Wrong_Input2_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput2WrongTypeMinimum test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
