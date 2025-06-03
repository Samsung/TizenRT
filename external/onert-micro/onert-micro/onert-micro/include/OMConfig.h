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

#ifndef ONERT_MICRO_CONFIG_H
#define ONERT_MICRO_CONFIG_H

#include <stdint.h>
#include <stdlib.h>

namespace onert_micro
{

/*
 * OMTrainOptimizer - enum to store optimizers supported by training with onert-micro
 */
enum OMTrainOptimizer
{
  SGD,
  ADAM,
};

/*
 * OMMetrics - enum to store metrics supported by training with onert-micro
 */
enum OMMetrics
{
  MSE_METRICS,
  MAE_METRICS,
  CROSS_ENTROPY_METRICS,
  ACCURACY,
  SPARSE_CROSS_ENTROPY_ACCURACY,
  NONE,
};

/*
 * OMLoss - enum to store loss supported by training with onert-micro
 */
enum OMLoss
{
  BINARY_CROSS_ENTROPY,
  CROSS_ENTROPY,
  MSE,
  MAE,
  SPARSE_CROSS_ENTROPY,
};

/*
 * OMTrainingContext contains training specific parameters
 * batch_size - batch size for training (Note: 1 - default one)
 * num_of_train_layers - number of trainable last layers (Note: 0 - all layers will be trained)
 * optimizer - optimizer which onert-micro training will be used (Note: SGD - default one)
 * loss - loss which onert-micro training will be used (Note: CROSS_ENTROPY - default one)
 * learning_rate - used by all optimizers
 * beta - used by ADAM optimizer
 * beta_squares - used by ADAM optimizer
 * epsilon - used by ADAM optimizer
 * num_Step - used by ADAM optimizer
 * training_config_info_data - pointer to the training config data, to store training specific
 * scenario (default null)
 */
struct OMTrainingContext
{
  uint32_t batch_size = 1;
  uint32_t num_of_train_layers = 0;
  OMTrainOptimizer optimizer = SGD;
  OMLoss loss = MSE;
  float learning_rate = 0.001f;
  float beta = 0.9f;
  float beta_squares = 0.9f;
  float epsilon = 10e-8;
  uint32_t num_step = 0;
  uint32_t num_epoch = 0;
  uint32_t epochs = 0;

  char *training_config_info_data = nullptr;
};

/*
 * OMConfig - main config to store information for runtime and for training runtime
 * keep_input - will the allocated memory be saved for the input data, or can it be deleted after
 * use cmsis_nn - will CMSIS NN kernels be used or not (needed to some internal settings) wof_ptr -
 * a pointer to the data that stores weights separate from the model train_mode - a flag to indicate
 * whether we are currently in training mode or not
 */
struct OMConfig
{
  bool keep_input = false;
  bool cmsis_nn = false;
  // For case with divided weights and circle file
  char *wof_ptr = nullptr;
  bool train_mode = false;
  char *model_ptr = nullptr;
  size_t model_size = 0;
  OMTrainingContext training_context = {};
};

} // namespace onert_micro

#endif // ONERT_MICRO_CONFIG_H
