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
#pragma once

#include <stdint.h>

/**
 * @brief Enum defines types of data source.
 * DataSource implementation for each defined type is done in application.
 */
typedef enum _DATA_SOURCE {
	_CSV_ARRAY = 3,
	_CSV_FILE = 4
} DATA_SOURCE;

struct model_input_output {
		int InputCount;
		int OutputCount;
		float *InputVals;
		float *OutputVals;
};

#ifdef __cplusplus
extern "C" {
#endif

int lw_aifw_sine_test_app_main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

