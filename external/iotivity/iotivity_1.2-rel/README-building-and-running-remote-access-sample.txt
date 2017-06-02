== Quick guide: build and run IoTivity(remote access) projects on Ubuntu ==

1. Build
	Go to the top directory of 'iotivity' project
    (Note: should always run 'scons' command in this directory)

    Install external libraries:
      $ sudo apt-get install libboost-dev libboost-program-options-dev libboost-thread-dev uuid-dev libssl-dev
      $ sudo apt-get install libglib2.0-dev autoconf libtool

    Build release binaries:
      $ scons WITH_RA=1 WITH_RA_IBB=1
(Note: C sdk requires tiny-cbor. Please follow the instruction in the build
message to install tiny-cbor)

    Build debug binaries:
      $ scons WITH_RA=1 WITH_RA_IBB=1 RELEASE=false

    Help:
      $ scons -h

    Clear:
      $ scons -c

2. Run the remote access testing
    remote access samples in <iotivity>/out/linux/x86_64/release/resource/csdk/stack/samples/linux/SimpleClientServer

    before testing,
      setup a XMPP server and create an account in XMPP server
      ex: account: user1@localhost / password: 1234

    oic server:
      $ ocserver -o 1 -u user1 -d localhost -w 1234 -s localhost

    oic client:
      $ ocremoteaccessclient -t 1 -u user1 -d localhost -w 1234 -s localhost

    test steps:
      1. run ocserver, and copy the bound jid
      2. run ocremoteaccessclient on the another term, and paste the bound jid of ocserver and press 'Enter'
        use '-t' to change testcase.

