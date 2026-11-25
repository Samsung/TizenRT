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
#include "test_models/sub/FloatSubKernel.h"
#include "test_models/sub/NegSubKernel.h"
#include "test_models/sub/IntSubKernel.h"
#include "test_models/sub/S8SubKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class SubTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(SubTest, INT_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataIntSub test_data_add_no_broadcasting(is_with_broadcast);
    std::vector<int32_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int32_t>(2, &test_data_add_no_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_no_broadcasting.get_output_data_by_index(0));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    test_model::TestDataIntSub test_data_add_with_broadcasting(is_with_broadcast);
    std::vector<int32_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int32_t>(2, &test_data_add_with_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_with_broadcasting.get_output_data_by_index(0));
  }
}

TEST_F(SubTest, S8_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataS8Sub test_data_add_no_broadcasting(is_with_broadcast);
    std::vector<int8_t> output_data_vector =
      onert_micro::execute::testing::checkKernel<int8_t>(2, &test_data_add_no_broadcasting);
    EXPECT_THAT(output_data_vector, test_data_add_no_broadcasting.get_output_data_by_index(0));
  }
}

TEST_F(SubTest, Float_P)
{
  // No broadcast
  {
    const bool is_with_broadcast = false;
    test_model::TestDataFloatSub test_data_float_add_no_broadcasting(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_float_add_no_broadcasting);
    EXPECT_THAT(
      output_data_vector,
      FloatArrayNear(test_data_float_add_no_broadcasting.get_output_data_by_index(0), 0.0001f));
  }
  // With broadcast
  {
    const bool is_with_broadcast = true;
    test_model::TestDataFloatSub test_data_float_add_with_broadcasting(is_with_broadcast);
    std::vector<float> output_data_vector =
      onert_micro::execute::testing::checkKernel<float>(2, &test_data_float_add_with_broadcasting);
    EXPECT_THAT(
      output_data_vector,
      FloatArrayNear(test_data_float_add_with_broadcasting.get_output_data_by_index(0), 0.0001f));
  }
}

TEST_F(SubTest, Input_output_type_mismatch_NEG)
{
  onert_micro::test_model::NegTestDataInputOutputTypeMismatchSubKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(SubTest, Inputs_type_mismatch_NEG)
{
  onert_micro::test_model::NegTestDataInputsTypeMismatchSubKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(SubTest, No_quant_params_NEG)
{
  onert_micro::test_model::NegTestDataNoQuantParamsSubKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
