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
 * arch/arm/src/stm32h745/stm32h745_wwdg.c
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
#include <tinyara/mm/heap_regioninfo.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <tinyara/init.h>
#include <tinyara/kmalloc.h>
#include <tinyara/watchdog.h>
#include <tinyara/irq.h>

#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"
#include "nvic.h"

#include <stm32h7xx_hal.h>
#include <stm32h7xx_ll_rcc.h>
#include <stm32h7xx_ll_bus.h>
#include <stm32h7xx_ll_wwdg.h>
#include <system_stm32h745.h>


#if defined(CONFIG_WATCHDOG) && defined(CONFIG_STM32H745_WWDG1)

#ifdef CONFIG_DEBUG_WATCHDOG
#define wddbg    lldbg
#define wdvdbg   llvdbg
#else
#define wddbg(x...)
#define wdvdbg(x...)
#endif

#define CONFIG_STM32_WWDG_SETWINDOW (127)
#define CONFIG_STM32_WWDG_TIMEOUT   (300)
/* Control Register (32-bit) */

#define STM32H745_WWDG_CR_T_SHIFT        (0)       /* Bits 6:0 T[6:0]: 7-bit counter (MSB to LSB) */
#define STM32H745_WWDG_CR_T_MASK         (0x7f << STM32H745_WWDG_CR_T_SHIFT)
#define STM32H745_WWDG_CR_T_MAX          (0x7f << STM32H745_WWDG_CR_T_SHIFT)
#define STM32H745_WWDG_CR_T_RESET        (0x40 << STM32H745_WWDG_CR_T_SHIFT)
#define STM32H745_WWDG_CR_WDGA           (1 << 7)  /* Bit 7: Activation bit */

/* Configuration register (32-bit) */

#define STM32H745_WWDG_CFR_W_SHIFT       (0)       /* Bits 6:0 W[6:0] 7-bit window value */
#define STM32H745_WWDG_CFR_W_MASK        (0x7f << STM32H745_WWDG_CFR_W_SHIFT)
#define STM32H745_WWDG_CFR_WDGTB_SHIFT   (11)       /* Bits 13:11 [2:0]: Timer Base */
#define STM32H745_WWDG_CFR_WDGTB_MASK    (7 << STM32H745_WWDG_CFR_WDGTB_SHIFT)
#define STM32H745_WWDG_CFR_APB3          (0 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 000: CK Counter Clock (PCLK1 div 4096) div 1 */
#define STM32H745_WWDG_CFR_APB3d2        (1 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 001: CK Counter Clock (PCLK1 div 4096) div 2 */
#define STM32H745_WWDG_CFR_APB3d4        (2 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 010: CK Counter Clock (PCLK1 div 4096) div 4 */
#define STM32H745_WWDG_CFR_APB3d8        (3 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 011: CK Counter Clock (PCLK1 div 4096) div 8 */
#define STM32H745_WWDG_CFR_APB3d16       (4 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 100: CK Counter Clock (PCLK1 div 4096) div 16 */
#define STM32H745_WWDG_CFR_APB3d32       (5 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 101: CK Counter Clock (PCLK1 div 4096) div 32 */
#define STM32H745_WWDG_CFR_APB3d64       (6 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 110: CK Counter Clock (PCLK1 div 4096) div 64 */
#define STM32H745_WWDG_CFR_APB3d128      (7 << STM32H745_WWDG_CFR_WDGTB_SHIFT) /* 111: CK Counter Clock (PCLK1 div 4096) div 128 */
#define STM32H745_WWDG_CFR_EWI           (1 << 9)  /* Bit 9: Early Wakeup Interrupt */

/* Status register (32-bit) */
#define STM32H745_WWDG_SR_EWIF           (1 << 0)  /* Bit 0: Early Wakeup Interrupt Flag */

#define STM32H745_APB3_FREQUENCY  (100000000ul)
//#define STM32H745_WWDG_MAXTIMEOUT ((1/(STM32H745_APB3_FREQUENCY/1000)) * 4096 * 128 * (127+1))


#define STM32H745_WWDG_FMIN       (STM32H745_APB3_FREQUENCY / 4096 / 128)
#define STM32H745_WWDG_MAXTIMEOUT (1000 * (STM32H745_WWDG_CR_T_MAX+1) / STM32H745_WWDG_FMIN)

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */

