Setup guide:
a)Build TP1x source code in with rtl8721csm/security_hal_test_tz config
b)H/W Requirement:
	1)Access Point
	2)Device under test (DUT) – TP1x board - Simulating connect and data transfer process on Samsung DA device
	3)Client Device - TP1x board - Simulating connect and data transfer process on mobile SmartThings Application 
c)Add hostname in input file
	wm_test add_hostname [filepath] [hostname]
	e.g. wm_test add_hostname /mnt/wt_hostname.txt www.samsung.com 
	
Test guide:
DUT(Connect Server): 
connect_test server [ap ssid] [ap security mode] [ap password] [softap ssid] [softap password] [softap channel] [repeat count] [udp/dtls]* [udp/dtls data size in KB]** [tcp/tls]* [tcp/tls data size in KB]** [hostname filepath]***
e.g. connect_test server Infa_6A_3 wpa2_aes 12345678 tp1x 12341234 1 3 udp 100 tcp 100 /mnt/wt_hostname.txt

Client device(Connect Client): 
connect_test client [ap ssid] [ap security mode] [ap password] [softap ssid] [softap password] [softap channel] [repeat count] [udp/dtls]* [udp/dtls data size in KB]** [tcp/tls]* [tcp/tls data size in KB]**
e.g. connect_test client Infa_6A_3 wpa2_aes 12345678 tp1x 12341234 1 3 udp 100 tcp 100

Notes:
	*   optional if given data size in next argument is mandatory
	**  must be given in pair to previous argument
	*** Optional if not given 3 domains names in script will be used

Test List:
This test covers following items:-
1) Wifi Mode Change of server - From STA mode to SoftAP mode.
2) Scan in SoftAP mode by server.
3) Connect to SoftAP server by client.
4) Data Tranfer through UDP/DTLS protocol.
5) Wifi Mode Change of server - From SoftAP mode to STA mode.
6) Scan in STA disconnected state by server.
7) Connect both devices to same AP.
8) Server provides DNS Service which returns IP Address of given hostnames.
9) Data Transfer through TCP/TLS protocol.

Performance is measured by time taken in steps 1,2,4,5,6,9. 
