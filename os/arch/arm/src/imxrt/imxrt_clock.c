/****************************************************************************
 *
 * Copyright 2019 NXP Semiconductors All Rights Reserved.
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

#include "imxrt_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* To make full use of CM7 hardware FPU, use double instead of uint64_t in clock driver to
achieve better performance, it is depend on the IDE Floating point settings, if double precision is selected
in IDE, clock_64b_t will switch to double type automatically. only support IAR and MDK here */
#ifdef ARCH_HAVE_FPU

#if ((defined(__ICCARM__)) || (defined(__GNUC__)))

#if (__ARMVFP__ >= __ARMFPV5__) && \
    (__ARM_FP == 0xE) /*0xe implies support for half, single and double precision operations*/
typedef double clock_64b_t;
#else
typedef uint64_t clock_64b_t;
#endif

#elif defined(__CC_ARM) || defined(__ARMCC_VERSION)

#if defined __TARGET_FPU_FPV5_D16
typedef double clock_64b_t;
#else
typedef uint64_t clock_64b_t;
#endif

#else
typedef uint64_t clock_64b_t;
#endif

#else
typedef uint64_t clock_64b_t;
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* External XTAL (OSC) clock frequency. */
volatile uint32_t g_xtalFreq;
/* External RTC XTAL clock frequency. */
volatile uint32_t g_rtcXtalFreq;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief Get the periph clock frequency.
 *
 * @return Periph clock frequency in Hz.
 */
static uint32_t imxrt_clock_getperiphclkfreq(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/****************************************************************************
 * Function: imxrt_clock_getperiphclkfreq
 *
 * Description:
 *   Get the peripheral clock frequency
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns the resulting frequency
 *
 ****************************************************************************/
static uint32_t imxrt_clock_getperiphclkfreq(void)
{
    uint32_t freq;

    /* Periph_clk2_clk ---> Periph_clk */
    if (CCM->CBCDR & CCM_CBCDR_PERIPH_CLK_SEL_MASK) {
        switch (CCM->CBCMR & CCM_CBCMR_PERIPH_CLK2_SEL_MASK) {
        /* Pll3_sw_clk ---> Periph_clk2_clk ---> Periph_clk */
        case CCM_CBCMR_PERIPH_CLK2_SEL(0U):
            freq = imxrt_clock_getpllfreq(kCLOCK_PllUsb1);
            break;
        /* Osc_clk ---> Periph_clk2_clk ---> Periph_clk */
        case CCM_CBCMR_PERIPH_CLK2_SEL(1U):
            freq = imxrt_clock_getoscfreq();
            break;
        case CCM_CBCMR_PERIPH_CLK2_SEL(2U):
            freq = imxrt_clock_getpllfreq(kCLOCK_PllSys);
            break;
        case CCM_CBCMR_PERIPH_CLK2_SEL(3U):
        default:
            freq = 0U;
            break;
        }

        freq /= (((CCM->CBCDR & CCM_CBCDR_PERIPH_CLK2_PODF_MASK) >> CCM_CBCDR_PERIPH_CLK2_PODF_SHIFT) + 1U);
    } else { /* Pre_Periph_clk ---> Periph_clk */
        switch (CCM->CBCMR & CCM_CBCMR_PRE_PERIPH_CLK_SEL_MASK) {
        /* PLL2 ---> Pre_Periph_clk ---> Periph_clk */
        case CCM_CBCMR_PRE_PERIPH_CLK_SEL(0U):
            freq = imxrt_clock_getpllfreq(kCLOCK_PllSys);
            break;
        /* PLL2 PFD2 ---> Pre_Periph_clk ---> Periph_clk */
        case CCM_CBCMR_PRE_PERIPH_CLK_SEL(1U):
            freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd2);
            break;
        /* PLL2 PFD0 ---> Pre_Periph_clk ---> Periph_clk */
        case CCM_CBCMR_PRE_PERIPH_CLK_SEL(2U):
            freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd0);
            break;
        /* PLL1 divided(/2) ---> Pre_Periph_clk ---> Periph_clk */
        case CCM_CBCMR_PRE_PERIPH_CLK_SEL(3U):
            freq = imxrt_clock_getpllfreq(kCLOCK_PllArm) /
                    (((CCM->CACRR & CCM_CACRR_ARM_PODF_MASK) >> CCM_CACRR_ARM_PODF_SHIFT) + 1U);
            break;
        default:
            freq = 0U;
            break;
        }
    }

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_initexternalclk
 *
 * Description:
 *   brief Initialize the external 24MHz clock.
 *
 *   This function supports two modes:
 *     1. Use external crystal oscillator.
 *     2. Bypass the external crystal oscillator, using input source clock directly.
 *
 *   After this function, please call ref CLOCK_SetXtal0Freq to inform clock driver
 *   the external clock frequency.
 *
 * Input Parameters:
 *   bypassXtalOsc Pass in true to bypass the external crystal oscillator.
 *   note This device does not support bypass external crystal oscillator, so
 *   the input parameter should always be false.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initexternalclk(bool bypassXtalOsc)
{
    /* This device does not support bypass XTAL OSC. */
    DEBUGASSERT(!bypassXtalOsc);

    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power up */
    while ((XTALOSC24M->LOWPWR_CTRL & XTALOSC24M_LOWPWR_CTRL_XTALOSC_PWRUP_STAT_MASK) == 0) {
    }
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK; /* detect freq */
    while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_OSC_XTALOK_MASK) == 0) {
    }
    CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_OSC_XTALOK_EN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitexternalclk
 *
 * Description:
 *   brief Deinitialize the external 24MHz clock.
 *
 *   This function disables the external 24MHz clock.
 *
 *   After this function, please call ref CLOCK_SetXtal0Freq to set external clock
 *   frequency to 0.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitexternalclk(void)
{
    CCM_ANALOG->MISC0_SET = CCM_ANALOG_MISC0_XTAL_24M_PWD_MASK; /* Power down */
}

