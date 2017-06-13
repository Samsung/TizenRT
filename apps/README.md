# Application Folder

### Contents

> [**General**](README.md#general)  
> [**Directory Location**](README.md#directory-location)  
> [**Built-In Applications**](builtin/README.md)  
> [**Shell (TASH)**](shell/README.md)  
> [**Application Configuration File**](README.md#application-configuration-file)  
> [**Building TinyAra with Board-Specific Pieces Outside the Source Tree**](README.md#building-tinyara-with-board-specific-pieces-outside-the-source-tree)

## General

This folder provides various applications found in sub-directories.  These
applications are not inherently a part of TinyAra but are provided to help
you develop your own applications.  The apps/ directory is a "break away"
part of the configuration that you may choose to use or not.

## Directory Location

The default application directory used by the TinyAra build should be named
apps/ (or apps-x.y/ where x.y is the TinyAra version number).  This apps/
directory should appear in the directory tree at the same level as the
TinyAra os directory.  Like:

	.  
	|- os  
	|  
	`- apps

If all of the above conditions are TRUE, then TinyAra will be able to
find the application directory.  If your application directory has a
different name or is location at a different position, then you will
have to inform the TinyAra build system of that location.  There are several
ways to do that:

1) You can define CONFIG_APPS_DIR to be the full path to your application
   directory in the TinyAra configuration file.  
2) You can provide the path to the application directory on the command line  
   like:  make APPDIR=\<path\> or make CONFIG_APPS_DIR=\<path\>
3) When you configure TinyAra using tools/configure.sh, you can provide that
   path to the application directory on the configuration command line  
   like: ./configure.sh -a \<app-dir\> \<board-name\>/\<config-name\>

## Application Configuration File

The TinyAra configuration uses kconfig-frontends tools and the TinyAra
configuration file (.config) file.  For example, the TinyAra .config
may have:

	CONFIG_EXAMPLES_HELLO=y

This will select the apps/examples/hello in the following way:

- The top-level make will include examples/Make.defs  
- examples/Make.defs will set CONFIGURED_APPS += examples/hello  
  like this:

	ifeq ($(CONFIG_EXAMPLES_HELLO),y)  
	CONFIGURED_APPS += examples/hello  
	endif


## FAQ

Q: Has anyone come up with a tidy way to build TinyAra with board-
   specific pieces outside the source tree?  
A: Here are four:

   1) There is a make target called 'make export'. It will build
      TinyAra, then bundle all of the header files, libaries, startup
      objects, and other build components into a .zip file. You
      can can move that .zip file into any build environment you
      want. You even build TinyAra under a DOS CMD window.

   2) You can replace the entire apps/ directory. If there is
      nothing in the apps/ directory that you need, you can define
      CONFIG_APPS_DIR in your .config file so that it points to a
      different, custom application directory.

      You can copy any pieces that you like from the old apps/directory
      to your custom apps directory as necessary.

      This is documented in build/configs/README.txt and  
      docs/TinyAra_Config_Variables.html. And in the apps/README.md file.

   3) If you like the random collection of stuff in the apps/ directory
      but just want to expand the existing components with your own,
      external sub-directory then there is an easy way to that too:
      You just create the sympolic link at apps/external that
      redirects to your application sub-directory. The apps/Makefile
      will always automatically check for the existence of an
      apps/external directory and if it exists, it will automatically
      incorporate it into the build.

      This feature of the apps/Makefile is documented only here.

      You can, for example, create a script called install.sh that
      installs a custom application, configuration, and board specific
      directory:

      a) Copy 'MyBoard' directory to configs/MyBoard.  
      b) Add a symbolic link to MyApplication at apps/external  
      c) Configure TinyAra (usually by:  

		tools/configure.sh MyBoard/MyConfiguration  
		or simply by copying defconfig->os/.config,  
		setenv.sh->os/setenv.sh and Make.defs->os/Make.defs.

      Using the 'external' link makes it especially easy to add a
      'built-in' application an existing configuration.

   4) Add any link to apps/

      a) Add symbolic links apps/ to as many other directories as you
         want,  
      b) Add the symbolic link to the list of candidate paths in the
         top level apps/Makefile, and  
      b) Add the (relative) paths to the CONFIGURED_APPS list
         in the Make.defs file in your new directory.

      That is basically the same as my option #3 but doesn't use the
      magic 'external' link.
