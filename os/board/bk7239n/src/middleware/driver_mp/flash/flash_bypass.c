#include <common/bk_include.h>
#include "bk_arm_arch.h"
#include <common/sys_config.h>
#include "flash_bypass.h"
#include "sys_driver.h"
#include "gpio_driver.h"
#include <driver/spi.h>
#include "spi_hal.h"
#include "driver/flash.h"

#if defined(CONFIG_SOC_BK7236XX)
#define SPI_R_0X2(_id)  (SPI_R_BASE(_id) + 2 * 0x04)
__attribute__((section(".iram"))) int flash_bypass_op_write(uint8_t *op_code, uint8_t *tx_buf, uint32_t tx_len)
{
	uint32_t reg;
	uint32_t reg_0x2, reg_ctrl, reg_dat;
	uint32_t reg_stat, reg_cfg;
	uint32_t int_status = 0;
	int exceptional_flag = 0;
	uint32_t reg_sys_clk_en_0xc, reg_sys_clk_sel_0xa;

	int_status =  rtos_disable_int();

	/*step 1, save spi register configuration*/
	reg_0x2 = REG_READ(SPI_R_0X2(0));
	reg_ctrl = REG_READ(SPI_R_CTRL(0));
	reg_stat = REG_READ(SPI_R_INT_STATUS(0));
	reg_dat  = REG_READ(SPI_R_DATA(0));
	reg_cfg  = REG_READ(SPI_R_CFG(0));

	/*step 2, en software reset bit, bk7236/58 should enable this bit*/
	reg = REG_READ(SPI_R_0X2(0));
	reg |= (1 << 0);
	REG_WRITE(SPI_R_0X2(0), reg);

	/*step 3, config spi master*/
	/*     3.1 clear spi fifo content*/
	reg = REG_READ(SPI_R_INT_STATUS(0));
	while (reg & SPI_STATUS_RXFIFO_RD_READY) {
		REG_READ(SPI_R_DATA(0));
		reg = REG_READ(SPI_R_INT_STATUS(0));
	}
	/*     3.2 disable spi block, and backup */
	reg_sys_clk_en_0xc = reg = REG_READ(SYS_R_ADD_X(0xc));
	reg &= ~(1 << 1);
	REG_WRITE(SYS_R_ADD_X(0xc), reg);

	/*     3.3 clear spi status*/
	REG_WRITE(SPI_R_CTRL(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), 0);

	/*     3.4 save the previous setting status*/
	/* gpioi of SPI0 are set as high-impedance state or input state ,
	          for spi mux with them*/
	//gpio_dev_unmap(SPI0_LL_CSN_PIN);
	//gpio_dev_unmap(SPI0_LL_SCK_PIN);
	//gpio_dev_unmap(SPI0_LL_MOSI_PIN);
	//gpio_dev_unmap(SPI0_LL_MISO_PIN);


	//gpio_dev_map(SPI0_LL_SCK_PIN, GPIO_DEV_SPI0_SCK);
	//gpio_dev_map(SPI0_LL_CSN_PIN, GPIO_DEV_SPI0_CSN);
	//gpio_dev_map(SPI0_LL_MOSI_PIN, GPIO_DEV_SPI0_MOSI);
	//gpio_dev_map(SPI0_LL_MISO_PIN, GPIO_DEV_SPI0_MISO);

	/*     3.5 set the spi master mode*/
	// open clock, and backup
	reg = REG_READ(SYS_R_ADD_X(0xc));
	reg |= (1 << 1);
	REG_WRITE(SYS_R_ADD_X(0xc), reg);

	// select 26M
	reg_sys_clk_sel_0xa = reg = REG_READ(SYS_R_ADD_X(0xa));
	reg &= ~(1 << 4);
	REG_WRITE(SYS_R_ADD_X(0xa), reg);

	// set to spi config directly
	reg = 0xC00100; // spien  msten  spi_clk=1---13M
	REG_WRITE(SPI_R_CTRL(0), reg);

	/*step 4, gpio(14/15/16/17) are set as high-impedance state or input state */

	/*step 5, switch flash interface to spi
	 *        Pay attention to prefetch instruction destination, the text can not
	 *        fetch from flash space after this timepoint.
	 */
	//sys_drv_set_cpu_storage_connect_op_select_flash_sel(1);
	reg = REG_READ(SYS_R_ADD_X(0x2));
	reg |= (1 << 9);
	REG_WRITE(SYS_R_ADD_X(0x2), reg);

	if(op_code != NULL)
	{
		/*step 6, write enable for volatile status register: 50H*/
		/*      6.1:take cs*/
		reg = REG_READ(SPI_R_CFG(0));
		reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_TX_TRAHS_LEN_POSI);
		reg |= (1 << SPI_CFG_TX_TRAHS_LEN_POSI);
		reg |= (SPI_CFG_TX_EN | SPI_CFG_TX_FIN_INT_EN);
		REG_WRITE(SPI_R_CFG(0), reg);

		/*      6.2:write tx fifo*/
		reg = REG_READ(SPI_R_INT_STATUS(0));
		if (reg & SPI_STATUS_TXFIFO_WR_READY)
			REG_WRITE(SPI_R_DATA(0), *op_code);
		else {
			exceptional_flag = -1;
			goto wr_exceptional;
		}

		/*      6.3:waiting for TXFIFO_EMPTY interrupt*/
		while (1) {
			reg = REG_READ(SPI_R_INT_STATUS(0));
			if (reg & SPI_STATUS_TX_FINISH_INT)
				break;
		}

		/*      6.4:release cs*/
		reg = REG_READ(SPI_R_CFG(0));
		reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_TX_TRAHS_LEN_POSI);
		reg &= ~(SPI_CFG_TX_EN | SPI_CFG_TX_FIN_INT_EN);
		REG_WRITE(SPI_R_CFG(0), reg);

		// cler stat and fifo
		reg = REG_READ(SPI_R_INT_STATUS(0));
		while (reg & SPI_STATUS_RXFIFO_RD_READY) {
			REG_READ(SPI_R_DATA(0));
			reg = REG_READ(SPI_R_INT_STATUS(0));
		}
		reg = REG_READ(SPI_R_INT_STATUS(0));
		REG_WRITE(SPI_R_INT_STATUS(0), reg);
	}

	if((tx_len == 0) || (tx_buf == NULL))
	{
		exceptional_flag = 0;
		goto wr_exceptional;
	}

	/*step 7, for tx_buf */
	/*      7.1:take cs*/
	reg = REG_READ(SPI_R_CFG(0));
	reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_TX_TRAHS_LEN_POSI);
	reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_RX_TRAHS_LEN_POSI);
	reg |= ((tx_len & SPI_CFG_TRX_LEN_MASK) << SPI_CFG_TX_TRAHS_LEN_POSI);
	reg |= (SPI_CFG_TX_EN | SPI_CFG_TX_FIN_INT_EN);
	REG_WRITE(SPI_R_CFG(0), reg);

	/*      7.2:write tx fifo*/
	// write tx first
	for (int i = 0, wait = 0; i < tx_len; ) {
		reg = REG_READ(SPI_R_INT_STATUS(0));
		if ((reg & SPI_STATUS_TXFIFO_WR_READY) == 0) {
			for(volatile int j=0; j<500; j++);
			wait++;
			if(wait > 100) {
				exceptional_flag = -2;
				goto wr_exceptional;
			}
		} else {
			wait = 0;
			REG_WRITE(SPI_R_DATA(0), tx_buf[i]);
			i++;
		}
	}

	/*      7.3:waiting for TXFIFO_EMPTY interrupt*/
	while (1) {
		reg = REG_READ(SPI_R_INT_STATUS(0));
		if (reg & SPI_STATUS_TX_FINISH_INT)
			break;
	}

	/*      7.4:release cs*/
	reg = REG_READ(SPI_R_CFG(0));
	reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_TX_TRAHS_LEN_POSI);
	reg &= ~(SPI_CFG_TRX_LEN_MASK << SPI_CFG_RX_TRAHS_LEN_POSI);
	reg &= (SPI_CFG_RX_EN | SPI_CFG_TX_EN | SPI_CFG_TX_FIN_INT_EN);
	REG_WRITE(SPI_R_CFG(0), reg);

	// cler stat and fifo
	reg = REG_READ(SPI_R_INT_STATUS(0));
	while (reg & SPI_STATUS_RXFIFO_RD_READY) {
		REG_READ(SPI_R_DATA(0));
		reg = REG_READ(SPI_R_INT_STATUS(0));
	}
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	exceptional_flag = 0;

