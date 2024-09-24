/*
 * Copyright (c) 2020 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef __LUCI_MICRO_INTERPRETER_MICRO_READER_H__
#define __LUCI_MICRO_INTERPRETER_MICRO_READER_H__

#include "luci_interpreter/core/ParamsType.h"
#include "luci_interpreter/core/DataType.h"

#include <circle-generated/circle/schema_generated.h>

#include <map>
#include <memory>
#include <vector>

namespace luci_interpreter
{

#ifdef USE_STATIC_ALLOC
namespace
{

using ExecutionPlanTable = std::map<uint32_t, std::vector<uint32_t>>;

template <typename VECTORTYPE> uint32_t read_u32(const VECTORTYPE &buffer, uint32_t idx)
{
  static_assert(std::is_same<typename VECTORTYPE::value_type, uint8_t>::value, "Types mismatch!");

  uint32_t val = 0;
  val += (buffer.at(idx + 0) << 0 * 8);
  val += (buffer.at(idx + 1) << 1 * 8);
  val += (buffer.at(idx + 2) << 2 * 8);
  val += (buffer.at(idx + 3) << 3 * 8);
  return val;
}

} // namespace

namespace read_metadata
{

template <typename VECTORTYPE>
ExecutionPlanTable decode_execution_plan(const VECTORTYPE &execution_plan_data)
{
  static_assert(std::is_same<typename VECTORTYPE::value_type, uint8_t>::value, "Types mismatch!");

  ExecutionPlanTable execution_plan_table;
  uint32_t idx = 0;

  if (execution_plan_data.size() < 4)
    assert(false && "Op table decode error : invalid entry number");

  uint32_t entry_number = read_u32(execution_plan_data, idx);
  idx += sizeof(uint32_t);

  while (idx < execution_plan_data.size())
  {
    if (idx + 2 * sizeof(uint32_t) > execution_plan_data.size())
      assert(false && "Op table decode error : invalid entry item");

    uint32_t id = read_u32(execution_plan_data, idx);
    idx += sizeof(uint32_t);

    uint32_t size = read_u32(execution_plan_data, idx);

    if (size == 0)
      assert(false && "Op table decode error : empty execution plan entry");

    idx += sizeof(uint32_t);

    if (idx + sizeof(uint32_t) * size > execution_plan_data.size())
      assert(false && "Source table decode error : invalid entry data");

    std::vector<uint32_t> execution_plan_vector;
    uint32_t position = read_u32(execution_plan_data, idx);
    idx += sizeof(uint32_t);

    for (uint32_t j = 1; j < size; ++j)
    {
      uint32_t execution_plan_inform = read_u32(execution_plan_data, idx);
      idx += sizeof(uint32_t);

      execution_plan_vector.push_back(execution_plan_inform);
    }

    if (!execution_plan_table.insert({position, execution_plan_vector}).second)
      assert(false && "Op table decode error : duplicated origin ID");
  }

  if (idx != execution_plan_data.size())
    assert(false && "Op table decode error : data size invalid");

  if (execution_plan_table.size() != entry_number)
    assert(false && "Op table decode error : entry number invalid");

  return execution_plan_table;
}

} // namespace read_metadata
#endif

DataType luci_datatype(circle::TensorType type);
FusedActFunc luci_actfunc(circle::ActivationFunctionType type);
Padding luci_padding(circle::Padding padding);
MirrorPadMode luci_mirrorpad_mode(circle::MirrorPadMode mode);

/**
 * @brief Wrapper to use flatbuffers::Vector pointer as std::vector entity
 */
template <typename T> class VectorWrapper
{
public:
  explicit VectorWrapper(const flatbuffers::Vector<T> *ptr);

  const T *data() const;
  uint32_t size() const;

  using iterator = typename flatbuffers::Vector<T>::const_iterator;
  iterator begin() const;
  iterator end() const;

  using value_type = typename flatbuffers::Vector<T>::return_type;
  value_type at(uint32_t i) const;
  value_type operator[](uint32_t i) const;

  bool null() const;
  bool empty() const;

private:
  const flatbuffers::Vector<T> *_vector;
};

template <typename T> VectorWrapper<T> wrap(const flatbuffers::Vector<T> *vec)
{
  return VectorWrapper<T>(vec);
}

/**
 * @brief Loads Circle file and provides helpers to access attributes
 */
class CircleReader
{
public:
  using CircleBuffers = VectorWrapper<flatbuffers::Offset<circle::Buffer>>;
  using CircleTensors = VectorWrapper<flatbuffers::Offset<circle::Tensor>>;
  using CircleOperators = VectorWrapper<flatbuffers::Offset<circle::Operator>>;
  using CircleOperatorCodes = VectorWrapper<flatbuffers::Offset<circle::OperatorCode>>;
  using CircleMetadataSet = VectorWrapper<flatbuffers::Offset<circle::Metadata>>;

public:
  CircleReader() = default;

public: // direct API
  CircleOperatorCodes opcodes() const { return wrap(_model->operator_codes()); }
  CircleBuffers buffers() const { return wrap(_model->buffers()); }
  CircleTensors tensors() const { return wrap(_current_subgraph->tensors()); }
  CircleOperators operators() const { return wrap(_current_subgraph->operators()); }
  VectorWrapper<int32_t> inputs() const { return wrap(_current_subgraph->inputs()); }
  VectorWrapper<int32_t> outputs() const { return wrap(_current_subgraph->outputs()); }
  circle::DataFormat data_format() const { return _current_subgraph->data_format(); }
  CircleMetadataSet metadata() const { return wrap(_model->metadata()); }

  uint32_t num_subgraph() const { return wrap(_model->subgraphs()).size(); }
  circle::BuiltinOperator builtin_code(const circle::Operator *op) const;

public:
  bool parse(const circle::Model *model);
  bool select_subgraph(uint32_t subgraph);
  uint32_t get_current_subgraph_index() const { return _current_subgraph_index; }

private:
  const circle::Model *_model{nullptr};
  const circle::SubGraph *_current_subgraph{nullptr};
  uint32_t _current_subgraph_index{0};
};

} // namespace luci_interpreter

#endif // __LUCI_MICRO_INTERPRETER_MICRO_READER_H__
