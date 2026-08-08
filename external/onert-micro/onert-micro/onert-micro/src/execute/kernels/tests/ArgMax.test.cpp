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
#include "test_models/arg_max/FloatArgMaxKernel.h"
#include "test_models/arg_max/NegArgMaxKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

class ArgMaxTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(ArgMaxTest, Float_P)
{
  test_model::TestDataFloatArgMax test_data_float_arg_max;
  std::vector<int> output_data_vector =
    onert_micro::execute::testing::checkKernel<float>(1, &test_data_float_arg_max);
  EXPECT_TRUE(output_data_vector == test_data_float_arg_max.get_output_data_by_index(0));
}

TEST_F(ArgMaxTest, InputTypeMismatch_NEG)
{
  onert_micro::test_model::TestDataAxisOutputTypeMismatchArgMax test_data_kernel;
  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
