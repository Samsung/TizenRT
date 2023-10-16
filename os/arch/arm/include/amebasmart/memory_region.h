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

#define MEM_VAR_REGION_COUNT 2

extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;

void *variable_region_start_addr[MEM_VAR_REGION_COUNT] = {&__bss_start__, &__data_start__};
void *variable_region_end_addr[MEM_VAR_REGION_COUNT] = {&__bss_end__, &__data_end__};
