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

#include "execute/kernels/PoolingCommon.h"
#include "PALL2Pool2D.h"

using namespace onert_micro;
using namespace onert_micro::execute;

// NOTE: doesnt currently support dynamic shapes
namespace onert_micro
{
namespace execute
{

OMStatus execute_kernel_CircleL2Pool2D(const OMExecuteArgs &execute_args)
{
  auto l2_pool_float_lambda = [](const core::Pool2DParams &params,
                                 const core::OMRuntimeShape &input_shape, const float *input_data,
                                 const core::OMRuntimeShape &output_shape, float *output_data) {
    return pal::L2Pool(params, input_shape, input_data, output_shape, output_data);
  };

#ifndef DIS_QUANT
  auto l2_pool_int8_lambda = [](const core::Pool2DParams &params,
                                const core::OMRuntimeShape &input_shape, const int8_t *input_data,
                                const core::OMRuntimeShape &output_shape,
                                int8_t *output_data) { return UnsupportedType; };
#else
  auto l2_pool_int8_lambda = [](const core::Pool2DParams &params,
                                const core::OMRuntimeShape &input_shape, const int8_t *input_data,
                                const core::OMRuntimeShape &output_shape,
                                int8_t *output_data) { return UnsupportedType; };
#endif // DIS_QUANT

  return execute_pooling_common(execute_args, l2_pool_float_lambda, l2_pool_int8_lambda);
}

} // namespace execute
} // namespace onert_micro
