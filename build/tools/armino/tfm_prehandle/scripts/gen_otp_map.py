#!/usr/bin/env python3

import logging
import os
import glob
import re
from .otp import *
from .gen_license import get_license
from .common import *

def gen_otp_map_file():
    otp_pattern = 'otp[0-9]*.csv'
    otp_list = glob.glob(otp_pattern)

    if not otp_list:
        logging.debug(f'no otp file')
        return

    otp_instance = []
    regex = re.compile(r'otp(\d+)\.csv')
    sorted_list = sorted(otp_list,key=lambda x:int(regex.search(x).group(1)) if regex.search(x) else -1)
    for otp_csv in sorted_list:
        match = regex.search(otp_csv)
        number = match.group(1)
        print(otp_csv)
        otp = OTP(otp_csv, number)
        otp_instance.append(otp)

    f = open('_otp.h',"w+")
    logging.debug(f'Create _otp.h')

    f.write(get_license())

    f.write("\n#include <stdint.h>\n")

    f.write("#define BK_ERR_OTP_READ_BUFFER_NULL   (BK_ERR_OTP_BASE - 1) /**< The pointed buff is NULL */\n")
    f.write("#define BK_ERR_OTP_ADDR_OUT_OF_RANGE  (BK_ERR_OTP_BASE - 2) /**< OTP address is out of range */\n")
    f.write("#define BK_ERR_NO_READ_PERMISSION     (BK_ERR_OTP_BASE - 3) /**< Not have read privilege  */\n")
    f.write("#define BK_ERR_NO_WRITE_PERMISSION    (BK_ERR_OTP_BASE - 4) /**< Not have write privilege */\n")
    f.write("#define BK_ERR_OTP_PERMISSION_WRONG   (BK_ERR_OTP_BASE - 5) /**< OTP permission value error*/\n")
    f.write("#define BK_ERR_OTP_NOT_EMPTY          (BK_ERR_OTP_BASE - 6) /**< try to write area already has value*/\n")
    f.write("#define BK_ERR_OTP_UPDATE_NOT_EQUAL   (BK_ERR_OTP_BASE - 7) /**< after update not equal to expected*/\n")
    f.write("#define BK_ERR_OTP_OPERATION_ERROR    (BK_ERR_OTP_BASE - 8) /**< otp operation error*/\n")
    f.write("#define BK_ERR_OTP_OPERATION_WARNING  (BK_ERR_OTP_BASE - 9) /**< otp operation warning*/\n")
    f.write("#define BK_ERR_OTP_OPERATION_WRONG    (BK_ERR_OTP_BASE - 10) /**< otp operation wrong*/\n")
    f.write("#define BK_ERR_OTP_OPERATION_FORBID   (BK_ERR_OTP_BASE - 11) /**< otp operation forbid*/\n")
    f.write("#define BK_ERR_OTP_OPERATION_FAIL     (BK_ERR_OTP_BASE - 12) /** otp operation other error*/\n")
    f.write("#define BK_ERR_OTP_INIT_FAIL          (BK_ERR_OTP_BASE - 13) /** otp init fail*/\n")
    f.write("#define BK_ERR_OTP_INDEX_WRONG        (BK_ERR_OTP_BASE - 14) /**< OTP item index error*/\n")
    f.write("#define BK_ERR_OTP_CRC_WRONG          (BK_ERR_OTP_BASE - 15) /**< OTP item index error*/\n")

    f.write("\ntypedef enum{\n")
    f.write("    OTP_READ_WRITE = 0,\n")
    f.write("    OTP_READ_ONLY = 0x3,\n")
    f.write("    OTP_NO_ACCESS = 0xF,\n")
    f.write("} otp_privilege_t;\n")

    f.write("\ntypedef enum{\n")
    f.write("    OTP_SECURITY = 0,\n")
    f.write("    OTP_NON_SECURITY,\n")
    f.write("} otp_security_t;\n")

    f.write("\ntypedef enum{\n")
    f.write("    OTP_NEED_CRC = 0,\n")
    f.write("    OTP_NO_NEED_CRC,\n")
    f.write("} otp_crc_t;\n")


    f.write("\ntypedef struct\n")
    f.write("{\n")
    f.write("    uint32_t  name;\n")
    f.write("    uint16_t  allocated_size;\n")
    f.write("    uint16_t  offset;\n")
    f.write("    otp_privilege_t privilege;\n")
    f.write("    otp_security_t  security;\n")
    f.write("    otp_crc_t  crc_en;\n")
    f.write("} otp_item_t;\n")

    max_row = []
    for otp in otp_instance:
        f.write("\ntypedef enum{\n")
        max_row.append(len(otp.data))
        for id,item in otp.data.items():
            f.write('    ' + item['name'] + ','+ '\n')
        f.write('    ' + 'OTP' + otp.index + '_MAX_ID' + ','+ '\n')
        f.write(f"}} otp{otp.index}_id_t;\n")
        f.write(f"\nextern const otp_item_t otp_map_{otp.index}[{len(otp.data)}];\n")
        f.write(f"\nuint32_t otp_map_{otp.index}_row(void);\n")
        f.write(f"\nuint32_t otp_map_{otp.index}_col(void);\n")

    f.write(f"\nextern const otp_item_t *otp_map;\n")

    f.close()

    f = open("_otp.c","w+")

    logging.debug(f'Create _otp.c')
    f.write(get_license())
    f.write('\n#include "_otp.h"\n')
    f.write('\n#include <stddef.h>\n')

    for otp in otp_instance:
        f.write(f"\nconst otp_item_t otp_map_{otp.index}[{max_row[int(otp.index)-1]}] = {{\n")
        max_len = max(len(item['name']) for id,item in otp.data.items())
        for id,item in otp.data.items():
            line = f'    {{%{-max_len-4}s %10d,    0x%x,    %s,    %s,    %s}},\n' \
              %((item['name']+','), item['size'], item['offset'],item['privilege'],item['security'],item['crc'])
            f.write(line)
        f.write("};\n")
        f.write(f"\nuint32_t otp_map_{otp.index}_row(void)\n{{\n")
        f.write(f"    return sizeof(otp_map_{otp.index}) / sizeof(otp_map_{otp.index}[0]);\n}}\n")
        f.write(f"\nuint32_t otp_map_{otp.index}_col(void)\n{{\n")
        f.write(f"    return sizeof(otp_map_{otp.index}) / sizeof(otp_map_{otp.index}[0]);\n}}\n")

    f.write(f"\nconst otp_item_t *otp_map = NULL;\n")

    f.close()