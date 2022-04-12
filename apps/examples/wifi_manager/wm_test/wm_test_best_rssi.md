# Best RSSI Access Point Connect Test:

### Setup

- Build TP1x source code with rtl8721csm
- H/W Requirement:
	- Multiple Access Points, some of them have same credential (same SSID & password)
	- Device under test (DUT) – TP1x board - Simulating Samsung DA device
	
## Test guide:
- wm_test connect_by_rssi [SSID_NAME] [SSID_authentication] [SSID_Password] [Repeat Count]
- Example - wm_test connect_by_rssi TP-Link_Hrishikesh wpa2_aes 20212021 3

This Test will scan various access points, for a given SSID (having multiple access points),
It will check whether TP1x board is connected or not connected to the access point having the
best RSSI value.