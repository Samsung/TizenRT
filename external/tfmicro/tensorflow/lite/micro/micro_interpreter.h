/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_LITE_MICRO_MICRO_INTERPRETER_H_
#define TENSORFLOW_LITE_MICRO_MICRO_INTERPRETER_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/core/api/op_resolver.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/micro/micro_allocator.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/type_to_tflitetype.h"

namespace tflite {

class MicroInterpreter {
 public:
  // The lifetime of the model, op resolver, tensor arena, and error reporter
  // must be at least as long as that of the interpreter object, since the
  // interpreter may need to access them at any time. This means that you should
  // usually create them with the same scope as each other, for example having
  // them all allocated on the stack as local variables through a top-level
  // function.
  // The interpreter doesn't do any deallocation of any of the pointed-to
  // objects, ownership remains with the caller.
  MicroInterpreter(const Model* model, const OpResolver& op_resolver,
                   uint8_t* tensor_arena, size_t tensor_arena_size,
                   ErrorReporter* error_reporter);

  // Runs through the model and allocates all necessary input, output and
  // intermediate tensors.
  TfLiteStatus AllocateTensors();

  TfLiteStatus Invoke();

  size_t tensors_size() const { return context_.tensors_size; }
  TfLiteTensor* tensor(size_t tensor_index);
  template <class T>
  T* typed_tensor(int tensor_index) {
    if (TfLiteTensor* tensor_ptr = tensor(tensor_index)) {
      if (tensor_ptr->type == typeToTfLiteType<T>()) {
        return GetTensorData<T>(tensor_ptr);
      }
    }
    return nullptr;
  }

  TfLiteTensor* input(size_t index);
  size_t inputs_size() const { return subgraph_->inputs()->Length(); }
  const flatbuffers::Vector<int32_t>& inputs() const {
    return *subgraph_->inputs();
  }
  TfLiteTensor* input_tensor(size_t index) { return input(index); }
  template <class T>
  T* typed_input_tensor(int tensor_index) {
    if (TfLiteTensor* tensor_ptr = input_tensor(tensor_index)) {
      if (tensor_ptr->type == typeToTfLiteType<T>()) {
        return GetTensorData<T>(tensor_ptr);
      }
    }
    return nullptr;
  }

  TfLiteTensor* output(size_t index);
  size_t outputs_size() const { return subgraph_->outputs()->Length(); }
  const flatbuffers::Vector<int32_t>& outputs() const {
    return *subgraph_->outputs();
  }
  TfLiteTensor* output_tensor(size_t index) { return output(index); }
  template <class T>
  T* typed_output_tensor(int tensor_index) {
    if (TfLiteTensor* tensor_ptr = output_tensor(tensor_index)) {
      if (tensor_ptr->type == typeToTfLiteType<T>()) {
        return GetTensorData<T>(tensor_ptr);
      }
    }
    return nullptr;
  }

  // Reset all variable tensors to the default value.
  TfLiteStatus ResetVariableTensors();

  TfLiteStatus initialization_status() const { return initialization_status_; }

  ErrorReporter* error_reporter() { return error_reporter_; }

  size_t operators_size() const { return operators_->size(); }

  // For debugging only.
  const NodeAndRegistration node_and_registration(int node_index) const {
    return node_and_registrations_[node_index];
  }

 private:
  void CorrectTensorEndianness(TfLiteTensor* tensorCorr);

  template <class T>
  void CorrectTensorDataEndianness(T* data, int32_t size);

  NodeAndRegistration* node_and_registrations_;

  const Model* model_;
  const OpResolver& op_resolver_;
  ErrorReporter* error_reporter_;
  TfLiteContext context_ = {};
  MicroAllocator allocator_;
  bool tensors_allocated_;
  bool tensors_prepared_;

  TfLiteStatus initialization_status_;
  const flatbuffers::Vector<flatbuffers::Offset<Tensor>>* tensors_;
  const flatbuffers::Vector<flatbuffers::Offset<Operator>>* operators_;

  const SubGraph* subgraph_;
};

}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_MICRO_INTERPRETER_H_
