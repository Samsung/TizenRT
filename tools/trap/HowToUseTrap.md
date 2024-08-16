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
===========================================================
Assertion details
===========================================================
print_assert_detail: Assertion failed CPU1 at file: armv7-a/arm_dataabort.c line 202 task: hello pid: 20
print_assert_detail: Assert location (PC) : 0x0e16fe34
check_assert_location: Code asserted in normal thread!
===========================================================
Asserted task's stack details
===========================================================
check_sp_corruption: Current SP is User Thread SP: 6060ad70
check_sp_corruption: User stack:
print_stack_dump:   base: 6060ad90
print_stack_dump:   size: 00001ff0
print_stack_dump:   used: 000002b4
up_stackdump: 6060ad60: xxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx 60608d90 0e16ba09 60608d90 0e0197bb
up_stackdump: 6060ad80: 00000000 00000000 00000000 00000000 00002010 d475dd55 552756de 000f5260
.
.
.
===========================================================
Checking app heap for corruption
===========================================================
mm_check_heap_corruption: Heap start = 0x60600a30 end = 0x606ffff0
mm_check_heap_corruption: No heap corruption detected
===========================================================
Checking kernel heap for corruption
===========================================================
mm_check_heap_corruption: Heap start = 0x60119000 end = 0x603ffff0
mm_check_heap_corruption: No heap corruption detected
##########################################################################################################################################
```
3. In the scenario where log file format is not supported, you will see a prompt as below:
```
test@VirtualBox ~/tizenRTGH/tools/trap (master) $sudo python3 trap.py -t logs

	- Below log format is not supported in TRAP
		-|19:27:38.36| up_assert: Assertion failed at file:armv8-m/up_memfault.c line: 156 task: main_task
	- Instead, supported log format in TRAP is as follows:
		-print_assert_detail: Assertion failed at file:armv8-m/up_memfault.c line: 156 task: main_task

	Kindly modify the log file as per accepted format.

```
Modify the log file to a supported format as shown in point 2.

4. Run Ramdump Parser Script and see the Output  
    $ python3 trap.py -t `<Log file path>`

ex)
$ python3 trap.py -t ./log_file

Example Call Stack Output for App crash is as follows:
```
ubuntu@ubuntu:~/TizenRT/tools/trap$ python3 trap.py -t ./crash_app


*************************************************************
dump_file                   : None
log_file                    : ./crash_app
Number of binaries          : 3 [kernel + apps]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
"app1" elf_file             : ../../build/output/bin/common_dbg 
"app2" elf_file             : ../../build/output/bin/app1_dbg 
*************************************************************

-----------------------------------------------------------------------------------------

1. Crash Binary             : app1

2. Crash type               : code assertion by code ASSERT or PANIC

3. Crash point
	- Assertion failed CPU0 at file: armv7-a/arm_dataabort.c line 202 task: app1 pid: 15


4. Code asserted in:

	- Code asserted in normal thread.

5. Call stack of last run thread

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
 User stack
0x60602a38	 0xe1645b1	 app1    	  task_startup	/root/tizenrt/os/include/tinyara/userspace.h:145
0x60602a40	 0xe01976d	 kernel  	  task_start	/root/tizenrt/os/kernel/task/task_start.c:133
0x60602a58	 0xe018e6d	 kernel  	  group_zalloc	/root/tizenrt/os/include/tinyara/kmalloc.h:166

h. Heap Region information:

	!!!! NO HEAP CORRUPTION DETECTED !!!!


Checking application heap for corruption
Checking corruption (0x60600a30 - 0x606ffff0) : Heap NOT corrupted
No app heap corruption detected.

Checking kernel heap for corruption
Checking corruption (0x60119000 - 0x603ffff0) : Heap NOT corrupted
No Kernel heap corruption detected.


x. Miscellaneous information:

List of all tasks in the system:

