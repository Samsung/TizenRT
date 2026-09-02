# How to use Memory Leak Checker

The Memory Leak Checker shows leakage information of dynamically allocated memory at that time.  
It checks the entire RAM regions for an allocated memory address. If no region has that address,  
it is a leakage because it can not be released.  
This document describes how to enable and use the Leak Checker.

## How to enable

1. Enable *CONFIG_DEBUG_MM_HEAPINFO* using menuconfig as shown below:
```
Debug Options -> Enable Debug Output Features -> [*] Heap Info debug option
```
2. Enable *CONFIG_MEM_LEAK_CHECKER* and set configuration values using menuconfig as shown below:
```
Application Configuration -> System Libraries and Add-Ons -> [*] Memory Leak Checker (Mark-and-Sweep)
```

**Mem Leak Checker requires *arch/arm/include/<chip>/memory_region.h* to work. The selected board must have the memory_region.h file in its folder for successful build.**

### Configuration Options

#### CONFIG_MEM_LEAK_CHECKER_HASH_SIZE

The Memory Leak Checker uses a hash table to improve searching performance.  
CONFIG_MEM_LEAK_CHECKER_HASH_SIZE represents the number of hash table entries.  
It has better performance with a larger size but uses more memory.

Note: We recommend using a prime number. Default value is 1007.

#### CONFIG_MEM_LEAK_CHECKER_MAX_BLOCKS

Maximum number of memory blocks that can be tracked. Must be larger than the total  
number of memory allocations in your system. Default value is 3000.

#### CONFIG_MEM_LEAK_CHECKER_WORKLIST_SIZE

Size of the worklist for transitive marking. Should be >= MAX_BLOCKS.  
Default value is 3000.

## How to use

This can be run on TASH.

### Command Syntax

```bash
TASH>> mem_leak [--heap=<name>] [--pid=<value>] [--help]
```

### Options

| Option | Description |
|--------|-------------|
| `--heap=<name>` | Check only the specified heap (default: all heaps)<br>Valid names: `kernel`, `app1`, `app2`, ... |
| `--pid=<value>` | Filter detailed leak output by PID:<br>`0` = summary only (default)<br>`-1` = show details for ALL PIDs<br>`N` = show details for PID N only |
| `--help`, `-h` | Display usage information |

### Examples

```bash
# Check all heaps, summary only (default)
TASH>> mem_leak

# Check kernel heap only
TASH>> mem_leak --heap=kernel

# Check app1 heap, show all PID details
TASH>> mem_leak --heap=app1 --pid=-1

# Check kernel heap, show details for PID 5 only
TASH>> mem_leak --heap=kernel --pid=5
```

### Sample Output

#### No Leaks Found

```bash
TASH>> mem_leak

Kernel :
*** NO MEMORY LEAK. (6499 blocks reachable, 0 broken)

Below are text addresses of loadable apps (and common binary if enabled) :
The pc value of the allocation can be obtained by subtracting the text start address of the appropriate binary

[common] Text Addr : 0xe161010, Text Size : 9728000
[app1] Text Addr : 0xeaa8030, Text Size : 4894720

app1 :
*** NO MEMORY LEAK. (26225 blocks reachable, 0 broken)

Memory leak checker execution time: 1523 ms
```

#### Leaks Found

```bash
TASH>> mem_leak --pid=-1

Kernel :
Type   |    Addr    | Size(byte) |    Owner   | PID  | Status
----------------------------------------------------------------
LEAK   | 0x60e31b60 |        16  |  0xe58bea5 |   57 | UNREACHABLE
[DATA] 20 5a e8 60 28 b9 d8 60 c5 95 16 0e 35 00 00 00
LEAK   | 0x60e80bc0 |        32  |  0xe58bea5 |   57 | UNREACHABLE
[DATA] 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
LEAK   | 0x60e80c40 |        16  |  0xe58bea5 |   57 | UNREACHABLE
[DATA] c0 0b e8 60 b0 0b e8 60 c5 95 16 0e 39 00 aa 96
*** 3 LEAKS (UNREACHABLE), 26225 reachable, 0 broken.

Memory leak checker execution time: 1847 ms
```

### Output Description

#### Type

There are two types: *LEAK* and *BROKEN*.

*LEAK* means that *Addr* is a memory leak and it was allocated by *Owner* with *Size*.  
*BROKEN* means that the given memory address has a wrong value in a chunk. It means memory is corrupted and is an abnormal state.

#### Addr and Owner

*Addr* is an allocated address.  
*Owner* is the address of the code segment that requested the memory allocation.

You can find a function symbol and a line number using the *addr2line* command with the owner address.  
For FLAT_BUILD or kernel, you can use tinyara.elf file.  
For APP_BINARY_SEPARATION, you can use app_dbg.elf file.

```bash
$ addr2line -e <ELF_PATH> 0x40d301c

.../TizenRT/apps/examples/hello/hello_main.c:71
```

**Note:** For APP_BINARY_SEPARATION, the pc value shown in the output is an offset from the text base.  
You need to subtract the text start address (shown in multi-heap output) to get the actual owner address.

#### Status

- *UNREACHABLE*: Block is not reachable from any true root (BSS, DATA, or stack). This is a memory leak.
- *REACHABLE*: Block is reachable from a true root. Not a leak.

#### [DATA]

Shows the first 16 bytes of the leaked block's content in hexadecimal.  
Useful for debugging to understand what the leaked memory contains.