struct stm32h745_lowerhalf_s
{
    FAR const struct watchdog_ops_s *ops;   /* Lower half operations */
    xcpt_t handler;             /* Current EWI interrupt handler */
    uint32_t timeout;          /* The actual timeout value */
    uint32_t fwwdg;             /* WWDG clock frequency */
    bool started;               /* The timer has been started */
    uint8_t reload;             /* The 7-bit reload field reset value */
    uint8_t window;             /* The 7-bit window (W) field value */
};


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static int __ramfunc__ stm32h745_wwdg1_start(FAR struct watchdog_lowerhalf_s *lower);
static int __ramfunc__ stm32h745_wwdg1_stop(FAR struct watchdog_lowerhalf_s *lower);
static int __ramfunc__ stm32h745_wwdg1_keepalive(FAR struct watchdog_lowerhalf_s *lower);
static int __ramfunc__ stm32h745_wwdg1_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status);
static int __ramfunc__ stm32h745_wwdg1_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout);
static xcpt_t __ramfunc__ stm32h745_wwdg1_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler);
static int __ramfunc__ stm32h745_wwdg1_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
struct watchdog_ops_s g_wdgops = 
{
    .start = stm32h745_wwdg1_start,
    .stop = stm32h745_wwdg1_stop,
    .keepalive = stm32h745_wwdg1_keepalive,
    .getstatus = stm32h745_wwdg1_getstatus,
    .settimeout = stm32h745_wwdg1_settimeout,
    .capture = stm32h745_wwdg1_capture,
    .ioctl = stm32h745_wwdg1_ioctl,
};

/* "Lower half" driver state */

static struct stm32h745_lowerhalf_s g_wdgdev;

/****************************************************************************
 * Private Functions
 ****************************************************************************/
static int __ramfunc__ up_interrupt(int irq, void *context, FAR void *arg)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)arg;

/* Decide to use this is way to handler than static way */
#if 1  
    /* To normal opertion, it needs to remove keep alive */
    if (priv->handler)
    {
      /* Yes... NOTE:  This interrupt service routine (ISR) must reload
       * the WWDG counter to prevent the reset.  Otherwise, we will reset
       * upon return.
       */

      priv->handler(irq, context, arg);
    }
    LL_WWDG_ClearFlag_EWKUP(WWDG1);
#else
    LL_WWDG_ClearFlag_EWKUP(WWDG1);
    stm32h745_wwdg1_keepalive((FAR struct watchdog_lowerhalf_s *)priv);

    if(priv->started == false)
    {
        return OK;
    }

    wddbg("Go reset!!\n");

    /* These interrupt must be disabled before the M7 reset to prevent IRQ irq_unexpected_isr Hardfault */
    up_disable_irq(STM32H745_IRQ_SYSTICK);
    up_disable_irq(STM32H745_IRQ_TIM17);
    up_disable_irq(STM32H745_IRQ_HSEM1);

    __WFI();
#endif    
    return OK;
}

/****************************************************************************
 * Name: stm32h745_setwindow
 *
 * Description:
 *   Set the CFR window value. The window value is compared to the down-
 *   counter when the counter is updated.  The WWDG counter should be updated
 *   only when the counter is below this window value (and greater than 64)
 *   otherwise a reset will be generated
 *
 ****************************************************************************/
static void __ramfunc__ stm32h745_wwdg1_setwindow(FAR struct stm32h745_lowerhalf_s *priv, uint8_t window)
{
    priv->window = window;

    LL_WWDG_SetWindow(WWDG1, window);
}


