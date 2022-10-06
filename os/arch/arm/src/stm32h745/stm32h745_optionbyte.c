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
/****************************************************************************
 * arch/arm/src/stm32h745/stm32h745_optionbyte.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <tinyara/config.h>
#include <tinyara/fs/ioctl.h>
#include <tinyara/fs/mtd.h>

#include <tinyara/init.h>
#include <tinyara/kmalloc.h>

#include <stm32h7xx_hal.h>
#include <string.h>


/****************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/************************************************************************************
 * Private Types
 ************************************************************************************/
/************************************************************************************
 * Private Function Prototypes
 ************************************************************************************/
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32h745_switch_boot_address
 *
 * Description:
 *   switch boot address after reset
 ************************************************************************************/
int stm32h745_switch_boot_address(uint32_t address)
{
    int result = ERROR;
    int timeout = 0x10000;
    FLASH_OBProgramInitTypeDef OBInit;

    if((address < FLASH_BANK1_BASE) || (address > FLASH_BANK2_BASE))
    {
        lldbg("Option byte address may be under or overflow !!\n");
        return ERROR;
    }

    OBInit.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&OBInit);

    __DSB();
    __ISB();

    __disable_irq();
    stm32h745_irq_clear_pending_all();

    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();

    OBInit.BootAddr0 = address;
    OBInit.BootConfig |= OB_BOOT_ADD0;

    if(HAL_FLASHEx_OBProgram(&OBInit) == HAL_ERROR)
    {
        result = ERROR;
    }
    else
    {
        if(HAL_FLASH_OB_Launch() == HAL_ERROR)
        {
            result = ERROR;
        }
        else
        {
            result = OK;
        }
    }


    /* If the I-Cache is enabled, Instruction cache must be invalidated after bank switching
       to ensure that CPU will fetch correct instructions from the flash */
    //SCB_InvalidateICache();
    result = ERROR;
    for(int i=0; i<timeout; i++)
    {
        HAL_FLASHEx_OBGetConfig(&OBInit);
        if(OBInit.BootAddr0 == address)
        {
            result = OK;
            break;
        }        
    }

    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();

    __enable_irq();

    /* To apply board reset required */
    //board_reset();

    return result;
}


/************************************************************************************
 * Name: stm32h745_switch_boot_address_m4
 *
 * Description:
 *   switch boot address after reset for M4 core
 ************************************************************************************/
int stm32h745_switch_boot_address_m4(uint32_t address)
{
    int result = ERROR;
    int timeout = 0x10000;
    FLASH_OBProgramInitTypeDef OBInit;

    if((address < FLASH_BANK1_BASE) || (address > FLASH_BANK2_BASE))
    {
        lldbg("Option byte address may be under or overflow !!\n");
        return ERROR;
    }

    OBInit.Banks = FLASH_BANK_2;
    HAL_FLASHEx_OBGetConfig(&OBInit);

    __DSB();
    __ISB();

    __disable_irq();
    stm32h745_irq_clear_pending_all();

    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();

    OBInit.CM4BootAddr0 = address;
    OBInit.CM4BootConfig |= OB_BOOT_ADD0;

    if(HAL_FLASHEx_OBProgram(&OBInit) == HAL_ERROR)
    {
        result = ERROR;
    }
    else
    {
        if(HAL_FLASH_OB_Launch() == HAL_ERROR)
        {
            result = ERROR;
        }
        else
        {
            result = OK;
        }
    }


    /* If the I-Cache is enabled, Instruction cache must be invalidated after bank switching
       to ensure that CPU will fetch correct instructions from the flash */
    //SCB_InvalidateICache();
    result = ERROR;
    for(int i=0; i<timeout; i++)
    {
        HAL_FLASHEx_OBGetConfig(&OBInit);
        if(OBInit.CM4BootAddr0 == address)
        {
            result = OK;
            break;
        }        
    }

    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();

    __enable_irq();

    /* To apply board reset required */
    //board_reset();

    return result;
}


/************************************************************************************
 * Name: stm32h745_switch_boot_address_m7_m4
 *
 * Description:
 *   switch boot address after reset for M4, M7 both core
 ************************************************************************************/
