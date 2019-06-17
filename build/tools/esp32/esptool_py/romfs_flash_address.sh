###########################################################################
# 
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
###########################################################################

#!/bin/sh

config_file="../../../os/.config"
flash_base_addr=`awk -F = '/CONFIG_ESP_FLASH_BASE/{print $2}' $config_file`
flash_size_list=`awk -F = '/CONFIG_FLASH_PART_SIZE/{print $2}' $config_file | sed -e 's/,/ /g' | sed -n 's/"/ /pg'`
flash_type_list=`awk -F = '/CONFIG_FLASH_PART_TYPE/{print $2}' $config_file | sed -e 's/,/ /g'| sed -n 's/"/ /pg'`

isromfs_exist=0
romfs_num=0
romfs_addr=0
i=0

#find romfs partition number
for pname in $flash_type_list
do
	if [ "$pname" = "romfs" ]; then
		isromfs_exist=1
		break	
	fi
	romfs_num=`expr $romfs_num + 1`
done

#find romfs partition start address
#if no romfs, return -1;

for psize in $flash_size_list
do
	if [ $isromfs_exist -eq 0 ]; then
		break	
	fi

	if [ $i -ge $romfs_num ]; then
		break	
	fi	
	romfs_addr=`expr $romfs_addr + $psize`
	i=`expr $i + 1`
done

if [ $isromfs_exist -eq 0 ]; then
	romfs_addr=-1
else
	romfs_addr=`expr $romfs_addr \\* 1024```		
	romfs_addr=`printf 0x"%08X" $romfs_addr`
	romfs_addr=`printf 0x"%08X" $(($romfs_addr + ${flash_base_addr}))`
fi
echo $romfs_addr 
