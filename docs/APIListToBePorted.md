# API List to be Ported

## Contents
- [API List](#api-list)
- [API Description](#api-description)

## API List
1. up_flashinitialize()
2. up_idle()
3. up_enable_irq()
4. up_netinitialize()
5. up_serialinit()
6. up_earlyserialinit()
7. up_putc()
8. up_getc()
9. up_puts()
10. up_lowputc()
11. up_timer_initialize()
12. up_wdog_init()
13. up_wdog_keepalive()
14. up_watchdog_disable()
15. up_i2cinitialize()
16. up_spiinitialize()
17. up_rtc_initialize()
18. up_reboot_reason_init()
19. up_reboot_reason_read()
20. up_reboot_reason_write()
21. up_reboot_reason_clear()

## API Description

- Add low level chip specific API **up_flashinitialize()** to create an initialize MTD device instance. MTD devices are not registered in the file system, but are created as instances that can be bound to other functions (such as a block or character driver front end).
```c
 * Prototype: struct mtd_dev_s *up_flashinitialize(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   Valid MTD device structure reference on success; a NULL on failure
```
*File Location:*	"os/arch/arm/src/chip/chip_flash.c"

*Prototype Location:*	"os/include/tinyara/fs/mtd.h"

- Add low level chip specific API **up_idle()** to be executed when their is no other ready-to-run task. This is processor idle time and will continue until some interrupt occurs to cause a context switch from the idle task. Processing in this state may be processor-specific.
For example, this is where power management operations might be performed.
```c
 * Prototype: void up_idle(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
To this we can further add **up_idlepm()** to perform IDLE state power management.
```c
static void up_idlepm(void)
```
*File Location:*	"os/arch/arm/src/chip/chip_idle.c"

*Prototype Location:*	"os/include/tinyara/arch.h"

- Add low level chip specific API **up_enable_irq()** to enable the IRQ specified by 'irq'
```c
 * Prototype: void up_enable_irq(int irq)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_irq.c"

*Prototype Location:*	"os/include/tinyara/arch.h"

- Add low level chip specific API **up_netinitialize()** to initialize network domain
[TO DO]
```c
 * Prototype: void up_netinitialize(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_enet.c"

*Prototype Location:*	"os/arch/arm/src/common/up_internal.h"

- Add low level chip specific API's to initialize and register serial console and ports:

a. **up_serialinit()** : Register serial console and serial ports.
```c
 * Prototype: void up_serialinit(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
b. **up_earlyserialinit()** : Performs the low level UART initialization early in debug so that
the serial console will be available during bootup.  This must be called before up_serialinit.
```c
 * Prototype: void up_earlyserialinit(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_serial.c"

*Prototype Location:*	""

- Add low level chip specific API's to receive and transfer characters through UART:

a. **up_putc()** : Output one byte on the serial console
```c
 * Prototype: int up_putc(int ch)
 * Input Parameters:
 *   ch - chatacter to output
 * Returned Value:
 *   sent character
```
b. **up_lowputc()** : Output one byte on the serial console.
```c
 * Prototype: void up_lowputc(void)
 * Input Parameters:
 *   character to output
 * Returned Value:
 *   none
```
c. **up_getc()** : Read one byte from the serial console
```c
 * Prototype: int up_getc(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   int value, -1 if error, 0~255 if byte successfully read
```
d. **up_puts()** : Output string on the serial console
```c
 * Prototype: void up_puts(const char *str)
 * Input Parameters:
 *   str - string to output
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_serial.c"

- Add low level chip specific API **up_timer_initialize()** called during start-up to initialize the timer interrupt.
```c
 * Prototype: void up_timer_initialize(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_timerisr.c"

*Prototype Location:*	"os/include/tinyara/arch.h"

- Add low level chip specific API's to enable, disable and initialize watchdog.

a. **up_wdog_init()** : Initialize the watchdog for irq
This is added only if CONFIG_WATCHDOG_FOR_IRQ is enabled.
```c
 * Prototype: void up_wdog_init(uint16_t timeout)
 * Input Parameters:
 *   timeout - timeout for watchdog to expire
 * Returned Value:
 *   none
```
b. **up_wdog_keepalive()** : Reset the watchdog timer for preventing timeouts.
```c
 * Prototype: void up_wdog_keepalive(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
c. **up_watchdog_disable()** : Disable board specific watchdog
```c
 * Prototype: void up_watchdog_disable(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
 * Cautions:
 *   This can be only used if we cannot use driver structure like assert.
```
*File Location:*	"os/arch/arm/src/chip/chip_wdog.c"

*Prototype Location:*	"os/include/tinyara/arch.h"

- Add low level chip specific API **up_i2cinitialize()** to initialize one I2C bus. 
```c
 * Prototype: struct i2c_dev_s *up_i2cinitialize(int port)
 * Input Parameters:
 *   Port number (for hardware that has multiple I2C interfaces)
 * Returned Value:
 *   Valid I2C device structure reference on success; a NULL on failure
```
*File Location:*	"os/arch/arm/src/chip/chip_i2c.c"

- Add low level chip specific API **up_spiinitialize()** to initialize the selected SPI bus.
```c
 * Prototype: struct spi_dev_s *up_spiinitialize(int port)
 * Input Parameters:
 *   Port number (for hardware that has multiple SPI interfaces)
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
```
*File Location:*	"os/arch/arm/src/chip/chip_spi.c"

- Add low level chip specific API **up_rtc_initialize()** to initialize the hardware RTC per the selected configuration.
```c
 * Prototype: int up_rtc_initialize(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
```
*File Location:*	"os/arch/arm/src/chip/chip_rtc.c"

*Prototype Location:*	"os/include/tinyara/arch.h"

- Add low level chip specific API **up_lowputc()** to send one byte on the serial console.
```c
 * Prototype: void up_lowputc(void)
 * Input Parameters:
 *   character to output
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_serial.c"

*Prototype Location:*	"os/arch/arm/src/common/up_internal.h"

- Add low level chip specific API **up_reboot_reason_init** to initialize the reboot reason register.
```c
 * Prototype: void up_reboot_reason_init(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_reboot_reason.c"

*Prototype Location:*	"os/arch/arm/include/reboot_reason.h"

- Add low level chip specific API **up_reboot_reason_read** to read the reboot reason from register.
```c
 * Prototype: void up_reboot_reason_read(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   reboot reason code
```
*File Location:*	"os/arch/arm/src/chip/chip_reboot_reason.c"

*Prototype Location:*	"os/arch/arm/include/reboot_reason.h"

- Add low level chip specific API **up_reboot_reason_write** to write the reboot reason to register
```c
 * Prototype: void up_reboot_reason_write(void)
 * Input Parameters:
 *   reboot reason code
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_reboot_reason.c"

*Prototype Location:*	"os/arch/arm/include/reboot_reason.h"

- Add low level chip specific API **up_reboot_reason_clear** to clear the reboot reason register.
```c
 * Prototype: void up_reboot_reason_clear(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   none
```
*File Location:*	"os/arch/arm/src/chip/chip_reboot_reason.c"

*Prototype Location:*	"os/arch/arm/include/reboot_reason.h"
