/****************************************************************************
 *
 * Copyright 2022 Samsung Electronics All Rights Reserved.
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
/************************************************************************************
 * configs/stm32h745zi-nucleo/src/stm32_boot.c
 *
 *   Copyright (C) 2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *           Juha Niskanen <juha.niskanen@haltian.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <tinyara/config.h>

#include <stdio.h>
#include <debug.h>

#include <tinyara/arch.h>
#include <tinyara/board.h>
#include <tinyara/spi/spi.h>
#include <tinyara/spi/ospi.h>

#include <arch/board/board.h>

#ifdef CONFIG_PRODCONFIG
#include <tinyara/prodconfig.h>
#include <sys/types.h>
#endif

#include "up_arch.h"
#include "up_internal.h"
#include <stm32h745.h>
#include "common.h"

#ifdef CONFIG_PRODCONFIG
int up_check_prodswd(void)
{
	return OK;
}
int up_check_proddownload(void)
{
	return OK;
}
#endif

/************************************************************************************
 * Name: stm32h745zi_board_initialize
 *
 * Description:
 *   All STM32H745 architectures must provide the following entry point.  This entry point
 *   is called early in the initialization -- after all memory has been configured
 *   and mapped but before any devices have been initialized.
 *
 ************************************************************************************/
void board_initialize(void)
{
    stm32h745_haltick_init();

#ifdef CONFIG_FLASH_PARTITION
    int ret;
    struct mtd_dev_s *mtd;
    partition_info_t partinfo;

    mtd = (FAR struct mtd_dev_s *)mtd_initialize();
    /* Configure mtd partitions */
    ret = configure_mtd_partitions(mtd, 0, &partinfo);
    if (ret != OK) 
    {
        lldbg("ERROR: configure_mtd_partitions failed\n");
        return;
    }

#ifdef CONFIG_AUTOMOUNT
//    automount_fs_partition(&partinfo);
#endif    
#endif

#ifdef CONFIG_SHARED_MEMORY
    stm32h745_shared_memory_init(HSEM_ID_0);
#endif

#if defined(CONFIG_WATCHDOG) && defined(CONFIG_STM32H745_WWDG1)
    stm32h745_wwdginitialize(CONFIG_WATCHDOG_DEVPATH);
#endif

    //up_flashinitialize();
#if defined(CONFIG_TIMER)
    stm32h745_tim_init("/dev/timer0", 16);
#endif

    lldbg("Board init \r\n");
}

/****************************************************************************
 * Name: board_late_initialize
 *
 * Description:
 *   If CONFIG_BOARD_LATE_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_late_initialize().  board_late_initialize() will be
 *   called immediately after up_intiialize() is called and just before the
 *   initial application is started.  This additional initialization phase
 *   may be used, for example, to initialize board-specific device drivers.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_LATE_INITIALIZE
void board_late_initialize(void)
{
    board_app_initialize();
}
#endif
