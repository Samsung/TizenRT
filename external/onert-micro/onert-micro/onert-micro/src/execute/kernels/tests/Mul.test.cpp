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
#include "test_models/mul/FloatMulKernel.h"
#include "test_models/mul/NegMulKernel.h"
#include "test_models/mul/IntMulKernel.h"
#include "test_models/mul/QuantMulKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class MulTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(MulTest, INT_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataIntMul test_data_add_no_broadcasting(is_with_broadcast);
    std::vector<int32_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int32_t>(2, &test_data_add_no_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_no_broadcasting.get_output_data_by_index(0));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    test_model::TestDataIntMul test_data_add_with_broadcasting(is_with_broadcast);
    std::vector<int32_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int32_t>(2, &test_data_add_with_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_with_broadcasting.get_output_data_by_index(0));
  }
}

TEST_F(MulTest, INT8_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataInt8Mul test_data_add_no_broadcasting(is_with_broadcast);
    std::vector<int8_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int8_t>(2, &test_data_add_no_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_no_broadcasting.get_output_data_by_index(0));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    test_model::TestDataInt8Mul test_data_add_with_broadcasting(is_with_broadcast);
    std::vector<int8_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int8_t>(2, &test_data_add_with_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_with_broadcasting.get_output_data_by_index(0));
  }
}

TEST_F(MulTest, Float_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataFloatMul test_data_float_add_no_broadcasting(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_float_add_no_broadcasting);
    EXPECT_THAT(
      output_data_vector,
      FloatArrayNear(test_data_float_add_no_broadcasting.get_output_data_by_index(0), 0.0001f));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    test_model::TestDataFloatMul test_data_float_add_with_broadcasting(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_float_add_with_broadcasting);
    EXPECT_THAT(
      output_data_vector,
      FloatArrayNear(test_data_float_add_with_broadcasting.get_output_data_by_index(0), 0.0001f));
  }
}

TEST_F(MulTest, Wrong_Input1_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput1WrongTypeMul test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(MulTest, Wrong_Input2_Type_NEG)
{
  onert_micro::test_model::NegTestDataInput2WrongTypeMul test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(MulTest, Wrong_Ouput_Type_NEG)
{
  onert_micro::test_model::NegTestDataInt16TypeMul test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(MulTest, No_output_scale_param_NEG)
{
  onert_micro::test_model::NegTestQuantMulNoScaleKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
