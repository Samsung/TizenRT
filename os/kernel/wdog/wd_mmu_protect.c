/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
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
/************************************************************************
 * kernel/wdog/wd_mmu_protect.c
 *
 *   MMU page protection for watchdog static memory.
 *
 *   This file is a stub — the implementation of wd_mmu_protect_init(),
 *   wd_mmu_write_begin(), and wd_mmu_write_end() has been moved to
 *   wd_initialize.c to keep all watchdog data and MMU protection logic
 *   in one place.
 *
 *   The implementation uses L1 section AP bits directly (no L2 split).

 *
 ************************************************************************/

#include <tinyara/config.h>

/* All MMU protection functions are implemented in wd_initialize.c */