/****************************************************************************
 * Name: stm32h745_start
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int __ramfunc__ stm32h745_wwdg1_start(FAR struct watchdog_lowerhalf_s *lower)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;

    stm32h745_wwdg1_keepalive(lower);

    LL_WWDG_ClearFlag_EWKUP(WWDG1);
    LL_WWDG_Enable(WWDG1);
    LL_WWDG_EnableIT_EWKUP(WWDG1);

    up_enable_irq(STM32H745_IRQ_WWDG);
    priv->started = true;
    return OK;
}


/****************************************************************************
 * Name: stm32h745_stop
 *
 * Description:
 *   Stop the watchdog timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int __ramfunc__ stm32h745_wwdg1_stop(FAR struct watchdog_lowerhalf_s *lower)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;

    stm32h745_wwdg1_keepalive(lower);

    priv->started = false;
    return ERROR;
}


/****************************************************************************
 * Name: stm32h745_keepalive
 *
 * Description:
 *   Reset the watchdog timer to the current timeout value, prevent any
 *   imminent watchdog timeouts.  This is sometimes referred as "pinging"
 *   the watchdog timer or "petting the dog".
 *
 *   The application program must write in the WWDG_CR register at regular
 *   intervals during normal operation to prevent an MCU reset. This operation
 *   must occur only when the counter value is lower than the window register
 *   value. The value to be stored in the WWDG_CR register must be between
 *   0xff and 0xC0:
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int __ramfunc__ stm32h745_wwdg1_keepalive(FAR struct watchdog_lowerhalf_s *lower)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;

    LL_WWDG_SetCounter(WWDG1, priv->reload);
    return OK;
}



/****************************************************************************
 * Name: stm32h745_getstatus
 *
 * Description:
 *   Get the current watchdog timer status
 *
 * Input Parameters:
 *   lower  - A pointer the publicly visible representation of the "lower-half"
 *            driver state structure.
 *   status - The location to return the watchdog status information.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int __ramfunc__ stm32h745_wwdg1_getstatus(FAR struct watchdog_lowerhalf_s *lower, FAR struct watchdog_status_s *status)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;
    uint32_t elapsed;
    uint16_t reload;

    DEBUGASSERT(priv);

    /* Return the status bit */
    status->flags = WDFLAGS_RESET;
    if (priv->started) {
        status->flags |= WDFLAGS_ACTIVE;
    }

    if (priv->handler) {
        status->flags |= WDFLAGS_CAPTURE;
    }

    /* Return the actual timeout is milliseconds */

    status->timeout = priv->timeout;

    /* Get the time remaining until the watchdog expires (in milliseconds) */

    reload = LL_WWDG_GetCounter(WWDG1);
    elapsed = priv->reload - reload;
    status->timeleft = (priv->timeout * elapsed) / (priv->reload + 1);

    return OK;
}


/****************************************************************************
 * Name: stm32h745_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the watchdog timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in milliseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int __ramfunc__ stm32h745_wwdg1_settimeout(FAR struct watchdog_lowerhalf_s *lower, uint32_t timeout)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;
    uint32_t fwwdg;
    uint32_t reload;
    uint16_t regval;
    int wdgtb;

    if (timeout < 5 || timeout > STM32H745_WWDG_MAXTIMEOUT)
    {
        wddbg("Cannot represent timeout=%d > %d\n", timeout, STM32H745_WWDG_MAXTIMEOUT);
        return ERROR;
    }

    fwwdg = (int)((double)(timeout) / (double)(((double)1/(double)STM32H745_APB3_FREQUENCY) * (double)4096 * (double)128 * (double)1000) + 0.5);
    reload = fwwdg + 0x3F;

    if(reload < 0x40)
    {
        wddbg("Cannot represent timeout is shortter than 5msec\n");
        return ERROR;
    }

    if(reload > 0x7F)
    {
        wddbg("Cannot represent timeout is greater than max timeout\n");
        return ERROR;
    }

    priv->fwwdg = fwwdg;
    priv->reload = reload;

    regval = LL_WWDG_ReadReg(WWDG1, CFR);
    regval &= ~STM32H745_WWDG_CFR_WDGTB_MASK;
    regval |= (uint16_t)0x07 << STM32H745_WWDG_CFR_WDGTB_SHIFT;
    LL_WWDG_WriteReg(WWDG1, CFR, regval);

    stm32h745_wwdg1_setwindow(priv, CONFIG_STM32_WWDG_SETWINDOW);
    return OK;
}



/****************************************************************************
 * Name: stm32h745_capture
 *
 * Description:
 *   Don't reset on watchdog timer timeout; instead, call this user provider
 *   timeout handler.  NOTE:  Providing handler==NULL will restore the reset
 *   behavior.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the "lower-half"
 *                driver state structure.
 *   newhandler - The new watchdog expiration function pointer.  If this
 *                function pointer is NULL, then the reset-on-expiration
 *                behavior is restored,
 *
 * Returned Values:
 *   The previous watchdog expiration function pointer or NULL is there was
 *   no previous function pointer, i.e., if the previous behavior was
 *   reset-on-expiration (NULL is also returned if an error occurs).
 *
 ****************************************************************************/

