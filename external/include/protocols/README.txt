protocols README.txt
^^^^^^^^^^^^^^^^^^^

Contents
--------

  - Network Applications
  - Tips for Using DHCPC

Network Applications
^^^^^^^^^^^^^^^^^^^^

This directory contains most of the network applications.
These applications are not all heavily tested.
Apps include:

  dhcpc     - Dynamic Host Configuration Protocol (DHCP) client.
              See external/include/protocols/dhcpc.h for interface information.
  dhcpd     - Dynamic Host Configuration Protocol (DHCP) server.
              See external/include/protocols/dhcpd.h for interface information.
  ftpc      - FTP client.
              See external/include/protocols/ftpc.h for interface information.
  ftpd      - FTP server.
              See external/include/protocols/ftpd.h for interface information.
  mdns      - multicast Domain Name System (mDNS).
              See external/mdns/mdns.h for interface information.
  netlib    - network libraries that includes IP address support, HTTP support and generic server logic.
              See external/netlib.h for interface information.
  ntpclient - This is a fragmentary NTP client. It neither well-tested
              nor mature nor complete at this point in time.
              See external/include/protocols/ntpclient.h for interface information.
  smtp      - Simple Mail Transfer Protocol (SMTP) client.
              See external/include/protocols/smtp.h for interface information.
  telnetd   - TELNET server.
              See external/include/protocols/telnetd.h for interface information.
  tftpc     - TFTP client.
              See external/include/protocols/tftp.h for interface information.
  webclient - HTTP web client.
              See external/include/protocols/webclient.h for interface information.
  webserver - HTTP web server.
              See os/include/external/webserver/http_server.h for interface information.
  websocket - websocket server and client.
              See external/include/protocols/websocket.h for interface information.
  xmlrpc    - The Embeddable Lightweight XML-RPC Server discussed at
              http://www.drdobbs.com/web-development/an-embeddable-lightweight-xml-rpc-server/184405364.
              See external/include/protocols/xmlrpc.h for interface information.

Tips for Using DHCPC
^^^^^^^^^^^^^^^^^^^^

If you use DHCPC, this configuration network option is required.
  CONFIG_NETUTILS_DHCPC=y
