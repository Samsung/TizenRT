# How to Use TRAP (TizenRT dump and Abort Parser) Tool
Here we explain how to enable build support for DUMP upload,
and how to actually upload and parse it in the event of a crash during runtime.

## Contents
> [Build Steps](#how-to-enable-memory-dumps)  
> [Upload Steps](#how-to-upload-RAMDUMP-or-UserfsDUMP)  
> [Parsing Steps](#how-to-parse-RAMDUMP)  
> [Porting Guide](#how-to-port-memory-dump-functionality)  

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

## How to upload RAMDUMP-or-UserfsDUMP
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

## How to parse RAMDUMP
DumpParser Script provides two interfaces: CUI and GUI

### DumpParser using CUI

#### To get call stack using RAM dump
1. Run Ramdump Parser Script
```
cd $TIZENRT_BASEDIR/tools/trap/
python dumpParser.py -r $TIZENRT_BASEDIR/build/output/bin/ramdump_0x02020000_0x0210c800.bin -e $TIZENRT_BASEDIR/build/output/bin/tinyara -g 0
```
2. See the Output

#### To display Debug Symbols/Crash point using assert logs
1. Copy crash logs
    First copy the crash logs to a file named log_file in tools/trap/`<log_file>`
2. Run Script in tools/trap path
    $ python3 ramdumpParser.py -e `<Elf path>` -t `<Log file path>`

    ex)
    $ python3 ramdumpParser.py -e ../../build/output/bin/app2_dbg -t ./log_file

Example Call Stack Output for App crash is as follows:
```
*************************************************************
dump_file         : None
log_file          : .log_file
elf_file  : ../../../output/app2_dbg
*************************************************************

Number of applicaions : 2
App[1] is : app1
App[2] is : app2

----------------------------------------------------------
App Crash point is as follows:
[ Caller - return address (LR) - of the function which has caused the crash ]
App name : app2
symbol addr : 0x000014d6
main
/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:121

[Probable crash point corresponding to the R15 register value (PC) might be -4 or -8 bytes]
App name : app2
0x000014e5
function name : main
file : /root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:113

-------------------------- DEBUG SYMBOLS IN APPLICATION TEXT RANGE -------------------------
Dump_address	 Symbol_address	  Symbol_name	File_name
0x1dc5	 0x1db5 	  write	/root/tizenrt/os/include/unistd.h:310
0x1dc5	 0x1db5 	  write	/root/tizenrt/os/include/unistd.h:310
0x14e5	 0x1479 	  main	/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:57
0x1479	 0x1479 	  main	/root/tizenrt/loadable_apps/loadable_sample/wifiapp/wifiapp.c:57
0x1ef5	 0x1edd 	  task_startup	/root/tizenrt/os/include/tinyara/userspace.h:127

PC_value	 Symbol_address	  Symbol_name	File_name
-----------------------------------------------------------------------------------------
----------------------------------------------------------
```
ex)
$ python3 ramdumpParser.py -e ../../build/output/bin/tinyara -t ./log_file

Example Call Stack Output for Kernel crash is as follows:
```
*************************************************************
dump_file         : None
log_file          : ./log_file
elf_file  : ../../build/output/bin/tinyara.axf
*************************************************************


----------------------------------------------------------
Kernel Crash point is as follows:
[ Caller - return address (LR) - of the function which has caused the crash ]
symbol addr : 0x10010f36
function name : os_start
file : /root/product/.tizenrt/os/kernel/init/os_start.c:615

--------------------------- DEBUG SYMBOLS IN KERNEL TEXT RANGE --------------------------
Dump_address	 Symbol_address	  Symbol_name	File_name
0xe0558ab	 0xe05589b 	  __FUNCTION__.6623
0xe006337	 0xe006310 	  lowvsyslog	/root/product/.tizenrt/os/include/syslog.h:263
0xe0087dd	 0xe0087dc 	  lowoutstream_putc	/root/product/.tizenrt/lib/libc/stdio/lib_lowoutstream.c:76
0xe009df5	 0xe009db0 	  up_usagefault	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_usagefault.c:83
0x10012565	 0x1001254c 	  up_doirq	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_doirq.c:91
0x10012275	 0x10012234 	  exception_common	/root/product/.tizenrt/os/arch/arm/src/armv8-m/up_exception.S:156
0x10012809	 0x10012808 	  up_unblock_task	/root/product/.tizenrt/os/include/tinyara/arch.h:472
0xe001c15	 0xe001be8 	  task_activate	/root/product/.tizenrt/os/include/sched.h:141
0x10010f36	 0x10010d90 	  os_start	/root/product/.tizenrt/os/include/tinyara/init.h:89
0xe000e6f	 0xe000d2c 	  app_start	/root/product/.tizenrt/os/board/rtl8721csm/src/component/soc/realtek/amebad/fwlib/ram_hp/rtl8721dhp_app_start.c:1318

PC_value	 Symbol_address	  Symbol_name	File_name
0x10010f36	 0x10010d90 	  os_start	/root/product/.tizenrt/os/include/tinyara/init.h:89
-----------------------------------------------------------------------------------------
----------------------------------------------------------
```
ex)
$python3 ramdumpParser.py -e ../../build/output/bin/common_dbg -t ./log_file

Example Call Stack Output for Common Binary crash is as follows:
```
*************************************************************
dump_file         : None
log_file          : ./log_file
elf_file  : ../../build/output/bin/common_dbg
*************************************************************

Number of applicaions : 2
App[1] is : common
App[2] is : app

----------------------------------------------------------
App Crash point is as follows:
[Probable crash point corresponding to the R15 register value (PC) might be -4 or -8 bytes]
[ Current location (PC) of assert ]
 - Exact crash point might be -4 or -8 bytes from the PC.
App name : common
symbol addr : 0x0003cdea
function name : sched_get_priority_max
/root/product/.tizenrt/lib/libc/sched/sched_getprioritymax.c:110

[ Caller - return address (LR) - of the function which has caused the crash ]
App name : app
symbol addr : 0x00000551
function name : set_app_main_task
file : /root/product/main/set_app_main/src/set_app_main.c:380 (discriminator 1)

-------------------------- DEBUG SYMBOLS IN APPLICATION TEXT RANGE -------------------------
Dump_address	 Symbol_address	  Symbol_name	File_name
0x551	 0x3e2 	  gstSupportCourseInfo	/root/product/main/set_app_main/src/set_app_main.c:312
0x3011	 0x3011 	  wm_easysetup_event_handler	/root/product/main/fill_ocf/src/fill_ocf.c:80
0x57a1	 0x5729 	  process_operation_uri_from_bixby
0x2eb9	 0x2e11 	  device_publish_resource_list_getter	/root/product/main/fill_ocf/src/fill_ocf.c:204
0x2e11	 0x2df7 	  init_product_resources	/root/product/main/fill_ocf/src/fill_ocf.c:228
0x1e49	 0x1e41 	  otn_wifi_apporve_update	/root/product/apps/otn/wifi_updater/src/otn_wifi_update.c:159
0x2df7	 0x2df7 	  init_product_resources	/root/product/main/fill_ocf/src/fill_ocf.c:228
0x105	 0x105 	  set_app_main_task	/root/product/main/set_app_main/src/set_app_main.c:128
0x105	 0x105 	  set_app_main_task	/root/product/main/set_app_main/src/set_app_main.c:128

PC_value	 Symbol_address	  Symbol_name	File_name
-----------------------------------------------------------------------------------------
----------------------------------------------------------

```
### DumpParser using GUI
The UI configuration of DumpParser is as follows

| (X) (-)           | Dump Parser           |        |
| ----------------- |:---------------------:| ------:|
| ELF path          | `<Your ELF path>`     | Browse |
| (O) AssertLog     |                       |        |
| (O) AssertLogFile |                       |        |
| (O) Ramdump       |                       |        |
| Ramdump path      | `<Your Ramdump path>` | Browse |
| Run DumpParser    |                       |        |

1. Run GUI Ramdump Parser Script
```
cd $TIZENRT_BASEDIR/tools/trap/
python gui_dumpParser.py
```

2. Browse ELF path
3. Select Ramdump mode
4. Browse Ramdump path
5. Click `Run DumpParser` button
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