1                          NAME |   PID |  PRI |    USED / TOTAL STACK | STACK ALLOC ADDR | TCB ADDR | TASK STATE
-------------------------------------------------------------------------------------------------------------------
                     CPU0 IDLE |     0 |    0 |     748 /    1024 |       0x60118c00 | 0x60105314 |  Assigned to CPU (Ready)
                     CPU1 IDLE |     1 |    0 |     656 /    1024 |       0x60118800 | 0x60105418 |  Assigned to CPU (Ready)
                        hpwork |     2 |  201 |     452 /    2032 |       0x60122530 | 0x60121ac0 |  Wait Signal
                        lpwork |     3 |   50 |     452 /    4080 |       0x601237b0 | 0x60122d40 |  Wait Signal
               inic_msg_q_task |     5 |  107 |     444 /    4048 |       0x60131910 | 0x60130ea0 |  Wait Semaphore
          inic_host_rx_tasklet |     6 |  104 |     500 /    4048 |       0x60133410 | 0x601329a0 |  Wait Semaphore
        inic_ipc_api_host_task |     7 |  103 |     460 /    8136 |       0x60134ed0 | 0x60134460 |  Wait Semaphore
                   LWIP_TCP/IP |     9 |  105 |     508 /    4072 |       0x6013b880 | 0x6013ae10 |  Wait Semaphore
          netmgr_event_handler |    10 |  100 |     388 /    2016 |       0x6013d300 | 0x6013c890 |  Wait Semaphore
                      log_dump |    11 |  200 |     548 /   16360 |       0x6013e650 | 0x6013dbe0 |  Wait MQ Receive (MQ Empty)
                binary_manager |    12 |  203 |     728 /    8168 |       0x60146200 | 0x60142660 |  Wait MQ Receive (MQ Empty)
                     bm_loader |    14 |   90 |     848 /    4072 |       0x601375a0 | 0x60125350 |  Running
                          app1 |    15 |  180 |     804 /    8168 |       0x60600a50 | 0x601257d0 |  Running
-------------------------------------------------------------------------------------------------------------------
```
ex)
$ python3 trap.py -t ./logs

Example Call Stack Output for Kernel crash is as follows:
```
ubuntu@ubuntu:~/TizenRT/tools/trap$ python3 trap.py -t ./crash_kernel


*************************************************************
dump_file                   : None
log_file                    : ./crash_kernel
Number of binary            : 1 [kernel]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
*************************************************************

-----------------------------------------------------------------------------------------
1. Crash Binary             : kernel

2. Crash type               : code assertion by code ASSERT or PANIC

3. Crash point
	- Assertion failed CPU0 at file: armv7-a/arm_dataabort.c line 202 task: CPU0 IDLE pid: 0


4. Code asserted in:

	- Code asserted in normal thread.

5. Call stack of last run thread

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
 User stack
0x60118f58	 0xe026931	 kernel  	  up_stack_frame	/root/tizenrt/os/include/tinyara/arch.h:375
0x60118f68	 0xe019711	 kernel  	  task_activate	/root/tizenrt/os/include/sched.h:253
0x60118f70	 0xe0198fd	 kernel  	  thread_termination_handler	/root/tizenrt/os/kernel/task/task_terminate.c:297
0x60118f88	 0xe0192c1	 kernel  	  thread_create	/root/tizenrt/os/kernel/task/task_create.c:133
0x60118fb0	 0xe0193d9	 kernel  	  kernel_thread	/root/tizenrt/os/include/tinyara/kthread.h:98
0x60118fb4	 0xe0271cd	 kernel  	  SOCPS_LPWAP_ipc_int	/root/tizenrt/os/arch/arm/src/chip/amebasmart_pmhelpers.c:154
0x60118fc0	 0xe0271f9	 kernel  	  bsp_pm_domain_register	/root/tizenrt/os/arch/arm/src/chip/amebasmart_pmhelpers.c:127
0x60118fd8	 0xe018891	 kernel  	  os_bringup	/root/tizenrt/os/kernel/init/os_bringup.c:475
0x60118fe8	 0xe018525	 kernel  	  os_start	/root/tizenrt/os/include/tinyara/init.h:134

h. Heap Region information:

	!!!! NO HEAP CORRUPTION DETECTED !!!!


Checking kernel heap for corruption
Checking corruption (0x60119000 - 0x603ffff0) : Heap NOT corrupted
No Kernel heap corruption detected.


