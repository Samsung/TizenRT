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
All structures and function prototypes to be implemented by the low-level  
SPI drivers are provided in the header file **[spi.h](../os/include/tinyara/spi/spi.h)**
under *os/include/tinyara/spi* folder.

### SPI device driver initialization
Each SPI device driver must implement the definitions of below prototypes for  
SPI device initialization
```
FAR struct spi_dev_s *up_spiinitialize(int port);
```

Example implementation is present in **[s5j_spi.c](../os/arch/arm/src/s5j/s5j_spi.c)**
under *os/arch/arm/src/s5j* folder.
```
struct spi_dev_s *up_spiinitialize(int port)
{
	FAR struct s5j_spidev_s *priv = NULL;

	if (port < 0 || port >= SPI_PORT_MAX) {
		return NULL;
	}

	switch (port) {
	case 0:
		priv = &g_spi0dev;
		break;

	...

	}

	...

	return (struct spi_dev_s *)priv;
}
```

Low level(hardware-specific) SPI device driver must create an instace of  
*struct spi_dev_s* and returned to higher level device driver as shown above.  
Also the SPI driver must create an instance of *struct spi_ops_s* and  
hook it to the *ops* member of *struct spi_dev_s* instance
```
struct spi_dev_s {
	FAR const struct spi_ops_s *ops;
};

struct spi_ops_s {
#ifndef CONFIG_SPI_OWNBUS
	int (*lock)(FAR struct spi_dev_s *dev, bool lock);
#endif
	void (*select)(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool selected);
	uint32_t (*setfrequency)(FAR struct spi_dev_s *dev, uint32_t frequency);
	void (*setmode)(FAR struct spi_dev_s *dev, enum spi_mode_e mode);
	void (*setbits)(FAR struct spi_dev_s *dev, int nbits);
	uint8_t (*status)(FAR struct spi_dev_s *dev, enum spi_dev_e devid);
#ifdef CONFIG_SPI_CMDDATA
	int (*cmddata)(FAR struct spi_dev_s *dev, enum spi_dev_e devid, bool cmd);
#endif
	uint16_t (*send)(FAR struct spi_dev_s *dev, uint16_t wd);
#ifdef CONFIG_SPI_EXCHANGE
	void (*exchange)(FAR struct spi_dev_s *dev, FAR const void *txbuffer, FAR void *rxbuffer, size_t nwords);
#else
	void (*sndblock)(FAR struct spi_dev_s *dev, FAR const void *buffer, size_t nwords);
	void (*recvblock)(FAR struct spi_dev_s *dev, FAR void *buffer, size_t nwords);
#endif
	int (*registercallback)(FAR struct spi_dev_s *dev, spi_mediachange_t callback, void *arg);
};

```

The above structure *struct spi_ops_s* defines a list of function pointers to be  
implemented by the low level SPI device driver.

Example implementation is present in **[s5j_spi.c](../os/arch/arm/src/s5j/s5j_spi.c)**
under *os/arch/arm/src/s5j* folder.
```
static const struct spi_ops_s g_spiops = {
#ifndef CONFIG_SPI_OWNBUS
	.lock = spi_lock,
#endif
	.select = spi_select,
	.setfrequency = spi_setfrequency,
	.setmode = (void *)spi_setmode,
	.setbits = (void *)spi_setbits,
	.status = 0,
#ifdef CONFIG_SPI_CMDDATA
	.cmddata = 0,
#endif
	.send = spi_send,
#ifdef CONFIG_SPI_EXCHANGE
	.exchange = spi_exchange,
#else
	.sndblock = spi_sndblock,
	.recvblock = spi_recvblock,
#endif
	.registercallback = 0,
};

static int spi_lock(struct spi_dev_s *dev, bool lock)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

        ...

	return OK;
}

static void spi_select(struct spi_dev_s *dev, enum spi_dev_e devid, bool selected)
{
	FAR struct s5j_spidev_s *priv = (FAR struct s5j_spidev_s *)dev;

	...

}
```

## I2C

## I2S