static xcpt_t __ramfunc__ stm32h745_wwdg1_capture(FAR struct watchdog_lowerhalf_s *lower, xcpt_t handler)
{
    FAR struct stm32h745_lowerhalf_s *priv = (FAR struct stm32h745_lowerhalf_s *)lower;
    irqstate_t flags;
    xcpt_t oldhandler;
    uint16_t regval;

    DEBUGASSERT(priv);
    /* Get the old handler return value */

    flags = irqsave();
    oldhandler = priv->handler;

    /* Save the new handler */

    priv->handler = handler;

    /* Are we attaching or detaching the handler? */

    regval = READ_REG(WWDG1->CFR);
    if (handler) {
        /* Attaching... Enable the EWI interrupt */

        regval |= WWDG_CFR_EWI;

        WRITE_REG(WWDG1->CFR, regval);
        up_enable_irq(STM32H745_IRQ_WWDG);
    } else {
        /* Detaching... Disable the EWI interrupt */

        regval &= ~WWDG_CFR_EWI;

        WRITE_REG(WWDG1->CFR, regval);
        up_disable_irq(STM32H745_IRQ_WWDG);
    }

    irqrestore(flags);
    return oldhandler;
}

/****************************************************************************
 * Name: stm32h745_ioctl
 *
 * Description:
 *   Any ioctl commands that are not recognized by the "upper-half" driver
 *   are forwarded to the lower half driver through this method.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the "lower-half"
 *           driver state structure.
 *   cmd   - The ioctl command value
 *   arg   - The optional argument that accompanies the 'cmd'.  The
 *           interpretation of this argument depends on the particular
 *           command.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int __ramfunc__ stm32h745_wwdg1_ioctl(FAR struct watchdog_lowerhalf_s *lower, int cmd, unsigned long arg)
{
    int ret=ERROR;

    return ret;
}


/****************************************************************************
 * Name: stm32h745_wwdginitialize
 *
 * Description:
 *   Initialize the WWDG watchdog time.  The watchdog timer is initialized and
 *   registers as 'devpath.  The initial state of the watchdog time is
 *   disabled.
 *
 * Input Parameters:
 *   devpath - The full path to the watchdog.  This should be of the form
 *     /dev/watchdog0
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

void __ramfunc__ stm32h745_wwdginitialize(FAR const char *devpath)
{
    FAR struct stm32h745_lowerhalf_s *priv = &g_wdgdev;

    /* NOTE we assume that clocking to the IWDG has already been provided by
     * the RCC initialization logic.
     */
    /* Initialize the driver state structure.  Here we assume: (1) the state
     * structure lies in .bss and was zeroed at reset time.  (2) This function
     * is only called once so it is never necessary to re-zero the structure.
     */

    priv->ops = &g_wdgops;

    /* Attach our EWI interrupt handler (But don't enable it yet) */

    (void)irq_attach(STM32H745_IRQ_WWDG, up_interrupt, priv);
    (void)up_prioritize_irq(STM32H745_IRQ_WWDG, NVIC_SYSH_PRIORITY_MAX);

    /* Select an arbitrary initial timeout value.  But don't start the watchdog
     * yet. NOTE: If the "Hardware watchdog" feature is enabled through the
     * device option bits, the watchdog is automatically enabled at power-on.
     */
    /* 
     * LL_RCC_WWDG1_EnableSystemReset(); 
     * Don't enable RCC_GCR_WW1RSC in RCC GCR, if it is enabled wwdg will reset whole system.
     */
    LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_WWDG1);

    stm32h745_wwdg1_settimeout((FAR struct watchdog_lowerhalf_s *)priv, CONFIG_STM32_WWDG_TIMEOUT);

    /* Register the watchdog driver as /dev/watchdog0 */
    (void)watchdog_register(devpath, (FAR struct watchdog_lowerhalf_s *)priv);

    wddbg("CF : 0x%08x\n",READ_REG(WWDG1->CFR));
}

#endif  











