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

/**
 * @file lwaifw_sine_test_input.h
 * @brief Model input
 */

#ifndef __lwaifw_sine_test_input_h__
#define __lwaifw_sine_test_input_h__

#define NO_OF_ROWS_TO_READ 20
#define NO_OF_SENSORS 1

float gDataValues[NO_OF_ROWS_TO_READ][NO_OF_SENSORS] = {
	{0.942477},
	{1.256637},
	{1.570796},
	{1.884956},
	{2.199115},
	{2.513274},
	{2.523274},
	{3.147593},
	{3.455752},
	{3.769912},
	{0.942477},
	{1.256637},
	{1.570796},
	{1.884956},
	{2.199115},
	{2.513274},
	{2.523274},
	{3.147593},
	{3.455752},
	{3.769912}
};
#endif /* __lwaifw_sine_test_input_h__ */
