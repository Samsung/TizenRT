examples/webserver
^^^^^^^^^^^^^^^^^^

  This is an example of webserver.
  It is executed by "webserver <operation>" command.
  The <operation> is one of "start" or "stop".
  If <operation> is "start", it starts a HTTP server with port 80 and a HTTPS server with port 443.
  But if CONFIG_NET_SECURITY_TLS is not defined, it starts only HTTP server.
  If <operation> is "stop", it stops both server.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_WEBSERVER

  Depends on:
  * CONFIG_NETUTILS_WEBSERVER

