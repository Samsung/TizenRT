# How to use global variable profiler
The *g_var_profiler.sh* script profiles static memory usages.  
It generates the list of global variables which have greater size than given.

# How to use
Please find the usages with *-h* option.
```bash
tools/memory$ ./g_var_profiler.sh -h
Usages: ./g_var_profiler.sh [-s SIZE] [-e ELF] [-o OUTPUT_FILENAME]
This script parses the global variable which size is greater than [SIZE]
	If no input params are specified, below is assumed
	SIZE : 256
	ELF : ../../build/output/bin/tinyara
	OUTPUT_FILENAME : var_list_over_SIZEbytes.txt
Output file info
	SIZE : variable size(bytes)
	TYPE : you can refer to this site <https://sourceware.org/binutils/docs/binutils/nm.html>
```
All of options are optional. When they are not specified, default values are used as explained in help.

# Example
```bash
tools/memory$ ./g_var_profiler.sh -s 1024
Size   : 1024
ELF    : ../../build/output/bin/tinyara
Output : var_list_over_1024bytes.txt
var_list_over_1024bytes.txt is generated.

```
The output file shows the list as below.
```txt
SIZE(bytes)	TYPE	VARIABLE		PATH
------------------------------------------------------------
1024		B	log_buffer /root/tizenrt/os/arch/arm/src/chip/imxrt_log.c:47		
1088		b	dns_table /root/tizenrt/os/net/lwip/src/core/dns.c:318		
1664		B	g_irqvector		
2560		b	g_qtdpool /root/tizenrt/os/arch/arm/src/chip/imxrt_ehci.c:592		
4096		b	g_framelist /root/tizenrt/os/arch/arm/src/chip/imxrt_ehci.c:574		
5120		b	g_qhpool /root/tizenrt/os/arch/arm/src/chip/imxrt_ehci.c:587		
```
