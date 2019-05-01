# hello
This prints "hello world" like minimal. But it has more functionality on kernel and file system.  
If network functionality is not mandatory, it can be used.  

Because of enabled many configs and big number of arrays compared with minimal,  
the [minimal](../minimal/README.md) config is better to know minimal the TizenRT kernel footprint.

## Enabled Feature
### Kernel
GPIO, I2C, SFLASH, SPI, UART  
Automount of USERFS  
POSIX APIs  
Round Robin Scheduling  
Signal  
Work Queue  

### File System
Smart FS  
Proc FS  
MTD

### ETC
Debug Message  
TASH  
Date, env, kill, ps, stack monitor, uptime, ramtest, cle, vi
