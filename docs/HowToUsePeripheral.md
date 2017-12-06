# How to Use Peripheral

## Peripherals
> [UART](#uart)  
> [SPI](#spi)  
> [I2S](#i2s)  
> [MPU](#mpu)  

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

## MPU
* MPU enables you to partition memory into regions and set individual protection attributes for each region.
* It supports 12 or 16 memory regions, depending on the configuration(ARMv7-R has 12 memory regions).
* Each region is programmed with a base address and size, and the regions can be overlapped to enable efficient programming of the memory map.
* To support overlapping, the regions are assigned priorities, with region 0 having the lowest priority and region 11 or 15 having the highest priority respectively when 12 or 16 memory regions are supported.
* The MPU returns access permissions and attributes for the highest priority region where the address hits.
* The MPU is programmed using CP15 registers c1 and c6.
* Memory region control read and write access is permitted only from privileged modes.

### MPU Memory Regions
* '''Region base address''': The base address defines the start of the memory region. IT must be aligned to a region-sized boundary.
For example, if a region size of 4KB is programmed for a given region, the base address must be a multiple of 4KB.
* Subregions: Each region can be split into eight equal sized non-overlapping subregions.
* Region attributes: Each region has a number of attributes associated with it.
* Region access permissions: Each region can be given no access, read-only access, or read/write access permissions for privileged or all modes. In addition, each region can be marked as eXecute Never (XN) to prevent instructions being fetched from that region.
For example, if a user mode application attempts to access a Privileged mode access only region, a permission fault occurs. 

### MPU Initialization
Initialzation of MPU is done by setting up memory regions. Memory regions are set up by using below functions and each function requires 2 arguments which are _Region Base Address_ and _Region Size_ to setup a memory region.

```
-----------------------------------
       MPU Memory Region Info
-----------------------------------
RNUM:	int32 - Region Number
RBASE:	int32 - Region Base Address
RSIZE:	int32 - Region Size
RATTR:	int32 - Region Attirbute
-----------------------------------
       MPU Init Functions
-----------------------------------
mpu_priv_stronglyordered()
mpu_priv_non_cache()
mpu_priv_flash()
mpu_priv_intsram()
mpu_priv_extsram()
mpu_priv_intsram_wb()
mpu_peripheral()
mpu_user_flash()
mpu_user_intsram()
mpu_user_extsram()
mpu_user_intsram_wb()
mpu_control()
mpu_setupregions()
-----------------------------------
```

The functions with name ```_priv_``` are used to configure the _kernel_(Privileged) MPU regions whereas the ```_user_``` functions are called to configure the _user_(Un-Privileged) MPU regions.

MPU region setup related macros and functions are defined in **[mpu.h](../os/arch/arm/src/armv7-r/mpu.h)** under *os/arch/arm/src/armv7-r/* directory.

### Example:
```
#ifdef CONFIG_ARMV7M_MPU
int s5j_mpu_initialize(void)
{
#ifdef CONFIG_ARCH_CHIP_S5JT200
	/*
	 * Vector Table		0x02020000	0x02020FFF	4
	 * Reserved		0x02021000	0x020217FF	2
	 * BL1			0x02021800	0x020237FF	8
	 * TizenRT		0x02023800	0x0210FFFF	946(WBWA)
	 * WIFI			0x02110000	0x0215FFFF	320(NCNB)
	 */

	/* Region 0, Set read only for memory area */
	mpu_priv_flash(0x0, 0x80000000);

	/* Region 1, for ISRAM(0x02020000++1280KB, RW-WBWA */
	mpu_user_intsram_wb(S5J_IRAM_PADDR, S5J_IRAM_SIZE);

	/* Region 2, wifi driver needs non-$(0x02110000++320KB, RW-NCNB */
	mpu_priv_noncache(S5J_IRAM_PADDR + ((4 + 2 + 8 + 946) * 1024), (320 * 1024));

	/* Region 3, for FLASH area, default to set WBWA */
	mpu_user_intsram_wb(S5J_FLASH_PADDR, S5J_FLASH_SIZE);

	/* region 4, for Sflash Mirror area to be read only */
	mpu_priv_flash(S5J_FLASH_MIRROR_PADDR, S5J_FLASH_MIRROR_SIZE);

	/* Region 5, for SFR area read/write, strongly-ordered */
	mpu_priv_stronglyordered(S5J_PERIPHERAL_PADDR, S5J_PERIPHERAL_SIZE);

	/*
	 * Region 6, for vector table,
	 * set the entire high vector region as read-only.
	 */
	mpu_priv_flash(S5J_IRAM_MIRROR_PADDR, S5J_IRAM_MIRROR_SIZE);

	mpu_control(true);
#endif
	return 0;
}
#endif
```