/****************************************************************************
 * Function: imxrt_clock_getperiphclkfreq
 *
 * Description:
 *   brief Switch the OSC.
 *
 *   This function switches the OSC source for SoC.
 *
 * Input Parameters:
 *   param osc   OSC source to switch to.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_switchosc(clock_osc_t osc)
{
    if (osc == kCLOCK_RcOsc)
        XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_SET_OSC_SEL_MASK;
    else
        XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_CLR_OSC_SEL_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initrcosc24m
 *
 * Description:
 *   brief Initialize the RC oscillator 24MHz clock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initrcosc24m(void)
{
    XTALOSC24M->LOWPWR_CTRL |= XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitrcosc24m
 *
 * Description:
 *   brief Power down the RCOSC 24M clock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitrcosc24m(void)
{
    XTALOSC24M->LOWPWR_CTRL &= ~XTALOSC24M_LOWPWR_CTRL_RC_OSC_EN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_getahbfreq
 *
 * Description:
 *   brief Gets the AHB clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns The AHB clock frequency value in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getahbfreq(void)
{
    return imxrt_clock_getperiphclkfreq() / (((CCM->CBCDR & CCM_CBCDR_AHB_PODF_MASK) >> CCM_CBCDR_AHB_PODF_SHIFT) + 1U);
}

/****************************************************************************
 * Function: imxrt_clock_getsemcfreq
 *
 * Description:
 *   brief Gets the SEMC clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns The SEMC clock frequency value in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getsemcfreq(void)
{
    uint32_t freq;

    /* SEMC alternative clock ---> SEMC Clock */
    if (CCM->CBCDR & CCM_CBCDR_SEMC_CLK_SEL_MASK) {
        /* PLL3 PFD1 ---> SEMC alternative clock ---> SEMC Clock */
        if (CCM->CBCDR & CCM_CBCDR_SEMC_ALT_CLK_SEL_MASK) {
            freq = imxrt_clock_getusb1pfdfreq(kCLOCK_Pfd1);
        } else { /* PLL2 PFD2 ---> SEMC alternative clock ---> SEMC Clock */
            freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd2);
        }
    } else { /* Periph_clk ---> SEMC Clock */
        freq = imxrt_clock_getperiphclkfreq();
    }

    freq /= (((CCM->CBCDR & CCM_CBCDR_SEMC_PODF_MASK) >> CCM_CBCDR_SEMC_PODF_SHIFT) + 1U);

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_getipgfreq
 *
 * Description:
 *   brief Gets the IPG clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns The IPG clock frequency value in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getipgfreq(void)
{
    return imxrt_clock_getahbfreq() / (((CCM->CBCDR & CCM_CBCDR_IPG_PODF_MASK) >> CCM_CBCDR_IPG_PODF_SHIFT) + 1U);
}

/****************************************************************************
 * Function: imxrt_clock_getperclkfreq
 *
 * Description:
 *   brief Gets the PER clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Returns The PER clock frequency value in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getperclkfreq(void)
{
    uint32_t freq;

    /* Osc_clk ---> PER Clock*/
    if (CCM->CSCMR1 & CCM_CSCMR1_PERCLK_CLK_SEL_MASK) {
        freq = imxrt_clock_getoscfreq();
    } else { /* Periph_clk ---> AHB Clock ---> IPG Clock ---> PER Clock */
        freq = imxrt_clock_getfreq(kCLOCK_IpgClk);
    }

    freq /= (((CCM->CSCMR1 & CCM_CSCMR1_PERCLK_PODF_MASK) >> CCM_CSCMR1_PERCLK_PODF_SHIFT) + 1U);

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_getfreq
 *
 * Description:
 *   brief Gets the clock frequency for a specific clock name.
 * 
 *   This function checks the current clock configurations and then calculates
 *   the clock frequency for a specific clock name defined in clock_name_t.
 *
 * Input Parameters:
 *   clockName Clock names defined in clock_name_t
 *
 * Returned Value:
 *   Returns Clock frequency value in hertz
 *
 ****************************************************************************/
