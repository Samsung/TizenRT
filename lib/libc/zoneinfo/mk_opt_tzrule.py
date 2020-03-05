#!/usr/bin/env python
############################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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

import struct
import time
import os

#zoneinfo data header information
#magic[4] 'TZif'
#version[1] '2' or '3'
#reserved[15]
#ttisgmt[4]
#ttisstd[4]
#leapcnt[4]
#timecnt[4]
#typecnt[4]
#charcnt[4]
#header size = 44

#for i : timecnt
# at (int) or (long long)
#for i : timecnt
# typ (char) 
#for i : typecnt
# gmtoff(int)
# isdst(char)
# abbrind(char)
#for i : charcnt
# chars(char)
#for i : leapcnt
# trans(int) or (long long)
# corr(int)
#for i : typecnt
# if(ttisstd != 0)
# ttisstd(char)
#for i : typecnt
# if(ttisgmt != 0)
# ttisgmt(char)

MAGIC_SIZE    = 4
VERSION_SIZE  = 1
RESERVED_SIZE = 15
TTISGMT_SIZE  = 4
TTISSTD_SIZE  = 4
LEAPCNT_SIZE  = 4
TIMECNT_SIZE  = 4
TYPECNT_SIZE  = 4
CHARCNT_SIZE  = 4
INT_SIZE      = 4
CHAR_SIZE     = 1

def latest_rule(file_name):
    now = int(time.time())
    fp = open(file_name, "rb")
    data = fp.read()
    fp.close()

    fp = open(file_name, "wb")

    store_size = [4, 8]
    point = 0

    for size in store_size:
        magic = data[point:point + MAGIC_SIZE]
        fp.write(magic)
        point += MAGIC_SIZE
        version = data[point:point + VERSION_SIZE]
        fp.write(version)
        point += VERSION_SIZE
        reserved = data[point:point + RESERVED_SIZE]
        fp.write(reserved)
        point += RESERVED_SIZE

        ttisgmt = struct.unpack('>I', data[point : point + TTISGMT_SIZE])[0]
        fp.write(struct.pack('>I', ttisgmt))
        point += TTISGMT_SIZE

        ttisstd = struct.unpack('>I', data[point : point + TTISSTD_SIZE])[0]
        fp.write(struct.pack('>I', ttisstd))
        point += TTISSTD_SIZE

        leapcnt = struct.unpack('>I', data[point : point + LEAPCNT_SIZE])[0]
        fp.write(struct.pack('>I', leapcnt))
        point += LEAPCNT_SIZE

        timecnt = struct.unpack('>I', data[point : point + TIMECNT_SIZE])[0]
        point += TIMECNT_SIZE

        typecnt = struct.unpack('>I', data[point : point + TYPECNT_SIZE])[0]
        point += TYPECNT_SIZE

        charcnt = struct.unpack('>I', data[point : point + CHARCNT_SIZE])[0]
        point += CHARCNT_SIZE

        at_list = list()
        index = 0
        for i in range(0, timecnt):
            if size == 4 :
                at = struct.unpack('>i', data[point : point + size])[0]
            else :
                at = struct.unpack('>q', data[point : point + size])[0]

            if at >= now and index == 0:
                index = i - 1

            point += size
            at_list.append(at)

        if index == 0 and timecnt >= 1:
            index = timecnt - 1

        fp.write(struct.pack('>I', timecnt - index))
        fp.write(struct.pack('>I', typecnt))
        fp.write(struct.pack('>I', charcnt))

        for i in range(index, timecnt) :
            if size == 4 :
                fp.write(struct.pack('>i', at_list[i]))
            else :
                fp.write(struct.pack('>q', at_list[i]))

        typ_list = list()
        for i in range(0, timecnt):
            typ = struct.unpack('b', data[point : point + CHAR_SIZE])[0]
            point += CHAR_SIZE
            typ_list.append(typ)

        for i in range(index, timecnt):
            fp.write(struct.pack('b', typ_list[i]))

        for i in range(0, typecnt):
            gmtoff = struct.unpack('>i', data[point : point + INT_SIZE])[0]
            fp.write(struct.pack('>i', gmtoff))
            point += INT_SIZE

            isdst = struct.unpack('b', data[point : point + CHAR_SIZE])[0]
            fp.write(struct.pack('b', isdst))
            point += CHAR_SIZE

            abbrind = struct.unpack('b', data[point : point + CHAR_SIZE])[0]
            fp.write(struct.pack('b', abbrind))
            point += CHAR_SIZE

        chars = data[point : point + charcnt]
        fp.write(chars)
        point += charcnt

        for i in range(0, leapcnt):
            if size == 4 :
                trans = struct.unpack('>i', data[point : point + size])[0]
                fp.write(struct.pack('>i', trans))
            else :
                trans = struct.unpack('>q', data[point : point + size])[0]
                fp.write(struct.pack('>q', trans))
            point += size

            corr = struct.unpack('>i', data[point : point + INT_SIZE])[0]
            fp.write(struct.pack('>i', corr))
            point += INT_SIZE

        if ttisstd != 0:
            for i in range(0, typecnt):
                std = struct.unpack('b', data[point : point + CHAR_SIZE])[0]
                fp.write(struct.pack('b', std))
                point += CHAR_SIZE

        if ttisgmt != 0:
            for i in range(0, typecnt):
                gmt = struct.unpack('b', data[point : point + CHAR_SIZE])[0]
                fp.write(struct.pack('b', gmt))
                point += CHAR_SIZE

        if version == '\0':
            break

    fp.write(data[point:])
    fp.close()

def files_in_dir(root_dir):
    files = os.listdir(root_dir)
    for file in files:
        path = os.path.join(root_dir, file)
        if os.path.isdir(path):
            files_in_dir(path)
        else :
            latest_rule(path)

root_dir = "./tzbin/usr/share/zoneinfo/"
files_in_dir(root_dir)

