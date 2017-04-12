build/output/libraries/ README File
===================================

This directory is reserved for libraries generated during the TinyAra build process

How to add external libraries
=============================

If you want to add pre-built libraries into TinyAra binary,
please refer belows.

1. place your libraries at build/output/libraries folder
2. modify mk files to add them to binary

  < Files >
   FlatLibs.mk for flat build
   ProtectedLibs.mk for protected build
   KernelLibs.mk for kernel build

  < Contents to modify >
   TINYARALIBS += $(LIBRARIES_DIR)$(DELIM)<File name of Library>$(LIBEXT)
   or
   USERLIBS += $(LIBRARIES_DIR)$(DELIM)<File name of Library>$(LIBEXT)
   You should replace <File name of Library> to yours.

3. build the TinyAra
