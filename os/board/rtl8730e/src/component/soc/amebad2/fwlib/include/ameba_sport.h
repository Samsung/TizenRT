/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AMEBA_SPORT_H_
#define _AMEBA_SPORT_H_

/** @addtogroup AUDIO_SPORT
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * audio sport:
  * 	- Base Address: AUDIO_SPORT0_DEV,AUDIO_SPORT1_DEV,AUDIO_SPORT2_DEV,AUDIO_SPORT3_DEV,
  * 	- Source clk: 40MHz
  * 	- Sample rate: 8/16/32/44.1/48/88.2/96 /11.025/12/22.05 KHz
  * 	- Sample bit: 16 bit, 20 bit,24 bit, 32bit
  * 	- Channel number: mono or stereo
  * 	- Data format: I2S, Left justified, PCM mode A, PCM mode B
  * 	- Use GDMA to move data
  *
  *****************************************************************************************
  * How to use audio sport
  *****************************************************************************************
  *      To use audio codec sport, the following steps are mandatory:
  *
  *      1. Open audio codec clock and function using
  *			RCC_PeriphClockCmd(APBPeriph_AUDIOC, APBPeriph_AUDIOC_CLOCK, ENABLE);
  *			RCC_PeriphClockCmd(APBPeriph_SPORT, APBPeriph_SPORT_CLOCK, ENABLE);
  *
  *      2. Fill the AUDIO_SP_InitStruct with the desired parameters.
  *
  *      3. configure AUDIO SPORT with the corresponding configuration.
  *			AUDIO_SP_Init(AUDIO_SP_DEV, &SP_InitStruct)
  *
  *      4. According to audio codec transfer direction, start Tx or Rx or both path

  *      5. Use AUDIO_SP_TXGDMA_Init or AUDIO_SP_RXGDMA_Init or both function to activate the GDMA according to transfer direction.
  *
  *      @note    All other functions can be used separately to modify, if needed,
  *			a specific feature of the AUDIO SPORT.
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup AUDIO
  * @{
  */

/** @defgroup AUDIO_SPORT
* @brief AUDIO_SPORT driver modules
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup AUDIO_SPORT_Register_Definitions AUDIO_SPORT Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup SP_REG_MUX
 * @brief SPORT MUX Register
 * @{
 **/
