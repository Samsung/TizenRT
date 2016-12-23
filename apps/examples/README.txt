examples
^^^^^^^^

  Selecting examples:

    The examples directory contains several sample applications that
    can be linked with TinyAra.  The specific example is selected in the
    configs/<board-name>/defconfig file via the CONFIG_EXAMPLES_xyz
    setting where xyz is the name of the example. For example,

      CONFIG_EXAMPLES_HELLO=y

    Selects the examples/hello example.

  Selecting main examples:

    You can enable several examples but you should set one as an entry point
    among them. There are several CONFIG_ENTRY_xyz in Kconfig_ENTRY. When you
    enable some example, then you can see that config in menuconfig and can select
    it as an entry point. If you select manual entry, you can write an entry point
    directly.

examples/artik_demo
^^^^^^^^^^^^^^^^^^^

  This is an example to show DM APIs working with artik cloud(https://artik.cloud).
  For more infomation, see the examples/artik_demo/artik_demo.c

  Depends on:
  * CONFIG_DM
  * CONFIG_NET
  * CONFIG_EXAMPLES_ARTIK_DEMO

examples/fota_sample
^^^^^^^^^^^^^^^^^^^^

  This is a sample example to test the firmware update functionality working at
  TASH. FOTA has two modules, driver in kernel and hal in application. You should
  enable them before testing it. You can find that running partition is changed.

  Depends on:
  * CONFIG_SYSTEM_FOTA_HAL
  * CONFIG_FOTA_DRIVER

examples/hello
^^^^^^^^^^^^^^

  This is the mandatory, "Hello, World!!" example.  It prints "Hello, World!!" third.
  Really useful only for bringing up new TinyAra architectures.

examples/hello_tash
^^^^^^^^^^^^^^^^^^^

  This is the example to run "Hello, World!!" thread in TASH.  It is registered
  call-back function to TASH in main function. And hello sentence is printed when you
  excute it at TASH.

examples/helloxx
^^^^^^^^^^^^^^^^

  This is C++ version of the "Hello, World!!" example.  It is intended
  only to verify that the C++ compiler is functional, that basic C++
  library suupport is available, and that class are instantiated
  correctly.

  TinyAra configuration prerequisites:

    CONFIG_HAVE_CXX -- Enable C++ Support

  Optional TinyAra configuration settings:

    CONFIG_HAVE_CXXINITIALIZE -- Enable support for static constructors
      (may not be available on all platforms).

  TinyAra configuration settings specific to this examp;le:

    CONFIG_EXAMPLES_HELLOXX_NOSTACKCONST - Set if the system does not
      support construction of objects on the stack.
    CONFIG_EXAMPLES_HELLOXX_CXXINITIALIZE - By default, if CONFIG_HAVE_CXX
      and CONFIG_HAVE_CXXINITIALIZE are defined, then this example
      will call the TinyAra function to initialize static C++ constructors.
      This option may be disabled, however, if that static initialization
      was performed elsewhere.

  Also needed:

    CONFIG_HAVE_CXX=y

  And you may have to tinker with the following to get libxx to compile
  properly:

    CONFIG_CXX_NEWLONG=y or =n

  The argument of the 'new' operators should take a type of size_t.  But size_t
  has an unknown underlying.  In the TinyAra sys/types.h header file, size_t
  is typed as uint32_t (which is determined by architecture-specific logic).
  But the C++ compiler may believe that size_t is of a different type resulting
  in compilation errors in the operator.  Using the underlying integer type
  Instead of size_t seems to resolve the compilation issues.

examples/iotivity
^^^^^^^^^^^^^^^^^
  This is an example to build and run IoTivity on TinyAra.

  Depends on:
  * CONFIG_EXAMPLES_IOTIVITY

examples/kernel_sample
^^^^^^^^^^^^^^^^^^^^^^

  This is the TinyAra 'qualification' suite.  It attempts to exercise
  a broad set of OS functionality.  Its coverage is not very extensive
  as of this writing, but it is used to qualify each TinyAra release.

  The behavior of the kernel_sample can be modified with the following
  settings in the configs/<board-name>/defconfig file:

  * CONFIG_EXAMPLES_KERNEL_SAMPLE_LOOPS
      Used to control the number of executions of the test.  If
      undefined, the test executes one time.  If defined to be
      zero, the test runs forever.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_STACKSIZE
      Used to create the kernel_sample task.  Default is 8192.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_NBARRIER_THREADS
      Specifies the number of threads to create in the barrier
      test.  The default is 8 but a smaller number may be needed on
      systems without sufficient memory to start so many threads.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_AIO
      Enables test of asynchronous I/O.  You must have a mounted, writable
		  directory in place to use this test.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_AIOPATH
      This is the location of a directory in a mounted file system that
		  the AIO test can write into.
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_RR_RANGE
      During round-robin scheduling test two threads are created. Each of the threads
      searches for prime numbers in the configurable range, doing that configurable
      number of times.
      This value specifies the end of search range and together with number of runs
      allows to configure the length of this test - it should last at least a few
      tens of seconds. Allowed values [1; 32767], default 10000
  * CONFIG_EXAMPLES_KERNEL_SAMPLE_RR_RUNS
      During round-robin scheduling test two threads are created. Each of the threads
      searches for prime numbers in the configurable range, doing that configurable
      number of times.

examples/mpu
^^^^^^^^^^^^

  mpu functionality test (read and write) example.

  Depends on:
  * CONFIG_BUILD_PROTECTED

examples/mdns_test
^^^^^^^^^^^^^^^^^^

  This is an example for mDNS test. mDNS resolves a host name to an IP address within small networks that do not include a local name setver.

  Depends on:
  * CONFIG_EXAMPLES_MDNS_TEST

examples/netperf_test
^^^^^^^^^^^^^^^^^^^^^

  This is an example for a network stack performance test. Simple client/server sends or receives data using UDP.

  Depends on:
  * CONFIG_EXAMPLES_NETPERF

examples/netstresstest
^^^^^^^^^^^^^^^^^^^^^^

  This is an example for a network stress test for TCP, UDP, and IP Multicast. You can test them all at one time.
  It launches a pthread to run tash cmd asynchronously. You can excute it in TASH.

  Depends on:
  * CONFIG_EXAMPLES_NETSTRESSTEST

examples/nettest
^^^^^^^^^^^^^^^^

  This is an example for a network test for TCP, UDP, and IP Multicast. You can excute it in TASH.

  Depends on:
  * CONFIG_EXAMPLES_NETTEST

examples/ntpclient_test
^^^^^^^^^^^^^^^^^^^^^^^

  This is an example for a NTP client test. You can excute it with below options in TASH.
  Options:
      start: Start NTP client daemon
      stop : Terminate NTP client daemon
      status : Show status of NTP client daemon
      date : Show current date and time
      link : Show line status of NTP client daemon

  Depends on:
  * CONFIG_EXAMPLES_NTPCLIENT_TEST
  * CONFIG_NETUTILS_NTPCLIENT
  * CONFIG_NETUTILS_NTPCLIENT_STACKSIZE
  * CONFIG_NETUTILS_NTPCLIENT_SERVERPRIO
  * CONFIG_NETUTILS_NTPCLIENT_SIGWAKEUP

examples/nv_manager
^^^^^^^^^^^^^^^^^^^

  This is a test example for nv manager. You can excute it with below options in TASH.
  Options:
      -f : NVM Factory processing
      -t : NVM testcase execution
      -w : Write NV RAM buff to Flash NVM
      -e : Erase All Flash NVM data
      -s : Print NVM Manager status

  Depends on:
  * CONFIG_NV_MANAGER

examples/proc_test
^^^^^^^^^^^^^^^^^^

  This is an sample example of proc usages. You can find informations about process, cpu usages, version
  and sytem up time.

examples/telnetd
^^^^^^^^^^^^^^^^

  This is an example for a Telnet daemon. You can excute it in TASH.

  Depends on:
  * CONFIG_EXAMPLES_TELNETD
  * CONFIG_EXAMPLES_TELNETD_NOMAC
  * CONFIG_EXAMPLES_TELNETD_IPADDR
  * CONFIG_EXAMPLES_TELNETD_DRIPADDR
  * CONFIG_EXAMPLES_TELNETD_NETMASK
  * CONFIG_EXAMPLES_TELNETD_DAEMONPRIO
  * CONFIG_EXAMPLES_TELNETD_DAEMONSTACKSIZE
  * CONFIG_EXAMPLES_TELNETD_CLIENTPRIO
  * CONFIG_EXAMPLES_TELNETD_CLIENTSTACKSIZE
  * CONFIG_NETUTILS_TELNETD

examples/testcase
^^^^^^^^^^^^^^^^^

  This is an example to test working APIs and system. It has unit testcases and
  integration testcases on kernel, system IO, filesystem, database and network sides.
  You can select below configurations what you want to test.

  sub-modules's configurations:
  * CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_UTC
  * CONFIG_EXAMPLES_TESTCASE_ARASTORAGE_ITC
  * CONFIG_EXAMPLES_TESTCASE_FILESYSTEM
  * CONFIG_EXAMPLES_TESTCASE_KERNEL
  * CONFIG_EXAMPLES_TESTCASE_NETWORK
  * CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_UTC
  * CONFIG_EXAMPLES_TESTCASE_SYSTEMIO_ITC
  * CONFIG_EXAMPLES_TESTCASE_DM_UTC
  * CONFIG_EXAMPLES_TESTCASE_DM_ITC

examples/workqueue
^^^^^^^^^^^^^^^^^^

  This is an example to show working sequence of workqueue.

  Depends on:
  * CONFIG_SCHED_WORKQUEUE
  * CONFIG_LIB_USRWORK

examples/tls_server
^^^^^^^^^^^^^^^^^^^

  This is an example to test tls server.
  For more information, see the examples/tls_server/tls_server_main.c
  or type the command "tlss help" on TASH.

  usage:
    ex) tlss help

  Depends on:
  * CONFIG_NET_TLS
  * CONFIG_EXAMPLES_TLS_SERVER

