# How to clean TizenRT

There are two types of commands on Makefile to clean all of made files.  
One is ```make clean```, another is ```make distclean```. They are executed on *os* folder.

## make clean
This command removes files which are made on build procedure like objects, libraries, .depend, Make.dep and etc.  
When configuration is changed, executing this guarantees a re-build from scratch.

## make distclean
This includes ```make clean``` inside. Additionally this removes configured files like .config and Make.defs.  
When you want to use new or another defconfig, this should be executed before configuring new one.
