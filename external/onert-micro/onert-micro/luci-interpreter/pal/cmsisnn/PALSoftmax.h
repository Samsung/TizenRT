/*
 * Copyright (c) 2023 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_PAL_SOFTMAX_H
#define LUCI_INTERPRETER_PAL_SOFTMAX_H

#include "PALSoftmaxCommon.h"

#include <arm_nnfunctions.h>

namespace
{
const int kInt16LUTArraySize = 513;

// LUTPopulate takes an optional type-erased transform_params to allow passing
// extra parameters to the transform function pointer. const void* is used
// instead of std::function to be compatible with TFLite Micro
template <typename FloatT, typename Func>
inline typename std::enable_if<std::is_same<Func, FloatT (*)(FloatT)>::value, FloatT>::type
LUTTransform(Func transform, const void * /*transform_params*/, FloatT value)
{
    static_assert(std::is_floating_point<FloatT>::value, "FloatT must be a floating-point type.");
    return transform(value);
}

// Keep floating-point type configurable for backward compatibility. float
// should be used for FloatT by default.
template <typename FloatT, typename Func>
inline void LUTPopulateInt16(FloatT input_scale, int32_t input_zero_point, FloatT output_scale,
                             int32_t output_zero_point, Func transform,
                             const void *transform_params, int16_t *lut)
{
    static_assert(std::is_floating_point<FloatT>::value, "FloatT must be a floating-point type.");
    const FloatT input_min = input_scale * (std::numeric_limits<int16_t>::min() - input_zero_point);
    const FloatT input_max = input_scale * (std::numeric_limits<int16_t>::max() - input_zero_point);
    const FloatT output_min =
            output_scale * (std::numeric_limits<int16_t>::min() - output_zero_point);
    const FloatT output_max =
            output_scale * (std::numeric_limits<int16_t>::max() - output_zero_point);

    const int nb_steps = 512;
    const FloatT step = (input_max - input_min) / nb_steps;
    const FloatT half_step = step / 2;
    const FloatT output_scaling_inv = static_cast<FloatT>(std::numeric_limits<int16_t>::max() -
                                                          std::numeric_limits<int16_t>::min() + 1) /
                                      (output_max - output_min);
    const FloatT table_min = static_cast<FloatT>(std::numeric_limits<int16_t>::min());
    const FloatT table_max = static_cast<FloatT>(std::numeric_limits<int16_t>::max());

    for (int i = 0; i < nb_steps; i++)
    {
        const FloatT val = LUTTransform<FloatT>(transform, transform_params, input_min + i * step);
        const FloatT val_midpoint =
                LUTTransform<FloatT>(transform, transform_params, input_min + i * step + half_step);
        const FloatT val_next =
                LUTTransform<FloatT>(transform, transform_params, input_min + (i + 1) * step);

        const FloatT sample_val = std::round(val * output_scaling_inv);
        const FloatT midpoint_interp_val =
                std::round((val_next * output_scaling_inv + std::round(val * output_scaling_inv)) / 2);
        const FloatT midpoint_val = std::round(val_midpoint * output_scaling_inv);
        const FloatT midpoint_err = midpoint_interp_val - midpoint_val;
        const FloatT bias = std::round(midpoint_err / 2);

        lut[i] = static_cast<int16_t>(
                std::min<FloatT>(std::max<FloatT>(sample_val - bias, table_min), table_max));
    }

    lut[nb_steps] = static_cast<int16_t>(std::min<FloatT>(
            std::max<FloatT>(
                    std::round(LUTTransform<FloatT>(transform, transform_params, input_max) * output_scaling_inv),
                    table_min),
            table_max));
}

template <typename T>
inline typename std::enable_if<std::is_same<T, int16_t>::value, void>::type
LUTPopulate(float input_scale, int32_t input_zero_point, float output_scale,
            int32_t output_zero_point, float (*transform)(float), T *lut)
{
    LUTPopulateInt16<float>(input_scale, input_zero_point, output_scale, output_zero_point, transform,
                            nullptr, lut);
}

} // namespace

namespace luci_interpreter_pal
{

inline void Softmax(const SoftmaxParams &params, const int8_t *input_data, int8_t *output_data)
{
    arm_softmax_s8(input_data, params.num_rows, params.row_size, params.input_multiplier,
                   params.input_left_shift, params.diff_min, output_data);
}

inline void Softmax(const SoftmaxParams &params, const int8_t *input_data, int16_t *output_data)
{
    arm_softmax_s8_s16(input_data, params.num_rows, params.row_size, params.input_multiplier,
                       params.input_left_shift, params.diff_min, output_data);
}

inline void Softmax(const SoftmaxParams &params, const int16_t *input_data, int16_t *output_data)
{
    cmsis_nn_softmax_lut_s16 softmax_params{};

    auto raw_exp_lut = std::make_unique<int16_t[]>(kInt16LUTArraySize);
    auto one_over_one_plus_x_lut = std::make_unique<int16_t[]>(kInt16LUTArraySize);

    // exp LUT only used on negative values
    // we consider exp(-10.0) is insignificant to accumulation
    const int32_t range = std::numeric_limits<int16_t>::max() - std::numeric_limits<int16_t>::min();

    LUTPopulate<int16_t>(
            10.0f / range, std::numeric_limits<int16_t>::max(), 2.0f / range, 0,
            [](float value)->float { return std::exp(value); }, raw_exp_lut.get());

    LUTPopulate<int16_t>(
            1.0f / range, std::numeric_limits<int16_t>::min(), 2.0f / range, 0,
            [](float value)->float { return 1.0f / (1.0f + value); }, one_over_one_plus_x_lut.get());

    softmax_params.exp_lut = raw_exp_lut.get();
    softmax_params.one_by_one_lut = one_over_one_plus_x_lut.get();

    arm_softmax_s16(input_data, params.num_rows, params.row_size, params.input_multiplier,
                    params.input_left_shift, &softmax_params, output_data);
}

} // namespace luci_interpreter_pal

#endif // LUCI_INTERPRETER_PAL_SOFTMAX_H
