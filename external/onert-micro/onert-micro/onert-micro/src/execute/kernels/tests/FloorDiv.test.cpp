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
#include "test_models/floordiv/FloatFloorDivKernel.h"
#include "test_models/floordiv/NegFloorDivKernel.h"

#include "PALFloorDiv.h"
#include <array>
#include <numeric>

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class FloorDivTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(FloorDivTest, Float_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    onert_micro::test_model::TestDataFloatFloorDiv test_data_kernel(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_kernel);

    EXPECT_THAT(output_data_vector,
                FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    onert_micro::test_model::TestDataFloatFloorDiv test_data_kernel(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_kernel);

    EXPECT_THAT(output_data_vector,
                FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }
}

TEST_F(FloorDivTest, Wrong_Input1_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput1WrongTypeFloorDiv test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(FloorDivTest, Wrong_Input2_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput2WrongTypeFloorDiv test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(FloorDivTest, PALFloat_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    onert_micro::test_model::TestDataFloatFloorDiv test_data_kernel(is_with_broadcast);

    const auto &input1 = test_data_kernel.get_input_data_by_index(0);
    const auto &input2 = test_data_kernel.get_input_data_by_index(1);

    const auto num_elements = input1.size();
    EXPECT_EQ(num_elements, input2.size());

    std::vector<float> output = std::vector<float>(num_elements);
    pal::FloorDiv(num_elements, input1.data(), input2.data(), const_cast<float *>(output.data()));

    EXPECT_THAT(output, FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }

  // With broadcast
  {
    const bool is_with_broadcast = true;
    onert_micro::test_model::TestDataFloatFloorDiv test_data_kernel(is_with_broadcast);

    const auto &input1 = test_data_kernel.get_input_data_by_index(0);
    const auto &input2 = test_data_kernel.get_input_data_by_index(1);

    const int32_t shape[2] = {2, 5};
    const int32_t shape_broadcast[2] = {2, 1};

    assert(input1.size() ==
           std::accumulate(std::begin(shape), std::end(shape), 1, std::multiplies<float>()));
    assert(input2.size() == std::accumulate(std::begin(shape_broadcast), std::end(shape_broadcast),
                                            1, std::multiplies<float>()));

    std::vector<float> output = std::vector<float>(
      std::accumulate(std::begin(shape), std::end(shape), 1, std::multiplies<float>()));
    pal::BroadcastFloorDiv4DSlow(
      core::OMRuntimeShape{2, shape}, input1.data(), core::OMRuntimeShape{2, shape_broadcast},
      input2.data(), core::OMRuntimeShape{2, shape}, const_cast<float *>(output.data()));

    EXPECT_THAT(output, FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
  }
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
