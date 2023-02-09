#ifndef _AMEBA_I2C_H_
#define _AMEBA_I2C_H_

/** @addtogroup I2C
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * I2C0:
  *		- Base Address: I2C0_DEV
  *		- IPclk: 10MHz
  *		- Speed: Standard (up to 100 kHz) and Fast (up to 400 kHz) Modes
  *		- Address: 7/10-bit Address Mode
  *		- Slave: Slave1 (support 7/10-bit address); Slave2 (only support 7-bit address)
  *		- IRQ: I2C0_IRQ_LP, I2C0_IRQ, I2C0_IRQ_AP
  *
  * I2C1:
  *		- Base Address: I2C1_DEV
  *		- IPclk: 100MHz
  *		- Speed: Standard (up to 100 kHz), Fast (up to 400 kHz) Modes and High Speed (up to 3.4Mbps) mode.
  *		- Address: 7/10-bit Address Mode
  *		- Slave: Slave1 (support 7/10-bit address); Slave2 (only support 7-bit address)
  *		- IRQ: I2C1_IRQ, I2C1_IRQ_AP
  *
  * I2C2:
  *		- Base Address: I2C2_DEV
  *		- IPclk: 100MHz
  *		- Speed: Standard (up to 100 kHz), Fast (up to 400 kHz) Modes and High Speed (up to 3.4Mbps) mode.
  *		- Address: 7/10-bit Address Mode
  *		- Slave: Slave1 (support 7/10-bit address); Slave2 (only support 7-bit address)
  *		- IRQ: I2C2_IRQ, I2C2_IRQ_AP
  *****************************************************************************************
  * How to use Normal I2C
  *****************************************************************************************
  *      To use the normal i2c mode, the following steps are mandatory:
  *
  *      1. Enable peripheral clock using the follwoing functions.(it is enabled by default)
  *			RCC_PeriphClockCmd(APBPeriph_I2Cx, APBPeriph_I2Cx_CLOCK, ENABLE);
  *
  *      2. configure the I2C pinmux.
  *			Pinmux_Config(Pin_Num, PINMUX_FUNCTION_I2C)
  *
  *      3. Program Role, Address Mode, Speed Mode, I2C CLK, Slave address, Threshold, Feature Supports
  *			I2C_StructInit()
  *
  *      4. Init Hardware use step3 parameters:
  *			I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef* I2C_InitStruct)
  *
  *      5. Enable the NVIC and the corresponding interrupt using following function if you need
  *			to use interrupt mode.
  *			I2C_INTConfig(): I2C IRQ Mask set
  *			InterruptRegister(): register the i2c irq handler
  *			InterruptEn(): Enable the NVIC interrupt
  *
  *      6. Enable i2c module using I2C_Cmd().
  *
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup I2C
  * @brief  I2C driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup I2C_Register_Definitions I2C Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup IC_CON
 * @brief I2C Control Register
 * @{
 **/
#define I2C_BIT_IC_SLAVE_DISABLE_1       ((u32)0x00000001 << 7)          /*!<R/W 0x0  This bit controls whether I2C has its slave2 (7-bit address) disabled. * 0: slave2 is enabled * 1: slave2 is disabled */
#define I2C_BIT_IC_SLAVE_DISABLE_0       ((u32)0x00000001 << 6)          /*!<R/W 0x0  This bit controls whether I2C has its slave1 (7-bit or 10-bit address) disabled. * 0: slave1 is enabled * 1: slave1 is disabled */
#define I2C_BIT_IC_RESTATRT_EN           ((u32)0x00000001 << 5)          /*!<R/W 0x0  Determine whether RESTART conditions may be sent when acting as a master. * 0: disable * 1: enable */
#define I2C_BIT_IC_10BITADDR_SLAVE       ((u32)0x00000001 << 3)          /*!<R/W 0x0  When acting as a slave, this bit controls whether the I2C responds to 7- or 10-bit addresses. * 0: 7-bit addressing * 1: 10-bit addressing */
#define I2C_MASK_SPEED                   ((u32)0x00000003 << 1)          /*!<R/W 0x0  These bits control at which speed the I2C operates; its setting is relevant only if one is operating the I2C in master mode. * 1: standard mode (0 to 100kbit/s) * 2: fast mode (<=400kbit/s) * 3: high speed mode (<=3.4Mbit/s) */
#define I2C_SPEED(x)                     ((u32)(((x) & 0x00000003) << 1))
#define I2C_GET_SPEED(x)                 ((u32)(((x >> 1) & 0x00000003)))
#define I2C_BIT_MASTER_MODE              ((u32)0x00000001 << 0)          /*!<R/W 0x0  This bit controls whether the I2C master is enabled. * 0: master disabled * 1: master enabled */
/** @} */

/** @defgroup IC_TAR
 * @brief I2C Target Address Register
 * @{
 **/
