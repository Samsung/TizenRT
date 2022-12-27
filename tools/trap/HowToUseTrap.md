# How to Use TRAP (TizenRT dump and Abort Parser) Tool
Here we explain how to enable build support for DUMP upload,
and how to actually upload and parse it in the event of a crash during runtime.

## Contents
> [Prerequisites](#prerequisites)  
> [Build Steps](#how-to-enable-memory-dumps)  
> [Parsing Steps](#how-to-parse-RAMDUMP)  
> [Upload Steps](#how-to-upload-RAMDUMP-or-UserfsDUMP)  
> [Porting Guide](#how-to-port-memory-dump-functionality)  

## Prerequisites
Install Python 3.7 or above.

## How to enable memory dumps
Below configuration must be enabled to support dump upload
1. Enable crashdump support (CONFIG_BOARD_CRASHDUMP=y)
```
Hardware Configuration > Board Selection -> Enable Board level logging of crash dumps to y
```
2. Enable board dumps (CONFIG_BOARD_DUMP_UART=y)
```
Hardware Configuration > Board Selection -> Enable dumping of contents via UART to y
```
3. Enable Stack Dump (CONFIG_ARCH_STACKDUMP=y)
```
Hardware Configuration > Chip Selection -> Dump stack on assertions to y
```
4. Enable Debug (CONFIG_DEBUG=y)
```
Debug Options -> Enable Debug Output Features to y
```
5. Enable frame pointer for generating stack frame (CONFIG_FRAME_POINTER=y)
```
Debug Options -> Enable backtracking using Frame pointer register  to y
```
```
NOTE: - For devices that use ARM Cortex M, backtracking of frame pointer is not supported.
Hence, it is not possible to obtain exact call stack for the crash.
Ramdump may not produce results as expected in such cases.
```
6. Enable CONFIG_BOARD_ASSERT_AUTORESET to enable resetting of TARGET device after extracting dumps
```
Hardware Configuration > Board Selection -> Reset a board on assert status automatically
```
7. Enable CONFIG_BCH to enable Block-to-Character driver support for extracting External Userfs Partition Dump
```
Device Drivers -> Block-to-character(BCH) Support
```

## How to parse RAMDUMP
TRAP Script provides two interfaces:  
1. [CUI](#trap-using-cui)  
2. [GUI](#trap-using-gui)  

>**Note**
>To get *Debug Symbols/Crash point using assert logs* ([CUI](#trap-using-cui)), enabling *memory dumps* ([Build Steps](#how-to-enable-memory-dumps)) is not mandatory.

### TRAP using CUI

#### To display Debug Symbols/Crash point using default build binary
1. Change the directory to trap
```
cd $TIZENRT_BASEDIR/tools/trap/
```
2. Copy crash logs  
First copy the entire crash logs to a file in tools/trap/`<log_file>`  
The crash log format should be something like below:
```
up_assert: Assertion failed at file:armv8-m/up_memfault.c line: 156 task: app2
up_dumpstate: sp:     1002cd0c
up_dumpstate: IRQ stack:
up_dumpstate:   base: 1002cea0
up_dumpstate:   size: 00000400
up_dumpstate:   used: 00000278
up_stackdump: 1002cd00: xxxxxxxx xxxxxxxx xxxxxxxx 0000000b 0000000e ffffffbc 01000213 1002cdf8
.
.
.
mpu_show_regioninfo:        7            2200DC0          12C500               1               1               1
mpu_show_regioninfo: *****************************************************************************
up_assert: Checking kernel heap for corruption...
up_assert: No kernel heap corruption detected
up_assert: Checking current app heap for corruption...
up_assert: No app heap corruption detected
elf_show_all_bin_section_addr: [common] Text Addr : 0x2100020, Text Size : 672480
elf_show_all_bin_section_addr: [app] Text Addr : 0x232d2c0, Text Size : 179616
up_assert: Assert location (PC) : 0x02100025
```
3. In the scenario where log file format is not supported, you will see a prompt as below:
```
test@VirtualBox ~/tizenRTGH/tools/trap (master) $sudo python3 ramdumpParser.py -t logs

	- Below log format is not supported in TRAP
		-|19:27:38.36| up_assert: Assertion failed at file:armv8-m/up_memfault.c line: 156 task: main_task
	- Instead, supported log format in TRAP is as follows:
		-up_assert: Assertion failed at file:armv8-m/up_memfault.c line: 156 task: main_task

	Kindly modify the log file as per accepted format.

```
Modify the log file to a supported format as shown in point 2.

4. Run Ramdump Parser Script and see the Output  
    $ python3 ramdumpParser.py -t `<Log file path>`

ex)
$ python3 ramdumpParser.py -t ./log_file

Example Call Stack Output for App crash is as follows:
```
test@VirtualBox ~/tizenRTGH/tools/trap (master) $python3 ramdumpParser.py -t wificrash


*************************************************************
dump_file                   : None
log_file                    : wificrash
Number of binaries          : 3 [kernel + apps]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
"app1" elf_file             : ../../build/output/bin/app1_dbg 
"app2" elf_file             : ../../build/output/bin/app2_dbg 
*************************************************************

-----------------------------------------------------------------------------------------

1. Crash Binary           : app2

2. Crash point (PC or LR)

	[ Caller - return address (LR) - of the function which has caused the crash ]
	- symbol addr        : 0x0000166d
	- function name      : task_startup
	- file               : /root/tizenrt/lib/libc/sched/task_startup.c:123

	[ Current location (PC) of assert ]
	- symbol addr        : 0x00000c52
	- function name      : main
	- file               : /root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:67

	[ Exact crash point might be -4 or -8 bytes from the PC ]
	- symbol addr of (pc - 4)       : 0x00000c4e
	- function name of (pc - 4)     : __aeabi_d2uiz
	- file of (pc - 4)              : lib_rawoutstream.c:?
	- symbol addr of (pc - 8)       : 0x00000c4a
	- function name of (pc - 8)     : __aeabi_d2uiz
	- file of (pc - 8)              : lib_rawoutstream.c:?

3. Call stack of last run thread using Stack dump

	- Current stack pointer:	 0x21812f8

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
0x2181308	 0xe0060b8	 kernel  	  lowvsyslog	/root/tizenrt/os/include/syslog.h:263
0x2181310	 0xe00681c	 kernel  	  lowoutstream_putc	/root/tizenrt/lib/libc/stdio/lib_lowoutstream.c:76
0x2181314	 0xe005eb4	 kernel  	  lib_noflush	/root/tizenrt/os/include/tinyara/streams.h:605
0x2181340	 0xe0085b0	 kernel  	  up_memfault	/root/tizenrt/os/arch/arm/src/armv8-m/up_memfault.c:117
0x2181360	 0x100123b4	 kernel  	  up_doirq	/root/tizenrt/os/arch/arm/src/armv8-m/up_doirq.c:91
0x2181368	 0xc51   	 app2    	  main	/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:57
0x2181378	 0x10012094	 kernel  	  exception_common	/root/tizenrt/os/arch/arm/src/armv8-m/up_exception.S:156
0x2181384	 0xc51   	 app2    	  main	/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:57
0x21813c0	 0x1655  	 app2    	  task_startup	/root/tizenrt/os/include/tinyara/userspace.h:127
0x21813c4	 0xc51   	 app2    	  main	/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:57
0x21813d0	 0xe001c2c	 kernel  	  task_start	/root/tizenrt/os/kernel/task/task_start.c:133

4. Miscellaneous information:
-----------------------------------------------------------------------------------------
```
ex)
$ python3 ramdumpParser.py -t ./logs

Example Call Stack Output for Kernel crash is as follows:
```
*************************************************************
dump_file                   : None
log_file                    : kernelassert
Number of binary            : 1 [kernel]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
*************************************************************

-----------------------------------------------------------------------------------------
1. Crash Binary             : kernel

2. Crash point (PC or LR)

	[ Caller - return address (LR) - of the function which has caused the crash ]
	- symbol addr        : 0x0e001c1b
	- function name      : setbasepri
	- file               : /root/tizenrt/os/include/arch/armv8-m/irq.h:247

	[ Current location (PC) of assert ]
	- symbol addr        : 0x10010f5e
	- function name      : os_start
	- file               : /root/tizenrt/os/kernel/init/os_start.c:609

	[ Exact crash point might be -4 or -8 bytes from the PC ]
	- symbol addr of (pc - 4)       : 0x10010f5a
	- function name of (pc - 4)     : os_start
	- file of (pc - 4)              : /root/tizenrt/os/kernel/init/os_start.c:609
	- symbol addr of (pc - 8)       : 0x10010f56
	- function name of (pc - 8)     : os_start
	- file of (pc - 8)              : /root/tizenrt/os/kernel/init/os_start.c:607 (discriminator 1)

3. Call stack of last run thread using Stack dump

	- Current stack pointer:	 0x10004f20

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
0x10004f30	 0xe0060b8	 kernel  	  lowvsyslog	/root/tizenrt/os/include/syslog.h:263
0x10004f38	 0xe0067bc	 kernel  	  __ultoa_invert	/root/tizenrt/lib/libc/stdio/lib_ultoa_invert.c:64
0x10004f3c	 0xe005eb4	 kernel  	  lib_noflush	/root/tizenrt/os/include/tinyara/streams.h:605
0x10004f68	 0xe008750	 kernel  	  up_usagefault	/root/tizenrt/os/arch/arm/src/armv8-m/up_usagefault.c:84
0x10004f78	 0x100123b4	 kernel  	  up_doirq	/root/tizenrt/os/arch/arm/src/armv8-m/up_doirq.c:91
0x10004f90	 0x10012094	 kernel  	  exception_common	/root/tizenrt/os/arch/arm/src/armv8-m/up_exception.S:156
0x10004fd4	 0x10012670	 kernel  	  up_unblock_task	/root/tizenrt/os/include/tinyara/arch.h:472
0x10004fd8	 0xe001bcc	 kernel  	  task_activate	/root/tizenrt/os/include/sched.h:141
0x10004fdc	 0x10010db8	 kernel  	  os_start	/root/tizenrt/os/include/tinyara/init.h:89

4. Miscellaneous information:
-----------------------------------------------------------------------------------------
```
ex)
python3 ramdumpParser.py -t ./logfile

Example Call Stack Output for Common Binary crash is as follows:
```
*************************************************************
dump_file                   : None
log_file                    : ./log_file
Number of binaries          : 3 [kernel + apps]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
"app1" elf_file             : ../../build/output/bin/common_dbg 
"app2" elf_file             : ../../build/output/bin/app_dbg 
*************************************************************

-----------------------------------------------------------------------------------------

1. Crash Binary           : common

2. Crash point (PC or LR)

	[ Current location (PC) of assert ]
	- symbol addr        : 0x0003d7b3
	- function name      : sched_get_priority_max
	- file               : /root/product/.tizenrt/lib/libc/sched/sched_getprioritymax.c:110

	[ Exact crash point might be -4 or -8 bytes from the PC ]
	- symbol addr of (pc - 4)       : 0x0003d7af
	- function name of (pc - 4)     : sched_get_priority_max
	- file of (pc - 4)              : /root/product/.tizenrt/lib/libc/sched/sched_getprioritymax.c:109
	- symbol addr of (pc - 8)       : 0x0003d7ab
	- function name of (pc - 8)     : sched_get_priority_max
	- file of (pc - 8)              : /root/product/.tizenrt/lib/libc/sched/sched_getprioritymax.c:109

3. Call stack of last run thread using Stack dump

	- Current stack pointer:	 0x220cf38

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
0x220cf38	 0xe009fa0	 kernel  	  up_assert	/root/product/.tizenrt/os/include/assert.h:211
0x220cf50	 0xe009904	 kernel  	  LOGUART_PutChar_RAM	/root/product/.tizenrt/os/arch/arm/src/chip/amebad_serial.c:382
0x220cf58	 0xe009904	 kernel  	  LOGUART_PutChar_RAM	/root/product/.tizenrt/os/arch/arm/src/chip/amebad_serial.c:382
0x220cf5c	 0x3d7a5 	 app1    	  sched_get_priority_max	/root/product/.tizenrt/os/include/sched.h:292
0x220cf88	 0xe03be70	 kernel  	  STUB_up_assert	/root/product/.tizenrt/os/syscall/stubs/STUB_up_assert.c:7
0x220cf90	 0x10012954	 kernel  	  dispatch_syscall	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_svcall.c:150
0x220cfa0	 0x3d7a5 	 app1    	  sched_get_priority_max	/root/product/.tizenrt/os/include/sched.h:292
0x220cfa8	 0x170   	 app2    	  coap_option_def
0x220d35c	 0xe0189dc	 kernel  	  serial_putc	/root/product/.tizenrt/os/board/rtl8721csm/src/component/common/mbed/targets/hal/rtl8721d/serial_api.c:729
0x220d36c	 0xe0099c0	 kernel  	  rtl8721d_up_txready	/root/product/.tizenrt/os/arch/arm/src/chip/amebad_serial.c:678
0x220d374	 0xe0134d8	 kernel  	  uart_xmitchars	/root/product/.tizenrt/os/include/tinyara/serial/serial.h:362
0x220d38c	 0xe01844c	 kernel  	  uart_irqhandler	/root/product/.tizenrt/os/board/rtl8721csm/src/component/common/mbed/targets/hal/rtl8721d/serial_api.c:295
0x220d3a4	 0x10011710	 kernel  	  wrapper_IrqFun	/root/product/.tizenrt/os/board/rtl8721csm/src/component/os/tizenrt/tizenrt_service.c:1010
0x220d3b0	 0x1001297c	 kernel  	  up_svcall	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_svcall.c:183
0x220d3dc	 0x10012894	 kernel  	  exception_common	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_exception.S:157
0x220d404	 0x41    	 app2    	  __aeabi_dadd
0x220d420	 0x3df95 	 app1    	  pthread_startup	/root/product/.tizenrt/os/include/tinyara/userspace.h:147
0x220d424	 0x41    	 app2    	  __aeabi_dadd
0x220d430	 0xe074d0c	 kernel  	  pthread_start	/root/product/.tizenrt/os/kernel/pthread/pthread_create.c:188

4. Miscellaneous information:
-----------------------------------------------------------------------------------------
```

#### To display Debug Symbols/Crash point using binary other than default build binary
1. Run Ramdump Parser Script and see the Output  
    $ python3 ramdumpParser.py -t `<Log file path>` -b `<binary folder path>` -c `<configuration file path>`
```
vidisha@vidisha:~/tizenRT/tools/trap(trap)$ sudo python3 ramdumpParser.py -t logs -b ../../os/vidisha/bin/ -c ../../os/cfile


*************************************************************
dump_file                   : None
log_file                    : logs
Number of binary            : 1 [kernel]
"kernel" elf_file           : ../../os/vidisha/bin/tinyara.axf
*************************************************************

-----------------------------------------------------------------------------------------
1. Crash Binary             : kernel

2. Crash type               : code assertion by code ASSERT or PANIC

3. Crash point
	-  up_assert: Assertion failed at file:init/os_start.c line: 621 task: Idle Task


	- Code asserted in normal thread.

4. Call stack of last run thread

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
 User stack
0x10004fa0	 0xe00183c	 kernel  	  thread_create	/root/tizenrt/os/kernel/task/task_create.c:133
0x10004fe0	 0x10010d90	 kernel  	  os_start	/root/tizenrt/os/include/tinyara/init.h:89

x. Miscellaneous information:
-----------------------------------------------------------------------------------------
vidisha@vidisha:~/tizenRT/tools/trap(trap)$
```

#### To get call stack using RAM dump
1. Enable memory dumps. Refer to [How to enable memory dumps](how-to-enable-memory-dumps)
2. Get RAM dump using [How to upload RAMDUMP or UserfsDUMP](how-to-upload-ramdump-or-userfsdump) [Options 1- 7]
3. Change the directory to trap.
```
cd $TIZENRT_BASEDIR/tools/trap/
```
4. [Optional] Copy crash logs if any  
    First copy the crash logs to a file in tools/trap/`<log_file>`
5. Run Ramdump Parser Script and see the Output  
    $ python3 ramdumpParser.py -t `<Log file path>` -r `<Ramdump file path>`

    ex)
    $ python3 ramdumpParser.py -t ./log_file -r ../../ramdump_0x02023800_0x02110000.bin OR
    $ python3 ramdumpParser.py -r ../../ramdump_0x02023800_0x02110000.bin

Example Call Stack Output for Kernel crash is as follows:
```
*************************************************************
dump_file         : ../../ramdump_0x02023800_0x02110000.bin
log_file          : logs
elf_file          : ../../build/output/bin/tinyara
*************************************************************

self.ram_base_addr 2023800
self.ram_end_addr 2110000
----------------------------------------------------------
Kernel Crash point is as follows:
[ Caller - return address (LR) - of the function which has caused the crash ]

symbol addr       : 0x040cd264
function name     : irqrestore
file              : /root/tizenrt/os/include/arch/armv7-r/irq.h:414

Kernel Crash point is as follows:
[ Current location (PC) of assert ]
 - Exact crash point might be -4 or -8 bytes from the PC.

symbol addr       : 0x040d53cc
function name     : test_func
file              : /root/tizenrt/apps/examples/hello/hello_main.c:67

--------------------------- DEBUG SYMBOLS IN KERNEL TEXT RANGE --------------------------
Dump_address	 Symbol_address	  Symbol_name	File_name
0x40c9718	 0x40c94fc 	  up_assert	/root/tizenrt/os/include/assert.h:211
0x40ebc72	 0x40ebbd8 	  __FUNCTION__.6146
0x40ebbcb	 0x40ebbcb 	  __FUNCTION__.6135
0x40cf4f8	 0x40cf4e0 	  lowsyslog	/root/tizenrt/os/include/syslog.h:251
0x40c98e4	 0x40c98b0 	  arm_dataabort	/root/tizenrt/os/arch/arm/src/armv7-r/arm_dataabort.c:101
0x40c98e4	 0x40c98b0 	  arm_dataabort	/root/tizenrt/os/arch/arm/src/armv7-r/arm_dataabort.c:101
0x40c827c	 0x40c8220 	  arm_vectordata	/root/tizenrt/os/arch/arm/src/armv7-r/arm_vectors.S:498
0x40cc1d4	 0x40cc18c 	  task_start	/root/tizenrt/os/kernel/task/task_start.c:133
0x40d3c30	 0x40d3c2c 	  hello_main	/root/tizenrt/apps/examples/hello/hello_main.c:73

PC_value	 Symbol_address	  Symbol_name	File_name
0x40d3c30	 0x40d3c2c 	  hello_main	/root/tizenrt/apps/examples/hello/hello_main.c:73
-----------------------------------------------------------------------------------------
----------------------------------------------------------

CALL STACK of Aborted task:
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

[<40d53cc>] hello_main+0x18 [Line 67 of hello_main.c]
[<40cceb8>] task_start+0x50 [Line 180 of task_start.c]

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

```
### TRAP using GUI
The UI configuration of TRAP is as follows

| (X) (-)           | Dump Parser           |        |
| ----------------- |:---------------------:| ------:|
| ELF path          | `<Your ELF path>`     | Browse |
| (O) AssertLog     |                       |        |
| (O) AssertLogFile |                       |        |
| (O) Ramdump       |                       |        |
| Ramdump path      | `<Your Ramdump path>` | Browse |
| Run TRAP    |                       |        |

1. Run GUI Ramdump Parser Script
```
cd $TIZENRT_BASEDIR/tools/trap/
python gui_dumpParser.py
```

2. Browse ELF path
3. Select Ramdump mode
4. Browse Ramdump path
5. Click `Run TRAP` button
6. See the Output

### Example Call Stack Output
```
********************************************************************
Board Crashed at :
PC: [0x40cb800] simulate_data_abort+0x20 [Line 63 of  "hello_main.c]"
LR: [0x40cb340] up_putc+0x28 [Line 1102 of  "chip/s5j_serial.c]"
FP: 0x2024fc4 and SP: 0x2024fb8
*******************************************************************
Call Trace of Crashed Task :[appmain] with pid :2 and state :TSTATE_TASK_RUNNING
*******************************************************************
[<40cb800>] simulate_data_abort+0x20         [Line 63 of \"hello_main.c\"]
[<40cb828>] hello_main+0x18         [Line 68 of \"hello_main.c\"]
[<40c9fec>] task_start+0x64         [Line 173 of \"task/task_start.c\"]
********************************************************************
```

## How to upload RAMDUMP or UserfsDUMP
### In Linux
With the DUMP Tool configured above, whenever the target board crashes because of an assert condition, it enters into PANIC mode, and displays the following message:
```
	****************************************************
	Disconnect this serial terminal and run TRAP Tool
	****************************************************
```
After you see this message, you can upload the dumps by following the step below:  
1. Disconnect/close your serial terminal (may be minicom)  

2. Run TRAP tool
```
cd $TIZENRT_BASEDIR/tools/trap/
./trap.sh
```
3. TRAP Tool will prompt for user's input for device adapter connected to the linux machine.
```
Please enter serial port adapter:
For example: /dev/ttyUSB0 or /dev/ttyACM0
Enter:
/dev/ttyUSB0
/dev/ttyUSB0 open failed!!
Please enter correct device port
Enter:
/dev/ttyUSB1
Target device locked and ready to DUMP!!!
```
4. After entering device adapter information, the tool will provide a list of options for the user to choose from
```
Choose from the following options:-
1. RAM Dump
2. Userfs Dump
3. Both RAM and Userfs dumps
4. External Userfs Partition Dump
5. Exit TRAP Tool
6. Reboot TARGET Device
1 (-> RAM Dump option chosen)
```
5. If RAM Dump option is chosen, tool will prompt the user for the regions to be dumped on successful handshake
```
DUMPING RAM CONTENTS
do_handshake: Target Handshake successful

=========================================================================
Ramdump Region Options:
1. ALL  ( Address: 02023800, Size: 968704 )

=========================================================================
Please enter desired ramdump option as below:
        1 for ALL
Please enter your input: 1

ramdump_recv: No. of Regions to be dumped received

```
6. TRAP Tool receives the RAM contents from target.
```
Receiving ramdump......

=========================================================================
```
```
Dumping data, Address: 0x02023800, Size: 968704bytes
=========================================================================
[===========================================================>]
Ramdump received successfully..!
```
7. The TRAP tool will again provide a list of options for the user to choose from
```
Choose from the following options:-
1. RAM Dump
2. Userfs Dump
3. Both RAM and Userfs dumps
4. External Userfs Partition Dump
5. Exit TRAP Tool
6. Reboot TARGET Device
2 (-> Userfs Dump option chosen)
```
8. If Userfs Dump option is chosen, tool will dump the region on successful handshake
```
DUMPING USERFS CONTENTS
do_handshake: Target Handshake successful

=========================================================================
Filesystem start address = 4620000, filesystem size = 1024000
=========================================================================

Receiving file system dump.....
[==============================================================>]

Filesystem Dump received successfully
```
9. The TRAP tool will again provide a list of options for the user to choose from
```
Choose from the following options:-
1. RAM Dump
2. Userfs Dump
3. Both RAM and Userfs dumps
4. External Userfs Partition Dump
5. Exit TRAP Tool
6. Reboot TARGET Device
4 (-> External Userfs Partition dump option chosen)
```
10. If External Userfs Partition dump option is chosen, tool will dump the Userfs partition on the External Flash on successful handshake..
```
DUMPING EXTERNAL USERFS DUMP PARTITION
do_handshake: Target Handshake successful

=========================================================================
External filesystem size = 03145728
=========================================================================

Receiving external file system dump.....
[===================-========================================================================================
=============================================================================================================
=================>]

External Userfs partition dump received successfully
```
11. The TRAP tool will again provide a list of options for the user to choose from
```
Choose from the following options:-
1. RAM Dump
2. Userfs Dump
3. Both RAM and Userfs dumps
4. External Userfs Partition Dump
5. Exit TRAP Tool
6. Reboot TARGET Device
6 (-> Reboot TARGET Device option chosen)
```
12. If Reboot TARGET Device option is chosen, tool will send a Reboot signal string to the TARGET device and exit
```
CONFIG_BOARD_ASSERT_AUTORESET needs to be enabled to reboot TARGET Device after a crash
do_handshake: Target Handshake successful
Dump tool exits after successful operation
```
13. The TRAP tool exits if user chooses Option 5.
```
Dump tool exits after successful operation
```
14. NOTE: The device stays in a loop, waiting for the next handshake/reboot signal till the user sends the Reboot signal.

### In Windows
With the RAMDUMP configured above, whenever the target board crashes because an assert condition, it enters PANIC mode, and displays the following message:  
```
	****************************************************
	Disconnect this serial terminal and Run Ramdump Tool
	****************************************************
```
After you see this message, you can upload the ramdump by following the step below:  
1. Disconnect/close your serial terminal (may be TeraTerm)

2. Open windows powershell & Run ramdump tool
```
<tool_path>:\Ramdump_windows.ps1
```
3. Enter the COM port number for your device
```
Please enter COM port number & press enter : <ComPort>
```
Once connection with the COM port is established, you will get following message:  
--> Connection established.

Now you can enter desired ramdump region from the options: (Multi-heap scenario)
```
Target Handshake SUCCESSFUL !!!
Target entered to ramdump mode

=========================================================================
Ramdump Region Options:
1. ALL
2. Region : 0 ( Address: 0x02023800, Size: 61440)       [Heap index = 0]
3. Region : 1 ( Address: 0x02032800, Size: 81920)       [Heap index = 1]
4. Region : 2 ( Address: 0x02046800, Size: 825344)      [Heap index = 0]
=========================================================================
Please enter desired ramdump option as below:
1 for ALL
2 for Region 0
25 for Region 0 & 3 ...

Please enter your input : 1

```
4. Ramdump Tool receives the ram contents from target.
```
Target No. of Regions to be dumped received!

Receiving ramdump......

Target Region info received!
=========================================================================
Dumping Region: 0, Address: 0x02023800, Size:61440bytes
=========================================================================
[===>]
Copying...
to C:\Users\thapa.v\ramdump_0x02023800_0x61440.bin

Target Region info received!
=========================================================================
Dumping Region: 1, Address: 0x02032800, Size:81920bytes
=========================================================================
[=====>]
Copying...
to C:\Users\thapa.v\ramdump_0x02032800_0x81920.bin

Target Region info received!
=========================================================================
Dumping Region: 2, Address: 0x02046800, Size:825344bytes
=========================================================================
[=======================================>
Copying...
to C:\Users\thapa.v\ramdump_0x02046800_0x825344.bin

Ramdump received successfully..!
```

## How to port memory dump functionality
To port TRAP tool for a new board, do the following steps:

1. Add low level chip specific API's to receive and transfer characters through UART:

a. **up_putc()** : Output one byte on the serial console
```
 * Prototype: int up_putc(int ch)
 * Input Parameters:
 *   ch - chatacter to output
 * Returned Value:
 *   sent character
```
b. **up_getc()** : Read one byte from the serial console
```
 * Prototype: int up_getc(void)
 * Input Parameters:
 *   none
 * Returned Value:
 *   int value, -1 if error, 0~255 if byte successfully read
```
c. **up_puts()** : Output string on the serial console
```
 * Prototype: void up_puts(const char *str)
 * Input Parameters:
 *   str - string to output
 * Returned Value:
 *   none
```
2. Source the low level API file in the Make.defs of os/arch/<cpu_name>/src/<chip_name>
```
CMN_CSRCS += <chip_name>_serial.c
```
3. Source the crashdump.c file in chip and board Makefile (os/board/<board_name>/src)
```
DEPPATH += --dep-path $(TOPDIR)/board/common
VPATH += :$(TOPDIR)/board/common

ifeq ($(CONFIG_BOARD_CRASHDUMP),y)
CSRCS += crashdump.c
endif
```
4. Add board_crashdump() API hook to architecture specific up_assert() if it does not exist already.
```
#if defined(CONFIG_BOARD_CRASHDUMP)
       board_crashdump(up_getsp(), this_task(), (uint8_t *)filename, lineno);
#endif
```
5. In trap.c, configure correct port parameters for the the board's tty serial device port n configure_tty function.
Like BaudRate, StopBits, Parity, Databits, HardwareFlowControl.
6. In trap.c, add if the serial device port does not exist already.
```
        /* Get the tty type  */
        if (!strcmp(dev_file, "/dev/ttyUSB1")) {
                strncpy(tty_type, "ttyUSB1", TTYTYPE_LEN);
        } else if (!strcmp(dev_file, "/dev/ttyACM0")) {
                strncpy(tty_type, "ttyACM0", TTYTYPE_LEN);
        } else {
                printf("Undefined tty %s\n", dev_file);
                return -1;
        }
```