uint32_t imxrt_clock_getfreq(clock_name_t name)
{
    uint32_t freq;

    switch (name) {
    case kCLOCK_CpuClk:
    case kCLOCK_AhbClk:
        freq = imxrt_clock_getahbfreq();
        break;
    case kCLOCK_SemcClk:
        freq = imxrt_clock_getsemcfreq();
        break;
    case kCLOCK_IpgClk:
        freq = imxrt_clock_getipgfreq();
        break;
    case kCLOCK_PerClk:
        freq = imxrt_clock_getperclkfreq();
        break;
    case kCLOCK_OscClk:
        freq = imxrt_clock_getoscfreq();
        break;
    case kCLOCK_RtcClk:
        freq = imxrt_clock_getrtcfreq();
        break;
    case kCLOCK_ArmPllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllArm);
        break;
    case kCLOCK_Usb1PllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllUsb1);
        break;
    case kCLOCK_Usb1PllPfd0Clk:
        freq = imxrt_clock_getusb1pfdfreq(kCLOCK_Pfd0);
        break;
    case kCLOCK_Usb1PllPfd1Clk:
        freq = imxrt_clock_getusb1pfdfreq(kCLOCK_Pfd1);
        break;
    case kCLOCK_Usb1PllPfd2Clk:
        freq = imxrt_clock_getusb1pfdfreq(kCLOCK_Pfd2);
        break;
    case kCLOCK_Usb1PllPfd3Clk:
        freq = imxrt_clock_getusb1pfdfreq(kCLOCK_Pfd3);
        break;
    case kCLOCK_Usb2PllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllUsb2);
        break;
    case kCLOCK_SysPllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllSys);
        break;
    case kCLOCK_SysPllPfd0Clk:
        freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd0);
        break;
    case kCLOCK_SysPllPfd1Clk:
        freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd1);
        break;
    case kCLOCK_SysPllPfd2Clk:
        freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd2);
        break;
    case kCLOCK_SysPllPfd3Clk:
        freq = imxrt_clock_getsyspfdfreq(kCLOCK_Pfd3);
        break;
    case kCLOCK_EnetPll0Clk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllEnet);
        break;
    case kCLOCK_EnetPll1Clk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllEnet25M);
        break;
    case kCLOCK_AudioPllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllAudio);
        break;
    case kCLOCK_VideoPllClk:
        freq = imxrt_clock_getpllfreq(kCLOCK_PllVideo);
        break;
    default:
        freq = 0U;
        break;
    }

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_enableusbhs0clock
 *
 * Description:
 *   brief Enable USB HS clock.
 * 
 *   This function only enables the access to USB HS prepheral, upper layer
 *   should first call the ref CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 *   clock to use USB HS.
 *
 * Input Parameters:
 *   src  USB HS does not care about the clock source, here must be ref kCLOCK_UsbSrcUnused.
 *   freq USB HS does not care about the clock source, so this parameter is ignored.
 *
 * Returned Value:
 *   Returns true The clock is set successfully.
 *   Returns false The clock source is invalid to get proper USB HS clock.
 *
 ****************************************************************************/
bool imxrt_clock_enableusbhs0clock(clock_usb_src_t src, uint32_t freq)
{
    CCM->CCGR6 |= CCM_CCGR6_CG0_MASK;
    USB1->USBCMD |= USBHS_USBCMD_RST_MASK;
    for (volatile uint32_t i = 0; i < 400000; i++) {/* Add a delay between RST and RS so make sure there is a DP pullup sequence*/
        __asm("nop");
    }
    PMU->REG_3P0 = (PMU->REG_3P0 & (~PMU_REG_3P0_OUTPUT_TRG_MASK)) |
                   (PMU_REG_3P0_OUTPUT_TRG(0x17) | PMU_REG_3P0_ENABLE_LINREG_MASK);
    return true;
}

/****************************************************************************
 * Function: imxrt_clock_enableusbhs1clock
 *
 * Description:
 *   brief Enable USB HS clock.
 *
 *   This function only enables the access to USB HS prepheral, upper layer
 *   should first call the ref CLOCK_EnableUsbhs0PhyPllClock to enable the PHY
 *   clock to use USB HS.
 *
 * Input Parameters:
 *   src  USB HS does not care about the clock source, here must be ref kCLOCK_UsbSrcUnused.
 *   freq USB HS does not care about the clock source, so this parameter is ignored.
 *
 * Returned Value:
 *   Returns true The clock is set successfully.
 *   Returns false The clock source is invalid to get proper USB HS clock.
 *
 ****************************************************************************/
bool imxrt_clock_enableusbhs1clock(clock_usb_src_t src, uint32_t freq)
{
    CCM->CCGR6 |= CCM_CCGR6_CG0_MASK;
    USB2->USBCMD |= USBHS_USBCMD_RST_MASK;
    for (volatile uint32_t i = 0; i < 400000; i++) { /* Add a delay between RST and RS so make sure there is a DP pullup sequence*/
        __asm("nop");
    }
    PMU->REG_3P0 = (PMU->REG_3P0 & (~PMU_REG_3P0_OUTPUT_TRG_MASK)) |
                   (PMU_REG_3P0_OUTPUT_TRG(0x17) | PMU_REG_3P0_ENABLE_LINREG_MASK);
    return true;
}

/****************************************************************************
 * Function: imxrt_clock_enableusbhs0phypllclock
 *
 * Description:
 *   brief Enable USB HS PHY PLL clock.
 *
 *   This function enables the internal 480MHz USB PHY PLL clock.
 *
 * Input Parameters:
 *   src  USB HS PHY PLL clock source.
 *   freq The frequency specified by src.
 *
 * Returned Value:
 *   Returns true The clock is set successfully.
 *   Returns false The clock source is invalid to get proper USB HS clock.
 *
 ****************************************************************************/
bool imxrt_clock_enableusbhs0phypllclock(clock_usb_phy_src_t src, uint32_t freq)
{
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
    if (CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_ENABLE_MASK) {
        CCM_ANALOG->PLL_USB1 |= CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
    } else {
        imxrt_clock_initusb1pll(&g_ccmConfigUsbPll);
    }
    USBPHY1->CTRL &= ~USBPHY_CTRL_SFTRST_MASK; /* release PHY from reset */
    USBPHY1->CTRL &= ~USBPHY_CTRL_CLKGATE_MASK;

    USBPHY1->PWD = 0;
    USBPHY1->CTRL |= USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK | USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK |
                     USBPHY_CTRL_ENUTMILEVEL2_MASK | USBPHY_CTRL_ENUTMILEVEL3_MASK;
    return true;
}

