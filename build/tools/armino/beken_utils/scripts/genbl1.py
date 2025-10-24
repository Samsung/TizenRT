#!/usr/bin/env python3
import os
import logging
import json
import subprocess

s_bc_code = {\
    "//":                   "This is a generate file, do not modify it",\
    "description": "flash control configurations",\
    "tlvs": [\
        {"name": "msp",      "size": "4b",     "value": "0", "hex":"y"},\
        {"name": "pc",       "size": "4b",     "value": "0", "hex":"y"}\
    ]\
}

s_key = {\
    "//":                   "This is a generate file, do not modify it",\
\
    "key_desc_cfg": {\
        "fmt_ver":          "0x00010001"\
    },\
\
    "mnft_sig_cfg": {\
        "pubkey_hash_sch":  "SHA256",\
        "mnft_sig_sch":     "ECDSA_256_SHA256",\
        "mnft_prvkey":      "",\
        "mnft_pubkey":      "bl1_ec256_pubkey.pem"\
    },\
\
    "img_dgst_cfg" : {\
        "img_hash_sch":     "SHA256"\
    },\
\
    "//":                   "Optional, only required if image(s) encrypted.",\
\
\
    "img_enc_cfg": {\
        "enc_sch":          "AES-ECB",\
        "enc_key":          "B75AFF7160BE351026753874D38846BEDE916E167043382B313FF2FA54250B67",\
        "bits_enc_key":     256,\
        "//":               "optional if enc_sch doesn't need it",\
        "iv":               "11111111111111111111111111111111",\
        "//":               "In our BootROM hal implementation, the key blob is not used in converting to",\
        "//":               "the real encryption key. The HAL implementation compares the key blob",\
        "//":               "and selects the actual image encryption key. Details see hal_crypto_aes.c",\
        "key_blob":         "325AFF7160BE351026753874D38846BEDE916E167043382B313FF2FA54250B67",\
        "//":               "0000000000000000000000000000000000000000000000000000000000000001",\
        "bits_key_blob":    256\
    }\
}

s_man = {\
    "//":                   "This is a generate file, do not modify it",\
\
    "mnft_desc_cfg": {\
        "fmt_ver":          "0x00010001",\
        "mnft_ver":         "0x00000000",\
        "sec_boot":         True,\
    },\
    "imgs": [\
        {\
            "is_enc":       False,\
            "static_addr":  "0x02024000",\
            "load_addr":    "0x28040000",\
            "entry":        "0x28040000",\
            "path":         "bl2.bin"\
        }\
    ]\
}\

class Genbl1:
    def run_cmd(self, cmd):
            p = subprocess.Popen(cmd, shell=True)
            ret = p.wait()
            if (ret):
                    logging.error(f'failed to run "{cmd}"')
                    exit(1)

    def sig_sch_to_scheme(self):
        if (self.sig_sch == None):
            return ''

        if (self.sig_sch == 'ec256'):
            return 'ECDSA_256_SHA256'
        elif (self.sig_sch == 'ec521'):
            return 'ECDSA_521_SHA256'
        elif (self.sig_sch == 'rsa1024'):
            return 'RSASSA_1024_PKCS_V21_SHA256'
        elif (self.sig_sch == 'rsa2048'):
            return 'RSASSA_2048_PKCS_V21_SHA256'
        else:
            logging.error(f'unknow signature scheme {self.sig_sch}, should be ec256/ec521/rsa1024/rsa2048')
            exit(1)
 
    def gen_key(self):
        pwd = os.getcwd()
        logging.debug(f'checking private/public key {self.sig_sch} {self.prvkey} {self.pubkey}')

        if (self.prvkey != '') and (os.path.exists(self.prvkey) == True):
            logging.debug(f'private key exists')

        if (self.pubkey != '') and (os.path.exists(self.pubkey) == True):
            logging.debug(f'public key exists')
            return

        self.run_cmd(f'./gen_key.sh {self.sig_sch} {self.prvkey} {self.pubkey}')

    def __init__(self, sec_boot=True, sig_sch='ec256', prvkey='', pubkey='bl1_ec256_pubkey.pem',outfile =''):
        self.is_sec_boot = sec_boot
        self.sig_sch = sig_sch
        self.prvkey = prvkey
        self.pubkey = pubkey

        self.outfile = outfile

        if (prvkey == ''):
            logging.debug(f'missing private key')

        logging.debug(f'sec_boot={self.is_sec_boot}, sig_sch={self.sig_sch}, prvkey={self.prvkey}, pubkey={self.pubkey}')

        self.scheme = self.sig_sch_to_scheme()
        logging.debug(f'scheme={self.scheme}')

        self.gen_key()

    def gen_key_desc(self):
        self.out_key_desc_file = 'key_desc.json'
        logging.debug(f'start to gen key description file')
        with open(self.out_key_desc_file, 'w+') as f:
            s_key['mnft_sig_cfg']['mnft_sig_sch'] = self.scheme
            s_key['mnft_sig_cfg']['mnft_prvkey'] = self.prvkey
            s_key['mnft_sig_cfg']['mnft_pubkey'] = self.pubkey
            json.dump(s_key, f, indent=4, separators=(',', ':'))

    def gen_manifest(self, security_counter, static_addr, load_addr="0x28040000", bin_name="bl2.bin", out_manifest_file="primary_manifest.json"):
        logging.debug(f'start to gen {out_manifest_file}')
        logging.debug(f'security_counter={security_counter}')
        logging.debug(f'static_addr={static_addr}')
        logging.debug(f'load_addr={load_addr}')
        logging.debug(f'bin_name={bin_name}')
        hex_security_counter = '0x%08x' %(int(security_counter))
        with open(out_manifest_file, 'w+') as f:
            s_man['mnft_desc_cfg']['mnft_ver'] = hex_security_counter 
            s_man['mnft_desc_cfg']['sec_boot'] = self.is_sec_boot
            s_man['imgs'][0]['static_addr'] = static_addr
            s_man['imgs'][0]['load_addr'] = load_addr
            s_man['imgs'][0]['entry'] = load_addr
            s_man['imgs'][0]['path'] = bin_name
            json.dump(s_man, f, indent=4, separators=(',', ':'))
