#!/usr/bin/env python3

import os
import json
import logging
from .common import *
from .genbl1 import *

def bl2_sign_hash(privkey_pem_file, hash, outfile):
    script_dir = get_script_dir()
    bl2_signing_tool = f'{script_dir}/../tools/mcuboot_tools/imgtool.py'
    cmd = f'{bl2_signing_tool} sign-hash -k {privkey_pem_file} -d {hash} -o {outfile}'
    run_cmd(cmd)

def bl2_sign(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file, partition_size, version, security_counter, sign_outfile, hash_outfile):

    logging.debug(f'bl2 sign, action_type={action_type}, key_type={key_type}')
    logging.debug(f'privkey_pem_file={privkey_pem_file}, pubkey_pem_file={pubkey_pem_file}, signature={signature} security_counter={security_counter} version={version}')
    logging.debug(f'bin={bin_file}, sign_outfile={sign_outfile} hash_outfile={hash_outfile} partition_size={partition_size}')

    if action_type == 'hash':
        key_opt = f''
        hash_outfile_opt = f'--hash_outfile {hash_outfile}'
    else:
        key_opt = f'-k {privkey_pem_file}'
        hash_outfile_opt = ''

    if action_type == 'sign_from_sig':
        signature_opt = f'--signature {signature}'
    else:
        signature_opt = ''

    script_dir = get_script_dir()
    bl2_signing_tool_dir = f'{script_dir}/../tools/mcuboot_tools/imgtool.py'
    cmd = f'{bl2_signing_tool_dir} sign {key_opt} --public-key-format full --max-align 8 --align 1 --version {version} --security-counter {security_counter} --pad-header --header-size 0x1000 --slot-size {partition_size} --pad --boot-record SPE --endian little --encrypt-keylen 128 {bin_file} {sign_outfile} --action_type {action_type} {signature_opt} --pubkeyfile {pubkey_pem_file} {hash_outfile_opt}'
    run_cmd(cmd)
    return