#define SP_MASK_REG_MUX                ((u32)0xFFFFFFFF << 0)          /*!<R/W 0xFFFFFFFF  Mux of register write with different base address of the same SPORT. "sp_reg_mux" can be set as different value with four different base address in one SPORT, but other registers share the same value with four different base address in one SPORT. */
#define SP_REG_MUX(x)                  (((u32)((x) & 0xFFFFFFFF) << 0))
#define SP_GET_REG_MUX(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SP_CTRL0
 * @brief SPORT Control Register 0
 * @{
 **/
#define SP_MASK_MCLK_SEL               ((u32)0x00000003 << 30)          /*!<R/W 0x0  2’b00: MCLK output=dsp_src_clk/4 2’b01: MCLK output=dsp_src_clk/2 2’b10/2’b11: MCLK output=dsp_src_clk */
#define SP_MCLK_SEL(x)                 (((u32)((x) & 0x00000003) << 30))
#define SP_GET_MCLK_SEL(x)             ((u32)(((x >> 30) & 0x00000003)))
#define SP_MASK_SEL_I2S_RX_CH          ((u32)0x00000003 << 28)          /*!<R/W 0x0  2’b00: L/R 2’b01: R/L 2’b10: L/L 2’b11: R/R x ADC path */
#define SP_SEL_I2S_RX_CH(x)            (((u32)((x) & 0x00000003) << 28))
#define SP_GET_SEL_I2S_RX_CH(x)        ((u32)(((x >> 28) & 0x00000003)))
#define SP_MASK_SEL_I2S_TX_CH          ((u32)0x00000003 << 26)          /*!<R/W 0x0  2’b00: L/R 2’b01: R/L 2’b10: L/L 2’b11: R/R x DAC path */
#define SP_SEL_I2S_TX_CH(x)            (((u32)((x) & 0x00000003) << 26))
#define SP_GET_SEL_I2S_TX_CH(x)        ((u32)(((x >> 26) & 0x00000003)))
#define SP_BIT_START_RX                ((u32)0x00000001 << 25)          /*!<R/W 0  1’b0: RX is disabled 1’b1: RX is started */
#define SP_BIT_RX_DISABLE              ((u32)0x00000001 << 24)          /*!<R/W 1  1’b1: SPORT RX is disabled. 1’b0: SPORT RX is enabled. */
#define SP_BIT_RX_LSB_FIRST_0          ((u32)0x00000001 << 23)          /*!<R/W 0  1’b0: MSB first when RX 1’b1: LSB first */
#define SP_BIT_TX_LSB_FIRST_0          ((u32)0x00000001 << 22)          /*!<R/W 0  1’b0: MSB first when TX 1’b1: LSB first */
#define SP_MASK_TDM_MODE_SEL_RX        ((u32)0x00000003 << 20)          /*!<R/W 0x0  2’b00: Without TDM 2’b01: TDM4 2’b10: TDM6 2’b11: TDM8 */
#define SP_TDM_MODE_SEL_RX(x)          (((u32)((x) & 0x00000003) << 20))
#define SP_GET_TDM_MODE_SEL_RX(x)      ((u32)(((x >> 20) & 0x00000003)))
#define SP_MASK_TDM_MODE_SEL_TX        ((u32)0x00000003 << 18)          /*!<R/W 0x0  2’b00: Without TDM 2’b01: TDM4 2’b10: TDM6 2’b11: TDM8 */
#define SP_TDM_MODE_SEL_TX(x)          (((u32)((x) & 0x00000003) << 18))
#define SP_GET_TDM_MODE_SEL_TX(x)      ((u32)(((x >> 18) & 0x00000003)))
#define SP_BIT_START_TX                ((u32)0x00000001 << 17)          /*!<R/W 0  1’b0: TX is disabled. 1’b1: TX is started. */
#define SP_BIT_TX_DISABLE              ((u32)0x00000001 << 16)          /*!<R/W 1  1’b1: SPORT TX is disabled. 1’b0: SPORT TX is enabled. */
#define SP_BIT_I2S_SELF_LPBK_EN        ((u32)0x00000001 << 15)          /*!<R/W 0  1’b1: internal loopback mode is enabled */
#define SP_MASK_DATA_LEN_SEL_TX_0      ((u32)0x00000007 << 12)          /*!<R/W 0x0  3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_DATA_LEN_SEL_TX_0(x)        (((u32)((x) & 0x00000007) << 12))
#define SP_GET_DATA_LEN_SEL_TX_0(x)    ((u32)(((x >> 12) & 0x00000007)))
#define SP_BIT_EN_I2S_MONO_TX_0        ((u32)0x00000001 << 11)          /*!<R/W 0  1’b1: mono 1’b0: stereo */
#define SP_BIT_INV_I2S_SCLK            ((u32)0x00000001 << 10)          /*!<R/W 0  1’b1: I2S/PCM bit clock is inverted */
#define SP_MASK_DATA_FORMAT_SEL_TX     ((u32)0x00000003 << 8)          /*!<R/W 0x0  2’b00: I2S 2’b01: Left Justified 2’b10: PCM mode A 2’b11: PCM mode B */
#define SP_DATA_FORMAT_SEL_TX(x)       (((u32)((x) & 0x00000003) << 8))
#define SP_GET_DATA_FORMAT_SEL_TX(x)   ((u32)(((x >> 8) & 0x00000003)))
#define SP_BIT_DSP_CTL_MODE            ((u32)0x00000001 << 7)          /*!<R/W 0  1’b1: DSP and SPORT1 handshaking is enabled. 1’b0: GDMA and SPORT1 handshaking is enabled. */
#define SP_BIT_LOOPBACK                ((u32)0x00000001 << 6)          /*!<R/W 0  1’b1: self loopback mode */
#define SP_BIT_WCLK_TX_INVERSE         ((u32)0x00000001 << 5)          /*!<R/W 0  1’b1: I2S/PCM word clock is inverted for TX (SPK path) */
#define SP_BIT_SLAVE_DATA_SEL          ((u32)0x00000001 << 4)          /*!<R/W 0  1’b1: To be an I2S or PCM slave (data path) */
#define SP_BIT_SLAVE_CLK_SEL           ((u32)0x00000001 << 3)          /*!<R/W 0  1’b1: To be an I2S or PCM slave (CLK path) */
#define SP_BIT_RX_INV_I2S_SCLK         ((u32)0x00000001 << 2)          /*!<R/W 0  1'b1: sclk to RX path (ADC path) is inverted */
#define SP_BIT_TX_INV_I2S_SCLK         ((u32)0x00000001 << 1)          /*!<R/W 0  1'b1: sclk to TX path (DAC path) is inverted */
#define SP_BIT_RESET                   ((u32)0x00000001 << 0)          /*!<R/W 0  1’b1: reset SPORT1 module, and remember to write “1” to reset and then write “0” to release from reset. */
/** @} */

/** @defgroup SP_CTRL1
 * @brief SPORT Control Register 1
 * @{
 **/
#define SP_BIT_RX_FIFO_1_REG_1_EN      ((u32)0x00000001 << 31)          /*!<R/W 0  1’b1: Enable last two channel of RX_FIFO_1. Only enable when "rx_fifo_1_reg_0_en" = 1. */
#define SP_BIT_RX_FIFO_1_REG_0_EN      ((u32)0x00000001 << 30)          /*!<R/W 0  1’b1: Enable first two channel of RX_FIFO_1. Only enable when "rx_fifo_0_reg_0_en" = 1. */
#define SP_BIT_RX_FIFO_0_REG_1_EN      ((u32)0x00000001 << 29)          /*!<R/W 0  1’b1: Enable last two channel of RX_FIFO_0. Only enable when "rx_fifo_0_reg_0_en" = 1. */
#define SP_BIT_RX_FIFO_0_REG_0_EN      ((u32)0x00000001 << 28)          /*!<R/W 1  1’b1: Enable first two channel of RX_FIFO_0. Disable 0x0008[28] ~ Disable 0x0008[31] at the same time to reset RX FIFO. */
#define SP_BIT_TX_FIFO_1_REG_1_EN      ((u32)0x00000001 << 27)          /*!<R/W 0  1’b1: Enable last two channel of TX_FIFO_1. Only enable when "tx_fifo_1_reg_0_en" = 1. */
#define SP_BIT_TX_FIFO_1_REG_0_EN      ((u32)0x00000001 << 26)          /*!<R/W 0  1’b1: Enable first two channel of TX_FIFO_1. Only enable when "tx_fifo_0_reg_0_en" = 1. */
#define SP_BIT_TX_FIFO_0_REG_1_EN      ((u32)0x00000001 << 25)          /*!<R/W 0  1’b1: Enable last two channel of TX_FIFO_0. Only enable when "tx_fifo_0_reg_0_en" = 1. */
#define SP_BIT_TX_FIFO_0_REG_0_EN      ((u32)0x00000001 << 24)          /*!<R/W 1  1’b1: Enable first two channel of TX_FIFO_0. Disable 0x0008[24] ~ Disable 0x0008[27] at the same time to reset TX FIFO. */
#define SP_BIT_RX_SNK_LR_SWAP_0        ((u32)0x00000001 << 23)          /*!<R/W 0  1’b1: swap L/R audio samples written to the sink memory of RX_FIFO_0; */
#define SP_BIT_RX_SNK_BYTE_SWAP_0      ((u32)0x00000001 << 22)          /*!<R/W 0  1’b1: swap H/L bytes written to the sink memory of RX_FIFO_0; */
#define SP_BIT_TX_SRC_LR_SWAP_0        ((u32)0x00000001 << 21)          /*!<R/W 0  1’b1: swap L/R audio samples read from the source memory of TX_FIFO_0; */
#define SP_BIT_TX_SRC_BYTE_SWAP_0      ((u32)0x00000001 << 20)          /*!<R/W 0  1’b1: swap H/L bytes read from the source memory of TX_FIFO_0; */
#define SP_BIT_DIRECT_MODE_EN          ((u32)0x00000001 << 19)          /*!<R/W 0  1’b1: WS(LRCK) and SCK(BCLK) are from other sport */
#define SP_MASK_DIRECT_SRC_SEL         ((u32)0x00000003 << 17)          /*!<R/W 0x0  2’b00: ws and sck are from sport0. 2’b01: ws and sck are from sport1. 2’b10: ws and sck are from sport2. 2’b11: ws and sck are from sport3. */
#define SP_DIRECT_SRC_SEL(x)           (((u32)((x) & 0x00000003) << 17))
#define SP_GET_DIRECT_SRC_SEL(x)       ((u32)(((x >> 17) & 0x00000003)))
#define SP_BIT_ERR_CNT_SAT_SET         ((u32)0x00000001 << 16)          /*!<R/W 0  1’b1: saturation count (65534 --> 65535 --> 65535 ...) 1’b0: wrap count (65534 --> 65535 --> 0 --> 1 --> 2 ...) */
#define SP_MASK_SPORT_CLK_SEL          ((u32)0x00000003 << 14)          /*!<R/W 0x0  2’b0x00: dsp_src_clk(BCLK*2) 2’b10: dsp_src_clk(BCLK*4)/2 2’b11: dsp_src_clk (BCLK*8)/4 */
#define SP_SPORT_CLK_SEL(x)            (((u32)((x) & 0x00000003) << 14))
#define SP_GET_SPORT_CLK_SEL(x)        ((u32)(((x >> 14) & 0x00000003)))
#define SP_BIT_CLEAR_RX_ERR_CNT        ((u32)0x00000001 << 13)          /*!<R/W 0  Write 1'b1 and then write 0 to clear RX error counter */
#define SP_BIT_CLEAR_TX_ERR_CNT        ((u32)0x00000001 << 12)          /*!<R/W 0  Write 1'b1 and then write 0 to clear TX error counter */
#define SP_BIT_ENABLE_MCLK             ((u32)0x00000001 << 11)          /*!<R/W 0  Enable mclk. */
#define SP_MASK_DEBUG_BUS_SEL          ((u32)0x00000007 << 8)          /*!<R/W 0x0  3’b000: debug_bus_a 3’b001: debug_bus_b … 3’b111: debug_bus_h */
#define SP_DEBUG_BUS_SEL(x)            (((u32)((x) & 0x00000007) << 8))
#define SP_GET_DEBUG_BUS_SEL(x)        ((u32)(((x >> 8) & 0x00000007)))
#define SP_BIT_WS_FORCE_VAL            ((u32)0x00000001 << 7)          /*!<R/W 1  When "ws_force" = 1, ws_out_tx and ws_out_rx = "ws_force_val." */
#define SP_BIT_WS_FORCE                ((u32)0x00000001 << 6)          /*!<R/W 0  1'b1: Make ws_out_tx and ws_out_rx = "ws_force_val." */
#define SP_BIT_BCLK_RESET              ((u32)0x00000001 << 5)          /*!<R/W 0  1’b0: Enable bclk 1’b1: Disable and reset bclk */
#define SP_BIT_BCLK_PULL_ZERO          ((u32)0x00000001 << 4)          /*!<R/W 0  Write 1’b1 to pull bclk to 0 smoothly. Write 1’b0 to reopen bclk. */
#define SP_BIT_MULTI_IO_EN_RX          ((u32)0x00000001 << 3)          /*!<R/W 1  1’b1: Enable multi-IO of rx 1’b0: Disable multi-IO of rx; SPORT2 & SPORT3 only. */
#define SP_BIT_MULTI_IO_EN_TX          ((u32)0x00000001 << 2)          /*!<R/W 1  1’b1: Enable multi-IO of tx 1’b0: Disable multi-IO of tx; SPORT2 & SPORT3 only. */
#define SP_BIT_TX_FIFO_FILL_ZERO       ((u32)0x00000001 << 1)          /*!<WA0 0  X is the burst size of TX_FIFO_0. Y is the burst size of TX_FIFO_1. Fill TX_FIFO_0 with X zero data and fill TX_FIFO_1 with Y zero data. This control bit is "write 1 clear" type */
#define SP_BIT_RESET_SMOOTH            ((u32)0x00000001 << 0)          /*!<R/W 0  1’b1: reset SPORT1 module with complete LRCK cycle. */
/** @} */

/** @defgroup SP_INT_CTRL
 * @brief SPORT Interrupt Control Register
 * @{
 **/
#define SP_MASK_INT_ENABLE_DSP_1       ((u32)0x000000FF << 24)          /*!<R/W 0x0  Bit[24]: for the interrupt of “sp_ready_to_tx_1” Bit[25]: for the interrupt of “sp_ready_to_rx_1” Bit[26]: for the interrupt of “tx_fifo_full_intr_1” Bit[27]: for the interrupt of “rx_fifo_full_intr_1” Bit[28]: for the interrupt of “tx_fifo_empty_intr_1” Bit[29]: for the interrupt of “rx_fifo_empty_intr_1” Bit[30]: for the interrupt of “tx_i2s_idle_1” Bit[31]: Reserved */
#define SP_INT_ENABLE_DSP_1(x)         (((u32)((x) & 0x000000FF) << 24))
#define SP_GET_INT_ENABLE_DSP_1(x)     ((u32)(((x >> 24) & 0x000000FF)))
#define SP_MASK_INT_ENABLE_DSP_0       ((u32)0x000000FF << 16)          /*!<R/W 0x0  Bit[16]: for the interrupt of “sp_ready_to_tx” Bit[17]: for the interrupt of “sp_ready_to_rx” Bit[18]: for the interrupt of “tx_fifo_full_intr” Bit[19]: for the interrupt of “rx_fifo_full_intr” Bit[20]: for the interrupt of “tx_fifo_empty_intr” Bit[21]: for the interrupt of “rx_fifo_empty_intr” Bit[22]: for the interrupt of “tx_i2s_idle” Bit[23]: Reserved */
#define SP_INT_ENABLE_DSP_0(x)         (((u32)((x) & 0x000000FF) << 16))
#define SP_GET_INT_ENABLE_DSP_0(x)     ((u32)(((x >> 16) & 0x000000FF)))
#define SP_MASK_INTR_CLR_1             ((u32)0x0000001F << 9)          /*!<R/W 0x0  Bit[9]: for the interrupt of “tx_fifo_full_intr_1” Bit[10]: for the interrupt of “rx_fifo_full_intr_1” Bit[11]: for the interrupt of “tx_fifo_empty_intr_1” Bit[12]: for the interrupt of “rx_fifo_empty_intr_1” Bit[13]: Reserved */
#define SP_INTR_CLR_1(x)               (((u32)((x) & 0x0000001F) << 9))
#define SP_GET_INTR_CLR_1(x)           ((u32)(((x >> 9) & 0x0000001F)))
#define SP_BIT_RX_DSP_CLEAR_INT_1      ((u32)0x00000001 << 8)          /*!<R/W 0  For DSP mode (bypass GDMA), F/W writes 1’b1 and then 1’b0 to clear RX interrupt. Note: RX interrupt is to indicate that DSP can get audio data from RX FIFO_1 */
#define SP_BIT_TX_DSP_CLEAR_INT_1      ((u32)0x00000001 << 7)          /*!<R/W 0  For DSP mode (bypass GDMA), F/W writes 1’b1 and then 1’b0 to clear TX interrupt. Note: TX interrupt is to indicate that DSP can write audio data to TX FIFO_1 */
#define SP_MASK_INTR_CLR_0             ((u32)0x0000001F << 2)          /*!<R/W 0x0  Bit[2]: for the interrupt of “tx_fifo_full_intr” Bit[3]: for the interrupt of “rx_fifo_full_intr” Bit[4]: for the interrupt of “tx_fifo_empty_intr” Bit[5]: for the interrupt of “rx_fifo_empty_intr” Bit[6]: Reserved */
#define SP_INTR_CLR_0(x)               (((u32)((x) & 0x0000001F) << 2))
#define SP_GET_INTR_CLR_0(x)           ((u32)(((x >> 2) & 0x0000001F)))
#define SP_BIT_RX_DSP_CLEAR_INT_0      ((u32)0x00000001 << 1)          /*!<R/W 0  For DSP mode (bypass GDMA), F/W writes 1’b1 and then 1’b0 to clear RX interrupt. Note: RX interrupt is to indicate that DSP can get audio data from RX FIFO_0 */
#define SP_BIT_TX_DSP_CLEAR_INT_0      ((u32)0x00000001 << 0)          /*!<R/W 0  For DSP mode (bypass GDMA), F/W writes 1’b1 and then 1’b0 to clear TX interrupt. Note: TX interrupt is to indicate that DSP can write audio data to TX FIFO_0 */
/** @} */

/** @defgroup SP_TRX_COUNTER_STATUS
 * @brief SPORT TRX Counter Status Register
 * @{
 **/
#define SP_BIT_RESET_STATE             ((u32)0x00000001 << 31)          /*!<R 0  1’b1: sp_reset is enable 1’b0: sp_reset is disable */
#define SP_MASK_RX_DEPTH_CNT_1         ((u32)0x0000003F << 24)          /*!<R 0x0  RX FIFO_1 depth counter status (MIC path) */
#define SP_RX_DEPTH_CNT_1(x)           (((u32)((x) & 0x0000003F) << 24))
#define SP_GET_RX_DEPTH_CNT_1(x)       ((u32)(((x >> 24) & 0x0000003F)))
#define SP_MASK_TX_DEPTH_CNT_1         ((u32)0x0000003F << 16)          /*!<R 0x0  TX FIFO_1 depth counter status (SPK path) */
#define SP_TX_DEPTH_CNT_1(x)           (((u32)((x) & 0x0000003F) << 16))
#define SP_GET_TX_DEPTH_CNT_1(x)       ((u32)(((x >> 16) & 0x0000003F)))
#define SP_MASK_RX_DEPTH_CNT_0         ((u32)0x0000003F << 8)          /*!<R 0x0  RX FIFO_0 depth counter status (MIC path) */
#define SP_RX_DEPTH_CNT_0(x)           (((u32)((x) & 0x0000003F) << 8))
#define SP_GET_RX_DEPTH_CNT_0(x)       ((u32)(((x >> 8) & 0x0000003F)))
#define SP_MASK_TX_DEPTH_CNT_0         ((u32)0x0000003F << 0)          /*!<R 0x0  TX FIFO_0 depth counter status (SPK path) */
#define SP_TX_DEPTH_CNT_0(x)           (((u32)((x) & 0x0000003F) << 0))
#define SP_GET_TX_DEPTH_CNT_0(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SP_ERR
 * @brief SPORT Error Register
 * @{
 **/
#define SP_MASK_RX_ERR_CNT             ((u32)0x0000FFFF << 16)          /*!<R 0x0  RX error counter (MIC path) Note: This counter should always be zero if everything works well. */
#define SP_RX_ERR_CNT(x)               (((u32)((x) & 0x0000FFFF) << 16))
#define SP_GET_RX_ERR_CNT(x)           ((u32)(((x >> 16) & 0x0000FFFF)))
#define SP_MASK_TX_ERR_CNT             ((u32)0x0000FFFF << 0)          /*!<R 0x0  TX error counter (SPK path) Note: This counter should always be zero if everything works well. */
#define SP_TX_ERR_CNT(x)               (((u32)((x) & 0x0000FFFF) << 0))
#define SP_GET_TX_ERR_CNT(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SP_TX_BCLK
 * @brief SPORT TX BCLK Register
 * @{
 **/
#define SP_BIT_TX_MI_NI_UPDATE         ((u32)0x00000001 << 31)          /*!<R/W 0  1’b1: to update “mi” and “ni” to get the new clock rate. This bit will be reset automatically when the update is done */
#define SP_MASK_TX_NI                  ((u32)0x00007FFF << 16)          /*!<R/W 0x30  BCLK = 40MHz*(ni/mi) For example: BCLK=3.072MHz=40MHz*(48/625) */
#define SP_TX_NI(x)                    (((u32)((x) & 0x00007FFF) << 16))
#define SP_GET_TX_NI(x)                ((u32)(((x >> 16) & 0x00007FFF)))
#define SP_MASK_TX_MI                  ((u32)0x0000FFFF << 0)          /*!<R/W 0x271   */
#define SP_TX_MI(x)                    (((u32)((x) & 0x0000FFFF) << 0))
#define SP_GET_TX_MI(x)                ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SP_TX_LRCLK
 * @brief SPORT TX LRCLK Register
 * @{
 **/
#define SP_MASK_RX_BCLK_DIV_RATIO      ((u32)0x000000FF << 24)          /*!<R/W 0x3F  RX bclk even-bit integer divider. Used in “mode_40mhz” set as 1’b1. (rx_bclk_div_ratio + 1) is the number of “sck_out” cycles within a “ws_out_rx” cycle (1/fs). Default of (rx_bclk_div_ratio + 1) is 64. Set as 64 – 1 = 63. Only odd number supported. Maximum is 255. */
#define SP_RX_BCLK_DIV_RATIO(x)        (((u32)((x) & 0x000000FF) << 24))
#define SP_GET_RX_BCLK_DIV_RATIO(x)    ((u32)(((x >> 24) & 0x000000FF)))
#define SP_MASK_TX_BCLK_DIV_RATIO      ((u32)0x000000FF << 16)          /*!<R/W 0x3F  TX bclk even-bit integer divider. Used in “mode_40mhz” set as 1’b1. (tx_bclk_div_ratio + 1) is the number of “sck_out” cycles within a “ws_out_tx” cycle (1/fs). Default of (tx_bclk_div_ratio + 1) is 64. Set as 64 – 1 = 63. Only odd number supported. Maximum is 255. */
#define SP_TX_BCLK_DIV_RATIO(x)        (((u32)((x) & 0x000000FF) << 16))
#define SP_GET_TX_BCLK_DIV_RATIO(x)    ((u32)(((x >> 16) & 0x000000FF)))
#define SP_MASK_RXDMA_BUSRTSIZE        ((u32)0x0000003F << 8)          /*!<R/W 0x10  RX DMA burst size */
#define SP_RXDMA_BUSRTSIZE(x)          (((u32)((x) & 0x0000003F) << 8))
#define SP_GET_RXDMA_BUSRTSIZE(x)      ((u32)(((x >> 8) & 0x0000003F)))
#define SP_MASK_TXDMA_BURSTSIZE        ((u32)0x0000003F << 0)          /*!<R/W 0x10  TX DMA burst size */
#define SP_TXDMA_BURSTSIZE(x)          (((u32)((x) & 0x0000003F) << 0))
#define SP_GET_TXDMA_BURSTSIZE(x)      ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SP_FIFO_CTRL
 * @brief SPORT FIFO Control Register
 * @{
 **/
#define SP_BIT_RX_FIFO_EMPTY_0         ((u32)0x00000001 << 31)          /*!<R 1  1: RX FIFO_0 is empty */
#define SP_BIT_TX_FIFO_EMPTY_0         ((u32)0x00000001 << 30)          /*!<R 1  1: TX FIFO_0 is empty */
#define SP_BIT_RX_FIFO_FULL_0          ((u32)0x00000001 << 29)          /*!<R 0  1: RX FIFO_0 is full */
#define SP_BIT_TX_FIFO_FULL_0          ((u32)0x00000001 << 28)          /*!<R 0  1: TX FIFO_0 is full */
#define SP_BIT_RX_FIFO_EMPTY_1         ((u32)0x00000001 << 27)          /*!<R 1  1: RX FIFO_1 is empty */
#define SP_BIT_TX_FIFO_EMPTY_1         ((u32)0x00000001 << 26)          /*!<R 1  1: TX FIFO_1 is empty */
#define SP_BIT_RX_FIFO_FULL_1          ((u32)0x00000001 << 25)          /*!<R 0  1: RX FIFO_1 is full */
#define SP_BIT_TX_FIFO_FULL_1          ((u32)0x00000001 << 24)          /*!<R 0  1: TX FIFO_1 is full */
#define SP_BIT_TX_I2S_IDLE_1           ((u32)0x00000001 << 13)          /*!<R 0  1: TX is working but FIFO_1 is empty. */
#define SP_BIT_RX_FIFO_EMPTY_INTR_1    ((u32)0x00000001 << 12)          /*!<R 0  1: RX FIFO_1 is empty (MIC path) */
#define SP_BIT_TX_FIFO_EMPTY_INTR_1    ((u32)0x00000001 << 11)          /*!<R 0  1: TX FIFO_1 is empty (SPK path) */
#define SP_BIT_RX_FIFO_FULL_INTR_1     ((u32)0x00000001 << 10)          /*!<R 0  1: RX FIFO_1 is full (MIC path) */
#define SP_BIT_TX_FIFO_FULL_INTR_1     ((u32)0x00000001 << 9)          /*!<R 0  1: TX FIFO_1 is full (SPK path) */
#define SP_BIT_READY_TO_RX_1           ((u32)0x00000001 << 8)          /*!<R 0  1: It is ready to receive data (MIC path) */
#define SP_BIT_READY_TO_TX_1           ((u32)0x00000001 << 7)          /*!<R 0  1: It is ready to send data out (SPK path) */
#define SP_BIT_TX_I2S_IDLE_0           ((u32)0x00000001 << 6)          /*!<R 0  1: TX is working but FIFO_0 is empty. */
#define SP_BIT_RX_FIFO_EMPTY_INTR_0    ((u32)0x00000001 << 5)          /*!<R 0  1: RX FIFO_0 is empty (MIC path) */
#define SP_BIT_TX_FIFO_EMPTY_INTR_0    ((u32)0x00000001 << 4)          /*!<R 0  1: TX FIFO_0 is empty (SPK path) */
#define SP_BIT_RX_FIFO_FULL_INTR_0     ((u32)0x00000001 << 3)          /*!<R 0  1: RX FIFO_0 is full (MIC path) */
#define SP_BIT_TX_FIFO_FULL_INTR_0     ((u32)0x00000001 << 2)          /*!<R 0  1: TX FIFO_0 is full (SPK path) */
#define SP_BIT_READY_TO_RX_0           ((u32)0x00000001 << 1)          /*!<R 0  1: It is ready to receive data (MIC path) */
#define SP_BIT_READY_TO_TX_0           ((u32)0x00000001 << 0)          /*!<R 0  1: It is ready to send data out (SPK path) */
/** @} */

/** @defgroup SP_FORMAT
 * @brief SPORT Format Register
 * @{
 **/
#define SP_BIT_TRX_SAME_CH_LEN         ((u32)0x00000001 << 31)          /*!<R/W 0  1: TX (SPK path) and RX (MIC path) have the same channel length. */
#define SP_MASK_CH_LEN_SEL_RX          ((u32)0x00000007 << 28)          /*!<R/W 0x4  3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_CH_LEN_SEL_RX(x)            (((u32)((x) & 0x00000007) << 28))
#define SP_GET_CH_LEN_SEL_RX(x)        ((u32)(((x >> 28) & 0x00000007)))
#define SP_MASK_CH_LEN_SEL_TX          ((u32)0x00000007 << 24)          /*!<R/W 0x4  3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_CH_LEN_SEL_TX(x)            (((u32)((x) & 0x00000007) << 24))
#define SP_GET_CH_LEN_SEL_TX(x)        ((u32)(((x >> 24) & 0x00000007)))
#define SP_BIT_RX_IDEAL_LEN_EN         ((u32)0x00000001 << 23)          /*!<R/W 0  Function enable of rx_ideal_len. */
#define SP_MASK_RX_IDEAL_LEN           ((u32)0x00000007 << 20)          /*!<R/W 0x0  sd_in can be received 1 ~ 8 ( = rx_ideal_len + 1 ) BCLK cycle latter. */
#define SP_RX_IDEAL_LEN(x)             (((u32)((x) & 0x00000007) << 20))
#define SP_GET_RX_IDEAL_LEN(x)         ((u32)(((x >> 20) & 0x00000007)))
#define SP_BIT_TX_IDEAL_LEN_EN         ((u32)0x00000001 << 19)          /*!<R/W 0  Function enable of tx_ideal_len. PCMA SDO will be delayed 1 LRCK. */
#define SP_MASK_TX_IDEAL_LEN           ((u32)0x00000007 << 16)          /*!<R/W 0x0  sd_out can be sent 1 ~ 8 ( = tx_ideal_len + 1 ) BCLK cycle earlier. */
#define SP_TX_IDEAL_LEN(x)             (((u32)((x) & 0x00000007) << 16))
#define SP_GET_TX_IDEAL_LEN(x)         ((u32)(((x >> 16) & 0x00000007)))
#define SP_MASK_DATA_LEN_SEL_RX_0      ((u32)0x00000007 << 12)          /*!<R/W 0x0  Data length of MIC path and it is valid if “trx_same_length” == 1’b0. 3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_DATA_LEN_SEL_RX_0(x)        (((u32)((x) & 0x00000007) << 12))
#define SP_GET_DATA_LEN_SEL_RX_0(x)    ((u32)(((x >> 12) & 0x00000007)))
#define SP_BIT_EN_I2S_MONO_RX_0        ((u32)0x00000001 << 11)          /*!<R/W 0  Channel format of MIC path and it is valid if “trx_same_ch” == 1’b0. 1: mono 0: stereo */
#define SP_BIT_TRX_SAME_LRC            ((u32)0x00000001 << 10)          /*!<R/W 0  1: “ws_out_rx” is as same as “ws_out_tx” */
#define SP_MASK_DATA_FORMAT_SEL_RX     ((u32)0x00000003 << 8)          /*!<R/W 0x0  Data format of MIC path and it is valid if “trx_same_fs” == 1’b0. 2’b00: I2S 2’b01: Left Justified 2’b10: PCM mode A 2’b11: PCM mode B */
#define SP_DATA_FORMAT_SEL_RX(x)       (((u32)((x) & 0x00000003) << 8))
#define SP_GET_DATA_FORMAT_SEL_RX(x)   ((u32)(((x >> 8) & 0x00000003)))
#define SP_BIT_FIXED_BCLK              ((u32)0x00000001 << 7)          /*!<R/W 0  1: Refer to the description of “fixed_bclk_sel” 0: BCLK = dsp_clk/2 when “mode_40mhz = 0” */
#define SP_BIT_FIXED_BCLK_SEL          ((u32)0x00000001 << 6)          /*!<R/W 0  0: BCLK is fixed at dsp_src_clk/4 1: BCLK is fixed at dsp_src_clk/2 */
#define SP_BIT_WCLK_RX_INVERSE         ((u32)0x00000001 << 5)          /*!<R/W 0  1: invert the phase of “ws_out_rx” which is also called as “ADCLRC” */
#define SP_BIT_SCK_OUT_INVERSE         ((u32)0x00000001 << 3)          /*!<R/W 0  1: invert the phase of “sck_out” */
#define SP_BIT_TRX_SAME_LENGTH         ((u32)0x00000001 << 2)          /*!<R/W 1  1: TX (SPK path) and RX (MIC path) have the same data length. Both are either 16 or 24 bits */
#define SP_BIT_TRX_SAME_CH             ((u32)0x00000001 << 1)          /*!<R/W 1  1: TX (SPK path) and RX (MIC path) have the same channel setting. Both are either stereo or mono */
#define SP_BIT_TRX_SAME_FS             ((u32)0x00000001 << 0)          /*!<R/W 1  1: TX (SPK path) and RX (MIC path) have the same sampling rate */
/** @} */

/** @defgroup SP_RX_BCLK
 * @brief SPORT RX BCLK Register
 * @{
 **/
#define SP_BIT_RX_MI_NI_UPDATE         ((u32)0x00000001 << 31)          /*!<R/W 0  1’b1: to update “mi” and “ni” to get the new clock rate. This bit will be reset automatically when the update is done */
#define SP_MASK_RX_NI                  ((u32)0x00007FFF << 16)          /*!<R/W 0x30  BCLK = 40MHz*(ni/mi) For example: BCLK=3.072MHz=40MHz*(48/625) */
#define SP_RX_NI(x)                    (((u32)((x) & 0x00007FFF) << 16))
#define SP_GET_RX_NI(x)                ((u32)(((x >> 16) & 0x00007FFF)))
#define SP_MASK_RX_MI                  ((u32)0x0000FFFF << 0)          /*!<R/W 0x271   */
#define SP_RX_MI(x)                    (((u32)((x) & 0x0000FFFF) << 0))
#define SP_GET_RX_MI(x)                ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SP_RX_LRCLK
 * @brief SPORT RX LRCLK Register
 * @{
 **/
#define SP_BIT_CLR_TX_SPORT_RDY        ((u32)0x00000001 << 31)          /*!<WA0 0  0x001: clear tx_sport_interrupt signal This control bit is "write 1 clear" type For read, the read data is from clr_tx_sport_rdy */
#define SP_BIT_EN_TX_SPORT_INTERRUPT   ((u32)0x00000001 << 30)          /*!<R/W 0  Enable tx_sport_interrupt */
#define SP_BIT_EN_FS_PHASE_LATCH       ((u32)0x00000001 << 29)          /*!<WA0 0  0x001: Latch the value of tx_fs_phase_rpt, rx_fs_phase_rpt, tx_sport_sounter, rx_sport_sounter at the same time. This control bit is "write 1 clear" type */
#define SP_BIT_EN_TSFT_TRIG_LATCH      ((u32)0x00000001 << 28)          /*!<R/W 0  Enable wifi tsft trigger fs_phase_latch */
#define SP_BIT_TX_FS_PHASE_RPT_B5      ((u32)0x00000001 << 27)          /*!<R 0  tx_fs_phase_rptp[5] when using div64 fs_phase_trace_cnt */
#define SP_MASK_TX_SPORT_COMPARE_VAL   ((u32)0x07FFFFFF << 0)          /*!<R/W 0x40  X = (tx_sport_compare_val). When counter equal X. Sport will send tx_sport_interrupt to DSP. FW should take care X={32~134217727} */
#define SP_TX_SPORT_COMPARE_VAL(x)     (((u32)((x) & 0x07FFFFFF) << 0))
#define SP_GET_TX_SPORT_COMPARE_VAL(x) ((u32)(((x >> 0) & 0x07FFFFFF)))
/** @} */

/** @defgroup SP_DSP_COUNTER
 * @brief SPORT DSP Counter Register
 * @{
 **/
#define SP_MASK_TX_SPORT_COUNTER       ((u32)0x07FFFFFF << 5)          /*!<R 0x0  For DSP read instant tx sport counter value, counter down */
#define SP_TX_SPORT_COUNTER(x)         (((u32)((x) & 0x07FFFFFF) << 5))
#define SP_GET_TX_SPORT_COUNTER(x)     ((u32)(((x >> 5) & 0x07FFFFFF)))
#define SP_MASK_TX_FS_PHASE_RPT        ((u32)0x0000001F << 0)          /*!<R 0x0  Report tx phase */
#define SP_TX_FS_PHASE_RPT(x)          (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_TX_FS_PHASE_RPT(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_DIRECT_CTRL0
 * @brief SPORT Direct Control Register 0
 * @{
 **/
#define SP_MASK_TX_CH7_DATA_SEL        ((u32)0x0000000F << 28)          /*!<R/W 0x7  4’h0: tx_fifo_0_reg_0_l 4’h1: tx_fifo_0_reg_0_r 4’h2: tx_fifo_0_reg_1_l 4’h3: tx_fifo_0_reg_1_r 4’h4: tx_fifo_1_reg_0_l 4’h5: tx_fifo_1_reg_0_r 4’h6: tx_fifo_1_reg_1_l 4’h7: tx_fifo_1_reg_1_r 4’h8: direct_reg_7 */
#define SP_TX_CH7_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 28))
#define SP_GET_TX_CH7_DATA_SEL(x)      ((u32)(((x >> 28) & 0x0000000F)))
#define SP_MASK_TX_CH6_DATA_SEL        ((u32)0x0000000F << 24)          /*!<R/W 0x6  (Ibid.) 4’h8: direct_reg_6 */
#define SP_TX_CH6_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 24))
#define SP_GET_TX_CH6_DATA_SEL(x)      ((u32)(((x >> 24) & 0x0000000F)))
#define SP_MASK_TX_CH5_DATA_SEL        ((u32)0x0000000F << 20)          /*!<R/W 0x5  (Ibid.) 4’h8: direct_reg_5 */
#define SP_TX_CH5_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 20))
#define SP_GET_TX_CH5_DATA_SEL(x)      ((u32)(((x >> 20) & 0x0000000F)))
#define SP_MASK_TX_CH4_DATA_SEL        ((u32)0x0000000F << 16)          /*!<R/W 0x4  (Ibid.) 4’h8: direct_reg_4 */
#define SP_TX_CH4_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 16))
#define SP_GET_TX_CH4_DATA_SEL(x)      ((u32)(((x >> 16) & 0x0000000F)))
#define SP_MASK_TX_CH3_DATA_SEL        ((u32)0x0000000F << 12)          /*!<R/W 0x3  (Ibid.) 4’h8: direct_reg_3 */
#define SP_TX_CH3_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 12))
#define SP_GET_TX_CH3_DATA_SEL(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define SP_MASK_TX_CH2_DATA_SEL        ((u32)0x0000000F << 8)          /*!<R/W 0x2  (Ibid.) 4’h8: direct_reg_2 */
#define SP_TX_CH2_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 8))
#define SP_GET_TX_CH2_DATA_SEL(x)      ((u32)(((x >> 8) & 0x0000000F)))
#define SP_MASK_TX_CH1_DATA_SEL        ((u32)0x0000000F << 4)          /*!<R/W 0x1  (Ibid.) 4’h8: direct_reg_1 */
#define SP_TX_CH1_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 4))
#define SP_GET_TX_CH1_DATA_SEL(x)      ((u32)(((x >> 4) & 0x0000000F)))
#define SP_MASK_TX_CH0_DATA_SEL        ((u32)0x0000000F << 0)          /*!<R/W 0x0  (Ibid.) 4’h8: direct_reg_0 */
#define SP_TX_CH0_DATA_SEL(x)          (((u32)((x) & 0x0000000F) << 0))
#define SP_GET_TX_CH0_DATA_SEL(x)      ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup SP_FIFO_IRQ
 * @brief SPORT FIFO IRQ Register
 * @{
 **/
#define SP_BIT_RX_LSB_FIRST_1          ((u32)0x00000001 << 31)          /*!<R/W 0  1’b0: MSB first when TX 1’b1: LSB first */
#define SP_BIT_TX_LSB_FIRST_1          ((u32)0x00000001 << 30)          /*!<R/W 0  1’b0: MSB first when TX 1’b1: LSB first */
#define SP_BIT_RX_SNK_LR_SWAP_1        ((u32)0x00000001 << 29)          /*!<R/W 0  1’b1: swap L/R audio samples written to the sink memory of RX_FIFO_1 */
#define SP_BIT_RX_SNK_BYTE_SWAP_1      ((u32)0x00000001 << 28)          /*!<R/W 0  1’b1: swap H/L bytes written to the sink memory of RX_FIFO_1 */
#define SP_BIT_TX_SRC_LR_SWAP_1        ((u32)0x00000001 << 27)          /*!<R/W 0  1’b1: swap L/R audio samples read from the source memory of TX_FIFO_1 */
#define SP_BIT_TX_SRC_BYTE_SWAP_1      ((u32)0x00000001 << 26)          /*!<R/W 0  1’b1: swap H/L bytes read from the source memory of TX_FIFO_1 */
#define SP_MASK_INT_ENABLE_MCU_1       ((u32)0x000000FF << 8)          /*!<R/W 0x0  Bit[8]: for t[he int[errupt[ of “sp_ready_t[o_t[x” Bit[9]: for t[he int[errupt[ of “sp_ready_t[o_rx” Bit[10]: for t[he int[errupt[ of “t[x_fifo_full_int[r” Bit[11]: for t[he int[errupt[ of “rx_fifo_full_int[r” Bit[12]: for t[he int[errupt[ of “t[x_fifo_empt[y_int[r” Bit[13]: for t[he int[errupt[ of “rx_fifo_empt[y_int[r” Bit[14]: for t[he int[errupt[ of “t[x_i2s_idle” Bit[15]: Reserved */
#define SP_INT_ENABLE_MCU_1(x)         (((u32)((x) & 0x000000FF) << 8))
#define SP_GET_INT_ENABLE_MCU_1(x)     ((u32)(((x >> 8) & 0x000000FF)))
#define SP_MASK_INT_ENABLE_MCU_0       ((u32)0x000000FF << 0)          /*!<R/W 0x0  Bit[0]: for t[he int[errupt[ of “sp_ready_t[o_t[x” Bit[1]: for t[he int[errupt[ of “sp_ready_t[o_rx” Bit[2]: for t[he int[errupt[ of “t[x_fifo_full_int[r” Bit[3]: for t[he int[errupt[ of “rx_fifo_full_int[r” Bit[4]: for t[he int[errupt[ of “t[x_fifo_empt[y_int[r” Bit[5]: for t[he int[errupt[ of “rx_fifo_empt[y_int[r” Bit[6]: for t[he int[errupt[ of “t[x_i2s_idle” Bit[7]: Reserved */
#define SP_INT_ENABLE_MCU_0(x)         (((u32)((x) & 0x000000FF) << 0))
#define SP_GET_INT_ENABLE_MCU_0(x)     ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SP_DIRECT_CTRL1
 * @brief SPORT Direct Control Register 1
 * @{
 **/
#define SP_BIT_EN_I2S_MONO_RX_1        ((u32)0x00000001 << 31)          /*!<R/W 0  Channel format of MIC path and it is valid if “trx_same_ch” == 1’b0. 1’b1: mono 1’b0: stereo */
#define SP_MASK_DATA_LEN_SEL_RX_1      ((u32)0x00000007 << 28)          /*!<R/W 0x0  Data length of MIC path and it is valid if “trx_same_length” == 1’b0. 3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_DATA_LEN_SEL_RX_1(x)        (((u32)((x) & 0x00000007) << 28))
#define SP_GET_DATA_LEN_SEL_RX_1(x)    ((u32)(((x >> 28) & 0x00000007)))
#define SP_BIT_EN_I2S_MONO_TX_1        ((u32)0x00000001 << 27)          /*!<R/W 0  1’b1: mono 1’b0: stereo */
#define SP_MASK_DATA_LEN_SEL_TX_1      ((u32)0x00000007 << 24)          /*!<R/W 0x0  3’b000: 16 bits 3’b001: 20 bits 3’b010: 24 bits 3’b100: 32 bits */
#define SP_DATA_LEN_SEL_TX_1(x)        (((u32)((x) & 0x00000007) << 24))
#define SP_GET_DATA_LEN_SEL_TX_1(x)    ((u32)(((x >> 24) & 0x00000007)))
#define SP_BIT_DIRECT_REG_3_EN         ((u32)0x00000001 << 23)          /*!<R/W 0  1’b1: Enable direct_reg_3. */
#define SP_MASK_DIRECT_REG_3_SEL       ((u32)0x0000001F << 18)          /*!<R/W 0x0  5’h0: spa_direct_in_0 5’h1: spa_direct_in_1 5’h2: spa_direct_in_2 5’h3: spa_direct_in_3 5’h4: spa_direct_in_4 5’h5: spa_direct_in_5 5’h6: spa_direct_in_6 5’h7: spa_direct_in_7 5’h8: spb_direct_in_0 5’h9: spb_direct_in_1 5’ha: spb_direct_in_2 5’hb: spb_direct_in_3 5’hc: spb_direct_in_4 5’hd: spb_direct_in_5 5’he: spb_direct_in_6 5’hf: spb_direct_in_7 5’h10: spc_direct_in_0 5’h11: spc_direct_in_1 5’h12: spc_direct_in_2 5’h13: spc_direct_in_3 5’h14: spc_direct_in_4 5’h15: spc_direct_in_5 5’h16: spc_direct_in_6 5’h17: spc_direct_in_7 5’h18: sp0_direct_in_tx_fifo_0_reg_0_l 5’h19: sp0_direct_in_tx_fifo_0_reg_0_r 5’h1a: sp0_direct_in_tx_fifo_0_reg_1_l 5’h1b: sp0_direct_in_tx_fifo_0_reg_1_r 5’h1c: TDM_RX_CH3 SPORT0: a = 1, b = 2, c = 3 SPORT1: a = 0, b = 2, c = 3 SPORT2: a = 0, b = 1, c = 3 SPORT3: a = 0, b = 1, c = 2 */
#define SP_DIRECT_REG_3_SEL(x)         (((u32)((x) & 0x0000001F) << 18))
#define SP_GET_DIRECT_REG_3_SEL(x)     ((u32)(((x >> 18) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_2_EN         ((u32)0x00000001 << 17)          /*!<R/W 0  1’b1: Enable direct_reg_2. */
#define SP_MASK_DIRECT_REG_2_SEL       ((u32)0x0000001F << 12)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH2 */
#define SP_DIRECT_REG_2_SEL(x)         (((u32)((x) & 0x0000001F) << 12))
#define SP_GET_DIRECT_REG_2_SEL(x)     ((u32)(((x >> 12) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_1_EN         ((u32)0x00000001 << 11)          /*!<R/W 0  1’b1: Enable direct_reg_1. */
#define SP_MASK_DIRECT_REG_1_SEL       ((u32)0x0000001F << 6)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH1 */
#define SP_DIRECT_REG_1_SEL(x)         (((u32)((x) & 0x0000001F) << 6))
#define SP_GET_DIRECT_REG_1_SEL(x)     ((u32)(((x >> 6) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_0_EN         ((u32)0x00000001 << 5)          /*!<R/W 0  1’b1: Enable direct_reg_0. */
#define SP_MASK_DIRECT_REG_0_SEL       ((u32)0x0000001F << 0)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH0 */
#define SP_DIRECT_REG_0_SEL(x)         (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_DIRECT_REG_0_SEL(x)     ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_DIRECT_CTRL2
 * @brief SPORT Direct Control Register 2
 * @{
 **/
#define SP_BIT_DIRECT_OUT_7_EN         ((u32)0x00000001 << 31)          /*!<R/W 0  Enable sp_direct_out_7. */
#define SP_BIT_DIRECT_OUT_6_EN         ((u32)0x00000001 << 30)          /*!<R/W 0  Enable sp_direct_out_6. */
#define SP_BIT_DIRECT_OUT_5_EN         ((u32)0x00000001 << 29)          /*!<R/W 0  Enable sp_direct_out_5. */
#define SP_BIT_DIRECT_OUT_4_EN         ((u32)0x00000001 << 28)          /*!<R/W 0  Enable sp_direct_out_4. */
#define SP_BIT_DIRECT_OUT_3_EN         ((u32)0x00000001 << 27)          /*!<R/W 0  Enable sp_direct_out_3. */
#define SP_BIT_DIRECT_OUT_2_EN         ((u32)0x00000001 << 26)          /*!<R/W 0  Enable sp_direct_out_2. */
#define SP_BIT_DIRECT_OUT_1_EN         ((u32)0x00000001 << 25)          /*!<R/W 0  Enable sp_direct_out_1. */
#define SP_BIT_DIRECT_OUT_0_EN         ((u32)0x00000001 << 24)          /*!<R/W 0  Enable sp_direct_out_0. */
#define SP_BIT_DIRECT_REG_7_EN         ((u32)0x00000001 << 23)          /*!<R/W 0  1’b1: Enable direct_reg_7. */
#define SP_MASK_DIRECT_REG_7_SEL       ((u32)0x0000001F << 18)          /*!<R/W 0x0  5’h0: spa_direct_in_0 5’h1: spa_direct_in_1 5’h2: spa_direct_in_2 5’h3: spa_direct_in_3 5’h4: spa_direct_in_4 5’h5: spa_direct_in_5 5’h6: spa_direct_in_6 5’h7: spa_direct_in_7 5’h8: spb_direct_in_0 5’h9: spb_direct_in_1 5’ha: spb_direct_in_2 5’hb: spb_direct_in_3 5’hc: spb_direct_in_4 5’hd: spb_direct_in_5 5’he: spb_direct_in_6 5’hf: spb_direct_in_7 5’h10: spc_direct_in_0 5’h11: spc_direct_in_1 5’h12: spc_direct_in_2 5’h13: spc_direct_in_3 5’h14: spc_direct_in_4 5’h15: spc_direct_in_5 5’h16: spc_direct_in_6 5’h17: spc_direct_in_7 5’h18: sp0_direct_in_tx_fifo_0_reg_0_l 5’h19: sp0_direct_in_tx_fifo_0_reg_0_r 5’h1a: sp0_direct_in_tx_fifo_0_reg_1_l 5’h1b: sp0_direct_in_tx_fifo_0_reg_1_r 5’h1c: TDM_RX_CH7 SPORT0: a = 1, b = 2, c = 3 SPORT1: a = 0, b = 2, c = 3 SPORT2: a = 0, b = 1, c = 3 SPORT3: a = 0, b = 1, c = 2 */
#define SP_DIRECT_REG_7_SEL(x)         (((u32)((x) & 0x0000001F) << 18))
#define SP_GET_DIRECT_REG_7_SEL(x)     ((u32)(((x >> 18) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_6_EN         ((u32)0x00000001 << 17)          /*!<R/W 0  1’b1: Enable direct_reg_6. */
#define SP_MASK_DIRECT_REG_6_SEL       ((u32)0x0000001F << 12)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH6 */
#define SP_DIRECT_REG_6_SEL(x)         (((u32)((x) & 0x0000001F) << 12))
#define SP_GET_DIRECT_REG_6_SEL(x)     ((u32)(((x >> 12) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_5_EN         ((u32)0x00000001 << 11)          /*!<R/W 0  1’b1: Enable direct_reg_5. */
#define SP_MASK_DIRECT_REG_5_SEL       ((u32)0x0000001F << 6)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH5 */
#define SP_DIRECT_REG_5_SEL(x)         (((u32)((x) & 0x0000001F) << 6))
#define SP_GET_DIRECT_REG_5_SEL(x)     ((u32)(((x >> 6) & 0x0000001F)))
#define SP_BIT_DIRECT_REG_4_EN         ((u32)0x00000001 << 5)          /*!<R/W 0  1’b1: Enable direct_reg_4. */
#define SP_MASK_DIRECT_REG_4_SEL       ((u32)0x0000001F << 0)          /*!<R/W 0x0  (Ibid.) 5’h1c: TDM_RX_CH4 */
#define SP_DIRECT_REG_4_SEL(x)         (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_DIRECT_REG_4_SEL(x)     ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_DIRECT_CTRL3
 * @brief SPORT Direct Control Register 3
 * @{
 **/
#define SP_MASK_RX_FIFO_0_REG_1_R_SEL  ((u32)0x0000001F << 24)          /*!<R/W 0x3  5’d0: RX_CH0_data_out (MIC path) 5’d1: RX_CH1_data_out (MIC path) 5’d2: RX_CH2_data_out (MIC path) 5’d3: RX_CH3_data_out (MIC path) 5’d4: RX_CH4_data_out (MIC path) 5’d5: RX_CH5_data_out (MIC path) 5’d6: RX_CH6_data_out (MIC path) 5’d7: RX_CH7_data_out (MIC path) 5’d8: spa_direct_in_0 5’d9: spa_direct_in_1 5’d10: spa_direct_in_2 5’d11: spa_direct_in_3 5’d12: spa_direct_in_4 5’d13: spa_direct_in_5 5’d14: spa_direct_in_6 5’d15: spa_direct_in_7 5’d16: spb_direct_in_0 5’d17: spb_direct_in_1 5’d18: spb_direct_in_2 5’d19: spb_direct_in_3 5’d20: spb_direct_in_4 5’d21: spb_direct_in_5 5’d22: spb_direct_in_6 5’d23: spb_direct_in_7 5’d24: spc_direct_in_0 5’d25: spc_direct_in_1 5’d26: spc_direct_in_2 5’d27: spc_direct_in_3 5’d28: spc_direct_in_4 5’d29: spc_direct_in_5 5’d30: spc_direct_in_6 5’d31: spc_direct_in_7 SPORT0: a = 1, b = 2, c = 3 SPORT1: a = 0, b = 2, c = 3 SPORT2: a = 0, b = 1, c = 3 SPORT3: a = 0, b = 1, c = 2 */
#define SP_RX_FIFO_0_REG_1_R_SEL(x)    (((u32)((x) & 0x0000001F) << 24))
#define SP_GET_RX_FIFO_0_REG_1_R_SEL(x) ((u32)(((x >> 24) & 0x0000001F)))
#define SP_MASK_RX_FIFO_0_REG_1_L_SEL  ((u32)0x0000001F << 16)          /*!<R/W 0x2  (Ibid.) */
#define SP_RX_FIFO_0_REG_1_L_SEL(x)    (((u32)((x) & 0x0000001F) << 16))
#define SP_GET_RX_FIFO_0_REG_1_L_SEL(x) ((u32)(((x >> 16) & 0x0000001F)))
#define SP_MASK_RX_FIFO_0_REG_0_R_SEL  ((u32)0x0000001F << 8)          /*!<R/W 0x1  (Ibid.) */
#define SP_RX_FIFO_0_REG_0_R_SEL(x)    (((u32)((x) & 0x0000001F) << 8))
#define SP_GET_RX_FIFO_0_REG_0_R_SEL(x) ((u32)(((x >> 8) & 0x0000001F)))
#define SP_MASK_RX_FIFO_0_REG_0_L_SEL  ((u32)0x0000001F << 0)          /*!<R/W 0x0  (Ibid.) */
#define SP_RX_FIFO_0_REG_0_L_SEL(x)    (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_RX_FIFO_0_REG_0_L_SEL(x) ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_DIRECT_CTRL4
 * @brief SPORT Direct Control Register 4
 * @{
 **/
#define SP_MASK_RX_FIFO_1_REG_1_R_SEL  ((u32)0x0000001F << 24)          /*!<R/W 0x7  5’d0: RX_CH0_data_out (MIC path) 5’d1: RX_CH1_data_out (MIC path) 5’d2: RX_CH2_data_out (MIC path) 5’d3: RX_CH3_data_out (MIC path) 5’d4: RX_CH4_data_out (MIC path) 5’d5: RX_CH5_data_out (MIC path) 5’d6: RX_CH6_data_out (MIC path) 5’d7: RX_CH7_data_out (MIC path) 5’d8: spa_direct_in_0 5’d9: spa_direct_in_1 5’d10: spa_direct_in_2 5’d11: spa_direct_in_3 5’d12: spa_direct_in_4 5’d13: spa_direct_in_5 5’d14: spa_direct_in_6 5’d15: spa_direct_in_7 5’d16: spb_direct_in_0 5’d17: spb_direct_in_1 5’d18: spb_direct_in_2 5’d19: spb_direct_in_3 5’d20: spb_direct_in_4 5’d21: spb_direct_in_5 5’d22: spb_direct_in_6 5’d23: spb_direct_in_7 5’d24: spc_direct_in_0 5’d25: spc_direct_in_1 5’d26: spc_direct_in_2 5’d27: spc_direct_in_3 5’d28: spc_direct_in_4 5’d29: spc_direct_in_5 5’d30: spc_direct_in_6 5’d31: spc_direct_in_7 SPORT0: a = 1, b = 2, c = 3 SPORT1: a = 0, b = 2, c = 3 SPORT2: a = 0, b = 1, c = 3 SPORT3: a = 0, b = 1, c = 2 */
#define SP_RX_FIFO_1_REG_1_R_SEL(x)    (((u32)((x) & 0x0000001F) << 24))
#define SP_GET_RX_FIFO_1_REG_1_R_SEL(x) ((u32)(((x >> 24) & 0x0000001F)))
#define SP_MASK_RX_FIFO_1_REG_1_L_SEL  ((u32)0x0000001F << 16)          /*!<R/W 0x6  (Ibid.) */
#define SP_RX_FIFO_1_REG_1_L_SEL(x)    (((u32)((x) & 0x0000001F) << 16))
#define SP_GET_RX_FIFO_1_REG_1_L_SEL(x) ((u32)(((x >> 16) & 0x0000001F)))
#define SP_MASK_RX_FIFO_1_REG_0_R_SEL  ((u32)0x0000001F << 8)          /*!<R/W 0x5  (Ibid.) */
#define SP_RX_FIFO_1_REG_0_R_SEL(x)    (((u32)((x) & 0x0000001F) << 8))
#define SP_GET_RX_FIFO_1_REG_0_R_SEL(x) ((u32)(((x >> 8) & 0x0000001F)))
#define SP_MASK_RX_FIFO_1_REG_0_L_SEL  ((u32)0x0000001F << 0)          /*!<R/W 0x4  (Ibid.) */
#define SP_RX_FIFO_1_REG_0_L_SEL(x)    (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_RX_FIFO_1_REG_0_L_SEL(x) ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_RX_COUNTER1
 * @brief SPORT RX Counter Register 1
 * @{
 **/
#define SP_BIT_CLR_RX_SPORT_RDY        ((u32)0x00000001 << 31)          /*!<W1C 0  X = (tx_sport_compare_val). When counter equal X. Sport will send tx_sport_interrupt to DSP. FW should take care X={32~8191} */
#define SP_BIT_EN_RX_SPORT_INTERRUPT   ((u32)0x00000001 << 30)          /*!<R/W 0  Enable rx sport interrupt. */
#define SP_BIT_FS_PHASE_EN_D64         ((u32)0x00000001 << 28)          /*!<R/W 0  Enable div64 fs_phase_trace_cnt for both tx and rx */
#define SP_BIT_RX_FS_PHASE_RPT_B5      ((u32)0x00000001 << 27)          /*!<R 0  rx_fs_phase_rptp[5] when using div64 fs_phase_trace_cnt */
#define SP_MASK_RX_SPORT_COMPARE_VAL   ((u32)0x07FFFFFF << 0)          /*!<R/W 0x40  X = (rx_sport_compare_val). When counter equal X. Sport will send rx_sport_interrupt to DSP. FW should take care X={32~134217727} */
#define SP_RX_SPORT_COMPARE_VAL(x)     (((u32)((x) & 0x07FFFFFF) << 0))
#define SP_GET_RX_SPORT_COMPARE_VAL(x) ((u32)(((x >> 0) & 0x07FFFFFF)))
/** @} */

/** @defgroup SP_RX_COUNTER2
 * @brief SPORT RX Counter Register 2
 * @{
 **/
#define SP_MASK_RX_SPORT_COUNTER       ((u32)0x07FFFFFF << 5)          /*!<R 0x0  For DSP read instant rx sport counter value, counter down */
#define SP_RX_SPORT_COUNTER(x)         (((u32)((x) & 0x07FFFFFF) << 5))
#define SP_GET_RX_SPORT_COUNTER(x)     ((u32)(((x >> 5) & 0x07FFFFFF)))
#define SP_MASK_RX_FS_PHASE_RPT        ((u32)0x0000001F << 0)          /*!<R 0x0  Report RX phase */
#define SP_RX_FS_PHASE_RPT(x)          (((u32)((x) & 0x0000001F) << 0))
#define SP_GET_RX_FS_PHASE_RPT(x)      ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup SP_TX_FIFO_0_WR_ADDR
 * @brief TX FIFO 0 Write Address Register
 * @{
 **/
#define SP_MASK_TX_FIFO_0_WR_ADDR      ((u32)0xFFFFFFFF << 0)          /*!<W 0x0  TX_FIFO_0_WR_ADDR */
#define SP_TX_FIFO_0_WR_ADDR(x)        (((u32)((x) & 0xFFFFFFFF) << 0))
#define SP_GET_TX_FIFO_0_WR_ADDR(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SP_RX_FIFO_0_RD_ADDR
 * @brief RX FIFO 0 Read Address Register
 * @{
 **/
#define SP_MASK_RX_FIFO_0_RD_ADDR      ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  RX_FIFO_0_RD_ADDR */
#define SP_RX_FIFO_0_RD_ADDR(x)        (((u32)((x) & 0xFFFFFFFF) << 0))
#define SP_GET_RX_FIFO_0_RD_ADDR(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SP_TX_FIFO_1_WR_ADDR
 * @brief TX FIFO 1 Write Address Register
 * @{
 **/
#define SP_MASK_TX_FIFO_1_WR_ADDR      ((u32)0xFFFFFFFF << 0)          /*!<W 0x0  TX_FIFO_1_WR_ADDR */
#define SP_TX_FIFO_1_WR_ADDR(x)        (((u32)((x) & 0xFFFFFFFF) << 0))
#define SP_GET_TX_FIFO_1_WR_ADDR(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SP_RX_FIFO_1_RD_ADDR
 * @brief RX FIFO 1 Read Address Register
 * @{
 **/
#define SP_MASK_RX_FIFO_1_RD_ADDR      ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  RX_FIFO_1_RD_ADDR */
#define SP_RX_FIFO_1_RD_ADDR(x)        (((u32)((x) & 0xFFFFFFFF) << 0))
#define SP_GET_RX_FIFO_1_RD_ADDR(x)    ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup AUDIO_SPORT_Exported_Types AUDIO_SPORT Exported Types
  * @{
*****************************************************************************/


/**
 * @brief AUDIO_SPORT Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t SP_REG_MUX;                             /*!< SPORT MUX REGISTER,  Address offset: 0x0000 */
	__IO uint32_t SP_CTRL0;                               /*!< SPORT CONTROL REGISTER 0 Register,  Address offset: 0x0004 */
	__IO uint32_t SP_CTRL1;                               /*!< SPORT CONTROL REGISTER 1 Register,  Address offset: 0x0008 */
	__IO uint32_t SP_INT_CTRL;                            /*!< SPORT INTERRUPT CONTROL REGISTER,  Address offset: 0x000C */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0x10 */
	__I  uint32_t SP_TRX_COUNTER_STATUS;                  /*!< SPORT TRX COUNTER STATUS REGISTER,  Address offset: 0x0014 */
	__I  uint32_t SP_ERR;                                 /*!< SPORT ERROR REGISTER,  Address offset: 0x0018 */
	__IO uint32_t SP_TX_BCLK;                             /*!< SPORT TX BCLK REGISTER,  Address offset: 0x001C */
	__IO uint32_t SP_TX_LRCLK;                            /*!< SPORT TX LRCLK REGISTER,  Address offset: 0x0020 */
	__I  uint32_t SP_FIFO_CTRL;                           /*!< SPORT FIFO CONTROL REGISTER,  Address offset: 0x0024 */
	__IO uint32_t SP_FORMAT;                              /*!< SPORT FORMAT REGISTER,  Address offset: 0x0028 */
	__IO uint32_t SP_RX_BCLK;                             /*!< SPORT RX BCLK REGISTER,  Address offset: 0x002C */
	__IO uint32_t SP_RX_LRCLK;                            /*!< SPORT RX LRCLK REGISTER,  Address offset: 0x0030 */
	__I  uint32_t SP_DSP_COUNTER;                         /*!< SPORT DSP COUNTER REGISTER,  Address offset: 0x0034 */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0x38 */
	__IO uint32_t SP_DIRECT_CTRL0;                        /*!< SPORT DIRECT CONTROL REGISTER 0 Register,  Address offset: 0x003C */
	__IO uint32_t RSVD2;                                  /*!<  Reserved,  Address offset:0x40 */
	__IO uint32_t SP_FIFO_IRQ;                            /*!< SPORT FIFO IRQ REGISTER,  Address offset: 0x0044 */
	__IO uint32_t SP_DIRECT_CTRL1;                        /*!< SPORT DIRECT CONTROL REGISTER 1 Register,  Address offset: 0x0048 */
	__IO uint32_t SP_DIRECT_CTRL2;                        /*!< SPORT DIRECT CONTROL REGISTER 2 Register,  Address offset: 0x004C */
	__IO uint32_t RSVD3;                                  /*!<  Reserved,  Address offset:0x50 */
	__IO uint32_t SP_DIRECT_CTRL3;                        /*!< SPORT DIRECT CONTROL REGISTER 3 Register,  Address offset: 0x0054 */
	__IO uint32_t SP_DIRECT_CTRL4;                        /*!< SPORT DIRECT CONTROL REGISTER 4 Register,  Address offset: 0x0058 */
	__IO uint32_t SP_RX_COUNTER1;                         /*!< SPORT RX COUNTER REGISTER 1 Register,  Address offset: 0x005C */
	__I  uint32_t SP_RX_COUNTER2;                         /*!< SPORT RX COUNTER REGISTER 2 Register,  Address offset: 0x0060 */
	__IO uint32_t RSVD4[487];                              /*!<  Reserved,  Address offset:0x64-0x7FC */
	__O  uint32_t SP_TX_FIFO_0_WR_ADDR;                   /*!< TX FIFO 0 WRITE ADDRESS REGISTER,  Address offset: 0x0800 */
	__IO uint32_t RSVD5[31];                              /*!<  Reserved,  Address offset:0x804-0x87C */
	__I  uint32_t SP_RX_FIFO_0_RD_ADDR;                   /*!< RX FIFO 0 READ ADDRESS REGISTER,  Address offset: 0x0880 */
	__IO uint32_t RSVD6[31];                              /*!<  Reserved,  Address offset:0x884-0x8FC */
	__O  uint32_t SP_TX_FIFO_1_WR_ADDR;                   /*!< TX FIFO 1 WRITE ADDRESS REGISTER,  Address offset: 0x0900 */
	__IO uint32_t RSVD7[31];                              /*!<  Reserved,  Address offset:0x904-0x97C */
	__I  uint32_t SP_RX_FIFO_1_RD_ADDR;                   /*!< RX FIFO 1 READ ADDRESS REGISTER,  Address offset: 0x0980 */
} AUDIO_SPORT_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/* Exported types ------------------------------------------------------------*/
/** @addtogroup AUDIO_SPORT_Exported_Types AUDIO_SPORT Exported Types
  * @{
  */

/**
  * @brief	AUDIO_SPORT Register Structure Definition
  */
typedef struct {

	u32 SP_DIR_TX_flag;
	u32 SP_DIR_RX_flag;
	u32 SP_Bclk;

} SP_RegTypeDef;

/**
* @brief  AUDIO_SPORT Init Structure Definition
*/
typedef struct {

	u32 SP_SelDataFormat; 			/*!< Specifies the AUDIO SPORT data format
									This parameter can be a value of @ref AUDIO_SPORT_Interface_Format */

	u32 SP_SelWordLen;		/*!< Specifies the AUDIO SPORT TX word length
									This parameter can be a value of @ref AUDIO_SPORT_Word_Length */

	u32 SP_SelChLen;		/*!< Specifies the AUDIO SPORT selection of Tx CHANNEL Length
									This parameter can be a value of @ref AUDIO_SPORT_Tx_Channel_Length */

	u32 SP_SR;				/*!< Specifies the AUDIO SPORT selection of sample rate
									This parameter can be a value of @ref AUDIO_SPORT_Sample_Rate */

	u32 SP_SelCh;			/*!< Specifies the AUDIO SPORT selection of TX channel for ADC path
									This parameter can be a value of @ref AUDIO_SPORT_Tx_Channel */
	u32 SP_SelTDM;			/*!< Specifies the AUDIO SPORT selection of Tx TDM MODE
									This parameter can be a value of @ref AUDIO_SPORT_Tx_TDM_Mode */

	u32 SP_SelFIFO; 	/*!< Specifies the AUDIO SPORT selection of Tx FIFO CHANNEL
								This parameter can be a value of @ref AUDIO_SPORT_Tx_FIFO */

	u32 SP_SelI2SMonoStereo;	/*!< Specifies the AUDIO SPORT channel number
									This parameter can be a value of @ref AUDIO_SPORT_Channel_Number */

	u32 SP_SetMultiIO;	/*!< Specifies the AUDIO SPORT selection of Tx MultiIO Mode
								This parameter can be a value of @ref AUDIO_SPORT_Tx_MultiIO_Mode */

	u32 SP_SelClk;		/*AUDIO SPORT clock sources, 40/98.304/45.158/24.576MHz*/

	u32 SP_Bclk;		/*!< Specifies the AUDIO SPORT BCLK */

	u32 SP_Fix_Bclk;

} SP_InitTypeDef;

/**
  * @}
  */

/* Exported constants ------------------------------------------------------------*/
/** @defgroup AUDIO_SPORT_Exported_Constants AUDIO_SPORT Exported Constants
  * @{
  */

/** @defgroup AUDIO_SPORT_Word_Length
  * @{
  */
#define SP_TXWL_16							((u32)0x00000000)
#define SP_TXWL_20							((u32)0x00000001)
#define SP_TXWL_24							((u32)0x00000002)
#define SP_TXWL_32							((u32)0x00000004)

#define IS_SP_TX_WL(LEN) (((LEN) == SP_TXWL_16) || \
										((LEN) == SP_TXWL_20) || \
										((LEN) == SP_TXWL_24) || \
										((LEN) == SP_TXWL_32))

#define SP_RXWL_16							((u32)0x00000000)
#define SP_RXWL_20							((u32)0x00000001)
#define SP_RXWL_24							((u32)0x00000002)
#define SP_RXWL_32							((u32)0x00000004)

#define IS_SP_RX_WL(LEN) (((LEN) == SP_RXWL_16) || \
										((LEN) == SP_RXWL_20) || \
										((LEN) == SP_RXWL_24) || \
										((LEN) == SP_RXWL_32))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Interface_Format
  * @{
  */
#define SP_DF_I2S						((u32)0x00000000)
#define SP_DF_LEFT						((u32)0x00000001)
#define SP_DF_PCM_A						((u32)0x00000002)
#define SP_DF_PCM_B						((u32)0x00000003)

#define IS_SP_DATA_FMT(FORMAT) (((FORMAT) == SP_DF_I2S) || \
								((FORMAT) == SP_DF_LEFT) || \
								((FORMAT) == SP_DF_PCM_A) || \
								((FORMAT) == SP_DF_PCM_B))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Channel_Number
  * @{
  */
#define SP_CH_STEREO					((u32)0x00000000)
#define SP_CH_MONO						((u32)0x00000001)

#define IS_SP_CHN_NUM(NUM) (((NUM) == SP_CH_STEREO) || \
							((NUM) == SP_CH_MONO))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Rx_Channel
  * @{
  */
#define SP_RX_CH_LR						((u32)0x00000000)
#define SP_RX_CH_RL						((u32)0x00000001)
#define SP_RX_CH_LL						((u32)0x00000002)
#define SP_RX_CH_RR						((u32)0x00000003)

#define IS_SP_SEL_RX_CH(CH) (((CH) == SP_RX_CH_LR) || \
							((CH) == SP_RX_CH_RL) || \
							((CH) == SP_RX_CH_LL) || \
							((CH) == SP_RX_CH_RR))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Tx_Channel
  * @{
  */
#define SP_TX_CH_LR						((u32)0x00000000)
#define SP_TX_CH_RL						((u32)0x00000001)
#define SP_TX_CH_LL						((u32)0x00000002)
#define SP_TX_CH_RR						((u32)0x00000003)

#define IS_SP_SEL_TX_CH(CH) (((CH) == SP_TX_CH_LR) || \
							((CH) == SP_TX_CH_RL) || \
							((CH) == SP_TX_CH_LL) || \
							((CH) == SP_TX_CH_RR))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Tx_TDM_Mode
  * @{
  */
#define SP_TX_NOTDM						((u32)0x00000000)
#define SP_TX_TDM4						((u32)0x00000001)
#define SP_TX_TDM6						((u32)0x00000002)
#define SP_TX_TDM8						((u32)0x00000003)

#define IS_SP_SEL_TX_TDM(SEL) (((SEL) == SP_TX_NOTDM) || \
								((SEL) == SP_TX_TDM4) || \
								((SEL) == SP_TX_TDM6) || \
								((SEL) == SP_TX_TDM8))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Rx_TDM_Mode
  * @{
  */
#define SP_RX_NOTDM						((u32)0x00000000)
#define SP_RX_TDM4						((u32)0x00000001)
#define SP_RX_TDM6						((u32)0x00000002)
#define SP_RX_TDM8						((u32)0x00000003)

#define IS_SP_SEL_RX_TDM(SEL) (((SEL) == SP_RX_NOTDM) || \
								((SEL) == SP_RX_TDM4) || \
								((SEL) == SP_RX_TDM6) || \
								((SEL) == SP_RX_TDM8))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Tx_FIFO
  * @{
  */
#define SP_TX_FIFO2						((u32)0x00000000)
#define SP_TX_FIFO4						((u32)0x00000001)
#define SP_TX_FIFO6						((u32)0x00000002)
#define SP_TX_FIFO8						((u32)0x00000003)

#define IS_SP_SEL_TX_FIFO(SEL) (((SEL) == SP_TX_FIFO2) || \
								((SEL) == SP_TX_FIFO4) || \
								((SEL) == SP_TX_FIFO6) || \
								((SEL) == SP_TX_FIFO8))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Rx_FIFO
  * @{
  */
#define SP_RX_FIFO2							((u32)0x00000000)
#define SP_RX_FIFO4							((u32)0x00000001)
#define SP_RX_FIFO6							((u32)0x00000002)
#define SP_RX_FIFO8							((u32)0x00000003)

#define IS_SP_SEL_RX_FIFO(SEL) (((SEL) == SP_RX_FIFO2) || \
							((SEL) == SP_RX_FIFO4) || \
							((SEL) == SP_RX_FIFO6) || \
							((SEL) == SP_RX_FIFO8))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Tx_Channel_Length
  * @{
  */
#define SP_TXCL_16							((u32)0x00000000)
#define SP_TXCL_20							((u32)0x00000001)
#define SP_TXCL_24							((u32)0x00000002)
#define SP_TXCL_8							((u32)0x00000003)
#define SP_TXCL_32							((u32)0x00000004)

#define IS_SP_TXCH_LEN(LEN) (((LEN) == SP_TXCL_16) || \
							((LEN) == SP_TXCL_20) || \
							((LEN) == SP_TXCL_24) || \
							((LEN) == SP_TXCL_32))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Rx_Channel_Length
  * @{
  */
#define SP_RXCL_16							((u32)0x00000000)
#define SP_RXCL_20							((u32)0x00000001)
#define SP_RXCL_24							((u32)0x00000002)
#define SP_RXCL_8							((u32)0x00000003)
#define SP_RXCL_32							((u32)0x00000004)

#define IS_SP_RXCH_LEN(LEN) (((LEN) == SP_RXCL_16) || \
							((LEN) == SP_RXCL_20) || \
							((LEN) == SP_RXCL_24) || \
							((LEN) == SP_RXCL_32))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_GMDA_Selection
  * @{
  */
#define GDMA_INT						((u32)0x00000000)
#define GDMA_EXT						((u32)0x00000001)

#define IS_SP_SEL_GDMA(SEL) (((SEL) == GDMA_INT) || \
							((SEL) == GDMA_EXT))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Tx_MultiIO_Mode
  * @{
  */
#define SP_TX_MULTIIO_DIS 					((u32)0x00000000)
#define SP_TX_MULTIIO_EN 					((u32)0x00000001)

#define IS_SP_SET_TX_MULTIIO(SEL) (((SEL) == SP_TX_MULTIIO_DIS) || \
									((SEL) == SP_TX_MULTIIO_EN))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Rx_MultiIO_Mode
  * @{
  */
#define SP_RX_MULTIIO_DIS						((u32)0x00000000)
#define SP_RX_MULTIIO_EN 						((u32)0x00000001)

#define IS_SP_SET_RX_MULTIIO(SEL) (((SEL) == SP_RX_MULTIIO_DIS) || \
									((SEL) == SP_RX_MULTIIO_EN))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Application_Mode
* @{
*/
#define SP_DIR_TX			((u32)0x00000001)
#define SP_DIR_RX			((u32)0x00000002)
#define IS_SP_SET_DIR(SEL) (((SEL) == SP_DIR_TX) || \
							((SEL) == SP_DIR_RX))
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_I2S
* @{
*/
#define I2S2	((u32)0x00000002)
#define I2S3	((u32)0x00000003)
#define IS_SP_SEL_I2S(SEL) (((SEL) == I2S2) || \
							((SEL) == I2S3))
/**
* @}
*/

/** @defgroup AUDIO_SPORT_Master_Slave_Mode
* @{
*/
#define MASTER	((u32)0x00000000)
#define SLAVE	((u32)0x00000001)
#define IS_SP_SEL_MODE(SEL) (((SEL) == MASTER) || \
							((SEL) == SLAVE))

/**
* @}
*/

/** @defgroup AUDIO_SPORT_Channel_Length
* @{
*/
#define SP_CL_16		(16)
#define SP_CL_20		(20)
#define SP_CL_24		(24)
#define SP_CL_32		(32)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_Sample_Rate
* @{
*/
#define SP_8K			(8000)
#define SP_12K			(12000)
#define SP_24K			(24000)
#define SP_16K			(16000)
#define SP_32K			(32000)
#define SP_48K			(48000)
#define SP_96K			(96000)
#define SP_192K			(192000)
#define SP_384K			(384000)
#define SP_11P025K		(11025)
#define SP_44P1K		(44100)
#define SP_88P2K		(88200)
#define SP_22P05K		(22050)
#define SP_176P4K		(176400)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_I2S_Mode
* @{
*/
#define TDM		((u32)0x00000000)
#define MULTIIO	((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_I2S_FIXED_BCLK_DIV
  * @{
  */
#define DSP_SRC_BCLK_DIV4   ((u32)0x00000000)
#define DSP_SRC_BCLK_DIV2   ((u32)0x00000001)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_I2S_MCLK
  * @{
  */
#define DSP_SRC_MCLK_DIV4   ((u32)0x00000000)
#define DSP_SRC_MCLK_DIV2   ((u32)0x00000001)
#define DSP_SRC_MCLK        ((u32)0x00000002)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_XTAL40M
  * @{
  */
#define I2S_XTAL40M	40000000
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_TXChannel
* @{
*/
#define TXCHN0		((u32)0x00000000)
#define TXCHN1		((u32)0x00000001)
#define TXCHN2		((u32)0x00000002)
#define TXCHN3		((u32)0x00000003)
#define TXCHN4		((u32)0x00000004)
#define TXCHN5		((u32)0x00000005)
#define TXCHN6		((u32)0x00000006)
#define TXCHN7		((u32)0x00000007)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_TXFIFOChannel
* @{
*/
#define TX_FIFO0_REG0_L		((u32)0x00000000)
#define TX_FIFO0_REG0_R		((u32)0x00000001)
#define TX_FIFO0_REG1_L		((u32)0x00000002)
#define TX_FIFO0_REG1_R		((u32)0x00000003)
#define TX_FIFO1_REG0_L		((u32)0x00000004)
#define TX_FIFO1_REG0_R		((u32)0x00000005)
#define TX_FIFO1_REG1_L		((u32)0x00000006)
#define TX_FIFO1_REG1_R		((u32)0x00000007)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_RXChannel
* @{
*/
#define RXCHN0		((u32)0x00000000)
#define RXCHN1		((u32)0x00000001)
#define RXCHN2		((u32)0x00000002)
#define RXCHN3		((u32)0x00000003)
#define RXCHN4		((u32)0x00000004)
#define RXCHN5		((u32)0x00000005)
#define RXCHN6		((u32)0x00000006)
#define RXCHN7		((u32)0x00000007)
/**
  * @}
  */

/** @defgroup AUDIO_SPORT_RXFIFOChannel
* @{
*/
#define RX_FIFO0_REG0_L		((u32)0x00000000)
#define RX_FIFO0_REG0_R		((u32)0x00000001)
#define RX_FIFO0_REG1_L		((u32)0x00000002)
#define RX_FIFO0_REG1_R		((u32)0x00000003)
#define RX_FIFO1_REG0_L		((u32)0x00000004)
#define RX_FIFO1_REG0_R		((u32)0x00000005)
#define RX_FIFO1_REG1_L		((u32)0x00000006)
#define RX_FIFO1_REG1_R		((u32)0x00000007)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions ------------------------------------------------------------*/
/** @defgroup AUDIO_SPORT_Exported_Functions AUDIO_SPORT Exported Functions
  * @{
  */

_LONG_CALL_ void AUDIO_SP_StructInit(SP_InitTypeDef *SP_InitStruct);
_LONG_CALL_ BOOL AUDIO_SP_Register(u32 index, u32 direction, SP_InitTypeDef *SP_InitStruct);
_LONG_CALL_ void AUDIO_SP_Unregister(u32 index, u32 direction);
_LONG_CALL_ void AUDIO_SP_Reset(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetTXChnLen(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetRXChnLen(u32 index);
_LONG_CALL_ void AUDIO_SP_SetMclk(u32 index, u32 NewState);
_LONG_CALL_ void AUDIO_SP_SetMclkDiv(u32 index, u32 mck_div);
_LONG_CALL_ void AUDIO_SP_SetFixBclk(u32 index, SP_InitTypeDef *SP_InitStruct);
_LONG_CALL_ void AUDIO_SP_SelFixBclk(u32 index, u32 bclk_sel);
_LONG_CALL_ void AUDIO_SP_SetTXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len);
_LONG_CALL_ void AUDIO_SP_SetRXClkDiv(u32 index, u32 clock, u32 sr, u32 tdm, u32 chn_len);
_LONG_CALL_ void AUDIO_SP_TXCHNSrcSel(u32 index, u32 tx_chn, u32 fifo_chn);
_LONG_CALL_ void AUDIO_SP_RXFIFOSrcSel(u32 index, u32 fifo_chn, u32 rx_chn);
_LONG_CALL_ void AUDIO_SP_TXSetFifo(u32 index, u32 fifo_num, u32 NewState);
_LONG_CALL_ void AUDIO_SP_RXSetFifo(u32 index, u32 fifo_num, u32 NewState);
_LONG_CALL_ void AUDIO_SP_Init(u32 index, u32 direction, SP_InitTypeDef *SP_InitStruct);
_LONG_CALL_ void AUDIO_SP_TXStart(u32 index, u32 NewState);
_LONG_CALL_ void AUDIO_SP_RXStart(u32 index, u32 NewState);
_LONG_CALL_ void AUDIO_SP_DmaCmd(u32 index, u32 NewState);
_LONG_CALL_ void AUDIO_SP_SetSelfLPBK(u32 index);
_LONG_CALL_ void AUDIO_SP_SetTXWordLen(u32 index, u32 SP_Tx_WordLen);
_LONG_CALL_ void AUDIO_SP_SetRXWordLen(u32 index, u32 SP_Rx_WordLen);
_LONG_CALL_ u32 AUDIO_SP_GetTXWordLen(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetRXWordLen(u32 index);
_LONG_CALL_ void AUDIO_SP_SetMonoStereo(u32 index, u32 SP_MonoStereo);
_LONG_CALL_ void AUDIO_SP_SetMasterSlave(u32 index, u32 SP_MasterSlave);
_LONG_CALL_ BOOL AUDIO_SP_TXGDMA_Init(u32 Index, u32 SelGDMA, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
									  IRQ_FUN CallbackFunc, u8 *pTxData, u32 Length);
_LONG_CALL_ BOOL AUDIO_SP_RXGDMA_Init(u32 Index, u32 SelGDMA, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
									  IRQ_FUN CallbackFunc, u8 *pRxData, u32 Length);
_LONG_CALL_ BOOL AUDIO_SP_TXGDMA_Restart(u8 GDMA_Index, u8 GDMA_ChNum, u32 TX_addr,	u32 TX_length);
_LONG_CALL_ BOOL AUDIO_SP_RXGDMA_Restart(u8 GDMA_Index,	u8 GDMA_ChNum,	u32 RX_addr,	u32 RX_length);
_LONG_CALL_ BOOL AUDIO_SP_LLPTXGDMA_Init(u32 Index,	u32 SelGDMA, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
		IRQ_FUN CallbackFunc, u32 Length, u32 MaxLLP, struct GDMA_CH_LLI *Lli);
_LONG_CALL_ BOOL AUDIO_SP_LLPRXGDMA_Init(u32 Index,	u32 SelGDMA, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
		IRQ_FUN CallbackFunc, u32 Length, u32 MaxLLP, struct GDMA_CH_LLI *Lli);
_LONG_CALL_ void AUDIO_SP_SetTXCounter(u32 index, u32 state);
_LONG_CALL_ void AUDIO_SP_SetTXCounterCompVal(u32 index, u32 comp_val);
_LONG_CALL_ void AUDIO_SP_ClearTXCounterIrq(u32 index);
_LONG_CALL_ void AUDIO_SP_SetPhaseLatch(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetTXCounterVal(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetTXPhaseVal(u32 index);
_LONG_CALL_ void AUDIO_SP_SetRXCounter(u32 index, u32 state);
_LONG_CALL_ void AUDIO_SP_SetRXCounterCompVal(u32 index, u32 comp_val);
_LONG_CALL_ void AUDIO_SP_ClearRXCounterIrq(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetRXCounterVal(u32 index);
_LONG_CALL_ u32 AUDIO_SP_GetRXPhaseVal(u32 index);
_LONG_CALL_ void AUDIO_SP_SetDirectOutMode(u32 index_src, u32 index_dir);
_LONG_CALL_ void AUDIO_SP_SelDirectOutSource(u32 index_src, u32 index_dir);
_LONG_CALL_ void AUDIO_SP_SetTSFTPhaseLatch(u32 index, u32 state);
_LONG_CALL_ void AUDIO_SP_Deinit(u32 index, u32 direction);


/**
  * @}
  */

/** @} */

/** @} */

/** @} */

/* Other Definitions --------------------------------------------------------*/
typedef struct {
	AUDIO_SPORT_TypeDef *SPORTx;
	u32 Tx_HandshakeInterface;
	u32 Rx_HandshakeInterface;
	u32 Tx_HandshakeInterface1;
	u32 Rx_HandshakeInterface1;
} AUDIO_DevTable;

extern const AUDIO_DevTable AUDIO_DEV_TABLE[4];
#define AUDIO_BLOCK_SIZE 2048

/* MANUAL_GEN_END */

#endif


