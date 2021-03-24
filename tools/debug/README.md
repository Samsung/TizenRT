# DebugSymbolViewer

The Debug Symbol viewer will help the developer when a crash occurs.  
It will show some call stack information when the application has faced an abort. To be precise, it will show the symbols corresponding to the stack addresses present in the kernel text address range.   

A sample of the values that will be shown are as below:

```
Dump_address	 Symbol_address	  Symbol_name	File_name
0xe005e15	 0xe005e14 	  hello_main	/home/test/tizenrt/apps/examples/hello/hello_main.c:73
```

where,
The `Dump_address` corresponds to an address on the stack. It might be an address of any line in the middle of a function, which was getting executed at the time of the dump.  
The `Symbol_address` is the address of the first instruction of a function. Since our map file only provides symbol address, we use this to print the symbol name and its corresponding line number.   
Hence, the line number printed after the *File _name* is the line number of the first line of the function. However, the execution might be at a later line at the time of crash. The user needs to manually check for this exact line during debugging.   
The `Symbol_name` is the name of the symbol corresponding to the *Symbol_address*.   
The `File_name` is the name of the file where the *Dump_address* lies.   

### Prerequisites
Install python 3.8 & python 2.7, pyserial(at pip)

### How to USE

1. Copy crash logs  
    First copy the crash logs to a file named log_file in tools/dump_tool/`<log_file>`
2. Run Script in tools/dump_tool path  
    $ python3 ramdumpParser.py -e `<Tinyara elf path>` -t `<Log file path>`

    ex)
    $ python3 ramdumpParser.py -e ../../build/output/bin/tinyara.axf -t ./log_file

```
*************************************************************
dump_file         : None
log_file          : .log_file
Tinyara_elf_file  : ../../build/output/bin/tinyara.axf
*************************************************************

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  DEBUG DISPLAY SYMBOLS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Dump_address	 Symbol_address	  Symbol_name	File_name
0xe005e01	 0xe005df8 	  test_func	/home/test/tizenrt/apps/examples/hello/hello_main.c:64
0xe05762c	 0xe0575d9 	  __FUNCTION__.5305
0xe005e21	 0xe005e14 	  hello_main	/home/test/tizenrt/apps/examples/hello/hello_main.c:73
0xe005e15	 0xe005e14 	  hello_main	/home/test/tizenrt/apps/examples/hello/hello_main.c:73
0xe001ad5	 0xe001aa8 	  task_start	/home/test/tizenrt/os/kernel/task/task_start.c:133

PC_value	 Symbol_address	  Symbol_name		File_name
0xe005df8	 0xe005dee 	  TZ_StoreContext_S	/home/test/tizenrt/os/include/tinyara/tz_context.h:110
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
```
