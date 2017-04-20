Board-Specific Configurations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Table of Contents
^^^^^^^^^^^^^^^^^

  o Board-Specific Configurations
  o Summary of Files
  o Configuration Variables
  o Supported Boards
  o Configuring TinyAra
  o Building Symbol Tables

Board-Specific Configurations
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The TinyAra configuration consists of:

o Processor architecture specific files.  These are the files contained
  in the arch/<arch-name>/ directory.

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
  configs/<board-name>/ sub-directories and are discussed in this
  README.  Additional configuration information maybe available in
  board-specific configs/<board-name>/README.txt files.

The configs/ subdirectory contains configuration data for each board.  These
board-specific configurations plus the architecture-specific configurations in
the arch/ subdirectory completely define a customized port of TinyAra.

Directory Structure
^^^^^^^^^^^^^^^^^^^

The configs directory contains board specific configurationlogic.  Each
board must provide a subdirectory <board-name> under configs/ with the
following characteristics:


  <board-name>
  |-- README.txt
  |-- scripts/
  |   |-- setenv.sh
  |   `-- .ld
  |-- <config1-dir>
  |   |-- Make.defs
  |   `-- defconfig
  |-- <config2-dir>
  |   |-- Make.defs
  |   `-- defconfig
  ...

Summary of Files
^^^^^^^^^^^^^^^^

README.txt -- This text file provides additional information unique to
  each board configuration sub-directory.

scripts/ -- This directory contains board specific scripts.

setenv.sh -- This is a script that you can include that will be installed at
  the toplevel of the directory structure and can be sourced to set any
  necessary environment variables.  You will most likely have to customize the
  default setenv.sh script in order for it to work correctly in your
  environment.

Each board configuration is described by two files:  Make.defs,
defconfig.  Typically, each set of configuration files is
retained in a separate configuration sub-directory (<config1-dir>,
<config2-dir>, .. in the above diagram).

Make.defs -- This makefile fragment provides architecture and
  tool-specific build options.  It will be included by all other
  makefiles in the build (once it is installed).  This make fragment
  should define:

    Tools: CC, LD, AR, NM, OBJCOPY, OBJDUMP
    Tool options: CFLAGS, LDFLAGS

  When this makefile fragment runs, it will be passed TOPDIR which
  is the path to the root directory of the build.  This makefile
  fragment should include:

    $(TOPDIR)/.config          : TinyAra configuration
    $(TOPDIR)/tools/Config.mk  : Common definitions

  Definitions in the Make.defs file probably depend on some of the
  settings in the .config file.  For example, the CFLAGS will most likely be
  different if CONFIG_DEBUG=y.

  The included tools/Config.mk file contains additional definitions that may
  be overriden in the architecture-specific Make.defs file as necessary:

    COMPILE, ASSEMBLE, ARCHIVE, CLEAN, and MKDEP macros

defconfig -- This is a configuration file similar to the Linux
  configuration file.  In contains variable/value pairs like:

  CONFIG_VARIABLE=value

  This configuration file will be used at build time:

    (1) as a makefile fragment included in other makefiles, and
    (2) to generate include/tinyara/config.h which is included by
        most C files in the system.

Configuration Variables
^^^^^^^^^^^^^^^^^^^^^^^

At one time, this section provided a list of all Tinyara configuration
variables. However, Tinyara has since converted to use the kconfig-frontends
tools (See http://ymorin.is-a-geek.org/projects/kconfig-frontends).  Now,
the TinyAra configuration is determined by a self-documenting set of Kconfig
files.

The current TinyAra configuration variables are also documented in separate,
auto-generated configuration variable document.  That configuration variable
document is generated using the kconfig2html tool that can be found in the
os/tools directory. That tool analyzes the TinyAra Kconfig files and
generates an excruciatingly boring HTML document.

The latest boring configuration variable documentation can be regenerated at
any time using that tool or, more appropriately, the wrapper script at
os/tools/mkconfigvars.sh.  That script will generate the file
docs/TinyAra_Config_Variables.html.

Supported Boards
^^^^^^^^^^^^^^^^

configs/artik053
  ARTIK-053 Starter Kit which is an evaluation board based on SAMSUNG S5JT200 WiFi SoC

configs/sidk_s5jt200
  Samsung IoT Development Kit for S5JT200


Configuring TinyAra
^^^^^^^^^^^^^^^^^^^

Configuring TinyAra requires only copying

  configs/<board-name>/<config-dir>/Make.def to ${TOPDIR}/Make.defs
  configs/<board-name>/<config-dir>/setenv.sh to ${TOPDIR}/setenv.sh
  configs/<board-name>/<config-dir>/defconfig to ${TOPDIR}/.config

tools/configure.sh
  There is a script that automates these steps.  The following steps will
  accomplish the same configuration:

    cd ${TOPDIR}/tools
   ./configure.sh <board-name>/<config-dir>

  There is an alternative Windows batch file that can be used in the
  windows native enironment like:

    cd ${TOPDIR}\tools
    configure.bat <board-name>\<config-dir>

  See tools/README.txt for more information about these scripts.

  And if your application directory is not in the standard loction (../apps
  or ../apps-<version>), then you should also specify the location of the
  application directory on the command line like:

    cd ${TOPDIR}/tools
    ./configure.sh -a <app-dir> <board-name>/<config-dir>

Building Symbol Tables
^^^^^^^^^^^^^^^^^^^^^^

Symbol tables are needed at several of the binfmt interfaces in order to bind
a module to the base code.  These symbol tables can be tricky to create and
will probably have to be tailored for any specific application, balancing
the number of symbols and the size of the symbol table against the symbols
required by the applications.

The top-level System.map file is one good source of symbol information
(which, or course, was just generated from the top-level tinyara file
using the GNU 'nm' tool).

There are also common-separated value (CSV) values in the source try that
provide information about symbols.  In particular:

  os/syscall/syscall.csv   - Describes the Tinyara RTOS interface, and
  lib/libc/libc.csv        - Describes the Tinyara C library interface.

There is a tool at os/tools/mksymtab that will use these CSV files as
input to generate a generic symbol table.  See os/tools/README.txt for
more information about using the mksymtab tool.