wr_exceptional:
	/*step 8, switch flash interface to flash controller */
	//sys_drv_set_cpu_storage_connect_op_select_flash_sel(0);
	reg = REG_READ(SYS_R_ADD_X(0x2));
	reg &= ~(1 << 9);
	REG_WRITE(SYS_R_ADD_X(0x2), reg);

	// recover icu and powerdown bits for spi
	REG_WRITE(SYS_R_ADD_X(0xc), reg_sys_clk_en_0xc);
	REG_WRITE(SYS_R_ADD_X(0xa), reg_sys_clk_sel_0xa);

	/*step 9, gpio(14/15/16/17) second function*/

	/*step 10, restore spi register configuration*/
	REG_WRITE(SPI_R_CTRL(0), reg_ctrl);
	REG_WRITE(SPI_R_INT_STATUS(0), reg_stat);
	REG_WRITE(SPI_R_DATA(0), reg_dat);
	REG_WRITE(SPI_R_CFG(0), reg_cfg);
	REG_WRITE(SPI_R_0X2(0), reg_0x2);

	/*step 11, enable interrupt*/
	rtos_enable_int(int_status);

	return exceptional_flag;
}
#else /*CONFIG_SOC_BK7236XX*/

void flash_bypass_init(void) {
	char *text_ptr, temp_buf = 0;
	uint32_t reg;
	
	/*step 2, resident cache*/
	REG_WRITE(SPI_R_CTRL(0), 0);
	do {
		text_ptr = (char *)flash_bypass_quad_enable;
		for (uint32_t i = 0; i < CURRENT_ROUTINE_TEXT_SIZE; i ++)
			temp_buf += text_ptr[i];

		REG_WRITE(SPI_R_INT_STATUS(0), temp_buf);
	} while (0);

	/*step 3, config spi master*/
	/*  clear spi status*/
	REG_WRITE(SPI_R_CTRL(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), 0);

	spi_config_t config = {0};
	config.role = SPI_ROLE_MASTER;
	config.bit_width = SPI_BIT_WIDTH_8BITS;
	config.polarity = 1;
	config.phase = 1;
	config.wire_mode = SPI_4WIRE_MODE;
	config.baud_rate = 1000000;
	config.bit_order = SPI_MSB_FIRST;
#if (defined(CONFIG_SPI_BYTE_INTERVAL))
	config.byte_interval = 1;
#endif
	bk_spi_driver_init();
	bk_spi_init(0, &config);

	/*step 4, gpioi of SPI0 are set as high-impedance state or input state ,
	          for spi mux with them*/
	gpio_dev_unmap(SPI0_LL_CSN_PIN);
	gpio_dev_unmap(SPI0_LL_SCK_PIN);
	gpio_dev_unmap(SPI0_LL_MOSI_PIN);
	gpio_dev_unmap(SPI0_LL_MISO_PIN);
}

