# How to use Shell Commands

This document shows the list of shell commands like ps, free and so on.  
These commands can be used in shell, and can be used without shell also like remote monitoring functionality.  
*Dependency* should be enabled first for using each commands.  
Most of the commands support *--help* option to show how to use.

## List of Commands
| Basic commands | Kernel Commands       | FileSystem Commands |
|----------------|-----------------------|---------------------|
| [exit](#exit)  | [date](#date)         | [cat](#cat)         |
| [help](#help)  | [dmesg](#dmesg)       | [cd](#cd)           |
| [sh](#sh)      | [free](#free)         | [df](#df)           |
| [sleep](#sleep)| [getenv/setenv/unsetenv](#getenvsetenvunsetenv) | [echo](#echo) |
|                | [heapinfo](#heapinfo) | [ls](#ls)               |
|                | [irqinfo](#irqinfo)   | [mkdir](#mkdir)         |
|                | [kill/killall](#killkillall) | [mkrd](#mkrd)    |
|                | [ps](#ps)             | [mksmartfs](#mksmartfs) |
|                | [reboot](#reboot)     | [mount](#mount)         |
|                | [stkmon](#stkmon)     | [pwd](#pwd)             |
|                | [uptime](#uptime)     | [rm](#rm)               |
|                |                       | [rmdir](#rmdir)         |
|                |                       | [umount](#umount)       |


## exit
This command supports exiting shell.
```bash
TASH>>exit
TASH: Good bye!!
```


## help
This command shows list of commands.
```bash
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


## cat
This command prints the contents of an existing file to standard output or writes it to the target file.  

```
Usage:
   cat [OPTIONS] [source_file_path] [> or >>] [target_file_path]  
   OPTIONS: `--help` - display the usage.  
```
```bash
TASH>>echo -n "abc" > /mnt/test.txt
TASH>>echo -n "123" > /mnt/test1.txt
TASH>>ls /mnt
/mnt:
test.txt
test1.txt
TASH>>cat /mnt/test.txt
abc
TASH>>cat /mnt/test1.txt
123
TASH>>cat /mnt/test1.txt >> /mnt/test.txt
TASH>>cat /mnt/test.txt
abc123
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## cd
This changes current working directory to given path.
```
Usage:
   cd <directory or - or .. or ~>
```
```bash
TASH>>ls
/:
 dev/
 mnt/
 proc/
 ramfs/
 rom/
TASH>>cd rom
TASH>>ls
/rom:
 .
 print_hello2_help1_hello1.sh
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## date
This command prints the date. This has an argument which sets system time and date information.
```bash
TASH>>date --help

Usage: date
   or: date [-s FORMAT]
Display, or Set system time and date information

Options:
 -s FORMAT     Set system time in the given FORMAT
               FORMAT: MMM DD HH:MM:SS YYYY
                'month', 'day', 'hour':'minute':'second' 'year'
                Example: Apr 21 10:35:22 1991

TASH>>date
Jan 01 00:00:09 2010

TASH>>date -s Feb 27 14:41:05 2018
TASH>>date
Feb 27 14:41:06 2018
```
### How to Enable
Enable *CONFIG_ENABLE_DATE* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] date
```


## df
This shows information about the File System on which each FILE resides, or all File Systems by default.
```bash
TASH>>df -h
Filesystem    Size      Used  Available Mounted on
smartfs      1000K     7680B       992K /mnt
procfs          0B        0B         0B /proc
smartfs       256K        7K       249K /ramfs
romfs         256B      256B         0B /rom

TASH>>df
  Block  Number
  Size   Blocks     Used Available Mounted on
   512     2000       15      1985 /mnt
     0        0        0         0 /proc
   512      512       14       498 /ramfs
   256        1        1         0 /rom
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_MOUNTPOINT.
```
File Systems -> [ ] Disable support for mount points
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## dmesg
This command shows all debug messages which saved in RAM log buffer.
```bash
TASH>>dmesg
s5j_sflash_init: FLASH Quad Enabled
uart_register: Registering /dev/console
uart_register: Registering /dev/ttyS0
uart_register: Registering /dev/ttyS1
uart_register: Registering /dev/ttyS2
uart_register: Registering /dev/ttyS3
uart_register: Registering /dev/ttyS4
i2c_uioregister: Registering /dev/i2c-0
i2c_uioregister: Registering /dev/i2c-1
```
### How to Enable
Enable *CONFIG_ENABLE_DMESG* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] dmesg
```
#### Dependency
- Enable CONFIG_SYSLOG.
```
File System -> [*] Advanced SYSLOG features
```
- Enable CONFIG_RAMLOG_SYSLOG.
```
Device Drivers -> [*] RAM log device support
```

## echo
This command prints the 'input_text' contents to standard output or writes/appends it to target file.  

```
Usage:
   echo [OPTIONS] [input_text] [> or >>] [target_file_path]  
   OPTIONS: `-n` - do not output the trailing newline.  
            `--help` - displays usage.  
```
```bash
TASH>>ls /mnt
/mnt:
TASH>>echo "abc" > /mnt/test.txt
TASH>>cat /mnt/test.txt
abc

TASH>>echo -n "abc" > /mnt/test.txt
TASH>>cat /mnt/test.txt
abc
TASH>>echo -n "123" >> /mnt/test.txt
TASH>>cat /mnt/test.txt
abc123
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task


## free
This command shows heap information.
```bash
TASH>>free
              total       used       free    largest
Data:        959312      44144     915168     909296
```
### How to Enable
Enable *CONFIG_ENABLE_FREE* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] free
```


## getenv/setenv/unsetenv
These commands are related with setting or getting the environment variables.  
The **getenv** prints all of environment variables. If specific environment name is given, this prints the value of given environment.   
The **setenv** save new environment variable with given value.  
The **unsetenv** unsets environment variable with NAME.
```bash
TASH>>getenv --help

Usage : getenv [NAME]
Display the value of a variable, NAME or all environment variables
If NAME is not specified, 'name=value' pairs of all environment variables will be displayed

TASH>>setenv --help
Usage : setenv NAME VALUE

TASH>>unsetenv --help

Usage : unsetenv NAME
Remove the variable, NAME from the environment

TASH>>setenv TEST1 1
env "TEST1=1" is SET
TASH>>setenv TEST2 2
env "TEST2=2" is SET
TASH>>setenv TEST3 3
env "TEST3=3" is SET
TASH>>getenv
TEST1=1
TEST2=2
TEST3=3
TASH>>getenv TEST2
TEST2=2
TASH>>unsetenv TEST2
env "TEST2" is UNSET
TASH>>getenv TEST2
GET is failed, err (2)

```
### How to Enable
Enable *CONFIG_ENABLE_ENV_GET/SET/UNSET* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] env get / [*] env set / [*]env unset
```
#### Dependency
Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```


## heapinfo
This command shows heap memory usages per thread. This has arguments which configure the printing information.
```bash
TASH>>heapinfo --help

Usage: heapinfo [OPTIONS]
Display information of heap memory

Options:
 -i           Initialize the heapinfo
 -a           Show the all allocation details
 -p PID       Show the specific PID allocation details
 -f           Show the free list
 -g           Show the User defined group allocation details
              (for -g option, CONFIG_HEAPINFO_GROUP is needed)
 -e HEAP_IDX  Show the heap[HEAP_IDX] allocation details
              (-e option is available when CONFIG_MM_NHEAPS is greater than 1)
 -r           Show the all region information
              (-r option is available when CONFIG_MM_REGIONS is greater than 1)
 -k OPTION    Show the kernel heap memory allocation details based on above options

TASH>>heapinfo

****************************************************************
     Summary of Heap Usages (Size in Bytes)
****************************************************************
Total                           : 960320 (100%)
  - Allocated (Current / Peak)  : 44256 (4%) / 48624 (5%)
  - Free (Current)              : 916032 (95%)
  - Reserved                    : 32

****************************************************************
     Details of Heap Usages (Size in Bytes)
****************************************************************
< Free >
  - Number of Free Node               : 2
  - Largest Free Node Size            : 911840

< Allocation >
  - Current Size (Alive Allocation) = (1) + (2) + (3)
     . by Dead Threads (*) (1)        : 2272
     . by Alive Threads
        - Sum of "STACK"(**) (2)      : 6176
        - Sum of "CURR_HEAP" (3)      : 35808
** NOTE **
(*)  Alive allocation by dead threads might be used by others or might be a leakage.
(**) Only Idle task has a separate stack region,
  rest are all allocated on the heap region.

< by Alive Threads >
PID |  PPID | STACK | CURR_HEAP | PEAK_HEAP | NAME
----|-------|-------|-----------|-----------|----------
  0 |     0 |  1024 |     35664 |     40000 | Idle Task()
  1 |     0 |  2064 |         0 |         0 | hpwork()
  3 |     2 |  4112 |       144 |       176 | tash()
```

#### Term
- CURR_HEAP : The current allocated heap size until running this command(bytes)  
- PEAK_HEAP : The peak allocated heap size until running this command(bytes)  
- MemAddr   : The address of allocation  
- Owner     : The holder who allocate the memory  

### How to Enable
Enable *CONFIG_ENABLE_HEAPINFO_CMD* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] heapinfo
```

#### Dependency
Enable CONFIG_DEBUG_MM_HEAPINFO.
```
Debug options -> [*] Enable Debug Output Features -> [*] Heap Info debug option
```

### Heapinfo for User defined Group
With specified task/thread name, heapinfo can show User defined group heap memory usage.

Enable *CONFIG_HEAPINFO_GROUP* to use this option.
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] heapinfo -> [*] Enable User defined Group Memory Usage -> set Thread Name List for measuring Alloc size
```

- Thread Name List for measuring Alloc size can be set like below:  
  Task/Thread name is separated with ',' and Group is separated with '/'.  
  For example, "abc,def/ghi,jklmn/opqr".

```
****************************************************************
Heap Allocation Information per User defined Group
****************************************************************
 PEAK | HEAP_ON_PEAK | STACK_ON_PEAK | THREADS_IN_GROUP
----------------------------------------------------------------
 4572 |         3568 |          1004 | abc,def
 5772 |         4768 |          1004 | ghi,jklmn
 2940 |          896 |          2044 | opqr
```



## irqinfo
This command shows the number of registered interrupts, it's occurrence counts and corresponding isr.
```bash
TASH>>irqinfo
 INDEX | IRQ_NUM | INT_COUNT | ISR_NAME
-------|---------|-----------|----------
     1 |      36 |         0 | gic_interrupt
     2 |      37 |         0 | gic_interrupt
     3 |      58 |       228 | up_timerisr
     4 |      90 |        36 | up_interrupt
```
### How to Enable
Enable *CONFIG_ENABLE_IRQINFO_CMD* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] irqinfo
```
#### Dependency
- Enable CONFIG_DEBUG.
```
Debug Options -> [*] Enable Debug Output Features
```
- Enable CONFIG_DEBUG_IRQ_INFO.
```
Debug Options -> Enable Debug Output Features -> [*] Interrupt Debug information
```


## kill/killall
These commands send a signal to specific or all threads.  
kill sends a signal to specific thread.  
killall sends a signal to all threads which have a same name.
```bash
TASH>>kill --help

Usage: kill [-SIGNAME|SIGNUM] PID
   or: kill -l
Send a signal SIGNAME or SIGNUM to a process PID
If no signal is specified, SIGKILL is sent because we don't support SIGTERM
* Caution: SIGKILL terminates task/thread without any operations

Options:
 -l             List all signal names

TASH>>killall --help

Usage: killall [-SIGNAME|SIGNUM] NAME
   or: killall -l
Send a signal specified by SIGNAME or SIGNUM to a processes named NAME
If no signal is specified, SIGKILL is sent because we don't support SIGTERM
* Caution: SIGKILL terminates task/thread without any operations

Options:
 -l           List all signal names
```
### How to Enable
Enable *CONFIG_ENABLE_KILL/KILLALL* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] kill/killall
```
#### Dependency
- Disable CONFIG_DISABLE_SIGNALS.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable signal support
```
- The *CONFIG_TASK_NAME_SIZE* should be greater than 0 to use killall.
```
Kernel Features -> Tasks and Scheduling -> Maximum task name size to greater than 0
```


## ls
This lists information about the FILEs (the current directory by default).
```
Usage:
   ls [-lRs] <directory>
```
```bash
TASH>>ls
/:
 dev/
 mnt/
 proc/
 ramfs/
 rom/
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## mkdir
This creates the DIRECTORY(ies), if they do not already exist.
```
Usage:
   mkdir [directory name]
```
```bash
TASH>>ls
/mnt:
TASH>>mkdir test
TASH>>ls
/mnt:
 test/
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## mkrd
This makes RAM or ROM.
```
Usage:
   mkrd [-m <minor>] [-s <sector-size>] <nsectors> or mkrd <nsectors>
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_MOUNTPOINT.
```
File Systems -> [ ] Disable support for mount points
```
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## mksmartfs
This makes SmartFS File System on the specified block device.
```
Usage:
   mksmartfs <source directory> [-f] <target directory>
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_MOUNTPOINT.
```
File Systems -> [ ] Disable support for mount points
```
- Enable CONFIG_FS_SMARTFS.
```
File Systems -> [*] SMART File System
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## mount
This mounts specific File System.
```
Usage:
   mount -t <filesystem name> <source directory> <target directory>
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_MOUNTPOINT.
```
File Systems -> [ ] Disable support for mount points
```
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## ps
This command shows information about a selection of the active threads.
```
TASH>>ps

  PID | PRIO | FLAG |  TYPE   | NP |  STATUS  | NAME
------|------|------|---------|----|----------|----------
    0 |    0 | FIFO | KTHREAD |    | READY    | Idle Task
    1 |  224 | RR   | KTHREAD |    | WAITSIG  | hpwork
    3 |  125 | RR   | TASK    |    | RUNNING  | tash
```

#### Term
- FLAG : The policy of scheduling for each task/thread.  
- TYPE : The type of task/thread. It can be KTHREAD(kernel thread), PTHREAD(user pthread) and TASK.  
- NP : The flag of cancelable.  

### How to Enable
Enable *CONFIG_ENABLE_PS* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] ps
```


## pwd
This shows current working directory.
```bash
TASH>>pwd
         /mnt
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## reboot
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

### How to Enable
Enable *CONFIG_BOARDCTL_RESET* on menuconfig.
```
Hardware Configuration -> Board Selection -> [*] Enable boardctl() interface -> [*] Enable reset interface
```


## rm
This unlinks target file.
```
Usage:
   rm [file path]
```
```bash
TASH>>ls
/mnt:
 test.txt
TASH>>rm test.txt
TASH>>ls
/mnt:
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## rmdir
This unlinks target directory.
```
Usage:
   rmdir [directory path]
```
```bash
TASH>>ls
/mnt:
 test/
TASH>>rmdir test
TASH>>ls
/mnt:
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## sh
This command supports executing shell script.  
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

### How to Enable
Enable *CONFIG_TASH_SCRIPT* to use this command on menuconfig as shown below:
```
Application Configuration -> Shell -> [*] Enable shell -> [*] enable shell script
```


## sleep
This command supports sleeping of shell. This has an argument which shows sleeping seconds.  
#### Dependency
Disable CONFIG_DISABLE_SIGNALS.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable signal support
```


## stkmon
This command shows stack information per thread periodically. This has an argument which stops the stack monitor daemon.
```
TASH>>stkmon --help

Usage: stkmon
   or: stkmon stop
Start, or Stop stack monitor daemon

TASH>>stkmon
TASH>>Stack Monitor: Running

=============================================================================
  PID |   STATUS |     SIZE | PEAK_STACK |  PEAK_HEAP |    TIME | THREAD NAME
------|----------|----------|------------|------------|---------|------------
    2 | INACTIVE |     2028 |        452 |      14496 |     375 | appinit
    4 | INACTIVE |     2028 |        308 |          0 |     375 | appmain
------|----------|----------|------------|------------|---------|------------
    0 |   ACTIVE |     1024 |        548 |      43952 |  153570 | Idle Task
    1 |   ACTIVE |     2028 |        164 |          0 |  153571 | hpwork
    3 |   ACTIVE |     4076 |        876 |       3680 |  153571 | tash
    5 |   ACTIVE |     1020 |        516 |          0 |  153571 | StackMonitor

TASH>>stkmon stop
Stack Monitor: Stopping, not stopped yet
TASH>>Stack Monitor: Stopped well
```

#### Term
- STATUS : The status of each task/thread. *INACTIVE* is for finished task/thread. *ACTIVE* is living task/thread.  
- PEAK_STACK : The peak used stack size of each task/thread when run this command.  
- PEAK_HEAP : The peak allocated heap siz of each task/thread when run this command.  

### How to Enable
Enable *CONFIG_ENABLE_STACKMONITOR* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] Stack monitor
```

#### Dependency
Enable CONFIG_STACK_COLORATION.
```
Debug Options -> [*] Stack coloration
```


## umount
This unmounts specific File System.
```
Usage:
   umount <mounted directory>
```
### How to Enable
Enable *CONFIG_FS_CMDS* on menuconfig.
```
Application Configuration -> System Libraries and Add-Ons -> [*] FS shell commands
```
#### Dependancy
- Disable CONFIG_DISABLE_MOUNTPOINT.
```
File Systems -> [ ] Disable support for mount points
```
- Disable CONFIG_DISABLE_ENVIRON.
```
Kernel Features -> Disable TinyAra interfaces -> [ ] Disable environment variable support
```
- Set a value which is greater than zero on CONFIG_NFILE_DESCRIPTORS.
```
Kernel Features -> Files and I/O -> Maximum number of file descriptors per task
```


## uptime
This command shows how long the system has been running.
```
TASH>>uptime
Uptime : 1502.82
```
### How to Enable
Enable *CONFIG_ENABLE_STACKMONITOR* to use this command on menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Kernel shell commands -> [*] uptime
``` 
