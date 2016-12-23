Architecture-Specific Code
^^^^^^^^^^^^^^^^^^^^^^^^^^
Table of Contents
^^^^^^^^^^^^^^^^^

  o Architecture-Specific Code
  o Summary of Files
  o Supported Architectures
  o Configuring TinyAra

Architecture-Specific Code
^^^^^^^^^^^^^^^^^^^^^^^^^^

The TinyAra configuration consists of:

o Processor architecture specific files.  These are the files contained
  in the arch/<arch-name>/ directory discussed in this README.

o Chip/SoC specific files.  Each processor processor architecture
  is embedded in chip or System-on-a-Chip (SoC) architecture.  The
  full chip architecture includes the processor architecture plus
  chip-specific interrupt logic, general purpose I/O (GIO) logic, and
  specialized, internal peripherals (such as UARTs, USB, etc.).

  These chip-specific files are contained within chip-specific
  sub-directories in the arch/<arch-name>/ directory and are selected
  via the CONFIG_ARCH_name selection

o Board specific files.  In order to be usable, the chip must be
  contained in a board environment.  The board configuration defines
  additional properties of the board including such things as
  peripheral LEDs, external peripherals (such as network, USB, etc.).

  These board-specific configuration files can be found in the
  configs/<board-name>/ sub-directories.

This README will address the processor architecture specific files
that are contained in the arch/<arch-name>/ directory. The file
include/tinyara/arch.h identifies all of the APIs that must
be provided by this architecture specific logic.  (It also includes
arch/<arch-name>/arch.h as described below).

Directory Structure
^^^^^^^^^^^^^^^^^^^

The arch directory contains architecture specific logic.  The complete
board port in is defined by the architecture-specific code in this
directory (plus the board-specific configurations in the config/
subdirectory).  Each architecture must provide a subdirectory <arch-name>
under arch/ with the following characteristics:


        <arch-name>/
        |-- include/
        |   |--<chip-name>/
        |   |  `-- (chip-specific header files)
        |   |--<other-chips>/
        |   |-- arch.h
        |   |-- irq.h
        |   `-- types.h
        `-- src/
            |--<chip-name>/
            |  `-- (chip-specific source files)
            |--<other-chips>/
            |--<board-name>/
            |  `-- (board-specific source files)
            |--<other-boards>/
            |-- Makefile
            `-- (architecture-specific source files)

Summary of Files
^^^^^^^^^^^^^^^^

include/<chip-name>/
  This sub-directory contains chip-specific header files.

include/arch.h
  This is a hook for any architecture specific definitions that may
  be needed by the system.  It is included by include/tinyara/arch.h

include/types.h
  This provides architecture/toolchain-specific definitions for
  standard types.  This file should typedef:

    _int8_t, _uint8_t, _int16_t, _uint16_t, _int32_t, _uint32_t

  and if the architecture supports 64-bit integers.

    _int24_t, _uint24_t, int64_t, uint64_t

  NOTE that these type names have a leading underscore character.  This
  file will be included(indirectly) by include/stdint.h and typedef'ed to
  the final name without the underscore character.  This roundabout way of
  doings things allows the stdint.h to be removed from the include/
  directory in the event that the user prefers to use the definitions
  provided by their toolchain header files

    irqstate_t

  Must be defined to the be the size required to hold the interrupt
  enable/disable state.

  This file will be included by include/sys/types.h and be made
  available to all files.

include/irq.h
  This file needs to define some architecture specific functions (usually
  inline if the compiler supports inlining) and structure.  These include:

  - struct xcptcontext.  This structures represents the saved context
    of a thread.

  - irqstate_t irqsave(void) -- Used to disable all interrupts.

  - void irqrestore(irqstate_t flags) -- Used to restore interrupt
    enables to the same state as before irqsave was called.

  This file must also define NR_IRQS, the total number of IRQs supported
  by the board.

src/<chip-name>/
  This sub-directory contains chip-specific source files.

src/Makefile
  This makefile will be executed to build the targets src/libup.a and
  src/up_head.o.  The up_head.o file holds the entry point into the system
  (power-on reset entry point, for example).  It will be used in
  the final link with libup.a and other system archives to generate the
  final executable.

Supported Architectures
^^^^^^^^^^^^^^^^^^^^^^^

arch/arm - ARM-based micro-controllers
    This directory holds common ARM architectures.  At present, this includes
    the following subdirectories:

    Architecture Support
      arch/arm/include and arch/arm/src/common
      arch/arm/src/arm and arch/arm/include/arm
      arch/arm/src/armv7-m and arch/arm/include/armv7-m
      arch/arm/src/armv7-r and arch/arm/include/armv7-r
