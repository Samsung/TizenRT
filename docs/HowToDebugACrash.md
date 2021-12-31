# How to use Addr2Line tool for debugging a crash
**addr2line** is a tool which comes with the Toolchain in Linux.
It converts the given address into file names and line numbers.  
Developers can debug the TizenRT ELF/AXF output file through **addr2line**.
It is especially helpful while debugging a crash and can be used to find the 
exact line number where the crash occurred and also the function callstack
which resulted in the crash.

## Contents
- [Prerequisites](#prerequisites)  
- [How to use addr2line](#how-to-use-addr2line)
- [How to debug a crash with addr2line](#how-to-debug-a-crash-with-addr2line)
- [Example debugging scenario](#Example-debugging-scenario)
- [Understanding Debug messages during a fault](#Understanding-debug-messages-during-a-fault)

## Prerequisites
1. To use addr2line, please enable below config.  

In menuconfig, enable **CONFIG_DEBUG_SYMBOLS**.

```
menuconfig -> Debug Options -> Generate Debug Symbols
```

2. Add your toolchain to the system path
It is easy to invoke addr2line if the toolchain has been included in the
system path using the PATH environment variable.
For example, if your ARM toolchain is located at :

```
/home/tools/gcc-arm-none-eabi-9-2019-q4-major
```
Then, your PATH variable must include the toolchain bin folder as follows :

```
/home/tools/gcc-arm-none-eabi-9-2019-q4-major/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
```

3. Enable stackdump
When a crash occurs in the system, it is useful to find out where exactly the crash occurred and also
the function callstack which resulted in the crash. In order to find these, we need to print the corresponding
address values when the crash occurs. This can be done by enabling the following configs:

CONFIG_ARCH_STACKDUMP
```
menuconfig -> Chip selection -> Dump stack on assertions
```

## How to use addr2line
After build, use **addr2line** from Linux shell.  
Please refer to Linux man page.  
(https://man7.org/linux/man-pages/man1/addr2line.1.html)  
Simple command is like below.  
```
$ addr2line -e [filepath] -a [address]
```

However, if the path to your toolchain is not configured as mentioned above, or if there are multiple toolchains installed on your system,
then you might have to specify the correct version of the addr2line to be used. It can be done as follows:

Below command specifies that ARM toolchain is to be used.
For this to work, the ARM toolchain needs to be configured in the PATH:
```
$ arm-none-eabi-addr2line -e [filepath] -a [address]
```

However, if the toolchain path is not configured, or if you have multiple versions of toolchain, then you may specify the complete path to the
required addr2line tool as follows:
```
$ /home/tools/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-addr2line -e [filepath] -a [address]
```

### How to use in Binary Separation
When binary separation is enabled, the address of text section in user binary starts from 0x0 for relocation.  
After relocation, its real address is allocated and those address can be used for debugging.  
For this, some steps are required like below.  

1. Enable these configs:

	CONFIG_SAVE_BIN_SECTION_ADDR  
	```
	menuconfig -> Binary Loader -> Save binary section address
	```

	CONFIG_DEBUG_BINFMT_INFO
	```
	menuconfig -> Debug Options -> Enable Debug Features -> Binary Loader Debug Features -> Binary Loader Informational Output
	```

2. Check the user binary's text address from the printed log.  

3. Calculate the address based on No.2.  
For example, let assume that user binary text address is 0x1000000, and symbol address ix 0x1200000.  
Then subtract 0x1000000 from 0x1200000 and pass this address for -a option's parameter.
	```
	$ addr2line -e [filepath] -a 0x200000
	```

## How to debug a crash with addr2line
When a crash occurs in the system, and if the above configs are enabled, then the system prints the stackdump for the currently
running task in which the crash occurred and also the register dump showing the contents of the CPU registers at the time of crash.
You can then use the addr2line command as stated above to convert the printed address values into source file line numbers and the
following debug information can be obtained.

1. Location where the crash occurred
This can be found from the register dump. The register dump contains the values of R0 to R15 registers. The R15 register gives the
PC (Program Counter) value. This address can be used with the addr2line command to find the exact line number in the source code
which caused the crash.

2. Callstack during the crash
The callstack can be found from the address values printed in the stack dump. However, the stack dump contains both valid address and
other HEX values. There is no way to determine the exact locations in the stack dump where the callstack function addresses are stored.
So, we need to manually find out the probable address values from the stack dump. This can be done by knowing the address where the
text sections are loaded in memory. This information is generally present in the linker scripts for the particular project or the app
specific linker script.

As an example, suppose you find from the linker script that the text section is loaded between addresses 0x80000000 to 0x84000000.
Now, scan the stack dump for any values in this address range. The first line of the stackdump indicates the most recent function
at the time of crash  and the last line represents the oldest function in the callstack. For each valid address of the above range
in the stackdump, use the addr2line tool to find the source code line number. Thus, you can reconstruct the call stack at the time of crash.

## Example debugging scenario
Here you can see an example scenario when a crash has occurred. Since the stackdump is enabled, the system will print the stack and
register values. It is seen from the linker script that the text section is loaded in the address range 0x0E000000 to 0x10000000. In the
stack dump shown below, we will select all values that fall in this range and use the addr2line tool to obtain the callstack as shown.

```
up_stackdump: 10064140: 00000003 10064150 0e006575 0e051b28 0e051d3c 00000fe4 00000066 10062d38
up_stackdump: 10064160: 00000000 100202b4 00000000 100202b4 00000000 100202b4 00000000 100202b4
up_stackdump: 10064180: 100641c0 00000005 00000000 10062c10 00000001 100643f4 00000000 0e001571
up_stackdump: 100641a0: 0e00155d 10011f55 000000e0 10064380 00000005 00000000 00000001 10011ce1         
up_stackdump: 100641c0: 10064210 000000e0 10064380 00000005 00000000 00000001 10062c10 00000001         
up_stackdump: 100641e0: 100643f4 00000000 ffffffb8 10063370 10062d10 00000002 0210c840 00000000         
up_stackdump: 10064200: 0e001d53 0e0119a3 0e006c40 61000000 00000001 00000001 00000032 00000000         
up_stackdump: 10064220: 00000000 00000000 100642cc 10064380 00000000 00000001 100642cc 00000000
up_stackdump: 10064240: 0e0513de 0e0513e8 00000000 0e0111d3 013414d5 00000001 1001fa68 1001fa60
up_stackdump: 10064260: 00000000 0e002281 1001fa68 013414d5 6f63696d 0000006d 00000000 00000000
up_stackdump: 10064280: 00003ce8 0000c800 00001000 00dc0032 013414d5 00000000 90503df7 0001002e
up_stackdump: 100642a0: 3ce803dc 696d0000 006d6f63 00000000 00000000 14d50000 c8000134 10000000
up_stackdump: 100642c0: 00000000 00004000 2e940000 746e6d2f 6e69622f 696d2f73 5f6d6f63 39313032
up_stackdump: 100642e0: 31323430 29c00300 c112e7b8 f7b806f4 f8ddae0a 3128bb94 3c5c97ee 0834d974
up_stackdump: 10064300: cfeddb5f 824d64e7 649ae971 6fc9b138 00000000 00000001 1001fa60 00000002
up_stackdump: 10064320: 00000000 00000000 00000000 0e0024d7 00000002 0e002495 00000000 00000000
up_stackdump: 10064340: 00000000 0e001b53 00000000 00000000 3306f747 10064360 1006436a 00000000
up_registerdump: R0: 10062d10 00000002 0210c840 00000000 10064380 00000005 00000000 00000001
up_registerdump: R8: 10062c10 00000001 100643f4 00000000 0e001d53 10064210 0e0119a3 0e006c40

Location of crash from PC value:
================================
0e006c40	os/arch/arm/src/armv8-m/up_mpu.c:152

Callstack from values in stackdump
==================================
0e006575	os/arch/arm/src/armv8-m/up_assert.c:362
0e001571	os/kernel/irq/irq_unexpectedisr.c:104
0e00155d	os/kernel/irq/irq_unexpectedisr.c:99
0e001d53	os/kernel/semaphore/sem_holder.c:172
0e0119a3	os/binfmt/binfmt_execmodule.c:199
0e0111d3	os/binfmt/binfmt_loadbinary.c:215
0e002281	os/kernel/binary_manager/binary_manager_load.c:109
0e0024d7	os/kernel/binary_manager/binary_manager_load.c:415
0e002495	os/kernel/binary_manager/binary_manager_load.c:392
0e001b53	os/kernel/task/task_start.c:180
```

## Understanding Debug messages during a fault
A crash can occur due to any one of the faults, such as, memory fault, usage fault, hard fault, etc.. When a fault occurs, the hardware encodes the cause of the fault in a corresponding fault status register. The fault handler prints the contents of this register in hexadecimal format and also provides a brief description for the cause of the fault. The following sections provide additional information and tips to understand and debug each of the fault scenarios. Here we discuss the various fault scenarios encoded in the MMFSR, BFSR, UFSR and HFSR registers for memfault, busfault, usage fault and hard fault respectively. Please refer to the ARM Architecture reference manual of the corresponding architecture to find the description of these registers and their bit fields.

Armv7-m : https://developer.arm.com/documentation/ddi0403/eb/

Armv8-m : https://developer.arm.com/documentation/ddi0553/latest

### MemManage Fault Status Register (MMFSR)

1. ```MMARVALID```

Debug msg: Access violation occurred at address (MMFAR) : 0xAddress

This message indicates that the address which caused the memfault is available to us. Check the printed address value. If the address is zero, it is probably caused due to a null pointer access. If the address value is non-zero, then the fault can be caused due to one of the following reasons. 
- The address is outside of the configured MPU regions
- The MPU region configuration does not match the access type. For example, trying to write to a read-only region


Debug msg: Unable to determine access violation address.

This message indicates that the hardware was not able to find the address which caused the fault. This generally happens in case of an instruction access violation. However, the debug logs show the values of PC and LR register at the time of crash. The PC register would contain an invalid address. The developer needs to debug further to find the root cause of the crash. The following guidelines may be used:
- If the PC value seems to be a valid address, then check whether it falls within a valid MPU region (with execute permission)
- If PC value seems to be totally invalid, then we need to find all locations in the code, which can cause the PC value to get changed in a non sequential manner. It could be either due to an invalid function pointer or an invalid value filled into the PC during a context switch or irq handling.
- The developer may use the LR value to find the previous function, and then manually walk through the code to find the probable cause for PC corruption.


In both the above cases, the developer can use the addr2line tool along with the PC and LR values to find the line number in code, where the error has occurred. Please note the following points:
- The LR value points to the next instruction to be executed after return from a function call. So, the LR will point to a line number after a function in which the fault would have occurred.
- The LR and PC might not contain exact line numbers if the function was declared as "inline", or if some compiler optimization has caused the function to be inlined.


2. ```DACCVIOL```

Debug msg: Data access violation occurred.

Use the PC value and the address printed in the debug message to find which variable is causing an invalid data access as mentioned in point (1) above.


3. ```IACCVIOL```

Debug msg: Instruction access violation occurred while fetching instruction from an Execute Never (XN) region.

Execution from an MPU region with no execute permission. Check the line number of PC and make sure that corresponding MPU region has the proper privileges. 


4. ```MSTKERR``` and ```MUNSTKERR```

Debug msg:
Error while stacking registers during exception entry.
or
Error while unstacking registers during exception return.

In both these cases, the data access violation has occurred during stack handling. So, check the stack pointer value and find out if it is within the stack. Also, stack handling happens either in case of function call / return and in case of exception handling. So, developer might need to check the sanity of exception handling code.


5. ```MLSPERR```

Debug msg: Error occurred during lazy state preservation of Floating Point unit registers.

This bit also indicates an error during stacking of registers during exception handling.


### Bus Fault Status Register (BFSR)
A bus fault is very similar to memory management fault as mentioned above. Any access of data or instructions from an invalid memory address can cause a bus fault. If the MPU is enabled, then most of such invalid accesses will result in a corresponding memfault. However, if the MPU is disabled, all invalid memory accesses result in bus fault. Bus faults can either be data abort or prefetch abort, based on whether the fault occurred while accessing data or instructions. The BFARVALID, STKERR, UNSTKERR, LSPERR fields of this register are similar to the corresponding bits in the memfault scenario. The IBUSERR bit indicates an instruction access fault; whereas, the PRECISERR and IMPRECISERR bits indicate a data access violation. The PRECISERR is similar to DACCVIOL in the memfault register.

The only difference lies in the IMPRECISERR bit. This bit is set if an imprecise data access violation has occurred. It means that the current PC value does NOT point to the instruction which caused the data access violation. The violation is caused by some previous instruction, but was delayed because memory accesses are not always synchronous. In order to debug this type of fault, the developer can check the previous few lines of code starting from the current PC value. The developer can also use the objdump tool to print the disassembly of the code at the current PC value and then check each assembly instruction which is either loading or storing values into the memory.


### Usage Fault Status Register (UFSR)

1. ```DIVBYZERO```

Indicates divide by zero error.

2. ```UNALIGNED```

Indicates unaligned memory access.

3. ```UNDEFINSTR```

Indicates undefined instruction execution.

4. ```STKOF```

Indicates stack overflow.

5. ```NOCP```

Indicates co-processor is disabled.

6. ```INVPC```

This bit is set if the integrity check fails during an exception return. It can happen due to any one of the following reasons:
- If exception number held in IPSR register does NOT match the current exception (corresponding bit set in SHCSR register).
- If bit[1] of EXC_RETURN is set to (1). EXC_RETURN is the value in R14 at the time of exception return.
- If returning to thread mode and IPSR is not restored to (0).
- If returning to handler mode and IPSR is not restored to non-zero.
The developer has to check any code which might result in one of the above conditions.

7. ```INVSTATE```

The bit[0] of PC register must always be set to (1). If this condition is not met, then INVSTATE exception is generated. In order to debug this fault, the developer needs to check any instruction which could load the PC with an invalid value.


### Hard Fault Status Register (HFSR)
The most common cause of a hard fault is escalation of other faults. This happens when a fault condition occurs, but the corresponding fault is not implemented by the hardware or not enabled. For example, if a memory access violation occurs and the memfault and busfault are not enabled, then it gets escalated to hard fault. If the hard fault is caused by such an escalation, then the FORCED bit is set to (1) in the HFSR. The hard fault handler prints the value of all fault registers mentioned above. The developer can find which fault had occurred from these register values and then perform debugging according to the above mentioned guidelines for the corresponding fault scenario.
