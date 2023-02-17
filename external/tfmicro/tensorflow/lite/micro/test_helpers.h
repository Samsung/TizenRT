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

#ifndef TENSORFLOW_LITE_MICRO_TEST_HELPERS_H_
#define TENSORFLOW_LITE_MICRO_TEST_HELPERS_H_

// Useful functions for writing tests.

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/schema/schema_generated.h"

namespace tflite {
namespace testing {

// Returns a simple example flatbuffer TensorFlow Lite model. Contains 1 input,
// 1 layer of weights, 1 output Tensor, and 1 operator.
const Model* GetSimpleMockModel();

// Returns a flatbuffer TensorFlow Lite model with more inputs, variable
// tensors, and operators.
const Model* GetComplexMockModel();

// Builds a one-dimensional flatbuffer tensor of the given size.
const Tensor* Create1dFlatbufferTensor(int size, bool is_variable = false);

// Builds a one-dimensional flatbuffer tensor of the given size with
// quantization metadata.
const Tensor* CreateQuantizedFlatbufferTensor(int size);

// Creates a one-dimensional tensor with no quantization metadata.
const Tensor* CreateMissingQuantizationFlatbufferTensor(int size);

// Creates a vector of flatbuffer buffers.
const flatbuffers::Vector<flatbuffers::Offset<Buffer>>*
CreateFlatbufferBuffers();

// Performs a simple string comparison without requiring standard C library.
int TestStrcmp(const char* a, const char* b);

// Wrapper to forward kernel errors to the interpreter's error reporter.
void ReportOpError(struct TfLiteContext* context, const char* format, ...);

void PopulateContext(TfLiteTensor* tensors, int tensors_size,
                     TfLiteContext* context);

// Create a TfLiteIntArray from an array of ints.  The first element in the
// supplied array must be the size of the array expressed as an int.
TfLiteIntArray* IntArrayFromInts(const int* int_array);

// Create a TfLiteFloatArray from an array of floats.  The first element in the
// supplied array must be the size of the array expressed as a float.
TfLiteFloatArray* FloatArrayFromFloats(const float* floats);

TfLiteTensor CreateFloatTensor(const float* data, TfLiteIntArray* dims,
                               const char* name, bool is_variable = false);

void PopulateFloatTensor(TfLiteTensor* tensor, float* begin, float* end);

TfLiteTensor CreateBoolTensor(const bool* data, TfLiteIntArray* dims,
                              const char* name, bool is_variable = false);

TfLiteTensor CreateInt32Tensor(const int32_t*, TfLiteIntArray* dims,
                               const char* name, bool is_variable = false);

TfLiteTensor CreateQuantizedTensor(const uint8_t* data, TfLiteIntArray* dims,
                                   float scale, int zero_point,
                                   const char* name, bool is_variable = false);

TfLiteTensor CreateQuantizedTensor(const float* input, uint8_t* quantized,
                                   TfLiteIntArray* dims, float scale,
                                   int zero_point, const char* name,
                                   bool is_variable = false);

TfLiteTensor CreateQuantizedTensor(const int8_t* data, TfLiteIntArray* dims,
                                   float scale, int zero_point,
                                   const char* name, bool is_variable = false);

TfLiteTensor CreateQuantizedTensor(const int16_t* data, TfLiteIntArray* dims,
                                   float scale, int zero_point,
                                   const char* name, bool is_variable = false);

TfLiteTensor CreateQuantizedTensor(const float* input, int8_t* quantized,
                                   TfLiteIntArray* dims, float scale,
                                   int zero_point, const char* name,
                                   bool is_variable = false);

TfLiteTensor CreateQuantizedBiasTensor(const float* data, int32_t* quantized,
                                       TfLiteIntArray* dims, float input_scale,
                                       float weights_scale, const char* name,
                                       bool is_variable = false);

// Quantizes int32 bias tensor with per-channel weights determined by input
// scale multiplied by weight scale for each channel.
TfLiteTensor CreatePerChannelQuantizedBiasTensor(
    const float* input, int32_t* quantized, TfLiteIntArray* dims,
    float input_scale, float* weight_scales, float* scales, int* zero_points,
    TfLiteAffineQuantization* affine_quant, int quantized_dimension,
    const char* name, bool is_variable = false);

TfLiteTensor CreateSymmetricPerChannelQuantizedTensor(
    const float* input, int8_t* quantized, TfLiteIntArray* dims, float* scales,
    int* zero_points, TfLiteAffineQuantization* affine_quant,
    int quantized_dimension, const char* name, bool is_variable = false);

}  // namespace testing
}  // namespace tflite

#endif  // TENSORFLOW_LITE_MICRO_TEST_HELPERS_H_
