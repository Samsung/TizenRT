# Trouble Shooting

## Contents
> [Common](#common)  
> [Board-Specific](#board-specific)

## Common
### Issues on Kconfig-frontend
When ```make menuconfig``` executes after installing Kconfig-frontend, someone meets below:
```
kconfig-mconf: error while loading shared libraries: libkconfig-parser-x.xx.0.so: cannot open shared object file: No such file or directory
Makefile.unix:579: recipe for target 'menuconfig' failed
make: *** [menuconfig] Error 127
```
To resolve:
```
cd <Kconfig-frontend_package_PATH>
./configure --prefix=/usr
make
sudo make install
```

### Issue on build of Proto buffers
When Proto buffers (a.k.a., protobuf) which is mandatory on using gRPC is enabled, 
you can meet build breaks as shown below:
```
AR: helloxx_main.o
make[2]: Leaving directory '/TizenRT/apps/examples/helloxx'
make[2]: Entering directory '/TizenRT/apps/examples/grpc_greeter_client'
protoc -I . --cpp_out=. helloworld.proto
make[2]: protoc: Command not found
Makefile:86: recipe for target 'helloworld.pb.cc' failed
make[2]: *** [helloworld.pb.cc] Error 127
make[2]: Leaving directory '/TizenRT/apps/examples/grpc_greeter_client'
Makefile:109: recipe for target 'examples/grpc_greeter_client_all' failed
make[1]: *** [examples/grpc_greeter_client_all] Error 2
make[1]: Leaving directory '/TizenRT/apps'
LibTargets.mk:158: recipe for target '../apps/libapps.a' failed
make: *** [../apps/libapps.a] Error 2
```
This is coming from missing ```protoc``` installation.  
Please find [Proto buffers' README](https://github.com/Samsung/TizenRT/blob/master/external/protobuf/README.md).

## Board-Specific
### ARTIK
#### Issues on Programming
When USB connection is not established, someone meets below:
```
[Command] make download ALL
Generating partition map ... Done
Open On-Chip Debugger 0.10.0-dirty (2017-09-02-08:32)
Licensed under GNU GPL v2
For bug reports, read
http://openocd.org/doc/doxygen/bugs.html
adapter speed: 2000 kHz
Info : auto-selecting first available session transport "jtag". To override use 'transport select '.
force hard breakpoints
trst_and_srst separate srst_gates_jtag trst_push_pull srst_push_pull connect_deassert_srst
adapter_nsrst_assert_width: 50
adapter_nsrst_delay: 100
debug_level: -1

Makefile.unix:543: recipe for target 'download' failed
make: *** [download] Error 1
[STATUS] FAILED
```

To resolve,  
1. See [USB device rules](https://github.com/Samsung/TizenRT/blob/master/build/configs/artik053/README.md#add-usb-device-rules).
2. Change or re-plugin USB cable and confirm USB connection.
