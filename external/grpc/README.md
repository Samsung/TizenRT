# gRPC framework for TizenRT
TizenRT features the gRPC framework from the opensource project [gRPC](https://github.com/grpc/grpc), and is versioned at 1.9.

The following covers the pre-requisites to building grpc on TizenRT.

## PreRequisites
TizenRT's version of gRPC supports only C++ binding, so it is expected that the developer should write C++ applications in order to 
use gRPC. In that context, gRPC needs specific *cpp plugins* in order to convert certain high-level RPC specifications (covered in detail under [Using `Protobuf` to generate Service and message classes](#using-protobuf-to-generate-service-and-message-classes)) to C++ header and source files.
At present, TizenRT's gRPC build borrows the plugins from a native build of gRPC on the host environment (usually Linux or Mac OS). This requires building gRPC 1.9.0. The following steps describe how to do so on a linux machine:

1. Install Pre-requisites
	```
	$ [sudo] apt-get install build-essential autoconf libtool pkg-config
	$ [sudo] apt-get install libgflags-dev libgtest-dev
	$ [sudo] apt-get install clang libc++-dev
	```

2. Clone gRPC from Github, and checkout branch `v1.9.x`
	```
	$ git clone https://github.com/grpc/grpc
	$ cd grpc
	$ git checkout v1.9.x
	```
3. Fetch the gRPC submodules, including `Protocol Buffer` which gRPC uses for RPC specifications
	```
	$ git submodule update --init
	```
4. Build and install from the grpc repository root
	```
	$ make
	$ [sudo] make install
	```
5. Build `Protocol Buffers` from within grpc folder. Assuming that you are on grpc folder, do the following:
	```
	$ cd third_party/protobuf
	$ git submodule update --init --recursive
	$ ./autogen.sh
	$ ./configure
	$ make
	$ make check
	$ [sudo] make install
	$ [sudo] ldconfig
	```
As a reference, please consult [C++ Installation Instructions](https://github.com/google/protobuf/blob/master/src/README.md) for details on how to install `Protobuf` from source.
After executing the steps above, you will have successfully installed the grpc and Protocol Buffer libraries, including cpp plugin and the protoc compiler on your host environment. The above steps are described in detail in [Installing gRPC from source](https://github.com/grpc/grpc/blob/master/BUILDING.md) In the following, we cover two main aspects of porting gRPC to TizenRT, mainly details on how to build gRPC library, and how to develop, build and run applications that use gRPC.

## Build gRPC in TizenRT
The source files for gRPC are located at `external/grpc`, and the associated header files are located at 
`external/include/grpc` folders. To build gRPC in TizenRT, you need to enable CONFIG_GRPC via menuconfig. In this case, the parameter is located
under `Networking Support -> Protocols -> gRPC`.
As gRPC uses C++ binding, make sure that you have enabled C++ library in TizenRT as well.
Especially, you need to include the relevant config parameters using menuconfig as shown below:
1. HAVE_CXX parameter, located at `Built-in Libraries -> Have C++ compiler`
2. HAVE_CXXINITIALIZE, located at `Application Configuration -> Platform-specific Support -> Have C++ initialization`
3. LIBC_LOCALE, located at `Built-in Libraries -> Enable I18N (LOCALE) support`.

After you have enabled the configuration parameters and typed `make` at the `os` folder, the object files for gRPC will be
linked together under static library `os/build/libraries/libexternal.a`.

## Develop, Build and Run TizenRT applications that use gRPC
This section is divided into three parts, namely,
1. Using *Protocol Buffer* external module located under `external/protobuf` to convert `.proto` specifications to C++ service and message classes,
2. Customizing TizenRT's `Makefile` to include the `protoc` compiler along with the grpc application,
3. TizenRT's run-time environment support for grpc application.


Let us look in detail at the points above.

### Using `Protobuf` to generate service and message classes
The C++ binding for gRPC uses Service and message classes to implement RPC stubs. Although the Service and message classes can be written by hand,
the *Protocol Buffer* compiler provides a convenient specification language, as well as automated C++ code generation for these classes. TizenRT has ported *Protocol Buffer* under
`external/protobuf` folder, so it is highly recommended to use this external module to generate the aforestated classes.
Accordingly, please include `CONFIG_PROTOBUF` in your TizenRT build configuration as well. You can refer to `external/protobuf/Kconfig` for details of this configuration. In order to generate stub code from `protobuf` specifications, `gRPC` features a set of plugins, one for each programming language. TizenRT requires the `grpc-cpp-plugin` to convert `.proto` files into C++ implementation of Message and Service classes. For this purpose, additional plugins must be installed on the host build environment (your Linux machine or VM). To do so, follow the installation steps described earlier in [Pre-Requisites](#pre-requisites).

In the following subsection, we describe in detail how
to include the `protoc` compilation command in TizenRT's application-level `Makefile`.

### Customizing `Makefile` for gRPC
In order to understand the details below, please refer to `apps/examples/greeter_client/Makefile` as an example.
In general, the application-level `Makefile` must include an additional step for auto-generating the Service and message classes using the `protoc` compiler.
For convenience, developers are expected to create their `.proto` specification at the same application folder, and provide it to the `Makefile` as shown below:
```
CXXPROTO	= <proto-filename>.proto
```
Next, the `protoc` command will be invoked to generate the C++ Service and message classes. The Service files have extensions `.grpc.pb.cc` (source) and `.grpc.pb.h` (header),
while the Message files have extensions `.pb.cc` and `.pb.h` correspondingly. These Source and message classes are generated in Makefile as shown:
```
# Message class
$(CXXSRCS): %$(GENCXXEXT): %$(PROTOEXT)
	protoc -I . --cpp_out=. $<
# Service class
$(CXXSRCS2): %$(SERVICECXXEXT): %$(PROTOEXT)
	protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $<
```
The `protoc` step above uses the `grpc_cpp_plugin` that comes with the `gRPC` installation on your host build machine. After the `protoc` step shown above, the Message class and Service class sources are compiled to generate object files of type `.pb.o` and `.grpc.pb.o` respectively.
These object files are combined together with the main application source file under the static library `os/build/libraries/libapps.a`.

Additionally, applications using gRPC need to be run through an `ASYNC` task from TASH, given their memory consumption. Accordingly, you need
to specify this in Makefile as shown below:
```
THREADEXEC = TASH_EXECMD_ASYNC
```

### Run-time Environment Support for gRPC
In this section, we cover the following details concerning TizenRT run-time support for gRPC.
#### Deploy gRPC as Thread
TizenRT applications are deployed as tasks from the TASH console. However, gRPC modules need to run in a thread context.
Therefore, our TizenRT application must internally use either `pthread_create` API or C++'s `std::thread` module to start gRPC.
Please refer to `grpc_greeter_client` and `grpc_route_client` applications under `apps/examples` folder as examples on how to achieve this.

An important consideration when running gRPC on TizenRT is the stack size allocated for its threads.
From our initial verification, we observe that a minimum thread stack size of 16384 bytes is necessary for the main gRPC thread to run successfully on TizenRT.
This can be achieved by either using the `pthread_attr_setstacksize` API or configuring the default pthread stacksize via menuconfig, at the location `Kernel Features -> Stack size information -> Default pthread stack size`.
Additionally, this gRPC main thread engine uses `pthread_create` internally for its run-time procedures, for which we recommend setting a stack size of 10240 bytes or higher. In order to configure this easily, TizenRT features a menuconfig parameter at the location `Networking Support -> Protocols -> gRPC -> Set thread size for grpc modules`.


#### Enable Debug Log
To enable run-time logs, you need to set the environment variable `GRPC_VERBOSITY` to `DEBUG`. This can be done on the TASH console
by using the `setenv` command as shown below:
```
TASH>> setenv GRPC_VERBOSITY DEBUG
```
Alternatively, it is a better option to use the setenv function inside the TizenRT application, as shown below:
```
setenv("GRPC_VERBOSITY", "DEBUG", 1);
```
gRPC also allows logging tracers by setting the `GRPC_TRACE` variable to `all`. However, this functionality is not enabled yet, so interested developers must
manually enable the tracers in the gRPC source.
