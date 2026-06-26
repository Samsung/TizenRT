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
#include "test_models/gather/FloatGatherKernel.h"
#include "test_models/gather/IntGatherKernel.h"
#include "test_models/gather/S8GatherKernel.h"
#include "test_models/gather/NegGatherKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class GatherTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(GatherTest, Gather_Float_P)
{
  onert_micro::test_model::TestDataFloatGather test_data_kernel;
  std::vector<float> output_data_vector =
    onert_micro::execute::testing::checkKernel<float>(1, &test_data_kernel);
  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(GatherTest, Gather_Int_P)
{
  onert_micro::test_model::TestDataIntGather test_data_kernel;
  std::vector<int32_t> output_data_vector =
    onert_micro::execute::testing::checkKernel<int32_t>(1, &test_data_kernel);
  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(GatherTest, Gather_S8_P)
{
  onert_micro::test_model::TestDataS8Gather test_data_kernel;
  std::vector<int8_t> output_data_vector =
    onert_micro::execute::testing::checkKernel<int8_t>(1, &test_data_kernel);
  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(GatherTest, Input_output_type_mismatch_NEG)
{
  onert_micro::test_model::NegTestDataInputOutputTypeMismatchGatherKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(GatherTest, Wrong_position_type_NEG)
{
  onert_micro::test_model::NegTestDataWrongPositionTypeGatherKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(GatherTest, Wrong_scale_mismatch_NEG)
{
  onert_micro::test_model::NegTestDataInputOutputScaleMismatchGatherKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

TEST_F(GatherTest, Wrong_axis_NEG)
{
  onert_micro::test_model::NegTestDataWrongAxisGatherKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

// TODO: add S8 test

} // namespace testing
} // namespace execute
} // namespace onert_micro
