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
 * Public Data
 ****************************************************************************/

/*
 * This header is included in os/include/sys/boardctl.h
 * and These values are used to call board_reset(), which is a board specification
 * function of stm32h745_systemreset.c
 */
#define M7_RESET                   0    /* Reset only M7 core */
#define M7_M4_RESET                1    /* Reset all cores, M7 and M4 */