x. Miscellaneous information:

List of all tasks in the system:

                    NAME |   PID |  PRI |    USED / TOTAL STACK | STACK ALLOC ADDR | TCB ADDR | TASK STATE
-------------------------------------------------------------------------------------------------------------------
               CPU0 IDLE |     0 |    0 |     892 /    1024 |       0x60118c00 | 0x60105314 |  Running
               CPU1 IDLE |     1 |    0 |     656 /    1024 |       0x60118800 | 0x60105418 |  Running
                  hpwork |     2 |  201 |     328 /    2032 |       0x60122530 | 0x60121ac0 |  Invalid
-------------------------------------------------------------------------------------------------------------------
```
ex)
python3 trap.py -t ./logfile

Example Call Stack Output for Common Binary crash is as follows:
```
ubuntu@ubuntu:~/TizenRT/tools/trap$ python3 trap.py -t ./crash3


*************************************************************
dump_file                   : None
log_file                    : ./crash3
Number of binaries          : 3 [kernel + apps]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
"app1" elf_file             : ../../build/output/bin/common_dbg 
"app2" elf_file             : ../../build/output/bin/app1_dbg 
*************************************************************

-----------------------------------------------------------------------------------------

1. Crash Binary             : common

2. Crash type               : code assertion by code ASSERT or PANIC

3. Crash point
	- Assertion failed CPU1 at file: hello_main.c line 72 task: hello pid: 20


4. Code asserted in:

	- Code asserted in normal thread.

5. Call stack of last run thread

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
 User stack
0x6060acf8	 0xe17c44c	 app1    	  builtin_list
0x6060ad18	 0xe16fd8d	 app1    	  utils_readfile	/root/tizenrt/apps/system/utils/utils_proc.c:96
0x6060ad34	 0xe01f551	 kernel  	  STUB_up_assert	/root/tizenrt/os/syscall/stubs/STUB_up_assert.c:7
0x6060ad38	 0xe17c44c	 app1    	  builtin_list
0x6060ad3c	 0xe001395	 kernel  	  dispatch_syscall	/root/tizenrt/os/arch/arm/src/armv7-a/arm_syscall.c:148
0x6060ad40	 0xe16fd8d	 app1    	  utils_readfile	/root/tizenrt/apps/system/utils/utils_proc.c:96
0x6060ad4c	 0xe16fe29	 app1    	  vulnerable_function	/root/tizenrt/apps/examples/hello/hello_main.c:66
0x6060ad50	 0xe16fe29	 app1    	  vulnerable_function	/root/tizenrt/apps/examples/hello/hello_main.c:66
0x6060ad58	 0xe16b9f1	 app1    	  task_startup	/root/tizenrt/os/include/tinyara/userspace.h:145
0x6060ad60	 0xe01976d	 kernel  	  task_start	/root/tizenrt/os/kernel/task/task_start.c:133

h. Heap Region information:

	!!!! NO HEAP CORRUPTION DETECTED !!!!


Checking application heap for corruption
Checking corruption (0x60600a30 - 0x606ffff0) : Heap NOT corrupted
No app heap corruption detected.

Checking kernel heap for corruption
Checking corruption (0x60119000 - 0x603ffff0) : Heap NOT corrupted
No Kernel heap corruption detected.


x. Miscellaneous information:

List of all tasks in the system:

             NAME |   PID |  PRI |    USED / TOTAL STACK | STACK ALLOC ADDR | TCB ADDR | TASK STATE
-------------------------------------------------------------------------------------------------------------------
        CPU0 IDLE |     0 |    0 |     748 /    1024 |       0x60118c00 | 0x60105314 |  Running
        CPU1 IDLE |     1 |    0 |     656 /    1024 |       0x60118800 | 0x60105418 |  Assigned to CPU (Ready)
           hpwork |     2 |  201 |     452 /    2032 |       0x60122530 | 0x60121ac0 |  Wait Signal
           lpwork |     3 |   50 |     452 /    4080 |       0x601237b0 | 0x60122d40 |  Wait Signal
  inic_msg_q_task |     5 |  107 |     444 /    4048 |       0x60131910 | 0x60130ea0 |  Wait Semaphore
