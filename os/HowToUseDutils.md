# How to use dutils.sh

Here we explain how to use dutils.sh to run toolchain and trap commands in TizenRT 

## Contents
1. [To run TOOLCHAIN commands](#to-run-toolchain-commands)
2. [To run TRAP tool commands](#to-run-trap-tool-commands) 

## To run TOOLCHAIN commands,


When user has not installed toolchain, but only using the docker container, then there needs to be a method to run toolchain commands like nm, readelf, addr2line for debugging purpose.

The user can call dutils.sh with "tool" command, followed by the actual toolchain command to be run.
```
./dutils.sh tool nm build/output/bin/tinyara.axf
./dutils.sh tool addr2line -e build/output/bin/tinyara.axf 0x0123abcd
```

The toolchain commands can also be run from the dutils as below:

```
1. Go to os/ folder of the TizenRT repository
2. Run ./dutils.sh
3. Press 2
4. Select the desired option
```


### Example
```
test@Focal:~/Desktop/product_Refrigerator/.tizenrt/os$ sudo ./dutils.sh 
Check Docker Image
Docker Image Version : tizenrt/tizenrt:1.5.6
======================================================
  "Select Option"
======================================================
  "1. Run TRAP Debug tool"
  "2. Run Toolchain Command"
  "h. Help"
  "x. Exit"
======================================================
2
make: Entering directory '/home/test/Desktop/product_Refrigerator/.tizenrt/os/tools'
chmod 755 ../makeinfo.sh
make: Leaving directory '/home/test/Desktop/product_Refrigerator/.tizenrt/os/tools'
======================================================
  "Select Binary for Toolchain Command"
======================================================
  "1. Kernel"
  "2. App1"
  "3. App2"
  "4. Common"
  "5. Enter manually"
  "x. Exit"
======================================================
1
======================================================
  "Select Toolchain Command"
======================================================
  "1. nm"
  "2. addr2line"
  "3. Enter manually"
  "x. Exit"
======================================================
2
Enter address: 
0e07844c
Executing: arm-none-eabi-addr2line -e ../build/output/bin/tinyara.axf 0e07844c  
net.c:?
>> Output is stored in dutils_output_addr2line_2024.09.06-14.33.16.txt

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

## To run TRAP tool commands,

We use this to display debug symbol or crashpoint by just giving inputs instead of worrying about which scripts or commands to use 

```
1. copy crash logs, if any, anywhere in the TizenRT repository(Ex: os/crashlogs)
2. Go to os/ folder of the TizenRT repository2. Go to os/ folder of the TizenRT repository
3. Run ./dutils.sh
4. Press 1
5. Select the desired option and enter the desired path
```

### Example

```
test@Focal:~/Desktop/product_Refrigerator/.tizenrt/os$ sudo ./dutils.sh 
Check Docker Image
Docker Image Version : tizenrt/tizenrt:1.5.6
======================================================
  "Select Option"
======================================================
  "1. Run TRAP Debug tool"
  "2. Run Toolchain Command"
  "h. Help"
  "x. Exit"
======================================================
1
=============================================================================
  "Select binary to debug crash with TRAP tool"
=============================================================================
  "1. Default binary from previous build at build/output/bin location"
  "2. Custom binary at any location"
  "x. Exit"
=============================================================================
1
Enter the crash log file name relative to os folder: (ex: ../tools/trap/testlogs)
logdump
make: Entering directory '/home/test/Desktop/product_Refrigerator/.tizenrt/os/tools'
chmod 755 ../makeinfo.sh
make: Leaving directory '/home/test/Desktop/product_Refrigerator/.tizenrt/os/tools'
Executing: ramdumpParser.py -t /root/tizenrt/os/logdump -e tinyara.axf


*************************************************************
dump_file                   : None
log_file                    : /root/tizenrt/os/logdump
Number of binary            : 1 [kernel]
"kernel" elf_file           : ../../build/output/bin/tinyara.axf
*************************************************************

-----------------------------------------------------------------------------------------
1. Crash Binary             : NA

2. Crash type               : etc
   Crash log
	- 
3. Crash point (PC or LR)
	-  PC & LR values not in any text range! No probable crash point detected.

4. Code asserted in:

	- Code asserted in normal thread.

5. Call stack of last run thread

Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
 User thread stack dump
0x6014f3fc	 0xe095e4c	 kernel  	  utils_ttypenames	/root/tizenrt/apps/system/utils/utils_ps.c:94
0x6014f414	 0xe0181b5	 kernel  	  this_task	/root/tizenrt/os/kernel/sched/sched_thistask.c:54
0x6014f418	 0xe039ed5	 kernel  	  fs_getfilep	/root/tizenrt/os/include/tinyara/fs/fs.h:810
0x6014f46c	 0xe095e4c	 kernel  	  utils_ttypenames	/root/tizenrt/apps/system/utils/utils_ps.c:94
0x6014f478	 0xe095e4c	 kernel  	  utils_ttypenames	/root/tizenrt/apps/system/utils/utils_ps.c:94
0x6014f490	 0xe000b31	 kernel  	  up_assert	/root/tizenrt/os/include/assert.h:253
0x6014f494	 0xe000f29	 kernel  	  up_saveusercontext	/root/tizenrt/os/arch/arm/src/armv7-a/arm_saveusercontext.c:64
0x6014f5d8	 0xe000b31	 kernel  	  up_assert	/root/tizenrt/os/include/assert.h:253
0x6014f5dc	 0xe095e4c	 kernel  	  utils_ttypenames	/root/tizenrt/apps/system/utils/utils_ps.c:94
0x6014f5f0	 0xe018b99	 kernel  	  sem_post	/root/tizenrt/os/include/semaphore.h:244
0x6014f610	 0xe095e4c	 kernel  	  utils_ttypenames	/root/tizenrt/apps/system/utils/utils_ps.c:94
0x6014f620	 0xe0276b1	 kernel  	  log_dump_main	/root/tizenrt/apps/examples/log_dump/log_dump_main.c:42
0x6014f938	 0xe018511	 kernel  	  task_activate	/root/tizenrt/os/include/sched.h:253
0x6014fa38	 0xe01856d	 kernel  	  task_start	/root/tizenrt/os/kernel/task/task_start.c:133

h. Heap Region information:

	!!!! NO HEAP CORRUPTION DETECTED !!!!


Checking kernel heap for corruption
Checking corruption (0x60119200 - 0x607fbff0) : Heap NOT corrupted
No Kernel heap corruption detected.


x. Miscellaneous information:
	- SP is out of the stack range. Debug symbols corresponding to the wrong stack pointer addresses are given below:
Stack_address	 Symbol_address	 Symbol location  Symbol_name		File_name
0x607ffed0	 0xe087f16	 kernel  	  __FUNCTION__.5

List of all tasks in the system:

                          NAME |   PID |  PRI |    USED / TOTAL STACK | STACK ALLOC ADDR | TCB ADDR | TASK STATE
-------------------------------------------------------------------------------------------------------------------
                     CPU0 IDLE |     0 |    0 |     692 /    1024 |       0x60118e00 | 0x601052d4 |  Assigned to CPU (Ready)
                     CPU1 IDLE |     1 |    0 |     656 /    1024 |       0x60118a00 | 0x601053a8 |  Running
                        hpwork |     2 |  201 |     436 /    2032 |       0x60122640 | 0x60121c20 |  Wait Signal
                        lpwork |     3 |   50 |     436 /    4080 |       0x60123870 | 0x60122e50 |  Wait Signal
               inic_msg_q_task |     5 |  107 |     436 /    4048 |       0x6012c660 | 0x6012bc40 |  Wait Semaphore
          inic_host_rx_tasklet |     6 |  104 |     492 /    4048 |       0x6012e110 | 0x6012d6f0 |  Wait Semaphore
        inic_ipc_api_host_task |     7 |  103 |     452 /    8136 |       0x6012fb80 | 0x6012f160 |  Wait Semaphore
                   LWIP_TCP/IP |     9 |  105 |     500 /    4072 |       0x60136490 | 0x60135a70 |  Wait Semaphore
          netmgr_event_handler |    10 |  100 |     380 /    2016 |       0x60137ec0 | 0x601374a0 |  Wait Semaphore
                      log_dump |    11 |  200 |    6652 /   16360 |       0x601391c0 | 0x601387a0 |  Wait MQ Receive (MQ Empty)
                       appinit |    12 |  101 |     544 /    2032 |       0x60140d20 | 0x6013d1d0 |  Wait Semaphore
                binary_manager |    13 |  203 |     876 /    8168 |       0x60141f50 | 0x60141530 |  Wait MQ Receive (MQ Empty)
                          tash |    15 |  125 |     688 /    4080 |       0x60149990 | 0x60145230 |  Wait Semaphore
              wifi msg handler |    16 |  100 |     524 /    4064 |       0x6014b1e0 | 0x601256a0 |  Wait Semaphore
               ble msg handler |    17 |  100 |     524 /    4072 |       0x6014ca30 | 0x601257e0 |  Wait Semaphore
                      log_dump |    18 |  100 |    1680 /    8168 |       0x6014da40 | 0x60144e70 |  Running
-------------------------------------------------------------------------------------------------------------------
>> Output is stored in dutils_output_trap_2024.09.06-16.24.56.txt

```

