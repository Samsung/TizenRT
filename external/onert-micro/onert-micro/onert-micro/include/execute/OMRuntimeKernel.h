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

#ifndef ONERT_MICRO_EXECUTE_RUNTIME_KERNEL_H
#define ONERT_MICRO_EXECUTE_RUNTIME_KERNEL_H

#include "core/reader/OMCircleReader.h"
#include "core/OMRuntimeContext.h"
#include "core/OMRuntimeStorage.h"

#include <cstdint>

constexpr static uint32_t maxInputSize = 24; // was 6, but lstm takes 24 inputs
constexpr static uint32_t maxOutputSize = 5;

namespace onert_micro
{
namespace execute
{

class OMRuntimeKernel
{
public:
  OMRuntimeKernel() = default;
  OMRuntimeKernel(const OMRuntimeKernel &) = delete;
  OMRuntimeKernel(OMRuntimeKernel &&) = delete;
  ~OMRuntimeKernel() = default;
  OMRuntimeKernel &operator=(const OMRuntimeKernel &) = delete;
  OMRuntimeKernel &&operator=(const OMRuntimeKernel &&) = delete;

public:
  OMStatus readKernel(uint16_t op_index, core::OMRuntimeContext &runtime_context);

  OMStatus getDataFromStorage(uint16_t op_index, core::OMRuntimeStorage &storage,
                              core::OMRuntimeContext &context);

public:
  const circle::Tensor *inputs[maxInputSize] = {nullptr};
  const circle::Tensor *outputs[maxOutputSize] = {nullptr};

  uint8_t *inputs_data[maxInputSize] = {nullptr};
  uint8_t *outputs_data[maxOutputSize] = {nullptr};

  int32_t inputs_index[maxInputSize] = {-1};
  int32_t outputs_index[maxOutputSize] = {-1};

  uint32_t outputs_num = 0;
  uint32_t inputs_num = 0;

  const circle::Operator *first_operator = nullptr;
};

} // namespace execute
} // namespace onert_micro

#endif // ONERT_MICRO_EXECUTE_RUNTIME_KERNEL_H
