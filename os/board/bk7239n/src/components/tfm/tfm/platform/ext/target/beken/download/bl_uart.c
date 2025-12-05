#include "system.h"
#include "bl_uart.h"
#include "cmsis_gcc.h"
#include "soc/bk7239n/reg_base.h"


#define TX_FIFO_THRD                0x40
#define RX_FIFO_THRD                0x40
// Wait for maximum 60ms, CPU 26M/Flash 26M, 1ms loops about 1700 times
// 1700 * 6 = 10200
#define MAX_LOOP_CNT (102000)
#define ANA_REG(id)  (SOC_SYSTEM_REG_BASE + ((0x40 + id) << 2))

volatile char uart_rx_done_state = 0;
volatile unsigned long uart_rx_index = 0;
volatile unsigned long uart_rx_index_cur = 0;
unsigned char uart_rx_end_flag = 0;
char buf[128 / 2];
u16 COUNT;
u32 g_baud_rate;

typedef enum {
	CLOCK_XTAL_FLASH_26M = 1,
	CLOCK_PLL_FLASH_40M,
	CLOCK_PLL_FLASH_80M,
} flash_clock_t;

typedef enum {
	CLOCK_XTAL_CPU_26M = 1,
	CLOCK_PLL_CPU_40M,
	CLOCK_PLL_CPU_80M,
	CLOCK_PLL_CPU_120M,
    CLOCK_PLL_CPU_160M,
} cpu_clock_t;

/*
 * CPU     FLASH     1ms loop cnt   1ms loop + one time reg read
 * -----  --------  -------------- -----------------------------
 * 26M    26M        3000           1700
 * 120M   60M        10000          5300
 * */

static inline void hal_loop_delay(uint32_t loop_cnt)
{
    volatile uint32_t loop = 0;
    while (loop++ < loop_cnt);
}

static inline void ana_reg_write(uint32_t id, uint32_t v)
{
    REG_WRITE(ANA_REG(id), v);
    volatile uint32_t loop_cnt = 0;
    while ((REG_READ(SOC_SYSTEM_REG_BASE + (0x3a << 2)) & (1 << (id))) && (loop_cnt++ < MAX_LOOP_CNT));
}

void hal_enable_pll(void)
{
	// Initial register values
	uint32_t ana_reg0_val = 0x09219d00;  // sys0x40 corresponds to ana_reg_0
	uint32_t ana_reg1_val = 0xc02e3a00;  // sys0x41 corresponds to ana_reg_1
	uint32_t ana_reg5_val = 0x8407a340;  // sys0x45 corresponds to ana_reg_5

	ana_reg_write(0, ana_reg0_val);

	// 1. Enable PLL (set bit 5 of ana_reg_5)
	ana_reg5_val |= (1 << 5);  // Set bit 5
	ana_reg_write(5, ana_reg5_val);

	// 2. Enable SPI reset (set bit 14 of ana_reg_1) and Set osccal_trig (set bit 15 of ana_reg_1)
	ana_reg1_val |= (1 << 14);  // Set bit 14
	ana_reg1_val |= (1 << 15);  // Set bit 15
	ana_reg_write(1, ana_reg1_val);

	// 3. Delay for 200us
	hal_loop_delay(1000); //from peng.liu

	// 4. Clear osccal_trig (clear bit 15 of ana_reg_1) trig twice, provided by tenglong.
	ana_reg1_val &= ~(1 << 15);  // Clear bit 15
	ana_reg_write(1, ana_reg1_val);

	// 5. Set osccal_trig (set bit 15 of ana_reg_1)
	ana_reg1_val |= (1 << 15);  // Set bit 15
	ana_reg_write(1, ana_reg1_val);

	// 6. Delay for 200us
	hal_loop_delay(1000); //from peng.liu

	// 7. Disable CBEN (clear bit 11 of ana_reg_0)
	ana_reg0_val &= ~(1 << 11);  // Clear bit 11
	ana_reg_write(0, ana_reg0_val);

	// 8. Synchronize ASIC
	addSYSTEM_Reg0xc |= (1 << 20);
}

void hal_set_cpu_clock(cpu_clock_t clock)
{
	if(clock == CLOCK_PLL_CPU_160M){
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0xf))) | 0x02);// sel 480M PLL ; div4
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0x30))) | 0x10);// sel 480M PLL ; div4 ,support 120M, 1st set div
	}else if (clock == CLOCK_PLL_CPU_120M) {
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0xf))) | 0x03);// sel 480M PLL ; div4
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0x30))) | 0x10);// sel 480M PLL ; div4 ,support 120M, 1st set div
	} else if (clock == CLOCK_PLL_CPU_80M) {
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0xf))) | 0x05);// sel 480M PLL ; div6;// sel 480M PLL ; div6 ,support 80M, 1st set div
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0x30))) | 0x10);// 2nd, set clk
	} else {
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0xf))) | 0x00);// 1st sel 0 div : 40M
		addSYSTEM_Reg0x8 = ((addSYSTEM_Reg0x8 & (~(0x30))) | 0x00);// 2nd, set XTAL clk
	}
}
void boot_uart_init(uint32_t baud_rate,uint8_t rx_fifo_thr)
{
    uint32_t baud_divisor;

    g_baud_rate = baud_rate;
    hal_enable_pll();
    hal_set_cpu_clock(CLOCK_PLL_CPU_120M);
    addSYSTEM_Reg0xa |= (1 << 12);
    baud_divisor           = UART_CLOCK_FREQ/baud_rate;
    baud_divisor           = baud_divisor-1;

    COUNT = UART_TX_FIFO_COUNT;

    REG_APB3_UART_CFG      = (   (DEF_STOP_BIT    << sft_UART_CONF_STOP_LEN)
                                 | (DEF_PARITY_MODE << sft_UART_CONF_PAR_MODE)
                                 | (DEF_PARITY_EN   << sft_UART_CONF_PAR_EN)
                                 | (DEF_DATA_LEN    << sft_UART_CONF_UART_LEN)
                                 | (baud_divisor    << sft_UART_CONF_CLK_DIVID)
                                 | (DEF_IRDA_MODE   << sft_UART_CONF_IRDA)
                                 | (DEF_RX_EN       << sft_UART_CONF_RX_ENABLE)
                                 | (DEF_TX_EN       << sft_UART_CONF_TX_ENABLE));

    REG_APB3_UART_FIFO_THRESHOLD = ((rx_fifo_thr << sft_UART_FIFO_CONF_RX_FIFO)
                                    | (TX_FIFO_THRD << sft_UART_FIFO_CONF_TX_FIFO));
    REG_APB3_UART_INT_ENABLE=0;             /* Disable UART Interrupts */
    REG_APB3_UART_INT_ENABLE = bit_UART_INT_RX_NEED_READ | bit_UART_INT_RX_STOP_END ; //enable Rx interrupt

}

