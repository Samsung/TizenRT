#!/usr/bin/env python3
import os
import logging

class copy:

    def __init__(self, srcfile, dstfile, outfile, srcoffset_str, dstoffset_str, size):
        srcfile_sz = os.path.getsize(srcfile)
        dstfile_sz = os.path.getsize(dstfile)

        length = int(size, 16)
        if (length == 0):
            length = srcfile_sz

        if (srcoffset_str == '0xFFFFFFFF'):
            srcoffset = srcfile_sz - length
        else:
            srcoffset = int(srcoffset_str, 16)

        if (dstoffset_str == '0xFFFFFFFF'):
            dstoffset = dstfile_sz - length
        else:
            dstoffset = int(dstoffset_str, 16)

        srcfile_base = os.path.basename(srcfile)
        dstfile_base = os.path.basename(dstfile)
        outfile_base = os.path.basename(outfile)
        logging.info(f'copy from {srcfile_base} offset %x len %x' %(srcoffset, length))
        logging.info(f'to {dstfile_base} offset %x' %(dstoffset))
        with open(outfile, 'wb+') as outfile_opened:
            with open(dstfile, 'rb') as dstfile:
                buf = dstfile.read()
                outfile_opened.write(buf)
                with open(srcfile, 'rb') as srcfile_opened:
                    srcfile_opened.seek(srcoffset)
                    buf = srcfile_opened.read(length)
                    outfile_opened.seek(dstoffset)
                    outfile_opened.write(buf)

