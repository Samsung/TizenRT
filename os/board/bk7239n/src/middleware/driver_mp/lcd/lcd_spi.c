#include <common/bk_include.h>
#include <os/mem.h>
#include <os/str.h>
#include <os/os.h>
#include <stdlib.h>
#include <driver/lcd_spi.h>
#include <driver/gpio.h>
#include "bk_misc.h"
#include <driver/hal/hal_gpio_types.h>
#include "gpio_driver.h"



static void SPI_SendData(uint8_t data)
{
	uint8_t n;

	//in while loop, to avoid disable IRQ too much time, release it if finish one byte.
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();
	for (n = 0; n < 8; n++)
	{
		if (data & 0x80)
		{
			bk_gpio_set_output_high(LCD_SPI_SDA_GPIO);
		}
		else
		{
			bk_gpio_set_output_low(LCD_SPI_SDA_GPIO);
		}

		delay_us(LCD_SPI_DELAY);
		data <<= 1;

		bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
		delay_us(LCD_SPI_DELAY);
		bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
		delay_us(LCD_SPI_DELAY);

	}
	GLOBAL_INT_RESTORE();
}

void lcd_spi_write_cmd(uint8_t cmd)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_low(LCD_SPI_SDA_GPIO);
	delay_us(LCD_SPI_DELAY);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);

	SPI_SendData(cmd);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}
void lcd_spi_write_data(uint8_t data)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_SDA_GPIO);
	delay_us(LCD_SPI_DELAY);

	bk_gpio_set_output_low(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);
	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	delay_us(LCD_SPI_DELAY);

	SPI_SendData(data);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}

void lcd_spi_write_hf_word_data(unsigned int data)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);


	SPI_SendData(0x40);
	SPI_SendData(data);


	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}
void lcd_spi_write_hf_word_cmd(unsigned int cmd)
{
	bk_gpio_set_output_low(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);

	SPI_SendData(0x20);
	SPI_SendData(cmd >> 8); //high 8bit

	SPI_SendData(0x00);	  //low 8bit
	SPI_SendData(cmd);

	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(LCD_SPI_DELAY);
}

void lcd_spi_init_gpio(void)
{
	gpio_dev_unmap(LCD_SPI_RST);
	bk_gpio_set_capacity(LCD_SPI_RST, 0);
	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_RST));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_RST));

	gpio_dev_unmap(LCD_SPI_CLK_GPIO);
	bk_gpio_set_capacity(LCD_SPI_CLK_GPIO, 0);
	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CLK_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CLK_GPIO));

	gpio_dev_unmap(LCD_SPI_CSX_GPIO);
	bk_gpio_set_capacity(LCD_SPI_CSX_GPIO, 0);
	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_CSX_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_CSX_GPIO));

	gpio_dev_unmap(LCD_SPI_SDA_GPIO);
	bk_gpio_set_capacity(LCD_SPI_SDA_GPIO, 0);
	BK_LOG_ON_ERR(bk_gpio_disable_input(LCD_SPI_SDA_GPIO));
	BK_LOG_ON_ERR(bk_gpio_enable_output(LCD_SPI_SDA_GPIO));

	bk_gpio_set_output_high(LCD_SPI_CLK_GPIO);
	bk_gpio_set_output_high(LCD_SPI_CSX_GPIO);
	delay_us(200);
}


