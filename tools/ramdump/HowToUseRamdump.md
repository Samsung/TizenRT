# How to Use Ramdump
Here we explain how to enable build support for RAMDUMP upload,  
and how to actually upload and parse it in the event of a crash during runtime.

## Contents
> [Build Steps](#how-to-enable-ramdump)  
> [Upload Steps](#how-to-upload-ramdump)  
> [Parsing Steps](#how-to-parse-ramdump)  

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
Run ramdump tool from host after target crashed.  
1. If target got PANIC (asserted), below messages will be promted.
```
	****************************************************
	Disconnect this serial terminal and Run Ramdump Tool
	****************************************************
```
2. Disconnect/close serial termial (may be minicom)  

3. Run ramdump tool
```
cd $TIZENRT_BASEDIR/tools/ramdump/
./ramdump.sh
```
4. Ramdump Tool receives the ram contents from target.
```
Target Handshake successful do_handshake
Target entered to ramdump mode
Receiving ramdump......
ramdump_recv: ramdump_address = 02020000, ramdump_size = 968704
[===========================================================>]
Ramdump received successfully
copying ramdump_0x02020000_0x0210c800.bin to  $TIZENRT_BASEDIR/build/output/bin
```

## How to parse RAMDUMP
DumpParser Script privodes two interfaces: CUI and GUI

### DumpParser using CUI
1. Run Ramdump Parser Script
```
cd $TIZENRT_BASEDIR/tools/ramdump/
python dumpParce.py -r $TIZENRT_BASEDIR/build/output/bin/ramdump_0x02020000_0x0210c800.bin -e $TIZENRT_BASEDIR/build/output/bin/tinyara -g 0
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
