#!/usr/bin/env python
############################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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
############################################################################

import sys
import zlib
import os


def read_nbytes(fp, n):
    data = fp.read(n)
    if len(data) != n:
        return None
    return data


def decompress_file(src, dst):
    with open(src, "rb") as fp:
        while True:
            len_field = read_nbytes(fp, 4)
            if len_field is None:      # Normal Exit
                break

            block_len = int(len_field)
            comp_data = read_nbytes(fp, block_len)
            decomp_data = zlib.decompress(comp_data)
            # Filter ASCII characters:
            # 0x20 (space) - Minimum printable ASCII character
            # 0x0A (LF) - Line feed/newline
            # 0x0D (CR) - Carriage return
            # 0x09 (TAB) - Horizontal tab
            # This keeps only readable text and basic formatting
            try:
                # Python 2 compatibility
                filtered_data = ''.join(chr(ord(b)) for b in decomp_data if 0x20 <= ord(b) or ord(b) in (0x0A, 0x0D, 0x09))
            except TypeError:
                # Python 3 compatibility
                filtered_data = ''.join(chr(b) for b in decomp_data if 0x20 <= b or b in (0x0A, 0x0D, 0x09))
            dst.write(filtered_data)


def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Usage: python logdump_decompress.py <input file> [output file]")
        sys.exit(1)

    in_path = sys.argv[1]
    if len(sys.argv) == 3:
        out_arg = sys.argv[2]
    else:
        base, ext = os.path.splitext(in_path)
        out_arg = "%s_decomp%s" % (base, ext)

    out_stream = open(out_arg, "w")
    decompress_file(in_path, out_stream)
    out_stream.close()


if __name__ == "__main__":
    main()
