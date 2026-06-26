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

#ifndef ONERT_MICRO_TRAIN_TESTS_TEST_UTILS_H
#define ONERT_MICRO_TRAIN_TESTS_TEST_UTILS_H

#include "OMTrainingInterpreter.h"
#include "train/tests/OMTestTrainBase.h"

#include <vector>
#include <numeric>

namespace onert_micro
{
namespace train
{
namespace test
{

// Train training_interpreter using config params and OMTestTrainBase to obtain data
template <typename T, typename U = T>
OMStatus train(OMTrainingInterpreter &train_interpreter, OMConfig &config,
               OMTestTrainBase<T, U> &test_base)
{
  OMStatus status = Ok;
  const uint32_t training_epochs = config.training_context.epochs;
  const uint32_t num_train_data_samples = test_base.getTrainNumSamples();
  const uint32_t batch_size = config.training_context.batch_size;
  const uint32_t input_size = train_interpreter.getInputSizeAt(0);
  const uint32_t target_size = train_interpreter.getOutputSizeAt(0);
  for (uint32_t e = 0; e < training_epochs; ++e)
  {
    config.training_context.num_epoch = e + 1;
    uint32_t num_steps = num_train_data_samples / batch_size;
    for (int i = 0; i < num_steps; ++i)
    {
      // Set batch size
      uint32_t cur_batch_size = std::min(batch_size, num_train_data_samples - batch_size * i - 1);
      cur_batch_size = std::max(1u, cur_batch_size);

      config.training_context.batch_size = cur_batch_size;

      // Read current input and target data
      const uint32_t cur_input_size = sizeof(T) * input_size * cur_batch_size;
      const uint32_t cur_target_size = sizeof(U) * target_size * cur_batch_size;
      const uint32_t cur_input_offset = sizeof(T) * input_size * i * batch_size;
      const uint32_t cur_target_offset = sizeof(U) * target_size * i * batch_size;

      // Read input and target
      std::vector<T> input_data = test_base.readTrainInputData(cur_input_size, cur_input_offset);
      std::vector<U> target_data =
        test_base.readTrainTargetData(cur_target_size, cur_target_offset);

      // Set input and target
      train_interpreter.setInput(reinterpret_cast<uint8_t *>(input_data.data()), 0);
      train_interpreter.setTarget(reinterpret_cast<uint8_t *>(target_data.data()), 0);

      // Train with current batch size
      status = train_interpreter.trainSingleStep(config);
      assert(status == Ok);
      if (status != Ok)
        return status;
    }
  }

  return status;
}

// Evaluate trained model using metric and save result in metric_res
template <typename T, typename U = T>
OMStatus evaluate(OMTrainingInterpreter &train_interpreter, OMConfig &config,
                  OMTestTrainBase<T, U> &test_base, OMMetrics metric, U *metric_res)
{
  OMStatus status = Ok;

  // To store all calculated metrics values
  std::vector<U> result_v;

  const uint32_t num_test_data_samples = test_base.getTestNumSamples();
  const uint32_t batch_size = 1;
  const uint32_t input_size = train_interpreter.getInputSizeAt(0);
  const uint32_t target_size = train_interpreter.getOutputSizeAt(0);
  for (int i = 0; i < num_test_data_samples; ++i)
  {
    // Read current input and target data
    const uint32_t cur_input_size = sizeof(T) * input_size;
    const uint32_t cur_target_size = sizeof(U) * target_size;
    const uint32_t cur_input_offset = sizeof(T) * input_size * i;
    const uint32_t cur_target_offset = sizeof(U) * target_size * i;

    // Read input and target
    std::vector<T> input_data = test_base.readTestInputData(cur_input_size, cur_input_offset);
    std::vector<U> target_data = test_base.readTestTargetData(cur_target_size, cur_target_offset);

    // Set input and target
    train_interpreter.setInput(reinterpret_cast<uint8_t *>(input_data.data()), 0);
    train_interpreter.setTarget(reinterpret_cast<uint8_t *>(target_data.data()), 0);

    U result = U(0);

    status = train_interpreter.evaluateMetric(config, metric, reinterpret_cast<void *>(&result),
                                              batch_size);
    assert(status == Ok);
    result_v.push_back(result);
  }
  // Calculate and save average values
  *metric_res =
    static_cast<U>(std::accumulate(result_v.begin(), result_v.end(), U(0)) / result_v.size());

  return status;
}

} // namespace test
} // namespace train
} // namespace onert_micro

#endif // ONERT_MICRO_TRAIN_TESTS_TEST_UTILS_H