inic_host_rx_tasklet |     6 |  104 |     500 /    4048 |       0x60133410 | 0x601329a0 |  Wait Semaphore
inic_ipc_api_host_task |     7 |  103 |     460 /    8136 |       0x60134ed0 | 0x60134460 |  Wait Semaphore
      LWIP_TCP/IP |     9 |  105 |     508 /    4072 |       0x6013b880 | 0x6013ae10 |  Wait Semaphore
netmgr_event_handler |    10 |  100 |     388 /    2016 |       0x6013d300 | 0x6013c890 |  Wait Semaphore
         log_dump |    11 |  200 |     668 /   16360 |       0x6013e650 | 0x6013dbe0 |  Wait MQ Receive (MQ Empty)
   binary_manager |    12 |  203 |     728 /    8168 |       0x601431b0 | 0x60142660 |  Wait MQ Receive (MQ Empty)
             app1 |    15 |  180 |     780 /    8168 |       0x60600a50 | 0x601257a0 |  Wait Signal
            uwork |    16 |  100 |     472 /    2032 |       0x606046b0 | 0x601258c0 |  Wait Signal
             tash |    17 |  125 |     676 /    4080 |       0x60605150 | 0x60124f00 |  Wait Semaphore
 wifi msg handler |    18 |  100 |     572 /    4064 |       0x60606880 | 0x60125020 |  Wait Semaphore
  ble msg handler |    19 |  100 |     572 /    4072 |       0x60607ad0 | 0x60125140 |  Wait Semaphore
            hello |    20 |  100 |     472 /    8176 |       0x60608d70 | 0x60125370 |  Running
-------------------------------------------------------------------------------------------------------------------
```

#### To display Debug Symbols/Crash point using binary other than default build binary
1. Run Ramdump Parser Script and see the Output  
    $ python3 trap.py -t `<Log file path>` -b `<binary folder path>` -c `<configuration file path>`
```
vidisha@vidisha:~/tizenRT/tools/trap(trap)$ sudo python3 trap.py -t logs -b ../../os/vidisha/bin/ -c ../../os/cfile


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
	-  print_assert_detail: Assertion failed at file:init/os_start.c line: 621 task: Idle Task


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
    $ python3 trap.py -t `<Log file path>` -r `<Ramdump file path>`

    ex)
    $ python3 trap.py -t ./log_file -r ../../ramdump_0x02023800_0x02110000.bin OR
    $ python3 trap.py -r ../../ramdump_0x02023800_0x02110000.bin

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
5. Dump specific file from target device
r. Reboot TARGET Device
x. Exit TRAP tool
You may chose mutiple dump options together by entering the option numbers consectuively
For Example - '14' to dump both RAM and External USERFS partition contents OR '35' to print RAM, USERFS and FILE contents
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
5. Dump specific file from target device
r. Reboot TARGET Device
x. Exit TRAP Tool
You may chose mutiple dump options together by entering the option numbers consectuively
For Example - '14' to dump both RAM and External USERFS partition contents OR '35' to print RAM, USERFS and FILE contents
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
5. Dump specific file from target device
r. Reboot TARGET Device
x. Exit TRAP Tool
You may chose mutiple dump options together by entering the option numbers consectuively
For Example - '14' to dump both RAM and External USERFS partition contents OR '35' to print RAM, USERFS and FILE contents
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
5. Dump specific file from target device
r. Reboot TARGET Device
x. Exit TRAP Tool
You may chose mutiple dump options together by entering the option numbers consectuively
For Example - '14' to dump both RAM and External USERFS partition contents OR '35' to print RAM, USERFS and FILE contents
r (-> Reboot TARGET Device option chosen)
```
12. If Reboot TARGET Device option is chosen, tool will send a Reboot signal string to the TARGET device and exit
```
CONFIG_BOARD_ASSERT_AUTORESET needs to be enabled to reboot TARGET Device after a crash
do_handshake: Target Handshake successful
Dump tool exits after successful operation
```
13. The TRAP tool exits if user chooses Option 'x'.
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
