
examples/tls_handshake
^^^^^^^^^^^^^^^^^^^^^
  usage: tls_handshake mode
				 mode: server, client
				 
				 server mode
				 TASH> tls_handshake -s

				 client mode
				 TASH> tls_handshake -c target_address
    		 ex) tls_handshake -c 192.168.1.2

	Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_TLS_HANDSHAKE

  Depends on:
  * CONFIG_NET_SECURITY_TLS

