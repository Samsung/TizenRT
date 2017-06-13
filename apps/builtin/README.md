# builtin

The **builtin** supports registering commands in TASH through Makefile without calling any TASH public API.
This helps in porting the application directly to TinyAra without changing application's code.  
It is needed only when TASH is enabled.

## How to use

### Configuration

Enable configuration of builtin through menuconfig
```bash
cd $TIZENRT_BASEDIR
cd os
make menuconfig
```
Select menu
```bash
Application Configuration -> Support builtin applications to y
```

### Makefile

1. Add calling REGISTER at context tab
```bash
$(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat: $(DEPCONFIG) Makefile
	$(call REGISTER,$(APPNAME),$(FUNCNAME),$(THREADEXEC))

context: $(BUILTIN_REGISTRY)$(DELIM)$(APPNAME)_main.bdat
```

2. Include apps/Make.defs to use REGISTER
```bash
include $(APPDIR)/Make.defs
```

3. Set argumens, command name, callback function and execution type
```bash
APPNAME =
FUNCNAME =
THREADEXEC =
```

For arguments details, please refer **Public APIs** [[tab]](../shell/README.md#public-apis) of TASH README
