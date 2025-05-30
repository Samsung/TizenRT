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
#include "test_models/less/FloatLessKernel.h"
#include "test_models/less/IntLessKernel.h"
#include "test_models/less/QuantLessKernel.h"
#include "test_models/less/S8LessKernel.h"
#include "test_models/less/NegTestDataLessKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{
using namespace testing;

class LessTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(LessTest, FloatNoBroadcast_P)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataFloatLess test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<float, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, FloatWithBroadcast_P)
{
  const bool is_with_broadcast = true;
  onert_micro::test_model::TestDataFloatLess test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<float, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, FloatNoBroadcast_NEG)
{

  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataFloatLess test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<float, bool>(2, &test_data_kernel)), "");
}

TEST_F(LessTest, FloatWithBroadcast_NEG)
{
  const bool is_with_broadcast = true;
  onert_micro::test_model::TestDataFloatLess test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<float, bool>(2, &test_data_kernel)), "");
}

TEST_F(LessTest, IntWithBroadcast_P)
{
  const bool is_with_broadcast = true;
  onert_micro::test_model::TestDataIntLess test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<int32_t, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, IntNoBroadcast_P)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataIntLess test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<int32_t, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, IntWithBroadcast_NEG)
{
  const bool is_with_broadcast = true;
  onert_micro::test_model::TestDataIntLess test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<int32_t, bool>(2, &test_data_kernel)),
               "");
}

TEST_F(LessTest, IntNoBroadcast_NEG)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataIntLess test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<int32_t, bool>(2, &test_data_kernel)),
               "");
}

TEST_F(LessTest, Quant_P)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataQuantLess test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<uint8_t, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, S8_P)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataS8Less test_data_kernel(is_with_broadcast, false);

  std::vector<bool> output_data_vector =
    onert_micro::execute::testing::checkKernel<int8_t, bool>(2, &test_data_kernel);

  EXPECT_THAT(output_data_vector, test_data_kernel.get_output_data_by_index(0));
}

TEST_F(LessTest, S8_NEG)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataS8Less test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<int8_t, bool>(2, &test_data_kernel)),
               "");
}

TEST_F(LessTest, Quant_NEG)
{
  const bool is_with_broadcast = false;
  onert_micro::test_model::TestDataQuantLess test_data_kernel(is_with_broadcast, true);

  EXPECT_DEATH((onert_micro::execute::testing::checkKernel<uint8_t, bool>(2, &test_data_kernel)),
               "");
}

TEST_F(LessTest, Wrong_Output_Type_NEG)
{
  onert_micro::test_model::NegTestDataLessKernel test_data_kernel;

  EXPECT_DEATH(checkNEGSISOKernel(&test_data_kernel), "");
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
