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
 * @file aifw_test_main.h
 * @brief Sample application to show case usage of AI Framework basic functionality such as data collection, pre-processing, inference, post-processing, and finally ensembling.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief: It initializes AI Helper module. Model set is then loaded with its unique model code.
 * A service is started for the model set. After this application will start receiving callback data collection callback.
 * @return: -1 indicated error. 0 indicates success
*/
int aifw_test_main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

