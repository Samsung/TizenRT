examples/dtls_server
^^^^^^^^^^^^^^^^^^^

  This is an example to test dtls server.
  For more information, see the examples/dtls_server/dtls_server_main.c.
  There is no usage for server, so it will run by default configuration.
  If you wan to change ciphersuite or other configuration,
  you should change source code manually.

  usage:
    ex) dtlss

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_DTLS_SERVER

  Depends on:
  * CONFIG_NET_TLS

