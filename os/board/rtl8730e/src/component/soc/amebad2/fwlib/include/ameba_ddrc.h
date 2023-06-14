#ifndef _AMEBAD2_DDRC_H_
#define _AMEBAD2_DDRC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */


/** @defgroup DDRC
  * @brief DDRC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup DDRC_Register_Definitions DDRC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup DDRC_CCR
 * @brief Configuration Control Register
 * @{
 **/
#define DDRC_BIT_CR_UPDATE              ((u32)0x00000001 << 31)          /*!<W 0x0  To update the related control registers. Control registers: DCR, IOCR, CSR, MR_INFO,and MRx.To update the registers dynamically, it only allows until all transitions are complete. Timing counter registers: TPR0, TPR1, TPR2, and TPR3. */
#define DDRC_CR_UPDATE(x)               ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_FLUSH_FIFO             ((u32)0x00000001 << 8)          /*!<R/W 0x1  Flush all FIFO in DRAMC. Write: 0: Not to flush FIFO 1: Start to Flush FIFO Read: 0: It is still active to flush FIFO. 1: Flushing FIFO operation is done. */
#define DDRC_FLUSH_FIFO(x)              ((u32)(((x) & 0x00000001) << 8))
#define DDRC_BIT_FRQC                   ((u32)0x00000001 << 5)          /*!<R/W 0x1  Set to implementation FRQC function. DRAMC would be automatically to implement serial operations and cooperate with RTK_PHY at the next Auto-refresh implementation finish. The operations include Self-refresh entry (SRE), to trigger RTK_PHY: Frequency change, wait until RTK_PHY: Frequency change done, set DRAM exit Self refresh mode (SRX) and re-initialize DRAM to set mode registers. The corresponding register group is according to CR_CSR.GP. Write: 0: Not to implement FRQC function. 1: Set FRQC function Read : 0: FRQC_function is still active. 1: FRQC function done. *Only support with configuration: FRQC_SUPPORT. */
#define DDRC_FRQC(x)                    ((u32)(((x) & 0x00000001) << 5))
#define DDRC_BIT_SRXT                   ((u32)0x00000001 << 4)          /*!<R/W 0x1  Set to implementation SREX function. DRAMC would be automatically to implement DRAM: Self- Refresh entry command (SRE) at the next Auto-Refresh implementation finishing. And DRAMC would issue DRAM: Self-Refresh exit command (SRX) if Read/Write data access to DRAM. CR_CSR.SRE_ST is indicated DRAMC still implement SRE function. Write: 0: Not to set AUTO_SREX function. 1: Set AUTO_SREX function Read : 0: AUTO_SREX function is still active. 1: Indicates AUTO_SREX function done (SREX_DONE). */
#define DDRC_SRXT(x)                    ((u32)(((x) & 0x00000001) << 4))
#define DDRC_BIT_DPIT                   ((u32)0x00000001 << 3)          /*!<R/W 0x1  Start to implement DPIN function. According to CR_CMD_DPIN//CR_CMD_DPIN_NDGE/CR_TIE_DPIN register value to set to DRAM pins. Write: 0: Not to set value to DRAM command pins. 1: Set value to DRAM pins. Read: 0: DPIN function is still active. 1: Indicates that DRAM command had issued to DRAM (DPIN_DONE). */
#define DDRC_DPIT(x)                    ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_BSTC                   ((u32)0x00000001 << 2)          /*!<R/W 0x1  Start to run BSTC function. Write: 0: Disable BSTC function 1: Run BSTC Function * Write enable only with BSTC module is configured. Read: 0: BSTC function is still active. 1: BSTC function is done (BSTC_DONE). */
#define DDRC_BSTC(x)                    ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_TM                     ((u32)0x00000001 << 1)          /*!<R/W 0x1  Start to run TM function. Write: 0: Disable TM function 1: Run TM Function * Write enable only with CR_CSR.TM_IDLE=0 is configured. Read: 0: TM function is still active. 1: TM function is done. */
#define DDRC_TM(x)                      ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_INIT                   ((u32)0x00000001 << 0)          /*!<R/W 0x0  Start to issue DRAM initialization sequence Write: 0: Disable DRAM initialization function 1: Enable DRAM initialization function Read: 0: DRAM initialization function is still active 1: DRAM initialization function is done (INIT_DONE). */
#define DDRC_INIT(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_DCR
 * @brief DRAM Configuration Control Register
 * @{
 **/
#define DDRC_BIT_ZQC                    ((u32)0x00000001 << 30)          /*!<R/W 0x0  Issue DRAM ZQCS/ZQCL command after serial Auto-refresh command implementation. This function should set with CR_DRR.ZQCL_INV and CR_TPR2.ZQCS_INV. Note: LPDDR4 does not support ZQCx command. */
#define DDRC_ZQC(x)                     ((u32)(((x) & 0x00000001) << 30))
#define DDRC_BIT_FULLWR                 ((u32)0x00000001 << 27)          /*!<R/W 0  Always Full Write hint to RXI316. RXI316 will recognize RMW when transaction is not aligned to dram atomic size or it's partial write. * Only available (can write, and be meaningful) when ECC_EN (/FULLWR_EN/DHC_STARK_EN) is defined. */
#define DDRC_FULLWR(x)                  ((u32)(((x) & 0x00000001) << 27))
#define DDRC_MASK_RMW_GRA               ((u32)0x00000003 << 24)          /*!<R/W 2'b10  RMW granularity to RXI316. 2'b00: 16-bit 2'b01: 32-bit 2'b10: 64-bit 2'b11: 128-bit * Only available when ECC_EN (/SEC_EN/FULLWR_EN/DHC_STARK_EN) is defined. */
#define DDRC_RMW_GRA(x)                 ((u32)(((x) & 0x00000003) << 24))
#define DDRC_GET_RMW_GRA(x)             ((u32)(((x >> 24) & 0x00000003)))
#define DDRC_BIT_BG_UNMAP               ((u32)0x00000001 << 22)          /*!<R/W 0x0  Disable to remap BG0 function to optimize DDR4:tCCD_L. CMD to CMD can be continuously in DDR4: burst access at BG remap function. BG remap function would cause DRAM active banks only 1/2 Bank number. For example, BANK0 and BANK4 would active with the same ROW address. 0: Enable BG0 remap 1: Disable BG0 remap * Only available (can write, and be meaningful) when DDR4_DRAM is defined. */
#define DDRC_BG_UNMAP(x)                ((u32)(((x) & 0x00000001) << 22))
#define DDRC_BIT_BG2                    ((u32)0x00000001 << 21)          /*!<R/W 0x0  Support BankGroup2 function for DDR4. *Only support with configuration: BG BITS = 2. */
#define DDRC_BG2(x)                     ((u32)(((x) & 0x00000001) << 21))
#define DDRC_BIT_GEAR_DOWN              ((u32)0x00000001 << 19)          /*!<R/W 0x0  Sets GEAR_DOWN function for DDR4 DRAM command and xDQ pins would be operated valid at 2 cycles. Before to implement this function, DRAM should set into Gear Down mode. */
#define DDRC_GEAR_DOWN(x)               ((u32)(((x) & 0x00000001) << 19))
#define DDRC_BIT_PAR                    ((u32)0x00000001 << 18)          /*!<R/W 0x0  Sets PAR function for DDR4 DRAM priority pin would execute with the related pins. Before to implement this function, DRAM should enable CA parity mode. */
#define DDRC_PAR(x)                     ((u32)(((x) & 0x00000001) << 18))
#define DDRC_BIT_RD_DBI                 ((u32)0x00000001 << 17)          /*!<R/W 0x0  Sets READ DBI function for DDR4/LPDDR4 If setting this function, DRAMC would invert DRAM read data if DM_n/DM is indicate to DBI. Before to implement this function, DRAM should set to enable Read DBI and Read latency should be added according to DRAM spec. */
#define DDRC_RD_DBI(x)                  ((u32)(((x) & 0x00000001) << 17))
#define DDRC_BIT_WR_DBI                 ((u32)0x00000001 << 16)          /*!<R/W 0x0  Sets Write DBI function for LPDDR4 (DDR4: WR_DBI only can be implemented at BSTC: WR without mask write). If setting this function, DRAMC would set DBI function at write data and write mask bits. Before to implement this function, DRAM should set to enable Write DBI. */
#define DDRC_WR_DBI(x)                  ((u32)(((x) & 0x00000001) << 16))
#define DDRC_BIT_RANK2                  ((u32)0x00000001 << 12)          /*!<R/W 0x0  Set DRAM RANK2 architecture implement. In RANK2 architecture, it should have 2 CS pins and Command/DQ bus is shared to connect with 2 DRAM dies. CR_TPR3.tCCD_R should be set for this function. *Only support with configuration: RANK_BITS = 2 */
#define DDRC_RANK2(x)                   ((u32)(((x) & 0x00000001) << 12))
#define DDRC_MASK_DFI_RATE              ((u32)0x00000007 << 8)          /*!<R/W 0x0  Sets DFI ratio. 3'b001: 1:1 3'b010: 2:1 Others: Reserved *If using SDR, it doesn't support DFI */
#define DDRC_DFI_RATE(x)                ((u32)(((x) & 0x00000007) << 8))
#define DDRC_GET_DFI_RATE(x)            ((u32)(((x >> 8) & 0x00000007)))
#define DDRC_BIT_QUAR_DQ                ((u32)0x00000001 << 7)          /*!<R/W 0x0  Setting the DDR architecture is Half of DQ_32. If setting QUAR_DQ, CR_DCR.{HYBR_DQ, HALF_DQ, DQ_DQ32} should set 0. *Only support with configuration: QUAR_DQ_EN. */
#define DDRC_QUAR_DQ(x)                 ((u32)(((x) & 0x00000001) << 7))
#define DDRC_BIT_HYBR_DQ                ((u32)0x00000001 << 6)          /*!<R/W 0x0  Indicates DRAM architecture is (Big+ Little) density. To access DRAM in DQx16-bit or DQx32-bit is according to CR_MISC.HYBR_SIZE. If setting HYBR_DQ, CR_DCR.{HALF_DQ, DQ32} should set 0. *Only support with configuration: HYBR_DQ. */
#define DDRC_HYBR_DQ(x)                 ((u32)(((x) & 0x00000001) << 6))
#define DDRC_BIT_HALF_DQ                ((u32)0x00000001 << 5)          /*!<R/W 0x0  Setting the DDR architecture is Half of DQ_32. If setting HALF_DQ, CR_DCR.{QUAR_DQ, HYBR_DQ, DQ_DQ32} should set 0. */
#define DDRC_HALF_DQ(x)                 ((u32)(((x) & 0x00000001) << 5))
#define DDRC_BIT_DQ_32                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  Sets the DDR architecture is 2 x 16-Bit DRAM. It is valid to set if the hardware architecture is valid configuration. If setting DQ_32, CR_DCR.{HYBR_DQ, HALF_DQ32} should be set to 0. */
#define DDRC_DQ_32(x)                   ((u32)(((x) & 0x00000001) << 4))
#define DDRC_MASK_DRAM_TYPE             ((u32)0x0000000F << 0)          /*!<R/W 0x0  Indicates the target DRAM type 4'b0001: DDR1 4'b0010: DDR2 4'b0011: DDR3 4'b0100: DDR4 4'b1001: LPDDR1 4'b1010: LPDDR2 4'b1011: LPDDR3 4'b1100: LPDDR4 Others: reserved */
#define DDRC_DRAM_TYPE(x)               ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_DRAM_TYPE(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_IOCR
 * @brief I/O Configuration Control Register
 * @{
 **/
#define DDRC_BIT_FPGA_DFI               ((u32)0x00000001 << 31)          /*!<R/W 0x0  Enable to connect external PHY on FPGA */
#define DDRC_FPGA_DFI(x)                ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_FPGA_LP                ((u32)0x00000001 << 30)          /*!<R/W 0x0  Enable to loopback mode form output DFI ports to input DFI ports on FPGA. */
#define DDRC_FPGA_LP(x)                 ((u32)(((x) & 0x00000001) << 30))
#define DDRC_BIT_AC_SHARE               ((u32)0x00000001 << 29)          /*!<R/W 0x0  Enable to share DFI interface: command and address with CH1: DRAMC. Ch0: DFI interface is only used DFI phase 0 (xxx_p0) Ch1: DFI interface is only used DFI phase 1 (xxx_p1) If enable this function, DRAM command is only to support 1T mode. */
#define DDRC_AC_SHARE(x)                ((u32)(((x) & 0x00000001) << 29))
#define DDRC_BIT_ADDR_SHARE             ((u32)0x00000001 << 28)          /*!<R/W 0x0  Enable to share DFI interface: address with CH1: DRAMC for LPDDR4 */
#define DDRC_ADDR_SHARE(x)              ((u32)(((x) & 0x00000001) << 28))
#define DDRC_BIT_DC_CH1                 ((u32)0x00000001 << 27)          /*!<R/W 0x0  Enable CH1: DRAMC clock for AC_SHARE, ADDR_SHARE or Dual Channel function. */
#define DDRC_DC_CH1(x)                  ((u32)(((x) & 0x00000001) << 27))
#define DDRC_MASK_TPHY_RDATA_EN         ((u32)0x0000001F << 20)          /*!<R/W 0x0  Sets the delay latency from DFI read command to dfi_rddata_en signal. If it works on DDR, the smallest value is 1. */
#define DDRC_TPHY_RDATA_EN(x)           ((u32)(((x) & 0x0000001F) << 20))
#define DDRC_GET_TPHY_RDATA_EN(x)       ((u32)(((x >> 20) & 0x0000001F)))
#define DDRC_MASK_TPHY_WLAT             ((u32)0x00000007 << 17)          /*!<R/W 0x0  Sets the delay latency from DFI write command to dfi_wrdata_en signal. If it works on DDR type, the smallest value is 0. */
#define DDRC_TPHY_WLAT(x)               ((u32)(((x) & 0x00000007) << 17))
#define DDRC_GET_TPHY_WLAT(x)           ((u32)(((x >> 17) & 0x00000007)))
#define DDRC_MASK_TPHY_WDATA            ((u32)0x0000001F << 12)          /*!<R/W 0x0  Sets the delay latency from dfi_wrdata_en signal to dfi_wddata. If it works on DDR type, the smallest value is 1. */
#define DDRC_TPHY_WDATA(x)              ((u32)(((x) & 0x0000001F) << 12))
#define DDRC_GET_TPHY_WDATA(x)          ((u32)(((x >> 12) & 0x0000001F)))
#define DDRC_MASK_RD_PIPE               ((u32)0x0000000F << 8)          /*!<R/W 0x0  Sets the extra delay latency of CAS on SDR. If it works on SDR, the smallest value is 0. */
#define DDRC_RD_PIPE(x)                 ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_RD_PIPE(x)             ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_BIT_DYN_SRE                ((u32)0x00000001 << 7)          /*!<R/W 0x0  Set DRAMC: dynamic self refresh. According to CR_DPERF0.DIW value, DRAMC will issue SREF automatically. */
#define DDRC_DYN_SRE(x)                 ((u32)(((x) & 0x00000001) << 7))
#define DDRC_BIT_HALF_CS_N              ((u32)0x00000001 << 6)          /*!<R/W 0x0  Sets DRAM:CS only keeps half cycle for LPDDR2/3. */
#define DDRC_HALF_CS_N(x)               ((u32)(((x) & 0x00000001) << 6))
#define DDRC_BIT_TRTW_2T_DIS            ((u32)0x00000001 << 5)          /*!<R/W 0x0  Sets DRAM: read -> write command interval time >= 2t. */
#define DDRC_TRTW_2T_DIS(x)             ((u32)(((x) & 0x00000001) << 5))
#define DDRC_BIT_2N_PRE_CMD             ((u32)0x00000001 << 4)          /*!<R/W 0x0  Sets DRAM: command pins = keeps 2t* but DRAM:CS keeps only 1t*. If DFI ratio is1, it only can be allowed setting to 0. If DFI ratio is 2, it would suggest setting to 1. The timing margin is better at DRAM. In LPDDR4, it doesn't support. */
#define DDRC_2N_PRE_CMD(x)              ((u32)(((x) & 0x00000001) << 4))
#define DDRC_BIT_STC_ODT                ((u32)0x00000001 << 3)          /*!<R/W 0x0  Set DRAM: ODT value is static according to CR_TIE_DPIN.ODT. Else DRAMC supports dynamic ODT function. */
#define DDRC_STC_ODT(x)                 ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_STC_CKE                ((u32)0x00000001 << 2)          /*!<R/W 0x0  Set DRAM: CKE value is static according to CR_TIE_DPIN.CKE. Else DRAMC supports dynamic CKE function. */
#define DDRC_STC_CKE(x)                 ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_CKE_NOP                ((u32)0x00000001 << 1)          /*!<R/W 0x1  Set before to implement DPIT function with CR_TIE_DPIN.CKE is 0 (Low power entry command) with 2 NOP CMD for LPDDR3. 0: No NOP CMD after low power entry 1: Follow with 2 NOP CMD for low power entry */
#define DDRC_CKE_NOP(x)                 ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_ODT_2PIN               ((u32)0x00000001 << 0)          /*!<R/W 0x1  Driver the same value to all ODT pins according to CR_TIE_DPIN.ODT. *Only support with configuration: ODT_BITS=2. */
#define DDRC_ODT_2PIN(x)                ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_CSR
 * @brief Controller Status Register
 * @{
 **/
#define DDRC_BIT_CR_GP                  ((u32)0x00000001 << 31)          /*!<R/W 0x0  Set to indicate control register group number.ForFRQC function, there are 2 group registers at GP0/GP1: (CR_IOCR, CR_DRR, CR_TPR0~3, and CR_MRx). Before setting the particular group number registers,usershould set this field to decide which group number is used to read/write. Anddifferent group numberof CR_IOCR, CR_DRR, CR_TPR0~3, and CR_MRx aresharethe same address mapping. *CR_GP:1 only supports with configuration: FRQC_SUPPORT. */
#define DDRC_CR_GP(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_CUR_GP                 ((u32)0x00000001 << 30)          /*!<R 0x0  Indicates the current group number which is internal used. CUR _GP is updated when FRQC is trigger and FRQC function is done. */
#define DDRC_CUR_GP(x)                  ((u32)(((x) & 0x00000001) << 30))
#define DDRC_MASK_DPIN_MOD              ((u32)0x00000007 << 16)          /*!<R/W 0x0  Indicate DPIN function: DRAM command type before DPIT implementation. Used to sync DRAMC: internal each bank page information. 3'b000: DRAM command, without data 3'b001: DRAM: MRR, used for LPDDR234 3'b010: DRAM: Read data 3'b011: DRAM: Write data 3'b100: DRAM: MPC Write, only used for LPDDR4 3'b101: DRAM: MPC Read, only used for LPDDR4 Others are reserved */
#define DDRC_DPIN_MOD(x)                ((u32)(((x) & 0x00000007) << 16))
#define DDRC_GET_DPIN_MOD(x)            ((u32)(((x >> 16) & 0x00000007)))
#define DDRC_BIT_BSTC_IDLE              ((u32)0x00000001 << 10)          /*!<R/W 0x1  Disable BSTC function state Write: To set BSTC idle state 0: Enable BSTC function 1: Set BSTC idle state; disable BSTC function. Read: To check if BSTC state is in idle 0: BSTC function implementationis allowed. 1: BSTC state is idle and user can't implement BSTC function. */
#define DDRC_BSTC_IDLE(x)               ((u32)(((x) & 0x00000001) << 10))
#define DDRC_BIT_TM_IDLE                ((u32)0x00000001 << 9)          /*!<R/W 0x1  Disable TM function state Write: To set TM idle state 0: Enable TM function 1: Set TM idle state; disable TM function. Read: To check if TM state is in idle 0: TM function implementations allowed. 1: TM state is idle and user can't implement TM function. */
#define DDRC_TM_IDLE(x)                 ((u32)(((x) & 0x00000001) << 9))
#define DDRC_BIT_MEM_IDLE               ((u32)0x00000001 << 8)          /*!<R/W 0x1  Disable memory access state Write: To set memory access idle state 0: Enable memory access state. 1: Set memory access idle; disable memory access. Read: To check ifmemory access stateis in idle 0: Access DRAM is allowed. 1: Memory state is idle and user can't access DRAM throughdata bus. */
#define DDRC_MEM_IDLE(x)                ((u32)(((x) & 0x00000001) << 8))
#define DDRC_BIT_SRE_ST                 ((u32)0x00000001 << 4)          /*!<R 0x0  Indicates DRAMC is issuing SRE command to DRAM. */
#define DDRC_SRE_ST(x)                  ((u32)(((x) & 0x00000001) << 4))
#define DDRC_MASK_DPIN_CMD_INFO         ((u32)0x00000003 << 0)          /*!<R/W 0x0  Set DRAM command with data type before DPIT implementation. 2'b00: Reserved 2'b01: DPIN precharge all 2'b10: DPIN precharge 2'b11: DPIN active */
#define DDRC_DPIN_CMD_INFO(x)           ((u32)(((x) & 0x00000003) << 0))
#define DDRC_GET_DPIN_CMD_INFO(x)       ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup DDRC_DRR
 * @brief DRAM Refresh Control Register
 * @{
 **/
#define DDRC_MASK_ZQCL_INV              ((u32)0x00000007 << 29)          /*!<R/W 0x0  It is used for indicate ZQCL implementation interval time after ZQCS implementation times. For example: set ZQC_INV to 2 and ZQCL to1 -----|||||------|||||+ZQCS -----|||||------|||||+ZQCL---- Serial-REF Serial-REF+ZQCS Serial-REF Serial-REF+ZQCL *LPDDR4 does not support ZQCx command. */
#define DDRC_ZQCL_INV(x)                ((u32)(((x) & 0x00000007) << 29))
#define DDRC_GET_ZQCL_INV(x)            ((u32)(((x >> 29) & 0x00000007)))
#define DDRC_BIT_REF_DIS                ((u32)0x00000001 << 28)          /*!<R/W 0x0  Disable DRAMC issues Refresh command to DRAM. */
#define DDRC_REF_DIS(x)                 ((u32)(((x) & 0x00000001) << 28))
#define DDRC_MASK_REF_NUM               ((u32)0x0000000F << 24)          /*!<R/W 0x8  Sets the number of Refresh commands to issue in between two surrounding Refresh commands */
#define DDRC_REF_NUM(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_REF_NUM(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_TREF                  ((u32)0x0000FFFF << 8)          /*!<R/W 0xf00  Maximum average Refresh commands delay cycles. CR_tREF[4:0] is also used to indicate the early cycles to inform RTK_PHY before ZQC command implementation. */
#define DDRC_TREF(x)                    ((u32)(((x) & 0x0000FFFF) << 8))
#define DDRC_GET_TREF(x)                ((u32)(((x >> 8) & 0x0000FFFF)))
#define DDRC_MASK_TRFC                  ((u32)0x000000FF << 0)          /*!<R/W 0x72  Refresh to Active or Refresh commands delay cycles. Total tRFC= {CR_TPR3.tRFC_MSB, CR_DRR[7:0].tRTW} */
#define DDRC_TRFC(x)                    ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_TRFC(x)                ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_TPR0
 * @brief DRAM Timing Parameter Register 0
 * @{
 **/
#define DDRC_MASK_TZQCS                 ((u32)0x000000FF << 24)          /*!<R/W 0x0  ZQCS command cycles */
#define DDRC_TZQCS(x)                   ((u32)(((x) & 0x000000FF) << 24))
#define DDRC_GET_TZQCS(x)               ((u32)(((x >> 24) & 0x000000FF)))
#define DDRC_BIT_TPD_EN                 ((u32)0x00000001 << 23)          /*!<R/W 0x0  Enable to keep CKE is registered 0 before CKE is registered to 1 for Precharge/Active power down. The timing is equal to CR_TPR0.tCKE. *Some DRAM spec defines thetimingtotCKE/tXP. *Not used this function in SDR/DDR1. */
#define DDRC_TPD_EN(x)                  ((u32)(((x) & 0x00000001) << 23))
#define DDRC_MASK_TCKE                  ((u32)0x0000003F << 17)          /*!<R/W 0x0  Enable to keep CKE is registered 1 before the next valid command for Precharge/Active power down exit. The timing = Max(tCKE,tXP) cycles For AUTO_SREX function, tCKE= {CR_TPR2.tNS, CR_TPR0.tCKE} else tCKE = {CR_TPR0.tCKE} */
#define DDRC_TCKE(x)                    ((u32)(((x) & 0x0000003F) << 17))
#define DDRC_GET_TCKE(x)                ((u32)(((x >> 17) & 0x0000003F)))
#define DDRC_MASK_TRTP                  ((u32)0x0000000F << 13)          /*!<R/W 0x3  Read to Precharge command delay cycles for Percharge command. The tRTP should be followed as Equation/DFI_RATIO. SDR: BL/2 DDR1: BL/2 DDR2: AL + BL/2 - 2 +tRTP DDR3/4: AL+tRTP LPDDR1: BL/2 LPDDR2: BL/2 - 1(S2)/2(S4) +tRTP LPDDR3: BL/2 - 4 +tRTP LPDDR4: BL/2 - 8 +tRTP BL: DRAM burst length */
#define DDRC_TRTP(x)                    ((u32)(((x) & 0x0000000F) << 13))
#define DDRC_GET_TRTP(x)                ((u32)(((x >> 13) & 0x0000000F)))
#define DDRC_MASK_TWR                   ((u32)0x0000000F << 9)          /*!<R/W 0x4  Write recovery delay cycles for Precharge command. Totalt WR= {CR_TPR3.tWR_MSB, CR_TPR0[12:9].tWR} */
#define DDRC_TWR(x)                     ((u32)(((x) & 0x0000000F) << 9))
#define DDRC_GET_TWR(x)                 ((u32)(((x >> 9) & 0x0000000F)))
#define DDRC_MASK_TRAS                  ((u32)0x0000001F << 4)          /*!<R/W 0x5  Active to Precharge command delay cycles totalt RAS= {CR_TPR3.tRAS_MSB, CR_TPR0[8:4].tRAS} */
#define DDRC_TRAS(x)                    ((u32)(((x) & 0x0000001F) << 4))
#define DDRC_GET_TRAS(x)                ((u32)(((x >> 4) & 0x0000001F)))
#define DDRC_MASK_TRP                   ((u32)0x0000000F << 0)          /*!<R/W 0x6  Precharge command cycles. tRP is also indicated to LPDDR234: tRPpb for signal perbank precharge time. */
#define DDRC_TRP(x)                     ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_TRP(x)                 ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_TPR1
 * @brief DRAM Timing Parameter Register 1
 * @{
 **/
#define DDRC_MASK_TFAW                  ((u32)0x0000001F << 24)          /*!<R/W 0x0  Four active window cycles total tFAW={CR_TPR3.tFAW_MSB, CR_TPR1[28:24].tFAW} */
#define DDRC_TFAW(x)                    ((u32)(((x) & 0x0000001F) << 24))
#define DDRC_GET_TFAW(x)                ((u32)(((x >> 24) & 0x0000001F)))
#define DDRC_MASK_TRTW                  ((u32)0x0000000F << 20)          /*!<R/W 0x0  Read to Write command delay cycles. The equation of tRTW: totalt RTW= {CR_TPR3.tRTW_MSB, CR_TPR1[23:20].tRTW} * Minimum value is 0. tRTW: SDR: BL+RL DDR1: BL/2+RL DDR2: BL/2 + 2 DDR3/4: RL + BL/2 + 2 - WL LPDDR1: BL/2 + RL LPDDR2: RL + RU(tDQSCKmax) + BL/2 + 1 -WL LPDDR3: RL + RU(tDQSCKmax) + BL/2 + 1 -WL LPDDR4: RL+RU(tDQSCKmax/tCK)+BL/2 - WL + RD(tRPST) + tWPRE * LPDDR4: Read parity latency should be added into RL. */
#define DDRC_TRTW(x)                    ((u32)(((x) & 0x0000000F) << 20))
#define DDRC_GET_TRTW(x)                ((u32)(((x >> 20) & 0x0000000F)))
#define DDRC_MASK_TWTR                  ((u32)0x00000007 << 17)          /*!<R/W 0x3  Write to Read command delay cycles In DDR4 tWTR_L = CR_TPR1.tWTR for the same Bank Group. The equation of tWTR: totaltWTR= {CR_TPR3.tWTR_MSB, CR_TPR1[19:17].tWTR} *Minima value is 0. */
#define DDRC_TWTR(x)                    ((u32)(((x) & 0x00000007) << 17))
#define DDRC_GET_TWTR(x)                ((u32)(((x >> 17) & 0x00000007)))
#define DDRC_MASK_TCCD                  ((u32)0x00000007 << 14)          /*!<R/W 0x1  CAS to CAS command delay cycles. CR_tPR1.tCCD = (tCCD/ DFI_RATIO) In LPDDR4,tCCDMW= CR_TPR1.tCCD * 2^2 cycles. In DDR4 tRRD_L = CR_TPR1.tCCD for the same Bank Group. */
#define DDRC_TCCD(x)                    ((u32)(((x) & 0x00000007) << 14))
#define DDRC_GET_TCCD(x)                ((u32)(((x >> 14) & 0x00000007)))
#define DDRC_MASK_TRCD                  ((u32)0x0000000F << 10)          /*!<R/W 0xf  Active to Read or Write delay cycles */
#define DDRC_TRCD(x)                    ((u32)(((x) & 0x0000000F) << 10))
#define DDRC_GET_TRCD(x)                ((u32)(((x >> 10) & 0x0000000F)))
#define DDRC_MASK_TRC                   ((u32)0x0000003F << 4)          /*!<R/W 0x8  Active to Active or Refresh command delay cycles for the same Bank. */
#define DDRC_TRC(x)                     ((u32)(((x) & 0x0000003F) << 4))
#define DDRC_GET_TRC(x)                 ((u32)(((x >> 4) & 0x0000003F)))
#define DDRC_MASK_TRRD                  ((u32)0x0000000F << 0)          /*!<R/W 0x2  Active to Active command delay cycles for the different Bank. In DDR4 tRRD_L = CR_TPR1.tRRD for the same Bank Group. */
#define DDRC_TRRD(x)                    ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_TRRD(x)                ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_TPR2
 * @brief DRAM Timing Parameter Register 2
 * @{
 **/
#define DDRC_MASK_RST_TUS               ((u32)0x000003FF << 22)          /*!<R/W 0x0  Set to keep Reset low at least times and it must be >1. The time=CR_TPR2.RST_tUS*210 < 220 cycles For DDR3, it is least to wait 200us For DDR4, it is least to wait 100ns For LPDDR2/3:tINIT5, it is least to wait 10us The time = CR_TPR2.tUS *211 < 221 cycles For LPDDR4:tINIT1, it is least to wait 200us *Other DRAM types don't used. *CR_TPR2.RST_tUS can't be 0 and can't equal to CR_TPR2.tUS. *Only support without configuration: CR_INTI_RMBIT */
#define DDRC_RST_TUS(x)                 ((u32)(((x) & 0x000003FF) << 22))
#define DDRC_GET_RST_TUS(x)             ((u32)(((x >> 22) & 0x000003FF)))
#define DDRC_MASK_TUS                   ((u32)0x000003FF << 12)          /*!<R/W 0x0  Set to wait CKE stable at least tUS ns. The time=CR_TPR2.tUS*210 < 220 cycles For SDR, it is least to wait 100us. For DDR1/2, it is least to wait 200us. For DDR3/4, it is least to wait 500us ForLPDDR1~3: tINIT3, it is least to wait 200us The time=CR_TPR2.tUS*211 < 221 cycles. For LPDDR4:tINIT3, it is least to wait 2ms. *CR_TPR2.tUS can't be0 can't equal to CR_TPR2.RST_tUS. *Only support without configuration: CR_INTI_RMBIT. */
#define DDRC_TUS(x)                     ((u32)(((x) & 0x000003FF) << 12))
#define DDRC_GET_TUS(x)                 ((u32)(((x >> 12) & 0x000003FF)))
#define DDRC_MASK_TNS                   ((u32)0x0000000F << 8)          /*!<R/W 0x0  Set to wait at least tNS ns. The time=CR_TPR2.tNS*27 cycles For SDR/DDR1/LPDDR1, it is least to wait 1tCK For DDR2, it is least to wait 400ns For DDR3/4, it is least to wait max (5tck, ~tRFC+10ns). For LPDDR2/3:tINIT1, itis least towait 100 ns The time >=CR_TPR2.tNS*28 cycles For LPDDR4:tINIT4, it is least to wait 2us AfterINIT_DONE,this is used the extend bits of CR_TPR0.tCKE for AUTO_SREX function. *CR_TPR2.tNS can't be 0. *If CR_TPR2.tNS[3] ==1, CR_TPR2.tNS[2:0] should be >= 1. *Only support without configuration: CR_INTI_RMBIT */
#define DDRC_TNS(x)                     ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_TNS(x)                 ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_TMRD_INIT_MRR         ((u32)0x0000000F << 4)          /*!<R/W 0x0  Mode register operations delay cycles. The value =max (tMRD, tMOD) 2: CR_TPR2[4].INIT_MRR (CR_TPR2.tMRD[0]): is also use to enable MRR checking mode status at LPDDR2/LPDDR3: initialization flow to save tINIT5 timing (~10 us). 3: When using lpddr4, needs to share this field [6:4] with timing CR_tescke of SRE function. */
#define DDRC_TMRD_INIT_MRR(x)           ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_TMRD_INIT_MRR(x)       ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_INIT_REF_NUM_ZQC_INV  ((u32)0x0000000F << 0)          /*!<R/W 0x0  Set the number of Refresh command to issue in DRAM (SDR/DDR1/DDR2) initialization flow. After DRAM initial flow, it isalsoused forindicate ZQCS implementation interval times after serial AUTO_Refresh implementation times. For example: set ZQC_INV to 2 and ZQCL to1 -----||||| -------|||||+ZQCS -------|||||--------|||||+ZQCL---- Serial-REF Serial-REF+ZQCS Serial-REF Serial-REF+ZQCL */
#define DDRC_INIT_REF_NUM_ZQC_INV(x)    ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_INIT_REF_NUM_ZQC_INV(x) ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_TPR3
 * @brief DRAM Timing Parameter Register 3
 * @{
 **/
#define DDRC_MASK_TCCD_K                ((u32)0x0000000F << 28)          /*!<R/W 0x0  CAS to CAS command extra delay cycles for different rank DRAM die. The different rank CAS->CAS latency = CR_TPR1.tCCD + CR_TPR1.tCCD_K */
#define DDRC_TCCD_K(x)                  ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_TCCD_K(x)              ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_TCCD_KW               ((u32)0x0000000F << 24)          /*!<R/W 0x0  WR to WR command extra delay cycles for different rank DRAM die. The different rank WR->WR latency = CR_TPR1.tCCD + CR_TPR1.tCCD_KW */
#define DDRC_TCCD_KW(x)                 ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_TCCD_KW(x)             ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_TRRD_S                ((u32)0x0000000F << 20)          /*!<R/W 0x0  Active to Active command delay cycles for the different bank group in DDR4. *CR_TPR1.tRRD is used for the same bank group in DDR4:tRRD_L */
#define DDRC_TRRD_S(x)                  ((u32)(((x) & 0x0000000F) << 20))
#define DDRC_GET_TRRD_S(x)              ((u32)(((x >> 20) & 0x0000000F)))
#define DDRC_MASK_TWTR_S                ((u32)0x00000007 << 17)          /*!<R/W 0x0  Delay cycles start of internal Write transaction to internal read command at different bank group in DDR4. *CR_TPR1.tWTR is used to the same bank group in DDR4.tWTR_L */
#define DDRC_TWTR_S(x)                  ((u32)(((x) & 0x00000007) << 17))
#define DDRC_GET_TWTR_S(x)              ((u32)(((x >> 17) & 0x00000007)))
#define DDRC_MASK_TCCD_S                ((u32)0x00000007 << 14)          /*!<R/W 0x0  CAS to CAS command delay cycles at different bank group in DDR4. *CR_TPR1.tCCD is used to the same bank group in DDR4.tCCD_L */
#define DDRC_TCCD_S(x)                  ((u32)(((x) & 0x00000007) << 14))
#define DDRC_GET_TCCD_S(x)              ((u32)(((x >> 14) & 0x00000007)))
#define DDRC_MASK_TPPD                  ((u32)0x00000003 << 12)          /*!<R/W 0x0  Different bank Precharge to Precharge command delay cycles for LPDDR4 */
#define DDRC_TPPD(x)                    ((u32)(((x) & 0x00000003) << 12))
#define DDRC_GET_TPPD(x)                ((u32)(((x >> 12) & 0x00000003)))
#define DDRC_BIT_TWR_MSB                ((u32)0x00000001 << 11)          /*!<R/W 0x0  Extend MSB of tWR cycles, total tWR = {CR_TPR3.tWR_MSB, CR_TP R0[12:9].tWR} */
#define DDRC_TWR_MSB(x)                 ((u32)(((x) & 0x00000001) << 11))
#define DDRC_BIT_TRFC_MSB               ((u32)0x00000001 << 10)          /*!<R/W 0x0  Extend MSB oftRFC cycles, total tRFC = {CR_TPR3.tRFC_MSB, CR_DRR[7:0].tRTW} */
#define DDRC_TRFC_MSB(x)                ((u32)(((x) & 0x00000001) << 10))
#define DDRC_BIT_TRTW_MSB               ((u32)0x00000001 << 9)          /*!<R/W 0x0  Extend MSBoftRTW cycles, total tRTW = {CR_TPR3.tRTW_MSB, CR_TPR1[23:20].tRTW} */
#define DDRC_TRTW_MSB(x)                ((u32)(((x) & 0x00000001) << 9))
#define DDRC_BIT_TWTR_MSB               ((u32)0x00000001 << 8)          /*!<R/W 0x0  Extend MSBoftWTR cycles, total tWTR = {CR_TPR3.tWTR_MSB, CR_TPR1[19:17].tWTR} */
#define DDRC_TWTR_MSB(x)                ((u32)(((x) & 0x00000001) << 8))
#define DDRC_BIT_TRCD_MSB               ((u32)0x00000001 << 7)          /*!<R/W 0x0  Extend MSBoftWTR cycles, total tRCD = {CR_TPR3.tRCD_MSB, CR_TPR1[13:10].tRCD} */
#define DDRC_TRCD_MSB(x)                ((u32)(((x) & 0x00000001) << 7))
#define DDRC_MASK_DPIN_CMD_LAT          ((u32)0x00000007 << 4)          /*!<R/W 0x0  Set CS_N pin delay cycles with others command pins. *It is usually CS_N to avoid setup/hold violation for DDR4 gear down function. */
#define DDRC_DPIN_CMD_LAT(x)            ((u32)(((x) & 0x00000007) << 4))
#define DDRC_GET_DPIN_CMD_LAT(x)        ((u32)(((x >> 4) & 0x00000007)))
#define DDRC_BIT_TFAW_MSB               ((u32)0x00000001 << 3)          /*!<R/W 0x0  Extend MSBoftFAW cycles. total tFAW = { CR_TPR3.tFAW_MSB, CR_TPR1[28:24].tFAW} */
#define DDRC_TFAW_MSB(x)                ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_TRAS_MSB               ((u32)0x00000001 << 2)          /*!<R/W 0x0  Extend MSBoftRAS cycles. total tRAS = {CR_TPR3.tRAS_MSB, CR_TPR0[8:4].tRAS} */
#define DDRC_TRAS_MSB(x)                ((u32)(((x) & 0x00000001) << 2))
#define DDRC_MASK_TRPAB_PB              ((u32)0x00000003 << 0)          /*!<R/W 0x0  Precharge all command- precharge command cycles tRPAB_PB=tRPab - tRPpb For LPDDR2/3/4, tRPab would different with tRPpb */
#define DDRC_TRPAB_PB(x)                ((u32)(((x) & 0x00000003) << 0))
#define DDRC_GET_TRPAB_PB(x)            ((u32)(((x >> 0) & 0x00000003)))
/** @} */

/** @defgroup DDRC_CMD_DPIN_NDGE
 * @brief Command Data Pin Register for LPDDR2/3
 * @{
 **/
#define DDRC_BIT_CS_N_1_NDGE            ((u32)0x00000001 << 31)          /*!<R/W 0x1  Set the value of CS_1 pin if parameter: CS_BITS=2 */
#define DDRC_CS_N_1_NDGE(x)             ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_PARITY_NDGE            ((u32)0x00000001 << 26)          /*!<R/W 0x0  Set the value of PARITY pin for DDR4 */
#define DDRC_PARITY_NDGE(x)             ((u32)(((x) & 0x00000001) << 26))
#define DDRC_BIT_ACT_N_NDGE             ((u32)0x00000001 << 25)          /*!<R/W 0x1  Set the value of ACT_N pin for DDR4 */
#define DDRC_ACT_N_NDGE(x)              ((u32)(((x) & 0x00000001) << 25))
#define DDRC_BIT_CS_N_NDGE              ((u32)0x00000001 << 24)          /*!<R/W 0x1  Set the value of CS_N pin */
#define DDRC_CS_N_NDGE(x)               ((u32)(((x) & 0x00000001) << 24))
#define DDRC_BIT_RAS_N_NDGE             ((u32)0x00000001 << 23)          /*!<R/W 0x1  Set the value of RAS_N pin */
#define DDRC_RAS_N_NDGE(x)              ((u32)(((x) & 0x00000001) << 23))
#define DDRC_BIT_CAS_N_NDGE             ((u32)0x00000001 << 22)          /*!<R/W 0x1  Set the value of CAS_N pin */
#define DDRC_CAS_N_NDGE(x)              ((u32)(((x) & 0x00000001) << 22))
#define DDRC_BIT_WE_N_NDGE              ((u32)0x00000001 << 21)          /*!<R/W 0x1  Set the value of WE_N pin */
#define DDRC_WE_N_NDGE(x)               ((u32)(((x) & 0x00000001) << 21))
#define DDRC_MASK_BA_NDGE               ((u32)0x0000000F << 17)          /*!<R/W 0x0  Set the value of BA pins */
#define DDRC_BA_NDGE(x)                 ((u32)(((x) & 0x0000000F) << 17))
#define DDRC_GET_BA_NDGE(x)             ((u32)(((x >> 17) & 0x0000000F)))
#define DDRC_MASK_ADDR_NDGE             ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Set the value of ADDR pins DRAM Active command: this is row address DRAM Write/Read command: this is column address */
#define DDRC_ADDR_NDGE(x)               ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_ADDR_NDGE(x)           ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_CMD_DPIN
 * @brief Command Data Pin Register
 * @{
 **/
#define DDRC_BIT_CS_N_1                 ((u32)0x00000001 << 31)          /*!<R/W 0x1  Set the value of CS_1 pin if parameter: CS_BITS=2 */
#define DDRC_CS_N_1(x)                  ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_PARITY                 ((u32)0x00000001 << 26)          /*!<R/W 0x0  Set the value of PARITY pin for DDR4 */
#define DDRC_PARITY(x)                  ((u32)(((x) & 0x00000001) << 26))
#define DDRC_BIT_ACT_N                  ((u32)0x00000001 << 25)          /*!<R/W 0x1  Set the value of ACT_N pin for DDR4 */
#define DDRC_ACT_N(x)                   ((u32)(((x) & 0x00000001) << 25))
#define DDRC_BIT_CS_N                   ((u32)0x00000001 << 24)          /*!<R/W 0x1  Set the value of CS_N pin */
#define DDRC_CS_N(x)                    ((u32)(((x) & 0x00000001) << 24))
#define DDRC_BIT_RAS_N                  ((u32)0x00000001 << 23)          /*!<R/W 0x1  Set the value of RAS_N pin */
#define DDRC_RAS_N(x)                   ((u32)(((x) & 0x00000001) << 23))
#define DDRC_BIT_CAS_N                  ((u32)0x00000001 << 22)          /*!<R/W 0x1  Set the value of CAS_N pin */
#define DDRC_CAS_N(x)                   ((u32)(((x) & 0x00000001) << 22))
#define DDRC_BIT_WE_N                   ((u32)0x00000001 << 21)          /*!<R/W 0x1  Set the value of WE_N pin */
#define DDRC_WE_N(x)                    ((u32)(((x) & 0x00000001) << 21))
#define DDRC_MASK_BA                    ((u32)0x0000000F << 17)          /*!<R/W 0x0  Set the value of BA pins */
#define DDRC_BA(x)                      ((u32)(((x) & 0x0000000F) << 17))
#define DDRC_GET_BA(x)                  ((u32)(((x >> 17) & 0x0000000F)))
#define DDRC_MASK_ADDR                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Set the value of ADDR pins DRAM Active command: this is row address DRAM Write/Read command: this is column address */
#define DDRC_ADDR(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_ADDR(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_TIE_DPIN
 * @brief Tied Data Pin Register
 * @{
 **/
#define DDRC_BIT_ODT_1                  ((u32)0x00000001 << 31)          /*!<R/W 0x0  Set the value of ODT_1 pin if parameter: ODT_BITS>1. If CR_DCR.STD_ODT setting, ODT_1 pin valuewould be set directly and immediately according to this field. */
#define DDRC_ODT_1(x)                   ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_CKE_1                  ((u32)0x00000001 << 30)          /*!<R/W 0x1  Set the value of CKE_1 pin directly and immediatelyaccording to this field if parameter CKE_BITS=2. If CR_DCR.STD_CKE setting, CKE_1 pin value would be according to this field and setting by DPIT function */
#define DDRC_CKE_1(x)                   ((u32)(((x) & 0x00000001) << 30))
#define DDRC_BIT_RST_N_1                ((u32)0x00000001 << 29)          /*!<R/W 0x1  Set the value of RST_N_1 pin directly and immediately according to this field if parameter RST_BITS=2. */
#define DDRC_RST_N_1(x)                 ((u32)(((x) & 0x00000001) << 29))
#define DDRC_BIT_ALERT_N                ((u32)0x00000001 << 4)          /*!<R/W 0x1  Set the value of ALERT_N pin. * Only available when enable DDR4_EN */
#define DDRC_ALERT_N(x)                 ((u32)(((x) & 0x00000001) << 4))
#define DDRC_BIT_TEN                    ((u32)0x00000001 << 3)          /*!<R/W 0x0  Set the value of TEN pin. * Only available when enable DDR4_EN */
#define DDRC_TEN(x)                     ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_ODT                    ((u32)0x00000001 << 2)          /*!<R/W 0x0  Set the value of ODT pin. If CR_DCR.STD_ODTsetting, ODTpin valuewould be set directly and immediately according to this field. */
#define DDRC_ODT(x)                     ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_CKE                    ((u32)0x00000001 << 1)          /*!<R/W 0x1  Set the value of CKE pin. If CR_DCR.STD_CKE setting, CKE pin value would be according to this field and setting by DPIT function. */
#define DDRC_CKE(x)                     ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_RST_N                  ((u32)0x00000001 << 0)          /*!<R/W 0x1  Set the value of RST_N pin directly and immediately according to this field. */
#define DDRC_RST_N(x)                   ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_MR_INFO
 * @brief Mode Latency Information Register
 * @{
 **/
#define DDRC_MASK_RTP                   ((u32)0x0000001F << 25)          /*!<R/W 0x0  It is DRAM mode register: write recoverary cyclesvaluefor AutoPrecharge command. It dependes on DRAM mode register definition. The equation is DRAM RTP value/(DFI_RATIO * DRAM_PERIOD) */
#define DDRC_RTP(x)                     ((u32)(((x) & 0x0000001F) << 25))
#define DDRC_GET_RTP(x)                 ((u32)(((x >> 25) & 0x0000001F)))
#define DDRC_MASK_NWR                   ((u32)0x0000001F << 20)          /*!<R/W 0x0  It is DRAM mode register: write recoverary cyclesvaluefor AutoPrecharge command. It dependes on DRAM moderegister definition. The equation is: DRAM write recovery value/(DFI_RATIO * DRAM_PERIOD) */
#define DDRC_NWR(x)                     ((u32)(((x) & 0x0000001F) << 20))
#define DDRC_GET_NWR(x)                 ((u32)(((x >> 20) & 0x0000001F)))
#define DDRC_MASK_T_DQSCK_MAX           ((u32)0x0000001F << 15)          /*!<R/W 0x0  Indicates tDQSCK_MAX cycle value for LPDDR23 The equation is (tDQSCK_MAX/ DFI_RATIO * DRAM_PERIOD) */
#define DDRC_T_DQSCK_MAX(x)             ((u32)(((x) & 0x0000001F) << 15))
#define DDRC_GET_T_DQSCK_MAX(x)         ((u32)(((x >> 15) & 0x0000001F)))
#define DDRC_MASK_ADD_LAT               ((u32)0x0000001F << 10)          /*!<R/W 0x0  Indicates DRAM additive latency. */
#define DDRC_ADD_LAT(x)                 ((u32)(((x) & 0x0000001F) << 10))
#define DDRC_GET_ADD_LAT(x)             ((u32)(((x >> 10) & 0x0000001F)))
#define DDRC_MASK_RD_LAT                ((u32)0x0000001F << 5)          /*!<R/W 0x0  Indicates DRAM read data latency. The equation is (Read latency + Additive latency +*Parity latency(DDR4))/DFI_RATIO */
#define DDRC_RD_LAT(x)                  ((u32)(((x) & 0x0000001F) << 5))
#define DDRC_GET_RD_LAT(x)              ((u32)(((x >> 5) & 0x0000001F)))
#define DDRC_MASK_WR_LAT                ((u32)0x0000001F << 0)          /*!<R/W 0x0  Indicates DRAM write data latency. The equation is (Write latency+ Additive latency +*Parity latency(DDR4))/DFI_RATIO */
#define DDRC_WR_LAT(x)                  ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_WR_LAT(x)              ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_MR0
 * @brief DRAM Mode Register 0
 * @{
 **/
#define DDRC_MASK_MR0                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0aa2  Sets the value to Mode register MR0 in DRAM initialization flow (INIT function). */
#define DDRC_MR0(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR0(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR1
 * @brief DRAM Mode Register 1
 * @{
 **/
#define DDRC_MASK_MR1                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR1 in DRAM initialization flow (INIT function). */
#define DDRC_MR1(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR1(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR2
 * @brief DRAM Mode Register 2
 * @{
 **/
#define DDRC_MASK_MR2                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR2 in DRAM initialization flow (INIT function). */
#define DDRC_MR2(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR2(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR3
 * @brief DRAM Mode Register 3
 * @{
 **/
#define DDRC_MASK_MR3                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR3 in DRAM initialization flow (INIT function). */
#define DDRC_MR3(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR3(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR4
 * @brief DRAM Mode Register 4
 * @{
 **/
#define DDRC_MASK_MR4                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR4 in DDR4 initialization flow (INIT function). */
#define DDRC_MR4(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR4(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR5
 * @brief DRAM Mode Register 5
 * @{
 **/
#define DDRC_MASK_MR5                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x400  Sets the value to Mode register MR5 in DDR4 initialization flow (INIT function). */
#define DDRC_MR5(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR5(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR6
 * @brief DRAM Mode Register 6
 * @{
 **/
#define DDRC_MASK_MR6                   ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR6 in DDR4 initialization flow (INIT function). */
#define DDRC_MR6(x)                     ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR6(x)                 ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR11
 * @brief DRAM Mode Register 11
 * @{
 **/
#define DDRC_MASK_MR11                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR11 in LPDDR4 initialization flow (INIT function). */
#define DDRC_MR11(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR11(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR12
 * @brief DRAM Mode Register 12
 * @{
 **/
#define DDRC_MASK_MR12                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR12 in LPDDR4 initialization flow (INIT function). */
#define DDRC_MR12(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR12(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR13
 * @brief DRAM Mode Register 13
 * @{
 **/
#define DDRC_MASK_MR13                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR13 in LPDDR4initialization flow (INIT function). MR13 is special used to set for LPDDR4: Frequency Set Point function to refer INIT function. */
#define DDRC_MR13(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR13(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR14
 * @brief DRAM Mode Register 14
 * @{
 **/
#define DDRC_MASK_MR14                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR14 in LPDDR4 initialization flow (INIT function). */
#define DDRC_MR14(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR14(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_MR22
 * @brief DRAM Mode Register 22
 * @{
 **/
#define DDRC_MASK_MR22                  ((u32)0x0001FFFF << 0)          /*!<R/W 0x0  Sets the value to Mode register MR22 in LPDDR4 initialization flow (INIT function). */
#define DDRC_MR22(x)                    ((u32)(((x) & 0x0001FFFF) << 0))
#define DDRC_GET_MR22(x)                ((u32)(((x >> 0) & 0x0001FFFF)))
/** @} */

/** @defgroup DDRC_TPR4
 * @brief DRAM Timing Parameter Register 4
 * @{
 **/
#define DDRC_MASK_TRFCPB                ((u32)0x000000FF << 8)          /*!<R/W 0x0  Sets the value to REFpb to next valid command extra delay cycles for refresh per bank function Only lpddr2, lpddr3, lpddr4 support REFpb commaned If setting CR_tRFCpb=0, it won't enable refpb function */
#define DDRC_TRFCPB(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define DDRC_GET_TRFCPB(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define DDRC_BIT_TRP_MSB                ((u32)0x00000001 << 0)          /*!<R/W 0x0  Extend MSB of tRP cycles. total tRP = {CR_TPR4.tRP_MSB, CR_TPR0[3:0].tRP} */
#define DDRC_TRP_MSB(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_TPR5
 * @brief DRAM Timing Parameter Register 5
 * @{
 **/
#define DDRC_MASK_ODT_DLY               ((u32)0x00000003 << 8)          /*!<R/W 0x0  Sets ODT on delay time when DRAM command is Write operation. Time unit: DRAM cycles Suggestion: 0 */
#define DDRC_ODT_DLY(x)                 ((u32)(((x) & 0x00000003) << 8))
#define DDRC_GET_ODT_DLY(x)             ((u32)(((x >> 8) & 0x00000003)))
#define DDRC_MASK_ODT_HOLD              ((u32)0x0000001F << 0)          /*!<R/W 0x0  To keep ODT on time after ODT delay time. Time unit: DRAM cycles Suggestion: DDR2: (WL+ DRAM burst length /2 + 1) DDR3/4: (DRAM burst length /2 + 2) LPDDR3: (WL+ DRAM burst length /2 + 1) */
#define DDRC_ODT_HOLD(x)                ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_ODT_HOLD(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_DPDRI
 * @brief In DPIN data flow, set value of DPDR index
 * @{
 **/
#define DDRC_MASK_DPDRI                 ((u32)0x0000007F << 0)          /*!<R/W 0x0  It is used to set the value of DPDR index in DPIN data flow. Index 0~ 31: DPIN read data Index 32~ 63: DPIN write data Index 64~ 67: DPIN write data mask Index 68: DPIN read data level(word, read only) Index 69: DPIN write data level(word, read only) */
#define DDRC_DPDRI(x)                   ((u32)(((x) & 0x0000007F) << 0))
#define DDRC_GET_DPDRI(x)               ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup DDRC_DPDR
 * @brief In DPIN data flow, setvalueaccording to DPDRI
 * @{
 **/
#define DDRC_MASK_DPDR                  ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  It is used to write/read data according to CR_DPDRI. Write: the data of CR_DPDR would write into CR_DPDRI index address. Read: the data of CR_DPDR would read from CR_DPDRI index address. */
#define DDRC_DPDR(x)                    ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DPDR(x)                ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_ISR
 * @brief Controller Interrupt Register
 * @{
 **/
#define DDRC_MASK_INTR_ID               ((u32)0x000000FF << 16)          /*!<R/W 0x0  Interrupt ID Number, include Assertion Group and Assertion Number {Assertion Group, Assertion Number}. Bit[23:22] indicates Assertion Group. Bit[21:16] indicates Assertion Number. */
#define DDRC_INTR_ID(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_INTR_ID(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_BIT_WAR_HZD                ((u32)0x00000001 << 2)          /*!<R/W 0x0  Write after read (war) hazard interrupt. */
#define DDRC_WAR_HZD(x)                 ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_ADDR_OVER_SIZE         ((u32)0x00000001 << 1)          /*!<R/W 0x0  This field is working when not define ERR_HANDLER_EN If not define ERR_HANDLER_EN, Command address exceeds maximum DRAM size (CR_DRAM_SIZE) interrupt. If define ERR_HANDLER_EN, this interrupt would not active. */
#define DDRC_ADDR_OVER_SIZE(x)          ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_ALERT                  ((u32)0x00000001 << 0)          /*!<R/W 0x0  DDR4 alert interrupt. */
#define DDRC_ALERT(x)                   ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_IMR
 * @brief DRAMC interrupt mask bits
 * @{
 **/
#define DDRC_MASK_INTR_MASK             ((u32)0x0000FFFF << 0)          /*!<R/W 0xffff  CR_ISR [15:0] interrupt bit mask. 0: Interrupt bit is masked. 1: Interrupt bit enable. */
#define DDRC_INTR_MASK(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_INTR_MASK(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_BCR
 * @brief BSTC Control Register
 * @{
 **/
#define DDRC_MASK_LOOP_CNT              ((u32)0x000000FF << 16)          /*!<R/W 0x0  If set LOOP_CNT >0 and enable CR_BCR.LOOP, BSTC test patterns would be tested in LOOP_CNT times. */
#define DDRC_LOOP_CNT(x)                ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_LOOP_CNT(x)            ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_RELOAD_SRAM           ((u32)0x00000007 << 12)          /*!<R/W 0x0  RELOAD_SRAM[2]: Reload to keep the previous RG_SRAM level RELOAD_SRAM[1]: Reload to keep the previous WD_SRAM level RELOAD_SRAM[0]: Reload to keep the previous CMD_SRAM level */
#define DDRC_RELOAD_SRAM(x)             ((u32)(((x) & 0x00000007) << 12))
#define DDRC_GET_RELOAD_SRAM(x)         ((u32)(((x >> 12) & 0x00000007)))
#define DDRC_BIT_FUS_RD                 ((u32)0x00000001 << 11)          /*!<R/W 0x0  Flush RD_SRAM data. */
#define DDRC_FUS_RD(x)                  ((u32)(((x) & 0x00000001) << 11))
#define DDRC_BIT_FUS_RG                 ((u32)0x00000001 << 10)          /*!<R/W 0x0  Flush RG_SRAM data. */
#define DDRC_FUS_RG(x)                  ((u32)(((x) & 0x00000001) << 10))
#define DDRC_BIT_FUS_WD                 ((u32)0x00000001 << 9)          /*!<R/W 0x0  Flush WD_SRAM data. */
#define DDRC_FUS_WD(x)                  ((u32)(((x) & 0x00000001) << 9))
#define DDRC_BIT_FUS_CMD                ((u32)0x00000001 << 8)          /*!<R/W 0x0  Flush CMD_SRAM data. */
#define DDRC_FUS_CMD(x)                 ((u32)(((x) & 0x00000001) << 8))
#define DDRC_BIT_HW_PAT                 ((u32)0x00000001 << 7)          /*!<R/W 0x0  Indicates WD/RG patterns are generated by hardware design. Before BSTC function implementation user must set this field and BST.WD_EX_CNT then write a dummy data to WD_SRAM to load the expected number to internal WD counter. If Write data is from inWR_SRAM, internal WD counter is updated by WR_SRAM writing. */
#define DDRC_HW_PAT(x)                  ((u32)(((x) & 0x00000001) << 7))
#define DDRC_BIT_CRR                    ((u32)0x00000001 << 5)          /*!<W 0x0  Sets to clean CR_BER and CR_BST.{ERR_CNT, ERR_FST_TH} registers */
#define DDRC_CRR(x)                     ((u32)(((x) & 0x00000001) << 5))
#define DDRC_BIT_AT_ERR_STOP            ((u32)0x00000001 << 4)          /*!<R/W 0x0  DRAMC would stop the BSTC function implementation if RD_SRAM and RG_SRAM compare error data automatically. To enable this function it also must enable the loop mode and compare mode (CR_BCR[2:1]). */
#define DDRC_AT_ERR_STOP(x)             ((u32)(((x) & 0x00000001) << 4))
#define DDRC_BIT_DIS_MSK                ((u32)0x00000001 << 3)          /*!<R/W 0x0  Disables using MSK_SRAM. DRAMC would write full data to DRAM without masking function. */
#define DDRC_DIS_MSK(x)                 ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_LOOP                   ((u32)0x00000001 << 2)          /*!<R/W 0x0  Enables loop mode. DRAMC repeats to implement BSTC function for overnight testing */
#define DDRC_LOOP(x)                    ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_CMP                    ((u32)0x00000001 << 1)          /*!<R/W 0x0  Enables compare mode. DRAMC would compare read data form DRAM with the expected data in RG_SRAM. */
#define DDRC_CMP(x)                     ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_STOP                   ((u32)0x00000001 << 0)          /*!<W 0x0  Disables to run BSTC hardware function. This bit read data is reserved to 0. */
#define DDRC_STOP(x)                    ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_BECC
 * @brief BSTC Sideband ECC Register
 * @{
 **/
#define DDRC_MASK_ECC_ERR_BIT           ((u32)0x000000FF << 16)          /*!<R 0x0  Indicates which bit ever had occurred error in auto compare function. It can clear by CR_BCR.CRR. *When using sideband ecc function, it will needs additional DQ8 width. If ECC_EN configuration is not set, this field are no used. */
#define DDRC_ECC_ERR_BIT(x)             ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_ECC_ERR_BIT(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_ECC_CMP_MBIT          ((u32)0x000000FF << 0)          /*!<R/W 0x0  It is used to mask data bit to compare in auto compare function. *When using sideband ecc function, it will needs additional DQ8 width. If ECC_EN configuration is not set, this field aren't valid to set. */
#define DDRC_ECC_CMP_MBIT(x)            ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_ECC_CMP_MBIT(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_BCM
 * @brief BSTC compare bit mask Register
 * @{
 **/
#define DDRC_MASK_BCM_CMP_MBIT          ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  It is used to mask data bit to compare in auto compare function. *If DQ32 configuration is not set, it is only 16 bit valid to set. */
#define DDRC_BCM_CMP_MBIT(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_BCM_CMP_MBIT(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_BST
 * @brief BSTC Status Register
 * @{
 **/
#define DDRC_MASK_ERR_CNT               ((u32)0x000003FF << 16)          /*!<R 0x0  Indicates the entry number of the error read data if compare mode is setting. It can clear by CR_BCR.CRR. For example: if all return data are error BUS_64, DRAM BST8, DRAM x16, 2x DRAM Read command ERR_CNT max value=2x (8x16)/64 = 4 BUS_128, DRAM BST8, DRAM x16 ERR_CNT max value=2x(8x16)/128 = 2 BUS_128, DRAM BST8, DRAM x32 ERR_CNT max value=2x(8x32)/128 = 4 */
#define DDRC_ERR_CNT(x)                 ((u32)(((x) & 0x000003FF) << 16))
#define DDRC_GET_ERR_CNT(x)             ((u32)(((x >> 16) & 0x000003FF)))
#define DDRC_BIT_RD_IN_ST               ((u32)0x00000001 << 15)          /*!<R 0x0  Indicates DRAMC had received read data from DRAM. It can clear by BSTC_START function. */
#define DDRC_RD_IN_ST(x)                ((u32)(((x) & 0x00000001) << 15))
#define DDRC_MASK_ERR_FST_TH            ((u32)0x000003FF << 0)          /*!<R 0x0  Indicates the address of the first error data in the RD_SRAM. It can clear by CR_BCR.CRR. */
#define DDRC_ERR_FST_TH(x)              ((u32)(((x) & 0x000003FF) << 0))
#define DDRC_GET_ERR_FST_TH(x)          ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup DDRC_BSRAM0
 * @brief BSTC SRAM Status Register 0
 * @{
 **/
#define DDRC_MASK_WD_CNT                ((u32)0x000003FF << 16)          /*!<R CONF  Indicates the word counts of the data in the write data SRAM (WD_SRAM) For example: There is one valid entry at WD_SRAM. If DATA BUS Width is 64-bit, WD_CNT is 2 If DATA BUS Width is 128-bit, WD_CNT is 4 */
#define DDRC_WD_CNT(x)                  ((u32)(((x) & 0x000003FF) << 16))
#define DDRC_GET_WD_CNT(x)              ((u32)(((x >> 16) & 0x000003FF)))
#define DDRC_MASK_CMD_CNT               ((u32)0x000003FF << 0)          /*!<R CONF  Indicates the word counts of the data in the command SRAM (CMD_SRAM) For example, there is one valid entry at CMD_SRAM (CMD_SRAM bit width is 37) CMD_CNT = CMD_SRAM bit width/32 = 2 */
#define DDRC_CMD_CNT(x)                 ((u32)(((x) & 0x000003FF) << 0))
#define DDRC_GET_CMD_CNT(x)             ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup DDRC_BSRAM1
 * @brief BSTC SRAM Status Register 1
 * @{
 **/
#define DDRC_MASK_RD_CNT                ((u32)0x000003FF << 16)          /*!<R CONF  Indicates the word counts of the data in the read data (RD_SRAM). For example: There is one valid entry at RD_SRAM. If DATA BUS Width is 64-bit, RD_CNT is 2 If DATA BUS Width is 128-bit, RD_CNT is 4 */
#define DDRC_RD_CNT(x)                  ((u32)(((x) & 0x000003FF) << 16))
#define DDRC_GET_RD_CNT(x)              ((u32)(((x >> 16) & 0x000003FF)))
#define DDRC_MASK_RG_CNT                ((u32)0x000003FF << 0)          /*!<R CONF  Indicates the word counts of the data in the golden data SRAM (RG_SRAM) For example: There is one valid entry at RD_SRAM. If DATA BUS Width is 64-bit, RG_CNT is 2 If DATA BUS Width is 128-bit, RG_CNT is 4 */
#define DDRC_RG_CNT(x)                  ((u32)(((x) & 0x000003FF) << 0))
#define DDRC_GET_RG_CNT(x)              ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup DDRC_BER
 * @brief BSTC Error Bit Register
 * @{
 **/
#define DDRC_MASK_ERR_BIT               ((u32)0xFFFFFFFF << 0)          /*!<R 0x0  Indicates which bit ever had occurred error in auto compare function. It can clear by CR_BCR.CRR. *If DQ32 configuration is not set,it is only 16 bit valid to set. */
#define DDRC_ERR_BIT(x)                 ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_ERR_BIT(x)             ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_BEXPC
 * @brief BSTC SRAM expected execute numbers of words register
 * @{
 **/
#define DDRC_MASK_RD_EXPC_CNT           ((u32)0x000003FF << 21)          /*!<R/W 0x0  Sets the expected word number of RD_SRAM. The equation is (total return bits) / 32. For example: 4 DRAM Read commands with DQ16, DRAM_BST8 RD_EXPC_CNT=(4x16x8)/32 = 16 4 DRAM Read commands with DQ32 & DRAM_BST8 RD_EXPC_CNT=(4x32x8)/32 =32 */
#define DDRC_RD_EXPC_CNT(x)             ((u32)(((x) & 0x000003FF) << 21))
#define DDRC_GET_RD_EXPC_CNT(x)         ((u32)(((x >> 21) & 0x000003FF)))
#define DDRC_MASK_WD_EXPC_CNT           ((u32)0x000003FF << 10)          /*!<R/W 0x0  Sets the expected word number of WD_SRAM. The expected word number should equal all write command issue WD number. The equation is (total return bits) / 32. For example: Write commands with DQ16, DRAM_BST8 WD_EXPC_CNT=(4x16x8)/32 = 16 Write commands with DQ32 & DRAM_BST8 WD_EXPC_CNT=(4x32x8)/32 =32 */
#define DDRC_WD_EXPC_CNT(x)             ((u32)(((x) & 0x000003FF) << 10))
#define DDRC_GET_WD_EXPC_CNT(x)         ((u32)(((x >> 10) & 0x000003FF)))
#define DDRC_MASK_CMD_EXPC_CNT          ((u32)0x000003FF << 0)          /*!<R/W 0x0  Sets the expected word number of CMD_SRAM. The equation is (total cmd bits) / 32. For example: 1 command for CMD_SRAM with 37 bits CMD_EXPC_CNT=(1x37)/32 = 2 1 command for CMD_SRAM with 31 bits CMD_EXPC_CNT=(1x31)/32 = 1 */
#define DDRC_CMD_EXPC_CNT(x)            ((u32)(((x) & 0x000003FF) << 0))
#define DDRC_GET_CMD_EXPC_CNT(x)        ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup DDRC_SVN_ID
 * @brief PCTL Version ID Register
 * @{
 **/
#define DDRC_MASK_GIT_NUM               ((u32)0x0000FFFF << 16)          /*!<R CONF  Indicates the commit number of the released RTL code */
#define DDRC_GIT_NUM(x)                 ((u32)(((x) & 0x0000FFFF) << 16))
#define DDRC_GET_GIT_NUM(x)             ((u32)(((x >> 16) & 0x0000FFFF)))
#define DDRC_MASK_RELEASE_DATE          ((u32)0x0000FFFF << 0)          /*!<R CONF  Indicates the IP release date for the particular project */
#define DDRC_RELEASE_DATE(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_RELEASE_DATE(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_IDR
 * @brief PCTL Identification Register
 * @{
 **/
#define DDRC_MASK_CR_VER                ((u32)0x0000FFFF << 16)          /*!<R 0x0  The control register version number */
#define DDRC_CR_VER(x)                  ((u32)(((x) & 0x0000FFFF) << 16))
#define DDRC_GET_CR_VER(x)              ((u32)(((x >> 16) & 0x0000FFFF)))
#define DDRC_MASK_PCTL_DEF              ((u32)0x0000FFFF << 0)          /*!<R CONF  If user ties this field at input port tie, it indicates user definition. Or it only shows one main support DRAM type. */
#define DDRC_PCTL_DEF(x)                ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_PCTL_DEF(x)            ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_MSG
 * @brief Reserved register
 * @{
 **/
#define DDRC_MASK_MSG                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  This is reversed register for DRAMC debug. */
#define DDRC_MSG(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_MSG(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_DPERF0
 * @brief DRAM Performance Regitser
 * @{
 **/
#define DDRC_MASK_PHYC_REF_NUM          ((u32)0x00000007 << 24)          /*!<R/W 0x0  Indicates an refab command number. After PHYC_INV interval, it needs to sending numbers of refab cmd for PHY calibration using. It is used for refpb function. It also needs to be used with PHYC_INV. If any value of PHYC_INV or PHYC_REF_NUM is 0, phy calibration function for refpb couldn't be enabled. */
#define DDRC_PHYC_REF_NUM(x)            ((u32)(((x) & 0x00000007) << 24))
#define DDRC_GET_PHYC_REF_NUM(x)        ((u32)(((x >> 24) & 0x00000007)))
#define DDRC_MASK_PHYC_INV              ((u32)0x000000FF << 16)          /*!<R/W 0x0  Indicates an interval. It needs to wait numbers of refpb cmd to DRAM, before start sending refab cmd for PHY calibration. It is used for refpb function. It also needs to be used with PHYC_REF_NUM. If any value of PHYC_INV or PHYC_REF_NUM is 0, phy calibration function for refpb couldn't be enabled. */
#define DDRC_PHYC_INV(x)                ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_PHYC_INV(x)            ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_DIW                   ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  Indicates cycles for a DRAM idle window. Means it has n cycles no any RD/WR cmd from bus and also have no command to send dram. If user has set this value, DRAMC is used to pull-in REF command, include REFab and REFpb cmd. If user has set this value and setting CR_IOCR.DYN_SRE, DRAMC will pull-in REF command and issue SREF command automatically. */
#define DDRC_DIW(x)                     ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_DIW(x)                 ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_MISC
 * @brief Memory Information Setting Control Register
 * @{
 **/
#define DDRC_BIT_ONE_BANK_EN            ((u32)0x00000001 << 30)          /*!<R/W 0x0  Indicates DRAM is only one bank and internal row address counter would be increasing if crossing page size. */
#define DDRC_ONE_BANK_EN(x)             ((u32)(((x) & 0x00000001) << 30))
#define DDRC_MASK_HYBR_SIZE             ((u32)0x00000003 << 10)          /*!<R/W 0x0  Indicates the target DRAM page size (unit: Byte): 2'b000: 256M 2'b001: 512M 2'b010: 1G 2'b011: 2G According to CR_MISC. HYBR_SIZE to access DRAM in DQx16-bit or DQx32-bit. *It needs hardware configuration set HYBR_DRAM. For example: Setting HYBR_SIZE is 512MB, If bus_address >= 0x2000_0000, it only accesses DQx16-bit. If bus_address < 0x2000_0000, it would access DQx32-bit. */
#define DDRC_HYBR_SIZE(x)               ((u32)(((x) & 0x00000003) << 10))
#define DDRC_GET_HYBR_SIZE(x)           ((u32)(((x >> 10) & 0x00000003)))
#define DDRC_MASK_BST_SIZE              ((u32)0x00000007 << 6)          /*!<R/W 0x1  Indicates DRAMC to issue DRAM Burst size: 3'b000a4 3'b001: 8 3'010: 16 3'011: 2 and others are reserved. */
#define DDRC_BST_SIZE(x)                ((u32)(((x) & 0x00000007) << 6))
#define DDRC_GET_BST_SIZE(x)            ((u32)(((x >> 6) & 0x00000007)))
#define DDRC_MASK_BANK_SIZE             ((u32)0x00000003 << 4)          /*!<R/W 0x2  Indicates the target DRAM bank size*(CR_DCR.RANK2+1) 2'b00: 2 2'b01: 4 2'b10: 8 2'b11: 16 */
#define DDRC_BANK_SIZE(x)               ((u32)(((x) & 0x00000003) << 4))
#define DDRC_GET_BANK_SIZE(x)           ((u32)(((x >> 4) & 0x00000003)))
#define DDRC_MASK_PAGE_SIZE             ((u32)0x0000000F << 0)          /*!<R/W 0x3  Indicates the target DRAM page size (unit: Byte) 4'b000: 256B 4'b001: 512B 4'b010: 1K 4'b011: 2K 4'b100: 4K 4'b101: 8K 4'b110: 16K 4'b101: 32K For example: If DRAM column address is 10-bit and DRAM bit number is x16, page size is equal to 210×16/8 => 2KB If DRAM column address is 10-bit and DRAM bit number is x32, page size is equal to 210×32/8 => 4KB */
#define DDRC_PAGE_SIZE(x)               ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_PAGE_SIZE(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_CHOP_SIZE
 * @brief ChopSize Control Register
 * @{
 **/
#define DDRC_MASK_CR_CHOP_SIZE          ((u32)0x00000007 << 0)          /*!<R/W*3 0x1  It determine the following items: chopsize to front-end.the space mapping between each entry of wcmd_buf and wdata_sram of wdata_buf.the bits of range when hazard check. Chop size (unit:byte): 3'd0: 8 3'd1: 16 3'd2: 32 3'd3: 64 3'd4: 128 *note 1: CR_CHOP_SIZE must be greater than or equal to dram_atomic_size. (dram_atomic_size= dram_dq_width * dram_burst_length) *note 2 : CR_CHOP_SIZE must be greater than or equal to data bus_width(DC_DATA_WIDTH) example: If DC_DATA_WIDTH=128-bit, CR_CHOP_SIZE=0 (8byte) is not available. *note 3: Max. value of CR_CHOP_SIZE depends on the config DC_MAX_CHOP_SIZE in config form. (e.g., If DC_MAX_CHOP_SIZE=32 (byte), CR_CHOP_SIZE can only set 0(8byte), 1(16byte), 2(32byte) ) */
#define DDRC_CR_CHOP_SIZE(x)            ((u32)(((x) & 0x00000007) << 0))
#define DDRC_GET_CR_CHOP_SIZE(x)        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup DDRC_DRAM_SIZE
 * @brief DRAM Size Control Register
 * @{
 **/
#define DDRC_MASK_DRAM_SIZE             ((u32)0xFFFFFFFF << 0)          /*!<R/W 0xffffffff  DRAM max density, physical address range is 0 ~ dram_size. If master cmd address over dram_size, frontend will block this cmd not send to backend, and send ERROR response. Also frontend interrupt will asserted for this illegal access. DRAM total density, for example: 32Gb: 0xFFFF_FFFF 24Gb: 0xBFFF_FFFF (non-binary dram density ) 16Gb: 0x7FFF_FFFF 12Gb: 0x5FFF_FFFF (non-binary dram density ) 8Gb: 0x3FFF_FFFF 6Gb: 0x2FFF_FFFF (non-binary dram density ) 4Gb: 0x1FFF_FFFF 3Gb: 0x1EFF_FFFF (non-binary dram density ) 2Gb: 0x0FFF_FFFF */
#define DDRC_DRAM_SIZE(x)               ((u32)(((x) & 0xFFFFFFFF) << 0))
#define DDRC_GET_DRAM_SIZE(x)           ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDRC_DSIZE_UP_4GB
 * @brief DRAM Size Up 4GB Control Register
 * @{
 **/
#define DDRC_BIT_DSIZE_8GB              ((u32)0x00000001 << 0)          /*!<R/W 1'b0  DRAM maximum access address, it is physical bus address. DRAM max size 8GB */
#define DDRC_DSIZE_8GB(x)               ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_BUS_WRAP_IDR
 * @brief BUS_WRAP ID Number Control Register
 * @{
 **/
#define DDRC_MASK_CR_WRAP_IDR           ((u32)0x0000FFFF << 16)          /*!<R 0x1  Indicates BUS_WRAP ID number. */
#define DDRC_CR_WRAP_IDR(x)             ((u32)(((x) & 0x0000FFFF) << 16))
#define DDRC_GET_CR_WRAP_IDR(x)         ((u32)(((x >> 16) & 0x0000FFFF)))
#define DDRC_MASK_CR_WRAP_DEF           ((u32)0x0000FFFF << 0)          /*!<R 0x316  Indicates DRAMC number */
#define DDRC_CR_WRAP_DEF(x)             ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_CR_WRAP_DEF(x)         ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_BUS_WRAP_SVN
 * @brief BUS_WRAP Version Control Register
 * @{
 **/
#define DDRC_MASK_SVN_NUM               ((u32)0x0000FFFF << 16)          /*!<R CONF  Indicates the commit number of the release RTL code */
#define DDRC_SVN_NUM(x)                 ((u32)(((x) & 0x0000FFFF) << 16))
#define DDRC_GET_SVN_NUM(x)             ((u32)(((x >> 16) & 0x0000FFFF)))
#define DDRC_MASK_RELEASE_DATE          ((u32)0x0000FFFF << 0)          /*!<R CONF  Indicates the IP release date for the particular project */
#define DDRC_RELEASE_DATE(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_RELEASE_DATE(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_ICG_EN
 * @brief Clock gating enable control register
 * @{
 **/
#define DDRC_BIT_CGATE_EN_SRAM          ((u32)0x00000001 << 31)          /*!<R/W 1'b1  frontend & backend SRAM clock gating enable (except BSTC) */
#define DDRC_CGATE_EN_SRAM(x)           ((u32)(((x) & 0x00000001) << 31))
#define DDRC_BIT_CGATE_EN_PROF          ((u32)0x00000001 << 30)          /*!<R/W 1'b1  chx_dc_prof_event clock gating enable */
#define DDRC_CGATE_EN_PROF(x)           ((u32)(((x) & 0x00000001) << 30))
#define DDRC_BIT_CGATE_EN_BSTC          ((u32)0x00000001 << 15)          /*!<R/W 1'b1  bstc module clock gating enable */
#define DDRC_CGATE_EN_BSTC(x)           ((u32)(((x) & 0x00000001) << 15))
#define DDRC_BIT_CGATE_EN_DPIN          ((u32)0x00000001 << 14)          /*!<R/W 1'b1  dpin module clock gating enable */
#define DDRC_CGATE_EN_DPIN(x)           ((u32)(((x) & 0x00000001) << 14))
#define DDRC_BIT_CGATE_EN_SREF          ((u32)0x00000001 << 13)          /*!<R/W 1'b1  self_ref module clock gating enable */
#define DDRC_CGATE_EN_SREF(x)           ((u32)(((x) & 0x00000001) << 13))
#define DDRC_BIT_CGATE_EN_REF           ((u32)0x00000001 << 12)          /*!<R/W 1'b1  ref_unit module clock gating enable */
#define DDRC_CGATE_EN_REF(x)            ((u32)(((x) & 0x00000001) << 12))
#define DDRC_BIT_CGATE_EN_INIT          ((u32)0x00000001 << 11)          /*!<R/W 1'b1  init_unit module clock gating enable */
#define DDRC_CGATE_EN_INIT(x)           ((u32)(((x) & 0x00000001) << 11))
#define DDRC_BIT_CGATE_EN_TIC           ((u32)0x00000001 << 10)          /*!<R/W 1'b1  tic module clock gating enable */
#define DDRC_CGATE_EN_TIC(x)            ((u32)(((x) & 0x00000001) << 10))
#define DDRC_BIT_CGATE_EN_DC            ((u32)0x00000001 << 9)          /*!<R/W 1'b1  data_channel module clock gating enable */
#define DDRC_CGATE_EN_DC(x)             ((u32)(((x) & 0x00000001) << 9))
#define DDRC_BIT_CGATE_EN_DCKE          ((u32)0x00000001 << 8)          /*!<R/W 1'b1  cmd_sch, cmd_dispatch_block module clock gating enable */
#define DDRC_CGATE_EN_DCKE(x)           ((u32)(((x) & 0x00000001) << 8))
#define DDRC_BIT_CGATE_EN_PCR           ((u32)0x00000001 << 7)          /*!<R/W 1'b1  protocal control register clock gating enable */
#define DDRC_CGATE_EN_PCR(x)            ((u32)(((x) & 0x00000001) << 7))
#define DDRC_BIT_CGATE_EN_QOS           ((u32)0x00000001 << 6)          /*!<R/W 1'b1  cmd_qos module clock gating enable */
#define DDRC_CGATE_EN_QOS(x)            ((u32)(((x) & 0x00000001) << 6))
#define DDRC_BIT_CGATE_EN_RDB           ((u32)0x00000001 << 5)          /*!<R/W 1'b1  rdata_buf module clock gating enable */
#define DDRC_CGATE_EN_RDB(x)            ((u32)(((x) & 0x00000001) << 5))
#define DDRC_BIT_CGATE_EN_WDB           ((u32)0x00000001 << 4)          /*!<R/W 1'b1  wdata_buf module clock gating enable */
#define DDRC_CGATE_EN_WDB(x)            ((u32)(((x) & 0x00000001) << 4))
#define DDRC_BIT_CGATE_EN_WCB           ((u32)0x00000001 << 3)          /*!<R/W 1'b1  wcmd_buf module clock gating enable */
#define DDRC_CGATE_EN_WCB(x)            ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_CGATE_EN_RCB           ((u32)0x00000001 << 2)          /*!<R/W 1'b1  rcmd_buf module clock gating enable */
#define DDRC_CGATE_EN_RCB(x)            ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_CGATE_EN_CR            ((u32)0x00000001 << 1)          /*!<R/W 1'b1  cr_reg module clock gating enable */
#define DDRC_CGATE_EN_CR(x)             ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_CGATE_EN_BW            ((u32)0x00000001 << 0)          /*!<R/W 1'b1  bus_wrap module clock gating enable */
#define DDRC_CGATE_EN_BW(x)             ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_ROW_REMAP0
 * @brief DRAM row address remap control register 0, remap row address bit 0 ~ bit 7
 * @{
 **/
#define DDRC_MASK_ROW7_REMAP            ((u32)0x0000000F << 28)          /*!<R/W 4'd8  BFO = 12, mapping to BUS_ADDR_BIT = BFO+ROW7_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW7_REMAP(x)              ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_ROW7_REMAP(x)          ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_ROW6_REMAP            ((u32)0x0000000F << 24)          /*!<R/W 4'd8  BFO = 11, mapping to BUS_ADDR_BIT = BFO+ROW6_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW6_REMAP(x)              ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_ROW6_REMAP(x)          ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_ROW5_REMAP            ((u32)0x0000000F << 20)          /*!<R/W 4'd8  BFO = 10, mapping to BUS_ADDR_BIT = BFO+ROW5_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW5_REMAP(x)              ((u32)(((x) & 0x0000000F) << 20))
#define DDRC_GET_ROW5_REMAP(x)          ((u32)(((x >> 20) & 0x0000000F)))
#define DDRC_MASK_ROW4_REMAP            ((u32)0x0000000F << 16)          /*!<R/W 4'd8  BFO = 9, mapping to BUS_ADDR_BIT = BFO+ROW4_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW4_REMAP(x)              ((u32)(((x) & 0x0000000F) << 16))
#define DDRC_GET_ROW4_REMAP(x)          ((u32)(((x >> 16) & 0x0000000F)))
#define DDRC_MASK_ROW3_REMAP            ((u32)0x0000000F << 12)          /*!<R/W 4'd8  BFO = 8, mapping to BUS_ADDR_BIT = BFO+ROW3_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW3_REMAP(x)              ((u32)(((x) & 0x0000000F) << 12))
#define DDRC_GET_ROW3_REMAP(x)          ((u32)(((x >> 12) & 0x0000000F)))
#define DDRC_MASK_ROW2_REMAP            ((u32)0x0000000F << 8)          /*!<R/W 4'd8  BFO = 7, mapping to BUS_ADDR_BIT = BFO+ROW2_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW2_REMAP(x)              ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_ROW2_REMAP(x)          ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_ROW1_REMAP            ((u32)0x0000000F << 4)          /*!<R/W 4'd8  BFO = 6, mapping to BUS_ADDR_BIT = BFO+ROW1_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW1_REMAP(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_ROW1_REMAP(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_ROW0_REMAP            ((u32)0x0000000F << 0)          /*!<R/W 4'd8  BFO = 5, mapping to BUS_ADDR_BIT = BFO+ROW0_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW0_REMAP(x)              ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_ROW0_REMAP(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_ROW_REMAP1
 * @brief DRAM row address remap control register 1, remap row address bit 8 ~ bit 15
 * @{
 **/
#define DDRC_MASK_ROW15_REMAP           ((u32)0x0000000F << 28)          /*!<R/W 4'd8  BFO = 20, mapping to BUS_ADDR_BIT = BFO+ROW15_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW15_REMAP(x)             ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_ROW15_REMAP(x)         ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_ROW14_REMAP           ((u32)0x0000000F << 24)          /*!<R/W 4'd8  BFO = 19, mapping to BUS_ADDR_BIT = BFO+ROW14_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW14_REMAP(x)             ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_ROW14_REMAP(x)         ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_ROW13_REMAP           ((u32)0x0000000F << 20)          /*!<R/W 4'd8  BFO = 18, mapping to BUS_ADDR_BIT = BFO+ROW13_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW13_REMAP(x)             ((u32)(((x) & 0x0000000F) << 20))
#define DDRC_GET_ROW13_REMAP(x)         ((u32)(((x >> 20) & 0x0000000F)))
#define DDRC_MASK_ROW12_REMAP           ((u32)0x0000000F << 16)          /*!<R/W 4'd8  BFO = 17, mapping to BUS_ADDR_BIT = BFO+ROW12_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW12_REMAP(x)             ((u32)(((x) & 0x0000000F) << 16))
#define DDRC_GET_ROW12_REMAP(x)         ((u32)(((x >> 16) & 0x0000000F)))
#define DDRC_MASK_ROW11_REMAP           ((u32)0x0000000F << 12)          /*!<R/W 4'd8  BFO = 16, mapping to BUS_ADDR_BIT = BFO+ROW11_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW11_REMAP(x)             ((u32)(((x) & 0x0000000F) << 12))
#define DDRC_GET_ROW11_REMAP(x)         ((u32)(((x >> 12) & 0x0000000F)))
#define DDRC_MASK_ROW10_REMAP           ((u32)0x0000000F << 8)          /*!<R/W 4'd8  BFO = 15, mapping to BUS_ADDR_BIT = BFO+ROW10_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW10_REMAP(x)             ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_ROW10_REMAP(x)         ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_ROW9_REMAP            ((u32)0x0000000F << 4)          /*!<R/W 4'd8  BFO = 14, mapping to BUS_ADDR_BIT = BFO+ROW9_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW9_REMAP(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_ROW9_REMAP(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_ROW8_REMAP            ((u32)0x0000000F << 0)          /*!<R/W 4'd8  BFO = 13, mapping to BUS_ADDR_BIT = BFO+ROW8_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW8_REMAP(x)              ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_ROW8_REMAP(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_ROW_REMAP2
 * @brief DRAM row address remap control register 2, remap row address bit 16
 * @{
 **/
#define DDRC_MASK_ROW16_REMAP           ((u32)0x0000000F << 0)          /*!<R/W 4'd8  BFO = 21, mapping to BUS_ADDR_BIT = BFO+ROW16_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_ROW16_REMAP(x)             ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_ROW16_REMAP(x)         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_BANK_REMAP0
 * @brief DRAM bank address remap control register 0, remap bank address bit 0 ~ bit 2
 * @{
 **/
#define DDRC_MASK_BA2_REMAP             ((u32)0x0000001F << 10)          /*!<R/W 5'd10  BFO = 2, mapping to BUS_ADDR_BIT = BFO+BA2_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_BA2_REMAP(x)               ((u32)(((x) & 0x0000001F) << 10))
#define DDRC_GET_BA2_REMAP(x)           ((u32)(((x >> 10) & 0x0000001F)))
#define DDRC_MASK_BA1_REMAP             ((u32)0x0000001F << 5)          /*!<R/W 5'd9  BFO = 2, mapping to BUS_ADDR_BIT = BFO+BA1_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_BA1_REMAP(x)               ((u32)(((x) & 0x0000001F) << 5))
#define DDRC_GET_BA1_REMAP(x)           ((u32)(((x >> 5) & 0x0000001F)))
#define DDRC_MASK_BA0_REMAP             ((u32)0x0000001F << 0)          /*!<R/W 5'd8  BFO = 2, mapping to BUS_ADDR_BIT = BFO+BA0_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_BA0_REMAP(x)               ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_BA0_REMAP(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_COLU_REMAP0
 * @brief DRAM clouaddress remap control register 0, remap clouaddress bit 2 ~bit 9
 * @{
 **/
#define DDRC_MASK_COL9_REMAP            ((u32)0x0000000F << 28)          /*!<R/W 4'd0  BFO = 9, mapping to BUS_ADDR_BIT = BFO+COL9_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL9_REMAP(x)              ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_COL9_REMAP(x)          ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_COL8_REMAP            ((u32)0x0000000F << 24)          /*!<R/W 4'd0  BFO = 8, mapping to BUS_ADDR_BIT = BFO+COL8_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL8_REMAP(x)              ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_COL8_REMAP(x)          ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_COL7_REMAP            ((u32)0x0000000F << 20)          /*!<R/W 4'd0  BFO = 7, mapping to BUS_ADDR_BIT = BFO+COL7_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL7_REMAP(x)              ((u32)(((x) & 0x0000000F) << 20))
#define DDRC_GET_COL7_REMAP(x)          ((u32)(((x >> 20) & 0x0000000F)))
#define DDRC_MASK_COL6_REMAP            ((u32)0x0000000F << 16)          /*!<R/W 4'd0  BFO = 6, mapping to BUS_ADDR_BIT = BFO+COL6_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL6_REMAP(x)              ((u32)(((x) & 0x0000000F) << 16))
#define DDRC_GET_COL6_REMAP(x)          ((u32)(((x >> 16) & 0x0000000F)))
#define DDRC_MASK_COL5_REMAP            ((u32)0x0000000F << 12)          /*!<R/W 4'd0  BFO = 5, mapping to BUS_ADDR_BIT = BFO+COL5_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL5_REMAP(x)              ((u32)(((x) & 0x0000000F) << 12))
#define DDRC_GET_COL5_REMAP(x)          ((u32)(((x >> 12) & 0x0000000F)))
#define DDRC_MASK_COL4_REMAP            ((u32)0x0000000F << 8)          /*!<R/W 4'd0  BFO = 4, mapping to BUS_ADDR_BIT = BFO+COL4_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL4_REMAP(x)              ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_COL4_REMAP(x)          ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_MASK_COL3_REMAP            ((u32)0x0000000F << 4)          /*!<R/W 4'd0  BFO = 3, mapping to BUS_ADDR_BIT = BFO+COL3_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL3_REMAP(x)              ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_COL3_REMAP(x)          ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_COL2_REMAP            ((u32)0x0000000F << 0)          /*!<R/W 4'd0  BFO = 2, mapping to BUS_ADDR_BIT = BFO+COL2_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_COL2_REMAP(x)              ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_COL2_REMAP(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_COLU_REMAP1
 * @brief DRAM clouaddress remap control register 1, remap clouaddress bit 10
 * @{
 **/
#define DDRC_MASK_COL11_REMAP           ((u32)0x0000000F << 4)          /*!<R/W 4'd15  BFO = 11, mapping to BUS_ADDR_BIT = BFO+COL11_REMAP +log2(dq/8), if unused mapping to 0 colu11_remap used for LPDDR3, column max to 12 bits */
#define DDRC_COL11_REMAP(x)             ((u32)(((x) & 0x0000000F) << 4))
#define DDRC_GET_COL11_REMAP(x)         ((u32)(((x >> 4) & 0x0000000F)))
#define DDRC_MASK_COL10_REMAP           ((u32)0x0000000F << 0)          /*!<R/W 4'd15  BFO = 10, mapping to BUS_ADDR_BIT = BFO+COL10_REMAP +log2(dq/8), if unused mapping to 0 colu10_remap used for LPDDR3, column max to 11 bits */
#define DDRC_COL10_REMAP(x)             ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_COL10_REMAP(x)         ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup DDRC_RANK_REMAP0
 * @brief DRAM rank address remap control register 0, remap rank address bit 0
 * @{
 **/
#define DDRC_MASK_RA_REMAP              ((u32)0x0000001F << 0)          /*!<R/W 5'd11  BFO = 2, mapping to BUS_ADDR_BIT = BFO+RA_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_RA_REMAP(x)                ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_RA_REMAP(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_CH_REMAP0
 * @brief DRAM channel address remap control register 0, remap channel address bit 0
 * @{
 **/
#define DDRC_MASK_CH_REMAP              ((u32)0x0000001F << 0)          /*!<R/W 5'd25  BFO = 2, mapping to BUS_ADDR_BIT = BFO+CH_REMAP +log2(dq/8), if unused mapping to 0 */
#define DDRC_CH_REMAP(x)                ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_CH_REMAP(x)            ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_BG_REMAP0
 * @brief DRAM bgaddress remap control register 0, remap bgaddress bit 0 ~bit1
 * @{
 **/
#define DDRC_MASK_BG1_REMAP             ((u32)0x0000001F << 5)          /*!<R/W 5'd11  BFO = 2, mapping to BUS_ADDR_BIT = BFO+BG1_REMAP+log2(dq/8), if unused mapping to 0 */
#define DDRC_BG1_REMAP(x)               ((u32)(((x) & 0x0000001F) << 5))
#define DDRC_GET_BG1_REMAP(x)           ((u32)(((x >> 5) & 0x0000001F)))
#define DDRC_MASK_BG0_REMAP             ((u32)0x0000001F << 0)          /*!<R/W 5'd10  BFO = 2, mapping to BUS_ADDR_BIT = BFO+BG0_REMAP+log2(dq/8), if unused mapping to 0 */
#define DDRC_BG0_REMAP(x)               ((u32)(((x) & 0x0000001F) << 0))
#define DDRC_GET_BG0_REMAP(x)           ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DDRC_ADDR_REMAP_CTRL
 * @brief NONBINARY dram density: 3/6/12/24Gb
 * @{
 **/
#define DDRC_MASK_NONB_DSIZE            ((u32)0x00000007 << 0)          /*!<R/W 3'd0  For LPDDR2/3 non-binary dram density (3/6/12/24Gb), only half of the row address space is valid. When the row[MSB]=1, the row[MSB-1] must be LOW. But now DRAM_SIZE(0x230) is used to set the max density. DRAM_SIZE can be non-binary size. So ADDR_REMAP_CTRL (0x424) would be not necessary for non-binary dram density. If physical address > dram_size, it is illegal DRAM access, frontend will block this cmd not send to backend, and send ERROR response. Also frontend interrupt will asserted for this illegal access. 3'd0: Not support non-binary dram density 3'd1: No dram density present at address with row[13]=row[12]=1 3'd2: No dram density present at address with row[14]=row[13]=1 3'd3: No dram density present at address with row[15]=row[14]=1 3'd4: No dram density present at address with row[16]=row[15]=1 3'd5~3'd7: Reserverd */
#define DDRC_NONB_DSIZE(x)              ((u32)(((x) & 0x00000007) << 0))
#define DDRC_GET_NONB_DSIZE(x)          ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup DDRC_SLOT_PARK
 * @brief Slot parking control register
 * @{
 **/
#define DDRC_MASK_IDLE_WINOW            ((u32)0x0000FFFF << 16)          /*!<R/W 16'h40  If current slot is no commands and current slot is not perfer parking slot, to wait IDLE_WINDOW cyclesand turn aroundslot if current slot is still no commands. */
#define DDRC_IDLE_WINOW(x)              ((u32)(((x) & 0x0000FFFF) << 16))
#define DDRC_GET_IDLE_WINOW(x)          ((u32)(((x >> 16) & 0x0000FFFF)))
#define DDRC_BIT_PERF_PARK              ((u32)0x00000001 << 0)          /*!<R/W 0  0: Read slot 1: Write slot */
#define DDRC_PERF_PARK(x)               ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_SLOT_STARVE
 * @brief Slot starvation control register
 * @{
 **/
#define DDRC_MASK_BASE_STARVE_CYC       ((u32)0x000000FF << 24)          /*!<R/W 8'hFF  This is basic starvation counter cycles for LPR_STARVE_CYC, HPR_STARVE_CYC and LPW_STARVE_CYC */
#define DDRC_BASE_STARVE_CYC(x)         ((u32)(((x) & 0x000000FF) << 24))
#define DDRC_GET_BASE_STARVE_CYC(x)     ((u32)(((x >> 24) & 0x000000FF)))
#define DDRC_MASK_LPR_STARVE_CYC        ((u32)0x000000FF << 16)          /*!<R/W 8'h2  The timeout of LPR slot would be set when LPR commans is no servicing in the interval time = (LPR_STARVE_CYC*BASE_STARVE_CYC) */
#define DDRC_LPR_STARVE_CYC(x)          ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_LPR_STARVE_CYC(x)      ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_HPR_STARVE_CYC        ((u32)0x000000FF << 8)          /*!<R/W 8'h1  The timeout of HPR slot would be set when HPR commans is no servicing in the interval time = (HPR_STARVE_CYC*BASE_STARVE_CYC) */
#define DDRC_HPR_STARVE_CYC(x)          ((u32)(((x) & 0x000000FF) << 8))
#define DDRC_GET_HPR_STARVE_CYC(x)      ((u32)(((x >> 8) & 0x000000FF)))
#define DDRC_MASK_LPW_STARVE_CYC        ((u32)0x000000FF << 0)          /*!<R/W 8'h4  The timeout of LPW slot would be set when HPR commans is no servicing in the interval time = (LPW_STARVE_CYC*BASE_STARVE_CYC) */
#define DDRC_LPW_STARVE_CYC(x)          ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_LPW_STARVE_CYC(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_SLOT_CNTR
 * @brief Slot constrain register
 * @{
 **/
#define DDRC_MASK_LPR_ISU_LMT           ((u32)0x000000FF << 16)          /*!<R/W 8'h20  This is limited number to issue LPR commands when LPR slot is at urgent state. */
#define DDRC_LPR_ISU_LMT(x)             ((u32)(((x) & 0x000000FF) << 16))
#define DDRC_GET_LPR_ISU_LMT(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define DDRC_MASK_HPR_ISU_LMT           ((u32)0x000000FF << 8)          /*!<R/W 8'h10  This is limited number to issue HPR commands when HPR slot is at urgent state. */
#define DDRC_HPR_ISU_LMT(x)             ((u32)(((x) & 0x000000FF) << 8))
#define DDRC_GET_HPR_ISU_LMT(x)         ((u32)(((x >> 8) & 0x000000FF)))
#define DDRC_MASK_LPW_ISU_LMT           ((u32)0x000000FF << 0)          /*!<R/W 8'h20  This is limited number to issue LPW commands when LPW slot is at urgent state. */
#define DDRC_LPW_ISU_LMT(x)             ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_LPW_ISU_LMT(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_EXP_CTRL
 * @brief Expire control register
 * @{
 **/
#define DDRC_MASK_EXP_BASE              ((u32)0x000000FF << 0)          /*!<R/W 8'h1  Used for expire function, exp_base value count down to 1, global_exp_timer+1 */
#define DDRC_EXP_BASE(x)                ((u32)(((x) & 0x000000FF) << 0))
#define DDRC_GET_EXP_BASE(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDRC_HZD_CTRL0
 * @brief Hazard priority control register 0
 * @{
 **/
#define DDRC_MASK_RHZD_THD              ((u32)0x0000001F << 8)          /*!<R/W 0  Used to constrain read hazard priority. When RHZD_THD setting and read command number >= RHZD_THD, it would keep the original read hazard priority at command QoS. If the value of RHZD_THD is 0, it would disable this hazard constrain function. */
#define DDRC_RHZD_THD(x)                ((u32)(((x) & 0x0000001F) << 8))
#define DDRC_GET_RHZD_THD(x)            ((u32)(((x >> 8) & 0x0000001F)))
#define DDRC_MASK_WHZD_THD              ((u32)0x0000003F << 0)          /*!<R/W 0  Used to constrain write hazard priority. When WHZD_THD setting and write command number >= WHZD_THD, it would keep the original write hazard priority at command QoS. If the value of WHZD_THD is 0, it would disable this hazard constrain function. */
#define DDRC_WHZD_THD(x)                ((u32)(((x) & 0x0000003F) << 0))
#define DDRC_GET_WHZD_THD(x)            ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup DDRC_HZD_CTRL1
 * @brief Hazard priority control register 0
 * @{
 **/
#define DDRC_MASK_RHZD_CRDT_NUM         ((u32)0x0000000F << 28)          /*!<R/W 4'h0  This is the max. number to limit the read hazard credit number. When read hazard credit number is larger then 0, it would keep the original read hazard priority at command QoS. If RHZD_CRDT_NUM is 0, it would disable this hazard constrain function. */
#define DDRC_RHZD_CRDT_NUM(x)           ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_RHZD_CRDT_NUM(x)       ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_RHZD_CRDT_CYC         ((u32)0x00000FFF << 16)          /*!<R/W 12'hFFF  Read hazard credit number would added 1 to in every interval cycles (RHZD_CRDT_CYC). */
#define DDRC_RHZD_CRDT_CYC(x)           ((u32)(((x) & 0x00000FFF) << 16))
#define DDRC_GET_RHZD_CRDT_CYC(x)       ((u32)(((x >> 16) & 0x00000FFF)))
#define DDRC_MASK_WHZD_CRDT_NUM         ((u32)0x0000000F << 12)          /*!<R/W 4'h0  This is the max. number to limit the write hazard credit number. When write hazard credit number is larger then 0, it would keep the original write hazard priority at command QoS. If WHZD_CRDT_NUM is 0, it would disable this hazard constrain function. */
#define DDRC_WHZD_CRDT_NUM(x)           ((u32)(((x) & 0x0000000F) << 12))
#define DDRC_GET_WHZD_CRDT_NUM(x)       ((u32)(((x >> 12) & 0x0000000F)))
#define DDRC_MASK_WHZD_CRDT_CYC         ((u32)0x00000FFF << 0)          /*!<R/W 12'hFFF  Write hazard credit number would add 1 to in every interval cycles (WHZD_CRDT_CYC). */
#define DDRC_WHZD_CRDT_CYC(x)           ((u32)(((x) & 0x00000FFF) << 0))
#define DDRC_GET_WHZD_CRDT_CYC(x)       ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup DDRC_CBCTL0
 * @brief Command buffer control register 0
 * @{
 **/
#define DDRC_MASK_LPR_RMW_RUN_LEN       ((u32)0x0000000F << 16)          /*!<R/W 0xF  When CBSTARVE3.LPR_RMW_MAX_STARVEreaches,lpr_rmwcmd que's priority will increase higher thanthe LPR inputexpired cmd and be serviced continuously LPR_RMW_RUN_LENlpr_rmw cmd when lpr_rmw cmd que's is not empty. */
#define DDRC_LPR_RMW_RUN_LEN(x)         ((u32)(((x) & 0x0000000F) << 16))
#define DDRC_GET_LPR_RMW_RUN_LEN(x)     ((u32)(((x >> 16) & 0x0000000F)))
#define DDRC_MASK_LPR_FREESLOT_NUM      ((u32)0x0000003F << 0)          /*!<R/W 0x10  Number of Free-slot for LP reads is the value +1; Due to HP reads share w/ LP reads in the Read Command Buffer, the number of Free-slot for HP reads is the value : 32 - (LPR_FREESLOT_NUM + 1) */
#define DDRC_LPR_FREESLOT_NUM(x)        ((u32)(((x) & 0x0000003F) << 0))
#define DDRC_GET_LPR_FREESLOT_NUM(x)    ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup DDRC_CBCTL1
 * @brief Command buffer control register 1
 * @{
 **/
#define DDRC_MASK_PAGE_HIT_THR_WR       ((u32)0x0000000F << 28)          /*!<R/W 0x0  When PAGE_HIT_THR_WR_EN=1, the cmd_buf for write same bank page_hit cmd threshold value is PAGE_HIT_THR_WR. */
#define DDRC_PAGE_HIT_THR_WR(x)         ((u32)(((x) & 0x0000000F) << 28))
#define DDRC_GET_PAGE_HIT_THR_WR(x)     ((u32)(((x >> 28) & 0x0000000F)))
#define DDRC_MASK_PAGE_HIT_THR_RD       ((u32)0x0000000F << 24)          /*!<R/W 0x0  When PAGE_HIT_THR_RD_EN=1, the cmd_buf for read same bank page_hit cmd threshold value is PAGE_HIT_THR_RD */
#define DDRC_PAGE_HIT_THR_RD(x)         ((u32)(((x) & 0x0000000F) << 24))
#define DDRC_GET_PAGE_HIT_THR_RD(x)     ((u32)(((x >> 24) & 0x0000000F)))
#define DDRC_MASK_LPW_RMW_RUN_LEN       ((u32)0x0000000F << 16)          /*!<R/W 0xF  When CBSTARVE4.LPW_RMW_MAX_STARVE reaches, lpw_rmw re-insert cmd priority will increase higher thanthe LPW inputexpired cmd & expired re-insert and be serviced continuously LPW_RMW_RUN_LENlpw_rmw cmd when lpr_rmw re-insert cmd is valid. */
#define DDRC_LPW_RMW_RUN_LEN(x)         ((u32)(((x) & 0x0000000F) << 16))
#define DDRC_GET_LPW_RMW_RUN_LEN(x)     ((u32)(((x >> 16) & 0x0000000F)))
#define DDRC_MASK_W_HAZ_EXP_CTRL        ((u32)0x0000000F << 8)          /*!<R/W 0xE  For RAW/WAW hit wcmd_entry triggers expired function to speed up hazard handler. W_HAZ_EXP_CTRL[1:0] 2'b11: always enable 2'b10: urgent only for read/write urgent signal assert 2'b0x: disable When W_HAZ_EXP_CTRL[3:2] 2'b1x: RAW hazard 2'bx1: WAW hazard 2'b00: disable Default function is urgent only. */
#define DDRC_W_HAZ_EXP_CTRL(x)          ((u32)(((x) & 0x0000000F) << 8))
#define DDRC_GET_W_HAZ_EXP_CTRL(x)      ((u32)(((x >> 8) & 0x0000000F)))
#define DDRC_BIT_PAGE_HIT_THR_WR_EN     ((u32)0x00000001 << 3)          /*!<R/W 0x0  when PAGE_HIT_THR_RD_EN=1, the cmd_buf for write same bank page_hit cmd threshold value is PAGE_HIT_THR_WR */
#define DDRC_PAGE_HIT_THR_WR_EN(x)      ((u32)(((x) & 0x00000001) << 3))
#define DDRC_BIT_PAGE_HIT_THR_RD_EN     ((u32)0x00000001 << 2)          /*!<R/W 0x0  when PAGE_HIT_THR_RD_EN=1, the cmd_buf for read same bank page_hit cmd threshold value is PAGE_HIT_THR_RD */
#define DDRC_PAGE_HIT_THR_RD_EN(x)      ((u32)(((x) & 0x00000001) << 2))
#define DDRC_BIT_PAGEOPEN               ((u32)0x00000001 << 1)          /*!<R/W 0x0  When PAGEOPEN =1,the last cmd of each bank will not be issued with auto-precharge to PCTL. */
#define DDRC_PAGEOPEN(x)                ((u32)(((x) & 0x00000001) << 1))
#define DDRC_BIT_DIS_WMG                ((u32)0x00000001 << 0)          /*!<R/W 0x0  Write merge function disable, it only works when enable WMG_EN feature. */
#define DDRC_DIS_WMG(x)                 ((u32)(((x) & 0x00000001) << 0))
/** @} */

/** @defgroup DDRC_CBSTARVE0
 * @brief command buffer starve register 0
 * @{
 **/
#define DDRC_BIT_LPR_STARVE_DIS         ((u32)0x00000001 << 16)          /*!<R/W 0x1  0x1: Disable */
#define DDRC_LPR_STARVE_DIS(x)          ((u32)(((x) & 0x00000001) << 16))
#define DDRC_MASK_LPR_MAX_STARVE        ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  LPR entry of command buffer is not serviced before it goes to expired in the interval time= {LPR_MAX_STARVE, timestamp_cycle} */
#define DDRC_LPR_MAX_STARVE(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_LPR_MAX_STARVE(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_CBSTARVE1
 * @brief command buffer starve register 1
 * @{
 **/
#define DDRC_BIT_HPR_STARVE_DIS         ((u32)0x00000001 << 16)          /*!<R/W 0x1  0x1: Disable */
#define DDRC_HPR_STARVE_DIS(x)          ((u32)(((x) & 0x00000001) << 16))
#define DDRC_MASK_HPR_MAX_STARVE        ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  HPR entry of command buffer is not serviced before it goes to expired in the interval time= {HPR_MAX_STARVE, timestamp_cycle} */
#define DDRC_HPR_MAX_STARVE(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_HPR_MAX_STARVE(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_CBSTARVE2
 * @brief command buffer starve register 2
 * @{
 **/
#define DDRC_BIT_LPW_STARVE_DIS         ((u32)0x00000001 << 16)          /*!<R/W 0x1  0x1: Disable */
#define DDRC_LPW_STARVE_DIS(x)          ((u32)(((x) & 0x00000001) << 16))
#define DDRC_MASK_LPW_MAX_STARVE        ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  LPW entry of command buffer is not serviced before it goes to expired in the interval time= {LPW_MAX_STARVE, timestamp_cycle} */
#define DDRC_LPW_MAX_STARVE(x)          ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_LPW_MAX_STARVE(x)      ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_CBSTARVE3
 * @brief command buffer starve register 3
 * @{
 **/
#define DDRC_BIT_LPR_RMW_STARVE_DIS     ((u32)0x00000001 << 16)          /*!<R/W 0x1  0x1: Disable */
#define DDRC_LPR_RMW_STARVE_DIS(x)      ((u32)(((x) & 0x00000001) << 16))
#define DDRC_MASK_LPR_RMW_MAX_STARVE    ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  lpr_rmw cmd que of command buffer is not serviced before it goes to expired in the interval time= {LPR_RMW_MAX_STARVE, timestamp_cycle} */
#define DDRC_LPR_RMW_MAX_STARVE(x)      ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_LPR_RMW_MAX_STARVE(x)  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_CBSTARVE4
 * @brief command buffer starve register 4
 * @{
 **/
#define DDRC_BIT_LPW_RMW_STARVE_DIS     ((u32)0x00000001 << 16)          /*!<R/W 0x1  0x1: Disable */
#define DDRC_LPW_RMW_STARVE_DIS(x)      ((u32)(((x) & 0x00000001) << 16))
#define DDRC_MASK_LPW_RMW_MAX_STARVE    ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  lpw_rmw re-insert cmd of command buffer is not serviced before it goes to expired in the interval time= {LPW_RMW_MAX_STARVE, timestamp_cycle} */
#define DDRC_LPW_RMW_MAX_STARVE(x)      ((u32)(((x) & 0x0000FFFF) << 0))
#define DDRC_GET_LPW_RMW_MAX_STARVE(x)  ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup DDRC_BSCH
 * @brief number of times for command schedule keeps to select the same bank
 * @{
 **/
#define DDRC_MASK_BA_SEL_LMT            ((u32)0x0000000F << 0)          /*!<R/W 4'h0  Number of times for command schedule keeps to select the same bank */
#define DDRC_BA_SEL_LMT(x)              ((u32)(((x) & 0x0000000F) << 0))
#define DDRC_GET_BA_SEL_LMT(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup DDRC_Exported_Types DDRC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief DDRC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t DDRC_CCR;                               /*!< CONFIGURATION CONTROL REGISTER,  Address offset: 0X00 */
	__IO uint32_t DDRC_DCR;                               /*!< DRAM CONFIGURATION CONTROL REGISTER,  Address offset: 0X04 */
	__IO uint32_t DDRC_IOCR;                              /*!< I/O CONFIGURATION CONTROL REGISTER,  Address offset: 0X08 */
	__IO uint32_t DDRC_CSR;                               /*!< CONTROLLER STATUS REGISTER,  Address offset: 0X0C */
	__IO uint32_t DDRC_DRR;                               /*!< DRAM REFRESH CONTROL REGISTER,  Address offset: 0X10 */
	__IO uint32_t DDRC_TPR0;                              /*!< DRAM TIMING PARAMETER REGISTER 0 Register,  Address offset: 0X14 */
	__IO uint32_t DDRC_TPR1;                              /*!< DRAM TIMING PARAMETER REGISTER 1 Register,  Address offset: 0X18 */
	__IO uint32_t DDRC_TPR2;                              /*!< DRAM TIMING PARAMETER REGISTER 2 Register,  Address offset: 0X1C */
	__IO uint32_t DDRC_TPR3;                              /*!< DRAM TIMING PARAMETER REGISTER 3 Register,  Address offset: 0X20 */
	__IO uint32_t DDRC_CMD_DPIN_NDGE;                     /*!< COMMAND DATA PIN REGISTER FOR LPDDR2/3 Register,  Address offset: 0X24 */
	__IO uint32_t DDRC_CMD_DPIN;                          /*!< COMMAND DATA PIN REGISTER,  Address offset: 0X28 */
	__IO uint32_t DDRC_TIE_DPIN;                          /*!< TIED DATA PIN REGISTER,  Address offset: 0X2C */
	__IO uint32_t DDRC_MR_INFO;                           /*!< MODE LATENCY INFORMATION REGISTER,  Address offset: 0X30 */
	__IO uint32_t DDRC_MR0;                               /*!< DRAM MODE REGISTER 0 Register,  Address offset: 0X34 */
	__IO uint32_t DDRC_MR1;                               /*!< DRAM MODE REGISTER 1 Register,  Address offset: 0X38 */
	__IO uint32_t DDRC_MR2;                               /*!< DRAM MODE REGISTER 2 Register,  Address offset: 0X3C */
	__IO uint32_t DDRC_MR3;                               /*!< DRAM MODE REGISTER 3 Register,  Address offset: 0X40 */
	__IO uint32_t DDRC_MR4;                               /*!< DRAM MODE REGISTER 4 Register,  Address offset: 0X44 */
	__IO uint32_t DDRC_MR5;                               /*!< DRAM MODE REGISTER 5 Register,  Address offset: 0X48 */
	__IO uint32_t DDRC_MR6;                               /*!< DRAM MODE REGISTER 6 Register,  Address offset: 0X4C */
	__IO uint32_t DDRC_MR11;                              /*!< DRAM MODE REGISTER 11 Register,  Address offset: 0X50 */
	__IO uint32_t DDRC_MR12;                              /*!< DRAM MODE REGISTER 12 Register,  Address offset: 0X54 */
	__IO uint32_t DDRC_MR13;                              /*!< DRAM MODE REGISTER 13 Register,  Address offset: 0X58 */
	__IO uint32_t DDRC_MR14;                              /*!< DRAM MODE REGISTER 14 Register,  Address offset: 0X5C */
	__IO uint32_t DDRC_MR22;                              /*!< DRAM MODE REGISTER 22 Register,  Address offset: 0X60 */
	__IO uint32_t RSVD0[11];                              /*!<  Reserved,  Address offset:0x64-0x8C */
	__IO uint32_t DDRC_TPR4;                              /*!< DRAM TIMING PARAMETER REGISTER 4 Register,  Address offset: 0X90 */
	__IO uint32_t DDRC_TPR5;                              /*!< DRAM TIMING PARAMETER REGISTER 5 Register,  Address offset: 0X94 */
	__IO uint32_t RSVD1[10];                              /*!<  Reserved,  Address offset:0x98-0xBC */
	__IO uint32_t DDRC_DPDRI;                             /*!< IN DPIN DATA FLOW, SET VALUE OF DPDR INDEX Register,  Address offset: 0XC0 */
	__IO uint32_t DDRC_DPDR;                              /*!< IN DPIN DATA FLOW, SETVALUEACCORDING TO DPDRI Register,  Address offset: 0XC4 */
	__IO uint32_t DDRC_ISR;                               /*!< CONTROLLER INTERRUPT REGISTER,  Address offset: 0XC8 */
	__IO uint32_t DDRC_IMR;                               /*!< DRAMC INTERRUPT MASK BITS Register,  Address offset: 0XCC */
	__IO uint32_t DDRC_BCR;                               /*!< BSTC CONTROL REGISTER,  Address offset: 0XD0 */
	__IO uint32_t DDRC_BECC;                              /*!< BSTC SIDEBAND ECC REGISTER,  Address offset: 0XD4 */
	__IO uint32_t DDRC_BCM;                               /*!< BSTC COMPARE BIT MASK REGISTER,  Address offset: 0XD8 */
	__I  uint32_t DDRC_BST;                               /*!< BSTC STATUS REGISTER,  Address offset: 0XDC */
	__I  uint32_t DDRC_BSRAM0;                            /*!< BSTC SRAM STATUS REGISTER 0 Register,  Address offset: 0XE0 */
	__I  uint32_t DDRC_BSRAM1;                            /*!< BSTC SRAM STATUS REGISTER 1 Register,  Address offset: 0XE4 */
	__I  uint32_t DDRC_BER;                               /*!< BSTC ERROR BIT REGISTER,  Address offset: 0XE8 */
	__IO uint32_t DDRC_BEXPC;                             /*!< BSTC SRAM EXPECTED EXECUTE NUMBERS OF WORDS REGISTER,  Address offset: 0XEC */
	__IO uint32_t RSVD2;                                  /*!<  Reserved,  Address offset:0xF0 */
	__I  uint32_t DDRC_SVN_ID;                            /*!< PCTL VERSION ID REGISTER,  Address offset: 0XF4 */
	__I  uint32_t DDRC_IDR;                               /*!< PCTL IDENTIFICATION REGISTER,  Address offset: 0XF8 */
	__IO uint32_t DDRC_MSG;                               /*!< RESERVED REGISTER,  Address offset: 0XFC */
	__IO uint32_t DDRC_DPERF0;                            /*!< DRAM PERFORMANCE REGITSER Register,  Address offset: 0X100 */
	__IO uint32_t RSVD3[72];                              /*!<  Reserved,  Address offset:0x104-0x220 */
	__IO uint32_t DDRC_MISC;                              /*!< MEMORY INFORMATION SETTING CONTROL REGISTER,  Address offset: 0X224 */
	__IO uint32_t DDRC_CHOP_SIZE;                         /*!< CHOPSIZE CONTROL REGISTER,  Address offset: 0X228 */
	__IO uint32_t RSVD4;                                  /*!<  Reserved,  Address offset:0x22C */
	__IO uint32_t DDRC_DRAM_SIZE;                         /*!< DRAM SIZE CONTROL REGISTER,  Address offset: 0X230 */
	__IO uint32_t DDRC_DSIZE_UP_4GB;                      /*!< DRAM SIZE UP 4GB CONTROL REGISTER,  Address offset: 0X234 */
	__IO uint32_t RSVD5[26];                              /*!<  Reserved,  Address offset:0x238-0x29C */
	__I  uint32_t DDRC_BUS_WRAP_IDR;                      /*!< BUS_WRAP ID NUMBER CONTROL REGISTER,  Address offset: 0X2A0 */
	__I  uint32_t DDRC_BUS_WRAP_SVN;                      /*!< BUS_WRAP VERSION CONTROL REGISTER,  Address offset: 0X2A4 */
	__IO uint32_t RSVD6[22];                              /*!<  Reserved,  Address offset:0x2A8-0x2FC */
	__IO uint32_t DDRC_ICG_EN;                            /*!< CLOCK GATING ENABLE CONTROL REGISTER,  Address offset: 0X300 */
	__IO uint32_t RSVD7[63];                              /*!<  Reserved,  Address offset:0x304-0x3FC */
	__IO uint32_t
	DDRC_ROW_REMAP0;                        /*!< DRAM ROW ADDRESS REMAP CONTROL REGISTER 0, REMAP ROW ADDRESS BIT 0 ~ BIT 7 Register,  Address offset: 0X400 */
	__IO uint32_t
	DDRC_ROW_REMAP1;                        /*!< DRAM ROW ADDRESS REMAP CONTROL REGISTER 1, REMAP ROW ADDRESS BIT 8 ~ BIT 15 Register,  Address offset: 0X404 */
	__IO uint32_t
	DDRC_ROW_REMAP2;                        /*!< DRAM ROW ADDRESS REMAP CONTROL REGISTER 2, REMAP ROW ADDRESS BIT 16 Register,  Address offset: 0X408 */
	__IO uint32_t
	DDRC_BANK_REMAP0;                       /*!< DRAM BANK ADDRESS REMAP CONTROL REGISTER 0, REMAP BANK ADDRESS BIT 0 ~ BIT 2 Register,  Address offset: 0X40C */
	__IO uint32_t
	DDRC_COLU_REMAP0;                       /*!< DRAM CLOUADDRESS REMAP CONTROL REGISTER 0, REMAP CLOUADDRESS BIT 2 ~BIT 9 Register,  Address offset: 0X410 */
	__IO uint32_t
	DDRC_COLU_REMAP1;                       /*!< DRAM CLOUADDRESS REMAP CONTROL REGISTER 1, REMAP CLOUADDRESS BIT 10 Register,  Address offset: 0X414 */
	__IO uint32_t
	DDRC_RANK_REMAP0;                       /*!< DRAM RANK ADDRESS REMAP CONTROL REGISTER 0, REMAP RANK ADDRESS BIT 0 Register,  Address offset: 0X418 */
	__IO uint32_t
	DDRC_CH_REMAP0;                         /*!< DRAM CHANNEL ADDRESS REMAP CONTROL REGISTER 0, REMAP CHANNEL ADDRESS BIT 0 Register,  Address offset: 0X41C */
	__IO uint32_t
	DDRC_BG_REMAP0;                         /*!< DRAM BGADDRESS REMAP CONTROL REGISTER 0, REMAP BGADDRESS BIT 0 ~BIT1 Register,  Address offset: 0X420 */
	__IO uint32_t DDRC_ADDR_REMAP_CTRL;                   /*!< NONBINARY DRAM DENSITY: 3/6/12/24GB Register,  Address offset: 0X424 */
	__IO uint32_t RSVD8[14];                              /*!<  Reserved,  Address offset:0x428-0x45C */
	__IO uint32_t DDRC_SLOT_PARK;                         /*!< SLOT PARKING CONTROL REGISTER,  Address offset: 0X460 */
	__IO uint32_t DDRC_SLOT_STARVE;                       /*!< SLOT STARVATION CONTROL REGISTER,  Address offset: 0X464 */
	__IO uint32_t DDRC_SLOT_CNTR;                         /*!< SLOT CONSTRAIN REGISTER,  Address offset: 0X468 */
	__IO uint32_t DDRC_EXP_CTRL;                          /*!< EXPIRE CONTROL REGISTER,  Address offset: 0X46C */
	__IO uint32_t DDRC_HZD_CTRL0;                         /*!< HAZARD PRIORITY CONTROL REGISTER 0 Register,  Address offset: 0X470 */
	__IO uint32_t DDRC_HZD_CTRL1;                         /*!< HAZARD PRIORITY CONTROL REGISTER 0 Register,  Address offset: 0X474 */
	__IO uint32_t RSVD9[2];                               /*!<  Reserved,  Address offset:0x478-0x47C */
	__IO uint32_t DDRC_CBCTL0;                            /*!< COMMAND BUFFER CONTROL REGISTER 0 Register,  Address offset: 0X480 */
	__IO uint32_t DDRC_CBCTL1;                            /*!< COMMAND BUFFER CONTROL REGISTER 1 Register,  Address offset: 0X484 */
	__IO uint32_t DDRC_CBSTARVE0;                         /*!< COMMAND BUFFER STARVE REGISTER 0 Register,  Address offset: 0X488 */
	__IO uint32_t DDRC_CBSTARVE1;                         /*!< COMMAND BUFFER STARVE REGISTER 1 Register,  Address offset: 0X48C */
	__IO uint32_t DDRC_CBSTARVE2;                         /*!< COMMAND BUFFER STARVE REGISTER 2 Register,  Address offset: 0X490 */
	__IO uint32_t DDRC_CBSTARVE3;                         /*!< COMMAND BUFFER STARVE REGISTER 3 Register,  Address offset: 0X494 */
	__IO uint32_t DDRC_CBSTARVE4;                         /*!< COMMAND BUFFER STARVE REGISTER 4 Register,  Address offset: 0X498 */
	__IO uint32_t RSVD10;                                 /*!<  Reserved,  Address offset:0x49C */
	__IO uint32_t
	DDRC_BSCH;                              /*!< NUMBER OF TIMES FOR COMMAND SCHEDULE KEEPS TO SELECT THE SAME BANK Register,  Address offset: 0X4A0 */
} DDRC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

// for uint32_t
#include <stdint.h>

#define PCTL_MR0_BL_BFO            0
#define PCTL_MR0_BL_BFW            3
#define PCTL_MR0_BST_TYPE_BFO      3
#define PCTL_MR0_BST_TYPE_BFW      1
#define PCTL_MR0_CAS_BFO           4
#define PCTL_MR0_CAS_BFW           3
#define PCTL_MR0_WR_BFO            9
#define PCTL_MR0_WR_BFW            3

#define PCTL_MR1_DLL_EN_BFO        0
#define PCTL_MR1_DLL_EN_BFW        1
#define PCTL_MR1_BL_BFO            0
#define PCTL_MR1_BL_BFW            3
#define PCTL_MR1_ADD_LAT_BFO       3
#define PCTL_MR1_ADD_LAT_BFW       2
#define PCTL_MR1_WR_LP23_BFO       5
#define PCTL_MR1_WR_LP23_BFW       3
#define PCTL_MR1_WR_LP4_BFO        4
#define PCTL_MR1_WR_LP4_BFW        3
#define PCTL_MR1_RTT_NORM_EN_BFO   8
#define PCTL_MR1_RTT_NORM_EN_BFW   3

#define PCTL_MR2_CAS_BFO           0
#define PCTL_MR2_CAS_BFW           4
#define PCTL_MR2_CWL_BFO           3
#define PCTL_MR2_CWL_BFW           3
#define PCTL_MR2_DYC_ODT_EN_BFO    9
#define PCTL_MR2_DYC_ODT_EN_BFW    2

/** @defgroup DDR_DPIN_CMD_INFO_definitions
  * @{
  */
#define DDR_DPIN_CMD_RSVD				((u32)0x00000000)
#define DDR_DPIN_CMD_PRECHARGE_ALL		((u32)0x00000001)
#define DDR_DPIN_CMD_PRECHARGE			((u32)0x00000002)
#define DDR_DPIN_CMD_ACTIVE				((u32)0x00000003)
#define DDR_DPIN_CMD_INFO(MODE) (((MODE) == DDR_DPIN_CMD_RSVD) || \
									   ((MODE) == DDR_DPIN_CMD_PRECHARGE_ALL) || \
									   ((MODE) == DDR_DPIN_CMD_PRECHARGE) || \
									   ((MODE) == DDR_DPIN_CMD_ACTIVE))
/**
  * @}
  */

/** @defgroup DDR_DPIN_MODE_definitions
  * @{
  */
#define DDR_DPIN_CMD				((u32)0x00000000)
#define DDR_DPIN_MRR				((u32)0x00000001)
#define DDR_DPIN_READ_DATA			((u32)0x00000002)
#define DDR_DPIN_WRITE_DATA			((u32)0x00000003)
#define DDR_DPIN_MODE(MODE) (((MODE) == DDR_DPIN_CMD) || \
									   ((MODE) == DDR_DPIN_MRR) || \
									   ((MODE) == DDR_DPIN_READ_DATA) || \
									   ((MODE) == DDR_DPIN_WRITE_DATA))
/**
  * @}
  */

/** @defgroup DDR_SIGNAL_MODE_definitions
  * @{
  */
#define DDR_SLOW			((u32)0x00000000)
#define DDR_SHIGH			((u32)0x00000001)
#define DDR_SIGNAL_MODE(MODE) (((MODE) == DDR_SLOW) || \
									   ((MODE) == DDR_SHIGH)
/**
  * @}
  */

/** @defgroup DDRC_Exported_Functions DDRC Exported Functions
  * @{
  */
_LONG_CALL_ void ddr_init(void);
_LONG_CALL_ void rxi316_DynSre_init(u32 IdleCnt, u32 state);

/**
  * @}
  */

/* MANUAL_GEN_END */
#endif

/** @} */

/** @} */
