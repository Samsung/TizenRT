examples/websocket
^^^^^^^^^^^^^^^^^^

  This is an example to introduce basic usage of websocket APIs.
  For more information, see the examples/websocket/websocket_main.c

  usage:
  websocket client [addr] [port] [path] [tls_enable] [size] [num]
    ex) websocket client 127.0.0.1 80 \ 0 2048 10
  websocket server [tls_enable]
    ex) websocket server server 1

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_WEBSOCKET

  Depends on:
  * CONFIG_NETUTILS_WEBSOCKET

