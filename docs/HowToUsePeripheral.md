
# How to Use Peripheral

Here are 6 parts, [GPIO](#gpio), [UART](#uart), [SPI](#spi), [PWM](#pwm), [I2C](#i2c) and [I2S](#i2s) to use peripheral.

## GPIO
Each board, shall;

Implement and expose an instance of `struct gpio_lowerhalf_s`.  
Implement the supported operations on `struct gpio_ops_s`.

### Registering GPIO on dev

```c
struct gpio_lowerhalf_s {
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;
};
```

```c
struct [BOARD]_lowerhalf_s {
	/* 
	* Must include common member value 
	*	struct gpio_lowerhalf_s 
	*/
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;

	/* Including private value */
	...
};
```

The implement of these structs will be good to follow below steps on its own BSP code.

```c
struct [BOARD]_lowerhalf_s {
	/* 
	* Must include common member value 
	*	struct gpio_lowerhalf_s 
	*/
	FAR const struct gpio_ops_s *ops;
	struct gpio_upperhalf_s *parent;

	/* Including private value */
	...
};

FAR struct gpio_lowerhalf_s *[BOARD]_gpio_lowerhalf(gpio_pinset_t pinset)
{
	/* 
	* gpio_pinset_t : unsigned int32 value for PIN setting. 
	*
	* 1. Allocate Board specific GPIO struct
	* 2. Set private value
	* 3. Set operation struct.
	*/

	return (struct gpio_lowerhalf_s *)([BOARD]_lowerhalf_s);
}
```

>struct gpio_lowerhalf_s *lower = [BOARD]_gpio_lowerhalf(pinset);  
>gpio_register(pin_number, lower);  

If these steps worked well, the result will be shown on /dev/gpio[pin_number].  

### Implement GPIO Operation

```c
struct gpio_ops_s {
	CODE int  (*get)(FAR struct gpio_lowerhalf_s *lower);
	CODE void (*set)(FAR struct gpio_lowerhalf_s *lower, FAR unsigned int value);
	CODE int  (*pull)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
	CODE int  (*setdir)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
	CODE int  (*enable)(FAR struct gpio_lowerhalf_s *lower,	int falling, int rising, gpio_handler_t handler);
	CODE int  (*ioctl)(FAR struct gpio_lowerhalf_s *lower, FAR int cmd,  unsigned long args);
};

CODE int  (*get)(FAR struct gpio_lowerhalf_s *lower);
/*********************************
- return : Current GPIO value
*********************************/

CODE void (*set)(FAR struct gpio_lowerhalf_s *lower, FAR unsigned int value);
/*********************************
- value : GPIO value to be set.
*********************************/

CODE int  (*pull)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
/*********************************
- arg : GPIO_DRIVE_PULLUP / GPIO_DRIVE_PULLDOWN / GPIO_DRIVE_FLOAT
- return : OK / negative value
*********************************/

CODE int  (*setdir)(FAR struct gpio_lowerhalf_s *lower, unsigned long arg);
/*********************************
- arg : GPIO_DIRECTION_NONE / GPIO_DIRECTION_OUT / GPIO_DIRECTION_IN
- return : OK / negative value
*********************************/

CODE int  (*enable)(FAR struct gpio_lowerhalf_s *lower, int falling, int rising, gpio_handler_t handler);
/*********************************
- falling : true / false
- rising : true / false
- gpio_handler_t handler : This handler should be called whenever each GPIO interrupt occurred.
	-> typedef CODE void (*gpio_handler_t)(FAR struct gpio_upperhalf_s *upper);
*********************************/
```

>In [enable] operation, below process must be included.

```c
static int [Interrupt_Function](int irq, FAR void *context, FAR void *arg);
/*******************************
- irq : irqvector number
- contetxt : identifying value.
- arg : user specific pointer
*******************************/
if (handler) {
	irq_attach([irqvector], [Interrupt_Function], (void *)arg);
	up_enable_irq([irqvector]);
} else {
	up_disable_irq([irqvector]);
	irq_detach([irqvector]);
}
```

>Example implementation is present in s5j_gpio_lowerhalf.c under os/arch/arm/src/s5j folder.

## UART
UART has two parts, setting early console and setting serial console driver, serial port drivers.

### Setting of early console
#### Add up_earlyserialinit function
Boot code must call this before os starting, to provide low level debug messages during bootup.

```c
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
```c
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

```c
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
```c
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
```c
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

```c
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

```c
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

```c
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


```c
struct i2c_dev_s {
	const struct i2c_ops_s *ops;	/* I2C vtable */
	FAR void *priv;			/* Used by the arch-specific logic */
};

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

```c
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

I2S port for a board involves the following:  
1. I2S Device Structure and its Operations.  
2. I2S Initialization and Device Access.  

### I2S Device Structure and Operations
Each board, shall;  

Implement and expose an instance of struct i2s_dev_s for each port.  
Implement the supported operations on i2s, ie struct i2s_op_s.  
Data structures below;  

```c
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

```c
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


## PWM

All structure and function prototypes to be implemented by low-level PWM drivers.
PWM drivers are provided in the header file [pwm.h](../os/include/tinyara/pwm.h).


### PWM device driver initialzation

Every PWM channel must implement some function to initialize the PWM device like below.
Please see ```imxrt_pwm_register```.

```c
FAR struct pwm_lowerhalf_s * imxrt_pwm_initialize(int channel)
{
	switch (channel) {
#ifdef CONFIG_IMXRT_PWM1
	case 1:
		lower = &g_ch1dev;
		break;
	...
#endif
#ifdef CONFIG_IMXRT_PWM2
	case 9:
		lower = &g_ch9dev;
		break;
#endif
	...
	if (imxrt_pwm_init(base, submodule, &pwmconfig) == kStatus_Fail) {
		return NULL;
	}
	//...
	imxrt_pwm_setup((FAR struct pwm_lowerhalf_s *)lower);

	return (FAR struct pwm_lowerhalf_s *)lower;
}

static void imxrt_pwm_register(int ch)
{
#ifdef CONFIG_PWM
	struct pwm_lowerhalf_s *pwm;
	char path[16];
	int ret;

	/* The last two channels(15, 16) multiplex the PINs with LPUART1,
	 * so for debugging they are not be set to PWM PINs */
	pwm = imxrt_pwminitialize(ch);
	if (!ppwm) {
		dbg("Failed to get imxrt PWM Channel lower half\n");
		return;
	}

	/* Register the PWM channel driver at "/dev/pwmx" */
	snprintf(path, sizeof(path), "/dev/pwm%d", ch);
	ret = pwm_register(path, pwm);
	if (ret < 0) {
		dbg("Imxrt PWM registrations failure: %d\n", ret);
	}
#endif
	return;
}
```

PWM device drive must create an instance of ```struct pwm_ops_s``` and returned higher level device driver as show above.

Example implementations is present in [imxrt_pwm.c](../os/arch/arm/src/imxrt/imxrt_pwm.c)

And PWM device will control using below 5 functions.

```c
struct pwm_ops_s {
	/* This method is called when the driver is opened. The lower half driver
	 * should configure and initialize the device so that it is ready for use.
	 * It should not, however, output pulses until the start method is called.
	 */

	CODE int (*setup)(FAR struct pwm_lowerhalf_s *dev);

	/* This method is called when the driver is closed. The lower half driver
	 * should stop pulsed output, free any resources, disable the timer hardware, and
	 * put the system into the lowest possible power usage state
	 */

	CODE int (*shutdown)(FAR struct pwm_lowerhalf_s *dev);

	/* (Re-)initialize the timer resources and start the pulsed output. The
	 * start method should return an error if it cannot start the timer with
	 * the given parameter (frequency, duty, or optionally pulse count)
	 */

	CODE int (*start)(FAR struct pwm_lowerhalf_s *dev, FAR const struct pwm_info_s *info);

	/* Stop the pulsed output and reset the timer resources */

	CODE int (*stop)(FAR struct pwm_lowerhalf_s *dev);

	/* Lower-half logic may support platform-specific ioctl commands */

	CODE int (*ioctl)(FAR struct pwm_lowerhalf_s *dev, int cmd, unsigned long arg);
};

/****************************************************************************
 * Name: pwm_setup
 *
 * Description:
 *   This method is called when the driver is opened. The lower half driver
 *   should configure and initialize the device so that it is ready for use.
 *   It should not, however, output pulses until the start method is called.
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int pwm_setup(FAR struct pwm_lowerhalf_s *dev);


struct pwm_info_s {
	uint32_t frequency;			/* Frequency of the pulse train */
	ub16_t duty;				/* Duty of the pulse train, "1"-to-"0" duration.
};

/****************************************************************************
 * Name: pwm_start
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int pwm_start(FAR struct pwm_lowerhalf_s *dev,
						   FAR const struct pwm_info_s *info);

/****************************************************************************
 * Name: pwm_stop
 *
 * Description:
 *   Stop the pulsed output and reset the timer resources
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   This function is called to stop the pulsed output at anytime. This
 *   method is also called from the timer interrupt handler when a repetition
 *   count expires... automatically stopping the timer.
 *
 ****************************************************************************/
static int pwm_stop(FAR struct pwm_lowerhalf_s *dev);
```

>This function makes 'dev' to stop.
But it still has its own resources so that it can be re-started.


```c

/****************************************************************************
 * Name: pwm_shutdown
 *
 * Description:
 *   This method is called when the driver is closed. The lower half driver
 *   stop pulsed output, free any resources, disable the timer hardware, and
 *   put the system into the lowest possible power usage state
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int pwm_shutdown(FAR struct pwm_lowerhalf_s *dev);
```

>Stop the dev, and release every resource. To use this again, setup should be called previously.


```c
/****************************************************************************
 * Name: pwm_ioctl
 *
 * Description:
 *   Lower-half logic may support platform-specific ioctl commands
 *
 * Input parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   cmd - The ioctl command
 *   arg - The argument accompanying the ioctl command
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/
static int pwm_ioctl(FAR struct pwm_lowerhalf_s *dev, int cmd,
						   unsigned long arg);
```

It will be set to file operation structure like below. And then user can control pwm through common file handling operations(functions).

```c
static const struct file_operations pwmops = {
	pwm_open,					/* open */
	pwm_close,					/* close */
	pwm_read,					/* read */
	pwm_write,					/* write */
	0,							/* seek */
	pwm_ioctl					/* ioctl */
#ifndef CONFIG_DISABLE_POLL
	, 0						/* poll */
#endif
};
```
You can see more in [pwm.c](../os/drivers/pwm.c).

And ```struct pwm_ops_s``` hooks to ```struct pwm_upperhalf_s``` through pwm_register function. After this user can access pwm through file operations like  open, read, write and close.