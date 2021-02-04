examples/performance/tls_seclink
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  usage:
    ex) tls_seclink

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_TLS_SECLINK

  Depends on:
  * CONFIG_NET_SECURITY_TLS

The purpose of this app is measuring of TLS packet encrypt/decrypt performance.
First, the app creates 2 threads, server side and client side and makes the TLS handshake with 'ECDHE_ECDSA_WITH_AES_128_GCM_SHA256'.
After that, check the start/end time of 'tls write' & 'tls read' while exchanging the packets.
(It uses dummy packets of 64, 128, 256, 512, 1024, 2048, 4096 bytes.)
