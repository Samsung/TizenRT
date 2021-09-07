# TizenRT Debug Analyser Tool(DAT)
The Debug Analyser Tool will help the developer for dynamic debugging.  
It will show some internal system information such as:  
	> Alive Thread List  
	> Scheduling Lists  
	> Loading List  
	> Heap Usages  

## Contents
> [Prerequisites](#prerequisites)  
> [Build Steps](#how-to-enable-DAT)  
> [Brief Module Information](#module-information)
> [Usage Guide](#how-to-use-DAT)  

## Prerequisites
Install python3 and pynput, pyserial(at pip3)

## How to enable DAT
Below configuration must be enabled to support dump upload
1. Enable DAT application (CONFIG_SYSTEM_DEBUG_ANALYSER_TOOL=y)
```
Application Configuration > System Libraries and Add-Ons > Debug Analyser Tool App
```

## Module Information
Here we show how to actually use DAT for dynamic debugging.  
It has the following menu tree:
```
	> Alive Thread List
		> Alive Thread List
		  (List of alive kernel/application threads in the system and their respective information such as name, priority, task status, stack range, current sp location, stack peak, etc)
		> GtaskList Table
		  (A list of all global task lists and the tasks present in them such as g_readytorun, g_pendingtasks, g_waitingforsemaphore, g_waitingforsignal, g_waitingformqnotempty, etc)
		> Alive List Call Stack
		  (Displays the call stack of the alive threads respectively)
	> Scheduling Lists
		> Task List
		  (Displays the task scheduling history)
		> IRQ List
		  (Displays the irq scheduling history)
		> Semaphore List
		  (Displays the semaphore scheduling history)
		> Task + IRQ List
		  (Displays the task and irq scheduling history in sequential order of occurence)
	> Loading List
		> No. of Binaries
		  (Number of loaded binaries on the device)
		> Memory addresses (text, ro, bss, data)
		  (Display text, ro, bss, data memory address and text size of loaded applications)
		> Application Runtimes
		  (Run times of loaded application since app launch)
		> Heap Info
		  (Heap usages for kernel/app1/app2)
	> Heap Usages
		> Kernal
		  (Kernel heap usages)
		> APP 1
		  (Application 1 heap usages)
		> APP 2
		  (Application 2 heap usages)
```
It also supports a `Custom Input` tab to extract information other than the above tabs. Any command supported in TASH can be used in this input box.

## How to use DAT
To run the tool, first enable the tool [Build Steps](#how-to-enable-DAT).  
Then run the following command:  
	$ python3 dat.py <device port>
```
ex)
$ python3 dat.py /dev/ttyUSB1
```
You can select any module for respective module information [Brief Module Information](#module-information). It also supports a `Custom Input` tab to extract information other than the above tabs. Any command supported in TASH can be used in this input box.  
In case of any disabled menu, a prompt will be shown as below:
```
xxxx Command is not registered
Enable appropriate command on the device and retry!
```
For this error you need to enable the respective module's CONFIG so as to get dynamic data for it.  
All the command outputs are stored in a file named `cmd_history.txt`. The current command output is stored in `cmd_output.txt`.
