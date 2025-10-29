/*************************************************************
 * @file        driver_uart0.c
 * @brief       code of UART0 driver of BK7231
 * @author      GuWenFu
 * @version     V1.0
 * @date        2016-09-29
 * @par
 * @attention
 *
 * @history     2016-09-29 gwf    create this file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#include "BK_System.h"
//#include "driver_system.h"
//#include "driver_gpio.h"
#include "driver_uart0.h"
#include "boot_protocol.h"


u32 uart_dl_port = DEBUG_PORT_UART0;
//volatile bool uart0_rx_done = FALSE;
//volatile unsigned long uart0_rx_index = 0;
extern rx_link_buf_t rx_link_buf;


void uart0_send(unsigned char *buff, int len);
void uart0_disable(void){};

//----------------------------------------------
// UART0 Interrupt Service Rountine
//----------------------------------------------
extern rx_link_buf_t			rx_link_buf;
void UART_InterruptHandler(void) 
{
	u32      status;
	u8       value;
	u16      next_wr_idx;

	rx_link_buf_t  * link_buf = &rx_link_buf;
	u8	* rx_temp_buff = (u8 *)&link_buf->rx_buf[0];
	status = REG_UART0_INT_STATUS;

	if(status & (UART0_INT_ENABLE_RX_NEED_RD_MASK | UART0_INT_ENABLE_RX_STOP_END_MASK))
	{
		while ((UART0_FIFO_STATUS_RD_READY_GET))
		{
			value = (unsigned char)UART0_READ_BYTE();;
			next_wr_idx = (link_buf->write_idx + 1);
			if(next_wr_idx >= sizeof(link_buf->rx_buf))
				next_wr_idx = 0;

			if(next_wr_idx != link_buf->read_idx)
			{
				rx_temp_buff[link_buf->write_idx] = value;
				link_buf->write_idx = next_wr_idx;
			}
			else
			{
				
			}
            //uart0_send((u8 *)&value, 1);  // for test
                
		}
	}
    
	REG_UART0_INT_STATUS = status;

}

//----------------------------------------------
// UART Tx/Rx character driver
//----------------------------------------------
static void uart0_send_byte(unsigned char data)
{
    while (!UART0_TX_WRITE_READY);
    UART0_WRITE_BYTE(data);
   	//bk_printf("data :0x%x\r\n ",data);
}
void uart0_send(unsigned char *buff, int len)
{
    while (len--)
    {	
        uart0_send_byte(*buff++);
    }
}

void uart0_send_string(const char *buff)
{
    while (*buff)
        uart0_send_byte(*buff++);
}

void uart0_wait_tx_finish(void)
{
    while (UART0_TX_FIFO_EMPTY == 0)
    {
    }
}


#if (PRINT_PORT == DEBUG_PORT_UART0)

#endif
//eof

