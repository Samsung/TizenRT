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

#include "OMInterpreter.h"

using namespace onert_micro;

OMStatus OMInterpreter::importModel(const char *model_ptr, const OMConfig &config)
{
  assert(model_ptr != nullptr && "Model ptr shouldn't be nullptr");
  if (model_ptr == nullptr)
    return UnknownError;

  return _runtime_module.importModel(model_ptr, config);
}

OMStatus OMInterpreter::run(const OMConfig &config) { return _runtime_module.run(config); }

OMStatus OMInterpreter::reset() { return _runtime_module.reset(); }

uint32_t OMInterpreter::getNumberOfInputs() { return _runtime_module.getNumberOfInputs(); }

uint32_t OMInterpreter::getNumberOfOutputs() { return _runtime_module.getNumberOfOutputs(); }

void *OMInterpreter::getInputDataAt(uint32_t position)
{
  return _runtime_module.getInputDataAt(position);
}

void *OMInterpreter::getOutputDataAt(uint32_t position)
{
  return _runtime_module.getOutputDataAt(position);
}

uint32_t OMInterpreter::getInputSizeAt(uint32_t position)
{
  return _runtime_module.getInputSizeAt(position);
}

uint32_t OMInterpreter::getOutputSizeAt(uint32_t position)
{
  return _runtime_module.getOutputSizeAt(position);
}

OMStatus OMInterpreter::allocateInputs() { return _runtime_module.allocateInputs(); }
