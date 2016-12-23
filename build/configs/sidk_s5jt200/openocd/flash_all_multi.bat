rem
rem Copyright 2016 Samsung Electronics All Rights Reserved.
rem
rem Licensed under the Apache License, Version 2.0 (the "License");
rem you may not use this file except in compliance with the License.
rem You may obtain a copy of the License at
rem
rem http://www.apache.org/licenses/LICENSE-2.0
rem
rem Unless required by applicable law or agreed to in writing,
rem software distributed under the License is distributed on an
rem "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
rem either express or implied. See the License for the specific
rem language governing permissions and limitations under the License.
rem
start  openocd -f s5jt200_silicon_evt0_fusing_flash_all.cfg -c "s5j_set_ft2232_serial ARTIK051_00A"
sleep 1
start  openocd -f s5jt200_silicon_evt0_fusing_flash_all.cfg -c "s5j_set_ft2232_serial ARTIK051_01A"
sleep 1
start  openocd -f s5jt200_silicon_evt0_fusing_flash_all.cfg -c "s5j_set_ft2232_serial ARTIK051_02A"
sleep 1
start  openocd -f s5jt200_silicon_evt0_fusing_flash_all.cfg -c "s5j_set_ft2232_serial ARTIK051_03A"
sleep 1
