Prerequisites
=============

1) Clone tinycbor library using following command from the path "iotivity/extlibs/tinycbor":
   git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor

Build Procedure
===============

1) If you are building first time, then delete GBS-ROOT-RI folder from home directory.
   Note: Default build server URL for Tizen is set in gbs configuration file @ iotivity/tools/tizen/.gbs.conf.
         If needed, same can be modified as per appropriate target.
2) Go to "iotivity/" folder.
3) Execute following command(s) to start build based on transport selection required :
    -> Building for all transports :
    $ scons -f resource/csdk/stack/samples/tizen/build/SConscript TARGET_OS=tizen TARGET_TRANSPORT=ALL

    -> Building for a specific transport :
    $ scons -f resource/csdk/connectivity/build/tizen/SConscript TARGET_OS=tizen TARGET_TRANSPORT=IP

    -> Building for multiple transports :
    $ scons -f resource/csdk/connectivity/build/tizen/SConscript TARGET_OS=tizen TARGET_TRANSPORT=IP,BT,BLE

4) By default, above command will build csdk samples also.

Note :- Upon successful execution of above command(s) RI library and sample applications will be
        generated at path "~/GBS-ROOT-RI/local/repos/<target>/armv7l/RPMS".

Procedure to Execute Sample App:
================================
    1) Copy the generated rpms(com-oic-ri-*.armv7l.rpm, com-oic-ri-sample-*.armv7l.rpm) to Tizen Device
       (say /opt/usr/media/xxx/)
    2) sdb shell
    3) su
    4) cd /opt/usr/media/xxx/
    5) change-booting-mode.sh --update

    6) Install the rpms using the following commands:
		a) rpm -Uvh com-oic-ri-*.armv7l.rpm --force --nodeps
		b) rpm -Uvh com-oic-ri-sample-*.armv7l.rpm --force --nodeps
	7) For IP transport, apply net-config smack rules using below command:
		a) chsmack -a "net-config" /usr/apps/com.oic.ri.sample/bin/*
		b) chsmack -e "net-config" /usr/apps/com.oic.ri.sample/bin/*
    7) RPM will be installed in "/usr/apps/"
       Execute the sample app by below commands
        i) cd /usr/apps/com.oic.ri.sample/bin
			a) ./ocserver -o0 [For Server sample]
			b) ./occlient -t2 [For client sample]
			c) ./ocrouting [For Gateway sample]