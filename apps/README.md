# Application Folder

### Contents

In this file,  
> [**General**](README.md#general)  
> [**Directory Location**](README.md#directory-location)  
> [**Application Configuration File**](README.md#application-configuration-file)

Outside this file,  
> [**Built-In Applications**](builtin/README.md)  
> [**Shell (TASH)**](shell/README.md)  
> [**How to add new application**](HowtoAddNewApp.md)

## General

This folder provides various applications found in sub-directories. These
applications are not inherently a part of TinyAra but are provided to help
you develop your own applications.  The apps/ directory is a "break away"
part of the configuration that you may choose to use or not.

## Directory Location

The default application directory used by the TinyAra build should be named
apps/ (or apps-x.y/ where x.y is the TinyAra version number). This apps/
directory should appear in the directory tree at the same level as the
TinyAra os directory.  Like:

	.  
	|- os  
	|  
	`- apps

If all of the above conditions are TRUE, then TinyAra will be able to
find the application directory. If your application directory has a
different name or is location at a different position, then you will
have to inform the TinyAra build system of that location. There are several
ways to do that:

1. You can define CONFIG_APPS_DIR to be the full path to your application
   directory in the TinyAra configuration file.  
2. You can provide the path to the application directory on the command line  
   like:  make APPDIR=\<path\> or make CONFIG_APPS_DIR=\<path\>
3. When you configure TinyAra using tools/configure.sh, you can provide that
   path to the application directory on the configuration command line  
   like: ./configure.sh -a \<app-dir\> \<board-name\>/\<config-name\>

## Application Configuration File

The TinyAra configuration uses kconfig-frontends tools and the TinyAra
configuration file (.config) file.  For example, the TinyAra .config
may have:
```
CONFIG_EXAMPLES_HELLO=y
```

This will select the apps/examples/hello in the following way:

- The top-level make will include examples/Make.defs  
- examples/Make.defs will set CONFIGURED_APPS += examples/hello  
  like this:
```
ifeq ($(CONFIG_EXAMPLES_HELLO),y)  
CONFIGURED_APPS += examples/hello  
endif
```

