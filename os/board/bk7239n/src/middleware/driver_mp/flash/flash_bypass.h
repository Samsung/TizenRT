#ifndef __FLASH_BYPASS_H__
#define __FLASH_BYPASS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CURRENT_ROUTINE_TEXT_SIZE       (0x400)

/*Write Enable for Status Register*/
#define FLASH_CMD_WR_EN_SR              (0x6)
#define FLASH_CMD_WR_DISABLE            (0x04)

/*Write Status Register*/
#define FLASH_CMD_WR_SR                 (0x31)

/*Others*/
#define FLASH_CMD_QUAD_IO_FAST_READ     (0xEB)
#define FLASH_GD25Q32C_SR_QUAD_EN       (0x2)
#define FLASH_CMD_ENTER_DEEP_PWR_DW     (0xB9)
#define FLASH_CMD_EXIT_DEEP_PWR_DW      (0xAB)

/*SYS regs define*/
#define SYS_R_ADD_X(x)                  (0x44010000+((x)*4))

/*SPI0 regs define*/
#define SPI_CFG_TRX_LEN_MASK            (0xffffU)
#define SPI_CFG_TX_TRAHS_LEN_POSI       (8)
#define SPI_CFG_RX_TRAHS_LEN_POSI       (20)
#define SPI_CFG_RX_FIN_INT_EN           (BIT(3))
#define SPI_CFG_TX_FIN_INT_EN           (BIT(2))
#define SPI_CFG_RX_EN                   (BIT(1))
#define SPI_CFG_TX_EN                   (BIT(0))
#define SPI_CFG_TX_EN_ONE_BYTE          (0x10D)
#define SPI_CFG_TX_EN_TWO_BYTE          (0x20D)
#define SPI_STATUS_TX_FINISH_INT        (BIT(13))
#define SPI_STATUS_TXFIFO_WR_READY      (BIT(1))
#define SPI_STATUS_RXFIFO_RD_READY      (BIT(2))

#define FLASH_ID_GD25Q32C               (0xC84016)
#define QE_RETRY_TIMES                  (10)

#if defined(CONFIG_SOC_BK7236XX)
__attribute__((section(".itcm_sec_code"))) int flash_bypass_op_write(uint8_t *op_code, uint8_t *tx_buf, uint32_t tx_len);
#else
__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_enable(void);
__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_test(uint32_t quad_enable, uint32_t delay_cycle1, uint32_t delay_cycle2);
#endif

#ifdef __cplusplus
}
#endif
#endif //__FLASH_BYPASS_H__
// eof

