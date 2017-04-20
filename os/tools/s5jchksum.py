#!/usr/bin/python

import os
import sys
import hashlib

inputfile  = sys.argv[1]
outputfile = sys.argv[2]

header = 32
tail   = 272

with open(inputfile, 'rb') as fin, open(outputfile, 'wb') as fout:
    data = fin.read()
    fout.write(data)

    size = fin.tell()
    fin.seek(header, 0)
    size -= (header + tail)

    data = fin.read(size)
    h = hashlib.sha256(data);
    digest = h.digest()[:4]

    fout.seek(4, 0)
    fout.write(digest)
