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

#ifndef ONERT_MICRO_CORE_OM_DATA_TYPE_H
#define ONERT_MICRO_CORE_OM_DATA_TYPE_H

#include "core/reader/OMCircleReader.h"

#include <cstdint>
#include <cstddef>
#include <string>
#include <cassert>

namespace onert_micro
{
namespace core
{
/**
 * @brief "scalar" value type
 */
enum class OMDataType
{
  Unknown, // Unknown type (serves as a default value)

  U8,  // 8-bit unsigned integer
  U16, // 16-bit unsigned integer
  U32, // 32-bit unsigned integer
  U64, // 64-bit unsigned integer

  S8,  // 8-bit signed integer
  S16, // 16-bit signed integer
  S32, // 32-bit signed integer
  S64, // 64-bit signed integer

  FLOAT16, // IEEE 16-bit floating-point
  FLOAT32, // IEEE 32-bit floating-point
  FLOAT64, // IEEE 64-bit floating-point

  BOOL, // Boolean
};

OMDataType onertMicroDatatype(const circle::TensorType type);

/**
 * @brief C++ scalar type corresponding to each OMDataType
 */
template <OMDataType DT> struct OMDataTypeImpl
{
  // using Type = ...
};

// TODO Support other enum values
template <> struct OMDataTypeImpl<OMDataType::S8>
{
  // Use C++ int8_t type for 8bit integer
  using Type = int8_t;
};

template <> struct OMDataTypeImpl<OMDataType::U8>
{
  // Use C++ uint8_t type for unsigned 8bit integer
  using Type = uint8_t;
};

template <> struct OMDataTypeImpl<OMDataType::S16>
{
  // Use C++ int16_t type for 16bit integer
  using Type = int16_t;
};

template <> struct OMDataTypeImpl<OMDataType::U16>
{
  // Use C++ uint16_t type for unsigned 16bit integer
  using Type = uint16_t;
};

template <> struct OMDataTypeImpl<OMDataType::S32>
{
  // Use C++ int32_t type for 32bit integer
  using Type = int32_t;
};

template <> struct OMDataTypeImpl<OMDataType::U32>
{
  // Use C++ uint32_t type for unsigned 32bit integer
  using Type = uint32_t;
};

template <> struct OMDataTypeImpl<OMDataType::S64>
{
  // Use C++ int64_t type for 64bit integer
  using Type = int64_t;
};

template <> struct OMDataTypeImpl<OMDataType::U64>
{
  // Use C++ uint64_t type for unsigned 64bit integer
  using Type = uint64_t;
};

template <> struct OMDataTypeImpl<OMDataType::FLOAT16>
{
  // float16 type with 16bit value, encoded with help of FP16 library
  // https://github.com/Maratyszcza/FP16/
  using Type = uint16_t;
};

template <> struct OMDataTypeImpl<OMDataType::FLOAT32>
{
  // Use C++ float type for IEEE 32-bit floating-point numbers
  using Type = float;
};

template <> struct OMDataTypeImpl<OMDataType::FLOAT64>
{
  // Use C++ double type for IEEE 64-bit floating-point numbers
  using Type = double;
};

// NOTE OMDataTypeImpl for BOOL is subject to change
template <> struct OMDataTypeImpl<OMDataType::BOOL>
{
  // Use C++ uint8_t type for bool
  using Type = uint8_t;
};

/**
 * @brief Returns the size of the data type.
 * @note If you need the size at compile time, use `sizeof(typename OMDataTypeImpl<DT>::Type)`.
 */
inline uint32_t size(OMDataType data_type)
{
  switch (data_type)
  {
    case OMDataType::S8:
      return sizeof(OMDataTypeImpl<OMDataType::S8>::Type);
    case OMDataType::U8:
      return sizeof(OMDataTypeImpl<OMDataType::U8>::Type);
    case OMDataType::S16:
      return sizeof(OMDataTypeImpl<OMDataType::S16>::Type);
    case OMDataType::U16:
      return sizeof(OMDataTypeImpl<OMDataType::U16>::Type);
    case OMDataType::S32:
      return sizeof(OMDataTypeImpl<OMDataType::S32>::Type);
    case OMDataType::U32:
      return sizeof(OMDataTypeImpl<OMDataType::U32>::Type);
    case OMDataType::S64:
      return sizeof(OMDataTypeImpl<OMDataType::S64>::Type);
    case OMDataType::U64:
      return sizeof(OMDataTypeImpl<OMDataType::U64>::Type);
    case OMDataType::FLOAT16:
      return sizeof(OMDataTypeImpl<OMDataType::FLOAT16>::Type);
    case OMDataType::FLOAT32:
      return sizeof(OMDataTypeImpl<OMDataType::FLOAT32>::Type);
    case OMDataType::FLOAT64:
      return sizeof(OMDataTypeImpl<OMDataType::FLOAT64>::Type);
    case OMDataType::BOOL:
      return sizeof(OMDataTypeImpl<OMDataType::BOOL>::Type);
    default:
      // TODO Support remaining data types.
      assert(false);
      return 1; // Avoid compiler warning.
  }
}

inline size_t getOMDataTypeSize(OMDataType data_type) { return size(data_type); }

} // namespace core
} // namespace onert_micro

#endif // ONERT_MICRO_CORE_OM_DATA_TYPE_H
