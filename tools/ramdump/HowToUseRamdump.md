# How to Use Ramdump
Here we explain how to enable build support for RAMDUMP upload,  
and how to actually upload and parse it in the event of a crash during runtime.

## Contents
> [Build Steps](#how-to-enable-ramdump)  
> [Upload Steps](#how-to-upload-ramdump)  
> [Parsing Steps](#how-to-parse-ramdump)  
> [Porting Guide](#how-to-port-ramdump)

## How to enable RAMDUMP
Below configuration must be enabled to support ramdump upload  
1. Enable crashdump support (CONFIG_BOARD_CRASHDUMP=y)
```
Hardware Configuration > Board Selection -> Enable Board level logging of crash dumps to y
```
2. Enable ramdump (CONFIG_BOARD_RAMDUMP_UART=y)
```
Hardware Configuration > Board Selection -> Enable ramdump via UART to y
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

## How to upload RAMDUMP
### In Linux
With the RAMDUMP configured above, whenever the target board crashes because an assert condition, it enters PANIC mode, and displays the following message:  
```
	****************************************************
	Disconnect this serial terminal and Run Ramdump Tool
	****************************************************
```
After you see this message, you can upload the ramdump by following the step below:  
1. Disconnect/close your serial terminal (may be minicom)  

2. Run ramdump tool
```
cd $TIZENRT_BASEDIR/tools/ramdump/
./ramdump.sh
```
3. Ramdump Tool will prompt for user's input for device adapter connected to the linux machine.
```
Please enter serial port adapter:
For example: /dev/ttyUSB0 or /dev/ttyACM0
Enter:
/dev/ttyUSB0
/dev/ttyUSB0 open failed!!
Please enter correct device port
Enter:
```
4. After entering device adapter information, tool will prompt for regions to be dumped
```
Enter:
/dev/ttyUSB1
Target Handshake successful do_handshake
Target entered to ramdump mode

=========================================================================
Ramdump Region Options:
1. ALL  ( Address: 02023800, Size: 968704 )

=========================================================================
Please enter desired ramdump option as below:
        1 for ALL
Please enter your input: 
```
5. Ramdump Tool receives the ram contents from target.
```
Target Handshake successful do_handshake
Target entered to ramdump mode
Receiving ramdump......
ramdump_recv: ramdump_address = 02020000, ramdump_size = 968704
[===========================================================>]
Ramdump received successfully
copying ramdump_0x02020000_0x0210c800.bin to  $TIZENRT_BASEDIR/build/output/bin
```
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
1. Run Ramdump Parser Script
```
cd $TIZENRT_BASEDIR/tools/ramdump/
python dumpParser.py -r $TIZENRT_BASEDIR/build/output/bin/ramdump_0x02020000_0x0210c800.bin -e $TIZENRT_BASEDIR/build/output/bin/tinyara -g 0
```
2. See the Output

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
cd $TIZENRT_BASEDIR/tools/ramdump/
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
## How to port RAMDUMP
To port ramdump for a new board, do the following steps:

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
5. In ramdump_tool.c, configure correct port parameters for the the board's tty serial device port n configure_tty function.
Like BaudRate, StopBits, Parity, Databits, HardwareFlowControl.
6. In ramdump_tool.c, add if the serial device port does not exist already.
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
