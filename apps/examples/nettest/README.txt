examples/nettest
^^^^^^^^^^^^^^^^

  This is an example for a network test for TCP, UDP, and IP Multicast. You can excute it in TASH.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_NETTEST

	* network_internal_test
	1) getaddrinfo_p
		 - If you want to see that dns packet sent out then you should configure NET_DNS_MAX_TTL to short DNS refresh interval.
