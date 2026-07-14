# Ameba WIFI CSI Demo

## Introduction
- This CSI example demonstrates fetching of CSI data via APIs
- It will print out the partial CSI raw data to loguart
- Wi-Fi CSI (Channel State Information) is a technology used to describe the characteristics of wireless channels

## Requirements
- Works on AmebaSmart (RTL8730E)
- This CSI Demo only works on flat configs like flat_dev_ddr or flat_dev_psram

## CSI Configurations
- There are various CSI configurations (Method), please pick one

### Method1 (Default)
- A Realtek STA transmits a unicast CSI triggering frame to the AP and derives CSI from the ACK response to the previous CSI triggering frame (one-to-one communication)
- By default, this CSI Demo will get the mac address of the connected AP automatically once connected
- No code modification needed

### Method1_Variant (Multi-STA CSI)
- A Realtek STA transmits a unicast CSI triggering frame to another STAx and derives CSI from the ACK response to the previous CSI triggering frame (one-to-many communication via polling)
- CSI driver sets `CSI_SUPPORT_MAX_NUM_STA` to 5 (including the AP)
- At the function `wifi_csi_thread`, at the part of code with the comment `/* config csi parameters and enable wifi csi for second peer device */`, add the following, and change the mac addresses to match the STAs
```c
	act_param.trig_flag = 1;
	memcpy(act_param.mac_addr, (unsigned char[6]){0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f}, 6); /* STA1 mac_addr connected to same AP */
	wifi_csi_config(&act_param);
	act_param.trig_flag = 2;
	memcpy(act_param.mac_addr, (unsigned char[6]){0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f}, 6); /* STA2 mac_addr connected to same AP */
	wifi_csi_config(&act_param);
	act_param.trig_flag = 3;
	memcpy(act_param.mac_addr, (unsigned char[6]){0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f}, 6); /* STA3 mac_addr connected to same AP */
	wifi_csi_config(&act_param);
	act_param.trig_flag = 4;
	memcpy(act_param.mac_addr, (unsigned char[6]){0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f}, 6); /* STA4 mac_addr connected to same AP */
	wifi_csi_config(&act_param);
```
- At the function `wifi_csi_thread_stop`, replace `wifi_csi_config(&act_param);` with the following
```c
	act_param.act = RTW_CSI_ACT_EN;  /* csi dis */
	act_param.enable = 0;
	for (int i = 0; i <= 4; i++) {
		act_param.trig_flag = i;
		wifi_csi_config(&act_param);
	}
```

### Method2 (SoftAP)
- A Realtek SoftAP transmits a unicast CSI triggering frame to associated stations and derives CSI from the ACK response to the previous unicast CSI triggering frame (one-to-many communication via polling)
- Max only 3 STAs can join SoftAP, because at `ameba_wificfg.c` set `wifi_user_config.ap_sta_num = 3;`
- At the function `wifi_csi_thread`, at the part of code with the comment `/* config csi parameters and enable wifi csi for second peer device */`, add the following
```c
	for (int i = 1; i <= 2; i++) {
		memcpy(act_param.mac_addr, client_info.mac_list[i].octet, 6);
		wifi_csi_config(&act_param);
	}
```
- At the function `wifi_csi_thread_stop`, replace `wifi_csi_config(&act_param);` with the following
```c
	for (int i = 0; i <= 2; i++) {
		memcpy(act_param.mac_addr, client_info.mac_list[i].octet, 6);
		wifi_csi_config(&act_param);
	}
```

### Method5 (Passive)
- A Realtek STA/SoftAP derives CSI from received packets in Rx Normal Mode (one-to-one communication)
- At the function `wifi_csi_thread`, at the part of code with the comment `/* config csi parameters and enable wifi csi for first peer device */`, set the following
```c
	act_param.group_num = RTW_CSI_GROUP_NUM_1;
	act_param.mode = RTW_CSI_MODE_NORMAL;
	act_param.accuracy = RTW_CSI_ACCU_1BYTE;
	act_param.trig_period = 200;
	act_param.data_rate = RTW_RATE_MCS0;
	act_param.trig_frame_mgnt = 0;
	act_param.trig_frame_ctrl = 0;
	act_param.trig_frame_data = RTW_CSI_TRIG_QOS_DATA;
	act_param.csi_role = RTW_CSI_OP_ROLE_RX;
```

## Setup
1. Launch dbuild menu
```bash
./os/dbuild.sh menu
```
2. Choose supported chip, RTL8730E, for WIFI CSI Demo
3. Choose config flat_dev_ddr, or flat_dev_psram
4. Open menuconfig by choosing `3. Modify Current Configuration`
5. At menuconfig, enable `CONFIG_EXAMPLES_WIFICSI`
```bash
Application Configuration  --->
    Examples  --->
        Board Specific Demos  --->
            [*] WIFI CSI example
```
6. Exit and save menuconfig
7. Build by choosing `1. Build with Current Configuration`
8. Flash by choosing `d. Download`, `1. ALL`, and port `2. ttyUSB1`

## Usage
- (Method1/Method1_Variant/Method5) Connect to Access Point (AP)
```bash
wm_test start
wm_test join AP_SSID wpa2_aes AP_PASSWORD
```

- (Method2/Method5) Start SoftAP
```bash
wm_test start
wm_test softap SOFTAP_SSID SOFTAP_PASSWORD SOFTAP_CHANNEL
```
- (Method2/Method5) Then have peer devices join SoftAP

- Start wificsi
```bash
wificsi start
```

- Stop wificsi
```bash
wificsi stop
```

## Example CSI logs
```
TASH>>wificsi start
TASH>>### STA Break ###
[CH INFO] csi header info:
# sta_mac_addr     = 9c:44:3d:59:75:be
# trigger_mac_addr = 3c:78:95:53:e3:18
# trigger_flag     = 0
# timestamp        = 11851924 us
# csi_data_len     = 104
# csi_sequence     = 1
# channel          = 13
# bandwidth        = 0
# rx_rate          = 12
# protocol_mode    = 0 [ofdm(0)/ht(1)/vht(2)/he(3)]
# num_sub_carrier  = 52
# num_bit_per_tone = 16 [I:8 bits; Q:8 bits]
# rssi[0]          = -26 dbm
# rxsc             = 0
# csi_valid        = 1
[CH INFO] csi raw data: len = 104[152]
[00]0x0904060604070107
[01]0x0ef90efc0dff0b01
[02]0x09f20bf30df50ef7
[03]0xfdf201f104f007f1
[04]0xf0fdf3f9f6f6faf4
[05]0xe90cea08ec04ee00
[06]0xf516f117eb13e910
[07]0x040e0111fd13f915
[08]0x0d010c040a07080b
[09]0x0cf70ef90efb0efe
[10]0x04f407f409f40bf5
[11]0xfbf8fdf6fff502f4
[12]0xf801f8fff8fdf9fa
[CH INFO] raw data done!
```

# References
- https://aiot.realmcu.com/en/latest/rtos/wifi/csi/index.html
