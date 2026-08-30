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

/*
 The MIT License (MIT)

 Copyright (c) 2017 Facebook Inc.
 Copyright (c) 2017 Georgia Institute of Technology
 Copyright 2019 Google LLC

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ONERT_MICRO_EXECUTE_PAL_DEQUANTIZE_COMMON_H
#define ONERT_MICRO_EXECUTE_PAL_DEQUANTIZE_COMMON_H

#include "core/OMRuntimeShape.h"
#include "OMStatus.h"
#include "core/OMKernelData.h"
#include "PALUtils.h"

#include <cmath>
#include <stdint.h>
//#include <bit>

namespace
{

/// Notice that this code comes from FP16(https://github.com/Maratyszcza/FP16) under MIT License

/*
 * Convert a 16-bit floating-point number in IEEE half-precision format, in bit representation, to
 * a 32-bit floating-point number in IEEE single-precision format, in bit representation.
 *
 * @note The implementation doesn't use any floating-point operations.
 */
uint32_t fp16_ieee_to_fp32_bits(uint16_t h)
{
  /*
   * Extend the half-precision floating-point number to 32 bits and shift to the upper part of the
   * 32-bit word:
   *      +---+-----+------------+-------------------+
   *      | S |EEEEE|MM MMMM MMMM|0000 0000 0000 0000|
   *      +---+-----+------------+-------------------+
   * Bits  31  26-30    16-25            0-15
   *
   * S - sign bit, E - bits of the biased exponent, M - bits of the mantissa, 0 - zero bits.
   */
  const uint32_t w = (uint32_t)h << 16;
  /*
   * Extract the sign of the input number into the high bit of the 32-bit word:
   *
   *      +---+----------------------------------+
   *      | S |0000000 00000000 00000000 00000000|
   *      +---+----------------------------------+
   * Bits  31                 0-31
   */
  const uint32_t sign = w & UINT32_C(0x80000000);
  /*
   * Extract mantissa and biased exponent of the input number into the bits 0-30 of the 32-bit word:
   *
   *      +---+-----+------------+-------------------+
   *      | 0 |EEEEE|MM MMMM MMMM|0000 0000 0000 0000|
   *      +---+-----+------------+-------------------+
   * Bits  30  27-31     17-26            0-16
   */
  const uint32_t nonsign = w & UINT32_C(0x7FFFFFFF);
  /*
   * Renorm shift is the number of bits to shift mantissa left to make the half-precision number
   * normalized. If the initial number is normalized, some of its high 6 bits (sign == 0 and 5-bit
   * exponent) equals one. In this case renorm_shift == 0. If the number is denormalize,
   * renorm_shift > 0. Note that if we shift denormalized nonsign by renorm_shift, the unit bit of
   * mantissa will shift into exponent, turning the biased exponent into 1, and making mantissa
   * normalized (i.e. without leading 1).
   */
#ifdef _MSC_VER
  unsigned long nonsign_bsr;
  _BitScanReverse(&nonsign_bsr, (unsigned long)nonsign);
  uint32_t renorm_shift = (uint32_t)nonsign_bsr ^ 31;
#else
  uint32_t renorm_shift = __builtin_clz(nonsign);
#endif
  renorm_shift = renorm_shift > 5 ? renorm_shift - 5 : 0;
  /*
   * Iff half-precision number has exponent of 15, the addition overflows it into bit 31,
   * and the subsequent shift turns the high 9 bits into 1. Thus
   *   inf_nan_mask ==
   *                   0x7F800000 if the half-precision number had exponent of 15 (i.e. was NaN or
   * infinity) 0x00000000 otherwise
   */
  const int32_t inf_nan_mask = ((int32_t)(nonsign + 0x04000000) >> 8) & INT32_C(0x7F800000);
  /*
   * Iff nonsign is 0, it overflows into 0xFFFFFFFF, turning bit 31 into 1. Otherwise, bit 31
   * remains 0. The signed shift right by 31 broadcasts bit 31 into all bits of the zero_mask. Thus
   *   zero_mask ==
   *                0xFFFFFFFF if the half-precision number was zero (+0.0h or -0.0h)
   *                0x00000000 otherwise
   */
  const int32_t zero_mask = (int32_t)(nonsign - 1) >> 31;
  /*
   * 1. Shift nonsign left by renorm_shift to normalize it (if the input was denormal)
   * 2. Shift nonsign right by 3 so the exponent (5 bits originally) becomes an 8-bit field and
   * 10-bit mantissa shifts into the 10 high bits of the 23-bit mantissa of IEEE single-precision
   * number.
   * 3. Add 0x70 to the exponent (starting at bit 23) to compensate the different in exponent bias
   *    (0x7F for single-precision number less 0xF for half-precision number).
   * 4. Subtract renorm_shift from the exponent (starting at bit 23) to account for renormalization.
   * As renorm_shift is less than 0x70, this can be combined with step 3.
   * 5. Binary OR with inf_nan_mask to turn the exponent into 0xFF if the input was NaN or infinity.
   * 6. Binary ANDNOT with zero_mask to turn the mantissa and exponent into zero if the input was
   * zero.
   * 7. Combine with the sign of the input number.
   */
  return sign | ((((nonsign << renorm_shift >> 3) + ((0x70 - renorm_shift) << 23)) | inf_nan_mask) &
                 ~zero_mask);
}

} // namespace

namespace onert_micro
{
namespace execute
{
namespace pal
{

template <typename InputT, typename OutputT>
OMStatus Dequantize(const core::QuantizationParams op_params, const uint32_t flat_size,
                    const InputT *input_data, OutputT *output_data)
{
  const int32_t zero_point = op_params.zero_point;
  const double scale = op_params.scale;

  for (uint32_t i = 0; i < flat_size; i++)
  {
    const int32_t val = input_data[i];
    const auto result = static_cast<OutputT>(scale * (val - zero_point));
    output_data[i] = result;
  }
  return Ok;
}

OMStatus DequantizeF16toF32(const uint32_t flat_size, const uint16_t *input_data,
                            float *output_data)
{
  for (uint32_t i = 0; i < flat_size; i++)
  {
    uint32_t f32_in_bits = (fp16_ieee_to_fp32_bits(input_data[i]));
    float output_data;
    memcpy(&output_data, &f32_in_bits, sizeof(float));
  }
  return Ok;
}

} // namespace pal
} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_PAL_DEQUANTIZE_COMMON_H
