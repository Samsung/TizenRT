# BLE Manger ITC application

### Integration Test Cases (positive & negative) for the APIs of BLE manager

**Path**
`apps/examples/testcase/ta_tc`

### Menuconfig Setup

**Turn ON ITC application**
>Application Configuration > Examples > TestCase Example > BLE Manager ITC TestCase Example

**Turn ON BLE RMC application**
>Application Configuration > Examples > BLE remote controller example

**Turn ON server on ble_rmc**
- `wm_test start`
- `ble_rmc init`
- `ble_rmc server set`
- `ble_rmc server start`
- `ble_rmc mac` (to find mac addr of the advertising server)

### Execute below commands:
- `wm_test start`
- `blemgr_itc mac_addr_of_server` (e.g: `blemgr_itc 88:57:1d:ee:95:7a`)