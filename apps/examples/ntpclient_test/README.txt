examples/ntpclient_test
^^^^^^^^^^^^^^^^^^^^^^^

  This is an example for a NTP client test. You can excute it with below options in TASH.
  Options:
      start: Start NTP client daemon
      stop : Terminate NTP client daemon
      status : Show status of NTP client daemon
      date : Show current date and time
      link : Show line status of NTP client daemon

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_NTPCLIENT_TEST

  Depends on:
  * CONFIG_NETUTILS_NTPCLIENT
  * CONFIG_NETUTILS_NTPCLIENT_STACKSIZE
  * CONFIG_NETUTILS_NTPCLIENT_SERVERPRIO
  * CONFIG_NETUTILS_NTPCLIENT_SIGWAKEUP

