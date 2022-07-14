# examples/ble_tester

  This is BLE spec tester.

  Configs (see the details on Kconfig):
  * CONFIG_EXAMPLES_BLE_TESTER

## How to test

Test Environment and Tash Commands are like below.
### 1. Test Environment
  - TP1X Board x 2 (A,B - A is main test board)
### 2. Tash Commands
#### 2-1. WiFi Init 
   - `wm_test start`
#### 2-2. BLE Connection
   - A) `ble_tester init`
   - B) `ble_tester init`
   - A) `ble_tester ready`
#### 2-3. Parameter Setting
   - A) `ble_tester set [SCAN_TIME] [PACKET_SIZE] [PACKET_COUNT]`   
     Recommended Example) `ble_tester set 20 244 100`
     - Scan Time : 20 sec
     - Packet Size : 244 bytes
     - Packet Count : 100
   
#### 2-4. Test
   - In board A, run tash command below ( ex. `ble_tester test1`, `ble_tester test2` ... ) 
   - Adv Interval
     - Performance : 20 ms
     -  Balanced : 1010 ms
     - Powersave : 10240ms

TC | Model | Tash cmd
-- | -- | --
test1 | Scan + Adv(performance) | `ble_tester test1`
test2 | Scan + Adv(balanced) | `ble_tester test2`
test3 | Scan + Adv(powersave) | `ble_tester test3`
test4 | Scan + BLE Write(w/o Resp) | `ble_tester test4`
test5 | Scan + BLE Write(w/ Resp) | `ble_tester test5`
test6 | Adv(balanced) + BLE Write(w/o Resp) | `ble_tester test6`
test7 | Adv(balanced) + BLE Write(w/ Resp) | `ble_tester test7`
