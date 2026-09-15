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
#include "test_models/l2pool2d/FloatL2Pool2DKernel.h"
#include "test_models/l2pool2d/NegL2Pool2DKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class L2Pool2DTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(L2Pool2DTest, Float_P)
{
  onert_micro::test_model::TestDataFloatL2Pool2D test_data_kernel;
  std::vector<float> output_data_vector =
    onert_micro::execute::testing::checkKernel<float>(1, &test_data_kernel);
  EXPECT_THAT(output_data_vector,
              FloatArrayNear(test_data_kernel.get_output_data_by_index(0), 0.0001f));
}

TEST_F(L2Pool2DTest, Input_output_type_mismatch_NEG)
{
  onert_micro::test_model::NegTestDataInputOutputTypeMismatchL2Pool2DKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
