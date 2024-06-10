/****************************************************************************
 *
 * Copyright 2024 Samsung Electronics All Rights Reserved.
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
#ifndef __CMSIS_DSP_H__
#define __CMSIS_DSP_H__

#include <pthread.h>

/*
 * Functions
 */

int32_t arm_bayes_example_f32(void);
int32_t arm_class_marks_example_f32(void);
int32_t arm_convolution_example_f32(void);
int32_t arm_dotproduct_example_f32(void);
int32_t arm_fft_bin_example_f32(void);
int32_t arm_fir_example_f32(void);
int32_t arm_graphic_equalizer_example_q31(void);
int32_t arm_linear_interp_example_f32(void);
int32_t arm_matrix_example_f32(void);
int32_t arm_signal_converge_example_f32(void);
int32_t arm_sin_cos_example_f32(void);
int32_t arm_svm_example_f32(void);
int32_t arm_variance_example_f32(void);

#endif /* __CMSIS_DSP_H__ */
