README
======

Directory Structure
===================

  tinyara/os/
   |
   `- net/
       |
       +- lwip     - LWIP network stack (IPv4 , TCP , UDP, RAW, DHCP)
       +- netiob   - I/O buffering logic
       +- netdev   - Socket network device interface
       +- socket   - BSD socket interface
       +- route    - Routing table support
       +- tls      - TLS support
       +- utils    - Miscellaneous utility functions


    +----------------------------------------------------------------+
    |                     Application layer                          |
    +----------------------------------------------------------------+
    +----------------------------------------------------------------+
    |                   Socket layer (socket/)                       |
    +----------------------------------------------------------------+
    +------------++--------------------------------------------------+
    |  Network   || LWIP Netwotk Stack (arp, ipv4, icmp, tcp, udp  ) |
    |   Device   |+--------------------------------------------------+
    | Interface  |+------------------------------------++------------+
    | (netdev/)  ||                    Utilities		     |
    +------------++------------------------------------++------------+
    +----------------------------------------------------------------+
    |                    Network Device Drivers                      |
    +----------------------------------------------------------------+
