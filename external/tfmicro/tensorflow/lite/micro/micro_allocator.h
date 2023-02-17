/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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
#ifndef TENSORFLOW_LITE_MICRO_MICRO_ALLOCATOR_H_
#define TENSORFLOW_LITE_MICRO_MICRO_ALLOCATOR_H_

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/core/api/flatbuffer_conversions.h"
#include "tensorflow/lite/micro/simple_memory_allocator.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {

// Namespace used for unittests.
namespace internal {
// Sets up all of the data structure members for a runtime tensor
// based on the contents of a serialized tensor.
TfLiteStatus InitializeRuntimeTensor(
    SimpleMemoryAllocator* allocator, const tflite::Tensor& flatbuffer_tensor,
    const flatbuffers::Vector<flatbuffers::Offset<Buffer>>* buffers,
    ErrorReporter* error_reporter, TfLiteTensor* result);
}  // namespace internal

typedef struct {
  TfLiteNode node;
  const TfLiteRegistration* registration;
} NodeAndRegistration;

// Allocator responsible for allocating memory for all intermediate tensors
// necessary to invoke a model.
class MicroAllocator {
 public:
  // The lifetime of the model, tensor allocator and error reporter must be at
  // least as long as that of the allocator object, since the allocator needs
  // them to be accessible during its entire lifetime.

  // Note: Please use __declspec(align(16)) to make sure tensor_arena is 16
  // bytes aligned, otherwise some head room will be wasted.
  MicroAllocator(TfLiteContext* context, const Model* model,
                 uint8_t* tensor_arena, size_t arena_size,
                 ErrorReporter* error_reporter);

  // Runs through the model and allocates all necessary input, output and
  // intermediate tensors.
  // WARNING: doing any allocation after calling this method has the risk of
  // corrupting tensor data so this method should be the last method to be
  // called in this class.
  TfLiteStatus FinishTensorAllocation();

  // Run through the model to allocate nodes and registrations. We need to keep
  // them for the entire life time of the model to allow persistent tensors.
  // This method needs to be called before FinishTensorAllocation method.
  TfLiteStatus AllocateNodeAndRegistrations(
      const OpResolver& op_resolver,
      NodeAndRegistration** node_and_registrations);

 private:
  TfLiteStatus Init();

  const Model* model_;
  SimpleMemoryAllocator* memory_allocator_;
  ErrorReporter* error_reporter_;
  TfLiteContext* context_;
  // Indicating if the allocator is ready for allocation.
  bool active_ = false;

  const SubGraph* subgraph_;
  const flatbuffers::Vector<flatbuffers::Offset<Operator>>* operators_;
  const flatbuffers::Vector<flatbuffers::Offset<Tensor>>* tensors_;
};

}  // namespace tflite
#endif  // TENSORFLOW_LITE_MICRO_MICRO_ALLOCATOR_H_
