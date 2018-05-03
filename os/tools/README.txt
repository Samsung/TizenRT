tools/README.txt
================

/****************************************************************************
 *
 * tools/ will migrate to build/ until 2016's end.
 *
 ****************************************************************************/

This README file addresses the contents of the TinyAra tools/ directory.

The tools/ directory contains miscellaneous scripts and host C programs
that are necessary parts of the TinyAra build system.  These files
include:

README.txt
----------

  This file!

Config.mk
---------

  This file contains common definitions used by many configuration files.
  This file (along with <tinyara>/.config) must be included at the top of
  each configuration-specific Make.defs file like:

    -include $(TOPDIR)/.config
    include $(TOPDIR)/tools/Config.mk

  Subsequent logic within the configuration-specific Make.defs file may then
  override these default definitions as necessary.

configure.sh
configure.bat
configure.c, cfgparser.c, and cfgparser.h
------------

  configure.sh is a bash script that is used to configure TinyAra for a given
  target board in a environment that supports POSIX paths (Linux, Cygwin,
  OSX, or similar).  See build/configs/README.txt

  configure.c, cfgparser.c, and cfgparser.h can be used to build a work-alike
  program as a replacement for configure.sh.  This work-alike program would be
  used in environments that do not support Bash scripting (such as the Windows
  native environment).

  configure.bat is a small Windows batch file that can be used as a replacement
  for configure.sh in a Windows native environment.  configure.bat is actually
  just a thin layer that executes configure.exe if it is available. If
  configure.exe is not available, then configure.bat will attempt to build it
  first.

  In order two build configure.exe from configure.c in the Windows native
  environment, two assumptions are made:

  1) You have installed the MinGW GCC toolchain.  This toolchain can be
     downloaded from http://www.mingw.org/.  Tt is recommended the you not
     install the optional MSYS components as there may be conflicts.
  2) That path to bin bin/ directory containing mingw-gcc.exe must be
     included in the PATH variable.

discover.py
-----------

  Example script for discovering devices in the local network.
  It is the counter part to apps/netutils/discover

mkconfig.c, cfgdefine.c, and cfgdefine.h
----------------------------------------

  These are Cs file that are used to build mkconfig program.  The mkconfig
  program is used during the initial TinyAra build.

  When you configure TinyAra, you will copy a configuration file called .config
  in the top level os directory (See build/configs/README.txt).
  The first time you make TinyAra,
  the top-level makefile will build the mkconfig executable from mkconfig.c
  (using Makefile.host).  The top-level Makefile will then execute the
  mkconfig program to convert the .config file in the top level directory
  into include/tinyara/config.h.  config.h is a another version of the
  TinyAra configuration that can be included by C files.

cmdconfig.c
-----------

  This C file can be used to build a utility for comparing two TinyAra
  configuration files.

kconfig2html.c
--------------

  This is a C file that can be used build a utility for converting the
  TinyAra configuration in the Kconfig files to an HTML document.  This
  auto-generated documentation will, eventually, replace the manually
  updated configuration documentation that is fallling woefully behind.

  $ tools/kconfig2html.exe -h
  USAGE: tools/kconfig2html [-d] [-a <apps directory>] {-o <out file>] [<Kconfig root>]
         tools/kconfig2html [-h]

  Where:

    -a : Select relative path to the apps/ directory. Theis path is relative
         to the <Kconfig directory>.  Default: ../apps
    -o : Send output to <out file>.  Default: Output goes to stdout
    -d : Enable debug output
    -h : Prints this message and exits
    <Kconfig root> is the directory containing the root Kconfig file.
         Default <Kconfig directory>: .

mkconfigvars.sh
---------------

  The HTML documentation expects to have a copy of the auto-generated
  configuration variable documentation docs/TinyAra_Config_Variables.html.
  The script mkconfigvars.sh is a simple script that can be used to
  re-generated that file as needed.

  $ tools/mkconfigvars.sh -h
  tools/mkconfigvars.sh is a tool for generation of configuration variable documentation

  USAGE: tools/mkconfigvars.sh [-d|h] [-v <major.minor>]

  Where:
    -v <major.minor>
       The TinyAra version number expressed as a major and minor number separated
       by a period
    -d
       Enable script debug
    -h
       show this help message and exit

mkdefconfig.sh
--------------

  The shell script expects to have a default configuration sets at build/configs folder when .config file makes and needs to save.
  Two arguments should be given for board name and configuration name.

  $ tools/mkdefconfig.sh --help

  USAGE: ${0} <board-name> <config-name>
  Make the default configuration sets at build/config folder from .config

  Where:
    <board-name> is the name of the board in the configs directory
    <config-name> is the name of the board configuration sub-directory

mkexport.sh and Makefile.export
-------------------------------

  These implement part of the top-level Makefile's 'export' target.  That
  target will bundle up all of the TinyAra libraries, header files, and the
  startup object into an export-able, binary TinyAra distribution.  The
  Makefile.export is used only by the mkexport.sh script to parse out
  options from the top-level Make.defs file.

