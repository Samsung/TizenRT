#ifndef _BL_UART_H_
#define _BL_UART_H_
#include <stdint.h>
#include "type.h"
#include "bl_bk_reg.h"
#include "bl_config.h"

#include <stdbool.h>          // standard boolean definitions
#define UART0_RX_FIFO_MAX_COUNT  (128)
#define UART0_TX_FIFO_MAX_COUNT  (128)
#define RX_FIFO_THR_COUNT        (32)

extern volatile char uart_rx_done_state ;
extern volatile unsigned long uart_rx_index ;
extern unsigned char uart_rx_end_flag;


enum
{
	UART_RX_IDLE = 0,
	UART_RX_START = 1,
	UART_RX_END  = 2,

};

#define UART_DEBUG

#ifdef UART_DEBUG
    #define UART_PRT               bl_printf
#else
    #define UART_PRT 
#endif

#define UART_TX_POLL_MODE
#define UART_DEBUG_TX_POLL_MODE

#define PRINT_BUF_SIZE              256

#define FLOW_CTL_HIGH_CNT          (96)
#define FLOW_CTL_LOW_CNT           (32)

#define DEF_TX_EN                   0x1
#define DEF_RX_EN                   0x1
#define DEF_IRDA_MODE               0x0    // 0:uart mode  1:IRDA MODE
#define DEF_DATA_LEN                0x3  // 0=5bit, 1=6bit, 2=7bit, 3=8bit
#define DEF_PARITY_EN               0x0  // 0=no parity  1: enable parity
#define DEF_PARITY_MODE             0x0  // 0:odd  1: even
#define DEF_STOP_BIT                0x0  // 1bit

#define UART_SYS_CLK                26000000 //For FPGA

#define DEF_BAUD_RATE	            115200
#define DEF_DEBUG_BAUD_RATE	        115200

#define DEF_TX_FIFO_THRE            64
#define DEF_RX_FIFO_THRE            64
#define DEF_RX_TIMEOUT              0
#define RX_BUF_LEN                  2048
#define TX_BUF_LEN                  512

#define TX_BUF_DEBUG_LEN            384

#define TX_FIFO_MAX_COUNT           128


#define DEBUG_TX_FIFO_MAX_COUNT     16

#if 0
#define PRINT_BUF_PREPARE(rc, buf, fmt)													\
    int rc;											\
    va_list args;                                   \
    va_start(args, fmt);                            \
    rc = vsnprintf(buf, sizeof(buf), fmt, args);    \
    va_end(args);                                   \
    buf[sizeof(buf) - 1] = '\0';
#else
#define PRINT_BUF_PREPARE(rc, buf, fmt)	int rc;
#endif

int bl_printf(const char *fmt, ...);
int ad_printf(const char *fmt, ...);

void *bl_memcpy(void *d, const void *s, size_t n);
void *bl_memset(void *s, int c, size_t n);
void bl_flash_read_cbus(uint32_t address, void *user_buf, uint32_t size);
void bl_flash_write_cbus(uint32_t address, const uint8_t *user_buf, uint32_t size);

void boot_uart_init(uint32_t baud_rate,uint8_t rx_fifo_thr);


void uart_tx_init_enable(u8 en);
void uart_send(unsigned char *buff, int len);

void UART_InterruptHandler(void);

#endif // _BL_UART_H_
// EOF
