# How to Use Peripheral

Here are 4 parts, [UART](#uart), [SPI](#spi), [I2C](#i2c) and [I2S](#i2s) to use peripheral.

## UART
UART has two parts, setting early console and setting serial console driver, serial port drivers.

### Setting of early console
#### Add up_earlyserialinit function
Boot code must call this before os starting, to provide low level debug messages during bootup.
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
> **priv** : private data of device like baudrate, interrupt number, parity and etc

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

Low level(hardware-specific) SPI device driver must create an instance of  
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

Each board, shall;  

Implement and expose an instance of struct i2c_dev_s.     
Implement the supported operations on i2c struct i2c_op_s.  


```
struct i2c_dev_s {
	const struct i2c_ops_s *ops;	/* I2C vtable */
	FAR void *priv;			/* Used by the arch-specific logic */
}; 
``` 
  
```
struct i2c_ops_s {
	uint32_t (*setfrequency)(FAR struct i2c_dev_s *dev, uint32_t frequency);
	int (*setaddress)(FAR struct i2c_dev_s *dev, int addr, int nbits);
	int (*write)(FAR struct i2c_dev_s *dev, const uint8_t *buffer, int buflen);
	int (*read)(FAR struct i2c_dev_s *dev, uint8_t *buffer, int buflen);
#ifdef CONFIG_I2C_WRITEREAD
	int (*writeread)(FAR struct i2c_dev_s *inst, const uint8_t *wbuffer, int wbuflen, uint8_t *rbuffer, int rbuflen);
#endif

#ifdef CONFIG_I2C_TRANSFER
	int (*transfer)(FAR struct i2c_dev_s *dev, FAR struct i2c_msg_s *msgs, int count);
#endif
#ifdef CONFIG_I2C_SLAVE
	int (*setownaddress)(FAR struct i2c_dev_s *dev, int addr, int nbits);

	int (*registercallback)(FAR struct i2c_dev_s *dev, int (*callback)(void));
#endif
};
```

Above declarations are present in **[i2c.h](../os/include/tinyara/i2c.h)**.  
  
In addition to above, each board level logic shall implement the other functions listed in **[i2c.h](../os/include/tinyara/i2c.h)**.  

```
1. EXTERN FAR struct i2c_dev_s *up_i2cinitialize(int port);
2. EXTERN int up_i2cuninitialize(FAR struct i2c_dev_s *dev);
3. EXTERN int up_i2creset(FAR struct i2c_dev_s *dev);
```  

### I2C user level I/O
If user level I/O is needed on i2c port, then board level logic should provide the following for registration and fs operations(read, write, writeread) as mentioned in i2c_ops above.  

```
#ifdef CONFIG_I2C_USERIO
int i2c_uioregister(FAR const char *path, FAR struct i2c_dev_s *dev);
#endif
```
>NOTE:Be aware that i2c user I/O is at premature stage  

### I2C Initialization  
I2C is initialized by invoking up_i2cinitialize.  
Initialization takes one i2c port and returns a i2c device structure.  
Initialization is done either as part of _up_initialize()_ or as part of _board_initialize()_.  

Please refer to **[i2c.h](../os/include/tinyara/i2c.h)** to know more about the i2c functions and declarations.  

## I2S

I2S port for a board involves the following;
1. I2S Device Structure and its Operations.
2. I2S Initialization and Device Access.  

### I2S Device Structure and Operations
Each board, shall;  

Implement and expose an instance of struct i2s_dev_s for each port.     
Implement the supported operations on i2s, ie struct i2s_op_s. 
Data structures below; 

```
/* I2S private data.  This structure only defines the initial fields of the
 * structure visible to the I2S client.  The specific implementation may
 * add additional, device specific fields
 */

struct i2s_dev_s {
	FAR const struct i2s_ops_s *ops;
};

struct i2s_ops_s {
	/* Receiver methods */

	CODE uint32_t (*i2s_rxsamplerate)(FAR struct i2s_dev_s *dev, uint32_t rate);
	CODE uint32_t (*i2s_rxdatawidth)(FAR struct i2s_dev_s *dev, int bits);
	CODE int (*i2s_receive)(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, i2s_callback_t callback, FAR void *arg, uint32_t timeout);

	/* Transmitter methods */

	CODE uint32_t (*i2s_txsamplerate)(FAR struct i2s_dev_s *dev, uint32_t rate);
	CODE uint32_t (*i2s_txdatawidth)(FAR struct i2s_dev_s *dev, int bits);
	CODE int (*i2s_send)(FAR struct i2s_dev_s *dev, FAR struct ap_buffer_s *apb, i2s_callback_t callback, FAR void *arg, uint32_t timeout);

	/* Errors handling methods */

	CODE int (*i2s_err_cb_register)(FAR struct i2s_dev_s *dev, i2s_err_cb_t cb, FAR void *arg);

	/* Generic stop method */
	CODE int (*i2s_stop)(FAR struct i2s_dev_s *dev);
};
```

Above declarations are present in **[i2s.h](../os/include/tinyara/audio/i2s.h)**.  
Each board should implement each of the above operations.

Refer **[s5j_i2s.c](../os/arch/arm/src/s5j/s5j_i2s.c)** for sample implementation of i2s operations on s5j chipset.
Sample code from s5j_i2s.c below, all operations implemented as static functions
are attached to the i2s device operations.
```
	/* static function to set sample rate */
static uint32_t i2s_samplerate(struct i2s_dev_s *dev, uint32_t rate)
{
	...
}
	/* Static function to set each sample data width */
static uint32_t i2s_txdatawidth(struct i2s_dev_s *dev, int bits)
{
	...
}
	/* Wrapping static implementation of i2s operations with i2s device */
static const struct i2s_ops_s g_i2sops = {
	/* Receiver methods */

	.i2s_rxsamplerate = i2s_samplerate,
	.i2s_rxdatawidth = i2s_rxdatawidth,
	.i2s_receive = i2s_receive,

	/* Transmitter methods */

	.i2s_txsamplerate = i2s_samplerate,
	.i2s_txdatawidth = i2s_txdatawidth,
	.i2s_send = i2s_send,

	.i2s_stop = i2s_stop,
	.i2s_err_cb_register = i2s_err_cb_register,
};
```
### I2S Initialization and Device Access
Board shall export ```struct i2s_dev_s* xxx_i2s_initialize(uint16_t port)``` to initialize a i2s port and get the corresponding device structure.    
**NOTE**: Current code in s5j_i2s.c has initialize function as ```struct i2s_dev_s* s5j_i2s_initialize(uint16_t port)```.

This i2s initialize function shall be called with the port number to get the corresponding device structure.