mkversion.c, cfgdefine.c, and cfgdefine.h
-----------------------------------------

  This is C file that is used to build mkversion program.  The mkversion
  program is used during the initial TinyAra build.

  When you build TinyAra there should be a version file called .version in
  the top level os directory.
  The first time you make TinyAra, the top-level makefile will build th
  mkversion executable from mkversion.c (using Makefile.host).  The top-
  level Makefile will then execute the mkversion program to convert the
  .version file in the top level directory into include/tinyara/version.h.
  version.h provides version information that can be included by C files.

mksyscall.c, cvsparser.c, and cvsparser.h
-----------------------------------------

  This is a C file that is used to build mksyscall program.  The mksyscall
  program is used during the initial TinyAra build by the logic in the top-
  level syscall/ directory.

  If you build TinyAra as a separately compiled, monolithic kernel and separate
  applications, then there is a syscall layer that is used to get from the
  user application space to the TinyAra kernel space.  In the user application
  "proxies" for each of the kernel functions are provided.  The proxies have
  the same function signature as the kernel function, but only execute a
  system call.

  Within the kernel, there are "stubs" for each of the system calls.  The
  stubs receive the marshalled system call data, and perform the actually
  kernel function call (in kernel-mode) on behalf of the proxy function.

  Information about the stubs and proxies is maintained in a comma separated
  value (CSV) file in the syscall/ directory.  The mksyscall program will
  accept this CVS file as input and generate all of the required proxy or
  stub files as output.  See syscall/README.txt for additonal information.

mksymtab.c, cvsparser.c, and cvsparser.h
----------------------------------------

  This is a C file that is used to build symbol tables from common-separated
  value (CSV) files.  This tool is not used during the TinyAra build, but
  can be used as needed to generate files.

  USAGE: ./mksymtab <cvs-file> <symtab-file>

  Where:

    <cvs-file>   : The path to the input CSV file
    <symtab-file>: The path to the output symbol table file
    -d           : Enable debug output

  Example:

    cd os/tools
    cat ../syscall/syscall.csv ../lib/libc.csv | sort >tmp.csv
    ./mksymtab.exe tmp.csv tmp.c

mkctags.sh
----------

  A script for creating ctags from Ken Pettit.  See http://en.wikipedia.org/wiki/Ctags
  and http://ctags.sourceforge.net/

Makefile.host
-------------

  This is the makefile that is used to make the mkconfig program from
  the mkconfig.c C file, the cmpconfig program from cmpconfig.c C file
  the mkversion program from the mkconfig.c C file, or the mksyscall
  program from the mksyscall.c file.  Usage:

  cd tools/
  make -f Makefile.host <program>

mkromfsimg.sh
-------------

  This script may be used to automate the generate of a ROMFS file system
  image.  It accepts an rcS script "template" and generates and image that
  may be mounted under /etc in the TinyAra pseudo file system.

mkdeps.sh
mkdeps.bat
mkdeps.c
mknulldeps.sh
-------------

  TinyAra uses the GCC compilers capabilities to create Makefile dependencies.
  The bash script mkdeps.sh is used to run GCC in order to create the
  dependencies.  If a TinyAra configuration uses the GCC toolchain,
  its Make.defs file (see build/configs/README.txt) will include a line like:

    MKDEP = $(TOPDIR)/tools/mkdeps.sh, or
    MKDEP = $(TOPDIR)/tools/mkdeps[.exe] (See NOTE below)

  If the TinyAra configuration does not use a GCC compatible toolchain, then
  it cannot use the dependencies and instead it uses mknulldeps.sh:

    MKDEP = $(TOPDIR)/tools/mknulldeps.sh

  The mknulldeps.sh is a stub script that does essentially nothing.

  NOTE:  The mk*deps.* files are undergoing change.  mkdeps.sh is a bash
  script that produces dependencies well for POSIX style hosts (e..g.,
  Linux and Cygwin).  It does not work well for mixed environments with
  a Windows toolchain running in a POSIX style environment (hence, the
  mknulldeps.sh script).  And, of course, cannot be used in a Windows
  nativ environment.

  [mkdeps.sh does have an option, --winpath, that purports to convert
  the dependencies generated by a Windows toolchain to POSIX format.
  However, that is not being used and mostly likely does not cover
  all of the conversion cases.]

  mkdeps.bat is a simple port of the bash script to run in a Windows
  command shell.  However, it does not work well either because some
  of the common CFLAGS use characters like '=' which are transformed
  by the CMD.exe shell.

  mkdeps.c generates mkdeps (on Linux) or mkdeps.exe (on Windows).
  However, this verison is still under-development.  It works well in
  the all POSIX environment or in the all Windows environment but also
  does not work well in mixed POSIX environment with a Windows toolchain.
  In that case, there are still issues with the conversion of things like
  'c:\Program Files' to 'c:program files' by bash.  Those issues may,
  eventually be solvable but for now continue to use mknulldeps.sh in
  that mixed environment.

