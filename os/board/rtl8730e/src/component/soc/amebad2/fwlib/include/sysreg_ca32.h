#ifndef _AMEBAD2_CA32_H_
#define _AMEBAD2_CA32_H_

/* AUTO_GEN_START */

/**************************************************************************//**
 * @defgroup CA32_MISC_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_CTIIRQ                     ((u32)0x00000003 << 12)          /*!<R 0  CTIIRQ status. TODO:if connect to intr control */
#define CA32_CTIIRQ(x)                       ((u32)(((x) & 0x00000003) << 12))
#define CA32_GET_CTIIRQ(x)                   ((u32)(((x >> 12) & 0x00000003)))
#define CA32_MASK_CTIIRQACK                  ((u32)0x00000003 << 8)          /*!<R/W 0  CTIIRQ ack */
#define CA32_CTIIRQACK(x)                    ((u32)(((x) & 0x00000003) << 8))
#define CA32_GET_CTIIRQACK(x)                ((u32)(((x >> 8) & 0x00000003)))
#define CA32_MASK_DBGNOPWRDWN                ((u32)0x00000003 << 6)          /*!<R 0  No power-down request For power flow emulation/debug only. To try power down flow but indicate to power controller not to really power down. Let floating if not used */
#define CA32_DBGNOPWRDWN(x)                  ((u32)(((x) & 0x00000003) << 6))
#define CA32_GET_DBGNOPWRDWN(x)              ((u32)(((x >> 6) & 0x00000003)))
#define CA32_MASK_DBGPWRUPREQ                ((u32)0x00000003 << 4)          /*!<R 0  Debug core power up request. (Can use external debugger to write CPU debug register to request for powering up ; left floating if not used) */
#define CA32_DBGPWRUPREQ(x)                  ((u32)(((x) & 0x00000003) << 4))
#define CA32_GET_DBGPWRUPREQ(x)              ((u32)(((x >> 4) & 0x00000003)))
#define CA32_MASK_DBGPWRDUP                  ((u32)0x00000003 << 0)          /*!<R/W 0x3  High : allow external debug access to the processor. Low : prevent external debug access to the processor if no debug-over-power-down feature, tie HIGH if with debug-over-power-down feature, connect to regif */
#define CA32_DBGPWRDUP(x)                    ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_DBGPWRDUP(x)                ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_C0_RST_CTRL
 * @brief Cluster 0 Reset Control Register
 * @{
 *****************************************************************************/
#define CA32_BIT_L2FLUSHDONE                 ((u32)0x00000001 << 29)          /*!<R 0  1:indicate L2 flush is done */
#define CA32_BIT_L2FLUSHREQ                  ((u32)0x00000001 << 28)          /*!<R/W 0  1:require L2 flush */
#define CA32_BIT_L2RSTDISABLE                ((u32)0x00000001 << 25)          /*!<R/W 0  Disable automatic L2 cache invalidate at reset 0: L2 cache is reset by hardware. 1: L2 cache is not reset by hardware. L2RSTDISABLE must be held LOW during the initial reset sequence for the multiprocessor device. */
#define CA32_BIT_NL2RESET                    ((u32)0x00000001 << 24)          /*!<R/W 0x1  SCU global reset 0: Apply reset to shared L2 memory system controller. 1: Do not apply reset to shared L2 memory system controller. */
#define CA32_BIT_NGICRESET                   ((u32)0x00000001 << 12)          /*!<R/W 0x1  GIC resest , active low. */
#define CA32_BIT_NRESETSOCDBG                ((u32)0x00000001 << 8)          /*!<R/W 0x1  Debug logic resets 0: Apply reset to debug, breakpoint and watchpoint logic. 1: Do not apply reset to debug, breakpoint and watchpoint logic. */
#define CA32_MASK_NCORERESET                 ((u32)0x00000003 << 4)          /*!<R/W 0x3  Individual processor resets excluding Debug and ETM 0: Apply reset to processor that includes NEON and VFP, but excludes Debug, ETM, breakpoint and watchpoint logic. 1: Do not apply reset to processor that includes NEON and VFP, but excludes Debug, ETM, breakpoint and watchpoint logic. */
#define CA32_NCORERESET(x)                   ((u32)(((x) & 0x00000003) << 4))
#define CA32_GET_NCORERESET(x)               ((u32)(((x >> 4) & 0x00000003)))
#define CA32_MASK_NCOREPORESET               ((u32)0x00000003 << 0)          /*!<R/W 0x3  All processor reset 0: Apply reset to all processor logic that includes NEON and VFP, Debug, ETM, breakpoint and watchpoint logic. 1: Do not apply reset to all processor logic that includes NEON and VFP, Debug, ETM, breakpoint and watchpoint logic. */
#define CA32_NCOREPORESET(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_NCOREPORESET(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_C0_CPU_STATUS
 * @brief Cluster0 CPU Status Register
 * @{
 *****************************************************************************/
#define CA32_BIT_STANDBYWFIL2                ((u32)0x00000001 << 19)          /*!<RO 0  Indicates if the L2 memory system is in WFE low-power state. This signal is active when the following conditions are met. * All processors are in WFI low-power state. * L2 memory system is idle. */
#define CA32_MASK_STANDBYWFE                 ((u32)0x00000003 << 16)          /*!<RO 0  Indicates if a processor is in WFE low-power state. 0: Processor not in WFE low-power state. 1: Processor in WFE low-power state. */
#define CA32_STANDBYWFE(x)                   ((u32)(((x) & 0x00000003) << 16))
#define CA32_GET_STANDBYWFE(x)               ((u32)(((x >> 16) & 0x00000003)))
#define CA32_MASK_STANDBYWFI                 ((u32)0x00000003 << 8)          /*!<RO 0x2  Indicates if a processor is in WFI low-power state. 0: Processor not in WFI low-power state. 1: Processor in WFI low-power state. */
#define CA32_STANDBYWFI(x)                   ((u32)(((x) & 0x00000003) << 8))
#define CA32_GET_STANDBYWFI(x)               ((u32)(((x >> 8) & 0x00000003)))
#define CA32_MASK_SMPNAMP                    ((u32)0x00000003 << 0)          /*!<RO 0x3  Signals Symmetric MultiProcessing (SMP) mode or Asymmetric MultiProcessing (AMP) for each processor in the Cortex-A7 MPCore processor. 0: AMP mode 1: SMP mode */
#define CA32_SMPNAMP(x)                      ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_SMPNAMP(x)                  ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_C0_CTRL_REG0
 * @brief Cluster 0 Control Register0
 * @{
 *****************************************************************************/
#define CA32_MASK_VINITHI                    ((u32)0x00000003 << 20)          /*!<R/W 0   */
#define CA32_VINITHI(x)                      ((u32)(((x) & 0x00000003) << 20))
#define CA32_GET_VINITHI(x)                  ((u32)(((x >> 20) & 0x00000003)))
#define CA32_MASK_CFGTE                      ((u32)0x00000003 << 18)          /*!<R/W 0   */
#define CA32_CFGTE(x)                        ((u32)(((x) & 0x00000003) << 18))
#define CA32_GET_CFGTE(x)                    ((u32)(((x >> 18) & 0x00000003)))
#define CA32_MASK_CFGEND                     ((u32)0x00000003 << 16)          /*!<R/W 0  00: little endian ([0]/[1] for two core) 11: bit endian */
#define CA32_CFGEND(x)                       ((u32)(((x) & 0x00000003) << 16))
#define CA32_GET_CFGEND(x)                   ((u32)(((x >> 16) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_C0_CTRL_REG1
 * @brief Cluster 0 Secure Control Register0
 * @{
 *****************************************************************************/
#define CA32_BIT_CFGSDISABLE                 ((u32)0x00000001 << 8)          /*!<R/W 0  Disables write access to some secure GIC registers */
#define CA32_MASK_CP15SDISABLE               ((u32)0x00000003 << 0)          /*!<R/W 0  Disable write access to some secure CP15 registers */
#define CA32_CP15SDISABLE(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_CP15SDISABLE(x)             ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DSS0
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_DSS_COUNT_OUT_31_0         ((u32)0xFFFFFFFF << 0)          /*!<R 32'h0  [19:0] - core 0 dss count_out [39:20] - core 1 dss count_out count_out[19:0] is counter value indicates the speed of selected ring oscillator Cycle time = (count_out*dss_clk period)/ 16384 */
#define CA32_DSS_COUNT_OUT_31_0(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CA32_GET_DSS_COUNT_OUT_31_0(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DSS1
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_DSS_COUNT_OUT_39_32        ((u32)0x000000FF << 0)          /*!<R 8'h0  ca7_dss_count_out[39:20] - core 1 dss count_out count_out[19:0] is counter value indicates the speed of selected ring oscillator Cycle time = (count_out*dss_clk period)/ 16384 */
#define CA32_DSS_COUNT_OUT_39_32(x)          ((u32)(((x) & 0x000000FF) << 0))
#define CA32_GET_DSS_COUNT_OUT_39_32(x)      ((u32)(((x >> 0) & 0x000000FF)))
#define CA32_MASK_DSS_READY                  ((u32)0x00000003 << 8)          /*!<R 2'b0  [0] - core 0 dss ready [1] - core 1 dss ready Ready strobe to read count_out value for each DSS. */
#define CA32_DSS_READY(x)                    ((u32)(((x) & 0x00000003) << 8))
#define CA32_GET_DSS_READY(x)                ((u32)(((x >> 8) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DSS2
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_DSS_RO_SEL                 ((u32)0x0000003F << 0)          /*!<R/W 6'h0  [2:0]- core 0 dss ro_sel [5:3]- core 1 dss ro_sel ro_sel[2:0] - Ring Oscillator selection: 000: select dss_clk (for test mode) 001 : select 1st ring oscillator 010 : select 2nd ring oscillator 011 : select 3rd ring oscillator 100 : select 4th ring oscillator 101 : select multi-library ring oscillator Others : not used */
#define CA32_DSS_RO_SEL(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define CA32_GET_DSS_RO_SEL(x)               ((u32)(((x >> 0) & 0x0000003F)))
#define CA32_MASK_DSS_SPEED_EN               ((u32)0x00000003 << 6)          /*!<R/W 2'b0  [0] - core 0 dss speed_en [1] - core 1 dss speed_en Speed sensor enable of each DSS 0: disable 1: enable */
#define CA32_DSS_SPEED_EN(x)                 ((u32)(((x) & 0x00000003) << 6))
#define CA32_GET_DSS_SPEED_EN(x)             ((u32)(((x >> 6) & 0x00000003)))
#define CA32_MASK_DSS_WIRE_SEL               ((u32)0x00000003 << 8)          /*!<R/W 2'b0  [0] - core 0 dss wire_sel [1] - core 1 dss wire_sel wire selection 0: loong wire/delay cell/clock buffer 1: short wire */
#define CA32_DSS_WIRE_SEL(x)                 ((u32)(((x) & 0x00000003) << 8))
#define CA32_GET_DSS_WIRE_SEL(x)             ((u32)(((x >> 8) & 0x00000003)))
#define CA32_MASK_DSS_RST_N                  ((u32)0x00000003 << 10)          /*!<R/W 2'b0  [0] - core 0 dss rst_n [1] - core 1 dss rst_n rst_n Reset signal of each Speed-Sensor (low active) (Software control reset) Function of DSS - Digital Speed Sensor On-chip variation measurement (PVT) – Process variation • Inter-die process variation • Intra-die process variation • Library cell variation • Specific component variation • PMOS and NMOS variation – Voltage variation • IR drop – Temperature variation •Testing – Sorting or binning ICs by target speed There is one DSS in ca7 core 0 and one DSS in ca7 core 1. */
#define CA32_DSS_RST_N(x)                    ((u32)(((x) & 0x00000003) << 10))
#define CA32_GET_DSS_RST_N(x)                ((u32)(((x >> 10) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_SRAM_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define CA32_BIT_DVSE_SCU_TAG                ((u32)0x00000001 << 0)          /*!<R/W 1'b0  delay option enable. Active high. */
#define CA32_BIT_DVSE_L2_TAG                 ((u32)0x00000001 << 1)          /*!<R/W 1'b0   */
#define CA32_BIT_DVSE_L2_DATA                ((u32)0x00000001 << 2)          /*!<R/W 1'b0   */
#define CA32_BIT_DVSE_L2_VIC                 ((u32)0x00000001 << 3)          /*!<R/W 1'b0   */
#define CA32_MASK_DVS_SCU_TAG                ((u32)0x0000000F << 4)          /*!<R/W 4'h6  delay option. SYNL DVS:1100 SENL DVS:0110 */
#define CA32_DVS_SCU_TAG(x)                  ((u32)(((x) & 0x0000000F) << 4))
#define CA32_GET_DVS_SCU_TAG(x)              ((u32)(((x >> 4) & 0x0000000F)))
#define CA32_MASK_DVS_L2_TAG                 ((u32)0x0000000F << 8)          /*!<R/W 4'h6   */
#define CA32_DVS_L2_TAG(x)                   ((u32)(((x) & 0x0000000F) << 8))
#define CA32_GET_DVS_L2_TAG(x)               ((u32)(((x >> 8) & 0x0000000F)))
#define CA32_MASK_DVS_L2_DATA                ((u32)0x0000000F << 12)          /*!<R/W 4'hc   */
#define CA32_DVS_L2_DATA(x)                  ((u32)(((x) & 0x0000000F) << 12))
#define CA32_GET_DVS_L2_DATA(x)              ((u32)(((x >> 12) & 0x0000000F)))
#define CA32_MASK_DVS_L2_VIC                 ((u32)0x0000000F << 16)          /*!<R/W 4'h6   */
#define CA32_DVS_L2_VIC(x)                   ((u32)(((x) & 0x0000000F) << 16))
#define CA32_GET_DVS_L2_VIC(x)               ((u32)(((x >> 16) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_SRAM_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define CA32_BIT_RME_DDATA                   ((u32)0x00000001 << 0)          /*!<R/W 1'b0  Read margin enable. 0: select internal default RM setting. 1: select external pin RM setting. */
#define CA32_BIT_RME_DTAG                    ((u32)0x00000001 << 1)          /*!<R/W 1'b0   */
#define CA32_BIT_RME_DDIRTY                  ((u32)0x00000001 << 2)          /*!<R/W 1'b0   */
#define CA32_BIT_RME_IDATA                   ((u32)0x00000001 << 3)          /*!<R/W 1'b0   */
#define CA32_BIT_RME_ITAG                    ((u32)0x00000001 << 4)          /*!<R/W 1'b0   */
#define CA32_BIT_RME_TLB                     ((u32)0x00000001 << 5)          /*!<R/W 1'b0   */
#define CA32_MASK_RM_DDATA                   ((u32)0x0000000F << 8)          /*!<R/W 4'h2  Read margin pin. RM[3:2] factory pins reserved for debug mode. RM[1:0] controls access time and cycle time of the memory, 2'b00 is slowest. RM[1:0] is not used when RME=0. */
#define CA32_RM_DDATA(x)                     ((u32)(((x) & 0x0000000F) << 8))
#define CA32_GET_RM_DDATA(x)                 ((u32)(((x >> 8) & 0x0000000F)))
#define CA32_MASK_RM_DTAG                    ((u32)0x0000000F << 12)          /*!<R/W 4'h2   */
#define CA32_RM_DTAG(x)                      ((u32)(((x) & 0x0000000F) << 12))
#define CA32_GET_RM_DTAG(x)                  ((u32)(((x >> 12) & 0x0000000F)))
#define CA32_MASK_RM_DDIRTY                  ((u32)0x0000000F << 16)          /*!<R/W 4'h2   */
#define CA32_RM_DDIRTY(x)                    ((u32)(((x) & 0x0000000F) << 16))
#define CA32_GET_RM_DDIRTY(x)                ((u32)(((x >> 16) & 0x0000000F)))
#define CA32_MASK_RM_IDATA                   ((u32)0x0000000F << 20)          /*!<R/W 4'h2   */
#define CA32_RM_IDATA(x)                     ((u32)(((x) & 0x0000000F) << 20))
#define CA32_GET_RM_IDATA(x)                 ((u32)(((x >> 20) & 0x0000000F)))
#define CA32_MASK_RM_ITAG                    ((u32)0x0000000F << 24)          /*!<R/W 4'h2   */
#define CA32_RM_ITAG(x)                      ((u32)(((x) & 0x0000000F) << 24))
#define CA32_GET_RM_ITAG(x)                  ((u32)(((x >> 24) & 0x0000000F)))
#define CA32_MASK_RM_TLB                     ((u32)0x0000000F << 28)          /*!<R/W 4'h2   */
#define CA32_RM_TLB(x)                       ((u32)(((x) & 0x0000000F) << 28))
#define CA32_GET_RM_TLB(x)                   ((u32)(((x >> 28) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_SRAM_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_CORE_SRAM_SD               ((u32)0x00000003 << 0)          /*!<R/W 2'b00  [0] : core 0 [1] : core 1 0: core ARM sram disable shut down. 1: core ARM sram enable shut down. */
#define CA32_CORE_SRAM_SD(x)                 ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_CORE_SRAM_SD(x)             ((u32)(((x >> 0) & 0x00000003)))
#define CA32_MASK_CORE_SRAM_LS               ((u32)0x00000003 << 2)          /*!<R/W 2'b00  [0] : core 0 [1] : core 1 0 : core ARM sram disable light sleep. 1 : core ARM sram enable light sleep. */
#define CA32_CORE_SRAM_LS(x)                 ((u32)(((x) & 0x00000003) << 2))
#define CA32_GET_CORE_SRAM_LS(x)             ((u32)(((x >> 2) & 0x00000003)))
#define CA32_MASK_CORE_SRAM_DS               ((u32)0x00000003 << 4)          /*!<R/W 2'b00  [0] : core 0 [1] : core 1 0 : core ARM sram disable deep sleep. 1 : core ARM sram enable deep sleep. */
#define CA32_CORE_SRAM_DS(x)                 ((u32)(((x) & 0x00000003) << 4))
#define CA32_GET_CORE_SRAM_DS(x)             ((u32)(((x >> 4) & 0x00000003)))
#define CA32_BIT_L2_SRAM_SLP                 ((u32)0x00000001 << 9)          /*!<R/W 1'b0  0: L2cache ARM sram disable shut down. 1: L2cache ARM sram enable shut down. Set CSB=1 & SLP=1 shut down both peripheral power and array power. Data contens in cell arrays are lost. */
#define CA32_BIT_L2_SRAM_NAP                 ((u32)0x00000001 << 10)          /*!<R/W 1'b0  0 : L2cache ARM sram disable light sleep. 1 : L2cache ARM sram enable light sleep. Set CSB=1 & NAP=1 reduce both peripheral power and array power. Data contens in cell arrays are retained. */
#define CA32_BIT_L2_SRAM_RET                 ((u32)0x00000001 << 11)          /*!<R/W 1'b0  0 : L2cache ARM sram disable deep sleep. 1 : L2cache ARM sram enable deep sleep.Set CSB=1 & RET=1 shut down only peripheral power. Data contens in cell arrays are retained. */
#define CA32_MASK_CORE_SRAM_TEST1            ((u32)0x00000003 << 12)          /*!<R/W 2'b0   */
#define CA32_CORE_SRAM_TEST1(x)              ((u32)(((x) & 0x00000003) << 12))
#define CA32_GET_CORE_SRAM_TEST1(x)          ((u32)(((x >> 12) & 0x00000003)))
#define CA32_BIT_L2_SRAM_TMCK                ((u32)0x00000001 << 14)          /*!<R/W 1'b0   */
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL0
 * @brief
 * @{
 *****************************************************************************/
#define CA32_BIT_COORD_RSTN                  ((u32)0x00000001 << 0)          /*!<R/W 0  Coordinator Controller Reset external register control reset signal, used for async reset of coordinator controller */
#define CA32_MASK_SHRBUS_EN_CPU              ((u32)0x00000003 << 1)          /*!<R/W 0  share bus enable bit for cpu top slice-0. To enable the share bus of the corresponding cpu core/top, MBIST share bus would not trigger until shrbus_go bit is set. */
#define CA32_SHRBUS_EN_CPU(x)                ((u32)(((x) & 0x00000003) << 1))
#define CA32_GET_SHRBUS_EN_CPU(x)            ((u32)(((x >> 1) & 0x00000003)))
#define CA32_BIT_SHRBUS_GO                   ((u32)0x00000001 << 3)          /*!<R/W 0  After programming ca7_shrbus_en_cpu and the related mbist/drf/mbisd control registers, F/W set this bit to trigger cpu MBIST test. For another test, shrbus_go bit shall be clear first and then set again. */
#define CA32_MASK_MBISTCFG_SEL               ((u32)0x00000003 << 4)          /*!<R/W 0  mbist configuration selection from external control register. 0 : mbistcfg will be set 0 1 : mbistcfg will be set 1 mbistcfg : mbist configuration 0 : MBIST ALL mode disabled 1 : MBIST ALL mode enabled */
#define CA32_MBISTCFG_SEL(x)                 ((u32)(((x) & 0x00000003) << 4))
#define CA32_GET_MBISTCFG_SEL(x)             ((u32)(((x >> 4) & 0x00000003)))
#define CA32_MASK_MBIST_FNSH                 ((u32)0x00000003 << 6)          /*!<R 0  coordinator mbist finish when all the selected memories had been tested, coordinator asserts the signal to indicate mbist or drf mbist finished. */
#define CA32_MBIST_FNSH(x)                   ((u32)(((x) & 0x00000003) << 6))
#define CA32_GET_MBIST_FNSH(x)               ((u32)(((x >> 6) & 0x00000003)))
#define CA32_MASK_MBISTREQ_DBG               ((u32)0x00000003 << 8)          /*!<R 0  share bus mbist request debug signal replica of mbist request signal, used for external monitor */
#define CA32_MBISTREQ_DBG(x)                 ((u32)(((x) & 0x00000003) << 8))
#define CA32_GET_MBISTREQ_DBG(x)             ((u32)(((x >> 8) & 0x00000003)))
#define CA32_MASK_MBISTACK_DBG               ((u32)0x00000003 << 10)          /*!<R 0  share bus mbist ack debug signal replica of mbist ack signal, used for external monitor */
#define CA32_MBISTACK_DBG(x)                 ((u32)(((x) & 0x00000003) << 10))
#define CA32_GET_MBISTACK_DBG(x)             ((u32)(((x >> 10) & 0x00000003)))
#define CA32_MASK_BIST_MODE_ALL              ((u32)0x00000003 << 12)          /*!<R/W 0  when set, coordinator will test all the memories despite of bist_mode. */
#define CA32_BIST_MODE_ALL(x)                ((u32)(((x) & 0x00000003) << 12))
#define CA32_GET_BIST_MODE_ALL(x)            ((u32)(((x >> 12) & 0x00000003)))
#define CA32_MASK_BIST_ALG_SELECTION         ((u32)0x0000007F << 16)          /*!<R/W 7'h1  [0] : default algorithm [1] : write + consecutive read 4 times, reverse test once again [2] : write + consecutive read 8 times, reverse test once again [3] : write + consecutive read 12 times, reverse test once again */
#define CA32_BIST_ALG_SELECTION(x)           ((u32)(((x) & 0x0000007F) << 16))
#define CA32_GET_BIST_ALG_SELECTION(x)       ((u32)(((x >> 16) & 0x0000007F)))
#define CA32_BIT_MBIST_DELAY_ENABLE          ((u32)0x00000001 << 23)          /*!<R/W 1'b0  mbist interface will not activate until delay time had reached the one specified by mbist_delay_num */
#define CA32_MASK_MBIST_DELAY_NUM            ((u32)0x00000003 << 24)          /*!<R/W 2'b00  Define mbist delay time interval 0 : 2^5 clock cycles 1 : 2^7 clock cycles 2 : 2^9 clock cycles 3 : 2^10 clock cycles */
#define CA32_MBIST_DELAY_NUM(x)              ((u32)(((x) & 0x00000003) << 24))
#define CA32_GET_MBIST_DELAY_NUM(x)          ((u32)(((x >> 24) & 0x00000003)))
#define CA32_BIT_MBIST_DMY_WR_ENABLE         ((u32)0x00000001 << 26)          /*!<R/W 1'b0  0 : do nothing during mbist delay timing interval 1 : issue dummy write operation during mbist delay time interval */
#define CA32_BIT_SHRBUS_DELAY_ENABLE         ((u32)0x00000001 << 27)          /*!<R/W 1'b0  Control share bus delay start function. 0 : share bus delayed start is disabled 1 : share bus delayed start is enabled. Next_shrbus_str signal of each coordinator will be activated. The delayed time interval depends on shrbus_delay_num. */
#define CA32_MASK_SHRBUS_DELAY_NUM           ((u32)0x0000000F << 28)          /*!<R/W 4'h0  share bus delay number Define share bus start timing of next coordinator. The share bus of the next coordinator will be activated when the tested mbist controllers of current coordinator equal to shrbus_delay_num+1. e.g., 0 : share bus starts when 1st mbist controller tested 1 : share bus starts when 2nd mbist controller tested */
#define CA32_SHRBUS_DELAY_NUM(x)             ((u32)(((x) & 0x0000000F) << 28))
#define CA32_GET_SHRBUS_DELAY_NUM(x)         ((u32)(((x >> 28) & 0x0000000F)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL1
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_BIST_MODE                  ((u32)0x0007FFFF << 0)          /*!<R/W 0  if ca7_bist_mode_all is 0, test memories whose corresponding ca7_bist_mode[15:0] bit is set 1. if ca7_bist_mode_all is 1, test all memories. */
#define CA32_BIST_MODE(x)                    ((u32)(((x) & 0x0007FFFF) << 0))
#define CA32_GET_BIST_MODE(x)                ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL2
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_BIST_DONE                  ((u32)0x0007FFFF << 0)          /*!<R 0  mbist done status of each memory . */
#define CA32_BIST_DONE(x)                    ((u32)(((x) & 0x0007FFFF) << 0))
#define CA32_GET_BIST_DONE(x)                ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL3
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_BIST_FAIL                  ((u32)0x0007FFFF << 0)          /*!<R 0  mbist fail status of each memory . when ca7_drf_mbist_mode = 0, bist_fail is corresponding to normal bist test. when ca7_drf_mbist_mode = 1, bist_fail is corresponding to drf bist test. */
#define CA32_BIST_FAIL(x)                    ((u32)(((x) & 0x0007FFFF) << 0))
#define CA32_GET_BIST_FAIL(x)                ((u32)(((x >> 0) & 0x0007FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL4
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_BIST_BANK_FAIL             ((u32)0x0000FFFF << 0)          /*!<R 0  sram maximum banks CA7 : 8 bank */
#define CA32_BIST_BANK_FAIL(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define CA32_GET_BIST_BANK_FAIL(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL5
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_BIST_SRAM_DBG_SEL          ((u32)0x0000003F << 0)          /*!<R/W 0  select debug sram 0000 : ddata 0001 : dtag 0010 : ddirty 0011 : idata 0100 : itag 0101 : tlb 0110 : scu_tag 0111 : l2_tag 1000 : l2_data */
#define CA32_BIST_SRAM_DBG_SEL(x)            ((u32)(((x) & 0x0000003F) << 0))
#define CA32_GET_BIST_SRAM_DBG_SEL(x)        ((u32)(((x >> 0) & 0x0000003F)))
#define CA32_BIT_BIST_SRAM_DBG_EN            ((u32)0x00000001 << 12)          /*!<R/W 0  bist_sram_dbg_en is a global control signal to latch internal bist fail status when set 1, the sram bank fail status selected by bist_sram_dbg_sel revealed on bist_bank_fail port 0 : bist_bank_fail is not available 1 : internal bist bank fail status latched on bist_bank_fail */
#define CA32_BIT_BIST_SRAM_DBG_CLR           ((u32)0x00000001 << 13)          /*!<R/W 0  bist_sram_dbg_clr is used to clear bist_bank_fail 1 : reset bist_bank_fail */
#define CA32_MASK_DRF_BIST_MODE              ((u32)0x00000003 << 16)          /*!<R/W 0  DRF bist mode enable 1: select DRF/VDDR mode mbist 0: select normal mode mbist */
#define CA32_DRF_BIST_MODE(x)                ((u32)(((x) & 0x00000003) << 16))
#define CA32_GET_DRF_BIST_MODE(x)            ((u32)(((x >> 16) & 0x00000003)))
#define CA32_MASK_DRF_TEST_RESUME            ((u32)0x00000003 << 18)          /*!<R/W 0  1: test resume for DRF/VDDR mode mbist mode */
#define CA32_DRF_TEST_RESUME(x)              ((u32)(((x) & 0x00000003) << 18))
#define CA32_GET_DRF_TEST_RESUME(x)          ((u32)(((x >> 18) & 0x00000003)))
#define CA32_MASK_DRF_START_PAUSE            ((u32)0x00000003 << 20)          /*!<R 0  1: start pause for DRF/VDDR mode mbist mode */
#define CA32_DRF_START_PAUSE(x)              ((u32)(((x) & 0x00000003) << 20))
#define CA32_GET_DRF_START_PAUSE(x)          ((u32)(((x >> 20) & 0x00000003)))
#define CA32_MASK_DRF_BIST_DONE              ((u32)0x00000003 << 22)          /*!<R 0  bist done of all memory for DRF/VDDR mode mbist */
#define CA32_DRF_BIST_DONE(x)                ((u32)(((x) & 0x00000003) << 22))
#define CA32_GET_DRF_BIST_DONE(x)            ((u32)(((x >> 22) & 0x00000003)))
#define CA32_MASK_DRF_DYN_READ_EN            ((u32)0x00000003 << 24)          /*!<R/W 0  drf dynamic read enable for VDDR(Dynamic Data Retetion) test 0 : data retention fail test 1 : dynamic data retention test VDDR mode mbist is selected if both ca7_drf_dyn_read_en=1 and ca7_drf_bist_mode=1 */
#define CA32_DRF_DYN_READ_EN(x)              ((u32)(((x) & 0x00000003) << 24))
#define CA32_GET_DRF_DYN_READ_EN(x)          ((u32)(((x >> 24) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL6
 * @brief
 * @{
 *****************************************************************************/
#define CA32_BIT_DIAG_MODE                   ((u32)0x00000001 << 0)          /*!<R/W 0  0 : ram is not diagnosed 1 : ram is to be diagnosed */
#define CA32_MASK_DIAG_MEMARRAY_SEL          ((u32)0x000001FF << 3)          /*!<R/W 0  select which memory bank will be diagnosized, it will substitute the original mbistarray when diag_mode is set. */
#define CA32_DIAG_MEMARRAY_SEL(x)            ((u32)(((x) & 0x000001FF) << 3))
#define CA32_GET_DIAG_MEMARRAY_SEL(x)        ((u32)(((x >> 3) & 0x000001FF)))
#define CA32_BIT_DIAG_SO                     ((u32)0x00000001 << 12)          /*!<R 0  diagnosis shift out data serial shift out data from MBISD. */
#define CA32_BIT_DIAG_START                  ((u32)0x00000001 << 13)          /*!<R 0  diagnosis start When failure is tested, diagnosis start will be asserted and deasserted until failure syndrome had been shifted out. */
#define CA32_MASK_BIST_LOOP_MODE             ((u32)0x0000001F << 15)          /*!<R/W 0  When the bist loop is set to 00, the coordinator can test every bank within a single memory. When the bist loop is set to 01, the coordinator can test a single memory bank repeatedly til the bist loop is set to 00. When the bist loop is set to 10, the coordinator can test a single memory bank. */
#define CA32_BIST_LOOP_MODE(x)               ((u32)(((x) & 0x0000001F) << 15))
#define CA32_GET_BIST_LOOP_MODE(x)           ((u32)(((x >> 15) & 0x0000001F)))
#define CA32_BIT_DIAG_CPUCORE_SEL            ((u32)0x00000001 << 20)          /*!<R/W 0  select cpu core to be diagnosized, it will substitute the original mbistarray when diag_mode is set. */
/** @} */

/**************************************************************************//**
 * @defgroup CA32_COOR_CTRL7
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_DRF_DEBUG_HL_WR            ((u32)0x00000003 << 0)          /*!<R/W 0   */
#define CA32_DRF_DEBUG_HL_WR(x)              ((u32)(((x) & 0x00000003) << 0))
#define CA32_GET_DRF_DEBUG_HL_WR(x)          ((u32)(((x >> 0) & 0x00000003)))
#define CA32_MASK_BIST_SCRMCONFIG            ((u32)0x000000FF << 8)          /*!<R/W 0  select which memory bank will be diagnosized, it will substitute the original mbistarray when diag_mode is set. */
#define CA32_BIST_SCRMCONFIG(x)              ((u32)(((x) & 0x000000FF) << 8))
#define CA32_GET_BIST_SCRMCONFIG(x)          ((u32)(((x >> 8) & 0x000000FF)))
#define CA32_MASK_BIST_REDUCE_ADDR_EN        ((u32)0x00000003 << 16)          /*!<R/W 0  diagnosis shift out data serial shift out data from MBISD. */
#define CA32_BIST_REDUCE_ADDR_EN(x)          ((u32)(((x) & 0x00000003) << 16))
#define CA32_GET_BIST_REDUCE_ADDR_EN(x)      ((u32)(((x >> 16) & 0x00000003)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DIAG_RCV_DATA_0
 * @brief diagnosis receive data bit[31:0]
 * @{
 *****************************************************************************/
#define CA32_MASK_DIAG_RCV_DATA_0            ((u32)0xFFFFFFFF << 0)          /*!<R 0  diagnosis shift out data package [31:0]. */
#define CA32_DIAG_RCV_DATA_0(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CA32_GET_DIAG_RCV_DATA_0(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DIAG_RCV_DATA_1
 * @brief diagnosis receive data bit[63:32]
 * @{
 *****************************************************************************/
#define CA32_MASK_DIAG_RCV_DATA_1            ((u32)0xFFFFFFFF << 0)          /*!<R 0  diagnosis shift out data package [63:32]. */
#define CA32_DIAG_RCV_DATA_1(x)              ((u32)(((x) & 0xFFFFFFFF) << 0))
#define CA32_GET_DIAG_RCV_DATA_1(x)          ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DIAG_RCV_DATA_2
 * @brief diagnosis receive data bit[94:64]
 * @{
 *****************************************************************************/
#define CA32_MASK_DIAG_RCV_DATA_2            ((u32)0x7FFFFFFF << 0)          /*!<R 0  diagnosis shift out data package [94:64]. */
#define CA32_DIAG_RCV_DATA_2(x)              ((u32)(((x) & 0x7FFFFFFF) << 0))
#define CA32_GET_DIAG_RCV_DATA_2(x)          ((u32)(((x >> 0) & 0x7FFFFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS0
 * @brief CA7 top module C30 DSS
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS0_RO_SEL            ((u32)0x00000007 << 0)          /*!<R/W 3'h0  Ring Oscillator selection: 000: select dss_clk (for test mode) 001 : select 1st ring oscillator 010 : select 2nd ring oscillator 011 : select 3rd ring oscillator 100 : select 4th ring oscillator 101 : select multi-library ring oscillator Others : not used */
#define CA32_TOP_DSS0_RO_SEL(x)              ((u32)(((x) & 0x00000007) << 0))
#define CA32_GET_TOP_DSS0_RO_SEL(x)          ((u32)(((x >> 0) & 0x00000007)))
#define CA32_BIT_TOP_DSS0_SPEED_EN           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  Speed sensor enable of each DSS 0: disable 1: enable */
#define CA32_BIT_TOP_DSS0_WIRE_SEL           ((u32)0x00000001 << 4)          /*!<R/W 1'b0  wire selection 0: loong wire/delay cell/clock buffer 1: short wire */
#define CA32_BIT_TOP_DSS0_RST_N              ((u32)0x00000001 << 5)          /*!<R/W 1'b0  rst_n Reset signal of each Speed-Sensor (low active) (Software control reset) */
#define CA32_BIT_TOP_DSS0_READY              ((u32)0x00000001 << 6)          /*!<R 1'b0  Ready strobe to read count_out value for each DSS. */
#define CA32_BIT_TOP_DSS0_WSORT_GO           ((u32)0x00000001 << 7)          /*!<R 1'b0  0: measured speed slower than expected. 1: measured speed faster than expected. */
#define CA32_MASK_TOP_DSS0_COUNT_OUT         ((u32)0x000FFFFF << 8)          /*!<R 20'h0  count_out[19:0] is counter value indicates the speed of selected ring oscillator Cycle time = (count_out*dss_clk period)/ 16384 */
#define CA32_TOP_DSS0_COUNT_OUT(x)           ((u32)(((x) & 0x000FFFFF) << 8))
#define CA32_GET_TOP_DSS0_COUNT_OUT(x)       ((u32)(((x >> 8) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS1
 * @brief CA7 top module C35 DSS
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS1_RO_SEL            ((u32)0x00000007 << 0)          /*!<R/W 3'h0  Ring Oscillator selection: 000: select dss_clk (for test mode) 001 : select 1st ring oscillator 010 : select 2nd ring oscillator 011 : select 3rd ring oscillator 100 : select 4th ring oscillator 101 : select multi-library ring oscillator Others : not used */
#define CA32_TOP_DSS1_RO_SEL(x)              ((u32)(((x) & 0x00000007) << 0))
#define CA32_GET_TOP_DSS1_RO_SEL(x)          ((u32)(((x >> 0) & 0x00000007)))
#define CA32_BIT_TOP_DSS1_SPEED_EN           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  Speed sensor enable of each DSS 0: disable 1: enable */
#define CA32_BIT_TOP_DSS1_WIRE_SEL           ((u32)0x00000001 << 4)          /*!<R/W 1'b0  wire selection 0: loong wire/delay cell/clock buffer 1: short wire */
#define CA32_BIT_TOP_DSS1_RST_N              ((u32)0x00000001 << 5)          /*!<R/W 1'b0  rst_n Reset signal of each Speed-Sensor (low active) (Software control reset) */
#define CA32_BIT_TOP_DSS1_READY              ((u32)0x00000001 << 6)          /*!<R 1'b0  Ready strobe to read count_out value for each DSS. */
#define CA32_BIT_TOP_DSS1_WSORT_GO           ((u32)0x00000001 << 7)          /*!<R 1'b0  0: measured speed slower than expected. 1: measured speed faster than expected. */
#define CA32_MASK_TOP_DSS1_COUNT_OUT         ((u32)0x000FFFFF << 8)          /*!<R 20'h0  count_out[19:0] is counter value indicates the speed of selected ring oscillator Cycle time = (count_out*dss_clk period)/ 16384 */
#define CA32_TOP_DSS1_COUNT_OUT(x)           ((u32)(((x) & 0x000FFFFF) << 8))
#define CA32_GET_TOP_DSS1_COUNT_OUT(x)       ((u32)(((x >> 8) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS2
 * @brief CA7 top module C40 DSS
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS2_RO_SEL            ((u32)0x00000007 << 0)          /*!<R/W 3'h0  Ring Oscillator selection: 000: select dss_clk (for test mode) 001 : select 1st ring oscillator 010 : select 2nd ring oscillator 011 : select 3rd ring oscillator 100 : select 4th ring oscillator 101 : select multi-library ring oscillator Others : not used */
#define CA32_TOP_DSS2_RO_SEL(x)              ((u32)(((x) & 0x00000007) << 0))
#define CA32_GET_TOP_DSS2_RO_SEL(x)          ((u32)(((x >> 0) & 0x00000007)))
#define CA32_BIT_TOP_DSS2_SPEED_EN           ((u32)0x00000001 << 3)          /*!<R/W 1'b0  Speed sensor enable of each DSS 0: disable 1: enable */
#define CA32_BIT_TOP_DSS2_WIRE_SEL           ((u32)0x00000001 << 4)          /*!<R/W 1'b0  wire selection 0: loong wire/delay cell/clock buffer 1: short wire */
#define CA32_BIT_TOP_DSS2_RST_N              ((u32)0x00000001 << 5)          /*!<R/W 1'b0  rst_n Reset signal of each Speed-Sensor (low active) (Software control reset) */
#define CA32_BIT_TOP_DSS2_READY              ((u32)0x00000001 << 6)          /*!<R 1'b0  Ready strobe to read count_out value for each DSS. */
#define CA32_BIT_TOP_DSS2_WSORT_GO           ((u32)0x00000001 << 7)          /*!<R 1'b0  0: measured speed slower than expected. 1: measured speed faster than expected. */
#define CA32_MASK_TOP_DSS2_COUNT_OUT         ((u32)0x000FFFFF << 8)          /*!<R 20'h0  count_out[19:0] is counter value indicates the speed of selected ring oscillator Cycle time = (count_out*dss_clk period)/ 16384 */
#define CA32_TOP_DSS2_COUNT_OUT(x)           ((u32)(((x) & 0x000FFFFF) << 8))
#define CA32_GET_TOP_DSS2_COUNT_OUT(x)       ((u32)(((x >> 8) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS_DATA_IN_0
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS0_DATA_IN           ((u32)0x000FFFFF << 0)          /*!<R/W 20'h0  Input data for speed comparison */
#define CA32_TOP_DSS0_DATA_IN(x)             ((u32)(((x) & 0x000FFFFF) << 0))
#define CA32_GET_TOP_DSS0_DATA_IN(x)         ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS_DATA_IN_1
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS1_DATA_IN           ((u32)0x000FFFFF << 0)          /*!<R/W 20'h0  Input data for speed comparison */
#define CA32_TOP_DSS1_DATA_IN(x)             ((u32)(((x) & 0x000FFFFF) << 0))
#define CA32_GET_TOP_DSS1_DATA_IN(x)         ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_TOP_DSS_DATA_IN_2
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DSS2_DATA_IN           ((u32)0x000FFFFF << 0)          /*!<R/W 20'h0  Input data for speed comparison */
#define CA32_TOP_DSS2_DATA_IN(x)             ((u32)(((x) & 0x000FFFFF) << 0))
#define CA32_GET_TOP_DSS2_DATA_IN(x)         ((u32)(((x >> 0) & 0x000FFFFF)))
/** @} */

/**************************************************************************//**
 * @defgroup CA32_DUMMY_CTRL
 * @brief
 * @{
 *****************************************************************************/
#define CA32_MASK_TOP_DUMMY_IN               ((u32)0x0000FFFF << 0)          /*!<R/W 16'hFF00  ca32 ECO_004:top_dummy_in[8] connect to ACLKENM, must set 0 when do ca32 mbist to close AXI. */
#define CA32_TOP_DUMMY_IN(x)                 ((u32)(((x) & 0x0000FFFF) << 0))
#define CA32_GET_TOP_DUMMY_IN(x)             ((u32)(((x >> 0) & 0x0000FFFF)))
#define CA32_MASK_TOP_DUMMY_OUT              ((u32)0x0000FFFF << 16)          /*!<R 0   */
#define CA32_TOP_DUMMY_OUT(x)                ((u32)(((x) & 0x0000FFFF) << 16))
#define CA32_GET_TOP_DUMMY_OUT(x)            ((u32)(((x >> 16) & 0x0000FFFF)))
/** @} */



/**************************************************************************//**
 * @defgroup AMEBAD2_CA32
 * @{
 * @brief AMEBAD2_CA32 Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t CA32_MISC_CTRL;                         /*!<  Register,  Address offset: 0x00 */
	__IO uint32_t CA32_C0_RST_CTRL;                       /*!< CLUSTER 0 RESET CONTROL REGISTER,  Address offset: 0x04 */
	__I  uint32_t CA32_C0_CPU_STATUS;                     /*!< CLUSTER0 CPU STATUS REGISTER,  Address offset: 0x08 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__IO uint32_t CA32_C0_CTRL_REG0;                      /*!< CLUSTER 0 CONTROL REGISTER0 Register,  Address offset: 0x10 */
	__IO uint32_t CA32_C0_CTRL_REG1;                      /*!< CLUSTER 0 SECURE CONTROL REGISTER0 Register,  Address offset: 0x14 */
	__IO uint32_t RSVD1[18];                              /*!<  Reserved,  Address offset:0x18-0x5C */
	__I  uint32_t CA32_DSS0;                              /*!<  Register,  Address offset: 0x60 */
	__I  uint32_t CA32_DSS1;                              /*!<  Register,  Address offset: 0x64 */
	__IO uint32_t CA32_DSS2;                              /*!<  Register,  Address offset: 0x68 */
	__IO uint32_t CA32_LPS_CTRL;                          /*!<  Register,  Address offset: 0x6C */
	__IO uint32_t CA32_SRAM_CTRL0;                        /*!<  Register,  Address offset: 0x70 */
	__IO uint32_t CA32_SRAM_CTRL1;                        /*!<  Register,  Address offset: 0x74 */
	__IO uint32_t CA32_SRAM_CTRL2;                        /*!<  Register,  Address offset: 0x78 */
	__IO uint32_t CA32_COOR_CTRL0;                        /*!<  Register,  Address offset: 0x7C */
	__IO uint32_t CA32_COOR_CTRL1;                        /*!<  Register,  Address offset: 0x80 */
	__I  uint32_t CA32_COOR_CTRL2;                        /*!<  Register,  Address offset: 0x84 */
	__I  uint32_t CA32_COOR_CTRL3;                        /*!<  Register,  Address offset: 0x88 */
	__I  uint32_t CA32_COOR_CTRL4;                        /*!<  Register,  Address offset: 0x8C */
	__IO uint32_t CA32_COOR_CTRL5;                        /*!<  Register,  Address offset: 0x90 */
	__IO uint32_t CA32_COOR_CTRL6;                        /*!<  Register,  Address offset: 0x94 */
	__IO uint32_t CA32_COOR_CTRL7;                        /*!<  Register,  Address offset: 0x98 */
	__I  uint32_t CA32_DIAG_RCV_DATA_0;                   /*!< DIAGNOSIS RECEIVE DATA BIT[31:0] Register,  Address offset: 0x9C */
	__I  uint32_t CA32_DIAG_RCV_DATA_1;                   /*!< DIAGNOSIS RECEIVE DATA BIT[63:32] Register,  Address offset: 0xA0 */
	__I  uint32_t CA32_DIAG_RCV_DATA_2;                   /*!< DIAGNOSIS RECEIVE DATA BIT[94:64] Register,  Address offset: 0xA4 */
	__IO uint32_t CA32_TOP_DSS0;                          /*!< CA7 TOP MODULE C30 DSS Register,  Address offset: 0xA8 */
	__IO uint32_t CA32_TOP_DSS1;                          /*!< CA7 TOP MODULE C35 DSS Register,  Address offset: 0xAC */
	__IO uint32_t CA32_TOP_DSS2;                          /*!< CA7 TOP MODULE C40 DSS Register,  Address offset: 0xB0 */
	__IO uint32_t CA32_TOP_DSS_DATA_IN_0;                 /*!<  Register,  Address offset: 0xB4 */
	__IO uint32_t CA32_TOP_DSS_DATA_IN_1;                 /*!<  Register,  Address offset: 0xB8 */
	__IO uint32_t CA32_TOP_DSS_DATA_IN_2;                 /*!<  Register,  Address offset: 0xBC */
	__IO uint32_t RSVD2[8];                               /*!<  Reserved,  Address offset:0xC0-0xDC */
	__IO uint32_t CA32_DUMMY_CTRL;                        /*!<  Register,  Address offset: 0xE0 */
} CA32_TypeDef;

#define CA32_BASE ((CA32_TypeDef *) (0x41000200))

#define CA32_STANDBYWFE_CORE0		((u32)0x00000001 << 16)
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* MANUAL_GEN_END */

#endif
