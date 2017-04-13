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

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_DTLS_CLIENT

  Depends on:
  * CONFIG_NET_TLS

