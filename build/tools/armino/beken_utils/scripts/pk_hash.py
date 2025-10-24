#!/usr/bin/env python3

import logging
import json
from .gen_security import *
from .rotpk_hash import *
from .common import *

def extract_pk_hash(bin_file, outfile):
    with open(bin_file, 'rb') as f:
        f.seek(4096 - 64)
        bl1_pubkey_hash = f.read(32).hex()
        bl2_pubkey_hash = f.read(32).hex()

    hash_dict = {key:value for key,value in [("bl1_pubkey_hash:",bl1_pubkey_hash),("bl2_pubkey_hash:",bl2_pubkey_hash)]}
    with open(outfile, 'w') as jf:
        json.dump(hash_dict, jf, indent=4)
    logging.debug(f'bl1_pk_hash:{bl1_pubkey_hash}')
    logging.debug(f'bl2_pk_hash:{bl2_pubkey_hash}')

def insert_pk_hash(bin_file, pubkey_pem_file):
    if not os.path.exists(bin_file):
        logging.debug(f'{bin_file} not exist, skip it')
        return

    s = Security('security.csv')
    h = Rotpk_hash(pubkey_pem_file)
    hash_dict = h.gen_rotpk_hash()
    bl1_rotpk_hash = bytes.fromhex(h.reverse_order(hash_dict['bl1_rotpk_hash']))
    bl2_rotpk_hash = bytes.fromhex(h.reverse_order(hash_dict['bl2_rotpk_hash']))
    with open(bin_file,"rb+") as f:
        f.seek(4096)
        data_after_delete = f.read()
        f.seek(4096-64)
        f.write(bl1_rotpk_hash)
        f.write(bl2_rotpk_hash)
        f.write(data_after_delete)
        f.truncate()

def get_pk_hash(pubkey_pem_file):
    h = Rotpk_hash(pubkey_pem_file)
    hash_dict = h.gen_rotpk_hash()
    bl1_rotpk_hash = hash_dict['bl1_rotpk_hash']
    bl2_rotpk_hash = hash_dict['bl2_rotpk_hash']
    logging.debug(f'bl1_pk_hash: {bl1_rotpk_hash}')
    logging.debug(f'bl2_pk_hash: {bl2_rotpk_hash}')
