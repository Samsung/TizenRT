/**************************************************************************//**
 * @file     rtl8721dhp_system.c
 * @brief    CMSIS Device System Source File for
 *           ARMCM3 Device Series
 * @version  V1.08
 * @date     23. November 2012
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2011 - 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/


#include "basic_types.h"
#include "ameba_soc.h"

/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = 200000000;/*!< System Clock Frequency (Core Clock)*/

u32
SystemGetCpuClk(void)
{
	return CPU_ClkGet();
}

/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)             /* Get Core Clock Frequency      */
{
	SystemCoreClock = SystemGetCpuClk();
}

/**
  * @brief  Set CPU clock.
  * @param  CpuClk: This parameter can be one of the following values
  *		 @arg CLK_KM4_200M
  *		 @arg CLK_KM4_100M
  *		 @arg CLK_KM4_50M
  *		 @arg CLK_KM4_25M
  *		 @arg CLK_KM4_XTAL
  */
void SystemSetCpuClk(u8 CpuClk)
{
	CPU_ClkSet(CpuClk);
	SystemCoreClockUpdate();
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit(void)
{
	// TODO: Hardware initial
#ifdef UNALIGNED_SUPPORT_DISABLE
	SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

	SystemCoreClockUpdate();
}

/**
 * @brief Generate random seed
 * @param  none
 * @return value: random seed value
 */
u32 Gen_RandomSeed(VOID)
{
#ifdef AMEBAD_TODO
	u32 adc_tmp, isr, AdcTempDat;
	u8 random_tmp, random[4];
	ADC_TypeDef *adc = ADC;
	int i, j;

	/* init ADC*/
	ADC_InitTypeDef AdcInitStruct;
	ADC_InitStruct(&AdcInitStruct);
	AdcInitStruct.ADC_BurstSz = 2;
	ADC_AnaparAd[1] = 0x41004;

	InterruptDis(ADC_IRQ);
	PLL2_Set(BIT_SYS_SYSPLL_CK_ADC_EN, ENABLE);
	ADC_Init(&AdcInitStruct);

	/* Clear ADC Status */
	ADC_INTClear();
	ADC_INTConfig(BIT_ADC_FIFO_FULL_EN | BIT_ADC_FIFO_RD_REQ_EN, ENABLE);
	ADC_Cmd(ENABLE);

	/* B CUT ADD patch for reset fail */
	AdcTempDat = adc->ANAPAR_AD1;
	AdcTempDat |= BIT(0);
	adc->ANAPAR_AD1 = AdcTempDat;

	for (i = 0; i < 4; i++) {
retry:
		random_tmp = 0;
		for (j = 0; j < 8; j++) {
			while (1) {
				isr = ADC_GetISR();
				if (isr & (BIT_ADC_FIFO_FULL | BIT_ADC_FIFO_RD_REQ)) {
					adc_tmp = ADC_Read();
					ADC_Read();

					ADC_INTClear();
					ADC_INTConfig(BIT_ADC_FIFO_FULL_EN | BIT_ADC_FIFO_RD_REQ_EN, DISABLE);

					break;
				}
			}
			random_tmp |= ((adc_tmp & 0x1) << j);
		}

		if ((random_tmp == 0x00) || (random_tmp == 0xff)) {
			goto retry;
		}

		random[i] = random_tmp;
	}

	/* B CUT ADD patch for reset fail */
	AdcTempDat = adc->ANAPAR_AD1;
	AdcTempDat &= ~ BIT(0);
	adc->ANAPAR_AD1 = AdcTempDat;

	/* disable ADC*/
	ADC_Cmd(DISABLE);
	ADC_INTClear();
	PLL2_Set(BIT_SYS_SYSPLL_CK_ADC_EN, DISABLE);

	return *(u32 *)random;
#else

	return 0;
#endif
}

/**
  * @brief  get the state of rdp, enable or disable
  * @retval : This parameter can be one of the following values:
  *            @arg TRUE: rdp is enable
  *            @arg FALSE: rdp is disable
  */
u32 IsRDPenabled(void)
{
	return FALSE;
}


/**
  * @brief  Get CPU interrupt status.
  * @return CPU interrupt status 0/1
  */
u32 CPU_InInterrupt(void)
{
#ifdef ARM_CORE_CA32
	return (__get_mode() != CPSR_M_USR) && (__get_mode() != CPSR_M_SYS);
#else
	return __get_IPSR() != 0;

#endif
}

