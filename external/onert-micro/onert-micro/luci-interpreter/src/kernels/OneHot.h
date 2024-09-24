/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All Rights Reserved
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

#ifndef LUCI_INTERPRETER_KERNELS_ONEHOT_H
#define LUCI_INTERPRETER_KERNELS_ONEHOT_H

#include "core/Kernel.h"
#include "core/KernelParams.h"

namespace luci_interpreter
{
namespace kernels
{

class OneHot : public KernelWithParams<OneHotParams>
{
public:
  OneHot(const Tensor *indices, const Tensor *depth, const Tensor *on_value,
         const Tensor *off_value, Tensor *output, const OneHotParams &params);

  const Tensor *indices() const { return _inputs[0]; }
  const Tensor *depth() const { return _inputs[1]; }
  const Tensor *on_value() const { return _inputs[2]; }
  const Tensor *off_value() const { return _inputs[3]; }

  Tensor *output() const { return _outputs[0]; }

  void configure() override;
  void execute() const override;
};

} // namespace kernels
} // namespace luci_interpreter

#endif // LUCI_INTERPRETER_KERNELS_ONEHOT_H
