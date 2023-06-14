#ifndef _AMEBAD2_SPI_H_
#define _AMEBAD2_SPI_H_

/** @addtogroup SPI
  * @verbatim
  *****************************************************************************************
  * Introduction
  *****************************************************************************************
  * SPI0:
  *		- Support Motorola SPI interface
  *		- Role: Master or Slave
  *		- Base Address: SPI0_DEV
  *		- Bus Clk: 100MHz
  *		- BaudRate: less than or equal to 50M
  *		- Transfer mode:Tx,Rx,TRx,EEPROM Read when configured as Master; TRx when configured as Slave
  *		- Data Frame Size: 4-16 bits supported
  *		- IRQ Number: SPI0_IRQ
  *		- GDMA TX handshake interface: GDMA_HANDSHAKE_INTERFACE_SPI0_TX
  *		- GDMA RX handshake interface: GDMA_HANDSHAKE_INTERFACE_SPI0_RX
  *
  * SPI1:
  *		- Support Motorola SPI interface
  *		- Role: Master or Slave
  *		- Base Address: SPI1_DEV
  *		- Bus Clk: 100MHz
  *		- BaudRate: less than or equal to 100M
  *		- Transfer mode:Tx,Rx,TRx,EEPROM Read when configured as Master; TRx when configured as Slave
  *		- Data Frame Size: 4-16 bits supported
  *		- IRQ Number: SPI1_IRQ
  *		- GDMA TX handshake interface: GDMA_HANDSHAKE_INTERFACE_SPI1_TX
  *		- GDMA RX handshake interface: GDMA_HANDSHAKE_INTERFACE_SPI1_RX
  *
  *****************************************************************************************
  * How to use Normal SPI
  *****************************************************************************************
  *	To use the SPI in DMA mode, the following steps are mandatory:
  *
  *      1. Enable peripheral clock using the following functions:
  *			-RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE) for SPI0;
  *			-RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, ENABLE) for SPI1;
  *
  *      2. Configure the SPIx pinmux:
  *			-Pinmux_Config(Pin_Num,  PINMUX_FUNCTION_SPI) when configured as Master;
  *			-Pinmux_Config(Pin_Num,  PINMUX_FUNCTION_SPI) when configured as Slave;
  *
  *      3. Program the Polarity,Phase,Transfer Mode,Baud Rate Prescaler,DataFrameSize,
  *			Interrupt TRx Threshold level,DMA TRx Threshold level and other parameters using
  *			SSI_StructInit() and change some parameters if needed
  *
  *      4. Init Hardware use step3 parameters:
  *			SSI_Init(SPI_TypeDef *spi_dev, SSI_InitTypeDef *SSI_InitStruct)
  *
  *      5. Enable the SPI:
  *			SSI_Cmd()
  *
  *      6. When using poll:
  *			-Using SSI_Writeable() function to make sure that the transmit FIFO is not full,
  *			then using SSI_WriteData() function to send data
  *
  *			-Using SSI_Readable() function to make sure that the receive FIFO is not empty,
  *			then using SSI_ReadData() function to receive data
  *
  *      7. Enable the NVIC and the corresponding interrupt using following function if you need
  *			to use interrupt mode.
  *			-SSI_INTConfig(): SPI IRQ Mask set
  *			-InterruptRegister(): register the SPI irq handler
  *			-InterruptEn(): Enable the NVIC interrupt and set irq priority
  *
  *
  *      @note in SPI_Exported_Functions group, these functions below are about Interrupts
  *			and flags management:
  *			-SSI_GetIsr()
  *			-SSI_GetRawIsr()
  *			-SSI_INTConfig()
  *			-SSI_SetRxFifoLevel()
  *			-SSI_SetTxFifoLevel()
  *			-SSI_SetIsrClean()
  *
  *
  *****************************************************************************************
  * How to use SPI in DMA mode
  *****************************************************************************************
  *	To use the SPI in DMA mode, the following steps are mandatory:
  *
  *      1. Enable peripheral clock using the following functions:
  *			-RCC_PeriphClockCmd(APBPeriph_SPI0, APBPeriph_SPI0_CLOCK, ENABLE) for SPI0;
  *			-RCC_PeriphClockCmd(APBPeriph_SPI1, APBPeriph_SPI1_CLOCK, ENABLE) for SPI1;
  *
  *      2. Configure the SPIx pinmux:
  *			-Pinmux_Config(Pin_Num,  PINMUX_FUNCTION_SPI) when configured as Master;
  *			-Pinmux_Config(Pin_Num,  PINMUX_FUNCTION_SPI) when configured as Slave;
  *
  *      3. Program the Polarity,Phase,Transfer Mode,Baud Rate Prescaler,DataFrameSize,
  *			Interrupt TRx Threshold level,DMA TRx Threshold level and other parameters using
  *			SSI_StructInit() and change some parameters if needed
  *
  *      4. Init Hardware use step3 parameters:
  *			SSI_Init(SPI_TypeDef *spi_dev, SSI_InitTypeDef *SSI_InitStruct)
  *
  *      5. Enable the SPI:
  *			SSI_Cmd()
  *
  *      6. GDMA related configurations(DMA burst size/source address/destination address/block size etc).
  *
  *      7. Active the SPI  DMA TX/RX using SSI_SetDmaEnable() function.
  *
  *      @note in SPI_Exported_Functions group, these functions below are about DMA:
  *			-SSI_SetDmaEnable()
  *			-SSI_SetDmaLevel()
  *
  *****************************************************************************************
  * @endverbatim
  */

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup SPI_Register_Definitions SPI Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup SPI_CTRLR0
 * @brief Control Register 0
 * @{
 **/
#define SPI_BIT_SS_T            ((u32)0x00000001 << 31)          /*!<R/W 0  When SCPH is 0 (Relevant only when the SPI is configured as a serial-master device) 0: ss_n_out doesn't toggle between successive frames 1: ss_n_out does toggle between successive frames */
#define SPI_BIT_RXBITSWAP       ((u32)0x00000001 << 24)          /*!<R/W 0  0: Order of receive bit doesn't swap 1: Order of receive bit does swap */
#define SPI_BIT_RXBYTESWAP      ((u32)0x00000001 << 23)          /*!<R/W 0  0: Order of receive byte doesn't swap 1: Order of receive byte does swap */
#define SPI_BIT_TXBITSWAP       ((u32)0x00000001 << 22)          /*!<R/W 0  0: Order of transmit bit doesn't swap 1: Order of transmit bit does swap */
#define SPI_BIT_TXBYTESWAP      ((u32)0x00000001 << 21)          /*!<R/W 0  0: Order of transmit byte doesn't swap 1: Order of transmit byte does swap */
#define SPI_BIT_SLV_OE          ((u32)0x00000001 << 10)          /*!<R/W 0  Slave Output Enable. Relevant only when the SPI is configured as a serial-slave device. When configured as a serial master, this bit field has no functionality. This bit enables or disables the setting of the ssi_oe_n output from the SPI serial slave. When SLV_OE = 1, the ssi_oe_n output can never be active. When the ssi_oe_n output controls the tri-state buffer on the txd output from the slave, a high impedance state is always present on the slave txd output when SLV_OE = 1. This is useful when the master transmits in broadcast mode (master transmits data to all slave devices). Only one slave may respond with data on the master rxd line. This bit is enabled after reset and must be disabled by software (when broadcast mode is used), if you do not want this device to respond with data. 0: Slave txd is enabled 1: Slave txd is disabled */
#define SPI_MASK_TMOD           ((u32)0x00000003 << 8)          /*!<R/W 00  Transfer Mode. Relevant only when the SPI is configured as a serial-master device. Selects the mode of transfer for serial communication. This field does not affect the transfer duplicity. Only indicates whether the receive or transmit data are valid. In transmit-only mode, data received from the external device is not valid and is not stored in the receive FIFO memory; it is overwritten on the next transfer. In receive-only mode, transmitted data are not valid. After the first write to the transmit FIFO, the same word is retransmitted for the duration of the transfer. In transmit-and-receive mode, both transmit and receive data are valid. The transfer continues until the transmit FIFO is empty. Data received from the external device are stored into the receive FIFO memory, where it can be accessed by the host processor. 00: Transmit & Receive 01: Transmit Only 10: Receive Only */
#define SPI_TMOD(x)             ((u32)(((x) & 0x00000003) << 8))
#define SPI_GET_TMOD(x)         ((u32)(((x >> 8) & 0x00000003)))
#define SPI_BIT_SCPOL           ((u32)0x00000001 << 7)          /*!<R/W 0  Serial Clock Polarity. Valid when the frame format (FRF) is set to Motorola SPI. Used to select the polarity of the inactive serial clock, which is held inactive when the SPI master is not actively transferring data on the serial bus. 0: Inactive state of serial clock is low 1: Inactive state of serial clock is high */
#define SPI_BIT_SCPH            ((u32)0x00000001 << 6)          /*!<R/W 0  Serial Clock Phase. Valid when the frame format (FRF) is set to Motorola SPI. The serial clock phase selects the relationship of the serial clock with the slave select signal. When SCPH = 0, data are captured on the first edge of the serial clock. When SCPH = 1, the serial clock starts toggling one cycle after the slave select line is activated, and data are captured on the second edge of the serial clock. 0: Serial clock toggles in middle of the first data bit 1: Serial clock toggles at start of the first data bit */
#define SPI_MASK_FRF            ((u32)0x00000003 << 4)          /*!<R/W 00  Frame Format. Selects which serial protocol transfers the data. 00: Motorola SPI Other: Reserved */
#define SPI_FRF(x)              ((u32)(((x) & 0x00000003) << 4))
#define SPI_GET_FRF(x)          ((u32)(((x >> 4) & 0x00000003)))
#define SPI_MASK_DFS            ((u32)0x0000000F << 0)          /*!<R/W 0111  Data Frame Size. Selects the data frame length. When the data frame size is programmed to be less than 16 bits, the receive data are automatically right-justified by the receive logic, with the upper bits of the receive FIFO zero-padded. You must right-justify transmit data before writing into the transmit FIFO. The transmit logic ignores the upper unused bits when transmitting the data. For the DFS decode, refer to the following description: 0000/0001/0010: Reserved (undefined operation) 0011: 4-bit serial data transfer 0100: 5-bit serial data transfer 0101: 6-bit serial data transfer 0110: 7-bit serial data transfer 0111: 8-bit serial data transfer 1000: 9-bit serial data transfer 1001: 10-bit serial data transfer 1010: 11-bit serial data transfer 1011: 12-bit serial data transfer 1100: 13-bit serial data transfer 1101: 14-bit serial data transfer 1110: 15-bit serial data transfer 1111: 16-bit serial data transfer */
#define SPI_DFS(x)              ((u32)(((x) & 0x0000000F) << 0))
#define SPI_GET_DFS(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup SPI_CTRLR1
 * @brief Control Register 1
 * @{
 **/
#define SPI_MASK_NDF            ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  Number of Data Frames. When TMOD = 10 or TMOD = 11, this register field sets the number of data frames to be continuously received by the SPI. The SPI continues to receive serial data until the number of data frames received is equal to this register value plus 1, which enables you to receive up to 64 KB of data in a continuous transfer. When the SPI is configured as a serial slave, the transfer continues for as long as the slave is selected. Therefore, this register serves no purpose and is not present when the SPI is configured as a serial slave. */
#define SPI_NDF(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define SPI_GET_NDF(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SPI_SSIENR
 * @brief SSI Enable Register
 * @{
 **/
#define SPI_BIT_SSI_EN          ((u32)0x00000001 << 0)          /*!<R/W 0  SPI Enable. Enables and disables all SPI operations. When disabled, all serial transfers are halted immediately. Transmit and receive FIFO buffers are cleared when the device is disabled. It is impossible to program some of the SPI control registers when enabled. When disabled, the ssi_sleep output is set (after delay) to inform the system that it is safe to remove the ssi_clk, thus saving power consumption in the system. */
/** @} */

/** @defgroup SPI_SER
 * @brief Slave Enable Register
 * @{
 **/
#define SPI_BIT_SER             ((u32)0x00000001 << 0)          /*!<R/W 0  Slave Select Enable Flag. When this bit is set (1), the corresponding slave select line from the master is activated when a serial transfer begins. It should be noted that setting or clearing bits in this register have no effect on the corresponding slave select outputs until a transfer is started. Before beginning a transfer, you should enable the bit in this register that corresponds to the slave device with which the master wants to communicate. 1: Selected 0: Not selected */
/** @} */

/** @defgroup SPI_BAUDR
 * @brief Baud Rate Select
 * @{
 **/
#define SPI_MASK_SCKDV          ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  SPI Clock Divider. The LSB for this field is always set to 0 and is unaffected by a write operation, which ensures an even value is held in this register. If the value is 0, the serial output clock (sclk_out) is disabled. The frequency of the sclk_out is derived from the equation: Fssi_clk = Fsclk_out/SCKDV, where SCKDV is any even value between 2 and 65534. */
#define SPI_SCKDV(x)            ((u32)(((x) & 0x0000FFFF) << 0))
#define SPI_GET_SCKDV(x)        ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SPI_TXFTLR
 * @brief Transmit FIFO Threshold Level
 * @{
 **/
#define SPI_MASK_TFT            ((u32)0x0000003F << 0)          /*!<R/W 0x0  Transmit FIFO Threshold. Controls the level of entries (or below) at which the transmit FIFO controller triggers an interrupt. The FIFO depth is 64; this register is sized to the number of address bits needed to access the FIFO. If you attempt to set this value greater than or equal to the depth of the FIFO, this field is not written and retains its current value. When the number of transmit FIFO entries is less than or equal to this value, the transmit FIFO empty interrupt is triggered. For TFT decode, refer to the following description: 000000: ssi_txe_intr is asserted when 0 data entry is present in transmit FIFO. 000001: ssi_txe_intr is asserted when 1 data entry is present in transmit FIFO. 000010: ssi_txe_intr is asserted when 2 data entries are present in transmit FIFO. 000011: ssi_txe_intr is asserted when 3 data entries are present in transmit FIFO. ... 111100: ssi_txe_intr is asserted when 60 data entries are present in transmit FIFO. 111101: ssi_txe_intr is asserted when 61 data entries are present in transmit FIFO. 111110: ssi_txe_intr is asserted when 62 data entries are present in transmit FIFO. 111111: ssi_txe_intr is asserted when 63 data entries are present in transmit FIFO. */
#define SPI_TFT(x)              ((u32)(((x) & 0x0000003F) << 0))
#define SPI_GET_TFT(x)          ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SPI_RXFTLR
 * @brief Receive FIFO Threshold Level
 * @{
 **/
#define SPI_MASK_RFT            ((u32)0x0000003F << 0)          /*!<R/W 0x0  Receive FIFO Threshold. Controls the level of entries (or above) at which the receive FIFO controller triggers an interrupt. The FIFO depth is configurable in the range 2~64. This register is sized to the number of address bits needed to access the FIFO. If you attempt to set this value greater than the depth of the FIFO, this field is not written and retains its current value. When the number of receive FIFO entries is greater than or equal to this value + 1, the receive FIFO full interrupt is triggered. For RFT decode, refer to the following description: 000000: ssi_rxf_intr is asserted when 1 or more data entries are present in receive FIFO. 000001: ssi_rxf_intr is asserted when 2 or more data entries are present in receive FIFO. 000010: ssi_rxf_intr is asserted when 3 or more data entries are present in receive FIFO. 000011: ssi_rxf_intr is asserted when 4 or more data entries are present in receive FIFO. ... 111110: ssi_rxf_intr is asserted when 61 or more data entries are present in receive FIFO. 111101: ssi_rxf_intr is asserted when 62 or more data entries are present in receive FIFO. 111110: ssi_rxf_intr is asserted when 63 or more data entries are present in receive FIFO. 111111: ssi_rxf_intr is asserted when 64 or more data entries are present in receive FIFO. */
#define SPI_RFT(x)              ((u32)(((x) & 0x0000003F) << 0))
#define SPI_GET_RFT(x)          ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SPI_TXFLR
 * @brief Transmit FIFO Level Register
 * @{
 **/
#define SPI_MASK_TXTFL          ((u32)0x0000007F << 0)          /*!<R 0x0  Transmit FIFO Level. Contains the number of valid data entries in the transmit FIFO. */
#define SPI_TXTFL(x)            ((u32)(((x) & 0x0000007F) << 0))
#define SPI_GET_TXTFL(x)        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup SPI_RXFLR
 * @brief Receive FIFO Level Register
 * @{
 **/
#define SPI_MASK_RXTFL          ((u32)0x0000007F << 0)          /*!<R 0x0  Receive FIFO Level. Contains the number of valid data entries in the receive FIFO. */
#define SPI_RXTFL(x)            ((u32)(((x) & 0x0000007F) << 0))
#define SPI_GET_RXTFL(x)        ((u32)(((x >> 0) & 0x0000007F)))
/** @} */

/** @defgroup SPI_SR
 * @brief Status Register
 * @{
 **/
#define SPI_BIT_DCOL            ((u32)0x00000001 << 6)          /*!<R 0  Data Collision Error. Relevant only when the SPI is configured as a master device. This bit is set if the SPI master is actively transmitting when another master selects this device as a slave. This informs the processor that the last data transfer was halted before completion. This bit is cleared when read. 0: No error 1: Transmit data collision error */
#define SPI_BIT_TXE             ((u32)0x00000001 << 5)          /*!<R 0  Transmission Error. Set if the transmit FIFO is empty when a transfer is started. This bit can be set only when the SPI is configured as a slave device. Data from the previous transmission is resent on the txd line. This bit is cleared when read. 0: No error 1: Transmission error */
#define SPI_BIT_RFF             ((u32)0x00000001 << 4)          /*!<R 0  Receive FIFO Full. When the receive FIFO is completely full, this bit is set. When the receive FIFO contains one or more empty location, this bit is cleared. 0: Receive FIFO is not full 1: Receive FIFO is full */
#define SPI_BIT_RFNE            ((u32)0x00000001 << 3)          /*!<R 0  Receive FIFO Not Empty. Set when the receive FIFO contains one or more entries and is cleared when the receive FIFO is empty. This bit can be polled by software to completely empty the receive FIFO. 0: Receive FIFO is empty 1: Receive FIFO is not empty */
#define SPI_BIT_TFE             ((u32)0x00000001 << 2)          /*!<R 1  Transmit FIFO Empty. When the transmit FIFO is completely empty, this bit is set. When the transmit FIFO contains one or more valid entries, this bit is cleared. This bit field does not request an interrupt. 0: Transmit FIFO is not empty 1: Transmit FIFO is empty */
#define SPI_BIT_TFNF            ((u32)0x00000001 << 1)          /*!<R 1  Transmit FIFO Not Full. Set when the transmit FIFO contains one or more empty locations, and is cleared when the FIFO is full. 0: Transmit FIFO is full 1: Transmit FIFO is not full */
#define SPI_BIT_BUSY            ((u32)0x00000001 << 0)          /*!<R 0  SSI Busy Flag. When set, indicates that a serial transfer is in progress; when cleared indicates that the SPI is idle or disabled. 0: SPI is idle or disabled 1: SPI is actively transferring data */
/** @} */

/** @defgroup SPI_IMR
 * @brief Interrupt Mask Register
 * @{
 **/
#define SPI_BIT_SSRIM           ((u32)0x00000001 << 7)          /*!<R/W 1  SS_N Rising Edge Detect Interrupt Mask. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_ssr_intr is masked 1: ssi_ssr_intr is not masked */
#define SPI_BIT_TXUIM           ((u32)0x00000001 << 6)          /*!<R/W 1  Transmit FIFO Underflow Interrupt Mask. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_txu_intr is masked 1: ssi_txu_intr is not masked */
#define SPI_BIT_MSTIM_FAEIM     ((u32)0x00000001 << 5)          /*!<R/W 1  When SPI is configured as serial-master, this bit field is present as Multi-Master Contention Interrupt Mask. 0: ssi_mst_intr interrupt is masked 1: ssi_mst_intr interrupt is not masked When SPI is configured as serial-slave, this bit field is present as Frame Alignment Interrupt Mask. 0: ssi_fae_intr interrupt is masked 1: ssi_fae_intr interrupt is not masked */
#define SPI_BIT_RXFIM           ((u32)0x00000001 << 4)          /*!<R/W 1  Receive FIFO Full Interrupt Mask. 0: ssi_rxf_intr interrupt is masked 1: ssi_rxf_intr interrupt is not masked */
#define SPI_BIT_RXOIM           ((u32)0x00000001 << 3)          /*!<R/W 1  Receive FIFO Overflow Interrupt Mask. 0: ssi_rxo_intr interrupt is masked 1: ssi_rxo_intr interrupt is not masked */
#define SPI_BIT_RXUIM           ((u32)0x00000001 << 2)          /*!<R/W 1  Receive FIFO Underflow Interrupt Mask. 0: ssi_rxu_intr interrupt is masked 1: ssi_rxu_intr interrupt is not masked */
#define SPI_BIT_TXOIM           ((u32)0x00000001 << 1)          /*!<R/W 1  Transmit FIFO Overflow Interrupt Mask. 0: ssi_txo_intr interrupt is masked 1: ssi_txo_intr interrupt is not masked */
#define SPI_BIT_TXEIM           ((u32)0x00000001 << 0)          /*!<R/W 1  Transmit FIFO Empty Interrupt Mask. 0: ssi_txe_intr interrupt is masked 1: ssi_txe_intr interrupt is not masked */
/** @} */

/** @defgroup SPI_ISR
 * @brief Interrupt Status Register
 * @{
 **/
#define SPI_BIT_SSRIS           ((u32)0x00000001 << 7)          /*!<R 0  SS_N Rising Edge Detect Interrupt Status. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_ssr_intr interrupt is not active after masking 1: ssi_ssr_intr interrupt is active after masking */
#define SPI_BIT_TXUIS           ((u32)0x00000001 << 6)          /*!<R 0  Transmit FIFO Under Flow Interrupt Status. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_txu_intr interrupt is not active after masking 1: ssi_txu_intr interrupt is active after masking */
#define SPI_BIT_MSTIS_FAEIS     ((u32)0x00000001 << 5)          /*!<R 0  When SPI is configured as serial-master, this bit field is present as Multi-Master Contention Interrupt Status. 0: ssi_mst_intr interrupt is not active after masking 1: ssi_mst_intr interrupt is active after masking When SPI is configured as serial-slave, this bit field is present as Frame Alignment Interrupt Status. 0: ssi_fae_intr interrupt not active after masking 1: ssi_fae_intr interrupt is active after masking */
#define SPI_BIT_RXFIS           ((u32)0x00000001 << 4)          /*!<R 0  Receive FIFO Full Interrupt Status. 0: ssi_rxf_intr interrupt is not active after masking 1: ssi_rxf_intr interrupt is full after masking */
#define SPI_BIT_RXOIS           ((u32)0x00000001 << 3)          /*!<R 0  Receive FIFO Overflow Interrupt Status. 0: ssi_rxo_intr interrupt is not active after masking 1: ssi_rxo_intr interrupt is active after masking */
#define SPI_BIT_RXUIS           ((u32)0x00000001 << 2)          /*!<R 0  Receive FIFO Underflow Interrupt Status. 0: ssi_rxu_intr interrupt is not active after masking 1: ssi_rxu_intr interrupt is active after masking */
#define SPI_BIT_TXOIS           ((u32)0x00000001 << 1)          /*!<R 0  Transmit FIFO Overflow Interrupt Status. 0: ssi_txo_intr interrupt is not active after masking 1: ssi_txo_intr interrupt is active after masking */
#define SPI_BIT_TXEIS           ((u32)0x00000001 << 0)          /*!<R 0  Transmit FIFO Empty Interrupt Status. 0: ssi_txe_intr interrupt is not active after masking 1: ssi_txe_intr interrupt is active after masking */
/** @} */

/** @defgroup SPI_RISR
 * @brief Raw Interrupt Status Register
 * @{
 **/
#define SPI_BIT_SSRIR           ((u32)0x00000001 << 7)          /*!<R 0  SS_N Rising Edge Detect Raw Interrupt Status. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_ssr_intr interrupt is not active prior to masking 1: ssi_ssr_intr interrupt is active prior to masking */
#define SPI_BIT_TXUIR           ((u32)0x00000001 << 6)          /*!<R 0  Transmit FIFO Under Flow Raw Interrupt Status. This bit field is present only if the SPI is configured as a serial-slave device. 0: ssi_txu_intr interrupt is not active prior to masking 1: ssi_txu_intr interrupt is active prior to masking */
#define SPI_BIT_MSTIR_FAEIR     ((u32)0x00000001 << 5)          /*!<R 0  When SPI is configured as serial-master, this bit field is present as Multi-Master Contention Raw Interrupt Status. 0: ssi_mst_intr interrupt is not active prior to masking 1: ssi_mst_intr interrupt is active prior to masking When SPI is configured as serial-slave, this bit field is present as Frame Alignment Error Raw Interrupt Status. 0: ssi_fae_intr interrupt not active prior to masking 1: ssi_fae_intr interrupt is active prior to masking */
#define SPI_BIT_RXFIR           ((u32)0x00000001 << 4)          /*!<R 0  Receive FIFO Full Raw Interrupt Status. 0: ssi_rxf_intr interrupt is not active prior to masking 1: ssi_rxf_intr interrupt is active prior to masking */
#define SPI_BIT_RXOIR           ((u32)0x00000001 << 3)          /*!<R 0  Receive FIFO Overflow Raw Interrupt Status. 0: ssi_rxo_intr interrupt is not active prior to masking 1: ssi_rxo_intr interrupt is active prior to masking */
#define SPI_BIT_RXUIR           ((u32)0x00000001 << 2)          /*!<R 0  Receive FIFO Underflow Raw Interrupt Status. 0: ssi_rxu_intr interrupt is not active prior to masking 1: ssi_rxu_intr interrupt is active prior to masking */
#define SPI_BIT_TXOIR           ((u32)0x00000001 << 1)          /*!<R 0  Transmit FIFO Overflow Raw Interrupt Status. 0: ssi_txo_intr interrupt is not active prior to masking 1: ssi_txo_intr interrupt is active prior to masking */
#define SPI_BIT_TXEIR           ((u32)0x00000001 << 0)          /*!<R 0  Transmit FIFO Empty Raw Interrupt Status. 0: ssi_txe_intr interrupt is not active prior to masking 1: ssi_txe_intr interrupt is active prior to masking */
/** @} */

/** @defgroup SPI_TXOICR
 * @brief Transmit FIFO Overflow Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_TXOICR          ((u32)0x00000001 << 0)          /*!<R 0  Clear Transmit FIFO Overflow Interrupt. This register reflects the status of the interrupt. A read from this register clears the ssi_txo_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_RXOICR
 * @brief Receive FIFO Overflow Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_RXOICR          ((u32)0x00000001 << 0)          /*!<R 0  Clear Receive FIFO Overflow Interrupt. This register reflects the status of the interrupt. A read from this register clears the ssi_rxo_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_RXUICR
 * @brief Receive FIFO Underflow Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_RXUICR          ((u32)0x00000001 << 0)          /*!<R 0  Clear Receive FIFO Underflow Interrupt. This register reflects the status of the interrupt. A read from this register clears the ssi_rxu_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_MSTICR_FAEICR
 * @brief Multi
 * @{
 **/
#define SPI_BIT_MSTICR_FAEICR   ((u32)0x00000001 << 0)          /*!<R 0  Multi-Master Interrupt Clear Register/Frame Alignment Error Interrupt Clear Register. When SPI is configured as serial-master, this bit field is used to Clear Multi-Master Contention Interrupt. A read from this register clears the ssi_mst_intr interrupt; writing has no effect. When SPI is configured as serial-slave, this bit field is used to Clear Frame Alignment Interrupt. A read from this register clears the ssi_fae_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_ICR
 * @brief Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_ICR             ((u32)0x00000001 << 0)          /*!<R 0  Clear Interrupt. This register is set if any of the interrupts below are active. A read clears the ssi_txo_intr, ssi_rxu_intr, ssi_rxo_intr, and the ssi_mst_intr/ssi_fae_intr interrupts. Writing to this register has no effect. */
/** @} */

/** @defgroup SPI_DMACR
 * @brief DMA Control Register
 * @{
 **/
#define SPI_BIT_TDMAE           ((u32)0x00000001 << 1)          /*!<R/W 0  Transmit DMA Enable. This bit enables/disables the transmit FIFO DMA channel. 0: Transmit DMA disabled 1: Transmit DMA enabled */
#define SPI_BIT_RDMAE           ((u32)0x00000001 << 0)          /*!<R/W 0  Receive DMA Enable. This bit enables/disables the receive FIFO DMA channel 0: Receive DMA disabled 1: Receive DMA enabled */
/** @} */

/** @defgroup SPI_DMATDLR
 * @brief DMA Transmit Data Level
 * @{
 **/
#define SPI_MASK_DMATDL         ((u32)0x0000003F << 0)          /*!<R/W 0x0  Transmit Data Level. This bit field controls the level at which a DMA request is made by the transmit logic. It is equal to the watermark level; that is, the dma_tx_req signal is generated when the number of valid data entries in the transmit FIFO is equal to or below this field value, and TDMAE = 1. For DMATDL decode, refer to the following description: 000000: dma_tx_req is asserted when 0 data entry is present in transmit FIFO. 000001: dma_tx_req is asserted when 1 data entry is present in transmit FIFO. 000010: dma_tx_req is asserted when 2 data entries are present in transmit FIFO. 000011: dma_tx_req is asserted when 3 data entries are present in transmit FIFO. ... 111100: dma_tx_req is asserted when 60 data entries are present in transmit FIFO. 111101: dma_tx_req is asserted when 61 data entries are present in transmit FIFO. 111110: dma_tx_req is asserted when 62 data entries are present in transmit FIFO. 111111: dma_tx_req is asserted when 63 data entries are present in transmit FIFO. */
#define SPI_DMATDL(x)           ((u32)(((x) & 0x0000003F) << 0))
#define SPI_GET_DMATDL(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SPI_DMARDLR
 * @brief DMA Receive Data Level
 * @{
 **/
#define SPI_MASK_DMARDL         ((u32)0x0000003F << 0)          /*!<R/W 0x0  Receive Data Level. This bit field controls the level at which a DMA request is made by the receive logic. The watermark level = DMARDL+1; that is, dma_rx_req is generated when the number of valid data entries in the receive FIFO is equal to or above this field value + 1, and RDMAE=1. For DMARDL decode, refer to the following description: 000000: dma_rx_req is asserted when 1 or more data entries are present in transmit FIFO. 000001: dma_rx_req is asserted when 2 or more data entries are present in transmit FIFO. 000010: dma_rx_req is asserted when 3 or more data entries are present in transmit FIFO. 000011: dma_rx_req is asserted when 4 or more data entries are present in transmit FIFO. ... 111100: dma_rx_req is asserted when 61 or more data entries are present in transmit FIFO. 111101: dma_rx_req is asserted when 62 or more data entries are present in transmit FIFO. 111110: dma_rx_req is asserted when 63 or more data entries are present in transmit FIFO. 111111: dma_rx_req is asserted when 64 data entries are present in transmit FIFO. */
#define SPI_DMARDL(x)           ((u32)(((x) & 0x0000003F) << 0))
#define SPI_GET_DMARDL(x)       ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SPI_TXUICR
 * @brief Transmit FIFO Underflow Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_TXUICR          ((u32)0x00000001 << 0)          /*!<R 0  When SPI is configured as serial-slave, this register is used to Clear Transmit FIFO Underflow Interrupt. This register reflects the status of the interrupt. A read from this register clears the ssi_txu_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_SSRICR
 * @brief SS_N Rising Edge Detect Interrupt Clear Register
 * @{
 **/
#define SPI_BIT_SSRICR          ((u32)0x00000001 << 0)          /*!<R 0  When SPI is configured as serial-slave, this register is used to Clear SS_N Rinsing Edge Detect Interrupt. This register reflects the status of the interrupt. A read from this register clears the ssi_ssr_intr interrupt; writing has no effect. */
/** @} */

/** @defgroup SPI_DRx
 * @brief Data Register x
 * @{
 **/
#define SPI_MASK_DRx            ((u32)0x0000FFFF << 0)          /*!<R/W 0x0  Data Register. When writing to this register, you must right-justify the data. Read data are automatically right-justified. Read: Receive FIFO buffer Write: Transmit FIFO buffer */
#define SPI_DRx(x)              ((u32)(((x) & 0x0000FFFF) << 0))
#define SPI_GET_DRx(x)          ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup SPI_RX_SAMPLE_DLY
 * @brief rxd Sample Delay Register
 * @{
 **/
#define SPI_MASK_RSD            ((u32)0x000000FF << 0)          /*!<R/W 0x0  Receive Data (rxd) Sample Delay. This register is used to delay the sample of the rxd input signal. Each value represents a single ssi_clk delay on the sample of the rxd signal. */
#define SPI_RSD(x)              ((u32)(((x) & 0x000000FF) << 0))
#define SPI_GET_RSD(x)          ((u32)(((x >> 0) & 0x000000FF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup SPI_Exported_Types SPI Exported Types
  * @{
*****************************************************************************/


/**
 * @brief SPI Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t SPI_CTRLR0;                             /*!< CONTROL REGISTER 0 Register,  Address offset: 0x00 */
	__IO uint32_t SPI_CTRLR1;                             /*!< CONTROL REGISTER 1 Register,  Address offset: 0x04 */
	__IO uint32_t SPI_SSIENR;                             /*!< SSI ENABLE REGISTER,  Address offset: 0x08 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__IO uint32_t SPI_SER;                                /*!< SLAVE ENABLE REGISTER,  Address offset: 0x10 */
	__IO uint32_t SPI_BAUDR;                              /*!< BAUD RATE SELECT Register,  Address offset: 0x14 */
	__IO uint32_t SPI_TXFTLR;                             /*!< TRANSMIT FIFO THRESHOLD LEVEL Register,  Address offset: 0x18 */
	__IO uint32_t SPI_RXFTLR;                             /*!< RECEIVE FIFO THRESHOLD LEVEL Register,  Address offset: 0x1C */
	__I  uint32_t SPI_TXFLR;                              /*!< TRANSMIT FIFO LEVEL REGISTER,  Address offset: 0x20 */
	__I  uint32_t SPI_RXFLR;                              /*!< RECEIVE FIFO LEVEL REGISTER,  Address offset: 0x24 */
	__I  uint32_t SPI_SR;                                 /*!< STATUS REGISTER,  Address offset: 0x28 */
	__IO uint32_t SPI_IMR;                                /*!< INTERRUPT MASK REGISTER,  Address offset: 0x2C */
	__I  uint32_t SPI_ISR;                                /*!< INTERRUPT STATUS REGISTER,  Address offset: 0x30 */
	__I  uint32_t SPI_RISR;                               /*!< RAW INTERRUPT STATUS REGISTER,  Address offset: 0x34 */
	__I  uint32_t SPI_TXOICR;                             /*!< TRANSMIT FIFO OVERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0x38 */
	__I  uint32_t SPI_RXOICR;                             /*!< RECEIVE FIFO OVERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0x3C */
	__I  uint32_t SPI_RXUICR;                             /*!< RECEIVE FIFO UNDERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0x40 */
	__I  uint32_t SPI_MSTICR_FAEICR;                      /*!< MULTI-MASTER & FRAME ALIGNMENT ERROR INTERRUPT CLEAR REGISTER,  Address offset: 0x44 */
	__I  uint32_t SPI_ICR;                                /*!< INTERRUPT CLEAR REGISTER,  Address offset: 0x48 */
	__IO uint32_t SPI_DMACR;                              /*!< DMA CONTROL REGISTER,  Address offset: 0x4C */
	__IO uint32_t SPI_DMATDLR;                            /*!< DMA TRANSMIT DATA LEVEL Register,  Address offset: 0x50 */
	__IO uint32_t SPI_DMARDLR;                            /*!< DMA RECEIVE DATA LEVEL Register,  Address offset: 0x54 */
	__I  uint32_t SPI_TXUICR;                             /*!< TRANSMIT FIFO UNDERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0x58 */
	__I  uint32_t SPI_SSRICR;                             /*!< SS_N RISING EDGE DETECT INTERRUPT CLEAR REGISTER,  Address offset: 0x5C */
	__IO uint32_t SPI_DRx[36];                            /*!< DATA REGISTER x Register,  Address offset: 0x60-0xEC */
	__IO uint32_t SPI_RX_SAMPLE_DLY;                      /*!< RXD SAMPLE DELAY REGISTER,  Address offset: 0xF0 */
} SPI_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported Types --------------------------------------------------------*/
/** @addtogroup SPI_Exported_Types SPI Exported Types
  * @{
  */

/**
  * @brief  SPI Init Structure Definition
  */
typedef struct {
	u32  SPI_DmaRxDataLevel;     /*!< Specifies the DMA receive data level.
							     The dma_rx_req is generated when the number of valid data entries in the
							     receive FIFO  is equal to or above this field value+1,and RDMAE=1.
							     @note For Amebaz, the value range of this parameter should be 0 to 63,because
							     the depth of Rx FIFO is 64. */

	u32  SPI_DmaTxDataLevel;    /*!< Specifies the DMA transmit data level.
							     The dma_tx_req is generated when the number of valid data entries in the
							     transmit FIFO is equal to or below this field value,and TDMAE=1.
							     @note For Amebaz, the value range of this parameter should be 0 to 63,because
							     the depth of Rx FIFO is 64. */

	u32  SPI_RxThresholdLevel;   /*!< Specifies the receive FIFO threshold level.
							     This Parameter controls the level of entries(or above) at which the receive FIFO controller
							     triggers an interrupt.When the number of receive FIFO entries is greater than or equal to this
							     value +1,the receive FIFO full interrupt is triggered.
							      @note For Amebaz, the value range of this parameter should be 0 to 63,because the depth
							      of Rx FIFO is 64. */

	u32  SPI_TxThresholdLevel;     /*!< Specifies the transmit FIFO threshold level.
							     This Parameter controls the level of entries (or below) at which the transmit FIFO controller
							     triggers an interrupt.When the number of transmit FIFO entries is less than or equal to this
							     value,the transmit FIFO empty interrupt is triggered.
							      @note For Amebaz, the value range of this parameter should be 0 to 63,because of the depth
							      of Rx FIFO is 64. */

	u32  SPI_SlaveSelectEnable;     /*!< Set the slave select enable flag.
							     This Parameter controls which slave to be selected by master,each bit in SER register
							     corresponds to a slave select line(ss_x_n) from spi master.
							     @note The default vlaue of this parameter is 0,and one slave is selected.if more slaves to be selected,
							     you may use SW way to do this.And this parameter is used only when the device is master. */

	u32  SPI_ClockDivider;		    /*!< Specifies the SPI Baud Rate.
							     The value of sclk_out equals to ssi_clk devides the value of this parameter
							     @note The LSB for this field is always set to 0 and is unaffected by a write operation,which ensures
							     an even value is held. */

	u32  SPI_DataFrameNumber;   /*!< Specifies the number of data frames master wants to receive .
							     When TMOD=10 or TMOD=11,Ctrl1 register uses this value to set the number of data frames to
							     be continuous received.
							     @note The value of this parameter should be set to the number of data frames that to be received
							     minus one.And this parameter is used only when the device is master. */

	u32   SPI_DataFrameFormat;    /*!< Selects which serial protocol transfers the data .
							    This parameter can be a value of @ref SPI_Frame_Format. */

	u32   SPI_DataFrameSize;        /*!< Selects the data frame length .
							    This parameter can be a value of @ref SPI_Data_Frame_Size.
							    @note Need to right-justify transmit data before writting into the transmit FIFO
							    The transmit logic ignores the upper unused bits when transmitting the data. */

	u32   SPI_InterruptMask;	     /*!< Specifies which interrupt to be enable.
							     Each bit in this parameter corresponds to a specific interrupt.*/

	u32   SPI_Role;                        /*!< Specifies the role of SPI device.
							    This parameter can be a value of @ref SPI_Role. */

	u32   SPI_SclkPhase;		    /*!< Specifies the serial clock phase.
							    When SPI_SclkPhase = 0, data are captured on the first edge of the serial clock. When SPI_SclkPhase = 1,
							    the serial clock starts toggling one cycle after the slave select line is activated, and data
							    are captured on the second edge of the serial clock.
							    This parameter can be a value of @ref SPI_SC_Phase.
							    @note Valid when the frame format(FRF) is set to Motorola SPI. */

	u32   SPI_SclkPolarity;              /*!< Specifies the serial clock polarity.
							    When SPI_SclkPolarity = 0, the serial clock remains low when idle. When SPI_SclkPolarity = 1,
							    the serial clock remains high when idle.
							     This parameter can be a value of @ref SPI_SC_Polarity.
							    @note Valid when the frame format(FRF) is set to Motorola SPI.*/

	u32   SPI_TransferMode;         /*!< Selects the mode of transfer for serial communication.
							    This parameter can be a value of @ref SPI_TMOD.
							    @note This transfer mode is only valid when the DW_apb_ssi is configured as a master device.*/
} SSI_InitTypeDef;

/**
  * @brief  SPI Dev table Structure Definition
  */
typedef struct {
	SPI_TypeDef *SPIx;
	u32 Tx_HandshakeInterface;
	u32 Rx_HandshakeInterface;
	IRQn_Type IrqNum;
} SPI_DevTable;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup SPI_Exported_Constants SPI Exported Constants
  * @{
  */

/** @defgroup SPI_TMOD
  * @{
  */
#define TMOD_TR				(0)
#define TMOD_TO				(1)
#define TMOD_RO				(2)
/**
  * @}
  */

/** @defgroup SPI_SC_Polarity
  * @{
  */
#define SCPOL_INACTIVE_IS_LOW		(0)
#define SCPOL_INACTIVE_IS_HIGH		(1)
/**
  * @}
  */

/** @defgroup SPI_SC_Phase
  * @{
  */
#define SCPH_TOGGLES_IN_MIDDLE		(0)
#define SCPH_TOGGLES_AT_START		(1)
/**
  * @}
  */

/** @defgroup SPI_Data_Frame_Size
  * @{
  */
#define DFS_4_BITS				(3)
#define DFS_5_BITS				(4)
#define DFS_6_BITS				(5)
#define DFS_7_BITS				(6)
#define DFS_8_BITS				(7)
#define DFS_9_BITS				(8)
#define DFS_10_BITS				(9)
#define DFS_11_BITS				(10)
#define DFS_12_BITS				(11)
#define DFS_13_BITS				(12)
#define DFS_14_BITS				(13)
#define DFS_15_BITS				(14)
#define DFS_16_BITS				(15)
/**
  * @}
  */

/** @defgroup SPI_SS_Toggle_Phase
  * @{
  */
#define SPI_SS_NOT_TOGGLE		(0)
#define SPI_SS_TOGGLE		(1)
/**
  * @}
  */

/** @defgroup SPI_Role
  * @{
  */
#define SSI_SLAVE				(0)
#define SSI_MASTER				(1)
/**
  * @}
  */

/** @defgroup SPI_Frame_Format
  * @{
  */
#define FRF_MOTOROLA_SPI		(0)
/**
  * @}
  */

/** @defgroup SPI_DMA_Control
  * @{
  */
#define SSI_NODMA				(0)
#define SSI_RXDMA_ENABLE		(1)
#define SSI_TXDMA_ENABLE		(2)
#define SSI_TRDMA_ENABLE		(3)
/**
  * @}
  */

/** @defgroup SPI_FIFO_Depth
  * @{
  */
#define SSI_TX_FIFO_DEPTH				(64)
#define SSI_RX_FIFO_DEPTH				(64)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SPI_Exported_Functions SPI Exported Functions
  * @{
  */

/** @defgroup SPI_Exported_Normal_Functions SPI Exported Normal Functions
  * @{
  */
_LONG_CALL_ void SSI_Cmd(SPI_TypeDef *spi_dev, u32 NewStaus);
_LONG_CALL_ void SSI_SetSclkPolarity(SPI_TypeDef *spi_dev, u32 SclkPolarity);
_LONG_CALL_ void SSI_SetSclkPhase(SPI_TypeDef *spi_dev, u32 SclkPhase);
_LONG_CALL_ void SSI_WriteData(SPI_TypeDef *spi_dev, u32 value);
_LONG_CALL_ VOID SSI_INTConfig(SPI_TypeDef *spi_dev, u32 SSI_IT, u32 newState);
_LONG_CALL_ void SSI_SetRxFifoLevel(SPI_TypeDef *spi_dev, u32 RxThresholdLevel);
_LONG_CALL_ void SSI_SetTxFifoLevel(SPI_TypeDef *spi_dev, u32 TxThresholdLevel);
_LONG_CALL_ void SSI_SetSlaveEnable(SPI_TypeDef *spi_dev, u32 SlaveIndex);
_LONG_CALL_ u32 SSI_Busy(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_Writeable(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_Readable(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetRxCount(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetTxCount(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetStatus(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetIsr(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_ReadData(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_ReceiveData(SPI_TypeDef *spi_dev, void *RxData, u32 Length);
_LONG_CALL_ u32 SSI_SendData(SPI_TypeDef *spi_dev, void *TxData, u32 Length, u32 Role);
_LONG_CALL_ u32 SSI_GetRawIsr(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetSlaveEnable(SPI_TypeDef *spi_dev);
_LONG_CALL_ u32 SSI_GetDataFrameSize(SPI_TypeDef *spi_dev);
_LONG_CALL_ void SSI_SetSampleDelay(SPI_TypeDef *spi_dev, u32 SampleDelay);
_LONG_CALL_ void SSI_Init(SPI_TypeDef *spi_dev, SSI_InitTypeDef *SSI_InitStruct);
_LONG_CALL_ void SSI_StructInit(SSI_InitTypeDef *SSI_InitStruct);
_LONG_CALL_ void SSI_SetDataFrameSize(SPI_TypeDef *spi_dev, u32 DataFrameSize);
_LONG_CALL_ void SSI_SetSSTogglePhase(SPI_TypeDef *spi_dev, u32 TogglePhase);
_LONG_CALL_ void SSI_SetDataSwap(SPI_TypeDef *spi_dev, u32 SwapStatus, u32 newState);
_LONG_CALL_ void SSI_SetBaud(SPI_TypeDef *SPIx, u32 BaudRate, u32 IpClk);
_LONG_CALL_ void SSI_SetIsrClean(SPI_TypeDef *spi_dev, u32 InterruptStatus);
_LONG_CALL_ void SSI_SetReadLen(SPI_TypeDef *spi_dev, u32 DataFrameNumber);
_LONG_CALL_ void SSI_SetBaudDiv(SPI_TypeDef *spi_dev, u32 ClockDivider);
_LONG_CALL_ void SSI_SetRole(SPI_TypeDef *spi_dev, u32 role);
/**
  * @}
  */

/** @defgroup SPI_Exported_DMA_Functions SPI Exported DMA Functions
  * @{
  */
_LONG_CALL_ BOOL SSI_TXGDMA_Init(u32 Index, PGDMA_InitTypeDef GDMA_InitStruct, void *CallbackData,
								 IRQ_FUN CallbackFunc, u8 *pTxData, u32 Length);
_LONG_CALL_ BOOL SSI_RXGDMA_Init(u8 Index, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData,
								 IRQ_FUN CallbackFunc, u8  *pRxData, u32 Length);
_LONG_CALL_ void SSI_SetDmaEnable(SPI_TypeDef *spi_dev, u32 newState, u32 Mask);
_LONG_CALL_ void SSI_SetDmaLevel(SPI_TypeDef *spi_dev, u32 TxLeve, u32 RxLevel);
/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other Definitions --------------------------------------------------------*/

extern const SPI_DevTable SPI_DEV_TABLE[2];

/* MANUAL_GEN_END */

#endif
