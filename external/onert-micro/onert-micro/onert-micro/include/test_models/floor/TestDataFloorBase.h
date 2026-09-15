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

#ifndef ONERT_MICRO_TEST_MODELS_FLOOR_KERNEL_BASE_H
#define ONERT_MICRO_TEST_MODELS_FLOOR_KERNEL_BASE_H

#include "test_models/TestDataBase.h"

namespace onert_micro
{
namespace test_model
{

template <typename T> class TestDataFloorBase : public TestDataBase<T>
{
public:
  TestDataFloorBase() = default;

  const unsigned char *get_model_ptr() override final { return _test_kernel_model_circle; }

  const std::vector<T> &get_input_data_by_index(int i) override final
  {
    switch (i)
    {
      case 0:
        return _input_data;
      default:
        assert(false && "Wrong input index");
    }
  }

  const std::vector<T> &get_output_data_by_index(int i) override final
  {
    assert(i == 0);
    return _reference_output_data;
  }

protected:
  std::vector<T> _input_data;
  std::vector<T> _reference_output_data;
  const unsigned char *_test_kernel_model_circle;
};

} // namespace test_model
} // namespace onert_micro

#endif // ONERT_MICRO_TEST_MODELS_FLOOR_KERNEL_BASE_H
