README
======

  Basic Configuration
  -------------------
  This directory provides an example of how to configure and use
  the NuttShell (TZSH) application.  TZSH is a simple shell
  application.  TZSH is described in its own README located at
  apps/nshlib/README.txt.  This function is enabled with:

    CONFIG_SYSTEM_TZSH=y

  Applications using this example will need to provide an defconfig
  file in the configuration directory with instruction to build
  the TZSH library like:

    CONFIG_TZSH_LIBRARY=y

  Other Configuration Requirements
  --------------------------------
  NOTE:  If the TZSH serial console is used, then following is also
  required to build the readline() library:

    CONFIG_SYSTEM_READLINE=y

  And if networking is included:

    CONFIG_NETUTILS_NETLIB=y
    CONFIG_NETUTILS_DHCPC=y
    CONFIG_NETDB_DNSCLIENT=y
    CONFIG_NETUTILS_TFTPC=y
    CONFIG_NETUTILS_WEBCLIENT=y

  If the Telnet console is enabled, then the defconfig file should
  also include:

    CONFIG_NETUTILS_TELNETD=y

  Also if the Telnet console is enabled, make sure that you have the
  following set in the NuttX configuration file or else the performance
  will be very bad (because there will be only one character per TCP
  transfer):

    CONFIG_STDIO_BUFFER_SIZE - Some value >= 64
    CONFIG_STDIO_LINEBUFFER=y

  C++ Support
  -----------
  If CONFIG_HAVE_CXX=y and CONFIG_HAVE_CXXINITIALIZE=y, then this TZSH
  example can be configured to initialize C++ constructors when it
  is started.  TZSH does not use C++ and, by default, assumes that
  constructors are initialized elsewhere.  However, you can force
  TZSH to initialize constructors by setting:

    CONFIG_SYSTEM_TZSH_CXXINITIALIZE=y
