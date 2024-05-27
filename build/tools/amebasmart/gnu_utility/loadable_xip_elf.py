#!/usr/bin/env python
###########################################################################
#
# Copyright 2024 Samsung Electronics All Rights Reserved.
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
import os

filepath = os.path.dirname(__file__) + "/km0_km4_app.bin"
alignment = 4096

def get_offset():
    bin_size = os.path.getsize(filepath)
    # Binary need to be aligned to 4K
    bin_size = (bin_size + alignment - 1) // alignment * alignment
    # 4K manifest before AP binary (ie. running on Tizen Lite) starts
    bin_size = bin_size + alignment
    return hex(bin_size)

