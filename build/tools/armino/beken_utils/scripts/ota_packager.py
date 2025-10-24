#!/usr/bin/env python3
import os
import binascii
import gzip
import struct
import zlib
import json
import logging

from Crypto import Random
from Crypto.Cipher import AES
from .common import *

class AESUtil:

    __BLOCK_SIZE_16 = BLOCK_SIZE_16 = AES.block_size

    @staticmethod
    def encryt(string, key, iv):
        """
        加密文本
        :param string: 文本
        :param key: 密钥
        :param iv: 偏移量/初始向量
        :return: 密文
        """
        # print (string)
        # print(binascii.hexlify(string).decode())

        cipher = AES.new(key, AES.MODE_CBC, iv)
        x = AESUtil.__BLOCK_SIZE_16 - (len(string) % AESUtil.__BLOCK_SIZE_16)
        # 如果最后一块不够16位需要用字符进行补全
        if x != 0:
            string = string + bytes(chr(x)*x, encoding="utf-8")

        # print(binascii.hexlify(string).decode())
        msg = cipher.encrypt(string)
        # print(binascii.hexlify(msg).decode())

        # msg = base64.urlsafe_b64encode(msg).replace('=', '')
        # msg = base64.b64encode(msg)
        return msg

    @staticmethod
    def decrypt(en_str, key, iv):
        cipher = AES.new(key, AES.MODE_CBC, iv)
        # en_str += (len(en_str) % 4)*"="
        # decryptByts = base64.urlsafe_b64decode(en_str)
        # decryptByts = base64.b64decode(en_str)
        # msg = cipher.decrypt(decryptByts)

        msg = cipher.decrypt(en_str)
        padding_len = msg[len(msg)-1]
        return msg[0:-padding_len]


def crc32(bytes_obj):
    hashs = 0
    hashs = zlib.crc32(bytes_obj)
    return hashs


def fnv1a_r(oneByte, has):
    return ((oneByte ^ has)*0x1000193) & 0xFFFFFFFF


def getHashCode(data):
    has = 0x811c9dc5
    for i in range(len(data)):
        has = fnv1a_r(data[i], has)
    return has


def gethead(compress, encrypt, firmware_name, firmware_version, filename, ori_obj, dst_obj):
    # ("type_4")
    type_name = b'RBL'
    type_4 = struct.pack('4s', type_name)

    # ("fota_algo")
    if compress == "1" and encrypt == "2":
        # int(statinfo.st_ctime) #===============>258 = 0x0201 :02=aes256 | 01 : gzip ;02 : quicklz ;03:fastlz
        fota_algo = 258
    elif compress == "0" and encrypt == "2":
        fota_algo = 2
    elif compress == "1" and encrypt == "0":
        fota_algo = 256
    elif compress == "2" and encrypt == "0":
        fota_algo = 512
    elif compress == "2" and encrypt == "2":
        fota_algo = 514
    else:
        fota_algo = 0
    algo = struct.pack('<H', fota_algo)

    # ("ctime")
    statinfo = os.stat(filename)

    ctime = int(statinfo.st_mtime)
    ct = struct.pack('<I', ctime)
    ct = b'\x00'*(6-len(ct)) + ct

    # ("app_part_name")
    app_part_name = firmware_name
    apn = struct.pack('16s', app_part_name.encode())

    # ("download_version")
    download_version = firmware_version
    dv = struct.pack('24s', download_version.encode())

    # ("current_version")
    current_version = b'00010203040506070809'
    cv = struct.pack('24s', current_version)

    # ("body_crc32")
    body_crc32 = struct.pack('<I', crc32(dst_obj))

    # ("HASH_CODE")
    hash_val = struct.pack('<I', getHashCode(ori_obj))

    raw_size = len(ori_obj)
    raw_size = struct.pack('<I', raw_size)
    com_size = len(dst_obj)
    com_size = struct.pack('<I', com_size)

    head = type_4 + algo + ct + apn + dv + cv + \
        body_crc32 + hash_val + raw_size + com_size
    head = head + struct.pack('<I', crc32(head))
    return head


class ota_packager:
    def __init__(self, infile, compress, encrypt, keyword, iv, firmware_name, firmware_version, outfile):
        if not os.path.exists("scripts/gzipHeader.bin"):
            logging.error(f'"gzipHeader.bin" not exists')
            exit(0)

        key = keyword.encode()
        iv = iv.encode()

        filename = infile
        with open(filename, "rb") as f:
            bytes_obj = f.read()
            logging.info("加密压缩前文件长度：", len(bytes_obj))
            # gizp 压缩
            dest_obj = bytes_obj
            if compress == "1":
                with open("scripts/gzipHeader.bin", "rb") as f:
                    gzipheader = f.read()
                    logging.info("gzip文件头", binascii.hexlify(gzipheader).decode())
                    logging.info("gzip文件头长度", len(gzipheader))

                tmp_gizp = gzip.compress(bytes_obj, compresslevel=6)
                dest_obj = gzipheader + tmp_gizp[10:]
            # aes 加密
            if encrypt == "2":
                dest_obj = AESUtil.encryt(dest_obj, key, iv)
            my_head = gethead(compress, encrypt, firmware_name,
                              firmware_version, filename, bytes_obj, dest_obj)
            # print(binascii.hexlify(my_head).decode())
            # print (my_head)
            logging.info("加密压缩后文件长度：", len(dest_obj))

        with open(outfile, "wb") as f:
            f.write(dest_obj)

            config = "configurationab.json"
            with open(config,"rb") as fj:
                json_dict = json.load(fj)
                if "section" in json_dict:
                    section = json_dict["section"]
                    size = section[1]["size"]
                    iSize = size2int(size)
                    for i in range(iSize - len(dest_obj)):
                        a = struct.pack('B',255)
                        f.write(a)
            f.write(my_head)