/****************************************************************************
 * Function: imxrt_clock_disableusbhs0phypllclock
 *
 * Description:
 *   brief Disable USB HS PHY PLL clock.
 *
 *   This function disables USB HS PHY PLL clock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_disableusbhs0phypllclock(void)
{
    CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK;
    USBPHY1->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* Set to 1U to gate clocks */
}

/****************************************************************************
 * Function: imxrt_clock_initarmpll
 *
 * Description:
 *   brief Initialize the ARM PLL.
 *
 *   This function initialize the ARM PLL with specific settings
 *
 * Input Parameters:
 *   config   configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initarmpll(const clock_arm_pll_config_t *config)
{
    /* Bypass PLL first */
    CCM_ANALOG->PLL_ARM = (CCM_ANALOG->PLL_ARM & (~CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC_MASK)) |
                          CCM_ANALOG_PLL_ARM_BYPASS_MASK | CCM_ANALOG_PLL_ARM_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_ARM =
        (CCM_ANALOG->PLL_ARM & (~(CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK | CCM_ANALOG_PLL_ARM_POWERDOWN_MASK))) |
        CCM_ANALOG_PLL_ARM_ENABLE_MASK | CCM_ANALOG_PLL_ARM_DIV_SELECT(config->loopDivider);

    while ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_ARM &= ~CCM_ANALOG_PLL_ARM_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitarmpll
 *
 * Description:
 *   brief De-initialize the ARM PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitarmpll(void)
{
    CCM_ANALOG->PLL_ARM = CCM_ANALOG_PLL_ARM_POWERDOWN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initsyspll
 *
 * Description:
 *   brief Initialize the System PLL.
 *
 *   This function initializes the System PLL with specific settings
 *
 * Input Parameters:
 *   config Configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initsyspll(const clock_sys_pll_config_t *config)
{
    /* Bypass PLL first */
    CCM_ANALOG->PLL_SYS = (CCM_ANALOG->PLL_SYS & (~CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC_MASK)) |
                          CCM_ANALOG_PLL_SYS_BYPASS_MASK | CCM_ANALOG_PLL_SYS_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_SYS =
        (CCM_ANALOG->PLL_SYS & (~(CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK | CCM_ANALOG_PLL_SYS_POWERDOWN_MASK))) |
        CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_DIV_SELECT(config->loopDivider);

    /* Initialize the fractional mode */
    CCM_ANALOG->PLL_SYS_NUM = CCM_ANALOG_PLL_SYS_NUM_A(config->numerator);
    CCM_ANALOG->PLL_SYS_DENOM = CCM_ANALOG_PLL_SYS_DENOM_B(config->denominator);

    /* Initialize the spread spectrum mode */
    CCM_ANALOG->PLL_SYS_SS = CCM_ANALOG_PLL_SYS_SS_STEP(config->ss_step) |
                             CCM_ANALOG_PLL_SYS_SS_ENABLE(config->ss_enable) |
                             CCM_ANALOG_PLL_SYS_SS_STOP(config->ss_stop);

    while ((CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_SYS &= ~CCM_ANALOG_PLL_SYS_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initsyspll
 *
 * Description:
 *   brief De-initialize the System PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitsyspll(void)
{
    CCM_ANALOG->PLL_SYS = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initusb1pll
 *
 * Description:
 *   brief Initialize the USB1 PLL.
 *
 *   This function initializes the USB1 PLL with specific settings
 *
 * Input Parameters:
 *   config Configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initusb1pll(const clock_usb_pll_config_t *config)
{
    /* Bypass PLL first */
    CCM_ANALOG->PLL_USB1 = (CCM_ANALOG->PLL_USB1 & (~CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC_MASK)) |
                           CCM_ANALOG_PLL_USB1_BYPASS_MASK | CCM_ANALOG_PLL_USB1_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_USB1 = (CCM_ANALOG->PLL_USB1 & (~CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK)) |
                           CCM_ANALOG_PLL_USB1_ENABLE_MASK | CCM_ANALOG_PLL_USB1_POWER_MASK |
                           CCM_ANALOG_PLL_USB1_EN_USB_CLKS_MASK | CCM_ANALOG_PLL_USB1_DIV_SELECT(config->loopDivider);

    while ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_USB1 &= ~CCM_ANALOG_PLL_USB1_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initusb1pll
 *
 * Description:
 *   brief Deinitialize the USB1 PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitusb1pll(void)
{
    CCM_ANALOG->PLL_USB1 = 0U;
}

/****************************************************************************
 * Function: imxrt_clock_initusb2pll
 *
 * Description:
 *   brief Initialize the USB2 PLL.
 *
 *   This function initializes the USB2 PLL with specific settings
 *
 * Input Parameters:
 *   config Configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initusb2pll(const clock_usb_pll_config_t *config)
{
    /* Bypass PLL first */
    CCM_ANALOG->PLL_USB2 = (CCM_ANALOG->PLL_USB2 & (~CCM_ANALOG_PLL_USB2_BYPASS_CLK_SRC_MASK)) |
                           CCM_ANALOG_PLL_USB2_BYPASS_MASK | CCM_ANALOG_PLL_USB2_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_USB2 = (CCM_ANALOG->PLL_USB2 & (~CCM_ANALOG_PLL_USB2_DIV_SELECT_MASK)) |
                           CCM_ANALOG_PLL_USB2_ENABLE_MASK | CCM_ANALOG_PLL_USB2_POWER_MASK |
                           CCM_ANALOG_PLL_USB2_EN_USB_CLKS_MASK | CCM_ANALOG_PLL_USB2_DIV_SELECT(config->loopDivider);

    while ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_USB2 &= ~CCM_ANALOG_PLL_USB2_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitusb2pll
 *
 * Description:
 *   brief Deinitialize the USB2 PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitusb2pll(void)
{
    CCM_ANALOG->PLL_USB2 = 0U;
}

/****************************************************************************
 * Function: imxrt_clock_initaudiopll
 *
 * Description:
 *   brief Initializes the Audio PLL.
 *
 *   This function initializes the Audio PLL with specific settings
 *
 * Input Parameters:
 *   config Configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initaudiopll(const clock_audio_pll_config_t *config)
{
    uint32_t pllAudio;
    uint32_t misc2 = 0;

    /* Bypass PLL first */
    CCM_ANALOG->PLL_AUDIO = (CCM_ANALOG->PLL_AUDIO & (~CCM_ANALOG_PLL_AUDIO_BYPASS_CLK_SRC_MASK)) |
                            CCM_ANALOG_PLL_AUDIO_BYPASS_MASK | CCM_ANALOG_PLL_AUDIO_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_AUDIO_NUM = CCM_ANALOG_PLL_AUDIO_NUM_A(config->numerator);
    CCM_ANALOG->PLL_AUDIO_DENOM = CCM_ANALOG_PLL_AUDIO_DENOM_B(config->denominator);

    /*
     * Set post divider:
     *
     * ------------------------------------------------------------------------
     * | config->postDivider | PLL_AUDIO[POST_DIV_SELECT]  | MISC2[AUDIO_DIV] |
     * ------------------------------------------------------------------------
     * |         1           |            2                |        0         |
     * ------------------------------------------------------------------------
     * |         2           |            1                |        0         |
     * ------------------------------------------------------------------------
     * |         4           |            2                |        3         |
     * ------------------------------------------------------------------------
     * |         8           |            1                |        3         |
     * ------------------------------------------------------------------------
     * |         16          |            0                |        3         |
     * ------------------------------------------------------------------------
     */
    pllAudio =
        (CCM_ANALOG->PLL_AUDIO & (~(CCM_ANALOG_PLL_AUDIO_DIV_SELECT_MASK | CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK))) |
        CCM_ANALOG_PLL_AUDIO_ENABLE_MASK | CCM_ANALOG_PLL_AUDIO_DIV_SELECT(config->loopDivider);

    switch (config->postDivider) {
    case 16:
        pllAudio |= CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(0);
        misc2 = CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK;
        break;
    case 8:
        pllAudio |= CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(1);
        misc2 = CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK;
        break;
    case 4:
        pllAudio |= CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(2);
        misc2 = CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK;
        break;
    case 2:
        pllAudio |= CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(1);
        break;
    default:
        pllAudio |= CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(2);
        break;
    }

    CCM_ANALOG->MISC2 =
        (CCM_ANALOG->MISC2 & ~(CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK)) | misc2;

    CCM_ANALOG->PLL_AUDIO = pllAudio;

    while ((CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_AUDIO &= ~CCM_ANALOG_PLL_AUDIO_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitaudiopll
 *
 * Description:
 *   brief De-initialize the Audio PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitaudiopll(void)
{
    CCM_ANALOG->PLL_AUDIO = CCM_ANALOG_PLL_AUDIO_POWERDOWN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initvideopll
 *
 * Description:
 *   brief Initialize the video PLL.
 *
 *   This function configures the Video PLL with specific settings
 *
 * Input Parameters:
 *   config   configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initvideopll(const clock_video_pll_config_t *config)
{
    uint32_t pllVideo;
    uint32_t misc2 = 0;

    /* Bypass PLL first */
    CCM_ANALOG->PLL_VIDEO = (CCM_ANALOG->PLL_VIDEO & (~CCM_ANALOG_PLL_VIDEO_BYPASS_CLK_SRC_MASK)) |
                            CCM_ANALOG_PLL_VIDEO_BYPASS_MASK | CCM_ANALOG_PLL_VIDEO_BYPASS_CLK_SRC(config->src);

    CCM_ANALOG->PLL_VIDEO_NUM = CCM_ANALOG_PLL_VIDEO_NUM_A(config->numerator);
    CCM_ANALOG->PLL_VIDEO_DENOM = CCM_ANALOG_PLL_VIDEO_DENOM_B(config->denominator);

    /*
     * Set post divider:
     *
     * ------------------------------------------------------------------------
     * | config->postDivider | PLL_VIDEO[POST_DIV_SELECT]  | MISC2[VIDEO_DIV] |
     * ------------------------------------------------------------------------
     * |         1           |            2                |        0         |
     * ------------------------------------------------------------------------
     * |         2           |            1                |        0         |
     * ------------------------------------------------------------------------
     * |         4           |            2                |        3         |
     * ------------------------------------------------------------------------
     * |         8           |            1                |        3         |
     * ------------------------------------------------------------------------
     * |         16          |            0                |        3         |
     * ------------------------------------------------------------------------
     */
    pllVideo =
        (CCM_ANALOG->PLL_VIDEO & (~(CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK | CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK))) |
        CCM_ANALOG_PLL_VIDEO_ENABLE_MASK | CCM_ANALOG_PLL_VIDEO_DIV_SELECT(config->loopDivider);

    switch (config->postDivider) {
    case 16:
        pllVideo |= CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(0);
        misc2 = CCM_ANALOG_MISC2_VIDEO_DIV(3);
        break;
    case 8:
        pllVideo |= CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(1);
        misc2 = CCM_ANALOG_MISC2_VIDEO_DIV(3);
        break;
    case 4:
        pllVideo |= CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(2);
        misc2 = CCM_ANALOG_MISC2_VIDEO_DIV(3);
        break;
    case 2:
        pllVideo |= CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(1);
        break;
    default:
        pllVideo |= CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(2);
        break;
    }

    CCM_ANALOG->MISC2 = (CCM_ANALOG->MISC2 & ~CCM_ANALOG_MISC2_VIDEO_DIV_MASK) | misc2;

    CCM_ANALOG->PLL_VIDEO = pllVideo;

    while ((CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_VIDEO &= ~CCM_ANALOG_PLL_VIDEO_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitvideopll
 *
 * Description:
 *   brief De-initialize the Video PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitvideopll(void)
{
    CCM_ANALOG->PLL_VIDEO = CCM_ANALOG_PLL_VIDEO_POWERDOWN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_initenetpll
 *
 * Description:
 *   brief Initialize the ENET PLL.
 *
 *   This function initializes the ENET PLL with specific settings.
 *
 * Input Parameters:
 *   config Configuration to set to PLL.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initenetpll(const clock_enet_pll_config_t *config)
{
    uint32_t enet_pll = CCM_ANALOG_PLL_ENET_DIV_SELECT(config->loopDivider);

    CCM_ANALOG->PLL_ENET = (CCM_ANALOG->PLL_ENET & (~CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC_MASK)) |
                           CCM_ANALOG_PLL_ENET_BYPASS_MASK | CCM_ANALOG_PLL_ENET_BYPASS_CLK_SRC(config->src);

    if (config->enableClkOutput) {
        enet_pll |= CCM_ANALOG_PLL_ENET_ENABLE_MASK;
    }

    if (config->enableClkOutput25M) {
        enet_pll |= CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_MASK;
    }

    CCM_ANALOG->PLL_ENET =
        (CCM_ANALOG->PLL_ENET & (~(CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK | CCM_ANALOG_PLL_ENET_POWERDOWN_MASK))) |
        enet_pll;

    /* Wait for stable */
    while ((CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_MASK) == 0) {
    }

    /* Disable Bypass */
    CCM_ANALOG->PLL_ENET &= ~CCM_ANALOG_PLL_ENET_BYPASS_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_deinitenetpll
 *
 * Description:
 *   brief Deinitialize the ENET PLL.
 * 
 *   This function disables the ENET PLL.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitenetpll(void)
{
    CCM_ANALOG->PLL_ENET = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
}

/****************************************************************************
 * Function: imxrt_clock_getpllfreq
 *
 * Description:
 *   brief Get current PLL output frequency.
 *
 *   This function get current output frequency of specific PLL
 *
 * Input Parameters:
 *   pll   pll name to get frequency.
 *
 * Returned Value:
 *   Returns The PLL output frequency in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getpllfreq(clock_pll_t pll)
{
    uint32_t freq;
    uint32_t divSelect;
    clock_64b_t freqTmp;

    const uint32_t enetRefClkFreq[] = {
        25000000U,  /* 25M */
        50000000U,  /* 50M */
        100000000U, /* 100M */
        125000000U  /* 125M */
    };

    /* check if PLL is enabled */
    if (!imxrt_clock_ispllenabled(CCM_ANALOG, pll)) {
        return 0U;
    }

    /* get pll reference clock */
    freq = imxrt_clock_getpllbypassrefclk(CCM_ANALOG, pll);

    /* check if pll is bypassed */
    if (imxrt_clock_ispllbypassed(CCM_ANALOG, pll)) {
        return freq;
    }

    switch (pll) {
    case kCLOCK_PllArm:
        freq = ((freq * ((CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_DIV_SELECT_MASK) >>
                            CCM_ANALOG_PLL_ARM_DIV_SELECT_SHIFT)) >>
                1U);
        break;
    case kCLOCK_PllSys:
        /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
        freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG->PLL_SYS_NUM))) /
                    ((clock_64b_t)(CCM_ANALOG->PLL_SYS_DENOM));

        if (CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_DIV_SELECT_MASK) {
            freq *= 22U;
        } else {
            freq *= 20U;
        }

        freq += (uint32_t)freqTmp;
        break;
    case kCLOCK_PllUsb1:
        freq = (freq * ((CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_DIV_SELECT_MASK) ? 22U : 20U));
        break;
    case kCLOCK_PllAudio:
        /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
        divSelect =
            (CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_AUDIO_DIV_SELECT_SHIFT;

        freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG->PLL_AUDIO_NUM))) /
                    ((clock_64b_t)(CCM_ANALOG->PLL_AUDIO_DENOM));

        freq = freq * divSelect + (uint32_t)freqTmp;

        /* AUDIO PLL output = PLL output frequency / POSTDIV. */

        /*
            * Post divider:
            *
            * PLL_AUDIO[POST_DIV_SELECT]:
            * 0x00: 4
            * 0x01: 2
            * 0x02: 1
            *
            * MISC2[AUDO_DIV]:
            * 0x00: 1
            * 0x01: 2
            * 0x02: 1
            * 0x03: 4
            */
        switch (CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT_MASK) {
        case CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(0U):
            freq = freq >> 2U;
            break;
        case CCM_ANALOG_PLL_AUDIO_POST_DIV_SELECT(1U):
            freq = freq >> 1U;
            break;
        default:
            break;
        }

        switch (CCM_ANALOG->MISC2 & (CCM_ANALOG_MISC2_AUDIO_DIV_MSB_MASK | CCM_ANALOG_MISC2_AUDIO_DIV_LSB_MASK)) {
        case CCM_ANALOG_MISC2_AUDIO_DIV_MSB(1) | CCM_ANALOG_MISC2_AUDIO_DIV_LSB(1):
            freq >>= 2U;
            break;
        case CCM_ANALOG_MISC2_AUDIO_DIV_MSB(0) | CCM_ANALOG_MISC2_AUDIO_DIV_LSB(1):
            freq >>= 1U;
            break;
        default:
            break;
        }
        break;

    case kCLOCK_PllVideo:
        /* PLL output frequency = Fref * (DIV_SELECT + NUM/DENOM). */
        divSelect =
            (CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_VIDEO_DIV_SELECT_SHIFT;

        freqTmp = ((clock_64b_t)freq * ((clock_64b_t)(CCM_ANALOG->PLL_VIDEO_NUM))) /
                    ((clock_64b_t)(CCM_ANALOG->PLL_VIDEO_DENOM));

        freq = freq * divSelect + (uint32_t)freqTmp;

        /* VIDEO PLL output = PLL output frequency / POSTDIV. */

        /*
            * Post divider:
            *
            * PLL_VIDEO[POST_DIV_SELECT]:
            * 0x00: 4
            * 0x01: 2
            * 0x02: 1
            *
            * MISC2[VIDEO_DIV]:
            * 0x00: 1
            * 0x01: 2
            * 0x02: 1
            * 0x03: 4
            */
        switch (CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT_MASK) {
        case CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(0U):
            freq = freq >> 2U;
            break;
        case CCM_ANALOG_PLL_VIDEO_POST_DIV_SELECT(1U):
            freq = freq >> 1U;
            break;
        default:
            break;
        }

        switch (CCM_ANALOG->MISC2 & CCM_ANALOG_MISC2_VIDEO_DIV_MASK) {
        case CCM_ANALOG_MISC2_VIDEO_DIV(3):
            freq >>= 2U;
            break;
        case CCM_ANALOG_MISC2_VIDEO_DIV(1):
            freq >>= 1U;
            break;
        default:
            break;
        }
        break;
    case kCLOCK_PllEnet:
        divSelect =
            (CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_DIV_SELECT_MASK) >> CCM_ANALOG_PLL_ENET_DIV_SELECT_SHIFT;
        freq = enetRefClkFreq[divSelect];
        break;
    case kCLOCK_PllEnet25M:
        /* ref_enetpll1 if fixed at 25MHz. */
        freq = 25000000UL;
        break;
    case kCLOCK_PllUsb2:
        freq = (freq * ((CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_DIV_SELECT_MASK) ? 22U : 20U));
        break;
    default:
        freq = 0U;
        break;
    }

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_initsyspfd
 *
 * Description:
 *   brief Initialize the System PLL PFD.
 *
 *   This function initializes the System PLL PFD. During new value setting,
 *   the clock output is disabled to prevent glitch.
 *
 * Input Parameters:
 *   pfd Which PFD clock to enable.
 *   pfdFrac The PFD FRAC value.
 *   note It is recommended that PFD settings are kept between 12-35.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initsyspfd(clock_pfd_t pfd, uint8_t pfdFrac)
{
    uint32_t pfdIndex = (uint32_t)pfd;
    uint32_t pfd528;

    pfd528 = CCM_ANALOG->PFD_528 &
             ~((CCM_ANALOG_PFD_528_PFD0_CLKGATE_MASK | CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) << (8 * pfdIndex));

    /* Disable the clock output first. */
    CCM_ANALOG->PFD_528 = pfd528 | (CCM_ANALOG_PFD_528_PFD0_CLKGATE_MASK << (8 * pfdIndex));

    /* Set the new value and enable output. */
    CCM_ANALOG->PFD_528 = pfd528 | (CCM_ANALOG_PFD_528_PFD0_FRAC(pfdFrac) << (8 * pfdIndex));
}

/****************************************************************************
 * Function: imxrt_clock_deinitsyspfd
 *
 * Description:
 *   brief De-initialize the System PLL PFD.
 *
 *   This function disables the System PLL PFD.
 *
 * Input Parameters:
 *   pfd Which PFD clock to disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitsyspfd(clock_pfd_t pfd)
{
    CCM_ANALOG->PFD_528 |= CCM_ANALOG_PFD_528_PFD0_CLKGATE_MASK << (8 * pfd);
}

/****************************************************************************
 * Function: imxrt_clock_initusb1pfd
 *
 * Description:
 *   brief Initialize the USB1 PLL PFD.
 *
 *   This function initializes the USB1 PLL PFD. During new value setting,
 *   the clock output is disabled to prevent glitch.
 *
 * Input Parameters:
 *   pfd Which PFD clock to enable.
 *   pfdFrac The PFD FRAC value.
 *   note It is recommended that PFD settings are kept between 12-35.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_initusb1pfd(clock_pfd_t pfd, uint8_t pfdFrac)
{
    uint32_t pfdIndex = (uint32_t)pfd;
    uint32_t pfd480;

    pfd480 = CCM_ANALOG->PFD_480 &
             ~((CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK | CCM_ANALOG_PFD_480_PFD0_FRAC_MASK) << (8 * pfdIndex));

    /* Disable the clock output first. */
    CCM_ANALOG->PFD_480 = pfd480 | (CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK << (8 * pfdIndex));

    /* Set the new value and enable output. */
    CCM_ANALOG->PFD_480 = pfd480 | (CCM_ANALOG_PFD_480_PFD0_FRAC(pfdFrac) << (8 * pfdIndex));
}

/****************************************************************************
 * Function: imxrt_clock_deinitusb1pfd
 *
 * Description:
 *   brief De-initialize the USB1 PLL PFD.
 *
 *   This function disables the USB1 PLL PFD.
 *
 * Input Parameters:
 *   pfd Which PFD clock to disable.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_deinitusb1pfd(clock_pfd_t pfd)
{
    CCM_ANALOG->PFD_480 |= CCM_ANALOG_PFD_480_PFD0_CLKGATE_MASK << (8 * pfd);
}

/****************************************************************************
 * Function: imxrt_clock_deinitusb1pfd
 *
 * Description:
 *   brief Get current System PLL PFD output frequency.
 *
 *   This function get current output frequency of specific System PLL PFD
 *
 * Input Parameters:
 *   pfd   pfd name to get frequency.
 *
 * Returned Value:
 *   Returns The PFD output frequency in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getsyspfdfreq(clock_pfd_t pfd)
{
    uint32_t freq = imxrt_clock_getpllfreq(kCLOCK_PllSys);

    switch (pfd) {
    case kCLOCK_Pfd0:
        freq /= ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD0_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd1:
        freq /= ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD1_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD1_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd2:
        freq /= ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD2_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd3:
        freq /= ((CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD3_FRAC_MASK) >> CCM_ANALOG_PFD_528_PFD3_FRAC_SHIFT);
        break;
    default:
        freq = 0U;
        break;
    }
    freq *= 18U;

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_getusb1pfdfreq
 *
 * Description:
 *   brief Get current USB1 PLL PFD output frequency.
 *
 *   This function get current output frequency of specific USB1 PLL PFD
 *
 * Input Parameters:
 *   pfd   pfd name to get frequency.
 *
 * Returned Value:
 *   Returns The PFD output frequency in hertz.
 *
 ****************************************************************************/
uint32_t imxrt_clock_getusb1pfdfreq(clock_pfd_t pfd)
{
    uint32_t freq = imxrt_clock_getpllfreq(kCLOCK_PllUsb1);

    switch (pfd) {
    case kCLOCK_Pfd0:
        freq /= ((CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD0_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD0_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd1:
        freq /= ((CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD1_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD1_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd2:
        freq /= ((CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD2_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD2_FRAC_SHIFT);
        break;
    case kCLOCK_Pfd3:
        freq /= ((CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD3_FRAC_MASK) >> CCM_ANALOG_PFD_480_PFD3_FRAC_SHIFT);
        break;
    default:
        freq = 0U;
        break;
    }
    freq *= 18U;

    return freq;
}

/****************************************************************************
 * Function: imxrt_clock_enableusbhs1phypllclock
 *
 * Description:
 *   brief Enable USB HS PHY PLL clock.
 *
 *   This function enables the internal 480MHz USB PHY PLL clock.
 *
 * Input Parameters:
 *   src  USB HS PHY PLL clock source.
 *   freq The frequency specified by src.
 *
 * Returned Value:
 *   Returns true The clock is set successfully.
 *   Returns false The clock source is invalid to get proper USB HS clock.
 *
 ****************************************************************************/
bool imxrt_clock_enableusbhs1phypllclock(clock_usb_phy_src_t src, uint32_t freq)
{
    const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};
    imxrt_clock_initusb2pll(&g_ccmConfigUsbPll);
    USBPHY2->CTRL &= ~USBPHY_CTRL_SFTRST_MASK; /* release PHY from reset */
    USBPHY2->CTRL &= ~USBPHY_CTRL_CLKGATE_MASK;

    USBPHY2->PWD = 0;
    USBPHY2->CTRL |= USBPHY_CTRL_ENAUTOCLR_PHY_PWD_MASK | USBPHY_CTRL_ENAUTOCLR_CLKGATE_MASK |
                     USBPHY_CTRL_ENUTMILEVEL2_MASK | USBPHY_CTRL_ENUTMILEVEL3_MASK;

    return true;
}

/****************************************************************************
 * Function: imxrt_clock_disableusbhs1phypllclock
 *
 * Description:
 *   brief Disable USB HS PHY PLL clock.
 *
 *   This function disables USB HS PHY PLL clock.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void imxrt_clock_disableusbhs1phypllclock(void)
{
    CCM_ANALOG->PLL_USB2 &= ~CCM_ANALOG_PLL_USB2_EN_USB_CLKS_MASK;
    USBPHY2->CTRL |= USBPHY_CTRL_CLKGATE_MASK; /* Set to 1U to gate clocks */
}
