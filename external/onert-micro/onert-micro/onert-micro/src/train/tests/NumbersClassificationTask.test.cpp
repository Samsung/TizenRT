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

#include "train/tests/numbers_classification_task/NumbersClassificationTask.h"
#include "train/tests/OMTestUtils.h"

#include <gtest/gtest.h>

namespace onert_micro
{
namespace train
{
namespace test
{

using namespace testing;

class NumbersClassificationTaskTest : public ::testing::Test
{
public:
  NumbersClassificationTaskTest()
  {
    // Set user defined training settings
    const uint32_t training_epochs = 10;
    const float lambda = 0.1f;
    const uint32_t batch_size = 1;
    // Train all layers
    const uint32_t num_train_layers = 0;
    const onert_micro::OMLoss loss = onert_micro::CROSS_ENTROPY;
    const onert_micro::OMTrainOptimizer train_optim = onert_micro::SGD;
    const float beta = 0.9;
    const float beta_squares = 0.999;
    const float epsilon = 1e-07;

    config.train_mode = true;
    {
      onert_micro::OMTrainingContext train_context;
      train_context.batch_size = batch_size;
      train_context.num_of_train_layers = num_train_layers;
      train_context.learning_rate = lambda;
      train_context.loss = loss;
      train_context.optimizer = train_optim;
      train_context.beta = beta;
      train_context.beta_squares = beta_squares;
      train_context.epsilon = epsilon;
      train_context.epochs = training_epochs;

      config.training_context = train_context;
    }
  }

  OMConfig config = {};

  // Some value for checking that the learning process has not become worse
  const float golden_accuracy_metric = 1.f;
};

TEST_F(NumbersClassificationTaskTest, SGD_CROSS_ENTROPY_P)
{
  // Create BostonHousing data handler
  NumbersClassificationTask<float> numbersClassificationTask;

  config.model_ptr = numbersClassificationTask.getModelPtr();
  config.model_size = numbersClassificationTask.getModelSize();
  config.train_mode = true;

  // Create and import train interpreter
  OMTrainingInterpreter train_interpreter;
  OMStatus status =
    train_interpreter.importTrainModel(numbersClassificationTask.getModelPtr(), config);
  EXPECT_EQ(status, Ok);

  // Evaluate result before training
  float acc_metric_before_training = 0.f;
  status = evaluate(train_interpreter, config, numbersClassificationTask, ACCURACY,
                    &acc_metric_before_training);
  EXPECT_EQ(status, Ok);

  // Train model with current config
  status = train(train_interpreter, config, numbersClassificationTask);
  EXPECT_EQ(status, Ok);

  // Evaluate result after training
  float acc_metric_after_training = 0.f;
  status = evaluate(train_interpreter, config, numbersClassificationTask, ACCURACY,
                    &acc_metric_after_training);
  EXPECT_EQ(status, Ok);

  // ACCURACY metric after training should be better then before (before training accuracy value
  // smaller then after)
  EXPECT_GT(acc_metric_after_training, acc_metric_before_training);

  // Compare with gold value (should be greater or equal)
  EXPECT_GE(acc_metric_after_training, golden_accuracy_metric);
}

} // namespace test
} // namespace train
} // namespace onert_micro
