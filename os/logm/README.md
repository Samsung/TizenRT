# LogM
Logging module to route debug messages.  
LogM has own task which runs asynchronously, it queues messages in buffer and writes them out periodically into the stdout.  
The main difference from others is queuing the incoming messages.  
Logm messages are queued and flushed whereas printf/syslog messages would be written immediately to stdout or other devices.  
If there are many log messages, these can be dropped because of buffer overflow. In order to avoid the message loss, please refer to [How to resolve buffer overflow](#how-to-resolve-buffer-overflow).

## How to enable and configure
Enable configuration of LogM with menuconfig.
```
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
Enable LogM feature.
```
Debug Options -> Logger Module -> [*] Logger Module
```
Select options.  
 * printf to logm
 ```
 [*] Route printf through logm
 ```
 * syslog to logm
  ```
 [*] Route all syslogs through logm
 ```
  * add timestamp to message
 ```
 [*] Prepend timestamp to message
 ```

Other Configurations
 * Logm Buffer size  
   > If it is not sufficient, some messages would be dropped.
 * Interval for flushing logm buffer  
   > It decides how frequently buffer is flushed (ms).
 * Logm Task priority  
   > If it is lower than other tasks, logm can not be operated properly.
 * Logm Task stack size

## How to configure LogM in run-time
You can configure logm setting using `logm` command in run-time.
If the buffer overflows, please increase the buffer size or decrease the interval.
1. Check current logm configuration
```
TASH>> logm
```

2. Change values suitable for usage
```
TASH >> logm [-b BUFFERSIZE] [-i TIME]
```
`-b` option is for buffer size, `-i` option is for interval of flushing.

## How to resolve buffer overflow
When the buffer is full, some messages can be dropped until buffer is flushed.  
To avoid the loss of messages, some options should be set carefully for usage.  
There are 2 options, **buffer size** and **interval of flushing buffer**.  
You can configure them in [build-time](#how-to-enable-and configure) or [run-time](#how-to-configure-logm-in-run-time).
1. Buffer size  
The size of buffer used for queuing messages. (default:10240)  
This value decides how many messages can be put at once.

2. Interval for flushing  
The periodic interval at which LogM task flushes the buffer. (default : 1000ms)  
This value decides how frequently buffer is flushed.