#define I2C_BIT_IC_10BITADDR_MASTER      ((u32)0x00000001 << 12)          /*!<R/W 0x0  Control whether I2C starts its transfers in 7- or 10-bit addressing mode when acting as a master. * 0: 7-bit addressing * 1: 10-bit addressing */
#define I2C_BIT_SPECIAL                  ((u32)0x00000001 << 11)          /*!<R/W 0x0  This bit indicates whether software performs a General Call or START BYTE command. * 0: ignore bit 10 GC_OR_START and use IC_TAR normally * 1: perform special I2C command as specified in GC_OR_START bit */
#define I2C_BIT_GC_OR_START              ((u32)0x00000001 << 10)          /*!<R/W 0x0  If SPECIAL is set to 1, then this bit indicates whether a General Call or START BYTE command is to be performed by I2C. (ic_clk domain) * 0: General Call - after issuing a General Call, only write may be performed * 1: START BYTE */
#define I2C_MASK_IC_TAR                  ((u32)0x000003FF << 0)          /*!<R/W 0x10  This is the target address for any master transaction. When transmitting a General Call, these bits are ignored. To generate a START BYTE, the CPU needs to write only once into these bits. */
#define I2C_IC_TAR(x)                    ((u32)(((x) & 0x000003FF) << 0))
#define I2C_GET_IC_TAR(x)                ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup IC_SAR
 * @brief I2C Slave Address Register
 * @{
 **/
#define I2C_MASK_IC_SAR                  ((u32)0x000003FF << 0)          /*!<R/W 0x11  The IC_SAR holds the slave address when the I2C is operating as slave1. For 7-bit addressing, only IC_SAR[6:0] is used. The register can be written only when the I2C interface is disabled, which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_SAR(x)                    ((u32)(((x) & 0x000003FF) << 0))
#define I2C_GET_IC_SAR(x)                ((u32)(((x >> 0) & 0x000003FF)))
/** @} */

/** @defgroup IC_HS_MAR
 * @brief I2C High Speed Master Mode Code Address Register
 * @{
 **/
#define I2C_MASK_IC_HS_MAR               ((u32)0x00000007 << 0)          /*!<R/W 0x0  This bit field holds the value of the I2C HS mode master code. HS-mode master codes are reserved 8-bit codes (00001xxx) that are not used for slave addressing or other purposes. This register can be written only when the I2C interface is disabled, which corresponds to the IC_ENABLE register being set to 0. Write at other times have no effect. */
#define I2C_IC_HS_MAR(x)                 ((u32)(((x) & 0x00000007) << 0))
#define I2C_GET_IC_HS_MAR(x)             ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup IC_DATA_CMD
 * @brief I2C Rx/Tx Data Buffer and Command Register
 * @{
 **/
#define I2C_BIT_NULL_DATA                ((u32)0x00000001 << 11)          /*!<W 0x0  *1 : null data transfer */
#define I2C_BIT_CMD_RESTART              ((u32)0x00000001 << 10)          /*!<W 0x0  This bit controls whether a RESTART is issued after the byte is sent or received. * 1: a RESTART is issued after the data is sent/received (according to the value of CMD_RW), regardless of whether or not the transfer direction is changing from the previous command. */
#define I2C_BIT_CMD_STOP                 ((u32)0x00000001 << 9)          /*!<W 0x0  This bit controls whether a STOP is issued after the byte is sent or received. * 1: STOP is issued after this byte, regardless of whether or not the Tx FIFO is empty. If the Tx FIFO is not empty, the master immediately tries to start a new transfer by issuing a START and arbitrating for the bus. * 0: STOP is not issued after this byte. */
#define I2C_BIT_CMD_RW                   ((u32)0x00000001 << 8)          /*!<W 0x0  This bit controls whether a read or write is performed. This bit does not control the direction when the I2C acts as a slave. It controls only the direction when it acts as a master. * 1: Read * 0: Write */
#define I2C_MASK_IC_DATA                 ((u32)0x000000FF << 0)          /*!<R/W 0x0  This register contains the data to be transmitted or received on the I2C bus. If you are writing to this register and want to perform a read, these bits are ignored by the I2C. However, when you read this register, these bits return the value of data received on the I2C interface. */
#define I2C_IC_DATA(x)                   ((u32)(((x) & 0x000000FF) << 0))
#define I2C_GET_IC_DATA(x)               ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IC_SS_SCL_HCNT
 * @brief Standard Speed I2C Clock SCL High Count Register
 * @{
 **/
#define I2C_MASK_IC_SS_SCL_HCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0190  This register sets the SCL clock high-period count for standard speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_SS_SCL_HCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_SS_SCL_HCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_SS_SCL_LCNT
 * @brief Standard Speed I2C Clock SCL Low Count Register
 * @{
 **/
#define I2C_MASK_IC_SS_SCL_LCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x01D6  This register sets the SCL clock low-period count for standard speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_SS_SCL_LCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_SS_SCL_LCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_FS_SCL_HCNT
 * @brief Fast Speed I2C Clock SCL High Count Register
 * @{
 **/
#define I2C_MASK_IC_FS_SCL_HCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x003C  This register sets the SCL clock high-period count for fast speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_FS_SCL_HCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_FS_SCL_HCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_FS_SCL_LCNT
 * @brief Fast Speed I2C Clock SCL Low Count Register
 * @{
 **/
#define I2C_MASK_IC_FS_SCL_LCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0082  This register sets the SCL clock low-period count for fast speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_FS_SCL_LCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_FS_SCL_LCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_HS_SCL_HCNT
 * @brief High Speed I2C Clock SCL High Count Register
 * @{
 **/
#define I2C_MASK_IC_HS_SCL_HCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0006  This register sets the SCL clock high-period count for high speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. The SCL High time depends on the loading of the bus. For 100pF loading, the SCL High time is 60ns; for 400pF loading, the SCL High time is 120ns. */
#define I2C_IC_HS_SCL_HCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_HS_SCL_HCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_HS_SCL_LCNT
 * @brief High Speed I2C Clock SCL Low Count Register
 * @{
 **/
#define I2C_MASK_IC_HS_SCL_LCNT          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0010  This register sets the SCL clock low-period count for high speed. This register must be set before any I2C bus transaction can take place to ensure proper I/O timing. This register can be written only when the I2C interface is disabled which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. The SCL Low time depends on the loading of the bus. For 100pF loading, the SCL High time is 160ns; for 400pF loading, the SCL High time is 320ns. */
#define I2C_IC_HS_SCL_LCNT(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_HS_SCL_LCNT(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_INTR_STAT
 * @brief I2C Interrupt Status Register
 * @{
 **/
#define I2C_BIT_R_LP_WAKE_2              ((u32)0x00000001 << 13)          /*!<R 0x0   */
#define I2C_BIT_R_LP_WAKE_1              ((u32)0x00000001 << 12)          /*!<R 0x0   */
#define I2C_BIT_R_GEN_CALL               ((u32)0x00000001 << 11)          /*!<R 0x0   */
#define I2C_BIT_R_START_DET              ((u32)0x00000001 << 10)          /*!<R 0x0   */
#define I2C_BIT_R_STOP_DET               ((u32)0x00000001 << 9)          /*!<R 0x0   */
#define I2C_BIT_R_ACTIVITY               ((u32)0x00000001 << 8)          /*!<R 0x0   */
#define I2C_BIT_R_RX_DONE                ((u32)0x00000001 << 7)          /*!<R 0x0   */
#define I2C_BIT_R_TX_ABRT                ((u32)0x00000001 << 6)          /*!<R 0x0   */
#define I2C_BIT_R_RD_REQ                 ((u32)0x00000001 << 5)          /*!<R 0x0   */
#define I2C_BIT_R_TX_EMPTY               ((u32)0x00000001 << 4)          /*!<R 0x0   */
#define I2C_BIT_R_TX_OVER                ((u32)0x00000001 << 3)          /*!<R 0x0   */
#define I2C_BIT_R_RX_FULL                ((u32)0x00000001 << 2)          /*!<R 0x0   */
#define I2C_BIT_R_RX_OVER                ((u32)0x00000001 << 1)          /*!<R 0x0   */
#define I2C_BIT_R_RX_UNDER               ((u32)0x00000001 << 0)          /*!<R 0x0   */
/** @} */

/** @defgroup IC_INTR_MASK
 * @brief I2C Interrupt Mask Register
 * @{
 **/
#define I2C_BIT_M_LP_WAKE_2              ((u32)0x00000001 << 13)          /*!<R/W 0x0   */
#define I2C_BIT_M_LP_WAKE_1              ((u32)0x00000001 << 12)          /*!<R/W 0x0   */
#define I2C_BIT_M_GEN_CALL               ((u32)0x00000001 << 11)          /*!<R/W 0x0   */
#define I2C_BIT_M_START_DET              ((u32)0x00000001 << 10)          /*!<R/W 0x0   */
#define I2C_BIT_M_STOP_DET               ((u32)0x00000001 << 9)          /*!<R/W 0x0   */
#define I2C_BIT_M_ACTIVITY               ((u32)0x00000001 << 8)          /*!<R/W 0x0   */
#define I2C_BIT_M_RX_DONE                ((u32)0x00000001 << 7)          /*!<R/W 0x0   */
#define I2C_BIT_M_TX_ABRT                ((u32)0x00000001 << 6)          /*!<R/W 0x0   */
#define I2C_BIT_M_RD_REQ                 ((u32)0x00000001 << 5)          /*!<R/W 0x0   */
#define I2C_BIT_M_TX_EMPTY               ((u32)0x00000001 << 4)          /*!<R/W 0x0   */
#define I2C_BIT_M_TX_OVER                ((u32)0x00000001 << 3)          /*!<R/W 0x0   */
#define I2C_BIT_M_RX_FULL                ((u32)0x00000001 << 2)          /*!<R/W 0x0   */
#define I2C_BIT_M_RX_OVER                ((u32)0x00000001 << 1)          /*!<R/W 0x0   */
#define I2C_BIT_M_RX_UNDER               ((u32)0x00000001 << 0)          /*!<R/W 0x0   */
/** @} */

/** @defgroup IC_RAW_INTR_STAT
 * @brief I2C Raw Interrupt Status Register
 * @{
 **/
#define I2C_BIT_LP_WAKE_2                ((u32)0x00000001 << 13)          /*!<R 0x0  Set when address SAR_2 match with address sending on I2C BUS. */
#define I2C_BIT_LP_WAKE_1                ((u32)0x00000001 << 12)          /*!<R 0x0  Set when address SAR match with address sending on I2C BUS. */
#define I2C_BIT_GEN_CALL                 ((u32)0x00000001 << 11)          /*!<R 0x0  Set only when a General Call address is received and it is acknowledged. */
#define I2C_BIT_START_DET                ((u32)0x00000001 << 10)          /*!<R 0x0  Indicates whether a START or RESTART condition has occurred on the I2C interface regardless of whether I2C is operating in slave or master mode. */
#define I2C_BIT_STOP_DET                 ((u32)0x00000001 << 9)          /*!<R 0x0  Indicates whether a STOP condition has occurred on the I2C interface regardless of whether I2C is operating in slave or master mode. */
#define I2C_BIT_ACTIVITY                 ((u32)0x00000001 << 8)          /*!<R 0x0  This bit captures I2C activity. */
#define I2C_BIT_RX_DONE                  ((u32)0x00000001 << 7)          /*!<R 0x0  When the I2C is acting as a slave-transmitter, this bit is set to 1 if the master does not acknowledge a transmitted byte. This occurs on the last byte of the transmission, indicating that the transmission is done. */
#define I2C_BIT_TX_ABRT                  ((u32)0x00000001 << 6)          /*!<R 0x0  This bit indicates if I2C as a transmitter, is unable to complete the intended actions on the contents of the transmit FIFO. This situation can occur both as an I2C master or an I2C slave, and is referred to as a 'transmit abort'. * 1: the IC_TX_ABRT_SOURCE register indicates the reason why the transmit abort take places. */
#define I2C_BIT_RD_REQ                   ((u32)0x00000001 << 5)          /*!<R 0x0  This bit is set to 1 when I2C is acting as a slave and another I2C master is attempting to read data from I2C. The I2C holds the I2C bus in a wait state(SCL=0) until this interrupt is serviced, which means that the slave has been addressed by a remote master that is asking for data to be transferred. The processor must respond to this interrupt and then write the requested data to the IC_DATA_CMD register. */
#define I2C_BIT_TX_EMPTY                 ((u32)0x00000001 << 4)          /*!<R 0x0  This bit is set to 1 when the transmit buffer is at or below the threshold value set in the IC_TX_TL register. It is automatically cleared by hardware when the buffer level goes above the threshold. */
#define I2C_BIT_TX_OVER                  ((u32)0x00000001 << 3)          /*!<R 0x0  Set during transmit if the transmit buffer is filled to IC_TX_BUFFER_DEPTH and the processor attempts to issue another I2C command by writing to the IC_DATA_CMD register. */
#define I2C_BIT_RX_FULL                  ((u32)0x00000001 << 2)          /*!<R 0x0  Set when the receive buffer reaches or goes above the RX_TL threshold in the IC_RX_TL register. It is automatically cleared by hardware when buffer level goes below the threshold. */
#define I2C_BIT_RX_OVER                  ((u32)0x00000001 << 1)          /*!<R 0x0  Not used. Set if the receive buffer is completely filled to IC_RX_BUFFER_DEPTH and an additional byte is received from an external I2C device. */
#define I2C_BIT_RX_UNDER                 ((u32)0x00000001 << 0)          /*!<R 0x0  Set if the processor attempts to read the receive buffer when it is empty by reading from the IC_DATA_CMD register. */
/** @} */

/** @defgroup IC_RX_TL
 * @brief I2C Receive FIFO Threshold Register
 * @{
 **/
#define I2C_MASK_IC_RX_0_TL              ((u32)0x000000FF << 0)          /*!<R/W 0x0  Receive FIFO Threshold Level. Control the level of entries (or above) that triggers the RX_FULL interrupt (bit 2 in IC_RAW_INTR_STAT register). */
#define I2C_IC_RX_0_TL(x)                ((u32)(((x) & 0x000000FF) << 0))
#define I2C_GET_IC_RX_0_TL(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IC_TX_TL
 * @brief I2C Transmit FIFO Threshold Register
 * @{
 **/
#define I2C_MASK_IC_TX_0_TL              ((u32)0x000000FF << 0)          /*!<R/W 0x0  Transmit FIFO Threshold Level. Control the level of entries (or below) that triggers the TX_EMPTY interrupt (bit 4 in IC_RAW_INTR_STAT register). */
#define I2C_IC_TX_0_TL(x)                ((u32)(((x) & 0x000000FF) << 0))
#define I2C_GET_IC_TX_0_TL(x)            ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IC_CLR_INTR
 * @brief Clear Combined and Individual Interrupt Register
 * @{
 **/
#define I2C_BIT_CLR_INRT                 ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the combined interrupt, all individual interrupts, and the IC_TX_ABRT_SOURCE register. This bit does not clear hardware clearable interrupts but software clearable interrupts. */
/** @} */

/** @defgroup IC_CLR_RX_UNDER
 * @brief Clear RX_UNDER Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_RX_UNDER             ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the RX_UNDER interrupt (bit 0) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_RX_OVER
 * @brief Clear RX_OVER Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_RX_OVER              ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the RX_OVER interrupt (bit 1) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_TX_OVER
 * @brief Clear TX_OVER Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_TX_OVER              ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the TX_OVER interrupt (bit 3) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_RD_REQ
 * @brief Clear RD_REQ Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_RD_REQ               ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the RD_REQ interrupt (bit 5) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_TX_ABRT
 * @brief Clear TX_ABRT Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_TX_ABRT              ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the TX_ABRT interrupt (bit 6) of IC_RAW_INTR_STAT register, and the IC_TX_ABRT_SOURCE register. This is also releases the TX FIFO from the flushed/reset state, allowing more writes to the TX FIFO. */
/** @} */

/** @defgroup IC_CLR_RX_DONE
 * @brief Clear RX_DONE Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_RX_DONE              ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the RX_DONE interrupt (bit 7) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_ACTIVITY
 * @brief Clear ACTIVITY Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_ACTIVITY             ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the ACTIVITY interrupt if the I2C is not active anymore. If the I2C module is still active on the bus, the ACTIVITY interrupt bit continues to be set. It is automatically cleared by hardware if the module is disabled and if there is no further activity on the bus. The value read from this register to get status of the ACTIVITY interrupt (bit 8) of the IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_STOP_DET
 * @brief Clear STOP_DET Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_STOP_DET             ((u32)0x00000001 << 0)          /*!<R 0x0  ead this register to clear the STOP_DET interrupt (bit 9) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_START_DET
 * @brief Clear START_DET Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_START_DET            ((u32)0x00000001 << 0)          /*!<R 0x0  ead this register to clear the START_DET interrupt (bit 10) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_CLR_GEN_CALL
 * @brief Clear GEN_ALL Interrpt Register
 * @{
 **/
#define I2C_BIT_CLR_GEN_CALL             ((u32)0x00000001 << 0)          /*!<R 0x0  ead this register to clear the GEN_CALL interrupt (bit 11) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_ENABLE
 * @brief I2C Enable Register
 * @{
 **/
#define I2C_BIT_FORCE                    ((u32)0x00000001 << 1)          /*!<R/W 0x0  Force master I2C stop transfer immediately after IC_ENABLE set 0. * 1: when IC_ENABLE is set 0 during transfer, force master FSM go to IDLE immediately. * 0: when IC_ENABLE is set 0 during transfer, master FSM while finish current transition (include RESTART) before go to IDLE. */
#define I2C_BIT_ENABLE                   ((u32)0x00000001 << 0)          /*!<R/W 0x0  Control whether the I2C is enabled. * 0: Disable I2C (TX and RX FIFOs are held in an erased state) * 1: Enable I2C */
/** @} */

/** @defgroup IC_STATUS
 * @brief I2C Status Register
 * @{
 **/
#define I2C_BIT_SLV_ACTIVITY             ((u32)0x00000001 << 6)          /*!<R 0x0  Slave FSM Activity Status. When the Slave FSM is not in the IDLE state, this bit is set. * 0: Slave FSM is in the IDLE state so the slave part of I2C is not active * 1: Slave FSM is not he IDLE state so the slave part of I2C is active */
#define I2C_BIT_MST_ACTIVITY             ((u32)0x00000001 << 5)          /*!<R 0x0  Master FSM Activity Status. When the Master FSM is not in the IDLE state, this bit is set. * 0: Master FSM is in the IDLE state so the master part of I2C is not active * 1: Master FSM is not he IDLE state so the master part of I2C is active */
#define I2C_BIT_RFF                      ((u32)0x00000001 << 4)          /*!<R 0x0  Receive FIFO Completely Full. When the receive FIFO is completely full, this bit is set. When the receive FIFO contains one or more empty location, this bit is cleared. * 0: Receive FIFO is not full * 1: Receive FIFO is full */
#define I2C_BIT_RFNE                     ((u32)0x00000001 << 3)          /*!<R 0x0  Receive FIFO Not Empty. This bit is set when the receive FIFO contains one or more entries; it is cleared when the receive FIFO is empty. * 0: Receive FIFO is empty * 1: Receive FIFO is not empty */
#define I2C_BIT_TFE                      ((u32)0x00000001 << 2)          /*!<R 0x1  Transmit FIFO Completely Empty. When the transmit FIFO is completely empty, this bit is set. When it contains one or more valid entries, this bit is cleared. This bit field does not request an interrupt. * 0: Transmit FIFO is not empty * 1: Transmit FIFO is empty */
#define I2C_BIT_TFNF                     ((u32)0x00000001 << 1)          /*!<R 0x1  Transmit FIFO Not Full. Set when the transmit FIFO contains one or more empty locations, and is cleared when the FIFO is full. * 0: Transmit FIFO is full * 1: Transmit FIFO is not full */
#define I2C_BIT_IC_ACTIVITY              ((u32)0x00000001 << 0)          /*!<R 0x0  I2C Activity Status. */
/** @} */

/** @defgroup IC_TXFLR
 * @brief I2C Transmit FIFO Level Register
 * @{
 **/
#define I2C_MASK_TXFLR                   ((u32)0x0000003F << 0)          /*!<R 0x0  Transmit FIFO Level. Contains the number of valid data entries in the transmit FIFO. */
#define I2C_TXFLR(x)                     ((u32)(((x) & 0x0000003F) << 0))
#define I2C_GET_TXFLR(x)                 ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup IC_RXFLR
 * @brief I2C Receive FIFO Level Register
 * @{
 **/
#define I2C_MASK_RXFLR                   ((u32)0x0000003F << 0)          /*!<R 0x0  Receive FIFO Level. Contains the number of valid data entries in the receive FIFO. */
#define I2C_RXFLR(x)                     ((u32)(((x) & 0x0000003F) << 0))
#define I2C_GET_RXFLR(x)                 ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup IC_SDA_HOLD
 * @brief I2C SDA Hold Time Length Register
 * @{
 **/
#define I2C_MASK_IC_SDA_HOLD             ((u32)0x0000FFFF << 0)          /*!<R/W 0x1  Set the required SDA hold time in units of ic_clk period. */
#define I2C_IC_SDA_HOLD(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define I2C_GET_IC_SDA_HOLD(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IC_TX_ABRT_SOURCE
 * @brief I2C Transmit Abort Source Register
 * @{
 **/
#define I2C_BIT_ABRT_SLV_ARBLOST         ((u32)0x00000001 << 14)          /*!<R 0x0  * 1: Slave lost the bus while transmitting data to a remote master. IC_TX_ABRT_SOURCE [12] is set at the same time. */
#define I2C_BIT_ABRT_SLVFLUSH_TXFIFO     ((u32)0x00000001 << 13)          /*!<R 0x0  * 1: Slave has received a read command and some data exists in the TXFIFO so the slave issues a TX_ABRT interrupt to flush old data in TXFIFO. */
#define I2C_BIT_ARBT_LOST                ((u32)0x00000001 << 12)          /*!<R 0x0  * 1: Master has lost arbitration, or if IC_TX_ABRT_SOURCE [* 14] is also set, then the slave transmitter has lost arbitration. */
#define I2C_BIT_ABRT_MASTER_DIS          ((u32)0x00000001 << 11)          /*!<R 0x0  * 1: User tries to initiate a Master operation with the Master mode disabled. */
#define I2C_BIT_ABRT_10B_RD_NORSTRT      ((u32)0x00000001 << 10)          /*!<R 0x0  * 1: The restart is disabled and the master sends a read command in * 10-bit addressing mode. */
#define I2C_BIT_ABRT_SBYTE_NORSTRT       ((u32)0x00000001 << 9)          /*!<R 0x0  * 1: The restart is disabled and the user is trying to send a START Byte. */
#define I2C_BIT_ABRT_HS_NORSTRT          ((u32)0x00000001 << 8)          /*!<R 0x0  * 1: The restart is disabled and the user is trying to use the master to transfer data in High Speed mode. */
#define I2C_BIT_ABRT_SBYTE_ACKDET        ((u32)0x00000001 << 7)          /*!<R 0x0  * 1: Master has sent a START Byte and the START Byte was acknowledged (wrong behavior). */
#define I2C_BIT_ABRT_HS_ACKDET           ((u32)0x00000001 << 6)          /*!<R 0x0  * 1: Master is in High Speed mode and the High Speed Master code was acknowledged (wrong behavior). */
#define I2C_BIT_ABRT_GCALL_READ          ((u32)0x00000001 << 5)          /*!<R 0x0  * 1: I2C in master mode sent a General Call but the user programmed the byte following the General Call to be a read from the bus. */
#define I2C_BIT_ABRT_GCALL_NOACK         ((u32)0x00000001 << 4)          /*!<R 0x0  * 1: I2C in master mode sent a General Call and no slave on the bus acknowledged the General Call. */
#define I2C_BIT_ABRT_TXDATA_NOACK        ((u32)0x00000001 << 3)          /*!<R 0x0  * 1: This is a master-mode only bit. Master has received an acknowledgement for the address, but when it sent data byte(s) following the address, it did not receive an acknowledge from the remote slave(s). */
#define I2C_BIT_ABRT_10ADDR2_NOACK       ((u32)0x00000001 << 2)          /*!<R 0x0  * 1: Master is in * 10-bit address mode and the second byte of the * 10-bit address was not acknowledged by any slave. */
#define I2C_BIT_ABRT_10ADDR1_NOACK       ((u32)0x00000001 << 1)          /*!<R 0x0  * 1: Master is in * 10-bit address mode and the first * 10-bit address byte was not acknowledged by any slave. */
#define I2C_BIT_ABRT_7B_ADDR_NOACK       ((u32)0x00000001 << 0)          /*!<R 0x0  * 1: Master is in 7-bit addressing mode and the address sent was not acknowledged by any slave */
/** @} */

/** @defgroup IC_SLV_DATA_NACK_ONLY
 * @brief Generate Slave Data NACK Register
 * @{
 **/
#define I2C_BIT_NACK                     ((u32)0x00000001 << 0)          /*!<R/W 0x0  Generate NACK. This NACK generation only occurs when I2C is a slave-receiver. If this register is set to a value of 1, it can only generate a NACK after a data byte is received; hence, the data transfer is aborted and the data received is not pushed to the receive buffer. When the register is set to a value of 0, it generates NACK/ACK, depending on normal criteria. * 1: generate NACK after data byte received * 0: generate NACK/ACK normally */
/** @} */

/** @defgroup IC_SDA_SETUP
 * @brief I2C SDA Setup Register
 * @{
 **/
#define I2C_MASK_IC_SDA_SETUP            ((u32)0x000000FF << 0)          /*!<R/W 0x5  Set the required SDA setup time in units of ic_clk period. */
#define I2C_IC_SDA_SETUP(x)              ((u32)(((x) & 0x000000FF) << 0))
#define I2C_GET_IC_SDA_SETUP(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup IC_ACK_GENERAL_CALL
 * @brief I2C ACK General Call Register
 * @{
 **/
#define I2C_BIT_ACK_GEN_CALL             ((u32)0x00000001 << 0)          /*!<R/W 0x1  * 1: I2C responds with a ACK when it receives a General Call. * 0: I2C does not generate General Call interrupts. */
/** @} */

/** @defgroup IC_ENABLE_STATUS
 * @brief I2C Enable Status Register
 * @{
 **/
#define I2C_BIT_SLV_DISABLE_WHILE_BUSY   ((u32)0x00000001 << 2)          /*!<R 0x0  Slave Disabled While Busy (Transmit, Receive). This bit indicates if a potential or active Slave operation has been aborted due to the setting of the IC_SLAVE_DISBALE register from 1 to 0. */
#define I2C_BIT_IC_EN                    ((u32)0x00000001 << 0)          /*!<R 0x0  ic_en Status. This bit always reflects the value driven on the output port ic_en. * 1: I2C is deemed to be in an enabled state. * 0: I2C is deemed completely inactive. */
/** @} */

/** @defgroup IC_DEBUG_SEL
 * @brief I2C Debug SEL Register
 * @{
 **/
#define I2C_MASK_IC_DEBUG_SEL            ((u32)0x0000000F << 0)          /*!<R/W 0x0  * 0: Debug clock * 1: Debug APB * 2: RSVD * 3: Debug FIFO * 4: Debug Timing * 5: Debug Slave Mode Address Match * 6: Debug Interrupt */
#define I2C_IC_DEBUG_SEL(x)              ((u32)(((x) & 0x0000000F) << 0))
#define I2C_GET_IC_DEBUG_SEL(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup IC_OUT_SMP_DLY
 * @brief
 * @{
 **/
#define I2C_MASK_IC_OUT_SMP_DLY          ((u32)0x00000007 << 0)          /*!<R/W 0x2  Delay cycles for sample of master drived SCL value. The sample of master drived SCL is used for slave stretch. Slave stretch is valid when master drive SCL High, but slave drive SCL low. Valid value of IC_OUT_SMP_DLY is 0-7. Inicate a delay of 1-8 cycles. */
#define I2C_IC_OUT_SMP_DLY(x)            ((u32)(((x) & 0x00000007) << 0))
#define I2C_GET_IC_OUT_SMP_DLY(x)        ((u32)(((x >> 0) & 0x00000007)))
/** @} */

/** @defgroup IC_CLR_ADDR_MATCH
 * @brief Clear Slave Mode Address Match Interrupt Register
 * @{
 **/
#define I2C_BIT_IC_CLR_ADDR_MATCH        ((u32)0x00000001 << 0)          /*!<R 0x0  Read this register to clear the slave mode address match interrupt (bit 12&13) of IC_RAW_INTR_STAT register. */
/** @} */

/** @defgroup IC_FILTER
 * @brief I2C Filter Register
 * @{
 **/
#define I2C_BIT_IC_DIG_FLTR_SEL          ((u32)0x00000001 << 8)          /*!<R/W 0x0  1: Enable filter */
#define I2C_MASK_IC_DIG_FLTR_DEG         ((u32)0x0000000F << 0)          /*!<R/W 0x0  DIG_FLTR_DEG is to define frequency range of filter. A greater value of DIG_FLTR_DEG results in a slower transfer speed and hardware would be able to filter a lower frequency. */
#define I2C_IC_DIG_FLTR_DEG(x)           ((u32)(((x) & 0x0000000F) << 0))
#define I2C_GET_IC_DIG_FLTR_DEG(x)       ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup IC_SAR2
 * @brief I2C Slave2 Address Register
 * @{
 **/
#define I2C_MASK_IC_SAR2                 ((u32)0x0000007F << 0)          /*!<R/W 0x12  The IC_SAR2 holds the slave address when the I2C is operating as slave2. Slave2 only supports 7 bit address. The register can be written only when the I2C interface is disabled, which corresponds to the IC_ENABLE register being set to 0. Writes at other times have no effect. */
#define I2C_IC_SAR2(x)                   ((u32)(((x) & 0x0000007F) << 0))
#define I2C_GET_IC_SAR2(x)               ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup IC_COMP_VERSION
 * @brief I2C Component Version Register
 * @{
 **/
#define I2C_MASK_IC_COMP_VERSION         ((u32)0xFFFFFFFF << 0)          /*!<R 0x20200911  I2C version number */
#define I2C_IC_COMP_VERSION(x)           ((u32)(((x) & 0xFFFFFFFF) << 0))
#define I2C_GET_IC_COMP_VERSION(x)       ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup I2C_Exported_Types I2C Exported Types
  * @{
*****************************************************************************/


/**
 * @brief I2C Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t IC_CON;                                 /*!< I2C CONTROL REGISTER,  Address offset: 0x000 */
	__IO uint32_t IC_TAR;                                 /*!< I2C TARGET ADDRESS REGISTER,  Address offset: 0x004 */
	__IO uint32_t IC_SAR;                                 /*!< I2C SLAVE ADDRESS REGISTER,  Address offset: 0x008 */
	__IO uint32_t IC_HS_MAR;                              /*!< I2C HIGH SPEED MASTER MODE CODE ADDRESS REGISTER,  Address offset: 0x00C */
	__IO uint32_t IC_DATA_CMD;                            /*!< I2C RX/TX DATA BUFFER AND COMMAND REGISTER,  Address offset: 0x010 */
	__IO uint32_t IC_SS_SCL_HCNT;                         /*!< STANDARD SPEED I2C CLOCK SCL HIGH COUNT REGISTER,  Address offset: 0x014 */
	__IO uint32_t IC_SS_SCL_LCNT;                         /*!< STANDARD SPEED I2C CLOCK SCL LOW COUNT REGISTER,  Address offset: 0x018 */
	__IO uint32_t IC_FS_SCL_HCNT;                         /*!< FAST SPEED I2C CLOCK SCL HIGH COUNT REGISTER,  Address offset: 0x01C */
	__IO uint32_t IC_FS_SCL_LCNT;                         /*!< FAST SPEED I2C CLOCK SCL LOW COUNT REGISTER,  Address offset: 0x020 */
	__IO uint32_t IC_HS_SCL_HCNT;                         /*!< HIGH SPEED I2C CLOCK SCL HIGH COUNT REGISTER,  Address offset: 0x024 */
	__IO uint32_t IC_HS_SCL_LCNT;                         /*!< HIGH SPEED I2C CLOCK SCL LOW COUNT REGISTER,  Address offset: 0x028 */
	__I  uint32_t IC_INTR_STAT;                           /*!< I2C INTERRUPT STATUS REGISTER,  Address offset: 0x02C */
	__IO uint32_t IC_INTR_MASK;                           /*!< I2C INTERRUPT MASK REGISTER,  Address offset: 0x030 */
	__I  uint32_t IC_RAW_INTR_STAT;                       /*!< I2C RAW INTERRUPT STATUS REGISTER,  Address offset: 0x034 */
	__IO uint32_t IC_RX_TL;                               /*!< I2C RECEIVE FIFO THRESHOLD REGISTER,  Address offset: 0x038 */
	__IO uint32_t IC_TX_TL;                               /*!< I2C TRANSMIT FIFO THRESHOLD REGISTER,  Address offset: 0x03C */
	__I  uint32_t IC_CLR_INTR;                            /*!< CLEAR COMBINED AND INDIVIDUAL INTERRUPT REGISTER,  Address offset: 0x040 */
	__I  uint32_t IC_CLR_RX_UNDER;                        /*!< CLEAR RX_UNDER INTERRPT REGISTER,  Address offset: 0x044 */
	__I  uint32_t IC_CLR_RX_OVER;                         /*!< CLEAR RX_OVER INTERRPT REGISTER,  Address offset: 0x048 */
	__I  uint32_t IC_CLR_TX_OVER;                         /*!< CLEAR TX_OVER INTERRPT REGISTER,  Address offset: 0x04C */
	__I  uint32_t IC_CLR_RD_REQ;                          /*!< CLEAR RD_REQ INTERRPT REGISTER,  Address offset: 0x050 */
	__I  uint32_t IC_CLR_TX_ABRT;                         /*!< CLEAR TX_ABRT INTERRPT REGISTER,  Address offset: 0x054 */
	__I  uint32_t IC_CLR_RX_DONE;                         /*!< CLEAR RX_DONE INTERRPT REGISTER,  Address offset: 0x058 */
	__I  uint32_t IC_CLR_ACTIVITY;                        /*!< CLEAR ACTIVITY INTERRPT REGISTER,  Address offset: 0x05C */
	__I  uint32_t IC_CLR_STOP_DET;                        /*!< CLEAR STOP_DET INTERRPT REGISTER,  Address offset: 0x060 */
	__I  uint32_t IC_CLR_START_DET;                       /*!< CLEAR START_DET INTERRPT REGISTER,  Address offset: 0x064 */
	__I  uint32_t IC_CLR_GEN_CALL;                        /*!< CLEAR GEN_ALL INTERRPT REGISTER,  Address offset: 0x068 */
	__IO uint32_t IC_ENABLE;                              /*!< I2C ENABLE REGISTER,  Address offset: 0x06C */
	__I  uint32_t IC_STATUS;                              /*!< I2C STATUS REGISTER,  Address offset: 0x070 */
	__I  uint32_t IC_TXFLR;                               /*!< I2C TRANSMIT FIFO LEVEL REGISTER,  Address offset: 0x074 */
	__I  uint32_t IC_RXFLR;                               /*!< I2C RECEIVE FIFO LEVEL REGISTER,  Address offset: 0x078 */
	__IO uint32_t IC_SDA_HOLD;                            /*!< I2C SDA HOLD TIME LENGTH REGISTER,  Address offset: 0x07C */
	__I  uint32_t IC_TX_ABRT_SOURCE;                      /*!< I2C TRANSMIT ABORT SOURCE REGISTER,  Address offset: 0x080 */
	__IO uint32_t IC_SLV_DATA_NACK_ONLY;                  /*!< GENERATE SLAVE DATA NACK REGISTER,  Address offset: 0x084 */
	__IO uint32_t IC_DMA_CR;                              /*!< DMA CONTROL REGISTER,  Address offset: 0x088 */
	__IO uint32_t IC_DMA_TDLR;                            /*!< DMA TRANSMIT DATA LEVEL REGISTER,  Address offset: 0x08C */
	__IO uint32_t IC_DMA_RDLR;                            /*!< I2C REVEIVE DATA LEVEL REGISTER,  Address offset: 0x090 */
	__IO uint32_t IC_SDA_SETUP;                           /*!< I2C SDA SETUP REGISTER,  Address offset: 0x094 */
	__IO uint32_t IC_ACK_GENERAL_CALL;                    /*!< I2C ACK GENERAL CALL REGISTER,  Address offset: 0x098 */
	__I  uint32_t IC_ENABLE_STATUS;                       /*!< I2C ENABLE STATUS REGISTER,  Address offset: 0x09C */
	__IO uint32_t IC_DMA_CMD;                             /*!< I2C DMA COMMAND REGISTER,  Address offset: 0x0A0 */
	__IO uint32_t IC_DMA_DATA_LEN;                        /*!< I2C DMA MODE TRANSFER DATA LENGTH REGISTER,  Address offset: 0x0A4 */
	__IO uint32_t IC_DMA_MODE;                            /*!< I2C DMA MODE REGISTER,  Address offset: 0x0A8 */
	__IO uint32_t IC_SLEEP;                               /*!< I2C SLEEP MODE REGISTER,  Address offset: 0x0AC */
	__IO uint32_t IC_DEBUG_SEL;                           /*!< I2C DEBUG SEL REGISTER,  Address offset: 0x0B0 */
	__IO uint32_t IC_OUT_SMP_DLY;                         /*!<  Register,  Address offset: 0x0B4 */
	__IO uint32_t RSVD0[11];                              /*!<  Reserved,  Address offset:0xB8-0xE0 */
	__I  uint32_t IC_CLR_ADDR_MATCH;                      /*!< CLEAR SLAVE MODE ADDRESS MATCH INTERRUPT REGISTER,  Address offset: 0x0E4 */
	__IO uint32_t IC_CLR_DMA_DONE;                        /*!< CLEAR DMA DONE INTERRUPT REGISTER,  Address offset: 0x0E8 */
	__IO uint32_t IC_FILTER;                              /*!< I2C FILTER REGISTER,  Address offset: 0x0EC */
	__IO uint32_t RSVD1;                                  /*!<  Reserved,  Address offset:0xF0 */
	__IO uint32_t IC_SAR2;                                /*!< I2C SLAVE2 ADDRESS REGISTER,  Address offset: 0x0F4 */
	__IO uint32_t RSVD2;                                  /*!<  Reserved,  Address offset:0xF8 */
	__I  uint32_t IC_COMP_VERSION;                        /*!< I2C COMPONENT VERSION REGISTER,  Address offset: 0x0FC */
} I2C_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup I2C_Exported_Types I2C Exported Types
  * @{
  */

/**
  * @brief  I2C Init Structure Definition
  */
typedef struct {
	u32	I2CIdx;         /*!< Specifies the I2C Device Index.
				This parameter should be 0 */

	u32	I2CMaster;      /*!< Specifies the I2C Role.
				This parameter can be a value of @ref I2C_Role_Mode */

	u32	I2CAddrMod;     /*!< Specifies the I2C Addressing Mode.
				This parameter can be a value of @ref I2C_Addr_Mode */

	u32	I2CSpdMod;      /*!< Specifies the I2C Speed Mode. Now the circuit don't support High Speed Mode.
				This parameter can be a value of @ref I2C_Speed_Mode */

	u32	I2CRXTL;        /*!< Specifies the I2C RX FIFO Threshold. It controls the level of
				entries(or above) that triggers the RX_FULL interrupt.
				This parameter must be set to a value in the 0-255 range. A value of 0 sets
				the threshold for 1 entry, and a value of 255 sets the threshold for 256 entry*/

	u32	I2CTXTL;        /*!< Specifies the I2C TX FIFO Threshold.It controls the level of
				entries(or below) that triggers the TX_EMPTY interrupt.
				This parameter must be set to a value in the 0-255 range. A value of 0 sets
				the threshold for 0 entry, and a value of 255 sets the threshold for 255 entry*/
	u32	I2CMstReSTR;    /*!< Specifies the I2C Restart Support of Master. */

	u32	I2CMstGC;       /*!< Specifies the I2C General Call Support of Master. */

	u32	I2CMstStartB;   /*!< Specifies the I2C Start Byte Support of Master. */

	u32	I2CSlvNoAck;    /*!< Specifies the I2C Slave No Ack Support. */

	u32	I2CSlvAckGC;    /*!< Specifies the I2C Slave Acks to General Call. */

	u32	I2CAckAddr;     /*!< Specifies the I2C Target Address in I2C Master Mode or
				Ack Address in I2C Slave0 Mode.
				This parameter must be set to a value in the 0-127 range if the I2C_ADDR_7BIT
				is selected or 0-1023 range if the I2C_ADDR_10BIT is selected. */

	u32	I2CSlvSetup;       /*!< Specifies the I2C SDA Setup Time. It controls the amount of time delay
				introduced in the rising edge of SCL—relative to SDA changing—by holding SCL low
				when I2C Device operating as a slave transmitter, in units of ic_clk period.
				This parameter must be set to a value in the 0-255 range. It must be set larger than I2CSdaHd */

	u32	I2CSdaHd;       /*!< Specifies the I2C SDA Hold Time. It controls the amount of
				hold time on the SDA signal after a negative edge of SCL in both master
				and slave mode, in units of ic_clk period.
				This parameter must be set to a value in the 0-0xFFFF range. */

	u32	I2CClk;         /*!< Specifies the I2C Bus Clock (in kHz). It is closely related to I2CSpdMod */

	u32	I2CIPClk;		/*!< Specifies the I2C IP Clock (in Hz). */

	u32	I2CFilter;      /*!< Specifies the I2C SCL/SDA Spike Filter. */
	u32	I2CAckAddr1;    /*!< Specifies the I2C Ack Address in I2C Slave1 Mode. I2C Slave1 only
				support I2C_ADDR_7BIT mode. This parameter must be set to a value in the 0-127 range. */
} I2C_InitTypeDef;

/**
  * @brief  I2C IntMode Structure Definition
  */
typedef struct {
	void (*I2CSendSem)(u32 IsWrite);    /*!< Interface for releasing semaphores */
	void (*I2CWaitSem)(u32 IsWrite);    /*!< Interface for acquiring semaphores */
	I2C_TypeDef *I2Cx;
} I2C_IntModeCtrl;

/**
  * @brief  I2C dev Table Definition
  */
typedef struct {
	I2C_TypeDef *I2Cx;
	IRQn_Type IrqNum;
} I2C_DevTable;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup I2C_Exported_Constants I2C Exported Constants
  * @{
  */

/** @defgroup I2C_Addr_Mode
  * @{
  */
#define I2C_ADDR_7BIT			((u32)0x00000000)
#define I2C_ADDR_10BIT			((u32)0x00000001)
#define IS_I2C_ADDR_MODE(MODE) (((MODE) == I2C_ADDR_7BIT) || \
									   ((MODE) == I2C_ADDR_10BIT))
/**
  * @}
  */

/** @defgroup I2C_Speed_Mode
  * @{
  */
#define I2C_SS_MODE				((u32)0x00000001)
#define I2C_FS_MODE				((u32)0x00000002)
#define I2C_HS_MODE				((u32)0x00000003)
#define IS_I2C_SPEED_MODE(MODE) (((MODE) == I2C_SS_MODE) || \
									   ((MODE) == I2C_FS_MODE) || \
									   ((MODE) == I2C_HS_MODE))
/**
  * @}
  */

/** @defgroup I2C_Role_Mode
  * @{
  */
#define I2C_SLAVE_MODE			((u32)0x00000000)
#define I2C_MASTER_MODE		((u32)0x00000001)
/**
  * @}
  */

/** @defgroup I2C_Clock
  * @{
  */
#define I2C0_1_IPCLK			100000000
/**
  * @}
  */


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup I2C_Exported_Functions I2C Exported Functions
  * @{
  */

/** @defgroup I2C_Exported_Normal_Functions I2C Exported Normal Functions
  * @{
  */
_LONG_CALL_ void I2C_Init(I2C_TypeDef *I2Cx, I2C_InitTypeDef *I2C_InitStruct);
_LONG_CALL_ void I2C_Cmd(I2C_TypeDef *I2Cx, u8 NewState);
_LONG_CALL_ void I2C_ClearAllINT(I2C_TypeDef *I2Cx);
_LONG_CALL_ u32 I2C_GetRawINT(I2C_TypeDef *I2Cx);
_LONG_CALL_ u32 I2C_GetINT(I2C_TypeDef *I2Cx);
_LONG_CALL_ u8 I2C_CheckFlagState(I2C_TypeDef *I2Cx, u32 I2C_FLAG);
_LONG_CALL_ void I2C_INTConfig(I2C_TypeDef *I2Cx, u32 I2C_IT, u32 NewState);
_LONG_CALL_ void I2C_ClearINT(I2C_TypeDef *I2Cx, u32 INTrAddr);
_LONG_CALL_ void I2C_SetSpeed(I2C_TypeDef *I2Cx, u32 SpdMd, u32 I2Clk, u32 I2CIPClk);
_LONG_CALL_ void I2C_StructInit(I2C_InitTypeDef *I2C_InitStruct);
_LONG_CALL_ u8 I2C_ReceiveData(I2C_TypeDef *I2Cx);
/**
  * @}
  */

/** @defgroup I2C_Exported_Master_Functions I2C Exported Master Functions
  * @{
  */
_LONG_CALL_ void I2C_MasterSendNullData(I2C_TypeDef *I2Cx, u8 *pBuf, u8  I2CCmd, u8  I2CStop, u8  I2CReSTR);
_LONG_CALL_ void I2C_MasterSend(I2C_TypeDef *I2Cx, u8 *pBuf, u8  I2CCmd, u8  I2CStop, u8  I2CReSTR);
_LONG_CALL_ void I2C_MasterWrite(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ void I2C_MasterReadDW(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ u32 I2C_MasterRead(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ void I2C_MasterRepeatRead(I2C_TypeDef *I2Cx, u8 *pWriteBuf, u32 Writelen, u8 *pReadBuf, u32 Readlen);
_LONG_CALL_ void I2C_SetSlaveAddress(I2C_TypeDef *I2Cx, u16 Address);
_LONG_CALL_ u8 I2C_MasterRead_TimeOut(I2C_TypeDef *I2Cx, u8 *pBuf, u8 len, u32 ms);
_LONG_CALL_ s32 I2C_MasterSendNullData_TimeOut(I2C_TypeDef *I2Cx, int address, u32 timeout_ms);
_LONG_CALL_ u8 I2C_MasterWrite_TimeOut(I2C_TypeDef *I2Cx, u8 *pBuf, u8 len, u32 ms);
_LONG_CALL_ u32 I2C_MasterWriteBrk(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ u32 I2C_MasterWriteInt(I2C_TypeDef *I2Cx, I2C_IntModeCtrl *I2C_SemStruct, u8 *pBuf, u32 len);
_LONG_CALL_ u32 I2C_MasterReadInt(I2C_TypeDef *I2Cx, I2C_IntModeCtrl *I2C_SemStruct, u8 *pBuf, u32 len);
_LONG_CALL_ void I2C_MasterISRHandle(I2C_IntModeCtrl *I2C_SemStruct);

/**
  * @}
  */

/** @defgroup I2C_Exported_Slave_Functions I2C Exported Slave Functions
  * @{
  */
_LONG_CALL_ u32 I2C_SlaveWrite(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ void I2C_SlaveRead(I2C_TypeDef *I2Cx, u8 *pBuf, u32 len);
_LONG_CALL_ void I2C_SlaveSend(I2C_TypeDef *I2Cx, u8 Data);
/**
  * @}
  */

/** @defgroup I2C_Exported_PowerSave_Functions I2C Exported PowerSave Functions
  * @{
  */
_LONG_CALL_ void I2C_Sleep_Cmd(I2C_TypeDef *I2Cx, u32 NewStatus);
_LONG_CALL_ void I2C_WakeUp(I2C_TypeDef *I2Cx);
/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other Definitions --------------------------------------------------------*/
#if 1
//I2C Timing Parameters
#define I2C_SS_MIN_SCL_HTIME		4600    //the unit is ns.
#define I2C_SS_MIN_SCL_LTIME		5400    //the unit is ns.

#define I2C_FS_MIN_SCL_HTIME		800     //the unit is ns.
#define I2C_FS_MIN_SCL_LTIME		1700    //the unit is ns.

#define I2C_HS_MIN_SCL_HTIME_100    100      //the unit is ns, with bus loading = 100pf, Maximum supported speed=3.4M
#define I2C_HS_MIN_SCL_LTIME_100    194     //the unit is ns., with bus loading = 100pf, Maximum supported speed=3.4M

#define I2C_HS_MIN_SCL_HTIME_400    200     //the unit is ns, with bus loading = 400pf, Maximum supported speed=1.7M
#define I2C_HS_MIN_SCL_LTIME_400    388     //the unit is ns, with bus loading = 400pf, Maximum supported speed=1.7M


extern const I2C_DevTable I2C_DEV_TABLE[2];
extern u32 I2C_SLAVEWRITE_PATCH;
extern u32 IC_FS_SCL_HCNT_TRIM;
extern u32 IC_FS_SCL_LCNT_TRIM;
#define I2C_EARLY_RX_DONE 			-1
#define I2C_TRX_BUFFER_DEPTH 16

#endif

/* MANUAL_GEN_END */

#endif
