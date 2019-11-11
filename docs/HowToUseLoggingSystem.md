# How to use Logging System
**Debug Messages** is the interface for logging events and **Logging Modules** are modules which process the log messages, LogM and Syslog Devices.

## Contents
- [Debug Messages](#debug-messages)  
- [Logging Modules](#logging-modules)

## Debug Messages
TizenRT uses Debug Messages for logging.  
These messages are processed through [logging modules](#logging-modules).  
It is similar to printf with respect to printing messages. However, it differs from printf in that Debug Messages can be configured by menuconfig.  
Debug Messages can be configured for each module or level. In contrast, printf messages cannot be selectively disabled during buildtime, unless you have disabled it explicitly by a macro definition.

### Adding new Debug Messages
TizenRT defines debug macros for Debug Messages according to the debug level and interface level.  
* Debug level represents the severity of the log message, and is available as a configuration in menuconfig.  
   * three debug levels : Error, Warning, Information.  
* Low-level interface uses architecture-specific I/O which writes something to device directly such as up_putc().  
  It should be used in low-level code only where it is inappropriate to use file descriptors because of disabling interrupt internally.  

Now you have six combinations of debug levels and interface levels to consider, and therefore, you have six macros, which are as follows:

| Level        | Normal Interface | Low-level Interfaces | Config               |
|--------------|------------------|----------------------|----------------------|
| Error        | dbg              | lldbg                | CONFIG_DEBUG_ERROR   |
| Warning      | wdbg             | llwdbg               | CONFIG_DEBUG_WARN    |
| Info(Verbose)| vdbg             | llvdbg               | CONFIG_DEBUG_VERBOSE |

TizenRT's modules can customize the debug macros above by defining suitable functions.  
For examples, filesystem defines fdbg for error output, network defines nwdbg for warning.  
Please refer to definitions in [os/include/debug.h](../os/include/debug.h).

### How to enable debug output
In order to enable debug output in TizenRT, the following steps in menuconfig are necessary.
```
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
1. Enable the debugging Feature.
	```
	Debug Option -> Enable Debug Features to y
	```
2. Enable the debugging levels.
	```
	Enable Error Debug Output
	Enable Warning Debug Output
	Enable Informational(Verbose) Debug Output
	```
Depending on which level you select, the corresponding options will be displayed on the menuconfig.

3. Enable the debugging modules with level. For example, to enable filesystem debug with error level and network debug with error and warning level,
	```
	 1. Select levels, `Enable Error Debug Output` and `Enable Warning Debug output`
	 2. Select sub-level for each modules, `File System Debug Output`, `File System Error Output`,
			   `Network Debug Output`, `Network Error Output` and `Network Warning Output`
	```

## Logging Modules
By default, all log output goes to stdout, i.e. console.  
Using Menuconfig, however, they can be configured to route to other interfaces or devices as well.  

There are three kinds of logging modules, LogM and two Syslog Devices.
 * [LogM](../os/logm/README.md)
 * [Syslog Devices](../os/drivers/syslog/README.txt) : Character device, RAM logging device  

LogM takes stronger precedence over syslog, so if LogM and either Syslog Devices are enabled at the same time, all logs will be directed to LogM.

### How to enable LogM
For details pertaining to LogM, please refer to [README](../os/logm/README.md).

### How to enable Syslog Devices
Using Menuconfig, Syslog Devices can be enabled as shown below:
```
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
1. Enable Syslog Devices as shown below:
	```
	File Systems -> Advanced SYSLOG features to y
	```
2. Enable specific device.
 * character device: useful for logging to specific character devices.
	  ```
	  System log character device to y
	  Set CONFIG_SYSLOG_DEVPATH as full path of device, CONFIG_SYSLOG_DEVPATH=/dev/ttyS1
	  ```
 * RAM logging device: useful when the normal serial output is not available.
	  ```
	  Exit File System menu
	  Device Driver -> RAM log device support to y
	  ```
