# TASH - TinyAra SHell

lightweight shell

## How to enable

Enable configuration of TASH with menuconfig
```bash
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
Select menu
```bash
Application Configuration -> Shell -> [*] Enable shell
```
## How to register a command on TASH

A command can be registered in TASH using two methods, one through calling public API's and
second through adding the commands in makefile.

### Method in code

TASH has two public APIs to install commands :  
* tash_cmd_install  
* tash_cmdlist_install
 
Refer below **Public APIs** [[tab]](README.md#public-apis) to use those functions

### Method in Makefile

When **builtin** functionality is enabled, TASH command can be registered through Makefile.  
This makes application executable on TASH without registering codes.  
Refer **builtin** functionality on [[README]](../builtin/README.md)

## Public APIs

```bash
int tash_cmd_install(const char *str, TASH_CMD_CALLBACK cb, int thread_exec);
void tash_cmdlist_install(const tash_cmdlist_t list[]);
```
The **tash_cmd_install** supports registering one command, the **tash_cmdlist_install** does many commands by list.  
Three arguments are used to register, _str_, _cb_ and _thread\_exec_.  
The _str_ is a command name, the _cb_ is a callback function, the _thread\_exec_ is a execution type of command.  
Two modes of execution:  
> **TASH_EXECMD_SYNC** - invoked callback directly in TASH task  
> **TASH_EXECMD_ASYNC** - execute callback as a separated task

Refer TASH header file, [[tash.h]](../include/shell/tash.h)

#### example

```bash
#include <stdio.h>
#include <apps/shell/tash.h>

int hello_main(int argc, char *argv[])
{
	printf("hello world!\n);
}

void main(void)
{
	tash_cmd_install("hello", hello_main, TASH_EXECMD_ASYNC);
}
```

## Basic commands

#### help
```bash
TASH>>help
         TASH command list
         --------------------
date             exit             free             hello
help             kill             killall          ps
reboot           sh               sleep
```

#### exit
```bash
TASH>>exit
TASH: Good bye!!
```

See all of [shell commands list](https://github.com/Samsung/TizenRT/blob/master/apps/system/utils/README.md) supported.
