#!/usr/bin/env python3
import os
import logging
import csv

from .common import *
from .parse_csv import *

security_keys = [
    'secureboot_en',
    'flash_aes_type',
    'crc_en',
    'flash_aes_key',
    'root_key_type',
    'root_pubkey',
    'root_privkey',
]

class Security(dict):

    def __getitem__(self, key):
        return self.csv.dic[key];

    def __init__(self, csv_file):
        self.csv = Csv(csv_file, False, security_keys)
        self.parse_csv()

    def is_flash_aes_fixed(self):
        return (self.flash_aes_type.upper() == 'FIXED')

    def is_flash_aes_random(self):
        return (self.flash_aes_type.upper() == 'RANDOM')

    def parse_csv(self):
        self.secureboot_en = parse_bool(self.csv.dic['secureboot_en'])
        self.flash_aes_type = self.csv.dic['flash_aes_type'].upper()

        self.flash_aes_key = self.csv.dic['flash_aes_key']
        if self.flash_aes_type == 'FIXED':
            if len(self.flash_aes_key) != 64:
                logging.error(f'Invalid AES key: key length should be 64')
                exit(1)
        else:
            self.flash_aes_key = None

        self.crc_en = self.csv.dic['crc_en'].upper()
        self.bl1_root_key_type = self.csv.dic['root_key_type']
        self.bl1_root_pubkey = self.csv.dic['root_pubkey']
        self.bl1_root_privkey = self.csv.dic['root_privkey']

        self.bl2_root_key_type = self.csv.dic['root_key_type']
        self.bl2_root_pubkey = self.csv.dic['root_pubkey']
        self.bl2_root_privkey = self.csv.dic['root_privkey']

        if self.secureboot_en == True:
            if (self.bl1_root_key_type != '') and (self.bl1_root_key_type != 'ec256') and (self.bl1_root_key_type != 'rsa2048'):
                logging.error(f'Unknown root key type {self.root_key_type}, only support ec256, rsa2048')
                exit(1)
