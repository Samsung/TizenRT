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

#ifndef ONERT_MICRO_CORE_READER_CIRCLE_READER_H
#define ONERT_MICRO_CORE_READER_CIRCLE_READER_H

#include "OMStatus.h"

#include <circle-generated/circle/schema_generated.h>

namespace onert_micro
{
namespace core
{
namespace reader
{

using CircleBuffers = flatbuffers::Vector<flatbuffers::Offset<circle::Buffer>>;
using CircleTensors = flatbuffers::Vector<flatbuffers::Offset<circle::Tensor>>;
using CircleOperators = flatbuffers::Vector<flatbuffers::Offset<circle::Operator>>;
using CircleOperatorCodes = flatbuffers::Vector<flatbuffers::Offset<circle::OperatorCode>>;
using CircleMetadataSet = flatbuffers::Vector<flatbuffers::Offset<circle::Metadata>>;
using CircleValues = flatbuffers::Vector<int32_t>;

/**
 * @brief Loads Circle file and provides helpers to access attributes
 */
class OMCircleReader
{
public:
  OMCircleReader() = default;
  OMCircleReader(const OMCircleReader &) = delete;
  OMCircleReader(OMCircleReader &&) = default;
  OMCircleReader &operator=(const OMCircleReader &) = delete;
  OMCircleReader &&operator=(const OMCircleReader &&) = delete;
  ~OMCircleReader() = default;

public: // direct API
  const CircleOperatorCodes *opcodes() const { return _model->operator_codes(); }
  const CircleBuffers *buffers() const { return _model->buffers(); }
  const CircleTensors *tensors() const { return _current_subgraph->tensors(); }
  const CircleOperators *operators() const { return _current_subgraph->operators(); }
  const CircleValues *inputs() const { return _current_subgraph->inputs(); }
  const CircleValues *outputs() const { return _current_subgraph->outputs(); }
  const CircleMetadataSet *metadata() const { return _model->metadata(); }

  uint32_t num_subgraph() const { return _model->subgraphs()->size(); }
  circle::BuiltinOperator builtin_code(const circle::Operator *op) const;

public:
  OMStatus parse(const char *model_ptr);
  OMStatus select_subgraph(uint32_t subgraph);
  uint32_t get_current_subgraph_index() const { return _current_subgraph_index; }

  // helpers
public:
  bool isConstTensor(uint32_t tensor_index);

private:
  const circle::Model *_model{nullptr};
  const circle::SubGraph *_current_subgraph{nullptr};
  uint32_t _current_subgraph_index{0};
};

} // namespace reader
} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_READER_CIRCLE_READER_H
