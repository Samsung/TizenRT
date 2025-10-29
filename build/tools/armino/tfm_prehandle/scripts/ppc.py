#!/usr/bin/env python3
import os
import logging
import csv
import struct

from .parse_csv import *

ppc_keys = [
    'Device',
    'Secure',
    'Privilege'
]

gpio_keys = [
    'GPIO',
    'Device'
]

class PPC(list):
    def check_gpio_security(self):
        gpio_dev_map_dic = {}
        for dict in self.gpio_dev_csv.dic_list:
            gpio_dev_map_dic[dict["GPIO"]] = dict["Device"]
            # logging.debug(f'gpio_dev_csv: {dict["GPIO"]}:{dict["Device"]}')

        dev_secure_dic = {}
        for dict in self.csv.dic_list:
            if (dict["Device"].find("GPIO") == -1) or (dict["Device"] == "GPIOHIG"):
                dev_secure_dic[dict["Device"]] = dict["Secure"]
                continue

            if (dict["Secure"] == "FALSE") and (dev_secure_dic[gpio_dev_map_dic[dict["Device"]]] == "TRUE"):
                dict["Secure"] = "TRUE"
                logging.error(f'The security of {dict["Device"]} and {gpio_dev_map_dic[dict["Device"]]} mismatch')
                #exit(1)
            elif (dict["Secure"] == "TRUE") and (dev_secure_dic[gpio_dev_map_dic[dict["Device"]]] == "FALSE"):
                dict["Secure"] = "FALSE"
                logging.error(f'The security of {dict["Device"]} and {gpio_dev_map_dic[dict["Device"]]} mismatch')
                #exit(1)

    def __getitem__(self, key):
        return self.csv.dic[key]

    def gen_ppc_bin(self):
        ppro = [0] * 12
        if not os.path.exists('ppro_ap.csv') or not os.path.exists('ppro_sec.csv') or not os.path.exists('ppro_default.csv'):
            return

        with open('ppro_default.csv',newline ="") as def_file:
            def_reader = csv.DictReader(def_file)
            data_def = list(def_reader)
            for row in data_def:
                if row['Value'] == "1":
                    reg_def = int(row["Reg"])
                    bit_def = int(row["Bit"])
                    ppro[reg_def] |= 1 << bit_def

        with open('ppro_ap.csv',newline="") as apfile, open("ppro_sec.csv",newline="") as secfile:
            ap_reader = csv.DictReader(apfile)
            data_ap = list(ap_reader)
            sec_reader = csv.DictReader(secfile)
            data_sec = list(sec_reader)

        for dict in self.csv.dic_list:
            device = dict["Device"]
            sec = dict["Secure"]
            privilege = dict["Privilege"]

            if device.startswith("GPIO") and not device == "GPIOHIG" and not sec == "TRUE":
                index = int(device[4:])
                if index < 32:
                    ppro[0] |= (1 << index)
                else:
                    ppro[1] |= (1 << (index - 32))

            for row in data_ap:
                if row['Device'] == device:
                    reg_ap = int(row['Reg'],16) - 4
                    bit_ap = int(row['Bit'])
                    if privilege == "TRUE":
                        ppro[reg_ap] &= ~(1 << bit_ap)
                    else:
                        ppro[reg_ap] |= 1 << bit_ap
                    break
            for row in data_sec:
                if row['Device'] == device:
                    reg_sec = int(row['Reg'],16) - 4
                    bit_sec = int(row['Bit'])
                    if sec == "TRUE":
                        ppro[reg_sec] &= ~(1 << bit_sec)
                    else:
                        ppro[reg_sec] |= 1 << bit_sec
                    break

        with open("ppc_config.bin",'wb') as file:
            for item in ppro:
                data = struct.pack('>I',item)
                file.write(data)
            pad_size = 1024 - file.tell()
            if pad_size > 0:
                pad = bytes([0xFF] * pad_size)
                file.write(pad)

    def __init__(self, csv_file, gpio_dev_csv_file):
        self.csv = Csv(csv_file, True, ppc_keys)
        self.gpio_dev_csv = Csv(gpio_dev_csv_file, True, gpio_keys)
        self.gen_ppc_bin()
