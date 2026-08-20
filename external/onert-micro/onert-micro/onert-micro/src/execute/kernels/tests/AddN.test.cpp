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
#include "test_models/add_n/FloatAddNKernel.h"
#include "test_models/add_n/NegAddNKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

class AddNTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(AddNTest, Float_P)
{
  test_model::TestDataFloatAddN test_data_float_add_n;
  std::vector<float> output_data_vector =
    onert_micro::execute::testing::checkKernel<float>(3, &test_data_float_add_n);
  EXPECT_THAT(output_data_vector,
              FloatArrayNear(test_data_float_add_n.get_output_data_by_index(0), 0.0001f));
}

TEST_F(AddNTest, InputTypeMismatch_NEG)
{
  onert_micro::test_model::TestDataInputTypeMismatchAddN test_data_kernel;
  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
