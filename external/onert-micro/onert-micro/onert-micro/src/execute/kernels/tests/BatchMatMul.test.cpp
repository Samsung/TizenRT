/*
 * Copyright (c) 2025 Samsung Electronics Co., Ltd. All Rights Reserved
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
#include "test_models/batchmatmul/FloatBMMKernel.h"

namespace onert_micro
{
namespace execute
{
namespace testing
{

using namespace testing;

class BatchMatmulTest : public ::testing::Test
{
  // Do nothing
};

TEST_F(BatchMatmulTest, Float32_P)
{
  test_model::TestDataFloatBMM test_data_bmm;
  std::vector<float> output_data_vector =
    onert_micro::execute::testing::checkKernel<float>(2, &test_data_bmm);
  EXPECT_THAT(output_data_vector, test_data_bmm.get_output_data_by_index(0));
}

} // namespace testing
} // namespace execute
} // namespace onert_micro
