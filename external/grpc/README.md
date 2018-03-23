# gRPC framework for TizenRT
TizenRT features the gRPC framework from the opensource project [gRPC](https://github.com/grpc/grpc), and is versioned at 1.9.


TizenRT's version of gRPC supports only C++ binding, so it is expected that the developer should write C++ applications in order to 
use gRPC. In this document, we cover two main aspects of porting gRPC to TizenRT, mainly details on how to build gRPC library, and how
to develop, build and run applications that use gRPC.

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
1. Using *Protocol buffer* external module located under `external/protobuf` to convert `.proto` specifications to C++ service and message classes,
2. Customizing TizenRT's `Makefile` to include the `protoc` compiler along with the grpc application,
3. TizenRT's run-time environment support for grpc application.


Let us look in detail at the points above.

### Using `Protobuf` to generate Service and Message Classes
The C++ binding for gRPC uses Service and Message classes to implement RPC stubs. Although the Service and Message classes can be written by hand,
the *Protocol buffer* compiler provides a convenient specification language, as well as automated C++ code generation for these classes. TizenRT has ported *Protocol buffer* under
`external/protobuf` folder, so it is highly recommended to use this external module to generate the aforestated classes.
Accordingly, please include `CONFIG_PROTOBUF` in your TizenRT build configuration as well.
You can refer to `external/protobuf/Kconfig` for details of this configuration.
In the following subsection, we describe in detail how
to include the `protoc` compilation command in TizenRT's application-level `Makefile`.

### Customizing `Makefile` for gRPC
In order to understand the details below, please refer to `apps/examples/greeter_client/Makefile` as an example.
In general, the application-level `Makefile` must include an additional step for auto-generating the Service and Message classes using the `protoc` compiler.
For convenience, developers are expected to create their `.proto` specification at the same application folder, and provide it to the `Makefile` as shown below:
```
CXXPROTO	= <proto-filename>.proto
```
Next, the `protoc` command will be invoked to generate the C++ Service and Message classes. The Service files have extensions `.grpc.pb.cc` (source) and `.grpc.pb.h` (header),
while the Message files have extensions `.pb.cc` and `.pb.h` correspondingly. These Source and Message classes are generated in Makefile as shown:
```
# Message class
$(CXXSRCS): %$(GENCXXEXT): %$(PROTOEXT)
	protoc -I . --cpp_out=. $<
# Service class
$(CXXSRCS2): %$(SERVICECXXEXT): %$(PROTOEXT)
	protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` $<
```
After the `protoc` step shown above, the Message class and Service class sources are compiled to generate object files of type `.pb.o` and `.grpc.pb.o` respectively.
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
From our initial verfication, we observe that a minimum thread stack size of 16384 bytes is necessary for the main gRPC thread to run successfully on TizenRT.
Additionally, this gRPC main thread engine uses `pthread_create` internally for its run-time procedures, for which we recommend setting a stack size of 10240 bytes or higher.
This can be configured via menuconfig at the location `Kernel Features -> Stack size information -> Default pthread stack size`.

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
