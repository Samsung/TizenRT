# How to Use Peripheral

Here are 4 parts, [UART](#uart), [SPI](#spi), [I2C](#i2c) and [I2S](#i2s) to use peripheral.

## UART
UART has two parts, setting early console and setting serial console driver, serial port drivers.

### Setting of early console
#### Add up_earlyserialinit function
Boot code must call this before os starting, to provide low level debug messgaes during bootup.
```
void up_earlyserialinit(void)
{
	...
	/* Configure whichever one is the console */

#ifdef HAVE_SERIAL_CONSOLE
	CONSOLE_DEV.isconsole = true;
	CONSOLE_DEV.ops->setup(&CONSOLE_DEV);
#endif
}
```
Please refer CONSOLE_DEV at [below tab](#add-uart-device-structure).

#### Add up_putc and up_lowputc function
This provides sending character by accessing MCU registers.
```
int up_putc(int ch)
{
#if defined(HAVE_SERIAL_CONSOLE)
	/* Check for LF */
	if (ch == '\n') {
		/* Add CR */
		up_lowputc('\r');
	}

	up_lowputc(ch);
#endif

	return ch;
}

void up_lowputc(char ch)
{
#ifdef HAVE_SERIAL_CONSOLE
	/* Wait until the TX FIFO is not full */

	while ((getreg32(XXX_CONSOLE_BASE + XXX_UART_FR_OFFSET) & UART_FR_TXFF) != 0) ;

	/* Then send the character */

	putreg32((uint32_t) ch, XXX_CONSOLE_BASE + XXX_UART_DR_OFFSET);
#endif
}
```

### Setting of serial console driver, serial port drivers
#### Add up_serialinit function
This function registers character UART device drivers.  
Os calls **up_serialinit** to initialize architecture specific functionality through *up_initialize*.
```
void up_serialinit(void)
{
	uart_register("dev/console", &CONSOLE_DEV);
	uart_register("dev/ttyS0", &TTYS0_DEV);
	uart_register("dev/ttyS1", &TTYS1_DEV);
	...
}
```

### Add UART device structure
The ```CONSOLE_DEV``` and ```TTYSx_DEV``` are UART device driver structure type named *uart_dev_t*.  
That is defined in **[serial.h](../os/include/tinyara/serial/serial.h)** under *os/include/tinyara/serial* folder.
```
struct uart_dev_s {
	...
	/* I/O buffers */

	struct uart_buffer_s xmit;	/* Describes transmit buffer */
	struct uart_buffer_s recv;	/* Describes receive buffer */

	/* Driver interface */

	FAR const struct uart_ops_s *ops;	/* Arch-specific operations */
	FAR void *priv;				/* Used by the arch-specific logic */
	...
};

typedef struct uart_dev_s uart_dev_t;
```

The **xmit**, **recv**, **ops** and **priv** should be defined.  
> **xmit** : size and address of transmit buffer  
> **recv** : size and address of receive buffer  
> **ops**  : tables of operations like setup, send, receive and etc  
> **priv** : private data of device like baudrate, interrupt number, parity and ect

Here is an example.
```
#define CONSOLE_DEV		g_uart0port		/* UART0 is console */

static uart_dev_t g_uart0port = {
	.recv = {
		.size	= CONFIG_UART0_RXBUFSIZE,
		.buffer	= g_uart0rxbuffer,
	},
	.xmit = {
		.size	= CONFIG_UART0_TXBUFSIZE,
		.buffer	= g_uart0txbuffer,
	},
	.ops		= &g_uart_ops,
	.priv		= &g_uart0priv,
};

static struct up_dev_s g_uart0priv = {
	.uartbase	= UART0_BASE,
	.baud		= CONFIG_UART0_BAUD,
	.irq		= IRQ_UART0,
	.parity		= CONFIG_UART0_PARITY,
	.bits		= CONFIG_UART0_BITS,
	.stopbits2	= CONFIG_UART0_2STOP,
	.rxd		= GPIO_UART0_RXD,
	.txd		= GPIO_UART0_TXD,
	.pclk		= CLK_GATE_UART0_PCLK,
	.extclk		= CLK_GATE_UART0_EXTCLK,
};
```

## SPI

## I2C

## I2S
