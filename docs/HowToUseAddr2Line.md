# How to use Addr2Line in Linux
**addr2line** in linux converts the address into file names and line numbers.  
Developers can debug the TizenRT ELF/AXF output file through **addr2line**.

## Contents
- [Prerequisite](#prerequisite)  
- [How to use addr2line](#how-to-use-addr2line)

## Prerequisite
To use addr2line, please enable below config.  

In menuconfig, enable **CONFIG_DEBUG_SYMBOLS**.  
	```
	Debug Options -> Generate Debug Symbols
	```

## How to use addr2line
After build, use **addr2line** from linux shell.  
Please refer to linux man page.  
(https://man7.org/linux/man-pages/man1/addr2line.1.html)  
Simple command is like below.  
```
$ addr2line -e [filepath] -a [address]
```

### How to use in Binary Separation
When binary separation, the address of text section in user binary starts from 0x0 for relocation.  
After relocation, its real address is allocated and those address can be used for debugging.  
For this, some steps are required like below.  

1. Enable CONFIG_SAVE_BIN_SECTION_ADDR  
	```
	Binary Loader -> Save binary section address
	```

2. Check the user binary's text address from the printed log.  

3. Calculate the address based on No.2.  
For example, let assume that user binary text address is 0x1000000, and symbol address ix 0x200000.  
Then add 0x1000000 and 0x200000 and pass this address for -a option's parameter.
	```
	$ addr2line -e [filepath] -a 0x1200000
	```