__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_enable(void)
{
	uint32_t reg;
	uint32_t reg_ctrl, reg_dat;
	uint32_t reg_stat, reg_cfg;
	uint32_t int_status = 0;
	uint32_t spi_status = 0;
	volatile uint32_t i, j, delay_count;

	int_status =  rtos_disable_int();

	/*step 1, save spi register configuration*/
	reg_ctrl = REG_READ(SPI_R_CTRL(0));
	reg_stat = REG_READ(SPI_R_INT_STATUS(0));
	reg_dat  = REG_READ(SPI_R_DATA(0));
	reg_cfg  = REG_READ(SPI_R_CFG(0));

	flash_bypass_init();

	/*step 5, switch flash interface to spi
	 *        Pay attention to prefetch instruction destination, the text can not
	 *        fetch from flash space after this timepoint.
	 */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(1);

	/*step 6, write enable for status register: 06H*/
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_ONE_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_EN_SR);

	/*step 7, write cmd 31H, data 0x02*/
	for(i = 0; i < 500; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < 20000; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}

	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_TWO_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_SR);
	REG_WRITE(SPI_R_DATA(0), FLASH_GD25Q32C_SR_QUAD_EN);

	for(i = 0; i < 500; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < 20000; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}
	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	/*step 8, switch flash interface to flash controller */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(0);

	/*step 9, restore spi register configuration*/
	REG_WRITE(SPI_R_CTRL(0), reg_ctrl);
	REG_WRITE(SPI_R_INT_STATUS(0), reg_stat);
	REG_WRITE(SPI_R_DATA(0), reg_dat);
	REG_WRITE(SPI_R_CFG(0), reg_cfg);
	rtos_enable_int(int_status);
}