void boot_uart_init_tx(uint32_t baud_rate,uint8_t rx_fifo_thr)
{
    uint32_t baud_divisor;
    g_baud_rate = baud_rate;
    baud_divisor           = UART_CLOCK_FREQ/baud_rate;
    baud_divisor           = baud_divisor-1;

    COUNT = UART_TX_FIFO_COUNT;

    REG_APB3_UART_CFG      = (   (DEF_STOP_BIT    << sft_UART_CONF_STOP_LEN)
                                 | (DEF_PARITY_MODE << sft_UART_CONF_PAR_MODE)
                                 | (DEF_PARITY_EN   << sft_UART_CONF_PAR_EN)
                                 | (DEF_DATA_LEN    << sft_UART_CONF_UART_LEN)
                                 | (baud_divisor    << sft_UART_CONF_CLK_DIVID)
                                 | (DEF_IRDA_MODE   << sft_UART_CONF_IRDA)
//                                 | (DEF_RX_EN       << sft_UART_CONF_RX_ENABLE)
                                 | (DEF_TX_EN       << sft_UART_CONF_TX_ENABLE));

    REG_APB3_UART_FIFO_THRESHOLD = ((rx_fifo_thr << sft_UART_FIFO_CONF_RX_FIFO)
                                    | (TX_FIFO_THRD << sft_UART_FIFO_CONF_TX_FIFO));
    REG_APB3_UART_INT_ENABLE=0;             /* Disable UART Interrupts */
//    REG_APB3_UART_INT_ENABLE = bit_UART_INT_RX_NEED_READ | bit_UART_INT_RX_STOP_END ; //enable Rx interrupt

}



void debug_uart_send_poll( uint8_t *buff, int len )
{
//	return;
    while (len--) {
        while(!UART_TX_WRITE_READY);
        UART_WRITE_BYTE(*buff++);
    }
}
void debug2_uart_send_poll( UINT8 *buff, int len )
{
    while (len--) {
        while(!UART_TX_WRITE_READY);
        UART_WRITE_BYTE(*buff++);
    }
}


int bl_printf(const char *fmt, ...)
{
	return 0;
    PRINT_BUF_PREPARE(rc, buf, fmt);
    debug_uart_send_poll((uint8_t *)&buf[0], rc);
    return rc;
}

int ad_printf(const char *fmt, ...)
{
		return 0;
    PRINT_BUF_PREPARE(rc, buf, fmt);
    debug_uart_send_poll((uint8_t *)&buf[0], rc);
    return rc;
}

void *bl_memcpy(void *d, const void *s, size_t n)
{
        /* attempt word-sized copying only if buffers have identical alignment */

        unsigned char *d_byte = (unsigned char *)d;
        const unsigned char *s_byte = (const unsigned char *)s;
        const uint32_t mask = sizeof(uint32_t) - 1;

        if ((((uint32_t)d ^ (uint32_t)s_byte) & mask) == 0) {

                /* do byte-sized copying until word-aligned or finished */

                while (((uint32_t)d_byte) & mask) {
                        if (n == 0) {
                                return d;
                        }
                        *(d_byte++) = *(s_byte++);
                        n--;
                };

                /* do word-sized copying as long as possible */

                uint32_t *d_word = (uint32_t *)d_byte;
                const uint32_t *s_word = (const uint32_t *)s_byte;

                while (n >= sizeof(uint32_t)) {
                        *(d_word++) = *(s_word++);
                        n -= sizeof(uint32_t);
                }

                d_byte = (unsigned char *)d_word;
                s_byte = (unsigned char *)s_word;
        }

        /* do byte-sized copying until finished */

        while (n > 0) {
                *(d_byte++) = *(s_byte++);
                n--;
        }

        return d;
}


void *bl_memset(void *s, int c, size_t n)
{
    unsigned char *ss = s;
    while (n--) {
        *ss++ = c;
    }
    return s;
}

void uart_send(unsigned char *buff, int len)
{
	 while (len--) {
	        while(!UART_TX_WRITE_READY);
	        UART_WRITE_BYTE(*buff++);
	    }
}
