# How to use Memory Leakage Checker

The Memory Leakage Checker shows leakage information of dynamic allocated memory at that time.  
It checks the entire RAM regions for an allocated memory address. If no region has that address,  
it is a leakage because it can not be released.  
This document describes how to enable and use the Leak Checker.

## How to enable

1. Enable *CONFIG_DEBUG_MEM_LEAK_CHECKER* using menuconfig as shown below:
```
Debug Options -> Enable Debug Features -> [*] Memory Leak Checker
```
2. Set *CONFIG_DEBUG_MEM_LEAK_CHECKER_HASH_TABLE_SIZE* value using menuconfig as shown below:
```
Debug Options -> Enable Debug Features -> Memory Leak Checker -> size of hash table
```
3. Enable *CONFIG_MEM_LEAK_CHECKER* to build the TASH command.
```
Application Configuration -> System Libraries and Add-Ons -> [*] Memory Leakage Checker
```

**Mem Leak Checker requires *arch/arm/include/<chip>/memory_region.h* to work. The selected board must have the memory_region.h file in its folder for the successful build.**

#### HASH_TABLE_SIZE

The Memory Leak Checker uses hash table to improve searching performance.  
CONFIG_DEBUG_MEM_LEAK_CHECKER_HASH_TABLE_SIZE represents the number of hash table indexes.
It has a good performance with big size but spends more memory.

Note: We recommand using a prime number. Default value of that configuration is 1007.

## How to use

This can be run on TASH.
```bash
TASH>> mem_leak [target]
       [target] is used only when APP_BINARY_SEPARATION is enabled.
       [target] is one of kernel, app1 or app2.

Kernel :
*** NO MEMORY LEAK.

Below are text addresses of loadable apps (and common binary if enabled) :
The pc value of the allocation can be obtained by subtracting the text start address of the appropriate binary

[common] Text Addr : 0xe260010, Text Size : 8679424
[app1] Text Addr : 0xeaa7030, Text Size : 4894720

app1 :
Type   |    Addr    | Size(byte) |    Owner   | PID
---------------------------------------------------
LEAK   | 0x63a93fb0 |        16  |  0xe26bf4b | 29
[DATA] 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 31 00 00 00
LEAK   | 0x63a93fd0 |        32  |  0xe26bf57 | 29
[DATA] 48 65 6c 6c 6f 2c 20 57 6f 72 6c 64 32 00 00 00
       00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
*** 2 LEAKS, 0 BROKENS.
```

Here are description of result.

#### Type

There are two types, *LEAK* and *BROKEN*.  

*LEAK* means that *Addr* is a memory leakage and it was allocated by *Owner* with *Size*.  
*BROKEN* means that the given memory address has a wrong value in a chunk. It means memory is corrupted and is abnormal state.

#### Addr and Owner

*Addr* is an address allocated.  
*Owner* is the address of the code segment that requested the memory allocation. 

You can find a function symbol and a line number using *addr2line* command with owner address.
For FLAT_BUILD or kernel, you can use tinyara.elf file.
For APP_BINARY_SEPARATION, you can use app_dbg.elf file.
```bash
$ addr2line -e <ELF_PATH> 0x40d301c

.../TizenRT/apps/examples/hello/hello_main.c:71
```
