#!/usr/bin/env python3

import logging
from .common import *
from scripts.rotpk_hash import *

def reverse_order(hex_str):
    hex_str_len = len(hex_str)
    word_len = hex_str_len // 8
    reverse_str = ''
    for i in range(word_len):
        idx = i << 3
        reverse_str = reverse_str + hex_str[idx + 6]
        reverse_str = reverse_str + hex_str[idx + 7]
        reverse_str = reverse_str + hex_str[idx + 4]
        reverse_str = reverse_str + hex_str[idx + 5]
        reverse_str = reverse_str + hex_str[idx + 2]
        reverse_str = reverse_str + hex_str[idx + 3]
        reverse_str = reverse_str + hex_str[idx + 0]
        reverse_str = reverse_str + hex_str[idx + 1]


    logging.debug(f'hex_str={hex_str}')
    logging.debug(f'reverse_str={reverse_str}')
    return reverse_str


def gen_otp_efuse_config_file(aes_type, flash_aes_key, pubkey_pem_file, secureboot_en, boot_ota, outfile):
    f = open(outfile, 'w+')
    logging.debug(f'Create {outfile}')

    otp_efuse_config = {
        "User_Operate_Enable":  "false",
        "Security_Data_Enable": "true",

        "User_Operate":[],
        "Security_Data":[]
    }

    data = {}
    if secureboot_en:
        data["name"] = "efuse"
        data["mode"] = "write"      #if want to read, please configure to "read"
        data["permission"] = "WR" #TODO change to NA
        data["start_addr"] = "0x4B10043C"
        data["last_valid_addr"] = "0x4B100440"
        data["byte_len"] = "0x1"
        if  aes_type == 'FIXED':
            data["data"] = "28000008"   #if want to configure other value, please configure this item_value
        else:
            data["data"] = "8000000"   #TODO temp use 8000000, the final value to be used 8000008 (bit3 reps: open secureboot)
        data["data_type"] = "hex"
        data["status"] = "true"
        otp_efuse_config["Security_Data"].append(data)
    else:
        pass

    if aes_type == 'FIXED':
        data = data.copy()
        data["name"] = "flash_aes_key"
        data["mode"] = "write"      #if want to read, please configure to "read"
        data["permission"] = "WR" #TODO change to NA
        data["start_addr"] = "0x4B100440"
        data["last_valid_addr"] = "0x4B100460"
        data["byte_len"] = "0x20"
        data["data"] = flash_aes_key
        data["data_type"] = "hex"
        data["status"] = "true"
        otp_efuse_config["Security_Data"].append(data)
    else:
        pass

    if secureboot_en:
        h = Rotpk_hash(pubkey_pem_file)
        hash_dict = h.gen_rotpk_hash()
        bl1_pk_hash = hash_dict['bl1_rotpk_hash']
        bl2_pk_hash = hash_dict['bl2_rotpk_hash']

        data = data.copy()
        data["name"] = "bl1_rotpk_hash"
        data["mode"] = "write"      #if want to read, please configure to "read"
        data["permission"] = "WR" #TODO change to RO
        data["start_addr"] = "0x4B100528"
        data["last_valid_addr"] = "0x4B100548"
        data["byte_len"] = "0x20"
        data["data"] = bl1_pk_hash
        data["data_type"] = "hex"
        data["status"] = "true"
        otp_efuse_config["Security_Data"].append(data)

        data = data.copy()
        data["name"] = "bl2_rotpk_hash"
        data["mode"] = "write"      #if want to read, please configure to "read"
        data["permission"] = "WR" #TODO change to RO
        data["start_addr"] = "0x4B100548"
        data["last_valid_addr"] = "0x4B100568"
        data["byte_len"] = "0x20"
        data["data"] = bl2_pk_hash
        data["data_type"] = "hex"
        data["status"] = "true"
        otp_efuse_config["Security_Data"].append(data)
    else:
        h = Rotpk_hash(pubkey_pem_file, False)
        hash_dict = h.gen_rotpk_hash()
        bl2_pk_hash = hash_dict['bl2_rotpk_hash']
        data = data.copy()
        data["name"] = "bl2_rotpk_hash"
        data["mode"] = "write"
        data["permission"] = "WR" #TODO change to RO
        data["start_addr"] = "0x4B100548"
        data["last_valid_addr"] = "0x4B100568"
        data["byte_len"] = "0x20"
        data["data"] = bl2_pk_hash
        data["data_type"] = "hex"
        data["status"] = "true"
        otp_efuse_config["Security_Data"].append(data)

    json_str = json.dumps(otp_efuse_config, indent=4)
    with open('otp_efuse_config.json', 'w',newline="\n") as file:
        file.write(json_str)
        if (secureboot_en):
            if (flash_aes_key):
                aes = f"\n# flash_aes_key in little endian:{reverse_order(flash_aes_key)}\n"
                file.write(aes)

            bl1 = f"# bl1_rotpk_hash in little endian:{reverse_order(bl1_pk_hash)}\n"
            file.write(bl1)
            bl2 = f"# bl2_rotpk_hash in little endian:{reverse_order(bl2_pk_hash)}\n"
            file.write(bl2)
            file.write(f"# data[\"mode\"] = \"write\" if want to read, please configure to \"read\"\n")
    file.close()
