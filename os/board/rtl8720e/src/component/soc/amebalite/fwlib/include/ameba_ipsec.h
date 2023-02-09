#ifndef _AMEBA_IPSEC_H_
#define _AMEBA_IPSEC_H_

/* AUTO_GEN_START */

/** @addtogroup Ameba_Periph_Driver
  * @{
  */

/** @addtogroup HW_CRYPTO_ENGINE
* @{
*/

/* Registers Definitions --------------------------------------------------------*/

/** @defgroup HW_CRYPTO_ENGINE_Register_Definitions HW_CRYPTO_ENGINE Register Definitions
 * @{
 *****************************************************************************/

/** @defgroup SDSR
 * @brief Source Descriptor Status Register
 * @{
 **/
#define IPSEC_BIT_SRC_RST                     ((u32)0x00000001 << 31)          /*!<R/W 0  Source descriptor reset. (only for pk_up = 1'b1) */
#define IPSEC_BIT_PK_UP                       ((u32)0x00000001 << 30)          /*!<R/W 1  Packet base update wptr to engine. If pk_up =1, the total number of souce descriptor in one packet can't be over 16, and the total number of destination descriptor in one packet can't be over 8. */
#define IPSEC_MASK_SRC_FAIL_STATUS            ((u32)0x00000003 << 25)          /*!<R 0  Source Descriptor failed status. 2'd1: Users write the first word twice consecutively 2'd2: Users write the second word directly without writing first word in the beginning 2'd3: Overflow (Detect users try to write source descriptor, but there isn't available FIFO node could use). */
#define IPSEC_SRC_FAIL_STATUS(x)              ((u32)(((x) & 0x00000003) << 25))
#define IPSEC_GET_SRC_FAIL_STATUS(x)          ((u32)(((x >> 25) & 0x00000003)))
#define IPSEC_BIT_SRC_FAIL                    ((u32)0x00000001 << 24)          /*!<R/W 0  Source Descriptor failed interrupt. Write 1 to clear this bit. */
#define IPSEC_MASK_SRPTR                      ((u32)0x000000FF << 16)          /*!<R 0  Source Descriptor FIFO read pointer. When engine read a descriptor and finished it, SRPTR = SRPTR+2 */
#define IPSEC_SRPTR(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define IPSEC_GET_SRPTR(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define IPSEC_MASK_SWPTR                      ((u32)0x000000FF << 8)          /*!<R 0  Source Descriptor FIFO write pointer. When write descriptor to SDSW successfully, SWPTR = SWPTR+2 */
#define IPSEC_SWPTR(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define IPSEC_GET_SWPTR(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define IPSEC_MASK_SRC_DESC_FIFO_EMPTY_CNT    ((u32)0x000000FF << 0)          /*!<R 0  Source Descriptor FIFO empty counter. Use this field to check how many available FIFO nodes in Source descriptor FIFO could use. */
#define IPSEC_SRC_DESC_FIFO_EMPTY_CNT(x)      ((u32)(((x) & 0x000000FF) << 0))
#define IPSEC_GET_SRC_DESC_FIFO_EMPTY_CNT(x)  ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup SDFWR
 * @brief REG_SDFWR Register
 * @{
 **/
#define IPSEC_MASK_SDFW                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Descriptor First Word */
#define IPSEC_SDFW(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_SDFW(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup SDSWR
 * @brief Source Descriptor Second Word Register
 * @{
 **/
#define IPSEC_MASK_SDSW                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Source Descriptor Second Word */
#define IPSEC_SDSW(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_SDSW(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup IPSCSR_RSTEACONFISR
 * @brief Reset Engine And Configure Interrupt Service Routine Register
 * @{
 **/
#define IPSEC_BIT_RST                         ((u32)0x00000001 << 31)          /*!<R/W 0  IPsec engine reset. Write '1' to reset the crypto engine and DMA engine. (Use to clear fatal error) */
#define IPSEC_BIT_AHB_ERR_INT_MASK            ((u32)0x00000001 << 26)          /*!<R/W 1   */
#define IPSEC_BIT_CLR_AHB_ERR_INT             ((u32)0x00000001 << 25)          /*!<R/W 0   */
#define IPSEC_BIT_AHB_ERR_INT                 ((u32)0x00000001 << 24)          /*!<R 0   */
#define IPSEC_MASK_CLEAR_OK_INT_NUM           ((u32)0x000000FF << 16)          /*!<R/W 1  Clear ok interrupt number. In interrupt counter mode, if the programmer wants to clear ok_intr_cnt , need to write ok_intr_cnt to this field first, then write '1' to cmd_ok. */
#define IPSEC_CLEAR_OK_INT_NUM(x)             ((u32)(((x) & 0x000000FF) << 16))
#define IPSEC_GET_CLEAR_OK_INT_NUM(x)         ((u32)(((x >> 16) & 0x000000FF)))
#define IPSEC_MASK_OK_INTR_CNT                ((u32)0x000000FF << 8)          /*!<R 0  Ok interrupt counter. Read this field can know how many interrupts are coming to notify crypto engine has calculated cryptographic feature results. Use this field in interrupt counter mode. */
#define IPSEC_OK_INTR_CNT(x)                  ((u32)(((x) & 0x000000FF) << 8))
#define IPSEC_GET_OK_INTR_CNT(x)              ((u32)(((x >> 8) & 0x000000FF)))
#define IPSEC_BIT_INTR_MODE                   ((u32)0x00000001 << 7)          /*!<R/W 0  Select ok interrupt mode: 1'd0: Interrupt mode is general mode. 1'd1: Interrupt mode is counter mode. General mode means no matter how many interrupts are coming to notify crypto engine has calculated cryptographic feature results, there always is one interrupt signal shows in cmd_ok field, and nothing in ok_intr_cnt. So if the programmer want to clear this signal, just write '1' to cmd_ok. Counter mode means the programmer can know how many interrupts are coming to notify crypto engine has calculated cryptographic feature results from ok_intr_cnt. The programmer also can know there are at least one interrupt from reading cmd_ok. So if the programmer want to clear ok_intr_cnt signals, need to write ok_intr_cnt into clear_ok_int_num, then write 1 to cmd_ok. */
#define IPSEC_BIT_CMD_OK                      ((u32)0x00000001 << 4)          /*!<R/W 0  Command ok Interrupt. Read this field to detect whether crypto engine has calculated a cryptographic feature result. Even if interrupt mode is counter mode, still can use this field to detect whether an interrupt is coming. Write 1 to clear this interrupt signal. */
#define IPSEC_BIT_DMA_BUSY                    ((u32)0x00000001 << 3)          /*!<R 0  Detect whether IPsec DMA is busy: 1'd0: DMA is not busy 1'd1: DMA is busy Using for debuging. To avoid data coherence issue[when user get crypto finish calculated interrupt, user can make sure DMA engine has already move data to result buffer. */
#define IPSEC_BIT_SOFT_RST                    ((u32)0x00000001 << 0)          /*!<W 0  Software Reset. Write 1 to reset the crypto engine */
/** @} */

/** @defgroup IPSCSR_INTM
 * @brief Interrupt Mask Register
 * @{
 **/
#define IPSEC_BIT_DES_ERR5_M                  ((u32)0x00000001 << 18)          /*!<R/W 1  Destination Descriptor Error 5 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_ERR4_M                  ((u32)0x00000001 << 17)          /*!<R/W 1  Destination Descriptor Error 4 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_ERR3_M                  ((u32)0x00000001 << 16)          /*!<R/W 1  Destination Descriptor Error 3 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_ERR2_M                  ((u32)0x00000001 << 15)          /*!<R/W 1  Destination Descriptor Error 2 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_ERR1_M                  ((u32)0x00000001 << 14)          /*!<R/W 1  Destination Descriptor Error 1 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_ERR0_M                  ((u32)0x00000001 << 13)          /*!<R/W 1  Destination Descriptor Error 0 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR9_M                  ((u32)0x00000001 << 12)          /*!<R/W 1  Source Descriptor Error 9 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR8_M                  ((u32)0x00000001 << 11)          /*!<R/W 1  Source Descriptor Error 8 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR7_M                  ((u32)0x00000001 << 10)          /*!<R/W 1  Source Descriptor Error 7 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR6_M                  ((u32)0x00000001 << 9)          /*!<R/W 1  Source Descriptor Error 6 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR5_M                  ((u32)0x00000001 << 8)          /*!<R/W 1  Source Descriptor Error 5 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR4_M                  ((u32)0x00000001 << 7)          /*!<R/W 1  Source Descriptor Error 4 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR3_M                  ((u32)0x00000001 << 6)          /*!<R/W 1  Source Descriptor Error 3 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR2_M                  ((u32)0x00000001 << 5)          /*!<R/W 1  Source Descriptor Error 2 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR1_M                  ((u32)0x00000001 << 4)          /*!<R/W 1  Source Descriptor Error 1 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_ERR0_M                  ((u32)0x00000001 << 3)          /*!<R/W 1  Source Descriptor Error 0 Interrupt Mask 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_DES_FAIL_M                  ((u32)0x00000001 << 2)          /*!<R/W 1  Source Descriptor fail interrupt mask. 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_SRC_FAIL_M                  ((u32)0x00000001 << 1)          /*!<R/W 1  Destination Descriptor fail interrupt mask. 1'd0: Disable mask 1'd1: Enable mask */
#define IPSEC_BIT_CMD_OK_M                    ((u32)0x00000001 << 0)          /*!<R/W 1  Command ok Interrupt Mask. 1'd0: Disable mask 1'd1: Enable mask If the programmer wants to disable cmd_ok interrupt signal, write '1' to enable interrupt mask. */
/** @} */

/** @defgroup IPSCSR_DBG
 * @brief Debug Regster
 * @{
 **/
#define IPSEC_BIT_DEBUG_WB                    ((u32)0x00000001 << 31)          /*!<R/W 0  Debug: write back mode. 1'd0: Disable DMA write back mode. 1'd1: Enable DMA write back mode. Enable this field, Crypto DMA will move data from source address to destination address. During this period, the data won't be processed in any calculation(Encryption, Decryption, Hash). Then the programmer can check whether the source data is the same as destination data. */
#define IPSEC_MASK_MST_BAD_SEL                ((u32)0x00000003 << 28)          /*!<R/W 0  <for debug port> select Lexra master data bus (option) 2'd0: mst_ipsec_bad[07:00] 2'd1: mst_ipsec_bad[15:08] 2'd2: mst_ipsec_bad[23:16] 2'd3: mst_ipsec_bad[31:24] */
#define IPSEC_MST_BAD_SEL(x)                  ((u32)(((x) & 0x00000003) << 28))
#define IPSEC_GET_MST_BAD_SEL(x)              ((u32)(((x >> 28) & 0x00000003)))
#define IPSEC_BIT_ENGINE_CLK_EN               ((u32)0x00000001 << 24)          /*!<R/W 0  IPsec Engine clock enable 1'd0: Disable IPsec engine clock 1'd1: Enable IPsec engine clock */
#define IPSEC_MASK_DEBUG_PORT_SEL             ((u32)0x0000000F << 20)          /*!<R/W 0  Debug port selection: 4'd0: engine_debug 4'd1: dma_lexra_debug 4'd2: dma_rx_debug 4'd3: dma_tx_debug */
#define IPSEC_DEBUG_PORT_SEL(x)               ((u32)(((x) & 0x0000000F) << 20))
#define IPSEC_GET_DEBUG_PORT_SEL(x)           ((u32)(((x >> 20) & 0x0000000F)))
#define IPSEC_BIT_ARBITER_MODE                ((u32)0x00000001 << 16)          /*!<R/W 1  DMA arbiter mode: 1'd0: round-robin 1'd1: detect fifo wasted level There are 2 fifo in DMA, that are read-fifo and write-fifo.Detect fifo wasted level means that DMA will process which fifo first depend on which wasted level is high. Because the wasted level is high means there are many fifo nodes waiting to be processed in this fifo, so DMA must process them in hurry. */
#define IPSEC_MASK_DMA_WAIT_CYCLE             ((u32)0x0000FFFF << 0)          /*!<R/W 0  Set DMA wait cycles to assert next DMA request */
#define IPSEC_DMA_WAIT_CYCLE(x)               ((u32)(((x) & 0x0000FFFF) << 0))
#define IPSEC_GET_DMA_WAIT_CYCLE(x)           ((u32)(((x >> 0) & 0x0000FFFF)))
/** @} */

/** @defgroup IPSCSR_ERR_INT
 * @brief Error Interrupt Register
 * @{
 **/
#define IPSEC_BIT_DES_ERR5                    ((u32)0x00000001 << 15)          /*!<RW1C 0  Destination Descriptor Error 5 Interrupt. The error condition is authentication digest length error in destination descriptor. MD5: adl != 16 or SHA1: adl != 20 or SHA2-224 adl != 28 or adl != 32 when set sequential hash and not sequential hash last SHA2-256 adl != 32 or SHA2-384 adl != 48 or adl != 64 when set sequential hash and not sequential hash last SHA2-512 adl != 64 Write '1' to clear this error signal. */
#define IPSEC_BIT_DES_ERR4                    ((u32)0x00000001 << 14)          /*!<RW1C 0  Destination Descriptor Error 4 Interrupt. The error condition is authentication digest length error in destination descriptor. MD5: adl > 16 or SHA1: adl > 20 or SHA2-224: adl > 28 or adl > 32when set sequential hash and not sequential hash last SHA2-256 : adl > 32 or SHA2-384 : adl > 48 or adl > 64when set sequential hash and not sequential hash last SHA2-512 : adl > 64 Write '1' to clear this error signal. */
#define IPSEC_BIT_DES_ERR3                    ((u32)0x00000001 << 13)          /*!<RW1C 0  Destination Descriptor Error 3 Interrupt. One of the error condition is the sum of enl is greater than Encryption Total Length. The other error condition is about destination descriptor length error ((enl(Encryption length)==0) & (adl(authentication data length)==0)). Write '1' to clear this error signal. Sum of enl reference the register REG_IPSCSR_SENLR (offset 0x24). */
#define IPSEC_BIT_DES_ERR2                    ((u32)0x00000001 << 12)          /*!<RW1C 0  Destination Descriptor Error 2 Interrupt. The error condition is the bit field ENC=1'b0( active Hash engine to process hash data), but only active Cipher engine. Write '1' to clear this error signal. */
#define IPSEC_BIT_DES_ERR1                    ((u32)0x00000001 << 11)          /*!<RW1C 0  Destination Descriptor Error 1 Interrupt. The error condition is the bit field ENC=1'b1( active cipher engine to process cipher data), but only active Hash engine. Write '1' to clear this error signal. */
#define IPSEC_BIT_DES_ERR0                    ((u32)0x00000001 << 10)          /*!<RW1C 0  Destination Descriptor Error 0 Interrupt. One of the error condition is destination descriptor starting address error (DDSA[1:0]!=2'd0 [At least 4 bytes-aligned]). The other error condition is memory protection event. Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR9                    ((u32)0x00000001 << 9)          /*!<RW1C 0  Source Descriptor Error 9 Interrupt. The error condition is sum of epl is not equal to EPTL(Encryption Padding Total Length). Write '1' to clear this error signal. Sum of epl reference the register REG_IPSCSR_SEPLR (offset 0x2C). */
#define IPSEC_BIT_SRC_ERR8                    ((u32)0x00000001 << 8)          /*!<RW1C 0  Source Descriptor Error 8 Interrupt. The error condition is sum of apl is not equal to HPTL(Hash Padding Total Length). Write '1' to clear this error signal. Sum of apl reference the register REG_IPSCSR_SAPLR (offset 0x28). */
#define IPSEC_BIT_SRC_ERR7                    ((u32)0x00000001 << 7)          /*!<RW1C 0  Source Descriptor Error 7 Interrupt. The error condition is sum of enl is not equal to ETL(Encryption Total Length). Write '1' to clear this error signal. Sum of enl reference the register REG_IPSCSR_SENLR (offset 0x24). */
#define IPSEC_BIT_SRC_ERR6                    ((u32)0x00000001 << 6)          /*!<RW1C 0  Source Descriptor Error 6 Interrupt. The error condition is sum of a2eo is not equal to HTL(Header Total Length). Write '1' to clear this error signal. Sum of a2eo reference the register REG_IPSCSR_SAADLR (offset 0x20). */
#define IPSEC_BIT_SRC_ERR5                    ((u32)0x00000001 << 5)          /*!<RW1C 0  Source Descriptor Error 5 Interrupt. The error condition is there is a source descriptor, but the data length( (a2eo/epl) +(enl) + (apl)) is 0. Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR4                    ((u32)0x00000001 << 4)          /*!<RW1C 0  Source Descriptor Error 4 Interrupt. The error condition is source data buffer pointer error(SDBP[1:0]!=2'd0[At least 4 byte aligned]). Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR3                    ((u32)0x00000001 << 3)          /*!<RW1C 0  Source Descriptor Error 3 Interrupt. About source descriptor length error. One of the error condition is set the first descriptor but the (CL!=3). The other error condition is there is a source descriptor but CL, KL, IL, PL, SHIVL are all 0. Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR2                    ((u32)0x00000001 << 2)          /*!<RW1C 0  Source Descriptor Error 2 Interrupt. One of the error condition is Source descriptor starting address error (SDSA[1:0]!=2'd0 [At least 4 bytes-aligned]). The other error condition is memory protection event. Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR1                    ((u32)0x00000001 << 1)          /*!<RW1C 0  Source Descriptor Error 1 Interrupt. The error condition is last Segment descriptor is set (LS=1), when descriptor is pointing to the first segment of the packet. Write '1' to clear this error signal. */
#define IPSEC_BIT_SRC_ERR0                    ((u32)0x00000001 << 0)          /*!<RW1C 0  Source Descriptor Error 0 Interrupt. One of the error condition is first segment descriptor is not set (FS=0), when descriptor is pointing to the first segment of the packet. The other error condition is first segment descriptor is set (FS=1), when descriptor is not pointing to the first segment of the packet. Write '1' to clear this error signal. */
/** @} */

/** @defgroup IPSCSR_SADDLR
 * @brief Sum of Additional Authentication Data Length Register
 * @{
 **/
#define IPSEC_MASK_A2EO_SUM                   ((u32)0x000007FF << 0)          /*!<R 0  Sum(a2eo): Sum of additional authentication data length. */
#define IPSEC_A2EO_SUM(x)                     ((u32)(((x) & 0x000007FF) << 0))
#define IPSEC_GET_A2EO_SUM(x)                 ((u32)(((x >> 0) & 0x000007FF)))
/** @} */

/** @defgroup IPSCSR_SENLR
 * @brief Sum of Encryption Data Length Register
 * @{
 **/
#define IPSEC_MASK_ENL_SUM                    ((u32)0x00FFFFFF << 0)          /*!<R 0  Sum(enl): Sum of encryption data length. */
#define IPSEC_ENL_SUM(x)                      ((u32)(((x) & 0x00FFFFFF) << 0))
#define IPSEC_GET_ENL_SUM(x)                  ((u32)(((x >> 0) & 0x00FFFFFF)))
/** @} */

/** @defgroup IPSCSR_SAPLR
 * @brief Sum of Authentication Padding Data Length Register
 * @{
 **/
#define IPSEC_MASK_APL_SUM                    ((u32)0x00000FFF << 0)          /*!<R 0  Sum(apl): Sum of authentication padding data length.[Mix mode uses this field] */
#define IPSEC_APL_SUM(x)                      ((u32)(((x) & 0x00000FFF) << 0))
#define IPSEC_GET_APL_SUM(x)                  ((u32)(((x >> 0) & 0x00000FFF)))
/** @} */

/** @defgroup IPSCSR_SEPLR
 * @brief Sum of Encryption Padding Data Length Register
 * @{
 **/
#define IPSEC_MASK_EPL_SUM                    ((u32)0x0000003F << 0)          /*!<R 0  Sum(epl): Sum of encryption padding data length. [Mix mode uses this field] */
#define IPSEC_EPL_SUM(x)                      ((u32)(((x) & 0x0000003F) << 0))
#define IPSEC_GET_EPL_SUM(x)                  ((u32)(((x >> 0) & 0x0000003F)))
/** @} */

/** @defgroup IPSCSR_SWAPABURSTR
 * @brief Swap And Burst Register
 * @{
 **/
#define IPSEC_BIT_CHA_INPUT_DATA_BYTE_SWAP    ((u32)0x00000001 << 27)          /*!<R/W 0  byte swap for cha input data 1 : Enable 0 : Disable */
#define IPSEC_BIT_CHA_OUTPUT_DATA_BYTE_SWAP   ((u32)0x00000001 << 26)          /*!<R/W 0  byte swap for cha output data 1 : Enable 0 : Disable */
#define IPSEC_BIT_MD5_INPUT_DATA_BYTE_SWAP    ((u32)0x00000001 << 25)          /*!<R/W 0  byte swap for md5 input data 1 : Enable 0 : Disable */
#define IPSEC_BIT_MD5_OUTPUT_DATA_BYTE_SWAP   ((u32)0x00000001 << 24)          /*!<R/W 0  byte swap for md5 output data 1 : Enable 0 : Disable */
#define IPSEC_MASK_DMA_BURST_LENGTH           ((u32)0x0000003F << 16)          /*!<R/W 0x10  Set DMA burst length: The maximum setting length is 16 bursts The minimum setting length is 1 burst Note: If all bits are zero(6'd0), it means maximum burst size or undefined burst size. */
#define IPSEC_DMA_BURST_LENGTH(x)             ((u32)(((x) & 0x0000003F) << 16))
#define IPSEC_GET_DMA_BURST_LENGTH(x)         ((u32)(((x >> 16) & 0x0000003F)))
#define IPSEC_BIT_TX_WD_SWAP                  ((u32)0x00000001 << 12)          /*!<R/W 0  Word swap for dma_tx engine input data: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_RX_WD_SWAP                  ((u32)0x00000001 << 11)          /*!<R/W 0  Word swap for dma_rx engine output data: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_MAC_OUT_LITTLE_ENDIAN       ((u32)0x00000001 << 10)          /*!<R/W 0  Ouput mac is little endian: 1'd0: Big endian 1'd1: Little endian */
#define IPSEC_BIT_DATA_OUT_LITTLE_ENDIAN      ((u32)0x00000001 << 9)          /*!<R/W 0  Ouput data is little endian: 1'd0: Big endian 1'd1: Little endian */
#define IPSEC_BIT_TX_BYTE_SWAP                ((u32)0x00000001 << 8)          /*!<R/W 0  Byte swap for dma_tx engine input data: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_AUTO_PADDING_SWAP           ((u32)0x00000001 << 5)          /*!<R/W 0  byte swap for padding_len input data 1 : Enable 0 : Disable */
#define IPSEC_BIT_DATA_IN_LITTLE_ENDIAN       ((u32)0x00000001 << 4)          /*!<R/W 0  Input data is little endian: 1'd0: Big endian 1'd1: Little endian */
#define IPSEC_BIT_HASH_INITIAL_VALUE_SWAP     ((u32)0x00000001 << 3)          /*!<R/W 0  Byte swap for sequential hash initial value: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_KEY_PAD_SWAP                ((u32)0x00000001 << 2)          /*!<R/W 0  Byte swap for HMAC key pad: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_KEY_IV_SWAP                 ((u32)0x00000001 << 1)          /*!<R/W 0  Byte swap for Cipher key and Initial Vector: 1'd0: Disable 1'd1: Enable */
#define IPSEC_BIT_SET_SWAP                    ((u32)0x00000001 << 0)          /*!<R/W 0  Byte swap for command setting data: 1'd0: Disable 1'd1: Enable */
/** @} */

/** @defgroup IPSEKCR
 * @brief KEY Control Register
 * @{
 **/
#define IPSEC_BIT_EFUSE_KEY_REQ               ((u32)0x00000001 << 31)          /*!<R/W 0  request of loading key from OTP, clear by key_rdy */
#define IPSEC_MASK_EFUSE_KEY_ID               ((u32)0x00000003 << 29)          /*!<R/W 0  2'b00/2'b01: load key from OTP key1 2'b10/2'b11: load key from OTP key2 */
#define IPSEC_EFUSE_KEY_ID(x)                 ((u32)(((x) & 0x00000003) << 29))
#define IPSEC_GET_EFUSE_KEY_ID(x)             ((u32)(((x >> 29) & 0x00000003)))
/** @} */

/** @defgroup ENG_DBG
 * @brief
 * @{
 **/
#define IPSEC_MASK_ENG_DBG                    ((u32)0xFFFFFFFF << 0)          /*!<R 0  crypto engine debug info */
#define IPSEC_ENG_DBG(x)                      ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_ENG_DBG(x)                  ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DMA_LX_DBG
 * @brief
 * @{
 **/
#define IPSEC_MASK_DMA_LX_DBG                 ((u32)0xFFFFFFFF << 0)          /*!<R 0  crypto engine lx bus debug info */
#define IPSEC_DMA_LX_DBG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DMA_LX_DBG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DMA_RX_DBG
 * @brief
 * @{
 **/
#define IPSEC_MASK_DMA_RX_DBG                 ((u32)0xFFFFFFFF << 0)          /*!<R 0  crypto engine dma rx debug info */
#define IPSEC_DMA_RX_DBG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DMA_RX_DBG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DMA_TX_DBG
 * @brief
 * @{
 **/
#define IPSEC_MASK_DMA_TX_DBG                 ((u32)0xFFFFFFFF << 0)          /*!<R 0  crypto engine dma tx debug info */
#define IPSEC_DMA_TX_DBG(x)                   ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DMA_TX_DBG(x)               ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDSR
 * @brief Destination Descriptor Status Register
 * @{
 **/
#define IPSEC_BIT_DES_RST                     ((u32)0x00000001 << 31)          /*!<R/W 0  Destination descriptor reset. (only for pk_up = 1'b1) */
#define IPSEC_MASK_DES_FAIL_STATUS            ((u32)0x00000003 << 25)          /*!<R 0  Destination Descriptor failed status: 2'd1: Users write the first word twice consecutively 2'd2: Users write the second word directly without writing first word in the beginning 2'd3: Overflow (Detect users try to write source descriptor, but there isn't available FIFO node could use). */
#define IPSEC_DES_FAIL_STATUS(x)              ((u32)(((x) & 0x00000003) << 25))
#define IPSEC_GET_DES_FAIL_STATUS(x)          ((u32)(((x >> 25) & 0x00000003)))
#define IPSEC_BIT_DES_FAIL                    ((u32)0x00000001 << 24)          /*!<R/W 0  Destination Descriptor failed interrupt. Write 1 to clear this bit. */
#define IPSEC_MASK_DRPTR                      ((u32)0x000000FF << 16)          /*!<R 0  Destination Descriptor FIFO read pointer. When engine read a descriptor and finished it, DRPTR = DRPTR+2 */
#define IPSEC_DRPTR(x)                        ((u32)(((x) & 0x000000FF) << 16))
#define IPSEC_GET_DRPTR(x)                    ((u32)(((x >> 16) & 0x000000FF)))
#define IPSEC_MASK_DWPTR                      ((u32)0x000000FF << 8)          /*!<R 0  Destination Descriptor FIFO write pointer. When write descriptor to DDSW successfully, DWPTR = DWPTR+2 */
#define IPSEC_DWPTR(x)                        ((u32)(((x) & 0x000000FF) << 8))
#define IPSEC_GET_DWPTR(x)                    ((u32)(((x >> 8) & 0x000000FF)))
#define IPSEC_MASK_DES_DES_FIFO_EMPTY_CNT     ((u32)0x000000FF << 0)          /*!<R 0  Destination Descriptor FIFO empty counter. Use this field to check how many available FIFO nodes in Destination descriptor FIFO could use. */
#define IPSEC_DES_DES_FIFO_EMPTY_CNT(x)       ((u32)(((x) & 0x000000FF) << 0))
#define IPSEC_GET_DES_DES_FIFO_EMPTY_CNT(x)   ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DDFWR
 * @brief Destination Descriptor First Word Register
 * @{
 **/
#define IPSEC_MASK_DDFW                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Descriptor First Word */
#define IPSEC_DDFW(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DDFW(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DDSWR
 * @brief Destination Descriptor Second Word Register
 * @{
 **/
#define IPSEC_MASK_DDSW                       ((u32)0xFFFFFFFF << 0)          /*!<R/W 0  Destination Descriptor Second Word */
#define IPSEC_DDSW(x)                         ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DDSW(x)                     ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DES_PKTCONF
 * @brief Packet Configure Register
 * @{
 **/
#define IPSEC_MASK_CFG_PK_ARBITER_MODE        ((u32)0x00000003 << 30)          /*!<R/W 0  arbiter rule between sec and no-sec access 0:round-robin 1/2/3:decided by the load_count of source descpritor fifo of sec and non-sec,the more full is the winner */
#define IPSEC_CFG_PK_ARBITER_MODE(x)          ((u32)(((x) & 0x00000003) << 30))
#define IPSEC_GET_CFG_PK_ARBITER_MODE(x)      ((u32)(((x >> 30) & 0x00000003)))
#define IPSEC_MASK_CFG_BUS3_PRIORITY_TH       ((u32)0x00000003 << 28)          /*!<R/W 0  the priority of bus3(rsvd now) */
#define IPSEC_CFG_BUS3_PRIORITY_TH(x)         ((u32)(((x) & 0x00000003) << 28))
#define IPSEC_GET_CFG_BUS3_PRIORITY_TH(x)     ((u32)(((x >> 28) & 0x00000003)))
#define IPSEC_MASK_CFG_BUS2_PRIORITY_TH       ((u32)0x00000003 << 26)          /*!<R/W 0  the priority of bus2-- security access. if 0x100C[31:30]=1,the load_count of source descriptor fifo is wr_ptr - rd_ptr + 0x100C[27:26] otherwise,the load_count of source descriptor fifo is wr_ptr - rd_ptr */
#define IPSEC_CFG_BUS2_PRIORITY_TH(x)         ((u32)(((x) & 0x00000003) << 26))
#define IPSEC_GET_CFG_BUS2_PRIORITY_TH(x)     ((u32)(((x >> 26) & 0x00000003)))
#define IPSEC_MASK_CFG_BUS1_PRIORITY_TH       ((u32)0x00000003 << 24)          /*!<R/W   the priority of bus2-- non-security access. if 0x100C[31:30]=1,the load_count of source descriptor fifo is wr_ptr - rd_ptr + 0x100C[25:24] otherwise,the load_count of source descriptor fifo is wr_ptr - rd_ptr */
#define IPSEC_CFG_BUS1_PRIORITY_TH(x)         ((u32)(((x) & 0x00000003) << 24))
#define IPSEC_GET_CFG_BUS1_PRIORITY_TH(x)     ((u32)(((x >> 24) & 0x00000003)))
#define IPSEC_MASK_DBG_DPTR                   ((u32)0x000000FF << 8)          /*!<R/W 0  Destination Descriptor FIFO read pointer: Users read this pointer value can know what the ID number of FIFO node that is now pointed by read pointer is. If Users want to know what the 32bits value of a specified FIFO node, users can write an ID number to this read pointer, then get the 32bits value from Debug Destination Descriptor Data Register. */
#define IPSEC_DBG_DPTR(x)                     ((u32)(((x) & 0x000000FF) << 8))
#define IPSEC_GET_DBG_DPTR(x)                 ((u32)(((x >> 8) & 0x000000FF)))
#define IPSEC_MASK_DBG_SPTR                   ((u32)0x000000FF << 0)          /*!<R/W 0  Source Descriptor FIFO read pointer: Users read this pointer value can know what the ID number of FIFO node that is now pointed by read pointer is. If Users want to know what the 32bits value of a specified FIFO node, users can write an ID number to this read pointer, then get the 32bits value from Debug Source Descriptor Data Register. */
#define IPSEC_DBG_SPTR(x)                     ((u32)(((x) & 0x000000FF) << 0))
#define IPSEC_GET_DBG_SPTR(x)                 ((u32)(((x >> 0) & 0x000000FF)))
/** @} */

/** @defgroup DBGSDR
 * @brief Debug Source Descriptor Data Register
 * @{
 **/
#define IPSEC_MASK_DBG_SD                     ((u32)0xFFFFFFFF << 0)          /*!<R 0  Debug Source Descriptor Data: This register is used with DBG_SPTR. */
#define IPSEC_DBG_SD(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DBG_SD(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */

/** @defgroup DBGDDR
 * @brief Debug Destination Descriptor Data Register
 * @{
 **/
#define IPSEC_MASK_DBG_DD                     ((u32)0xFFFFFFFF << 0)          /*!<R 0  Debug Destination Descriptor Data: This register is used with DBG_DPTR. */
#define IPSEC_DBG_DD(x)                       ((u32)(((x) & 0xFFFFFFFF) << 0))
#define IPSEC_GET_DBG_DD(x)                   ((u32)(((x >> 0) & 0xFFFFFFFF)))
/** @} */
/** @} */



/* Exported types --------------------------------------------------------*/

/** @defgroup HW_CRYPTO_ENGINE_Exported_Types HW_CRYPTO_ENGINE Exported Types
  * @{
*****************************************************************************/


/**
 * @brief IPSEC Register Declaration
 *****************************************************************************/
typedef struct {
	__IO uint32_t SDSR;                                   /*!< SOURCE DESCRIPTOR STATUS REGISTER,  Address offset: 0x000 */
	__IO uint32_t SDFWR;                                  /*!< REG_SDFWR REGISTER,  Address offset: 0x004 */
	__IO uint32_t SDSWR;                                  /*!< SOURCE DESCRIPTOR SECOND WORD REGISTER,  Address offset: 0x008 */
	__IO uint32_t RSVD0;                                  /*!<  Reserved,  Address offset:0xC */
	__IO uint32_t IPSCSR_RSTEACONFISR;                    /*!< RESET ENGINE AND CONFIGURE INTERRUPT SERVICE ROUTINE REGISTER,  Address offset: 0x010 */
	__IO uint32_t IPSCSR_INTM;                            /*!< INTERRUPT MASK REGISTER,  Address offset: 0x014 */
	__IO uint32_t IPSCSR_DBG;                             /*!< DEBUG REGSTER Register,  Address offset: 0x018 */
	__IO uint32_t IPSCSR_ERR_INT;                         /*!< ERROR INTERRUPT REGISTER,  Address offset: 0x01C */
	__IO uint32_t IPSCSR_SADDLR;                          /*!< SUM OF ADDITIONAL AUTHENTICATION DATA LENGTH REGISTER,  Address offset: 0x020 */
	__IO uint32_t IPSCSR_SENLR;                           /*!< SUM OF ENCRYPTION DATA LENGTH REGISTER,  Address offset: 0x024 */
	__IO uint32_t IPSCSR_SAPLR;                           /*!< SUM OF AUTHENTICATION PADDING DATA LENGTH REGISTER,  Address offset: 0x028 */
	__IO uint32_t IPSCSR_SEPLR;                           /*!< SUM OF ENCRYPTION PADDING DATA LENGTH REGISTER,  Address offset: 0x02C */
	__IO uint32_t IPSCSR_SWAPABURSTR;                     /*!< SWAP AND BURST REGISTER,  Address offset: 0x030 */
	__IO uint32_t IPSEKCR;                                /*!< KEY CONTROL REGISTER,  Address offset: 0x034 */
	__IO uint32_t RSVD1[2];                               /*!<  Reserved,  Address offset:0x38-0x3C */
	__I  uint32_t ENG_DBG;                                /*!<  Register,  Address offset: 0x040 */
	__I  uint32_t DMA_LX_DBG;                             /*!<  Register,  Address offset: 0x044 */
	__I  uint32_t DMA_RX_DBG;                             /*!<  Register,  Address offset: 0x048 */
	__I  uint32_t DMA_TX_DBG;                             /*!<  Register,  Address offset: 0x04C */
	__IO uint32_t RSVD2[1004];                              /*!<  Reserved,  Address offset:0x50-0xFFC */
	__IO uint32_t DDSR;                                   /*!< DESTINATION DESCRIPTOR STATUS REGISTER,  Address offset: 0x1000 */
	__IO uint32_t DDFWR;                                  /*!< DESTINATION DESCRIPTOR FIRST WORD REGISTER,  Address offset: 0x1004 */
	__IO uint32_t DDSWR;                                  /*!< DESTINATION DESCRIPTOR SECOND WORD REGISTER,  Address offset: 0x1008 */
	__IO uint32_t DES_PKTCONF;                            /*!< PACKET CONFIGURE REGISTER,  Address offset: 0x100C */
	__I  uint32_t DBGSDR;                                 /*!< DEBUG SOURCE DESCRIPTOR DATA REGISTER,  Address offset: 0x1010 */
	__I  uint32_t DBGDDR;                                 /*!< DEBUG DESTINATION DESCRIPTOR DATA REGISTER,  Address offset: 0x1014 */
	__IO uint32_t DUMMY;                                  /*!<  Register,  Address offset: 0x1018 */
} IPSEC_TypeDef;
/** @} */
/* AUTO_GEN_END */

/* MANUAL_GEN_START */

//Please add your defination here

/* Exported types --------------------------------------------------------*/
/** @addtogroup HW_CRYPTO_ENGINE_Exported_Types HW_CRYPTO_ENGINE Exported Types
  * @{
  */

/**
  * @brief  CRYPTO Source Descriptor Structure Definition
  */
typedef union {
	struct {
		u32 key_len: 4;			/*Offset 0, Bit[3:0], key length				*/
		u32 iv_len: 4;			/*Offset 0, Bit[7:3], IV length				*/
		u32 keypad_len: 8;		/*Offset 0, Bit[15:8], pad length			*/
		u32 hash_iv_len: 6;		/*Offset 0, Bit[21:16], Hash initial value length	*/
		u32 ap: 2;				/*Offset 0, Bit[23:22], auto-padding		*/
		u32 cl: 2;				/*Offset 0, Bit[25:24], Command length		*/
		u32 priv_key: 1;		/*Offset 0, Bit[26], Secure Key				*/
		u32 otp_key: 1;			/*Offset 0, Bit[27], Secure Key				*/
		u32 ls: 1;				/*Offset 0, Bit[28], Last segment descriptor	*/
		u32 fs: 1;				/*Offset 0, Bit[29], First segment descriptor	*/
		u32 rs: 1;				/*Offset 0, Bit[30], Read data source 		*/
		u32 rsvd: 1;			/*Offset 0, Bit[31], Reserved				*/
	} b;

	struct {
		u32 apl: 8;				/*Offset 0, Bit[7:0], Auth padding length		*/
		u32 a2eo: 5;			/*Offset 0, Bit[12:8], Auth to encryption offset	*/
		u32 zero: 1;			/*Offset 0, Bit[13], 0/1					*/
		u32 enl: 14;			/*Offset 0, Bit[27:14], Encryption data length	*/
		u32 ls: 1;				/*Offset 0, Bit[28], Last segment descriptor	*/
		u32 fs: 1;				/*Offset 0, Bit[29], First segment descriptor	*/
		u32 rs: 1;				/*Offset 0, Bit[30], Read data source		*/
		u32 rsvd: 1;			/*Offset 0, Bit[31], Reserved				*/
	} d;

	u32 w;
} rtl_crypto_srcdesc_t;

/**
  * @brief  CRYPTO Destination Descriptor Structure Definition
  */
typedef union {
	struct {
		u32 adl: 8;				/*Offset 0, Bit[7:0], Auth data length		*/
		u32 rsvd1: 19;			/*Offset 0, Bit[26:8], Reserved				*/
		u32 enc: 1;				/*Offset 0, Bit[27], Cipher or auth			*/
		u32 ls: 1;				/*Offset 0, Bit[28], Last segment descriptor	*/
		u32 fs: 1;				/*Offset 0, Bit[29], First segment descriptor	*/
		u32 ws: 1;				/*Offset 0, Bit[30], Write data source		*/
		u32 rsvd2: 1;			/*Offset 0, Bit[31], Reserved				*/
	} auth;

	struct {
		u32 enl: 24;			/*Offset 0, Bit[23:0], Auth padding length		*/
		u32 rsvd1: 3;			/*Offset 0, Bit[26:24], Reserved			*/
		u32 enc: 1;				/*Offset 0, Bit[27], Cipher or auth			*/
		u32 ls: 1;				/*Offset 0, Bit[28], Last segment descriptor	*/
		u32 fs: 1;				/*Offset 0, Bit[29], First segment descriptor	*/
		u32 ws: 1;				/*Offset 0, Bit[30], Write data source		*/
		u32 rsvd2: 1;			/*Offset 0, Bit[31], Reserved				*/
	} cipher;

	u32 w;
} rtl_crypto_dstdesc_t;

/**
  * @brief  CRYPTO Command Buffer Structure Definition
  */
typedef struct rtl_crypto_cl_struct_s {
	// offset 0 :
	u32 cipher_mode: 4;				/*Offset 0, Bit[3:0], Cipher mode			*/
	u32 cipher_eng_sel: 2;			/*Offset 0, Bit[5:4], Cipher algorithm selected	*/
	u32 rsvd1: 1;					/*Offset 0, Bit[6], Reserved				*/
	u32 cipher_encrypt: 1;			/*Offset 0, Bit[7], Encryption or decryption	*/
	u32 aes_key_sel: 2;				/*Offset 0, Bit[9:8], AES key length			*/
	u32 des3_en: 1;					/*Offset 0, Bit[10], 3DES					*/
	u32 des3_type: 1;				/*Offset 0, Bit[11], 3DES type				*/
	u32 ckbs: 1;					/*Offset 0, Bit[12], Cipher key byte swap		*/
	u32 hmac_en: 1;					/*Offset 0, Bit[13], HMAC 				*/
	u32 hmac_mode: 3;				/*Offset 0, Bit[16:14], Hash algorithm		*/
	u32 hmac_seq_hash_last: 1;		/*Offset 0, Bit[17], the last payload(seq hash)	*/
	u32 engine_mode: 3;				/*Offset 0, Bit[20:18], engine mode			*/
	u32 hmac_seq_hash_first: 1;		/*Offset 0, Bit[21], the first payload(seq hash)	*/
	u32 hmac_seq_hash: 1;			/*Offset 0, Bit[22], Seqential hash			*/
	u32 hmac_seq_hash_no_wb: 1;		/*Offset 0, Bit[23], Result hash output		*/
	u32 icv_total_length: 8;		/*Offset 0, Bit[31:24], icv length			*/

	// offset 4
	u32 aad_last_data_size: 4;		/*Offset 4, Bit[3:0], AAD last block data size				*/
	u32 enc_last_data_size: 4;		/*Offset 4, Bit[7:4], Message last block data size			*/
	u32 pad_last_data_size: 3;		/*Offset 4, Bit[10:8], Hash padding last block data size		*/
	u32 ckws: 1;					/*Offset 4, Bit[11], Cipher key word swap					*/
	u32 enc_pad_last_data_size: 3;	/*Offset 4, Bit[14:12], Encryption padding last block data size	*/
	u32 hsibs: 1;					/*Offset 4, Bit[15], Hash sequential initial value byte swap		*/
	u32 caws: 1;					/*Offset 4, Bit[16], Cipher align word swap				*/
	u32 cabs: 1;					/*Offset 4, Bit[17], Cipher align byte swap				*/
	u32 ciws: 1;					/*Offset 4, Bit[18], Cipher input word swap				*/
	u32 cibs: 1;					/*Offset 4, Bit[19], Cipher input byte swap				*/
	u32 cows: 1;					/*Offset 4, Bit[20], Cipher output word swap				*/
	u32 cobs: 1;					/*Offset 4, Bit[21], Cipher output byte swap				*/
	u32 codws: 1;					/*Offset 4, Bit[22], Cipher output double word swap			*/
	u32 cidws: 1;					/*Offset 4, Bit[23], Cipher input double word swap			*/
	u32 haws: 1;					/*Offset 4, Bit[24], Hash align word swap					*/
	u32 habs: 1;					/*Offset 4, Bit[25], Hash align byte swap					*/
	u32 hiws: 1;					/*Offset 4, Bit[26], Hash input word swap					*/
	u32 hibs: 1;					/*Offset 4, Bit[27], Hash input byte swap					*/
	u32 hows: 1;					/*Offset 4, Bit[28], Hash output word swap				*/
	u32 hobs: 1;					/*Offset 4, Bit[29], Hash output byte swap				*/
	u32 hkws: 1;					/*Offset 4, Bit[30], Hash key word swap					*/
	u32 hkbs: 1;					/*Offset 4, Bit[31], Hash key byte swap					*/

	// offset 8
	u32 hash_pad_len: 8;			/*Offset 8, Bit[7:0], Hash padding total length		*/
	u32 header_total_len: 6;		/*Offset 8, Bit[13:8], A2EO total length			*/
	u32 apl: 2;						/*Offset 8, Bit[15:14], Encryption padding total length*/
	u32 enl: 16;					/*Offset 8, Bit[31:16], Message total length		*/

	// offset
	u32 ap0;						/*Offset 12, Bit[31:0], Message length[31:0]		*/
	u32 ap1;						/*Offset 16, Bit[31:0], Message length[63:32]		*/
	u32 ap2;						/*Offset 20, Bit[31:0], Message length[95:64]		*/
	u32 ap3;						/*Offset 24, Bit[31:0], Message length[127:96]	*/
} rtl_crypto_cl_t;

/**
  * @brief  CRYPTO Adapter Definition
  */
typedef struct _HAL_CRYPTO_ADAPTER_ {
	u8 isInit;						/*0: not init, 1: init							*/
	u8 isIntMode;					/*0: disable interrupt mode, 1: enable interrupt mode*/

	u32 cipher_type;				/*cipher type								*/
	u8 aes;							/*AES									*/
	u8 isDecrypt;					/*0: encryption, 1: decryption					*/

	u32 auth_type;					/*auth type								*/
	u8 isHMAC;						/*0: not HMAC, 1: HMAC						*/
	u8 isMD5;						/*MD5									*/
	u8 isSHA1;						/*SHA1									*/
	u8 isSHA2;						/*SHA2									*/
	u8 sha2type;					/*SHA2 type: SHA2_224/SHA2_256			*/

	u32 enc_last_data_size;			/*message last data size						*/
	u32 aad_last_data_size;			/*AAD last data size							*/

	u32 lenAuthKey;					/*Auth key length							*/
	const u8  *pAuthKey;			/*Auth key								*/
	u32 digestlen;					/*digest									*/

	// sequential hash
	u8 hmac_seq_hash_first;			/* seq hash the first message payload block		*/
	u8 hmac_seq_hash_last;			/* seq hash the last message payload block		*/
	u32 hmac_seq_hash_total_len;	/* seq hash message total length 				*/
	u8 hmac_seq_is_recorded;		/* enter seq hash or not						*/
	u8 *hmac_seq_last_message;		/* previous message payload					*/
	u32 hmac_seq_last_msglen;		/* previous message payload length				*/
	u8 hmac_seq_buf_is_used_bytes;	/* seq buf used bytes(total 64-bytes)			*/

	u32 	lenCipherKey;			/*Cipher key length							*/
	const u8  	*pCipherKey;		/*Cipher key								*/

	u32 a2eo, apl_aad, enl, apl;	/*length									*/

	u8 *ipad;						/*HMAC ipad								*/
	u8 *opad;						/*HMAC opad								*/

	volatile u8 g_IOPAD[128 * 2 + 4] __attribute__((aligned(4)));
	volatile u8  gcm_iv[32] __attribute__((aligned(4)));
	volatile u8  cl_buffer[32] __attribute__((aligned(4)));
	volatile u8  hmac_seq_buf[128] __attribute__((aligned(4)));
} HAL_CRYPTO_ADAPTER, *PHAL_CRYPTO_ADAPTER ;

/**
  * @brief SHA-256 Context Structure Definition
  */
typedef struct {
	u8 isInit;						/*0: not init, 1: init							*/
	u8 isIntMode;					/*0: disable interrupt mode, 1: enable interrupt mode*/
	u8 isMD5;						/*MD5									*/
	u8 isSHA1;						/*SHA1									*/

	u32 usedlen;         			 /*!< number of bytes processed  */
	u32 state[16] __attribute__((aligned(8)));            /*!< intermediate digest state  */
	u8 isSHA2;						/*SHA2									*/
	u8 sha2type;					/*SHA2 type: SHA2_224/SHA2_256			*/
	u8 seq_hash;					/*1: enable initial value */
	u8 initial;
	u32 digestlen;					/*digest									*/
	u32 auth_type;					/*auth type								*/

	// sequential hash
	u8 hmac_seq_hash_first;			/* seq hash the first message payload block		*/
	u8 hmac_seq_hash_last;			/* seq hash the last message payload block		*/
	u8 hmac_seq_is_recorded;		/* enter seq hash or not						*/
	u8 hmac_seq_buf_is_used_bytes;	/* seq buf used bytes(total 64-bytes)			*/
	u32 hmac_seq_hash_total_len;	/* seq hash message total length 				*/
	u32 lasthash;
	unsigned char buffer[128];   	/*!< data block being processed */

} hw_sha_context;

/**
  * @brief SHA2 Type Definitions
  */
typedef enum _SHA2_TYPE_ {
	SHA2_NONE 	= 0,
	SHA2_224 	= 224 / 8,
	SHA2_256 	= 256 / 8,
	SHA2_384 	= 384 / 8,
	SHA2_512 	= 512 / 8
} SHA2_TYPE;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @addtogroup HW_CRYPTO_ENGINE_Exported_Constants HW_CRYPTO_ENGINE Exported Constants
  * @{
  */

/** @defgroup CRYPTO_Length
  * @{
  */
#define CRYPTO_MAX_DIGEST_LENGTH	32  /* SHA256 Digest length : 32 */
#define CRYPTO_MAX_KEY_LENGTH		32  /* MAX  is  AES-256 : 32 byte,  3DES : 24 byte */
#define CRYPTO_PADSIZE 				64
#define CRYPTO_AUTH_PADDING			64
#define CRYPTO_PADSIZE_512 			128
#define CRYPTO_AUTH_PADDING_512		128

#define CRYPTO_MD5_DIGEST_LENGTH 	16
#define CRYPTO_SHA1_DIGEST_LENGTH 	20
#define CRYPTO_SHA2_DIGEST_LENGTH 	32
/**
  * @}
  */

/** @defgroup CRYPTO_Authentication_Type
  * @{
  */
#define AUTH_TYPE_NO_AUTH 				((u32)-1)

#define AUTH_TYPE_MASK_FUNC				0x3	/* bit 0, bit 1*/
#define AUTH_TYPE_MD5					0x2
#define AUTH_TYPE_SHA1					0x0
#define AUTH_TYPE_SHA2					0x1

#define AUTH_TYPE_MASK_HMAC 			0x4	/* bit 2 */
#define AUTH_TYPE_HMAC_MD5 				(AUTH_TYPE_MD5 | AUTH_TYPE_MASK_HMAC)
#define AUTH_TYPE_HMAC_SHA1 			(AUTH_TYPE_SHA1 | AUTH_TYPE_MASK_HMAC)
#define AUTH_TYPE_HMAC_SHA2 			(AUTH_TYPE_SHA2 | AUTH_TYPE_MASK_HMAC)

#define AUTH_TYPE_MASK_FUNC_ALL			(AUTH_TYPE_MASK_FUNC| AUTH_TYPE_MASK_HMAC)

// SHA2
#define AUTH_TYPE_MASK_SHA2 			0xf0 /* bit 4,5,6,7 */
#define AUTH_TYPE_SHA2_224				0x10
#define AUTH_TYPE_SHA2_256  			0x20
#define AUTH_TYPE_SHA2_384				0x40
#define AUTH_TYPE_SHA2_512  			0x80

#define AUTH_TYPE_SHA2_224_ALL  		(AUTH_TYPE_SHA2_224|AUTH_TYPE_SHA2)
#define AUTH_TYPE_SHA2_256_ALL  		(AUTH_TYPE_SHA2_256|AUTH_TYPE_SHA2)
#define AUTH_TYPE_HMAC_SHA2_224_ALL		(AUTH_TYPE_SHA2_224|AUTH_TYPE_HMAC_SHA2)
#define AUTH_TYPE_HMAC_SHA2_256_ALL		(AUTH_TYPE_SHA2_256|AUTH_TYPE_HMAC_SHA2)

#define AUTH_TYPE_SHA2_384_ALL  		(AUTH_TYPE_SHA2_384|AUTH_TYPE_SHA2)
#define AUTH_TYPE_SHA2_512_ALL  		(AUTH_TYPE_SHA2_512|AUTH_TYPE_SHA2)
#define AUTH_TYPE_HMAC_SHA2_384_ALL		(AUTH_TYPE_SHA2_384|AUTH_TYPE_HMAC_SHA2)
#define AUTH_TYPE_HMAC_SHA2_512_ALL		(AUTH_TYPE_SHA2_512|AUTH_TYPE_HMAC_SHA2)
/**
  * @}
  */

/** @defgroup CRYPTO_Cipher_Type
  * @{
  */
#define CIPHER_TYPE_NO_CIPHER 		((u32)-1)

#define CIPHER_TYPE_MODE_ENCRYPT 	0x80

#define CIPHER_TYPE_MASK_FUNC 		0x30 /* 0x00 : DES, 0x10: 3DES, 0x20: AES */
#define CIPHER_TYPE_FUNC_AES 		0x20

#define CIPHER_TYPE_MASK_BLOCK 		0xF 	/*0x0 : ECB, 0x1: CBC, 0x2: CFB , 0x3 : OFB , 0x4 : CTR, 0x5 : GCTR, 0x6: GMAC, 0x7: GHASH, 0x8: GCM*/
#define CIPHER_TYPE_BLOCK_ECB 		0x0
#define CIPHER_TYPE_BLOCK_CBC 		0x1
#define CIPHER_TYPE_BLOCK_CFB 		0x2
#define CIPHER_TYPE_BLOCK_OFB 		0x3
#define CIPHER_TYPE_BLOCK_CTR 		0x4
#define CIPHER_TYPE_BLOCK_GCTR 		0x5
#define CIPHER_TYPE_BLOCK_GMAC 		0x6
#define CIPHER_TYPE_BLOCK_GHASH 	0x7
#define CIPHER_TYPE_BLOCK_GCM 		0x8
//
#define CIPHER_TYPE_AES_ECB			0x20
#define CIPHER_TYPE_AES_CBC			0x21
#define CIPHER_TYPE_AES_CFB			0x22
#define CIPHER_TYPE_AES_OFB			0x23
#define CIPHER_TYPE_AES_CTR			0x24  // 0x29
#define CIPHER_TYPE_AES_GCTR		0x25
#define CIPHER_TYPE_AES_GMAC		0x26
#define CIPHER_TYPE_AES_GHASH		0x27
#define CIPHER_TYPE_AES_GCM			0x28
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup HW_CRYPTO_ENGINE_Exported_Functions HW_CRYPTO_ENGINE Exported Functions
  * @{
  */

/** @defgroup HW_CRYPTO_ENGINE_Normal_Functions
  * @{
  */
_LONG_CALL_ void CRYPTO_MemDump(const u8 *start, u32 size, char *strHeader);
_LONG_CALL_ void CRYPTO_CleanCmdOk(u32 engine_type);
_LONG_CALL_ void CRYPTO_ClearAllINT(u32 engine_type);
_LONG_CALL_ void CRYPTO_Reset(HAL_CRYPTO_ADAPTER *pIE);
_LONG_CALL_ void CRYPTO_SHA_Reset(HAL_CRYPTO_ADAPTER *pIE);

_LONG_CALL_ int CRYPTO_SetSecurityModeAD(HAL_CRYPTO_ADAPTER *pIE, IN const u32 cipher_type, IN const u32 auth_type, IN const void *pCipherKey,
		IN const u32 lenCipherKey, IN const void *pAuthKey, IN const u32 lenAuthKey);
_LONG_CALL_ int CRYPTO_Init(HAL_CRYPTO_ADAPTER *pIE);
_LONG_CALL_ int CRYPTO_SHA_Init(HAL_CRYPTO_ADAPTER *pIE);
_LONG_CALL_ int CRYPTO_OTPKey_Init(u32 keynum, u32 status);
_LONG_CALL_ int CRYPTO_OTPKey_SHA_Init(u32 keynum, u32 status);
_LONG_CALL_ int CRYPTO_ProcessAD(HAL_CRYPTO_ADAPTER *pIE, IN const u8 *message, IN const u32 msglen, IN const u8 *pIv, IN const u32 ivlen, IN const u8 *paad,
								 IN const u32 aadlen, OUT u8 *pResult, OUT u8 *pTag, hw_sha_context *ctx);
_LONG_CALL_ int CRYPTO_CipherInit(HAL_CRYPTO_ADAPTER *pIE, IN const u32 cipher_type, IN const u8 *key, IN const u32 keylen);
_LONG_CALL_ int CRYPTO_CipherEncryptAD(HAL_CRYPTO_ADAPTER *pIE, IN const u8 *message, IN const u32 msglen, IN const u8 *piv, IN const u32 ivlen,
									   IN const u8 *paad, IN const u32 aadlen, OUT u8 *pResult, OUT u8 *pTag);
_LONG_CALL_ int CRYPTO_CipherDecryptAD(HAL_CRYPTO_ADAPTER *pIE, IN const u8 *message, IN const u32 msglen, IN const u8 *piv, IN const u32 ivlen,
									   IN const u8 *paad, IN const u32 aadlen, OUT u8 *pResult, OUT u8 *pTag);
_LONG_CALL_ int CRYPTO_SendSeqBuf(u8 *pDigest, hw_sha_context *ctx);
_LONG_CALL_ void hash_save(hw_sha_context *ctx);

/**
  * @}
  */

/**
  * @}
  */

/** @} */

/** @} */

/* Other Definitions --------------------------------------------------------*/
#define FIFOCNT_TIMEOUT		0x100000
extern HAL_CRYPTO_ADAPTER crypto_engine;
extern HAL_CRYPTO_ADAPTER crypto_sha_engine;
extern u32 SHA_OTPKEY;

#define AES_ENGINE	0x0
#define SHA_ENGINE	0x1
#define IPSEC_DMA_BVAL	((u32)0x00000001 << 17)

__STATIC_INLINE
int rtl_cryptoEngine_init(void)
{
	return CRYPTO_Init(NULL);
}

/* *** dump macro definition *** */
#undef crypto_dbg_mem_dump
#define crypto_dbg_mem_dump(start, size, str_header) \
	if (ConfigDebug[LEVEL_INFO] & BIT(MODULE_IPSEC)) { \
		DiagPrintf("%s(): memdump : address: %08x, size: %d\n", "rtl_cryptoEngine_dbg", start, size); \
		CRYPTO_MemDump((const u8*)(start), size, (char*)(str_header)); \
	}


/* MANUAL_GEN_END */

#endif
