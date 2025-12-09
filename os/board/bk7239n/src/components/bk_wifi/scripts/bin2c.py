#!/usr/bin/env python3

import os
import sys
import argparse
import binascii

number_of_columns = 16

copyright = """// Copyright 2020-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

"""

header = """
const uint8_t regulatory_db[] = {
"""

c_list = []


# Get component name from the command line
parser = argparse.ArgumentParser()
parser.add_argument("bin_path", help="Specify path the input binary file to be converted to a C array.")
parser.add_argument("c_path", help="Specify path the output C file with the C array.")
args = parser.parse_args()
bin_path = args.bin_path
c_path = args.c_path

c_file=os.path.basename(c_path)

f = open(bin_path, "rb")
data = list(f.read())

with open(c_path, "w") as c_fd:

    # Clear content of the file
    c_fd.seek(0)
    c_fd.truncate()

    # Write copyright
    c_fd.write(copyright.format(c_file))

    # Include headers
    c_fd.write("#include <stdint.h>\n")

    # Write template
    c_fd.write(header)

    # Generate list of the data bytes
    for n in data:
        c_list.append(format(n, '#04x'))

    for i in range(int(len(c_list) / number_of_columns) + 1):
        line_list = c_list[i * number_of_columns: (i + 1) * number_of_columns]
        if line_list:
            c_fd.write("   ")
        for item in line_list:
            c_fd.write(" %s," % item)
        c_fd.write("\n")

    c_fd.write("};\n\n")

    c_fd.write("int regulatory_db_size = sizeof(regulatory_db);\n");
f.close()
