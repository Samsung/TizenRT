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

#include "execute/OMRuntimeKernel.h"

using namespace onert_micro::execute;
using namespace onert_micro;

OMStatus onert_micro::execute::OMRuntimeKernel::readKernel(uint16_t op_index,
                                                           core::OMRuntimeContext &runtime_context)
{
  first_operator = runtime_context.getCircleOperatorAt(op_index);
  const circle::Operator *last_operator = runtime_context.getCircleOperatorAt(op_index);

  inputs_num = first_operator->inputs()->size();
  assert(inputs_num <= maxInputSize);

  if (inputs_num > maxInputSize)
    return UnknownError;

  outputs_num = last_operator->outputs()->size();
  assert(outputs_num <= maxOutputSize);

  if (outputs_num > maxOutputSize)
    return UnknownError;

  assert(inputs_num > 0 and outputs_num > 0);

  // Read inputs
  {
    const auto *inputs_op = first_operator->inputs();
    for (uint32_t i = 0; i < inputs_num; ++i)
    {
      inputs_index[i] = inputs_op->operator[](i);
      if (inputs_index[i] != -1)
        inputs[i] = runtime_context.getTensorByIndex(inputs_index[i]);
    }
  }
  // Read outputs
  {
    const auto *outputs_op = last_operator->outputs();
    for (uint32_t i = 0; i < outputs_num; ++i)
    {
      outputs_index[i] = outputs_op->operator[](i);
      if (outputs_index[i] != -1)
        outputs[i] = runtime_context.getTensorByIndex(outputs_index[i]);
    }
  }

  return Ok;
}

// Note: if inplace then first input and first output will be inplace
OMStatus onert_micro::execute::OMRuntimeKernel::getDataFromStorage(uint16_t op_index,
                                                                   core::OMRuntimeStorage &storage,
                                                                   core::OMRuntimeContext &context)
{
  OMStatus status = Ok;

  for (uint32_t i = 0; i < inputs_num; ++i)
  {
    if (inputs_index[i] == -1)
      continue;
    status = storage.getDataByTensorIndex(&inputs_data[i], inputs_index[i]);
    if (inputs_data[i] == nullptr)
      status = context.getConstDataByTensorIndex(&inputs_data[i], inputs_index[i]);
    if (status != Ok)
      return status;
  }

  for (uint32_t i = 0; i < outputs_num; ++i)
  {
    if (outputs_index[i] == -1)
      continue;
    status = storage.getDataByTensorIndex(&outputs_data[i], outputs_index[i]);

    if (storage.getKernelType(op_index) == core::Inplace)
    {
      outputs_data[i] = inputs_data[i];
      status = storage.removeTensorFromTensorIndexToData(inputs_index[i]);

      if (status != Ok)
        return status;

      status = storage.saveDataToTensorIndex(outputs_data[i], outputs_index[i]);
    }
  }

  return status;
}
