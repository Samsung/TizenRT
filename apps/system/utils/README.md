# How to use Shell Commands

This document shows the list of shell coammands like ps, free and so on.

## List of Commands
> [exit](#exit)  
> [help](#help)  
> [reboot](#reboot)  
> [sh](#sh)  
> [sleep](#sleep)  

### exit
This command supports exiting shell.
```
TASH>>exit
TASH: Good bye!!
```

### help
This command shows list of commands.
```
TASH>>help
TASH>>   TASH command list
         --------------------
cat              cd               date             df
dhcpd            exit             free             getenv
heapinfo         hello            help             ifconfig
ifdown           ifup             iotjs            iperf
irqinfo          kill             killall          logm
ls               mkdir            mkrd             mksmartfs
mount            netdb            ping             ps
pwd              ramtest          reboot           rm
rmdir            setenv           sh               sleep
stkmon           tftpc            ttrace           umount
unsetenv         uptime           vi               wifi
```

### reboot
This command supports rebooting a board.
```
TASH>>reboot

U-Boot 2017.01-g59977f7 (Nov 23 2017 - 16:09:49 +0900)

CPU:
Model:
DRAM:
WARNING: Caches not enabled
BL1 released at 2017-3-13 15:00
SSS released at 2017-09-12
WLAN released at 2017-03-21
Flash: 8 MiB
...
System Information:
        Version: 1.1
        Commit Hash: 34d5b12f4c3b506e627ef80e12f0a7fbc6f33f40
        Build User: sunghan@ubuntu
        Build Time: 2018-02-27 09:19:19
        System Time: 01 Jan 2010, 00:00:00 [s] UTC Hardware RTC Support
Hello, World!!
TASH>>
```

Enable *CONFIG_BOARDCTL_RESET* on menuconfig.
```
Hardware Configuration -> Board Selection -> Enable boardctl() interface to y -> Enable reset interface to y
```

### sh
This comamand supports executing shell script.  
There is an argument to give path and name of script file.
```
TASH>>sh /rom/print_hello2_help1_hello1.sh
Command will be launched with pri (100), stack size(4096)
Command will be launched with pri (100), stack size(4096)
         TASH command list
         --------------------
cat              cd               date             df
dhcpd            exit             free             getenv
heapinfo         hello            help             ifconfig
ifdown           ifup             iotjs            iperf
irqinfo          kill             killall          logm
ls               mkdir            mkrd             mksmartfs
mount            netdb            ping             ps
pwd              ramtest          reboot           rm
rmdir            setenv           sh               sleep
stkmon           tftpc            ttrace           umount
unsetenv         uptime           vi               wifi

Command will be launched with pri (100), stack size(4096)
Hello, World!!
Hello, World!!
Hello, World!!
```

Enable *CONFIG_TASH_SCRIPT* to use this command on menuconfig as shown below:
```
Application Configuration -> Shell -> enable shell script to y
```

### sleep
This command supports sleeping of shell. This has an argument which shows sleeping seconds.  
The *CONFIG_DISABLE_SIGNALS* should not be enabled to use this.

### date

### dmesg
### getenv / setenv / unsetenv
### free
### heapinfo
### irqinfo
### kill / killall
### ps
### stkmon
### uptime

### 
