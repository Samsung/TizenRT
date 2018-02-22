# How to add new application

The following files are required to integrate a new application with the TizenRT build system:  
- **Source file**  
- [**Kconfig**](#kconfig)  
- [**Kconfig_ENTRY**](#kconfig_entry)  
- [**Make.defs**](#makedefs)  
- [**Makefile**](#makefile)

## Kconfig
This file is required by Kconfig-frontends menu build system.  
It is used to specify various config values according to which the application should be built.  
The following two config values must be defined in the Kconfig file.
```
config <APP_NAME>
	bool "<Write Description>"
	default n
	---help---
		<Write Details>

config USER_ENTRYPOINT
	string
	default "<APP_MAIN_FUNCTION>" if ENTRY_<APP_NAME>
```
The first config parameter specifies whether the application should be built or not.

The second config parameter specifies the function name of the application which should be executed when OS executes the application. The entrypoint can be set manually but for it the name of function which has to be executed firstly should be known for each application. This option registers the app with the build system and can be selected from the menu to be used as application entrypoint.

## Kconfig_ENTRY
This contains only one configuration which allows entry point selection by application name.
```
config ENTRY_<APP_NAME>
	bool "<Write Description>"
	depends on <APP_NAME>
```

Steps in menuconfig to enable and selecting an app as user entry point are shown below:
```
Application Configuration -> <APP_NAME> to y
Application Configuration -> Application entry point -> <APP_NAME>
```

## Make.defs
This specifies the folder name of new application.  
Makefile of apps folder reads this file and includes the folder in build when new application is enabled by configuration.
```
ifeq ($(CONFIG_<APP_NAME>),y)
CONFIGURED_APPS += <APP_FOLDER_NAME>
endif
```

## Makefile
This has compilation information like location of library, file information to compile, compilation flags and so on.  
Here is a basic format.
```
include $(APPDIR)/Make.defs

APPNAME = <APP_TASHCMD_NAME>
THREADEXEC = <TASH_EXEC_TYPE>

ASRCS =
CSRCS =
MAINSRC = <FILE_NAME>.c

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

ifneq ($(CONFIG_BUILD_KERNEL),y)
  OBJS += $(MAINOBJ)
endif

ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
else
  BIN = ../libapps$(LIBEXT)
endif
endif

ifeq ($(WINTOOL),y)
  INSTALL_DIR = "${shell cygpath -w $(BIN_DIR)}"
else
  INSTALL_DIR = $(BIN_DIR)
endif

CONFIG_<APP_NAME>_PROGNAME ?= <APP_NAME>$(EXEEXT)
PROGNAME = $(CONFIG_<APP_NAME>_PROGNAME)

ROOTDEPPATH = --dep-path .

# Common build

VPATH =

all: .built
.PHONY: clean depend distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

.built: $(OBJS)
	$(call ARCHIVE, $(BIN), $(OBJS))
	@touch .built

ifeq ($(CONFIG_BUILD_KERNEL),y)
$(BIN_DIR)$(DELIM)$(PROGNAME): $(OBJS) $(MAINOBJ)
	@echo "LD: $(PROGNAME)"
	$(Q) $(LD) $(LDELFFLAGS) $(LDLIBPATH) -o $(INSTALL_DIR)$(DELIM)$(PROGNAME) $(ARCHCRT0OBJ) $(MAINOBJ) $(LDLIBS)
	$(Q) $(NM) -u  $(INSTALL_DIR)$(DELIM)$(PROGNAME)

install: $(BIN_DIR)$(DELIM)$(PROGNAME)

else
install:

endif

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_<APP_NAME>),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(APPNAME)_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APP_NAME)_main.bdat

else
context:

endif

.depend: Makefile $(SRCS)
	@$(MKDEP) $(ROOTDEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	@touch $@

depend: .depend

clean:
	$(call DELFILE, .built)
	$(call CLEAN)

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:
```

Description of above is:

#### Support builtin functionality
```
APPNAME = <APP_TASHCMD_NAME>
THREADEXEC = <TASH_EXEC_TYPE>

ifeq ($(CONFIG_BUILTIN_APPS)$(CONFIG_<APP_NAME>),yy)
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(Q) $(call REGISTER,$(APPNAME),$(APPNAME)_main,$(THREADEXEC),$(PRIORITY),$(STACKSIZE))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APP_NAME)_main.bdat

else
context:

endif
```
Refer Builtin [README](builtin/README.md).

#### Library information
This specifies the library and its location into which the application would be built .
```
ifeq ($(CONFIG_WINDOWS_NATIVE),y)
  BIN = ..\libapps$(LIBEXT)
else
ifeq ($(WINTOOL),y)
  BIN = ..\\libapps$(LIBEXT)
else
  BIN = ../libapps$(LIBEXT)
endif
endif
```

If application has many folders inside, follow the Makefile of apps/example/testcase as an example.