__attribute__((section(".itcm_sec_code"))) void flash_bypass_quad_test(uint32_t quad_enable, uint32_t delay_cycle1, uint32_t delay_cycle2)
{
	uint32_t reg;
	uint32_t reg_ctrl, reg_dat;
	uint32_t reg_stat, reg_cfg;
	uint32_t int_status = 0;
	uint32_t spi_status = 0;
	volatile uint32_t i, j, delay_count;

	int_status =  rtos_disable_int();

	/*step 1, save spi register configuration*/
	reg_ctrl = REG_READ(SPI_R_CTRL(0));
	reg_stat = REG_READ(SPI_R_INT_STATUS(0));
	reg_dat  = REG_READ(SPI_R_DATA(0));
	reg_cfg  = REG_READ(SPI_R_CFG(0));

	flash_bypass_init();

	/*step 5, switch flash interface to spi
	 *        Pay attention to prefetch instruction destination, the text can not
	 *        fetch from flash space after this timepoint.
	 */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(1);

	/*step 6, write enable for status register: 06H*/
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);
	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_ONE_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_EN_SR);

	/*step 7, write cmd 31H, data 0x02*/
	for(i = 0; i < delay_cycle1; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < delay_cycle2; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}

	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	REG_WRITE(SPI_R_CFG(0), SPI_CFG_TX_EN_TWO_BYTE);
	REG_WRITE(SPI_R_DATA(0), FLASH_CMD_WR_SR);
	if (quad_enable) {
		REG_WRITE(SPI_R_DATA(0), FLASH_GD25Q32C_SR_QUAD_EN);
	} else {
		REG_WRITE(SPI_R_DATA(0), 0);
	}

	for(i = 0; i < delay_cycle1; i++) {
		spi_status = REG_READ(SPI_R_INT_STATUS(0));
		if(0 != (spi_status & SPI_STATUS_TX_FINISH_INT)) {
			break;
		}
	}
	for(delay_count = 0; delay_count < delay_cycle2; delay_count ++)
	{
		for(j = 0; j < 8; j ++)
		;
	}
	REG_WRITE(SPI_R_CFG(0), 0);
	reg = REG_READ(SPI_R_INT_STATUS(0));
	REG_WRITE(SPI_R_INT_STATUS(0), reg);

	/*step 8, switch flash interface to flash controller */
	sys_drv_set_cpu_storage_connect_op_select_flash_sel(0);

	/*step 9, restore spi register configuration*/
	REG_WRITE(SPI_R_CTRL(0), reg_ctrl);
	REG_WRITE(SPI_R_INT_STATUS(0), reg_stat);
	REG_WRITE(SPI_R_DATA(0), reg_dat);
	REG_WRITE(SPI_R_CFG(0), reg_cfg);
	rtos_enable_int(int_status);
}

#endif
