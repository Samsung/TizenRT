/****************************************************************************
 *
 * Copyright 2023 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

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

#include "constants.h"
#include "sine_model_data.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_profiler.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include <tinyara/config.h>

#include <cstdio>
#include <debug.h>

#include <tinyara/init.h>

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
} // namespace

// The name of this function is important for Arduino compatibility.
void setup()
{
	// Set up logging. Google style is to avoid globals or statics because of
	// lifetime uncertainty, but since this has a trivial destructor it's okay.
	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::MicroErrorReporter micro_error_reporter;
	static tflite::MicroProfiler profiler;

	// Map the model into a usable data structure. This doesn't involve any
	// copying or parsing, it's a very lightweight operation.
	model = tflite::GetModel(g_sine_model_data);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.",
			model->version(), TFLITE_SCHEMA_VERSION);
		return;
	}

	// This pulls in all the operation implementations we need.
	// NOLINTNEXTLINE(runtime-global-variables)
	static tflite::AllOpsResolver resolver;

	// Build an interpreter to run the model with.
	//  static tflite::MicroInterpreter static_interpreter(
	//      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
	static tflite::MicroInterpreter static_interpreter(
		model, resolver, tensor_arena, kTensorArenaSize, nullptr, &profiler);
	interpreter = &static_interpreter;

	// Allocate memory from the tensor_arena for the model's tensors.
	TfLiteStatus allocate_status = interpreter->AllocateTensors();
	if (allocate_status != kTfLiteOk) {
		error_reporter->Report("AllocateTensors() failed");
		return;
	}

	// Obtain pointers to the model's input and output tensors.
	input = interpreter->input(0);
	output = interpreter->output(0);

	// Keep track of how many inferences we have performed.
	inference_count = 0;
}

// The name of this function is important for Arduino compatibility.
void loop()
{
	// Calculate an x value to feed into the model. We compare the current
	// inference_count to the number of inferences per cycle to determine
	// our position within the range of possible x values the model was
	// trained on, and use this to calculate a value.
	float position = static_cast<float>(inference_count) /
					 static_cast<float>(kInferencesPerCycle);
	float x_val = position * kXrange;

	// Place our calculated x value in the model's input tensor
	input->data.f[0] = x_val;
	printf("INPUT: %f   ", x_val);
	sleep(1);

	// Run inference, and report any error
	TfLiteStatus invoke_status = interpreter->Invoke();
	if (invoke_status != kTfLiteOk) {
		error_reporter->Report("Invoke failed on x_val: %f\n",
							   static_cast<double>(x_val));
		return;
	}

	// Read the predicted y value from the model's output tensor
	float y_val = output->data.f[0];

	// Output the results. A custom HandleOutput function can be implemented
	// for each supported hardware target.
	// HandleOutput(error_reporter, x_val, y_val);
	printf("OUTPUT: %f\n", y_val);

	// Increment the inference_counter, and reset it if we have reached
	// the total number per cycle
	inference_count += 1;
	if (inference_count >= kInferencesPerCycle)
		inference_count = 0;
}

void HandleOutput(tflite::ErrorReporter *error_reporter, float x_value,
				  float y_value)
{
	// Log the current X and Y values
	error_reporter->Report("x_value: %f, y_value: %f\n", x_value, y_value);
}

extern "C" {
int hello_tfmicro_main(int argc, char *argv[])
{
#ifdef TF_LITE_STATIC_MEMORY
	printf("Defined %zu\n", sizeof(TfLiteTensor));
#else
	printf("Not def %zu\n", sizeof(TfLiteTensor));
#endif
	sleep(2);
	setup();
	while (true) {
		loop();
	}
}
}