examples/tls_client
^^^^^^^^^^^^^^^^^^^

  This is an example to test tls client.
  For more information, see the examples/tls_client/tls_client_main.c
  or type the command "tlsc help" on TASH.

  usage:
    ex) tlsc help

  Depends on:
  * CONFIG_NET_TLS
  * CONFIG_EXAMPLES_TLS_CLIENT

examples/tls_selftest
^^^^^^^^^^^^^^^^^^^^^

  This is a unit test case for tls libarary.
  For more information, see the examples/tls_client/tls_selftest_main.c.

  usage:
    ex) tlsself

  Depends on:
  * CONFIG_NET_TLS
  * CONFIG_EXAMPLES_TLS_SELFTEST

examples/dtls_server
^^^^^^^^^^^^^^^^^^^

  This is an example to test dtls server.
  For more information, see the examples/dtls_server/dtls_server_main.c.
  There is no usage for server, so it will run by default configuration.
  If you wan to change ciphersuite or other configuration,
  you should change source code manually.

  usage:
    ex) dtlss

  Depends on:
  * CONFIG_NET_TLS
  * CONFIG_EXAMPLES_DTLS_SERVER

examples/dtls_client
^^^^^^^^^^^^^^^^^^^

  This is an example to test dtls client.
  For more information, see the examples/dtls_client/dtls_client_main.c.
  When you are using dtls client example, please type "dtlsc help" on Tash.

  usage:
    ex) dtlsc
    ex) dtlsc server_addr=000.000.000.000
    ex) dtlsc server_name=namename
    ex) dtlsc server_addr=000.000.000.000 server_name=namename

  Depends on:
  * CONFIG_NET_TLS
  * CONFIG_EXAMPLES_DTLS_CLIENT