define.sh
define.bat
---------

  Different compilers have different conventions for specifying pre-
  processor definitions on the compiler command line.  This bash
  script allows the build system to create create command line definitions
  without concern for the particular compiler in use.

  The define.bat script is a counterpart for use in the native Windows
  build.

incdir.sh
incdir.bat
---------

  Different compilers have different conventions for specifying lists
  of include file paths on the compiler command line.  This incdir.sh
  bash script allows the build system to create include file paths without
  concern for the particular compiler in use.

  The incdir.bat script is a counterpart for use in the native Windows
  build.  However, there is currently only one compiler supported in
  that context:  MinGW-GCC.

link.sh
link.bat
copydir.sh
copydir.bat
unlink.sh
unlink.bat
----------

  Different file system have different capabilities for symbolic links.
  Some windows file systems have no native support for symbolic links.
  Cygwin running under windows has special links built in that work with
  all cygwin tools.  However, they do not work when Windows native tools
  are used with cygwin.  In that case something different must be done.

  If you are building under Linux or under cygwin with a cygwin tool
  chain, then your Make.defs file may have definitions like the
  following:

    DIRLINK = $(TOPDIR)/tools/link.sh
    DIRUNLINK = (TOPDIR)/tools/unlink.sh

  The first definition is not always present because link.sh is the
  default.  link.sh is a bash script that performs a normal, Linux-style
  symbolic link;  unlink.sh is a do-it-all unlinking script.

  But if you are building under cygwin using a Windows native toolchain
  within a POSIX framework (such as Cygwin), then you will need something
  like the following in you Make.defs file:

    DIRLINK = $(TOPDIR)/tools/copydir.sh
    DIRUNLINK = (TOPDIR)/tools/unlink.sh

  copydir.sh will copy the whole directory instead of linking it.

  Finally, if you are running in a pure native Windows environment with
  a CMD.exe shell, then you will need something like this:

    DIRLINK = $(TOPDIR)/tools/copydir.bat
    DIRUNLINK = (TOPDIR)/tools/unlink.bat

  Note that this will copy directories.  ;ink.bat might also be used in
  this case.  link.bat will attempt to create a symbolic link using the
  NTFS mklink.exe command instead of copying files.  That logic, however,
  has not been verified as of this writing.

kconfig.bat
-----------

  Recent versions of TinyAra support building TinyAra from a native Windows
  CMD.exe shell.  But kconfig-frontends is a Linux tool and is not yet
  available in the pure CMD.exe environment.  At this point, there are
  only a few options for the Windows user (see the top-level README.txt
  file).

  You can, with some effort, run the Cygwin kconfig-mconf tool directly
  in the CMD.exe shell.  In this case, you do not have to modify the
  .config file, but there are other complexities:  You need to
  temporarily set the Cgywin directories in the PATH variable and
  then run kconfig-mconf outside of the Make system.

  kconfig.bat is a Windows batch file at tools/kconfig.bat that automates
  these steps.  It is used from the top-level TinyAra directory like:

    tools/kconfig menuconfig

  NOTE: There is an currently an issue with accessing DOS environment
  variables from the Cygwin kconfig-mconf running in the CMD.exe shell.
  The following change to the top-level Kconfig file seems to work around
  these problems:

     config APPSDIR
          string
     -   option env="APPSDIR"
     +   default "../apps"

refresh.sh
----------

  This is a bash script that automatics refreshing of board default
  configuration (defconfig) files.  It does not do anything special
  thet you cannot do manually, but is useful for me when I have to
  update dozens of confuration files.

  Configuration files have to be updated because over time, the
  configuration settings change:  New configurations are added and
  new dependencies are added.  So an old configuration file may
  not be usable anymore until it is refreshed.

  The steps to refresh the file are:

  1. Make tools/cmpconfig if it is not already built.
  2. Copy the the defconfig file to the top-level TinyAra
     directory as .config (being careful to save any previous
     .config file that you might want to keep!).
  3. Execute 'make oldconfig' to update the configuration.
     'make oldconfig' will prompt you for each change in the
     configuration that requires that you make some decision.
  4. Then it runs tools/cmpconfig to show the real differences
     between the configuration files.  Configuration files are
     complex and things can move around so a simple 'diff' between
     two configuration files is often not useful.  But tools/cmpconfig
     will show only the meaningful differences between the two
     configuration files.
  4. It will edit the .config file to comment out the setting
     of the CONFIG_APPS_DIR= setting.  This setting should not
     be in checked-in defconfig files because the actually must
     be determined at the next time that the configuration is
     installed.
  5. Finally, the refreshed defconfig file is copied back in
     place where it can be committed with the next set of
     difference to the command line.  refresh.sh will prompt
     you first to avoid overwriting the defconfig file with
     changes that you do not want.
`
zipme.sh
--------

  I use this script to create the tinyara-xx.yy.tar.gz tarballs for
  release on SourceForge.  It is handy because it also does the
  kind of clean that you need to do to make a clean code release.
