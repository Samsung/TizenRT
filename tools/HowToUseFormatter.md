# How to use formatter
Here are two formatter (or beautifier) for C and C++ languages, cformatter.sh and cxxformatter.sh.

## Prerequisites
```bash
sudo apt-get install indent astyle
sudo apt-get install clang-format
```

## formatter for C
The **cformatter** script applies TizenRT C coding rule.  
```--help``` option shows how to use it.
```bash
@ubuntu:~/TizenRT$ ./tools/cformatter.sh --help
usage: ./tools/cformatter.sh TARGET
     TARGET: folder path or target files
Apply TizenRT C coding rule

example:
    ./tools/cformatter.sh ../kernel
    ./tools/cformatter.sh ../../abc.c
```
Note: This sometimes makes incorrect changes.  
Usually it adds a space between type casting and variable as shown below:
```
buf = (int *) malloc(8);
size_a = (uint8_t) size_b;
```

## formatter for C++
The **cxxformatter** script applies TizenRT C++ coding rule.  
Type ```cxxformatter.sh --help``` to know the usage.
```bash
@ubuntu:~/TizenRT$ ./tools/cxxformatter.sh --help
usage: ./tools/cxxformatter.sh TARGET
     TARGET: folder path or target files
Apply TizenRT C++ coding rule

example:
    ./tools/cxxformatter.sh kernel/
    ./tools/cxxformatter.sh framework/src/media apps/system
    ./tools/cxxformatter.sh ../../abc.c
```