examples/websocket
^^^^^^^^^^^^^^^^^^

  This is an example to introduce basic usage of websocket APIs.
  For more information, see the examples/websocket/websocket_main.c

  usage:
  websocket client [addr] [port] [path] [tls_enable] [size] [num]
    ex) websocket client 127.0.0.1 80 \ 0 2048 10
  websocket server [tls_enable]
    ex) websocket server server 1

  Depends on:
  * CONFIG_NETUTILS_WEBSOCKET

examples/webserver
^^^^^^^^^^^^^^^^^^

  This is an example of webserver.
  It is executed by "webserver <operation>" command.
  The <operation> is one of "start" or "stop".
  If <operation> is "start", it starts a HTTP server with port 80 and a HTTPS server with port 443.
  But if CONFIG_NET_SECURITY_TLS is not defined, it starts only HTTP server.
  If <operation> is "stop", it stops both server.

  Depends on:
  * CONFIG_NETUTILS_WEBSERVER

examples/webclient
^^^^^^^^^^^^^^^^^^

  This is an example of webclient.
  It is executed by "webclient <method> <uri> (<entity> <encoding>)" command.
  <method> and <url> parameters should be given, <entity> and <encoding> parameters are optional.
  <method> is one if "GET", "POST", "PUT", "DELETE".
  <url> should start with "http://" or "https://".
  <entity> is a string value and its default value is NULL.
  The way of sending an entity is content-length by default.
  If <encoding> is "c", it sends an entity with chunked encoding.

examples/wakaama_client
^^^^^^^^^^^^^^^^^^^^^^^

  This is an example to show DM APIs working with a sample client application
  The application allows the developer to specify LWM2M client configuration such
  as LWM2M server IP address and port, as well as client lifetime. In addition,
  the application allows the developer to fetch values from LWM2M objects.
