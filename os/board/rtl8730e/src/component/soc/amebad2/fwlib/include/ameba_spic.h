#ifndef _AMEBAD2_SPIC_H_
#define _AMEBAD2_SPIC_H_

#include "ameba_flashclk.h"

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @defgroup SPIC
  * @brief SPIC driver modules
  * @{
  */

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup SPIC_Register_Definitions SPIC Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup CTRLR0
 * @brief Control Register 0
 * @{
 **/
#define BIT_USER_MODE              ((u32)0x00000001 << 31)          /*!<R/W 0x0  User mode bit. Enable to enter user mode. Disable to enter auto mode. It can not be changed while SPIC is busy (0x28 [0] BUSY). 1: User mode. User can push data to FIFO and set SSIENR to 1. SPIC won't accept auto read/write command. 0: Auto mode. User can't push data to FIFO and set SSIENR to 1. SPIC will accept auto read/write command. */
#define BIT_UAR                    ((u32)0x00000001 << 30)          /*!<R/W 0x0  User mode bit auto reset. Enable to auto reset USER_MODE to 0 after current user mode transaction is over (SPIC_EN reset to 0). */
#define MASK_CK_MTIMES             ((u32)0x0000001F << 23)          /*!<R/W ATCK_MTIMES  Indicates the check times. If Flash is always busy in auto-check times, it causes ACEIR interrupt. The delay time (cycles) is related to bus_clk. The cycles of the parameter = (CK_MTIMES << (11+ATCK_BIT_EXTEND).). User should ensure the timeout > Write time of Flash The timeout time(ns) = 2*BAUDR*(1/SPIC_freq)(8/CH) (CK_MTIMES << (11+ATCK_BIT_EXTEND).) Example 1: Flash Program time= 1ms, BAUDR=1, CH=1 (SPI) and (1/SPIC_freq) = 10ns. ATCK_MTIMES should be to set 4. Example 2: SPIC_freq = 200MHz, BAUDR = 1, CH =4 (QSPI), and ATCK_BIT_EXTEND = 2, the maximum auto_check time should be = 2*1*(1/200MHz)(8/4) (5'b11111<<13) = 2*1*(1/200MHz)(8/4) (18'b11_1110_0000_0000_0000) = 2*1*(1/200MHz)(8/4) (18'h3_e000) = 2*1*(1/200MHz)(8/4) (253952) = 5079040 ns = 5.07 ms Suggestion: If ACEIR interrupt occurs, the user should check the flash status by the software itself. Note: Before SVN 8369 Version, CK_MTIMES is fixed to shift 10bit; The formula become2*BAUDR*(1/SPIC_freq)(8/CH) (CK_MTIMES << (10)) */
#define CK_MTIMES(x)               ((u32)(((x) & 0x0000001F) << 23))
#define GET_CK_MTIMES(x)           ((u32)(((x >> 23) & 0x0000001F)))
#define BIT_FAST_RD                ((u32)0x00000001 << 22)          /*!<R/W 0x0  Indicates to use fast read command in user mode. If setting to 1, SPIC would use FBAUDR to generate spi_sclk. */
#define MASK_CMD_CH                ((u32)0x00000003 << 20)          /*!<R/W 0x0  Indicates channel number of command phase in transmitting or receiving data. Command phase is usually used to send SPI command. 0: single channel, 1: dual channels, 2: quad channels, 3: octal channel. */
#define CMD_CH(x)                  ((u32)(((x) & 0x00000003) << 20))
#define GET_CMD_CH(x)              ((u32)(((x >> 20) & 0x00000003)))
#define MASK_DATA_CH               ((u32)0x00000003 << 18)          /*!<R/W 0x0  Indicates channel number of data phase in transmitting or receiving data. Data phase is used to send data after address phase. 0: single channel, 1: dual channels, 2: quad channels, 3: octal channel. */
#define DATA_CH(x)                 ((u32)(((x) & 0x00000003) << 18))
#define GET_DATA_CH(x)             ((u32)(((x >> 18) & 0x00000003)))
#define MASK_ADDR_CH               ((u32)0x00000003 << 16)          /*!<R/W 0x0  Indicates channel number of address phase after command phase. Addr phase is used to send address or data. Addr phase is between one-byte command and a data phase. The number of bytes is determined by the ADDR_LENGTH. 0: single channel, 1: dual channels, 2: quad channels, 3: octal channel. */
#define ADDR_CH(x)                 ((u32)(((x) & 0x00000003) << 16))
#define GET_ADDR_CH(x)             ((u32)(((x >> 16) & 0x00000003)))
#define MASK_DDR_EN                ((u32)0x00000007 << 13)          /*!<R/W 0x0  Indicates the DDR mode in CMD_CH/DATA_CH/ADDR_CH. CTRLR0[15]: CMD_CH (always 2-Byte CMD type) CTRLR0[14]: DATA_CH CTRLR0[13]: ADDR_CH 0: Disable 1: Enable */
#define DDR_EN(x)                  ((u32)(((x) & 0x00000007) << 13))
#define GET_DDR_EN(x)              ((u32)(((x >> 13) & 0x00000007)))
#define BIT_SPI_DREIR_R_DIS        ((u32)0x00000001 << 11)          /*!<R/W 0x0  Set to disable DR timeout check */
#define BIT_GCLK_DIS               ((u32)0x00000001 << 10)          /*!<R/W 0x0  Set to disable gated clock of ICG cell */
#define MASK_TMOD                  ((u32)0x00000003 << 8)          /*!<R/W 0x0  Indicates transfer mode. 2'b00: transmit mode Others (or 2'b11): receive mode */
#define TMOD(x)                    ((u32)(((x) & 0x00000003) << 8))
#define GET_TMOD(x)                ((u32)(((x >> 8) & 0x00000003)))
#define BIT_SCPOL                  ((u32)0x00000001 << 7)          /*!<R/W SCPOL_DEF  Indicates serial clock polarity. It is used to select the polarity of the inactive serial clock. 0: inactive state of serial clock is low 1: inactive state of serial clock is high */
#define BIT_SCPH                   ((u32)0x00000001 << 6)          /*!<R/W SCPH_DEF  Indicates serial clock phase. The serial clock phase selects the relationship the serial clock with the slave select signal. 0: serial clock toggles in middle of first data bit 1: serial clock toggles at start of first data bit */
#define BIT_SIPOL_EN               ((u32)0x00000001 << 5)          /*!<R/W 0x0  Set to enable SIPOL */
#define MASK_SIPOL                 ((u32)0x0000001F << 0)          /*!<R/W 0x1F  While SIPOL_EN = 1: SPI_CS is inactive state (SPI_CS ==1): spi_sin[3:0] = SIPOL[3:0] SPI_CS is active (SPI_CS ==0): spi_sin[3:2] = SIPOL[3:2] (single or dual channel) (SIPOL[4]: reserved for spi_in[7:4]) */
#define SIPOL(x)                   ((u32)(((x) & 0x0000001F) << 0))
#define GET_SIPOL(x)               ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup RX_NDF
 * @brief Control Register 1
 * @{
 **/
#define MASK_RX_NDF                ((u32)0x00FFFFFF << 0)          /*!<R/W 0x0  Indicates a number of data frames (unit: Byte). When executing receives operation in user mode, SPIC receives data continuously until data frames are equal to RX_NDF. */
#define RX_NDF(x)                  ((u32)(((x) & 0x00FFFFFF) << 0))
#define GET_RX_NDF(x)              ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup SSIENR
 * @brief SPIC Enable Register
 * @{
 **/
#define BIT_PGM_RST_TEST_EN        ((u32)0x00000001 << 4)          /*!<W 0x0  For PGM RST Test only. Set this bit will generate a pulse as a warm reset signal and run PGM RST flow. */
#define BIT_ATCK_CMD               ((u32)0x00000001 << 1)          /*!<R/W 0x0  Set to enable ATCK_CMD implementation. After setting SPIC would not accept any command until checking Flash is not busy or checking time out. (Use in User Mode, especially for Erase/Program to check status automatically.) If CTRLR2.SEQ_SET setting, this function needs to disable. */
#define BIT_SPIC_EN                ((u32)0x00000001 << 0)          /*!<R/W 0x0  Set to enable SPIC, else to disable SPIC. */
/** @} */

/** @defgroup SER
 * @brief Slave Enable Register
 * @{
 **/
#define BIT_SER                    ((u32)0x00000001 << 0)          /*!<R/W 0x1  Each bit in the register corresponds to one SPI Flash. In user mode user program the register to select target flash. 0: Not selected 1: Selected */
/** @} */

/** @defgroup BAUDR
 * @brief Baud Rate Select
 * @{
 **/
#define MASK_SCKDV                 ((u32)0x00000FFF << 0)          /*!<R/W Check SPIC frequency in configure form * *: (spic frequency) / 32 ex: spic freq=120 MHz, SCKDV= ⌊120/32⌋ = ⌊3.75⌋ =3  Define spi_sclk divider value. The frequency of spi_sclk = The frequency of bus_clk / (2*SCKDV). */
#define SCKDV(x)                   ((u32)(((x) & 0x00000FFF) << 0))
#define GET_SCKDV(x)               ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup TXFTLR
 * @brief Transmit FIFO Threshold level
 * @{
 **/
#define MASK_TFT                   ((u32)0x0000001F << 0)          /*!<R/W 0x0  Transmit FIFO threshold. To Control FIFO entries of valid data in normal transfer except in receiving mode. When a number of FIFO entry is equal or smaller than TFT, SPIC triggers a spi_txeir interrupt. Ex: If TXFTLR = 5'h1, set spi_txeir when FIFO entries <= TFT (0x1) */
#define TFT(x)                     ((u32)(((x) & 0x0000001F) << 0))
#define GET_TFT(x)                 ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup RXFTLR
 * @brief Receive FIFO Threshold level
 * @{
 **/
#define MASK_RFT                   ((u32)0x0000001F << 0)          /*!<R/W {FIFO_ABW{1'b1}}  Receive FIFO threshold. To Control FIFO entries of valid data in receiving mode. When a number of FIFO entry is greater than RFT, SPIC triggers a spi_rxfir interrupt. Ex: If RXFTLR = 5'h1, set spi_rxfir when FIFO entries > RFT (0x1) */
#define RFT(x)                     ((u32)(((x) & 0x0000001F) << 0))
#define GET_RFT(x)                 ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup TXFLR
 * @brief Transmit FIFO level Register
 * @{
 **/
#define MASK_TXFLR                 ((u32)0x0000003F << 0)          /*!<R 0x0  Transmit FIFO level. Indicates the FIFO entry level of valid data in normal mode except in receiving data. (or as FIFO_FLR in any mode) */
#define TXFLR(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define GET_TXFLR(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup RXFLR
 * @brief Receive FIFO level Register
 * @{
 **/
#define MASK_RXFLR                 ((u32)0x0000003F << 0)          /*!<R 0x0  Receive FIFO level. Indicates the FIFO entries of valid data in receiving data. */
#define RXFLR(x)                   ((u32)(((x) & 0x0000003F) << 0))
#define GET_RXFLR(x)               ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup SR
 * @brief Status Register
 * @{
 **/
#define BIT_ATWR_RDSR_N            ((u32)0x00000001 << 8)          /*!<R/W 0x0  The previous auto write cmd didn't check the status register (RDSR). User should check the status register of Flash before next user mode transaction. ATWR_RDSR_N will only be set by SPIC when SEQ_WR_EN = 1. */
#define BIT_BOOT_FIN               ((u32)0x00000001 << 7)          /*!<R 0x0  (Not Yet Ready) Boot Finish. Set if count waiting cycles (Boot Delay Count) for SPI Flash becomes a stable state after power on (or system reset). 1: Boot Finish Note: Auto_mode would be blocked until boot finish. User_mode is allowed with SSIENR inactive before boot finish. */
#define BIT_DCOL                   ((u32)0x00000001 << 6)          /*!<R 0x0  Data Collision, or in Transmitting Status. 1: Status shows that SPIC is transmitting spi_flash_cmd/spi_flash_addr/spi_flash_data to SPI Flash. Suggest not reading DR during this transmitting state. (Check this status can avoid reading wrong data and cause SPIC error.) */
#define BIT_TXE                    ((u32)0x00000001 << 5)          /*!<R 0x0  Transmission error. Set if FIFO is empty and starting to transmit data to SPI Flash. This bit is cleared when read. */
#define BIT_RFF                    ((u32)0x00000001 << 4)          /*!<R 0x0  Receive FIFO full. Set if FIFO is full in receiving mode. This bit is cleared when read. */
#define BIT_RFNE                   ((u32)0x00000001 << 3)          /*!<R 0x0  Receive FIFO is not empty. Set if FIFO is not empty in receiving mode. This bit is cleared when read. */
#define BIT_TFE                    ((u32)0x00000001 << 2)          /*!<R 0x1  Transmit FIFO is empty. Set if FIFO is empty in transmit mode, else it is cleared when it has data in FIFO. */
#define BIT_TFNF                   ((u32)0x00000001 << 1)          /*!<R 0x1  Transmit FIFO is not full. Set if FIFO is not full in transmit mode, else it is cleared when FIFO is full. */
#define BIT_BUSY                   ((u32)0x00000001 << 0)          /*!<R 0x0  SPIC busy flag. Set if SPIC is still transmitting to or receiving data from SPI Flash, or TX_FIFO/RX_FIFO are not empty. */
/** @} */

/** @defgroup IMR
 * @brief Interrupt Mask Register
 * @{
 **/
#define BIT_NWEIM                  ((u32)0x00000001 << 16)          /*!<R/W 0x0  NAND Flash write error interrupt mask. 1: spi_nweir_r is not masked. 0: spi_nweir_r is masked. */
#define BIT_STFIM                  ((u32)0x00000001 << 15)          /*!<R/W 0x0  Status FIFO full interrupt mask. 1: spi_stfir is not masked. 0: spi_stfir is masked. */
#define BIT_STOIM                  ((u32)0x00000001 << 14)          /*!<R/W 0x0  Status FIFO overflow interrupt masked. 1: spi_stoir_r is not masked. 0: spi_stoir_r is masked. */
#define BIT_STUIM                  ((u32)0x00000001 << 13)          /*!<R/W 0x0  Status FIFO underflow interrupt masked. 1: spi_stuir_r is not masked. 0: spi_stuir_r is masked. */
#define BIT_DREIM                  ((u32)0x00000001 << 12)          /*!<R/W 0x0  DR timeout error interrupt mask. 1: spi_dreir_r is not masked. 0: spi_dreir_r is masked. */
#define BIT_ACSIM                  ((u32)0x00000001 << 11)          /*!<R/W 0x0  Auto-check Flash Status raw interrupt mask. 1: spi_acsir_r is not masked. 0: spi_acsis_r is masked. */
#define BIT_TFSIM                  ((u32)0x00000001 << 10)          /*!<R/W 0x0  Transmit finish interrupt mask. 1: spi_tfsir_r is not masked. 0: spi_tfsir_r is masked. */
#define BIT_USEIM                  ((u32)0x00000001 << 9)          /*!<R/W 0x0  User_mode error interrupt mask. 1: spi_useir_r is not masked. 0: spi_useir_r is masked. */
#define BIT_ACEIM                  ((u32)0x00000001 << 8)          /*!<R/W 0x1  Auto-check timeout error interrupt mask. 1: spi_aceir_r is not masked. 0: spi_aceir_r is masked. */
#define BIT_BYEIM                  ((u32)0x00000001 << 7)          /*!<R/W 0x1  The Byte-Enable error interrupt mask. 1: spi_byeir_r is not masked. 0: spi_byeir_r is masked. */
#define BIT_WBEIM                  ((u32)0x00000001 << 6)          /*!<R/W 0x1  Write burst error interrupt mask. 1: spi_wbier_r is not masked. 0: spi_wbeir_r is masked. */
#define BIT_FSEIM                  ((u32)0x00000001 << 5)          /*!<R/W 0x1  FIFO size error interrupt mask. 1: spi_fseir_r is not masked. 0: spi_fseir _r is masked */
#define BIT_RXFIM                  ((u32)0x00000001 << 4)          /*!<R/W 0x0  Receive FIFO full interrupt mask. 1: spi_rxfir is not masked. 0: spi_rxfir is masked. */
#define BIT_RXOIM                  ((u32)0x00000001 << 3)          /*!<R/W 0x1  Receive FIFO overflow interrupt masked. 1: spi_rxoir_r is not masked. 0: spi_rxoir_r is masked. */
#define BIT_RXUIM                  ((u32)0x00000001 << 2)          /*!<R/W 0x1  Receive FIFO underflow interrupt masked. 1: spi_rxuir_r is not masked. 0: spi_rxuir_r is masked. */
#define BIT_TXOIM                  ((u32)0x00000001 << 1)          /*!<R/W 0x1  Transmit FIFO overflow interrupt mask. 1: spi_txoir_r is not masked. 0: spi_txoir_r is masked. */
#define BIT_TXEIM                  ((u32)0x00000001 << 0)          /*!<R/W 0x0  Transmit FIFO empty interrupt masked. 1: spi_txeir is not masked. 0: spi_txeir is masked. */
/** @} */

/** @defgroup ISR
 * @brief Interrupt Status Register
 * @{
 **/
#define BIT_NWEIS                  ((u32)0x00000001 << 16)          /*!<R 0x0  NAND Flash write error interrupt status raw interrupt status after masking. 1: spi_nweir_r is active after masking. 0: spi_nweir_r is not active after masking. */
#define BIT_STFIS                  ((u32)0x00000001 << 15)          /*!<R 0x0  Status FIFO full raw interrupt status after masking. 1: spi_stfir_r is active after masking. 0: spi_stfir_r is not active after masking. */
#define BIT_STOIS                  ((u32)0x00000001 << 14)          /*!<R 0x0  Status FIFO overflow interrupt status after masking. 1: spi_stoir_r is active after masking. 0: spi_stoir_r is not active after masking. */
#define BIT_STUIS                  ((u32)0x00000001 << 13)          /*!<R 0x0  Status FIFO underflows interrupt status after masking. 1: spi_stuir_r is active after masking. 0: spi_stuir_r is not active after masking. */
#define BIT_DREIS                  ((u32)0x00000001 << 12)          /*!<R 0x0  DR timeout error status after masking. 1: spi_dreir_r is active after masking. 0: spi_dreir_r is not active after masking. */
#define BIT_ACSIS                  ((u32)0x00000001 << 11)          /*!<R 0x0  Auto-check Flash Status after masking. 1: spi_aceir_r is active after masking. 0: spi_aceir_r is not active after masking. */
#define BIT_TFSIS                  ((u32)0x00000001 << 10)          /*!<R 0x0  Transmit finish interrupt status after masking. 1: spi_tfsir_r is active after masking. 0: spi_tfsir_r is not active after masking. */
#define BIT_USEIS                  ((u32)0x00000001 << 9)          /*!<R 0x0  User mode error status after masking. 1: spi_useir_r is active after masking. 0: spi_useir_r is not active after masking. */
#define BIT_ACEIS                  ((u32)0x00000001 << 8)          /*!<R 0x0  Auto-check timeout error status after masking. 1: spi_aceir_r is active after masking. 0: spi_aceir_r is not active after masking. */
#define BIT_BYEIS                  ((u32)0x00000001 << 7)          /*!<R 0x0  The byte-Enable error interrupts status after masking. 1: spi_byeir_r is active after masking. 0: spi_byeir_r is not active after masking. */
#define BIT_WBEIS                  ((u32)0x00000001 << 6)          /*!<R 0x0  Write burst error interrupt status after masking. 1: spi_wbeir_r is active after masking. 0: spi_wbeir_r is not active after masking. */
#define BIT_FSEIS                  ((u32)0x00000001 << 5)          /*!<R 0x0  FIFO size error interrupts status after masking. 1: spi_fesir_r is active after masking. 0: spi_fesir_r is not active after masking. */
#define BIT_RXFIS                  ((u32)0x00000001 << 4)          /*!<R 0x0  Receive FIFO full interrupt status after masking 1: spi_rxfir is active after masking. 0: spi_rxfir is not active after masking. */
#define BIT_RXOIS                  ((u32)0x00000001 << 3)          /*!<R 0x0  Receive FIFO overflow interrupt status after masking. 1: spi_rxoir_r is active after masking. 0: spi_rxoir_r is not active after masking. */
#define BIT_RXUIS                  ((u32)0x00000001 << 2)          /*!<R 0x0  Receive FIFO underflows interrupt status after masking. 1: spi_rxuir_r is active after masking. 0: spi_rxuir_r is not active after masking. */
#define BIT_TXOIS                  ((u32)0x00000001 << 1)          /*!<R 0x0  Transmit FIFO overflow raw interrupt status after masking. 1: spi_txoir_r is active after masking. 0: spi_txoir_r is not active after masking. */
#define BIT_TXEIS                  ((u32)0x00000001 << 0)          /*!<R 0x0  Transmit FIFO empty raw interrupt status after masking. 1: spi_txeir is active after masking. 0: spi_txeir is not active after masking. */
/** @} */

/** @defgroup RISR
 * @brief Raw Interrupt Status Register
 * @{
 **/
#define BIT_NWEIR                  ((u32)0x00000001 << 16)          /*!<R 0x0  NAND Flash write error interrupt status raw interrupt status prior to masking. 1: spi_nweir_r is active prior to masking. 0: spi_nweir_r is not active prior to masking. */
#define BIT_STFIR                  ((u32)0x00000001 << 15)          /*!<R 0x0  Status FIFO full raw interrupt status prior to masking 1: spi_stfir is active prior to masking. 0: spi_stfir is not active prior to masking. */
#define BIT_STOIR                  ((u32)0x00000001 << 14)          /*!<R 0x0  Status FIFO overflows raw interrupt status prior to masking. 1: spi_stoir_r is active prior to masking. 0: spi_stoir_r is not active prior to masking. */
#define BIT_STUIR                  ((u32)0x00000001 << 13)          /*!<R 0x0  Status FIFO underflows raw interrupt status prior to masking. 1: spi_stuir_r is active prior to masking. 0: spi_stuir_r is not active prior to masking. */
#define BIT_DREIR                  ((u32)0x00000001 << 12)          /*!<R 0x0  DR Timeout error status raw interrupt status prior to masking 1: spi_dreir_r is active prior to masking. 0: spi_dreir_r is not active prior to masking. */
#define BIT_ACSIR                  ((u32)0x00000001 << 11)          /*!<R 0x0  Auto-check Flash Status raw interrupt status prior to masking 1: spi_acsir_r is active prior to masking. 0: spi_acsir_r is not active prior to masking. */
#define BIT_TFSIR                  ((u32)0x00000001 << 10)          /*!<R 0x0  Transmit Finish Status raw interrupt status prior to masking 1: spi_tfsir_r is active prior to masking. 0: spi_tfsir_r is not active prior to masking. */
#define BIT_USEIR                  ((u32)0x00000001 << 9)          /*!<R 0x0  User_mode error status raw interrupt status prior to masking 1: spi_useir_r is active prior to masking. 0: spi_useir_r is not active prior to masking. */
#define BIT_ACEIR                  ((u32)0x00000001 << 8)          /*!<R 0x0  Auto-check timeout error status raw interrupt status prior to masking 1: spi_aceir_r is active prior to masking. 0: spi_aceir_r is not active prior to masking. */
#define BIT_BYEIR                  ((u32)0x00000001 << 7)          /*!<R 0x0  The Byte-Enable error interrupt status raw interrupt status prior to mask. 1: spi_byeir_r is active prior to masking. 0: spi_byeir_r is not active prior to masking. */
#define BIT_WBEIR                  ((u32)0x00000001 << 6)          /*!<R 0x0  Write burst error interrupt status raw interrupt status prior to masking. 1: spi_wbeir_r is active prior to masking. 0: spi_wbeir_r is not active prior to masking. */
#define BIT_FSEIR                  ((u32)0x00000001 << 5)          /*!<R 0x0  FIFO size error interrupt status raw interrupt status prior to masking. 1: spi_fseir_r is active prior to masking. 0: spi_fseir _r is not active prior to masking. */
#define BIT_RXFIR                  ((u32)0x00000001 << 4)          /*!<R 0x0  Receive FIFO full raw interrupt status prior to masking 1:spi_rxfir is active prior to masking. 0: spi_rxfir is not active prior to masking. */
#define BIT_RXOIR                  ((u32)0x00000001 << 3)          /*!<R 0x0  Receive FIFO overflows raw interrupt status prior to masking. 1: spi_rxoir_r is active prior to masking. 0: spi_rxoir_r is not active prior to masking. */
#define BIT_RXUIR                  ((u32)0x00000001 << 2)          /*!<R 0x0  Receive FIFO underflows raw interrupt status prior to masking. 1: spi_rxuir_r is active prior to masking. 0: spi_rxuir_r is not active prior to masking. */
#define BIT_TXOIR                  ((u32)0x00000001 << 1)          /*!<R 0x0  Transmit FIFO overflow raw interrupt status prior to masking. 1: spi_txoir_r is active prior to masking. 0: spi_txoir_r is not active prior to masking. */
#define BIT_TXEIR                  ((u32)0x00000001 << 0)          /*!<R 0x0  Transmit FIFO empty raw interrupt status prior to masking. 1: spi_txeir is active prior to masking. 0: spi_txeir is not active prior to masking. */
/** @} */

/** @defgroup TXOICR
 * @brief Transmit FIFO Overflow Interrupt Clear Register
 * @{
 **/
#define BIT_TXOICR                 ((u32)0x00000001 << 0)          /*!<R/W 0x0  When reading/writing the register, spi_rxoir_r would be cleared. Note: Only response value 0x0 when read. */
/** @} */

/** @defgroup RXOICR
 * @brief Receive FIFO Overflow Interrupt Clear Register
 * @{
 **/
#define BIT_RXOICR                 ((u32)0x00000001 << 0)          /*!<R/W 0x0  When reading/writing the register, spi_rxoir_r would be cleared. Note: Only response value 0x0 when read. */
/** @} */

/** @defgroup RXUICR
 * @brief Receive FIFO Underflow Interrupt Clear Register
 * @{
 **/
#define BIT_RXUICR                 ((u32)0x00000001 << 0)          /*!<R/W 0x0  When reading/writing the register, spi_rxuir_r would be cleared. Note: Only response value 0x0 when read. */
/** @} */

/** @defgroup MSTICR
 * @brief Master error Interrupt Clear Register
 * @{
 **/
#define BIT_MSTICR                 ((u32)0x00000001 << 0)          /*!<R/W 0x0  When reading/writing the register, spi_mstir_r would be cleared. Note: Only response value 0x0 when read. */
/** @} */

/** @defgroup ICR
 * @brief Interrupt Clear Register
 * @{
 **/
#define BIT_ICR                    ((u32)0x00000001 << 0)          /*!<R/W 0x0  When reading/writing the register, all interrupt would be cleared. Note: Only response value 0x0 when read. */
/** @} */

/** @defgroup DMACR
 * @brief DMA Control Register
 * @{
 **/
#define BIT_TX_DMAC_EN             ((u32)0x00000001 << 1)          /*!<R/W 0x0  Transmit DMA Enable. This bit enables/disables the transmit FIFO DMA channel. 0: Transmit DMA disabled 1: Transmit DMA enabled */
#define BIT_RX_DMAC_EN             ((u32)0x00000001 << 0)          /*!<R/W 0x0  Receive DMA Enable. This bit enables/disables the receive FIFO DMA channel 0: Receive DMA disabled 1: Receive DMA enabled */
/** @} */

/** @defgroup DMATDLR
 * @brief DMA Transmit Data Level Register
 * @{
 **/
#define MASK_DMATDL                ((u32)0x0000001F << 0)          /*!<R/W 0x0  Transmit Data Level. This bit field controls the level at which a DMA request is made by the transmit logic. It is equal to the watermark level; that is, the dma_tx_req signal is generated when the number of valid data entries in the transmit FIFO is equal to or below this field value, and TDMAE = 1. */
#define DMATDL(x)                  ((u32)(((x) & 0x0000001F) << 0))
#define GET_DMATDL(x)              ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup DMARDLR
 * @brief DMA Receive Data Level Register
 * @{
 **/
#define MASK_DMARDL                ((u32)0x0000001F << 0)          /*!<R/W 0x0  Receive Data Level. This bit field controls the level at which a DMA request is made by the receive logic. The watermark level = DMARDL+1; that is, dma_rx_req is generated when the number of valid data entries in the receive FIFO is equal to or above this field value + 1, and RDMAE=1. */
#define DMARDL(x)                  ((u32)(((x) & 0x0000001F) << 0))
#define GET_DMARDL(x)              ((u32)(((x >> 0) & 0x0000001F)))
/** @} */

/** @defgroup IDR
 * @brief Identification Register
 * @{
 **/
#define MASK_IDCODE                ((u32)0xFFFFFFFF << 0)          /*!<R {16'h CR_version, 16'hFF01}
e.g., if there are two SPICs in a chip, the ID should be 0x0203_FF01 for #1 SPIC v2.3 and 0x0203_FF02 for #2 SPIC v2.3  Contain the decimal value of SPIC version. */
#define IDCODE(x)                  ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GET_IDCODE(x)              ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SPIC_VERSION
 * @brief SPIC version ID Register
 * @{
 **/
#define MASK_SPIC_VERSION          ((u32)0xFFFFFFFF << 0)          /*!<R {16'd SVN_NUM, 16'd Encryptd_Date}
SVN_NUM: version of SPIC rtl design
Encrypted_Date: generated date  Contain the decimal value of SPIC version. (After 201612: SVN_NUM changes to Git Counter) */
#define SPIC_VERSION(x)            ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GET_SPIC_VERSION(x)        ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DRx
 * @brief Data Register x
 * @{
 **/
#define MASK_DRx                   ((u32)0xFFFFFFFF << 0)          /*!<R/W 0x0  It is a data buffer with 8-bit width FIFO. If accessing in word data byte, it would read/write 4 entries of FIFO. */
#define DRx(x)                     ((u32)(((x) & 0xFFFFFFFF) << 0))
#define GET_DRx(x)                 ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DM_DRx
 * @brief Data Mask Data Register x
 * @{
 **/
#define BIT_DATA_EN_DRx            ((u32)0x00000001 << 1)          /*!<W 0x0  It is a data buffer for spi_data_en signal in user mode. The output value of spi_data_en is the data that has been pushed to DM_FIFO [1]. If DM_FIFO is empty, the output value of spi_data_en will be 0. */
#define BIT_DATA_MASK_DRx          ((u32)0x00000001 << 0)          /*!<W 0x0  It is a data buffer for spi_dm signal in user mode. The output value of spi_dm is the data that has been pushed to DM_FIFO [0]. If DM_FIFO is empty, the output value of spi_data_en will be (~DM_ACT). */
/** @} */

/** @defgroup READ_FAST_SINGLE
 * @brief Fast Read Data Command of SPI Flash
 * @{
 **/
#define MASK_FRD_CMD               ((u32)0x0000FFFF << 0)          /*!<R/W 0x0B0B  READ_FAST_SINGLE (or RD_OCTA_IO) Indicates SPI Flash command value of fast read command. The baud rate is used with FBAUDR. FRD_CMD[15:8] is for RD_OCTA_IO_CMD_2nd_BYTE (only available when configuration of DDR_EN is defined) FRD_CMD[7:0] is for RD_OCTA_IO_CMD_1st_BYTE */
#define FRD_CMD(x)                 ((u32)(((x) & 0x0000FFFF) << 0))
#define GET_FRD_CMD(x)             ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup READ_DUAL_DATA
 * @brief Dual Output Read Command of SPI Flash
 * @{
 **/
#define MASK_RD_DUAL_O_CMD         ((u32)0x000000FF << 0)          /*!<R/W 0x3B  Indicates SPI Flash command value of dual data read command. */
#define RD_DUAL_O_CMD(x)           ((u32)(((x) & 0x000000FF) << 0))
#define GET_RD_DUAL_O_CMD(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup READ_DUAL_ADDR_DATA
 * @brief Dual I/O Read Command of SPI Flash
 * @{
 **/
#define MASK_RD_DUAL_IO_CMD        ((u32)0x000000FF << 0)          /*!<R/W 0xBB  Indicates SPI Flash command value of dual address and data read command. */
#define RD_DUAL_IO_CMD(x)          ((u32)(((x) & 0x000000FF) << 0))
#define GET_RD_DUAL_IO_CMD(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup READ_QUAD_DATA
 * @brief Qaud Output Read Command of SPI Flash
 * @{
 **/
#define MASK_RD_QUAD_O_CMD         ((u32)0x000000FF << 0)          /*!<R/W 0x6B  Indicates SPI Flash command value of quad data read command. */
#define RD_QUAD_O_CMD(x)           ((u32)(((x) & 0x000000FF) << 0))
#define GET_RD_QUAD_O_CMD(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup READ_QUAD_ADDR_DATA
 * @brief Quad I/O Read Command of SPI Flash
 * @{
 **/
#define MASK_EXIT_PRM_CMD          ((u32)0x000000FF << 24)          /*!<N/A 0xFF  Exit High Performance Read Mode commend. */
#define EXIT_PRM_CMD(x)            ((u32)(((x) & 0x000000FF) << 24))
#define GET_EXIT_PRM_CMD(x)        ((u32)(((x >> 24) & 0x000000FF)))
#define MASK_PRM_VAL               ((u32)0x000000FF << 16)          /*!<R/W 0xA5  High Performance Read Mode Value. */
#define PRM_VAL(x)                 ((u32)(((x) & 0x000000FF) << 16))
#define GET_PRM_VAL(x)             ((u32)(((x >> 16) & 0x000000FF)))
#define MASK_EXIT_PRM_DUM_LEN      ((u32)0x000000FF << 8)          /*!<N/A 0x0  If Exit PRM commend is more than 1 byte, use EXIT_PRM_DUM_LEN to extend the commend. It is referenced by bus_clk. Ex. In qpi mode (4-4-4), FFFFFFFFh data cycle should be issued (4-byte data in 8 spi cycles). EXIT_PRM_CMD only push 1 byte (2 spi cycles), so use EXIT_PRM_DUM_LEN to extend 6 spi cycles. EXIT_PRM_DUM_LEN = 6 (spi_cycle) * (2 * baurd_rate) */
#define EXIT_PRM_DUM_LEN(x)        ((u32)(((x) & 0x000000FF) << 8))
#define GET_EXIT_PRM_DUM_LEN(x)    ((u32)(((x >> 8) & 0x000000FF)))
#define MASK_RD_QUAD_IO_CMD        ((u32)0x000000FF << 0)          /*!<R/W 0xEB  Indicates SPI Flash command value of quad address and data read command. */
#define RD_QUAD_IO_CMD(x)          ((u32)(((x) & 0x000000FF) << 0))
#define GET_RD_QUAD_IO_CMD(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WRITE_SIGNLE
 * @brief Page Program Command of SPI Flash
 * @{
 **/
#define MASK_WR_CMD                ((u32)0x0000FFFF << 0)          /*!<R/W 0x0202  WRITE_SINGLE (or WR_OCTA_IO) Indicates SPI Flash command value of write command. WR_CMD[15:8] is for WR_OCTA_IO_CMD_2nd_BYTE (only available when configuration of DDR_EN is defined) WR_CMD[7:0] is for WR_OCTA_IO_CMD_1st_BYTE */
#define WR_CMD(x)                  ((u32)(((x) & 0x0000FFFF) << 0))
#define GET_WR_CMD(x)              ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup WRITE_DUAL_DATA
 * @brief Dual Data Input Program Command of SPI Flash
 * @{
 **/
#define MASK_WR_DUAL_I_CMD         ((u32)0x000000FF << 0)          /*!<R/W 0xA2  Indicates SPI Flash command value of dual data write command. */
#define WR_DUAL_I_CMD(x)           ((u32)(((x) & 0x000000FF) << 0))
#define GET_WR_DUAL_I_CMD(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WRITE_DUAL_ADDR_DATA
 * @brief Dual Address and Data Output Read Command of SPI Flash
 * @{
 **/
#define MASK_WR_DUAL_II_CMD        ((u32)0x000000FF << 0)          /*!<R/W 0x0  Indicates SPI Flash command value of dual address and data write command. */
#define WR_DUAL_II_CMD(x)          ((u32)(((x) & 0x000000FF) << 0))
#define GET_WR_DUAL_II_CMD(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WRITE_QUAD_DATA
 * @brief Quad Data Input Program Command of SPI Flash
 * @{
 **/
#define MASK_WR_QUAD_I_CMD         ((u32)0x000000FF << 0)          /*!<R/W 0x32  Indicates SPI Flash command value of quad data write command. */
#define WR_QUAD_I_CMD(x)           ((u32)(((x) & 0x000000FF) << 0))
#define GET_WR_QUAD_I_CMD(x)       ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WRITE_QUAD_ADDR_DATA
 * @brief Quad Address and Data Output Read Command of SPI Flash
 * @{
 **/
#define MASK_WR_QUAD_II_CMD        ((u32)0x000000FF << 0)          /*!<R/W 0x38  Indicates SPI Flash command value of quad address and data write command. */
#define WR_QUAD_II_CMD(x)          ((u32)(((x) & 0x000000FF) << 0))
#define GET_WR_QUAD_II_CMD(x)      ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup WRITE_ENABLE
 * @brief Write Enable Command of SPI Flash
 * @{
 **/
#define MASK_WR_EN_CMD_2ND_BYTE    ((u32)0x000000FF << 8)          /*!<R/W 0x06  Indicates SPI Flash command value of 2nd byte of write enable. */
#define WR_EN_CMD_2ND_BYTE(x)      ((u32)(((x) & 0x000000FF) << 8))
#define GET_WR_EN_CMD_2ND_BYTE(x)  ((u32)(((x >> 8) & 0x000000FF)))
#define MASK_WR_EN_CMD_1ST_BYTE    ((u32)0x000000FF << 0)          /*!<R/W 0x06  Indicates SPI Flash command value of 1st byte of write enable. */
#define WR_EN_CMD_1ST_BYTE(x)      ((u32)(((x) & 0x000000FF) << 0))
#define GET_WR_EN_CMD_1ST_BYTE(x)  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup READ_STATUS
 * @brief Read Status Command of SPI Flash
 * @{
 **/
#define BIT_INTERVAL_EN            ((u32)0x00000001 << 31)          /*!<R/W 0x0  Set to enable INTERVAL_ODD. */
#define BIT_INTERVAL_ODD           ((u32)0x00000001 << 30)          /*!<R/W 0x0  Indicates SPI Flash read status with 2-Byte status but checking only odd byte or even byte. 0: even byte (2, 4, 6, ...) 1: odd byte (1, 3, 5, ...) */
#define MASK_ST_CMD_LEN            ((u32)0x00000003 << 28)          /*!<R/W 0x1  Indicates the number of bytes in read status command. ST_CMD_LEN can be 1~3. */
#define ST_CMD_LEN(x)              ((u32)(((x) & 0x00000003) << 28))
#define GET_ST_CMD_LEN(x)          ((u32)(((x >> 28) & 0x00000003)))
#define MASK_ST_CMD_CH             ((u32)0x00000003 << 26)          /*!<R/W 0x0  Indicates channel number of read status command in transmitting and receiving data. 0: single channel 1: dual channels 2: quad channels 3: octal channel */
#define ST_CMD_CH(x)               ((u32)(((x) & 0x00000003) << 26))
#define GET_ST_CMD_CH(x)           ((u32)(((x >> 26) & 0x00000003)))
#define BIT_ST_CMD_DDR_EN          ((u32)0x00000001 << 25)          /*!<R/W 0x0  Indicates the DDR mode in ST_CMD_CH. */
#define MASK_RD_ST_CMD_3RD_BYTE    ((u32)0x000000FF << 16)          /*!<R/W 0x00  Indicates SPI Flash command value of 3rd byte of read status. */
#define RD_ST_CMD_3RD_BYTE(x)      ((u32)(((x) & 0x000000FF) << 16))
#define GET_RD_ST_CMD_3RD_BYTE(x)  ((u32)(((x >> 16) & 0x000000FF)))
#define MASK_RD_ST_CMD_2ND_BYTE    ((u32)0x000000FF << 8)          /*!<R/W 0x05  Indicates SPI Flash command value of 2nd byte of read status. */
#define RD_ST_CMD_2ND_BYTE(x)      ((u32)(((x) & 0x000000FF) << 8))
#define GET_RD_ST_CMD_2ND_BYTE(x)  ((u32)(((x >> 8) & 0x000000FF)))
#define MASK_RD_ST_CMD_1ST_BYTE    ((u32)0x000000FF << 0)          /*!<R/W 0x05  Indicates SPI Flash command value of 1st byte of read status. */
#define RD_ST_CMD_1ST_BYTE(x)      ((u32)(((x) & 0x000000FF) << 0))
#define GET_RD_ST_CMD_1ST_BYTE(x)  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup CTRLR2
 * @brief Control Register 2
 * @{
 **/
#define BIT_DIS_DM_CA              ((u32)0x00000001 << 14)          /*!<R/W 0x0  Set to disable spi_dm_oe_n (let the device to drive DM) when SPIC pushing CMD and ADDR for both user mode and auto mode. */
#define BIT_FULL_WR                ((u32)0x00000001 << 13)          /*!<R/W 0x1  Set to enable full write in auto mode. If FULL_WR = 0, SPIC will use spi_dm to do the partial write. */
#define BIT_DM_ACT                 ((u32)0x00000001 << 12)          /*!<R/W 0x1  DM is active high or low. If DM is active high (DM=1 means "don't write"), DM_ACT should be set to 1. */
#define MASK_RX_FIFO_ENTRY         ((u32)0x0000000F << 8)          /*!<R/W 5  If using SPIC_HAS_DMA (NO_MERGE_FIFO) SPIC, this field indicates the valid entry of RX FIFO. It is an index of 2 and it should be equal or smaller than 5. The user can modify it to profile with a different entry. Ex: if RX_FIFO entry is 32, RX_FIFO_ENTYR <=5. */
#define RX_FIFO_ENTRY(x)           ((u32)(((x) & 0x0000000F) << 8))
#define GET_RX_FIFO_ENTRY(x)       ((u32)(((x >> 8) & 0x0000000F)))
#define MASK_TX_FIFO_ENTRY         ((u32)0x0000000F << 4)          /*!<R/W FIFO_ABW  If using SPIC_HAS_DMA (NO_MERGE_FIFO) SPIC, this field indicates the valid entry of TX_FIFO. If using MERGE_FIFO SPIC, there is only one FIFO is in the SPIC design, and this field indicates the valid entry of the FIFO. It is an index of 2 and it should be equal or smaller than FIFO_ABW. The user can modify it to profile with a different entry. Ex: if FIFO entry is 32, FIFO_ENTYR <=5. */
#define TX_FIFO_ENTRY(x)           ((u32)(((x) & 0x0000000F) << 4))
#define GET_TX_FIFO_ENTRY(x)       ((u32)(((x >> 4) & 0x0000000F)))
#define BIT_WPN_DNUM               ((u32)0x00000001 << 2)          /*!<R/W 0  Indicates the WPn input pin of SPI Flash is connected to spi_sout[2]or [3]. If setting 0, it connects to spi_sdata[2]. Otherwise, it connects to spi_sdata[3]. */
#define BIT_WPN_SET                ((u32)0x00000001 << 1)          /*!<R/W 0  To implement write protect function. spi_wen_out and the bit of spi_sout which connects to WPN would be initial to 0. WPN_SET: If setting 1 is protected. Otherwise, is not protected.(Configuration Form is using "WPN_SET_n": If setting 1 is NOT protected. Otherwise, is protected.) */
#define BIT_SO_DNUM                ((u32)0x00000001 << 0)          /*!<R/W 1  Indicates SO input pin of SPI Flash is connected to spi_sout[0]or [1]. It refers to SPI Flash and also the system connection with SPI_Flash (see 6.1.3) If setting 0, it connects to spi_sout[0] (single-channel connection). Otherwise, it connects to spi_sout[1] (multi-channels connection). (Default is configured to 1to support multi-channel connection) */
/** @} */

/** @defgroup FBAUDR
 * @brief Fast Baud Rate Select
 * @{
 **/
#define MASK_FSCKDV                ((u32)0x00000FFF << 0)          /*!<R/W 0x01  Indicates the divider of Fast read command when FAST_RD is setting in user mode or using fast read command in auto mode. The frequency of spi_sclk is derived from: Frequency of spi_sclk = Frequency of bus_clk / (2*FSCKDV). */
#define FSCKDV(x)                  ((u32)(((x) & 0x00000FFF) << 0))
#define GET_FSCKDV(x)              ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup USER_LENGTH
 * @brief User Length Register
 * @{
 **/
#define MASK_USER_ADDR_LENGTH      ((u32)0x0000000F << 16)          /*!<R/W 0x3  Indicates number of bytes in address phase (between command phase and write/read phase) in user mode. If it is set to 4, it will transmit 4-byte Address to support 4-byte address mode in SPI Flash. */
#define USER_ADDR_LENGTH(x)        ((u32)(((x) & 0x0000000F) << 16))
#define GET_USER_ADDR_LENGTH(x)    ((u32)(((x >> 16) & 0x0000000F)))
#define MASK_USER_CMD_LENGHT       ((u32)0x00000003 << 12)          /*!<R/W 0x0  Indicates number of bytes in command phase in user mode. USER_CMD_LENGHT can be 0~3. */
#define USER_CMD_LENGHT(x)         ((u32)(((x) & 0x00000003) << 12))
#define GET_USER_CMD_LENGHT(x)     ((u32)(((x >> 12) & 0x00000003)))
#define MASK_USER_RD_DUMMY_LENGTH  ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Indicates delay cycles for receiving data in user mode (USER_MODE == 1). It is referenced by bus_clk. */
#define USER_RD_DUMMY_LENGTH(x)    ((u32)(((x) & 0x00000FFF) << 0))
#define GET_USER_RD_DUMMY_LENGTH(x) ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup AUTO_LENGTH
 * @brief Auto Address Length Register
 * @{
 **/
#define MASK_RDSR_DUMMY_LENGTH     ((u32)0x000000FF << 20)          /*!<R/W 0x0  Indicates delay cycles for receiving data after Read Status Register (RDSR) command. (auto write or user mode with auto check) It is referenced by bus_clk. */
#define RDSR_DUMMY_LENGTH(x)       ((u32)(((x) & 0x000000FF) << 20))
#define GET_RDSR_DUMMY_LENGTH(x)   ((u32)(((x >> 20) & 0x000000FF)))
#define MASK_AUTO_ADDR_LENGTH      ((u32)0x0000000F << 16)          /*!<R/W FLASH_ADDR_BYTE  Indicates number of bytes address in read/write command in auto mode. AUTO_ADDR_LENGTH should be 1, 2, 3, 4 bytes. If it is set to 4, it will transmit 4-byte Address to support 4-byte address mode in SPI Flash. Note: PRM in auto mode should set VALID_CMD[11] (PRM_EN) with correct AUTO_ADDR_LENGTH. */
#define AUTO_ADDR_LENGTH(x)        ((u32)(((x) & 0x0000000F) << 16))
#define GET_AUTO_ADDR_LENGTH(x)    ((u32)(((x >> 16) & 0x0000000F)))
#define MASK_IN_PHYSICAL_CYC       ((u32)0x0000000F << 12)          /*!<R/W 0x0  Indicates how many SPIC CLK (bus_clk) cycles from pad to internal SPIC. */
#define IN_PHYSICAL_CYC(x)         ((u32)(((x) & 0x0000000F) << 12))
#define GET_IN_PHYSICAL_CYC(x)     ((u32)(((x >> 12) & 0x0000000F)))
#define MASK_RD_DUMMY_LENGTH       ((u32)0x00000FFF << 0)          /*!<R/W DUMMY_CYCLE  Indicates delay cycles for receiving data. It is referenced by bus_clk. Note: Don't include SPIC CLK (bus_clk) cycles from pad to internal SPIC */
#define RD_DUMMY_LENGTH(x)         ((u32)(((x) & 0x00000FFF) << 0))
#define GET_RD_DUMMY_LENGTH(x)     ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup VALID_CMD
 * @brief Valid Command Register
 * @{
 **/
#define BIT_SEQ_WR_EN              ((u32)0x00000001 << 15)          /*!<R/W 0x0  Set (1) to enable read sequential transaction write function in auto mode. If two bus write transactions are sequential (address is consecutive), SPIC can access second transaction data without sending Read CMD/ADDR/DUMMY. Once disable (0) this bit field, spi_csn will inactive immediately and also disable this function. Note: Run SEQ_WR_EN after SPIC boot finish. */
#define BIT_SEQ_RD_EN              ((u32)0x00000001 << 14)          /*!<R/W 0x1  Set (1) to enable read sequential transaction read function in auto mode. If two bus read transactions are sequential (address is consecutive), SPIC can access second transaction data without sending Read CMD/ADDR/DUMMY. Once disable (0) this bit field, spi_csn will inactive immediately and also disable this function. Note: Run SEQ_RD_EN after SPIC boot finish. */
#define BIT_DUM_EN                 ((u32)0x00000001 << 13)          /*!<R/W 0x0  Enable to push one dummy byte (DUM_BYTE_VAL) after pushing address to Flash in auto read. If PRM_EN is active, it will push PRM_Value instead of DUM_BYTE_VAL. If RD_DUMMY_LENGTH = 0, it won't push the dummy byte. */
#define BIT_CTRLR0_CH              ((u32)0x00000001 << 12)          /*!<R/W 0x0  Set (1) to use CTRLR0 CMD_CH/DATA_CH/ADDR_CH and DDR_EN field in Auto mode; Otherwise, SPIC will decode according to which VALID_CMD you choose in auto mode. Suggestion while use this bit field: Read Flash mode: (1-4-8), (4-4-8), (1-4D-8D), (4-4D-8D) with using RD_QUAD_IO[7:0] Read Flash mode (with 2-Byte CMD): (1-8-8), (8-8-8), (8D-8D-8D), (4D-4D-4D) with using RD_FAST_SINGLE_IO[15:0] (always using FBAUD) Write Flash mode: (1-4-8), (4-4-8) with using WR_QUAD_IO[7:0] Write Flash mode (with 2-Byte CMD): (1-8-8), (8-8-8), (8D-8D-8D) with using WR_SINGLE_IO[15:0] */
#define BIT_PRM_EN                 ((u32)0x00000001 << 11)          /*!<R/W 0x0  Set to enable SPIC performance read mode in Auto Mode. RXI-312 SPIC will auto exit performance read mode before auto write or enter to user mode. Enter PRM Flow: Set CR_VALID_CMD[11] (PRM_EN) with CR_VALID_CMD[4] (RD_QUAD_IO) or with (CR_VALID_CMD[0] and CR_VALID_CMD[12]) Set CR_READ_QUAD_ADDR_DATA[23:16] (PRM_VALUE) and [7:0] (RD_QUAD_IO_CMD) Set correct Dummy Cycle, and Valid Command then access in Auto Mode address range If PRM_EN, at 1st time Auto Rd, it will access with CMD, and store this condition. At 2nd Auto Rd or later, it will access without CMD but with Address and Mode Value. Our SPIC will check PRM_VALUE (CR_READ_QUAD_ADDR_DATA [23:16]), if PRM_VALUE ==0x00 (not a valid PRM value), and PRM_EN is set, SPIC will always access SPI Flash with (CMD+ADDR+MODE+DUMMY+...) format. */
#define BIT_RM_WEN                 ((u32)0x00000001 << 10)          /*!<R/W 0x0  Remove write enable command in auto write. */
#define BIT_RM_RDSR                ((u32)0x00000001 << 9)          /*!<R/W 0x0  Remove read status register in auto write. */
#define BIT_WR_QUAD_II             ((u32)0x00000001 << 8)          /*!<R/W VALID_CMD_DEF[8]  Indicates quad address/data write is a valid command to execute. (known as (1-4-4)) */
#define BIT_WR_QUAD_I              ((u32)0x00000001 << 7)          /*!<R/W VALID_CMD_DEF[7]  Indicates quad data write is a valid command to execute. (known as (1-1-4)) */
#define BIT_WR_DUAL_II             ((u32)0x00000001 << 6)          /*!<R/W VALID_CMD_DEF[6]  Indicates dual address/data write is a valid command to execute. (known as (1-2-2)) */
#define BIT_WR_DUAL_I              ((u32)0x00000001 << 5)          /*!<R/W VALID_CMD_DEF[5]  Indicates dual data write is a valid command to execute. (known as (1-1-2)) */
#define BIT_RD_QUAD_IO             ((u32)0x00000001 << 4)          /*!<R/W VALID_CMD_DEF[4]  Indicates quad address/data read is a valid command to execute. (known as (1-4-4)) */
#define BIT_RD_QUAD_O              ((u32)0x00000001 << 3)          /*!<R/W VALID_CMD_DEF[3]  Indicates quad data read is a valid command to execute. (known as (1-1-4)) */
#define BIT_RD_DUAL_IO             ((u32)0x00000001 << 2)          /*!<R/W VALID_CMD_DEF[2]  Indicates dual address/data read is a valid command to execute. (known as (1-2-2)) */
#define BIT_RD_DUAL_I              ((u32)0x00000001 << 1)          /*!<R/W VALID_CMD_DEF[1]  Indicates dual data read is a valid command to execute. (known as (1-1-2)) */
#define BIT_FRD_SINGLE             ((u32)0x00000001 << 0)          /*!<R/W VALID_CMD_DEF[0]  Indicates fast read command is a valid command to execute. (known as (1-1-1)) */
/** @} */

/** @defgroup FLASE_SIZE
 * @brief Flash Size Register
 * @{
 **/
#define MASK_FLASH_SIZE            ((u32)0x0000000F << 0)          /*!<R/W FLASH_MEM_SIZE - 12  Indicates the size of flash size to select the target SPI Flash in auto mode. The number is an exponent of 2 (Flash size) -12(The smallest flash size is 4kB). Ex: If flash size is 4MB (22-bit), set FLASH_SIZE = 10 (22-12). */
#define FLASH_SIZE(x)              ((u32)(((x) & 0x0000000F) << 0))
#define GET_FLASH_SIZE(x)          ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup FLUSH_FIFO
 * @brief Flush FIFO Register
 * @{
 **/
#define BIT_FLUSH_ST_FIFO          ((u32)0x00000001 << 2)          /*!<W 0x0  Clear all data in the ST_FIFO when writing to FLUSH_ST_FIFO register. */
#define BIT_FLUSH_DR_FIFO          ((u32)0x00000001 << 1)          /*!<W 0x0  Clear all data in the TX_FIFO and RX_FIFO when writing to FLUSH_DR_FIFOregister. */
#define BIT_FLUSH_ALL              ((u32)0x00000001 << 0)          /*!<W 0x0  Clear all data in all FIFO (include TX_FIFO, RX_FIFO and ST_FIFO) and reset all FSM when writing to FLUSH_ALL register. */
/** @} */

/** @defgroup DUM_BYTE
 * @brief Dummy byte value
 * @{
 **/
#define MASK_DUM_BYTE_VAL          ((u32)0x000000FF << 0)          /*!<R/W 0x00  Push dummy byte value. */
#define DUM_BYTE_VAL(x)            ((u32)(((x) & 0x000000FF) << 0))
#define GET_DUM_BYTE_VAL(x)        ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup TX_NDF
 * @brief TX_NDF
 * @{
 **/
#define MASK_TX_NDF                ((u32)0x00FFFFFF << 0)          /*!<R/W 0x00  Indicates a number of data frames (Unit: Byte). After transmitting command and address, SPIC transmits data continuously until data frames are equal to TX_NDF. TX_NDF can set larger than TX_FIFO size. */
#define TX_NDF(x)                  ((u32)(((x) & 0x00FFFFFF) << 0))
#define GET_TX_NDF(x)              ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup DEVICE_INFO
 * @brief Device info
 * @{
 **/
#define BIT_DATA_UNIT_2B           ((u32)0x00000001 << 12)          /*!<R/W 0x0  Set (1) when the SPI device stores 2-byte data for each address. */
#define BIT_JEDEC_P2CMF            ((u32)0x00000001 << 11)          /*!<R/W 0x0  Set (1) when the SPI cmd is JEDEC Profile 2.0 Command Modifier Formats. (6-byte command and address) */
#define BIT_PSRAM                  ((u32)0x00000001 << 10)          /*!<R/W 0x0  Set (1) when the SPI device is PSRAM. PSRAM will disable WEN and RDSR in auto write. */
#define BIT_NAND_FLASH             ((u32)0x00000001 << 9)          /*!<R/W 0x0  Set (1) when the SPI device is NAND Flash. (not support yet) */
#define BIT_NOR_FLASH              ((u32)0x00000001 << 8)          /*!<R/W 0x1  Set (1) when the SPI device is NOR Flash. */
#define ATOM_SIZE(x)			((u32)(((x) & 0x00000003) << 6))
#define BIT_RD_PAGE_EN             ((u32)0x00000001 << 5)          /*!<R/W 0x0  Enable SPIC to chop the burst read command across page boundaries in auto mode. */
#define BIT_WR_PAGE_EN             ((u32)0x00000001 << 4)          /*!<R/W 0x1  Enable SPIC to chop the burst write command across page boundaries in auto mode. */
#define MASK_PAGE_SIZE             ((u32)0x0000000F << 0)          /*!<R/W 0x08  The page size of the SPI device (byte) = 2^(PAGE_SIZE) eg. If the page size of the SPI Flash is 256-byte. PAGE_SIZE should be set to 8. */
#define PAGE_SIZE(x)               ((u32)(((x) & 0x0000000F) << 0))
#define GET_PAGE_SIZE(x)           ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup TPR0
 * @brief Timing parameters
 * @{
 **/
#define MASK_CS_TCEM               ((u32)0x000000FF << 24)          /*!<R/W 0x0  Set to chop auto cmd when CSN low pulse width = (CS_TCEM*32) bus_clk. If set CS_TCEM = 0 will disable this function. ex. PSRAM tCEM = 4us, SPIC frequency = 100Mhz. CS_TCEM = ((4000 ns/10 ns)/32) - BYTE_DELAY = 12 (0xc) - BYTE_DELAY If ((16*baud_rate)/data_channel) <=32. BYTE_DELAY = 0. If ((16*baud_rate)/data_channel) > 32. BYTE_DELAY = (((16*baud_rate)/data_channel) - 32)/32. ex. If baud_rate = 3, data_channel = 1. BYTE_DELAY = (((16*3)/1) - 32)/32 = 16/32 = 0.5 Therefore, BYTE_DELAY should be 1 in this example. */
#define CS_TCEM(x)                 ((u32)(((x) & 0x000000FF) << 24))
#define GET_CS_TCEM(x)             ((u32)(((x >> 24) & 0x000000FF)))
#define MASK_CS_SEQ_TIMEOUT        ((u32)0x000000FF << 16)          /*!<R/W 0x10  The timeout setting of auto command after sequential read command. If set CS_SEQ_TIMEOUT = 0x00 will disable this function. If set CS_SEQ_TIMEOUT > 0x00, SPIC will exit sequential transaction read and inactive CS when there is no auto read/write command for (CS_SEQ_TIMEOUT *4) bus_clocks. */
#define CS_SEQ_TIMEOUT(x)          ((u32)(((x) & 0x000000FF) << 16))
#define GET_CS_SEQ_TIMEOUT(x)      ((u32)(((x >> 16) & 0x000000FF)))
#define MASK_CS_ACTIVE_HOLD        ((u32)0x0000000F << 12)          /*!<R/W 0x00  For Flash chip select active hold time after SCLK rising edge (refer to tSLCH/CHSH, tCSS/tCSH) Note: Wait most 4 bus_clk cycles before CS inactive. (Implement CS_ACTIVE_Hold timing only, CS_ACTIVE_Setup timing using D-PHY to shift) */
#define CS_ACTIVE_HOLD(x)          ((u32)(((x) & 0x0000000F) << 12))
#define GET_CS_ACTIVE_HOLD(x)      ((u32)(((x >> 12) & 0x0000000F)))
#define MASK_CS_H_WR_DUM_LEN       ((u32)0x0000003F << 6)          /*!<R/W CS_H_WR_DUM  Dummy cycle between sending write command to SPI Flash. Using the dummy cycles to avoid the timing violation of CS high time. */
#define CS_H_WR_DUM_LEN(x)         ((u32)(((x) & 0x0000003F) << 6))
#define GET_CS_H_WR_DUM_LEN(x)     ((u32)(((x >> 6) & 0x0000003F)))
#define MASK_CS_H_RD_DUM_LEN       ((u32)0x0000003F << 0)          /*!<R/W CS_H_RD_DUM  Dummy cycle between sending read command to SPI Flash. Using the dummy cycles to avoid the timing violation of CS high time. */
#define CS_H_RD_DUM_LEN(x)         ((u32)(((x) & 0x0000003F) << 0))
#define GET_CS_H_RD_DUM_LEN(x)     ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup AUTO_LENGTH2
 * @brief Auto Address Length Register 2
 * @{
 **/
#define MASK_WR_DUMMY_LENGTH       ((u32)0x00000FFF << 0)          /*!<R/W 0x0  Indicates delay cycles for transmitting data in auto mode. It is referenced by bus_clk. */
#define WR_DUMMY_LENGTH(x)         ((u32)(((x) & 0x00000FFF) << 0))
#define GET_WR_DUMMY_LENGTH(x)     ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup ST_DRx
 * @brief Status Data Register x
 * @{
 **/
#define MASK_ST_DRx                ((u32)0x000000FF << 0)          /*!<R 0x0  It is a data buffer with 8-bit width FIFO. */
#define ST_DRx(x)                  ((u32)(((x) & 0x000000FF) << 0))
#define GET_ST_DRx(x)              ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup STFLR
 * @brief Status FIFO level Register
 * @{
 **/
#define MASK_STFLR                 ((u32)0x0000000F << 0)          /*!<R 0x0  Status FIFO level. Indicates the FIFO entries of valid data in status data. */
#define STFLR(x)                   ((u32)(((x) & 0x0000000F) << 0))
#define GET_STFLR(x)               ((u32)(((x >> 0) & 0x0000000F)))
/** @} */

/** @defgroup PAGE_READ
 * @brief Page Read Command of NAND Flash
 * @{
 **/
#define MASK_PAGE_RD_ADDR_LEN      ((u32)0x00000003 << 18)          /*!<R/W 0x3  Indicates number of bytes in address phase in page read command. */
#define PAGE_RD_ADDR_LEN(x)        ((u32)(((x) & 0x00000003) << 18))
#define GET_PAGE_RD_ADDR_LEN(x)    ((u32)(((x >> 18) & 0x00000003)))
#define MASK_PAGE_RD_CH            ((u32)0x00000003 << 16)          /*!<R/W 0x0  Indicates channel number of command and address phase in transmitting page read command. 0: single channel 1: dual channels 2: quad channels 3: octal channel */
#define PAGE_RD_CH(x)              ((u32)(((x) & 0x00000003) << 16))
#define GET_PAGE_RD_CH(x)          ((u32)(((x >> 16) & 0x00000003)))
#define MASK_PAGE_RD_CMD           ((u32)0x000000FF << 0)          /*!<R/W 0x13  Indicates SPI Flash command value of page read command */
#define PAGE_RD_CMD(x)             ((u32)(((x) & 0x000000FF) << 0))
#define GET_PAGE_RD_CMD(x)         ((u32)(((x >> 0) & 0x000000FF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup SPIC_Exported_Types SPIC Exported Types
  * @{
*****************************************************************************/


/**
 * @brief SPIC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t CTRLR0;                                 /*!< CONTROL REGISTER 0 Register,  Address offset: 0X000 */
	__IO uint32_t RX_NDF;                                 /*!< CONTROL REGISTER 1 Register,  Address offset: 0X004 */
	__IO uint32_t SSIENR;                                 /*!< SPIC ENABLE REGISTER,  Address offset: 0X008 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__IO uint32_t SER;                                    /*!< SLAVE ENABLE REGISTER,  Address offset: 0X010 */
	__IO uint32_t BAUDR;                                  /*!< BAUD RATE SELECT Register,  Address offset: 0X014 */
	__IO uint32_t TXFTLR;                                 /*!< TRANSMIT FIFO THRESHOLD LEVEL Register,  Address offset: 0X018 */
	__IO uint32_t RXFTLR;                                 /*!< RECEIVE FIFO THRESHOLD LEVEL Register,  Address offset: 0X01C */
	__I  uint32_t TXFLR;                                  /*!< TRANSMIT FIFO LEVEL REGISTER,  Address offset: 0X020 */
	__I  uint32_t RXFLR;                                  /*!< RECEIVE FIFO LEVEL REGISTER,  Address offset: 0X024 */
	__IO uint32_t SR;                                     /*!< STATUS REGISTER,  Address offset: 0X028 */
	__IO uint32_t IMR;                                    /*!< INTERRUPT MASK REGISTER,  Address offset: 0X02C */
	__I  uint32_t ISR;                                    /*!< INTERRUPT STATUS REGISTER,  Address offset: 0X030 */
	__I  uint32_t RISR;                                   /*!< RAW INTERRUPT STATUS REGISTER,  Address offset: 0X034 */
	__IO uint32_t TXOICR;                                 /*!< TRANSMIT FIFO OVERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0X038 */
	__IO uint32_t RXOICR;                                 /*!< RECEIVE FIFO OVERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0X03C */
	__IO uint32_t RXUICR;                                 /*!< RECEIVE FIFO UNDERFLOW INTERRUPT CLEAR REGISTER,  Address offset: 0X040 */
	__IO uint32_t MSTICR;                                 /*!< MASTER ERROR INTERRUPT CLEAR REGISTER,  Address offset: 0X044 */
	__IO uint32_t ICR;                                    /*!< INTERRUPT CLEAR REGISTER,  Address offset: 0X048 */
	__IO uint32_t DMACR;                                  /*!< DMA CONTROL REGISTER,  Address offset: 0X04C */
	__IO uint32_t DMATDLR;                                /*!< DMA TRANSMIT DATA LEVEL REGISTER,  Address offset: 0X050 */
	__IO uint32_t DMARDLR;                                /*!< DMA RECEIVE DATA LEVEL REGISTER,  Address offset: 0X054 */
	__I  uint32_t IDR;                                    /*!< IDENTIFICATION REGISTER,  Address offset: 0X058 */
	__I  uint32_t SPIC_VERSION;                           /*!< SPIC VERSION ID REGISTER,  Address offset: 0X05C */
	union {
		__IO uint8_t  BYTE;
		__IO uint16_t HALF;
		__IO uint32_t WORD;
	} DR[16];
	__O  uint32_t DM_DRx[16];                             /*!< DATA MASK DATA REGISTER x Register,  Address offset: 0X0A0-0X0DC */
	__IO uint32_t READ_FAST_SINGLE;                       /*!< FAST READ DATA COMMAND OF SPI FLASH Register,  Address offset: 0X0E0 */
	__IO uint32_t READ_DUAL_DATA;                         /*!< DUAL OUTPUT READ COMMAND OF SPI FLASH Register,  Address offset: 0X0E4 */
	__IO uint32_t READ_DUAL_ADDR_DATA;                    /*!< DUAL I/O READ COMMAND OF SPI FLASH Register,  Address offset: 0X0E8 */
	__IO uint32_t READ_QUAD_DATA;                         /*!< QAUD OUTPUT READ COMMAND OF SPI FLASH Register,  Address offset: 0X0EC */
	__IO uint32_t READ_QUAD_ADDR_DATA;                    /*!< QUAD I/O READ COMMAND OF SPI FLASH Register,  Address offset: 0X0F0 */
	__IO uint32_t WRITE_SIGNLE;                           /*!< PAGE PROGRAM COMMAND OF SPI FLASH Register,  Address offset: 0X0F4 */
	__IO uint32_t WRITE_DUAL_DATA;                        /*!< DUAL DATA INPUT PROGRAM COMMAND OF SPI FLASH Register,  Address offset: 0X0F8 */
	__IO uint32_t WRITE_DUAL_ADDR_DATA;                   /*!< DUAL ADDRESS AND DATA OUTPUT READ COMMAND OF SPI FLASH Register,  Address offset: 0X0FC */
	__IO uint32_t WRITE_QUAD_DATA;                        /*!< QUAD DATA INPUT PROGRAM COMMAND OF SPI FLASH Register,  Address offset: 0X100 */
	__IO uint32_t WRITE_QUAD_ADDR_DATA;                   /*!< QUAD ADDRESS AND DATA OUTPUT READ COMMAND OF SPI FLASH Register,  Address offset: 0X104 */
	__IO uint32_t WRITE_ENABLE;                           /*!< WRITE ENABLE COMMAND OF SPI FLASH Register,  Address offset: 0X108 */
	__IO uint32_t READ_STATUS;                            /*!< READ STATUS COMMAND OF SPI FLASH Register,  Address offset: 0X10C */
	__IO uint32_t CTRLR2;                                 /*!< CONTROL REGISTER 2 Register,  Address offset: 0X110 */
	__IO uint32_t FBAUDR;                                 /*!< FAST BAUD RATE SELECT Register,  Address offset: 0X114 */
	__IO uint32_t USER_LENGTH;                            /*!< USER LENGTH REGISTER,  Address offset: 0X118 */
	__IO uint32_t AUTO_LENGTH;                            /*!< AUTO ADDRESS LENGTH REGISTER,  Address offset: 0X11C */
	__IO uint32_t VALID_CMD;                              /*!< VALID COMMAND REGISTER,  Address offset: 0X120 */
	__IO uint32_t FLASE_SIZE;                             /*!< FLASH SIZE REGISTER,  Address offset: 0X124 */
	__O  uint32_t FLUSH_FIFO;                             /*!< FLUSH FIFO REGISTER,  Address offset: 0X128 */
	__IO uint32_t DUM_BYTE;                               /*!< DUMMY BYTE VALUE Register,  Address offset: 0X12C */
	__IO uint32_t TX_NDF;                                 /*!< TX_NDF Register,  Address offset: 0X130 */
	__IO uint32_t DEVICE_INFO;                            /*!< DEVICE INFO Register,  Address offset: 0X134 */
	__IO uint32_t TPR0;                                   /*!< TIMING PARAMETERS Register,  Address offset: 0X138 */
	__IO uint32_t AUTO_LENGTH2;                           /*!< AUTO ADDRESS LENGTH REGISTER 2 Register,  Address offset: 0X13C */
	__IO uint32_t RSVD1[16];                              /*!<  Reserved,  Address offset:0x140-0x17C */
	union {
		__IO uint8_t  BYTE;
		__IO uint16_t HALF;
		__IO uint32_t WORD;
	} ST_DR[16];
	__I  uint32_t STFLR;                                  /*!< STATUS FIFO LEVEL REGISTER,  Address offset: 0X1C0 */
	__IO uint32_t RSVD2[3];                               /*!<  Reserved,  Address offset:0x1C4-0x1CC */
	__IO uint32_t PAGE_READ;                              /*!< PAGE READ COMMAND OF NAND FLASH Register,  Address offset: 0X1D0 */
} SPIC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

/* Exported types ------------------------------------------------------------*/
/** @defgroup FLASH_Exported_Types FLASH Exported Types
  * @{
  */

/**
  * @brief  FLASH Init Structure Definition
  */
typedef struct {
	u32 FLASH_Id;				/*!< Specifies the flash vendor ID.
								This parameter can be a value of @ref FLASH_Vendor_ID */
	u8 FLASH_cur_bitmode;		/*!< Specifies the current bitmode of SPIC.
								This parameter can be a value of @ref FLASH_Bit_Mode */
	u8 FLASH_baud_rate;			/*!< Specifies the spi_sclk divider value. The frequency of spi_sclk is derived from:
								Frequency of spi_sclk = Frequency of oc_clk / (2 * FLASH_baud_rate) */
	u8 FLASH_baud_boot;			/*!< Specifies the spi_sclk divider value for rom boot. The frequency of spi_sclk is derived from:
								Frequency of spi_sclk = Frequency of oc_clk / (2 * FLASH_baud_rate) */
	u32 FLASH_cur_cmd; 			/*!< Specifies the current read cmd which is used to read data from flash
								in current bitmode. */

	/* status bits define */
	u32 FLASH_QuadEn_bit;		/*!< Specifies the QE bit in status register which is used to enable Quad I/O mode . */
	u32 FLASH_Busy_bit;			/*!< Specifies the WIP(Write in Progress) bit in status register which indicates whether
								the device is busy in program/erase/write status register progress. */
	u32 FLASH_WLE_bit; 			/*!< Specifies the WEL(Write Enable Latch) bit in status register which indicates whether
								the device will accepts program/erase/write status register instructions*/
	u32 FLASH_Status2_exist;		/*!< Specifies whether this flash chip has Status Register2 or not.
								This parameter can be 0/1. 0 means it doesn't have Status Register2, 1 means
								it has Status Register2.*/

	/* calibration data */
	u8 FLASH_rd_sample_phase_cal;	/*!< Specifies the read sample phase obtained from calibration. this is cal sample phase get from high speed cal */
	u8 FLASH_rd_sample_phase;	/*!< Specifies the read sample phase obtained from calibration. this is current sample phase */
	u8 FLASH_rd_dummy_cyle[5];	/*!< Specifies the read dummy cycle of different bitmode according to
								flash datasheet*/

	/* valid R/W command set */
	u32 FLASH_rd_dual_o; 			/*!< Specifies dual data read cmd */
	u32 FLASH_rd_dual_io; 			/*!< Specifies dual data/addr read cmd */
	u32 FLASH_rd_quad_o; 		/*!< Specifies quad data read cmd */
	u32 FLASH_rd_quad_io; 		/*!< Specifies quad data/addr read cmd */
	u32 FLASH_wr_dual_i; 			/*!< Specifies dual data write cmd */
	u32 FLASH_wr_dual_ii;			/*!< Specifies dual data/addr write cmd */
	u32 FLASH_wr_quad_i; 			/*!< Specifies quad data write cmd */
	u32 FLASH_wr_quad_ii;			/*!< Specifies quad data/addr write cmd */
	u32 FALSH_dual_o_valid_cmd;		/*!< Specifies valid cmd of dual o bitmode to program/read flash in auto mode */
	u32 FALSH_dual_io_valid_cmd;	/*!< Specifies valid cmd of dual io bitmode to program/read flash in auto mode */
	u32 FALSH_quad_o_valid_cmd;		/*!< Specifies valid cmd of quad o bitmode to program/read flash in auto mode */
	u32 FALSH_quad_io_valid_cmd;	/*!< Specifies valid cmd of quad io bitmode to program/read flash in auto mode */

	/* other command set */
	u8 FLASH_cmd_wr_en;			/*!< Specifies the Write Enable(WREN) instruction which is for setting WEL bit*/
	u8 FLASH_cmd_rd_id;			/*!< Specifies the Read ID instruction which is for getting the identity of the flash divice.*/
	u8 FLASH_cmd_rd_status;		/*!< Specifies the Read Status Register instruction which is for getting the status of flash */
	u8 FLASH_cmd_rd_status2;		/*!< Specifies the Read Status Register2 instruction which is for getting the status2 of flash */
	u8 FLASH_cmd_wr_status;		/*!< Specifies the Write Status Register instruction which is for setting the status register of flash */
	u8 FLASH_cmd_wr_status2;		/*!< Specifies the Write Status Register2 instruction which is for setting the status register2 of flash.
								 In some flash chips, status2 write cmd != status1 write cmd,
								 like: GD25Q32C, GD25Q64C,GD25Q128C etc.*/
	u8 FLASH_cmd_chip_e;			/*!< Specifies the Erase Chip instruction which is for erasing the whole chip*/
	u8 FLASH_cmd_block_e;		/*!< Specifies the Erase Block instruction which is for erasing 64kB*/
	u8 FLASH_cmd_sector_e;		/*!< Specifies the Erase Sector instruction which is for erasing 4kB*/
	u8 FLASH_cmd_pwdn_release;	/*!< Specifies the Release from Deep Power Down instruction which is for exiting power down mode.*/
	u8 FLASH_cmd_pwdn;			/*!< Specifies the Deep Power Down instruction which is for entering power down mode.*/
	u8 FLASH_dum_byte;

	/* debug log */
	u8 debug;					/*!< Specifies whether or not to print debug log.*/

	/* new calibration */
	u8 phase_shift_idx;			/*!< Specifies the phase shift idx in new calibration.*/

	u8 FLASH_addr_phase_len;	/*!< Specifies the number of bytes in address phase (between command phase and write/read phase).
								This parameter can be 0/1/2/3. 0 means 4-byte address mode in SPI Flash.*/
	u8 FLASH_dum_en;		/*!< Specifies enable to push one dummy byte (DUM_BYTE_VAL) after pushing address to Flash in auto read.
								If RD_DUMMY_LENGTH = 0, it won't push the dummy byte. */
	u8 FLASH_pinmux;			/*!< Specifies which pinmux is used. PINMUX_S0 or PINMUX_S1*/

	u32 FLASH_rd_fast_single;	/*!< Specifies fast read cmd in auto mode.*/

	u8 FLASH_pagesize_main_bit_exp;		/*!< Specifies main aera of the page size is expressed as a power of 2*/
	u8 FLASH_pagesize_spare_bit_exp;	/*!< Specifies spare aera of the page size is expressed as a power of 2*/

	u8 FLASH_cmd_page_read;		/*!< Specifies array read cmd in nand flash.*/
	u8 FLASH_cmd_page_write;	/*!< Specifies array write cmd in nand flash.*/

	u8 FLASH_EFail_bit; 		/*!< Specifies the E_Fail bit in status register which shows the status of erase failure or
								if host erase any invalid address or protected area (including protected blocks or protected Secure OTP area).*/
	u8 FLASH_PFail_bit;		/*!< Specifies the P_Fail bit in status register which shows the status of program failure or
								if host program any invalid address or protected area (including protected blocks or protected Secure OTP area).*/
	u8 FLASH_ECC_bit;			/*!< Specifies the ECC_Sx bit in status register which shows the ECC status of the current valid Read operation.*/
	u8 FLASH_ECC_EN_bit;		/*!< Specifies the ECC_EN bit in Configuration register which can enable or disable the internal ECC of nand flash.*/
} FLASH_InitTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** @defgroup FLASH_Bit_Mode
  * @{
  */
#define SpicOneBitMode			0
#define SpicDualOBitMode		1
#define SpicDualIOBitMode		2
#define SpicQuadOBitMode		3
#define SpicQuadIOBitMode		4
/**
  * @}
  */

/** @defgroup FLASH_Erase_Type
  * @{
  */
#define EraseChip				0
#define EraseBlock				1
#define EraseSector				2
/**
  * @}
  */

/** @defgroup FLASH_Wait_Type
  * @{
  */
#define WAIT_SPIC_BUSY			0
#define WAIT_FLASH_BUSY			1
#define WAIT_WRITE_DONE			2
#define WAIT_WRITE_EN			3
#define WAIT_TRANS_COMPLETE		4
/**
  * @}
  */

/** @defgroup SPIC_Addr_Phase_Length
  * @{
  */
#define ADDR_3_BYTE				0x3
#define ADDR_4_BYTE				0x4
#define ADDR_3_BYTE_USER_PRM	0x0
#define ADDR_4_BYTE_USER_PRM	0x4
/**
  * @}
  */

/** @defgroup NOR_FLASH_Default_CMD
  * @{
  */
#define FLASH_CMD_WREN			0x06            //write enable
#define FLASH_CMD_WRDI			0x04            //write disable
#define FLASH_CMD_WRSR			0x01            //write status register
#define FLASH_CMD_RDID			0x9F            //read idenfication
#define FLASH_CMD_RDSR			0x05            //read status register
#define FLASH_CMD_RDSR2			0x35            //read status register-2
#define FLASH_CMD_READ			0x03            //read data
#define FLASH_CMD_FREAD			0x0B            //fast read data
#define FLASH_CMD_RDSFDP		0x5A            //Read SFDP
#define FLASH_CMD_RES			0xAB            //Read Electronic ID
#define FLASH_CMD_REMS			0x90            //Read Electronic Manufacturer & Device ID
#define FLASH_CMD_DREAD			0x3B            //Double Output Mode command
#define FLASH_CMD_SE			0x20            //Sector Erase
#define FLASH_CMD_BE			0xD8            //0x52 //64K Block Erase
#define FLASH_CMD_CE			0x60            //Chip Erase(or 0xC7)
#define FLASH_CMD_PP			0x02            //Page Program
#define FLASH_CMD_DP			0xB9            //Deep Power Down
#define FLASH_CMD_RDP			0xAB            //Release from Deep Power-Down
#define FLASH_CMD_2READ			0xBB            // 2 x I/O read  command
#define FLASH_CMD_4READ			0xEB            // 4 x I/O read  command
#define FLASH_CMD_QREAD			0x6B            // 1I / 4O read command
#define FLASH_CMD_4PP			0x32            //quad page program //this is diff with MXIC
#define FLASH_CMD_FF			0xFF            //Release Read Enhanced
#define FLASH_CMD_REMS2			0x92            // read ID for 2x I/O mode //this is diff with MXIC
#define FLASH_CMD_REMS4			0x94            // read ID for 4x I/O mode //this is diff with MXIC
#define FLASH_CMD_RDSCUR		0x48            // read security register //this is diff with MXIC
#define FLASH_CMD_WRSCUR		0x42            // write security register //this is diff with MXIC

#define FLASH_DM_CYCLE_2O		0x08
#define FLASH_DM_CYCLE_2IO		0x04
#define FLASH_DM_CYCLE_4O		0x08
#define FLASH_DM_CYCLE_4IO		0x06

#define FLASH_STATUS_BUSY		((u32)0x00000001)
#define FLASH_STATUS_WLE		((u32)0x00000002)

#define FLASH_CMD_ENT_ADDR4B 	0xB7
#define FLASH_CMD_EXT_ADDR4B	0xE9
/**
  * @}
  */

/** @defgroup FLASH_Vendor_ID
  * @{
  */
#define FLASH_ID_OTHERS			0
#define FLASH_ID_MXIC			1
#define FLASH_ID_WINBOND		2
#define FLASH_ID_MICRON			3
#define FLASH_ID_EON			4
#define FLASH_ID_GD				5
#define FLASH_ID_BOHONG			6
/**
  * @}
  */

/** @defgroup FLASH_Manufacturer_ID
  * @{
  */
#define MANUFACTURER_ID_MXIC		0xC2
#define MANUFACTURER_ID_WINBOND		0xEF
#define MANUFACTURER_ID_MICRON		0x20
#define MANUFACTURER_ID_BOHONG		0x68
#define MANUFACTURER_ID_GD			0xC8
#define MANUFACTURER_ID_EON			0x1C
#define MANUFACTURER_ID_FM			0xA1
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Functions FLASH Exported Functions
  * @{
  */

/** @defgroup FLASH_Normal_Functions FLASH Normal Functions
  * @{
  */
_LONG_CALL_ void FLASH_Erase(u32 EraseType, u32 Address);
_LONG_CALL_ void FLASH_SetStatus(u8 Cmd, u32 Len, u8 *Status);
_LONG_CALL_ void FLASH_SetStatusBits(u32 SetBits, u32 NewState);
_LONG_CALL_ void FLASH_WaitBusy(u32 WaitType);
_LONG_CALL_ void FLASH_WriteEn(void);
_LONG_CALL_ void FLASH_TxCmd(u8 cmd, u8 DataPhaseLen, u8 *pData);
_LONG_CALL_ void FLASH_RxCmd(u8 cmd, u32 read_len, u8 *read_data);
_LONG_CALL_ void FLASH_StructInit(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void FLASH_StructInit_Micron(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void FLASH_StructInit_MXIC(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ void FLASH_StructInit_GD(FLASH_InitTypeDef *FLASH_InitStruct);
_LONG_CALL_ u8 FLASH_Init(u8 SpicBitMode);
_LONG_CALL_ void FLASH_SetSpiMode(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode);
_LONG_CALL_ void FLASH_DeepPowerDown(u32 NewState);
_LONG_CALL_ void FLASH_TxData(u32 StartAddr, u32 DataPhaseLen, u8 *pData);
_LONG_CALL_ void FLASH_RxData(u8 cmd, u32 StartAddr, u32 read_len, u8 *read_data);
_LONG_CALL_ u32 FLASH_Calibration(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode, u8 LineDelay);

_LONG_CALL_ u32 FLASH_PLLInit_ClockDiv(u8 Div);
_LONG_CALL_ u32 FLASH_CalibrationNew(FLASH_InitTypeDef *FLASH_InitStruct, u8 SpicBitMode, u8 Div, u8 CalStep,
									 u8 LineDelay, u8 StartIdx);
_LONG_CALL_ u32 FLASH_CalibrationNewCmd(u32 NewStatus);
_LONG_CALL_ u32 FLASH_CalibrationPhaseIdx(u8 phase_idx);
_LONG_CALL_ void FLASH_ClockSwitch(u32 Source, u32 Protection);
_LONG_CALL_ int  FLASH_WriteStream(u32 address, u32 len, u8 *data);
_LONG_CALL_ int  FLASH_ReadStream(u32 address, u32 len, u8 *data);

/**
  * @}
  */

/** @defgroup FLASH_XIP_Functions FLASH XIP Functions
  * @note These functions will lock cpu when exec to forbit XIP, and flush cache after exec.
  * @{
  */
void FLASH_Write_Lock(void);
void FLASH_Write_Unlock(void);
void FLASH_RxCmdXIP(u8 cmd, u32 read_len, u8 *read_data);
void FLASH_SetStatusXIP(u8 Cmd, u32 Len, u8 *Status);
void FLASH_SetStatusBitsXIP(u32 SetBits, u32 NewState);
void FLASH_TxData12BXIP(u32 StartAddr, u8 DataPhaseLen, u8 *pData);
void FLASH_TxData256BXIP(u32 StartAddr, u32 DataPhaseLen, u8 *pData);
void FLASH_EraseXIP(u32 EraseType, u32 Address);
void FLASH_Write_IPC_Int(VOID *Data, u32 IrqStatus, u32 ChanNum);
/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other definitions --------------------------------------------------------*/
extern FLASH_InitTypeDef flash_init_para;
extern u32 SPIC_CALIB_PATTERN[2];

typedef void (*FLASH_STRUCT_INIT_FUNC)(FLASH_InitTypeDef *FLASH_InitStruct);

#define PAGE_SIZE_4K	0x1000

#define SPIC_LOWSPEED_SAMPLE_PHASE	0

#define DUAL_DUM_CYCLE_NUM		4
#define QUAD_DUM_CYCLE_NUM		2

#define SPIC_VALID_CMD_MASK		(0xffff)

#define SPIC_DR_DEPTH		(32)//32*8Bit
#define U32BLOCKSIZE		(sizeof(u32))
#define UNALIGNED32(X)		((u32)(X) & (U32BLOCKSIZE - 1))

/**
  * @brief Flash_Region_Type
  */
typedef enum _FLASH_REGION_TYPE_ {
	IMG_BOOT   =   0,
	IMG_BOOT_OTA2   = 1,
	IMG_APP_OTA1	= 2,
	IMG_APP_OTA2  	= 3,
	IMG_APIMG_OTA1	= 4,
	IMG_APIMG_OTA2 	= 5,
	FTL   =   	6,
	VFS1  =   	7,
	VFS2  =   	8,
	USER  =   	9,
} FLASH_REGION_TYPE, *PFLASH_REGION_TYPE;

/**
  * @brief  FLASH Layout Info Structure Definition
  */
typedef struct {
	u32 region_type;
	u32 start_addr;
	u32 end_addr;
} FlashLayoutInfo_TypeDef;

/* MANUAL_GEN_END */

#endif
