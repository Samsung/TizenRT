## C++ Support in TizenRT
======================

TizenRT provides two levels of C++ support:

1. Minimal C++ runtime (without LLVM)
2. Full C++ support using LLVM + libc++ or below libraries
 - libstdc++  (part of GCC)
 - STLport    http://www.stlport.org/
 - uClibc++   http://cxx.uclibc.org/
 - uSTL       http://ustl.sourceforge.net/


This directory (lib/libxx) contains the minimal runtime used when LLVM is not enabled.
It will allow to build only the simplest of C++ applications

--------------------------------------------------
##Minimal C++ Support (without LLVM)
--------------------------------------------------

When LLVM is disabled, TizenRT uses the lightweight runtime in:

    lib/libxx

This mode is intended for memory-constrained embedded targets.

Supported features:
- Classes and objects
- Constructors / destructors
- new / delete
- Static initialization
- pure virtual handler

Limitations:
- No full STL
- No iostream/alogorithms
- Full exception handling
- Partial C++11 support only

Use this mode when:
- LLVM footprint is too large
- Only basic C++ language features are required

--------------------------------------------------
## How to Enable C++ in TizenRT (CONFIG_HAVE_CXX)
--------------------------------------------------

Run:
    ./dbuild.sh menuconfig

Enable:
    To enable minimal C++ support:
    	Built-in Libraries -->
		Have C++ compiler 

--------------------------------------------------
## How to Select C++ version in TizenRT (CONFIG_CXX_VERSION_XX)
--------------------------------------------------

Run:
    ./dbuild.sh menuconfig

Enable:
    Built-in Libraries -->
	c++version -->
		c++11/c++14/c++17

--------------------------------------------------
## Hello World C++ Example (CONFIG_EXAMPLES_HELLOXX)
--------------------------------------------------

Run:
    ./dbuild.sh menuconfig

Enable:
    Application Configuration -->
	Examples -->
		 "Hello, World!" C++ example "

--------------------------------------------------
## Notes
--------------------------------------------------

- lib/libxx provides only minimal C++ runtime
- Full STL and modern C++ features require LLVM + libc++
- libc++ 4.0 provides partial C++17 support
- See external/libcxx/README.md for LLVM details
