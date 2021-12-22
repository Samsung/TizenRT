/*
 * test_type, test_handler, test_parser, test_command
 */
CT_MEMBER_POOL(CT_ADD_HOSTNAME, _ct_add_hostname, _ct_parse_add_hostname, "add_hostname")
CT_MEMBER_POOL(CT_CLIENT, _ct_client, _ct_parse_client, "client")
CT_MEMBER_POOL(CT_SERVER, _ct_server, _ct_parse_server, "server")
// Following commands are for development and testing units purpose:
CT_MEMBER_POOL(CT_UDP_CLIENT, _ct_udp_client, _ct_parse_udp_client, "udp_client")
CT_MEMBER_POOL(CT_TCP_CLIENT, _ct_tcp_client, _ct_parse_tcp_client, "tcp_client")
CT_MEMBER_POOL(CT_DTLS_CLIENT, _ct_dtls_client, _ct_parse_dtls_client, "dtls_client")
CT_MEMBER_POOL(CT_TLS_CLIENT, _ct_tls_client, _ct_parse_tls_client, "tls_client")
CT_MEMBER_POOL(CT_UDP_SERVER, _ct_udp_server, _ct_parse_udp_server, "udp_server")
CT_MEMBER_POOL(CT_TCP_SERVER, _ct_tcp_server, _ct_parse_tcp_server, "tcp_server")
CT_MEMBER_POOL(CT_DTLS_SERVER, _ct_dtls_server, _ct_parse_dtls_server, "dtls_server")
CT_MEMBER_POOL(CT_TLS_SERVER, _ct_tls_server, _ct_parse_tls_server, "tls_server")
CT_MEMBER_POOL(CT_DNS_SERVICE, _ct_dns_service, _ct_parse_dns_service, "dns_service")
