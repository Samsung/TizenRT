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

#include "train/tests/models/checkoint_simple_example_model.h"
#include "train/tests/models/saved_checkpoint_example.h"
#include "core/train/OMCheckpointSaver.h"
#include "train/tests/OMTestUtils.h"

#include <gtest/gtest.h>

namespace onert_micro
{
namespace train
{
namespace test
{

using namespace testing;

class CheckpointsHandlerTest : public ::testing::Test
{
public:
  CheckpointsHandlerTest()
  {
    // Do nothing
  }

  OMConfig config = {};
};

// Check save functionality
TEST_F(CheckpointsHandlerTest, Save_check_P)
{
  // Create BostonHousing data handler
  config.model_ptr = reinterpret_cast<char *>(models::checkpoint_simple_example_model_data);
  config.model_size = models::checkpoint_simple_example_model_size;
  config.train_mode = true;

  // Import model
  core::OMTrainingRuntimeModule training_runtime_module;
  OMStatus status = training_runtime_module.importTrainModel(config.model_ptr, config);
  EXPECT_EQ(status, Ok);

  // Get DataBuffer (vector of chars) of checkpoints
  std::vector<char> checkpoint_data;
  status = training_runtime_module.createCheckpointFile(config, checkpoint_data);
  EXPECT_EQ(status, Ok);
  EXPECT_TRUE(!checkpoint_data.empty());
}

// Check load functionality
TEST_F(CheckpointsHandlerTest, Load_check_P)
{
  // Create BostonHousing data handler
  config.model_ptr = reinterpret_cast<char *>(models::checkpoint_simple_example_model_data);
  config.model_size = models::checkpoint_simple_example_model_size;
  config.train_mode = true;

  // Import model
  core::OMTrainingRuntimeModule training_runtime_module;
  OMStatus status = training_runtime_module.importTrainModel(config.model_ptr, config);
  EXPECT_EQ(status, Ok);

  // Get DataBuffer (vector of chars) of checkpoints
  std::vector<char> checkpoint_data(models::saved_checkpoint_example_size);
  std::memcpy(checkpoint_data.data(), models::saved_checkpoint_example,
              models::saved_checkpoint_example_size);
  // Load and check (validation inside)
  status = training_runtime_module.loadCheckpointData(config, checkpoint_data.data());
  EXPECT_EQ(status, Ok);
}

// Check load functionality
TEST_F(CheckpointsHandlerTest, Wrong_magic_num_NEG)
{
  // Create BostonHousing data handler
  config.model_ptr = reinterpret_cast<char *>(models::checkpoint_simple_example_model_data);
  config.model_size = models::checkpoint_simple_example_model_size;
  config.train_mode = true;

  // Import model
  core::OMTrainingRuntimeModule training_runtime_module;
  OMStatus status = training_runtime_module.importTrainModel(config.model_ptr, config);
  EXPECT_EQ(status, Ok);

  // Get DataBuffer (vector of chars) of checkpoints
  std::vector<char> checkpoint_data(models::saved_checkpoint_example_size);
  std::memcpy(checkpoint_data.data(), models::saved_checkpoint_example_with_wrong_magic_num,
              models::saved_checkpoint_example_size);
  // Load and check (validation inside)
  EXPECT_DEATH(training_runtime_module.loadCheckpointData(config, checkpoint_data.data()), "");
}

} // namespace test
} // namespace train
} // namespace onert_micro