int  stm32h745_switch_boot_address_m7_m4(uint32_t address_m7, uint32_t address_m4)
{
    int result = ERROR;
    int timeout = 0x10000;
    FLASH_OBProgramInitTypeDef OBInit;

    if((address_m7 < FLASH_BANK1_BASE) || (address_m7 > FLASH_BANK2_BASE))
    {
        lldbg("Option byte m7 address may be under or overflow !!\n");
        return ERROR;
    }

    if((address_m4 < FLASH_BANK1_BASE) || (address_m4 > FLASH_BANK2_BASE))
    {
        lldbg("Option byte m4 address may be under or overflow !!\n");
        return ERROR;
    }

    OBInit.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&OBInit);

    __DSB();
    __ISB();

    __disable_irq();
    stm32h745_irq_clear_pending_all();

    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();

    OBInit.BootAddr0 = address_m7;
    OBInit.BootConfig |= OB_BOOT_ADD0;

    OBInit.CM4BootAddr0 = address_m4;
    OBInit.CM4BootConfig |= OB_BOOT_ADD0;

    if(HAL_FLASHEx_OBProgram(&OBInit) == HAL_ERROR)
    {
        result = ERROR;
    }
    else
    {
        if(HAL_FLASH_OB_Launch() == HAL_ERROR)
        {
            result = ERROR;
        }
        else
        {
            result = OK;
        }
    }


    /* If the I-Cache is enabled, Instruction cache must be invalidated after bank switching
       to ensure that CPU will fetch correct instructions from the flash */
    //SCB_InvalidateICache();
    result = ERROR;
    for(int i=0; i<timeout; i++)
    {
        HAL_FLASHEx_OBGetConfig(&OBInit);
        if((OBInit.BootAddr0 == address_m7) && (OBInit.CM4BootAddr0 == address_m4))
        {
            result = OK;
            break;
        }        
    }

    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();

    __enable_irq();

    /* To apply board reset required */
    //board_reset();

    return result;
}


/************************************************************************************
 * Name: stm32h745_switch_boot_address_m7_m4
 *
 * Description:
 *   switch boot address after reset for M4, M7 both core
 ************************************************************************************/
int  stm32h745_get_boot_address_m7_m4(uint32_t *address_m7, uint32_t *address_m4)
{
    FLASH_OBProgramInitTypeDef OBInit;

    OBInit.Banks = FLASH_BANK_1;
    HAL_FLASHEx_OBGetConfig(&OBInit);

    (*address_m7) = OBInit.BootAddr0;
    (*address_m4) = OBInit.CM4BootAddr0;

    return OK;
}

/************************************************************************************
 * Name: stm32h745_switch_boot_control
 *
 * Description:
 *   switch execution boot after reset
 *   core  : 0 is CM7, 1 is CM4
 *   value : 0 is disable, 1 is enable
 ************************************************************************************/
int stm32h745_get_boot_control(int core, int *value)
{
    int result = OK;
    FLASH_OBProgramInitTypeDef OBInit;

    if(core == 0) /* CM7 */
    {
        OBInit.Banks = FLASH_BANK_1;
    }
    else /* CM4 */
    {
        OBInit.Banks = FLASH_BANK_2;
    }

    HAL_FLASHEx_OBGetConfig(&OBInit);

    if(core == 0) /* CM7 */
    {
        if((OBInit.USERConfig&FLASH_OPTSR_BCM7) == FLASH_OPTSR_BCM7)
        {
            (*value) = 1;
        }
        else
        {
            (*value) = 0;
        }
    }
    else /* CM4 */
    {
        if((OBInit.USERConfig&FLASH_OPTSR_BCM4) == FLASH_OPTSR_BCM4)
        {
            (*value) = 1;
        }
        else
        {
            (*value) = 0;
        }
    }

    return result;    
}
/************************************************************************************
 * Name: stm32h745_switch_boot_control
 *
 * Description:
 *   switch execution boot after reset
 *   core  : 0 is CM7, 1 is CM4
 *   value : 0 is disable, 1 is enable
 ************************************************************************************/
int stm32h745_switch_boot_control(int core, int value)
{
    int result = OK;
    FLASH_OBProgramInitTypeDef OBInit;
    volatile uint32_t USERConfig;

    if(core == 0) /* CM7 */
    {
        OBInit.Banks = FLASH_BANK_1;
    }
    else /* CM4 */
    {
        OBInit.Banks = FLASH_BANK_2;
    }

    HAL_FLASHEx_OBGetConfig(&OBInit);

    __DSB();
    __ISB();

    __disable_irq();
    stm32h745_irq_clear_pending_all();

    HAL_FLASH_Unlock();
    HAL_FLASH_OB_Unlock();

    if(core == 0) /* CM7 */
    {
        OBInit.USERType = OB_USER_BCM7;

        if(value == 0)  /* disable */
        {
            OBInit.USERConfig &= ~FLASH_OPTSR_BCM7;
        }
        else  /* enable */
        {
            OBInit.USERConfig |= FLASH_OPTSR_BCM7;
        }
    }
    else /* CM4 */
    {
        OBInit.USERType = OB_USER_BCM4;

        if(value == 0)  /* disable */
        {
            OBInit.USERConfig &= ~FLASH_OPTSR_BCM4;
        }
        else  /* enable */
        {
            OBInit.USERConfig |= FLASH_OPTSR_BCM4;
        }
    }

    USERConfig = OBInit.USERConfig;

    if(HAL_FLASHEx_OBProgram(&OBInit) == HAL_ERROR)
    {
        result = ERROR;
    }

    if(HAL_FLASH_OB_Launch() == HAL_ERROR)
    {
        result = ERROR;
    }

    HAL_FLASHEx_OBGetConfig(&OBInit);

    if(OBInit.USERConfig != USERConfig)
    {
        result = ERROR;
    }

    HAL_FLASH_OB_Lock();
    HAL_FLASH_Lock();

    __enable_irq();

    /* To apply board reset required */
    //board_reset();

    return result;
}
