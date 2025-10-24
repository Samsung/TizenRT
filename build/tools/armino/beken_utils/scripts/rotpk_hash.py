#!/usr/bin/env python3

import os
import json
import logging
import shutil
import re
import hashlib
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import ec
from .common import *

class Rotpk_hash:
    def __init__(self, pubkey_file=None):
        self.bl1_rotpk_hash = ""
        self.bl2_rotpk_hash = ""
        self.rotpk_hash_dict = {
            'bl1_rotpk_hash': '',
            'bl2_rotpk_hash': ''
        }

        script_dir = get_script_dir()
        self.tools_dir = f'{script_dir}/..'
        if pubkey_file == None:
            pubkey_file = f'root_ec256_pubkey.pem'
        if os.path.exists(pubkey_file) == False:
            logging.error(f'public key file {pubkey_file} not exists')
            exit(1)

        self.pubkey_file = pubkey_file
        logging.debug(f'public key file: {pubkey_file}')

    def gen_rotpk_hash(self):
        self.gen_bl1_rotpk_hash()
        self.gen_bl2_rotpk_hash()
        self.gen_rotpk_hash_json()
        return self.rotpk_hash_dict

    def gen_rotpk_hash_json(self):
        hash_json_file = f'rotpk_digest.json'
        with open(hash_json_file, 'w') as f:
            json.dump(self.rotpk_hash_dict, f, indent=4)
        logging.debug(f'generate {hash_json_file}')

    def gen_bl1_rotpk_hash(self):
        sh_sec_tools = f'{self.tools_dir}/tools/sh_sec_tools/secure_boot_tool'
        cmd = f'{sh_sec_tools} rotpk_hash -k {self.pubkey_file}'
        run_cmd(cmd)
        if (os.path.exists(f'bl1_rotpk_digest.txt') == True):
            with open('bl1_rotpk_digest.txt', 'r') as f:
                self.bl1_rotpk_hash = f.read()
                self.bl1_rotpk_hash = self.reverse_order(self.bl1_rotpk_hash)
                self.rotpk_hash_dict['bl1_rotpk_hash'] = self.bl1_rotpk_hash
                logging.debug(f'bl1 rotpk hash={self.bl1_rotpk_hash}')
        else:
            logging.debug(f'bl1 digest bin not exists')
            exit(1)

        return

    def reverse_order(self, hex_str):
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


    def sha256(self, data):
        sha256_hash = hashlib.sha256()
        sha256_hash.update(data)
        sha256_hash = sha256_hash.hexdigest()
        return self.reverse_order(sha256_hash)
 
    def gen_bl2_rotpk_hash(self):
        with open(self.pubkey_file, 'rt') as pem_file:
            pem_file_data = pem_file.read()

        try:
            public_key = serialization.load_pem_public_key(pem_file_data.encode())

            der_data = public_key.public_bytes(
                encoding=serialization.Encoding.DER,
                format=serialization.PublicFormat.SubjectPublicKeyInfo)

            self.bl2_rotpk_hash = self.sha256(der_data)
            self.rotpk_hash_dict['bl2_rotpk_hash'] = self.bl2_rotpk_hash
            logging.debug(f'bl2 rotpk hash={self.bl2_rotpk_hash}')
        except Exception as e:
            logging.error(f'failed to load PEM data {e}')
            exit(1)
