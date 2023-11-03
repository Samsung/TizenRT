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
 * arch/arm/src/armv7-a/arm_undefinedinsn.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/arch.h>

#ifdef CONFIG_SYSTEM_REBOOT_REASON
#include <tinyara/reboot_reason.h>
#include <arch/reboot_reason.h>
#endif

#include "up_internal.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/
extern uint32_t system_exception_location;

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_undefinedinsn
 ****************************************************************************/

uint32_t *arm_undefinedinsn(uint32_t *regs)
{
  _alert("Undefined instruction at 0x%x\n", regs[REG_PC]);
  uint32_t *saved_state = CURRENT_REGS;
  CURRENT_REGS = regs;

#ifdef CONFIG_SYSTEM_REBOOT_REASON
	up_reboot_reason_write(REBOOT_SYSTEM_PREFETCHABORT);
#endif
  system_exception_location = regs[REG_R15];
  PANIC();
  regs = CURRENT_REGS;
  CURRENT_REGS = saved_state;
  return regs; /* To keep the compiler happy */
}
