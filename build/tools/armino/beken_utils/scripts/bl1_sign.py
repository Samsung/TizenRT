#!/usr/bin/env python3

import os
import json
import logging
from .common import *
from .genbl1 import *

def bl1_sign_hash(privkey_pem_file, manifest_hash, outfile):
    script_dir = get_script_dir()
    sh_sec_tools = f'{script_dir}/../tools/sh_sec_tools/secure_boot_tool'

    digest_file = '_bl1_manifest_digest.txt'
    with open(digest_file, 'w') as f:
        f.write(manifest_hash)

    cmd = f'{sh_sec_tools} sign -k {privkey_pem_file} -d {digest_file}'
    run_cmd(cmd)

    signature_dict = {
        'bl1_sig_s': '',
        'bl1_sig_r': '',
    }

    with open('bl1_signature.txt', 'r') as f:
        signature_dict['bl1_sig_s'] = f.readline().strip();
        signature_dict['bl1_sig_r'] = f.readline();

    with open(outfile, 'w') as f:
        json.dump(signature_dict, f, indent=4)

def gen_manifest_bin(action_type, manifest_json_file, outfile):
    script_dir = get_script_dir()
    sh_sec_tools = f'{script_dir}/../tools/sh_sec_tools/secure_boot_tool'
    pwd = os.getcwd()
    logging.debug(f'generate manifest.bin')

    if (action_type == 'hash'):
        logging.debug(f'generate manifest hash only')
        cmd = f'{sh_sec_tools} manifest_digest -k key_desc.json -m {manifest_json_file} -o {pwd}/'
    elif (action_type == 'sign_from_sig'):
        logging.debug(f'generate manifest.bin from signature')
        cmd = f'{sh_sec_tools} gen_manifest_with_signature -k key_desc.json -m {manifest_json_file} -o {pwd}/'
    elif (action_type == 'sign'):
        logging.debug(f'generate manifest.bin from private key')
        cmd = f'{sh_sec_tools} -k key_desc.json -m {manifest_json_file} -o {pwd}/'
    else:
        logging.error(f'invalid bl1_sign action {action_type}')
        exit(1)

    run_cmd(cmd)
    cmd = f'mv manifest.bin {outfile}'
    run_cmd(cmd)

    if (action_type == 'hash'):
        data = {"hash":""}
        with open('bl1_manifest_digest.txt', 'r') as f:
            h = f.read()
            data['hash'] = h
            with open('manifest_hash.json', 'w') as hf:
                json.dump(data, hf, indent=4)
        os.remove('bl1_manifest_digest.txt')

def bl1_sign(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file, static_addr, load_addr, outfile):
    if outfile == "secondary_manifest.bin" :
        manifest_json_file = 'secondary_manifest.json'
    else :
        manifest_json_file = 'primary_manifest.json'
    g = Genbl1(True, key_type, privkey_pem_file, pubkey_pem_file, outfile)
    g.gen_key_desc()
    g.gen_manifest(5, static_addr, load_addr, bin_file, manifest_json_file)
    gen_manifest_bin(action_type, manifest_json_file, outfile)
