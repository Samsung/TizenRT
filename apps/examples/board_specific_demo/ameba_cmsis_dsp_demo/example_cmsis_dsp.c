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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

#include "cmsis_dsp_test.h"

/****************************************************************************
 * cmsis_dsp_main
 ****************************************************************************/

static void verification_cmsis_dsp_thread(void* param)
{
    printf("\r\n ========== Start Verification CMSIS_DSP ========== \r\n");

    printf("\n-----TEST CASE : arm_bayes_example_f32-----\n");
    arm_bayes_example_f32();

    printf("\n-----TEST CASE : arm_class_marks_example_f32-----\n");
    arm_class_marks_example_f32();

    printf("\n-----TEST CASE : arm_convolution_example_f32-----\n");
    arm_convolution_example_f32();

    printf("\n-----TEST CASE : arm_dotproduct_example_f32-----\n");
    arm_dotproduct_example_f32();

    printf("\n-----TEST CASE : arm_fft_bin_example_f32-----\n");
    arm_fft_bin_example_f32();  //FAILURE with run 2nd times

    printf("\n-----TEST CASE : arm_fir_example_f32-----\n");
    arm_fir_example_f32();

    printf("\n-----TEST CASE : arm_graphic_equalizer_example_q31-----\n");
    arm_graphic_equalizer_example_q31();

    printf("\n-----TEST CASE : arm_linear_interp_example_f32-----\n");
    arm_linear_interp_example_f32();

    printf("\n-----TEST CASE : arm_matrix_example_f32-----\n");
    arm_matrix_example_f32();

    printf("\n-----TEST CASE : arm_signal_converge_example_f32-----\n");
    arm_signal_converge_example_f32();

    printf("\n-----TEST CASE : arm_sin_cos_example_f32-----\n");
    arm_sin_cos_example_f32();

    printf("\n-----TEST CASE : arm_svm_example_f32-----\n");
    arm_svm_example_f32();

    printf("\n-----TEST CASE : arm_variance_example_f32-----\n");
    arm_variance_example_f32();

    printf("\r\n ========== Ended Verification CMSIS_DSP ========== \r\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int cmsis_dsp_main(int argc, char *argv[])
#endif
{
	printf("Example CMSIS DSP \n");

	pthread_t tid;
	pthread_attr_t attr;
	int res = 0;

	res = pthread_attr_init(&attr);
	if (res != 0) {
		printf("Failed to pthread_attr_init\n");
		return -1;
	}
	res = pthread_attr_setstacksize(&attr, 10240);
	if (res != 0) {
		printf("Failed to pthread_attr_setstacksize\n");
		return -1;
	}
	res = pthread_create(&tid, &attr, (pthread_startroutine_t)verification_cmsis_dsp_thread, NULL);
	if (res < 0) {
		printf("Verification cmsis_dsp test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("Verification cmsis_dsp fail\n");
		return -1;
	}

	return 0;
}
