# How to run network stack on QEMU
This document explains steps to be followed to run network stack on QEMU.  

## Steps to verify network stack on QEMU is as follows

Prerequisites: Connect WiFi AP to Linux machine(wired connection. dhcp should be enabled in the AP).  

### TizenRT QEMU image build procedure on Host
1. cd os/tools
2. ./configure.sh qemu/tc_16m
3. cd ..
4. make


### Bringup tap interface on Host
Create bridge and tap interface by adding below commands into shell script file(net_up.sh).  
script to bring up the tap interface on QEMU in bridged mode first parameter is name of tap device (e.g. tap0).  
```
#!/bin/sh
ETH0IPADDR=192.168.1.20
MASK=255.255.255.0
GATEWAY=192.168.1.1
BROADCAST=192.168.1.255

#First take enp3s0 down, then bring it up with IP address 0.0.0.0
/sbin/ifdown enp3s0
/sbin/ifconfig enp3s0 0.0.0.0 promisc up

#Bring up the tap device (name specified as first argument, by QEMU)
/usr/sbin/openvpn --mktun --dev $1 --user `id -un`
/sbin/ifconfig $1 0.0.0.0 promisc up

#create the bridge between enp3s0 and the tap device
/sbin/brctl addbr br0
/sbin/brctl addif br0 enp3s0
/sbin/brctl addif br0 $1

#only a single bridge so loops are not possible, turn off spanning tree protocol
/sbin/brctl stp br0 off
/sbin/ifconfig br0 $ETH0IPADDR netmask $MASK broadcast $BROADCAST
/sbin/route add default gw $GATEWAY

#stop firewall - comment this out if you don't use Firestarter
/sbin/service firestarter stop
```
save and execute the shell script with tap0 as argument(ex: sudo ./net_up.sh tap0).  

### Run Qemu with netwrok interface as below
```
$sudo qemu-system-arm -M lm3s6965evb -kernel ../build/output/bin/tinyara.bin -nographic -net nic -net tap,ifname=tap0,script=no
```
### Bringup Interface on QEMU
```
#bringup interface
$ifup en1

#Get IP from AP
$ifconfig en1 dhcp
```
Now Ping to Host IP(192.xxx.xx.xxx) and ping from Host to Qemu(192.xxx.xx.xxx).  

### Bring down interface
To bring down tap0 and bridge interfaces add the below commands into shell script(net_down.sh) and execute shell script.  
```
#!/bin/sh

#Bring down enp3s0 and br0
/sbin/ifdown enp3s0
/sbin/ifdown br0
/sbin/ifconfig br0 down

#Delete the bridge
/sbin/brctl delbr br0

#bring up enp3s0 in "normal" mode
/sbin/ifconfig enp3s0 -promisc
/sbin/ifup enp3s0

#delete the tap device
/usr/sbin/openvpn --rmtun --dev $1

#start firewall again
/sbin/service firestarter start
```
Save and execute the shell script with tap0 as argument(ex: sudo ./net_down.sh tap0).  
