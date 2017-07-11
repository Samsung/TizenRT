Compiling without DTLS 
---------

You can then build the client using the commands:
cmake wakaama/examples/client
make

Compiling with DTLS
---------

This client can support DTLS. You can switch by DTLS=1.

DTLS feature requires tinyDTLS.
TinyDTLS is included as a GIT submodule. On first usage, you need to run the following commands to retrieve the sources:
git submodule init
git submodule update

You need to install the packages libtool and autoreconf.

In the wakaama/examples/shared/tinydtls run the following commands:
autoreconf -i
./configure

You can then build the client using the commands:
cmake -DDTLS=1 wakaama/examples/client
make

