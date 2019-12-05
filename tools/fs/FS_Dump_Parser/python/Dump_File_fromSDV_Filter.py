###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################

#!/usr/bin/python

import sys


read_file = open(sys.argv[1])
write_file = open(sys.argv[2], 'wb')

try:
    for line in read_file.readlines():
        if line.find("dump") >= 0:
            continue
        elif line.find("finish") >= 0:
            continue
        elif len(line) < 32:
            continue

        if line.find("TASH") >= 0:
            parsed_line = line[13:]
        else:
            parsed_line = line[3:]
            
        for i in range(0, 16):            
            str_value = parsed_line.split()[i]
        
            int_value = int(str_value, 16)
        
            byte_value = int_value.to_bytes(1, 'big')
            write_file.write(byte_value)        
    read_file.close()
    write_file.close()

except IOError:
    print >> sys.stderr, 'Can not open the given file.'
