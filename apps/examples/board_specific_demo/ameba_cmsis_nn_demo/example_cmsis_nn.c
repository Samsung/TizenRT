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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************
 * cmsis_nn_main
 ****************************************************************************/

static void verification_cmsis_nn_thread(void* param) {
    printf("\r\n ========== Start Verification CMSIS_NN ========== \r\n");

    printf("\n-----TEST CASE : test_arm_avgpool_s8-----\n");
    avgpooling_arm_avgpool_s8();
    avgpooling_1_arm_avgpool_s8();
    avgpooling_2_arm_avgpool_s8();
    avgpooling_3_arm_avgpool_s8();
    avgpooling_4_arm_avgpool_s8();
    avgpooling_5_arm_avgpool_s8();

    printf("\n-----TEST CASE : test_arm_avgpool_s16-----\n");
    avgpooling_int16_arm_avgpool_s16();
    avgpooling_int16_1_arm_avgpool_s16();
    avgpooling_int16_2_arm_avgpool_s16();
    avgpooling_int16_3_arm_avgpool_s16();

    printf("\n-----TEST CASE : test_arm_convolve_1x1_s8_fast-----\n");
    kernel1x1_arm_convolve_1x1_s8_fast();

    printf("\n-----TEST CASE : test_arm_convolve_fast_s16-----\n");
    int16xint8_arm_convolve_fast_s16();
    requantize_s64_arm_convolve_fast_s16();

    printf("\n-----TEST CASE : test_arm_convolve_s8-----\n");
    basic_arm_convolve_s8();
    stride2pad1_arm_convolve_s8();
    conv_2_arm_convolve_s8();
    conv_3_arm_convolve_s8();
    conv_4_arm_convolve_s8();
    conv_1_x_n_1_arm_convolve_s8();
    conv_1_x_n_2_arm_convolve_s8();
    conv_1_x_n_3_arm_convolve_s8();
    conv_out_activation_arm_convolve_s8();
    conv_2x2_dilation_arm_convolve_s8();
    conv_2x2_dilation_5x5_input_arm_convolve_s8();
    conv_3x3_dilation_5x5_input_arm_convolve_s8();
    conv_2x3_dilation_arm_convolve_s8();
    conv_3x2_dilation_arm_convolve_s8();
    conv_dilation_golden_arm_convolve_s8();
    conv_5_arm_convolve_s8();

    printf("\n-----TEST CASE : test_arm_convolve_s16-----\n");
    int16xint8_arm_convolve_s16();
    requantize_s64_arm_convolve_s16();
    int16xint8_dilation_1_arm_convolve_s16();
    int16xint8_dilation_2_arm_convolve_s16();
    int16xint8_dilation_3_arm_convolve_s16();

    printf("\n-----TEST CASE : test_arm_depthwise_conv_3x3_s8-----\n");
    depthwise_kernel_3x3_arm_depthwise_conv_3x3_s8();
    depthwise_kernel_3x3_arm_depthwise_conv_3x3_1_s8();
    depthwise_kernel_3x3_null_bias_arm_depthwise_conv_3x3_null_bias_s8();

    printf("\n-----TEST CASE : test_arm_depthwise_conv_fast_s16-----\n");
    dw_int16xint8_fast_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_spill_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_stride_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_null_bias_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_stride_null_bias_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_spill_null_bias_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_test_bias_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_multiple_batches_uneven_buffers_arm_depthwise_conv_fast_s16();
    dw_int16xint8_fast_multiple_batches_uneven_buffers_null_bias_arm_depthwise_conv_fast_s16();

    printf("\n-----TEST CASE : test_arm_depthwise_conv_s8-----\n");
    basic_arm_depthwise_conv_s8();
    stride2pad1_arm_depthwise_conv_s8();
    depthwise_2_arm_depthwise_conv_s8();
    depthwise_out_activation_arm_depthwise_conv_s8();
    depthwise_mult_batches_arm_depthwise_conv_s8();
    depthwise_null_bias_0_arm_depthwise_conv_s8();
    depthwise_null_bias_1_arm_depthwise_conv_s8();
    depthwise_dilation_arm_depthwise_conv_s8();
    depthwise_x_stride_arm_depthwise_conv_s8();

    printf("\n-----TEST CASE : test_arm_depthwise_conv_s8_opt-----\n");
    basic_arm_depthwise_conv_s8_opt();
    stride2pad1_arm_depthwise_conv_s8_opt();
    depthwise_eq_in_out_ch_arm_depthwise_conv_s8_opt();
    depthwise_sub_block_arm_depthwise_conv_s8_opt();

    printf("\n-----TEST CASE : test_arm_depthwise_conv_s16-----\n");
    dw_int16xint8_arm_depthwise_conv_s16();
    dw_int16xint8_dilation_arm_depthwise_conv_s16();
    dw_int16xint8_mult4_arm_depthwise_conv_s16();

    printf("\n-----TEST CASE : test_arm_elementwise_add_s8-----\n");
    add_arm_elementwise_add_s8();

    printf("\n-----TEST CASE : test_arm_elementwise_add_s16-----\n");
    add_s16_arm_elementwise_add_s16();
    add_s16_spill_arm_elementwise_add_s16();

    printf("\n-----TEST CASE : test_arm_elementwise_mul_s8-----\n");
    mul_arm_elementwise_mul_s8();

    printf("\n-----TEST CASE : test_arm_elementwise_mul_s16-----\n");
    mul_s16_arm_elementwise_mul_s16();
    mul_s16_spill_arm_elementwise_mul_s16();

    printf("\n-----TEST CASE : test_arm_fully_connected_s8-----\n");
    fully_connected_arm_fully_connected_s8();
    fully_connected_mve_0_arm_fully_connected_s8();
    fully_connected_mve_1_arm_fully_connected_s8();
    fully_connected_null_bias_0_arm_fully_connected_s8();
    fully_connected_out_activation_arm_fully_connected_s8();

    printf("\n-----TEST CASE : test_arm_fully_connected_s16-----\n");
    fully_connected_int16_arm_fully_connected_s16();
    fully_connected_int16_big_arm_fully_connected_s16();
    fc_int16_slow_arm_fully_connected_s16();

    printf("\n-----TEST CASE : test_arm_max_pool_s8-----\n");
    maxpooling_arm_max_pool_s8();
    maxpooling_1_arm_max_pool_s8();
    maxpooling_2_arm_max_pool_s8();
    maxpooling_3_arm_max_pool_s8();
    maxpooling_4_arm_max_pool_s8();
    maxpooling_5_arm_max_pool_s8();
    maxpooling_6_arm_max_pool_s8();
    maxpooling_7_arm_max_pool_s8();

    printf("\n-----TEST CASE : test_arm_max_pool_s16-----\n");
    maxpool_int16_arm_max_pool_s16();
    maxpool_int16_1_arm_max_pool_s16();
    maxpool_int16_2_arm_max_pool_s16();

    printf("\n-----TEST CASE : test_arm_softmax_s8-----\n");
    softmax_arm_softmax_s8();
    softmax_invalid_diff_min_arm_softmax_s8();

    printf("\n-----TEST CASE : test_arm_softmax_s8_s16-----\n");
    softmax_s8_s16_arm_softmax_s8_s16();
    softmax_s8_s16_invalid_diff_min_arm_softmax_s8_s16();

    printf("\n-----TEST CASE : test_arm_softmax_s16-----\n");
    softmax_s16_arm_softmax_s16();

    printf("\n-----TEST CASE : test_arm_softmax_s16_s8-----\n");
    svdf_arm_svdf_state_s16_s8();
    svdf_1_arm_svdf_state_s16_s8();
    svdf_2_arm_svdf_state_s16_s8();
    svdf_3_arm_svdf_state_s16_s8();

    printf("\r\n ========== Ended Verification CMSIS_NN ========== \r\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int cmsis_nn_main(int argc, char *argv[])
#endif
{
	printf("Example CMSIS NN \n");

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
	res = pthread_create(&tid, &attr, (pthread_startroutine_t)verification_cmsis_nn_thread, NULL);
	if (res < 0) {
		printf("Verification cmsis_nn test fail\n");
		return -1;
	}
	res = pthread_join(tid, NULL);
	if (res < 0) {
		printf("Verification cmsis_nn fail\n");
		return -1;
	}

	return 0;
}
