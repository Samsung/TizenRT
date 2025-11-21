#pragma once
#include "driver/dvp_camera_types.h"
//#include <driver/hal/hal_gpio_types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    SPI_GPIO_CLK,
    SPI_GPIO_CSX,
    SPI_GPIO_SDA,
    SPI_GPIO_RST
}LCD_SPI_GPIO_TYPE_E;

extern int32_t lcd_driver_get_spi_gpio(LCD_SPI_GPIO_TYPE_E gpio_type);
#define LCD_SPI_CLK_GPIO  lcd_driver_get_spi_gpio(SPI_GPIO_CLK)
#define LCD_SPI_CSX_GPIO  lcd_driver_get_spi_gpio(SPI_GPIO_CSX)
#define LCD_SPI_SDA_GPIO  lcd_driver_get_spi_gpio(SPI_GPIO_SDA)
#define LCD_SPI_RST       lcd_driver_get_spi_gpio(SPI_GPIO_RST)

#define LCD_SPI_DELAY     2

void lcd_spi_write_cmd(uint8_t data);
void lcd_spi_init_gpio(void);
void lcd_spi_write_data(uint8_t data);


void lcd_spi_write_hf_word_cmd(unsigned int cmd);
void lcd_spi_write_hf_word_data(unsigned int data);



#ifdef __cplusplus
}
#endif

