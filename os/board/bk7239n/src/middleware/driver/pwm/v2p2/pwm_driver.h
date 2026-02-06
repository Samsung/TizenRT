// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <components/log.h>

#define PWM_TAG "pwm"
#define PWM_LOGI(...) BK_LOGI(PWM_TAG, ##__VA_ARGS__)
#define PWM_LOGW(...) BK_LOGW(PWM_TAG, ##__VA_ARGS__)
#define PWM_LOGE(...) BK_LOGE(PWM_TAG, ##__VA_ARGS__)
#define PWM_LOGD(...) BK_LOGD(PWM_TAG, ##__VA_ARGS__)
#define PWM_LOGV(...) BK_LOGV(PWM_TAG, ##__VA_ARGS__)

#define PWM_TIM0_CHAN_P    0
#define PWM_TIM0_CHAN_N    1
#define PWM_TIM1_CHAN_P    2
#define PWM_TIM1_CHAN_N    3
#define PWM_TIM2_CHAN_P    4
#define PWM_TIM2_CHAN_N    5
#define PWM_TIM3_CHAN_P    6
#define PWM_TIM3_CHAN_N    7
#define PWM_TIM4_CHAN_P    8
#define PWM_TIM4_CHAN_N    9
#define PWM_TIM5_CHAN_P    10
#define PWM_TIM5_CHAN_N    11
#define PWM_TIM6_CHAN_P    12
#define PWM_TIM6_CHAN_N    13
#define PWM_TIM7_CHAN_P    14
#define PWM_TIM7_CHAN_N    15
#define PWM_TIM8_CHAN_P    16
#define PWM_TIM8_CHAN_N    17
#define PWM_TIM9_CHAN_P    18
#define PWM_TIM9_CHAN_N    19
#define PWM_TIM10_CHAN_P   20
#define PWM_TIM10_CHAN_N   21
#define PWM_TIM11_CHAN_P   22
#define PWM_TIM11_CHAN_N   23

#define TIM_CAP_CHAN0     0
#define TIM_CAP_CHAN2     2
#define TIM_CAP_CHAN4     4
#define TIM_CAP_CHAN6     6
#define TIM_CAP_CHAN8     8
#define TIM_CAP_CHAN10    10
#define TIM_CAP_CHAN12    12
#define TIM_CAP_CHAN14    14
#define TIM_CAP_CHAN16    16
#define TIM_CAP_CHAN18    18
#define TIM_CAP_CHAN20    20
#define TIM_CAP_CHAN22    22

#define PWM_CHAN_DEFAULT_TIM_MAP \
{ \
	{0, PWM_TIM0_CHAN_P},\
	{1, PWM_TIM1_CHAN_P},\
	{2, PWM_TIM2_CHAN_P},\
	{3, PWM_TIM3_CHAN_P},\
	{4, PWM_TIM4_CHAN_P},\
	{5, PWM_TIM5_CHAN_P},\
	{6, PWM_TIM6_CHAN_P},\
	{7, PWM_TIM7_CHAN_P},\
	{8, PWM_TIM8_CHAN_P},\
	{9, PWM_TIM9_CHAN_P},\
	{10, PWM_TIM10_CHAN_P},\
	{11, PWM_TIM11_CHAN_P},\
}

#define PWM_GROUP_DEFAULT_TIM_MAP \
{ \
	{0, PWM_TIM0_CHAN_P, PWM_TIM0_CHAN_N},\
	{1, PWM_TIM1_CHAN_P, PWM_TIM1_CHAN_N},\
	{2, PWM_TIM2_CHAN_P, PWM_TIM2_CHAN_N},\
	{3, PWM_TIM3_CHAN_P, PWM_TIM3_CHAN_N},\
	{4, PWM_TIM4_CHAN_P, PWM_TIM4_CHAN_N},\
	{5, PWM_TIM5_CHAN_P, PWM_TIM5_CHAN_N},\
	{6, PWM_TIM6_CHAN_P, PWM_TIM6_CHAN_N},\
	{7, PWM_TIM7_CHAN_P, PWM_TIM7_CHAN_N},\
	{8, PWM_TIM8_CHAN_P, PWM_TIM8_CHAN_N},\
	{9, PWM_TIM9_CHAN_P, PWM_TIM9_CHAN_N},\
	{10, PWM_TIM10_CHAN_P, PWM_TIM10_CHAN_N},\
	{11, PWM_TIM11_CHAN_P, PWM_TIM11_CHAN_N},\
}

#define PWM_CAPTURE_DEFAULT_TIM_MAP \
{ \
	{0, TIM_CAP_CHAN0},\
	{1, TIM_CAP_CHAN2},\
	{2, TIM_CAP_CHAN4},\
	{3, TIM_CAP_CHAN6},\
	{4, TIM_CAP_CHAN8},\
	{5, TIM_CAP_CHAN10},\
	{6, TIM_CAP_CHAN12},\
	{7, TIM_CAP_CHAN14},\
	{8, TIM_CAP_CHAN16},\
	{9, TIM_CAP_CHAN18},\
	{10, TIM_CAP_CHAN20},\
	{11, TIM_CAP_CHAN22},\
}

