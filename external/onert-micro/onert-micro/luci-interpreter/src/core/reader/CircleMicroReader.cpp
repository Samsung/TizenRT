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

#include "luci_interpreter/core/reader/CircleMicroReader.h"
#include "luci_interpreter/core/reader/CircleMicroReaderHelper.h"

#include <algorithm>

namespace luci_interpreter
{

// TODO check can we remove it
DataType luci_datatype(const circle::TensorType type)
{
  switch (type)
  {
    case circle::TensorType_FLOAT32:
      return DataType::FLOAT32;
    case circle::TensorType_FLOAT16:
      return DataType::FLOAT16;
    case circle::TensorType_INT32:
      return DataType::S32;
    case circle::TensorType_UINT8:
      return DataType::U8;
    case circle::TensorType_INT64:
      return DataType::S64;
    case circle::TensorType_BOOL:
      return DataType::BOOL;
    case circle::TensorType_INT16:
      return DataType::S16;
    case circle::TensorType_COMPLEX64:
      break;
    case circle::TensorType_INT8:
      return DataType::S8;
    default:
      break;
  }
  assert(false);
  return DataType::Unknown;
}

FusedActFunc luci_actfunc(const circle::ActivationFunctionType type)
{
  switch (type)
  {
    case circle::ActivationFunctionType::ActivationFunctionType_NONE:
      return FusedActFunc::NONE;
    case circle::ActivationFunctionType::ActivationFunctionType_RELU:
      return FusedActFunc::RELU;
    case circle::ActivationFunctionType::ActivationFunctionType_RELU_N1_TO_1:
      return FusedActFunc::RELU_N1_TO_1;
    case circle::ActivationFunctionType::ActivationFunctionType_RELU6:
      return FusedActFunc::RELU6;
    case circle::ActivationFunctionType::ActivationFunctionType_TANH:
      return FusedActFunc::TANH;
    case circle::ActivationFunctionType::ActivationFunctionType_SIGN_BIT:
      return FusedActFunc::SIGN_BIT;
    default:
      break;
  }
  assert(false);
  return FusedActFunc::UNDEFINED;
}

Padding luci_padding(const circle::Padding padding)
{
  switch (padding)
  {
    case circle::Padding::Padding_SAME:
      return Padding::SAME;
    case circle::Padding::Padding_VALID:
      return Padding::VALID;
  }
  assert(false);
  return Padding::UNDEFINED;
}

MirrorPadMode luci_mirrorpad_mode(const circle::MirrorPadMode mode)
{
  switch (mode)
  {
    case circle::MirrorPadMode::MirrorPadMode_REFLECT:
      return MirrorPadMode::REFLECT;
    case circle::MirrorPadMode::MirrorPadMode_SYMMETRIC:
      return MirrorPadMode::SYMMETRIC;
  }
  assert(false);
  return MirrorPadMode::UNDEFINED;
}

circle::BuiltinOperator CircleReader::builtin_code(const circle::Operator *op) const
{
  assert(op != nullptr);

  const auto op_codes = opcodes();
  uint32_t index = op->opcode_index();
  assert(index < op_codes.size());
  const auto opcode = op_codes[index];
  assert(opcode != nullptr);

  return circle::builtin_code_neutral(opcode);
}

bool CircleReader::parse(const circle::Model *model)
{
  assert(model != nullptr);

  // for direct pointer access
  _model = model;

  return true;
}

bool CircleReader::select_subgraph(uint32_t sgindex)
{
  if (num_subgraph() <= sgindex)
  {
    assert(false);
    return false;
  }

  // for direct pointer access
  auto subgraphs = _model->subgraphs();
  assert(subgraphs != nullptr);

  _current_subgraph = subgraphs->Get(sgindex);
  assert(_current_subgraph != nullptr);

  _current_subgraph_index = sgindex;

  return true;
}

template <typename T>
VectorWrapper<T>::VectorWrapper(const flatbuffers::Vector<T> *ptr) : _vector(ptr)
{
  // Do nothing
}

template <typename T> uint32_t VectorWrapper<T>::size() const
{
  return null() ? 0 : _vector->size();
}

template <typename T> const T *VectorWrapper<T>::data() const
{
  return null() ? nullptr : _vector->data();
}

template <typename T> typename VectorWrapper<T>::iterator VectorWrapper<T>::begin() const
{
  return null() ? iterator(nullptr, 0) : _vector->begin();
}

template <typename T> typename VectorWrapper<T>::iterator VectorWrapper<T>::end() const
{
  return null() ? begin() : _vector->end();
}

template <typename T> typename VectorWrapper<T>::value_type VectorWrapper<T>::at(uint32_t i) const
{
  if (i >= size())
  {
    // TODO find better error message
    assert(false && "Access to prohibited vector element");
  }

  return _vector->Get(i);
}

template <typename T>
typename VectorWrapper<T>::value_type VectorWrapper<T>::operator[](uint32_t i) const
{
  return at(i);
}

template <typename T> bool VectorWrapper<T>::null() const { return _vector == nullptr; }
template <typename T> bool VectorWrapper<T>::empty() const { return size() == 0; }

#define REGISTER_WRAPPER(T) template class VectorWrapper<T>
REGISTER_WRAPPER(flatbuffers::Offset<circle::SubGraph>);
REGISTER_WRAPPER(flatbuffers::Offset<circle::Buffer>);
REGISTER_WRAPPER(flatbuffers::Offset<circle::Tensor>);
REGISTER_WRAPPER(flatbuffers::Offset<circle::Operator>);
REGISTER_WRAPPER(flatbuffers::Offset<circle::OperatorCode>);
REGISTER_WRAPPER(flatbuffers::Offset<circle::Metadata>);
REGISTER_WRAPPER(int32_t);
REGISTER_WRAPPER(uint8_t);
#undef REGISTER_WRAPPER

} // namespace luci_interpreter
