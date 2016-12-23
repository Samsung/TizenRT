#!/usr/bin/python
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
from socket import *
import time

SOCKET_NAME = '/tmp/osmocom_loader'

s = socket(AF_UNIX, SOCK_STREAM)
s.connect(SOCKET_NAME)

while 1:
  try:
    x = raw_input(">")
    y = len(x) + 1
    s.send(chr(y>>8) + chr(y&255) + x + "\n")
  except:
    print ''
    break

s.close()
