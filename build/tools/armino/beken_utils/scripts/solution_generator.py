#!/usr/bin/env python3
import sys
import os
import json
import struct
import random
import string
import hashlib
import subprocess
import logging
from Crypto.Cipher import AES
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.backends import default_backend

from .rotpk_hash import *


CFG_FOLDER_PATH = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'config')
FLASH_AES_KEY_CFG_PATH = os.path.join(CFG_FOLDER_PATH, 'flash_aes_key.json')
RSA_PRVI_KEY_PATH = os.path.join(CFG_FOLDER_PATH, 'rsa_privkey.pem')
RSA_PUB_KEY_PATH = os.path.join(CFG_FOLDER_PATH, 'rsa_pubkey.pem')

def random_generate_key_str(lenth):
    candidate_chars = string.hexdigits[:-6]
    return ''.join(random.choice(candidate_chars) for _ in range(lenth))

def run_cmd(cmd, cwd=None, check_ret=True):
    logging.debug(cmd)
    p = subprocess.Popen(
        cmd,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        encoding='utf-8',
        cwd=cwd)
    stdout, stderr = p.communicate()
    if check_ret and p.returncode:
        logging.error('FAILURE: %s' % cmd)
        logging.info(stdout)
        logging.error(stderr)
        exit(1)
    return stdout

class Mock:

    def __init__(self, output_folder):
        self.init_cfg_folder()
        self.output_folder = output_folder
        self.init_output_folder()

    def init_cfg_folder(self):
        os.makedirs(CFG_FOLDER_PATH, exist_ok=True)
    
    def init_output_folder(self):
        if not os.path.isdir(self.output_folder):
            os.makedirs(self.output_folder, exist_ok=True)
    
    def generate_flash_aes_key(self):
        flash_aes_key = None
        out_tmp_file = os.path.join(self.output_folder, 'aes_key.txt')
        run_cmd('{0} genkey 32 -outfile {1}'.format(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'tools', 'packager_tools', 'beken_aes'), out_tmp_file), check_ret=False)
        with open(out_tmp_file, 'r') as f:
            start_parse = False
            for tmp_line in f.readlines():
                if 'aes_key_plaintext' in tmp_line:
                    start_parse = True
                if start_parse and '"data":' in tmp_line:
                    flash_aes_key = tmp_line.split(':', 1)[1].strip().strip(',').strip('"').lower()
                    break
        if flash_aes_key is None:
            logging.error('generate flash aes key fail')
            exit(-1)
        else:
            if len(flash_aes_key) / 2 != 32:
                logging.error("Bad model flash_aes_key size, MUST be 256 bits!")
                exit(-1)
            for tmp_char in flash_aes_key:
                if tmp_char not in string.hexdigits[:-6]:
                    logging.error("Bad model flash_aes_key, MUST be hex char!")
                    exit(-1)
        tmp_key_info = {
            "content": flash_aes_key,
            "byte_lenth": int(len(flash_aes_key)/2)
        }
        with open(FLASH_AES_KEY_CFG_PATH, 'w') as f:
            json.dump(tmp_key_info, f, indent=4)
        return tmp_key_info
    
    def generate_aes_key(self):
        private_key = rsa.generate_private_key(
            public_exponent=65537,
            key_size=2048,
            backend=default_backend()
        )
        public_key = private_key.public_key()
        pem_private = private_key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.PKCS8,
            encryption_algorithm=serialization.NoEncryption()
        )
        with open(RSA_PRVI_KEY_PATH, 'wb') as f:
            f.write(pem_private)
        pem_public = public_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        )
        with open(RSA_PUB_KEY_PATH, 'wb') as f:
            f.write(pem_public)

    def generate_mock_data(self):
        self.generate_flash_aes_key()
        self.generate_aes_key()

class KeyGenerator:
    OUTPUT_FOLDER = None
    KEY_CONTENT_JSON_NAME = 'key_content.json'

    @classmethod
    def update_output_folder(cls, output_folder):
        cls.OUTPUT_FOLDER = output_folder

    @classmethod
    def random_generate_key_str(cls, lenth):
        candidate_chars = string.hexdigits[:-6]
        return ''.join(random.choice(candidate_chars) for _ in range(lenth))

    @classmethod
    def get_key_from_exist_config(cls, k_args: list):
        key_info_file_path = os.path.join(cls.OUTPUT_FOLDER, cls.KEY_CONTENT_JSON_NAME)
        key_info = {}
        if os.path.isfile(key_info_file_path):
            with open(key_info_file_path, 'r') as f:
                key_info = json.load(f)
        tmp_res = key_info
        for tmp_k in k_args:
            if tmp_k in tmp_res.keys():
                tmp_res = tmp_res[tmp_k]
            else:
                return None
        if tmp_res:
            return tmp_res
        else:
            return None

    @classmethod
    def generate_boot_key(cls):
        tmp_res = cls.get_key_from_exist_config(['boot_key'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(32 * 2),
                "byte_lenth": 32
            }
 
    @classmethod
    def generate_ek2(cls):
        tmp_res = cls.get_key_from_exist_config(['key_content', 'EK2'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(16 * 2),
                "byte_lenth": 16
            }
    
    @classmethod
    def generate_ek3(cls):
        tmp_res = cls.get_key_from_exist_config(['key_content', 'Ek3'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(16 * 2),
                "byte_lenth": 16
            }
       
    @classmethod
    def generate_model_key(cls):
        tmp_res = cls.get_key_from_exist_config(['key_content', 'Model key'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(16 * 2),
                "byte_lenth": 16
            }
    
    @classmethod
    def generate_aes_cbc_iv(cls):
        tmp_res = cls.get_key_from_exist_config(['aes_cfg', 'Aes cbc iv'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(16 * 2),
                "byte_lenth": 16
            }
    
    @classmethod
    def generate_aes_cbc_key(cls):
        tmp_res = cls.get_key_from_exist_config(['aes_cfg', 'Aes cbc key'])
        if tmp_res:
            return tmp_res
        else:
            return {
                "content": cls.random_generate_key_str(32 * 2),
                "byte_lenth": 32
            }
    
    @classmethod
    def aes_ecb_enc(cls, key, datain):
        ctx = AES.new(key, AES.MODE_ECB)
        dataout = ctx.encrypt(datain)
        return dataout
    
    @classmethod
    def aes_ecb_dec(cls, key, datain):
        ctx = AES.new(key, AES.MODE_ECB)
        dataout = ctx.decrypt(datain)
        return dataout
    
    @classmethod
    def generate_ek1(cls, boot_key, model_key, ek2, ek3):
        if (len(boot_key) / 2) not in [16, 32]:
            logging.error("Bad key blob size, only support 128 or 256 bits!")
            exit(-1)

        if len(model_key) / 2 != 16:
            logging.error("Bad model key size, MUST be 128 bits!")
            exit(-1)
        
        if len(ek2) / 2 != 16:
            logging.error("Bad model ek2 size, MUST be 128 bits!")
            exit(-1)
        
        if len(ek3) / 2 != 16:
            logging.error("Bad model ek3 size, MUST be 128 bits!")
            exit(-1)

        tmp_out = cls.aes_ecb_dec(bytes.fromhex(model_key), bytes.fromhex(ek3))
        tmp_out = cls.aes_ecb_dec(tmp_out, bytes.fromhex(ek2))
        tmp_out = cls.aes_ecb_enc(tmp_out, bytes.fromhex(boot_key))
        return {
            "content": tmp_out.hex(),
            "byte_lenth": int(len(tmp_out.hex())/2)
        }
    
    @classmethod
    def gen_boot_public_key_hash(cls):
        priv_key_path = os.path.join(cls.OUTPUT_FOLDER, 'root_ec256_privkey.pem')
        pub_key_path = os.path.join(cls.OUTPUT_FOLDER, 'root_ec256_pubkey.pem')
        bl1_rotpk_hash = None
        bl2_rotpk_hash = None
        if not os.path.isfile(priv_key_path) or not os.path.isfile(pub_key_path):
            run_cmd('openssl ecparam -name secp256r1 -genkey -out {0}'.format(priv_key_path))
            run_cmd('openssl ec -in {0} -inform PEM -out {1} -pubout'.format(priv_key_path, pub_key_path))
            h = Rotpk_hash(pub_key_path)
            hash_dict = h.gen_rotpk_hash()
            bl1_rotpk_hash = h.reverse_order(hash_dict['bl1_rotpk_hash'])
            bl2_rotpk_hash = h.reverse_order(hash_dict['bl2_rotpk_hash'])
        else:
            h = Rotpk_hash(pub_key_path)
            hash_dict = h.gen_rotpk_hash()
            bl1_rotpk_hash = h.reverse_order(hash_dict['bl1_rotpk_hash'])
            bl2_rotpk_hash = h.reverse_order(hash_dict['bl2_rotpk_hash'])
      
        return [
            {
                "content": bl1_rotpk_hash,
                "byte_lenth": int(len(bl1_rotpk_hash)/2)
            },
            {
                "content": bl2_rotpk_hash,
                "byte_lenth": int(len(bl2_rotpk_hash)/2)
            }
        ]

    @classmethod
    def generate_flash_aes_key(cls):
        # get from config file
        with open(FLASH_AES_KEY_CFG_PATH, 'r') as f:
            tmp_key = json.load(f)
            if 'content' not in tmp_key.keys() or 'byte_lenth' not in tmp_key.keys():
                logging.error('flash_aes_key info is error')
                exit(-1)
            else:
                return tmp_key

    @classmethod
    def generate_all_key_info(cls):
        key_info = {
            "key_content": {
                "Flash Aes key": None,
                "Bl1 boot Public key hash": None,
                "Bl2 boot Public key hash": None,
                "Ek1": None,
                "EK2": None,
                "Ek3": None,
                "Model key": None
            },
            "boot_key": None,
            "aes_cfg": {
                "Aes cbc iv": None,
                "Aes cbc key": None
            }
        }
        key_info["key_content"]["Flash Aes key"] = cls.generate_flash_aes_key()
        key_info["key_content"]["Model key"] = cls.generate_model_key()
        key_info["key_content"]["EK2"] = cls.generate_ek2()
        key_info["key_content"]["Ek3"] = cls.generate_ek3()
        key_info["boot_key"] = cls.generate_boot_key()
        key_info["key_content"]["Ek1"] = cls.generate_ek1(
            key_info["boot_key"]["content"], 
            key_info["key_content"]["Model key"]["content"], 
            key_info["key_content"]["EK2"]["content"], 
            key_info["key_content"]["Ek3"]["content"])
        key_info["aes_cfg"]["Aes cbc iv"] = cls.generate_aes_cbc_iv()
        key_info["aes_cfg"]["Aes cbc key"] = cls.generate_aes_cbc_key()
        (bl1_boot_key_hash, bl2_boot_key_hash) = cls.gen_boot_public_key_hash()
        key_info["key_content"]["Bl1 boot Public key hash"] = bl1_boot_key_hash
        key_info["key_content"]["Bl2 boot Public key hash"] = bl2_boot_key_hash
        with open(os.path.join(cls.OUTPUT_FOLDER, cls.KEY_CONTENT_JSON_NAME), 'w') as f:
            json.dump(key_info, f, indent=4)
        logging.debug(key_info)
        return key_info

class KeyContentEncrypt:
    def __init__(self, output_folder, cfg_info, debug=False):
        self.output_folder = output_folder
        self.cfg_info = cfg_info
        self.debug = debug

    def generate_key_content(self):
        key_content_cfg = self.cfg_info['key_content']
        key_content_bytes = None
        key_content_str = ''
        key_content_catgory = ['Flash Aes key', 'Bl1 boot Public key hash', 'Bl2 boot Public key hash', 'Ek1', 'EK2', 'Ek3', 'Model key']
        for tmp_catgory in key_content_catgory:
            if tmp_catgory not in key_content_cfg.keys():
                raise Exception('{0} is not exist, please check config file'.format(tmp_catgory))
            else:
                if len(key_content_cfg[tmp_catgory]['content']) != key_content_cfg[tmp_catgory]['byte_lenth'] * 2:
                    raise Exception('{0} content length is error, please check config file'.format(tmp_catgory))
                else:
                    key_content_str += key_content_cfg[tmp_catgory]['content']
        key_content_bytes = bytes.fromhex(key_content_str)
        with open(os.path.join(self.output_folder, 'key_content.bin'), 'wb') as f:
            f.write(key_content_bytes)
        
        return key_content_bytes

    def encrypt_key_content(self):
        key_content = self.generate_key_content()
        aes_key = bytes.fromhex(self.cfg_info['aes_cfg']['Aes cbc key']['content'])
        aes_iv = bytes.fromhex(self.cfg_info['aes_cfg']['Aes cbc iv']['content'])
        en_cipher = AES.new(aes_key, AES.MODE_CBC, aes_iv)
        encrypt_bytes = en_cipher.encrypt(key_content)
        with open(os.path.join(self.output_folder, 'aes_encrypt_key_content_gen.bin'), 'wb') as f:
            f.write(struct.pack('i', len(encrypt_bytes)))
            f.write(encrypt_bytes)
        if self.debug:
            logging.debug('encrypt:')
            logging.debug(encrypt_bytes)
            de_cipher = AES.new(aes_key, AES.MODE_CBC, aes_iv)
            de_res = de_cipher.decrypt(encrypt_bytes)
            logging.debug('decrypt:')
            logging.debug(de_res)

    def rsa_encrypt_aes_key(self):
        if not os.path.isfile(RSA_PRVI_KEY_PATH):
            logging.error('rsa private key is not exist.')
            exit(-1)
        if not os.path.isfile(RSA_PUB_KEY_PATH):
            logging.error('rsa public key is not exist.')
            exit(-1)
        pem_private = None
        pem_public = None
        with open(RSA_PRVI_KEY_PATH, 'rb') as f:
            pem_private = f.read()
        
        with open(RSA_PUB_KEY_PATH, 'rb') as f:
            pem_public = f.read()
        
        with open(os.path.join(self.output_folder, 'rsa_privkey_gen.bin'), 'wb') as f:
            f.write(struct.pack('i', len(pem_private)))
            f.write(pem_private)

        loaded_private_key = serialization.load_pem_private_key(
            pem_private,
            password=None,
            backend=default_backend()
        )
        loaded_public_key = serialization.load_pem_public_key(
            pem_public,
            backend=default_backend()
        )
        aes_iv = bytes.fromhex(self.cfg_info['aes_cfg']['Aes cbc iv']['content'])
        aes_key = bytes.fromhex(self.cfg_info['aes_cfg']['Aes cbc key']['content'])
        ciphertext = loaded_public_key.encrypt(
            aes_iv+aes_key,
            padding.PKCS1v15()
        )
        with open(os.path.join(self.output_folder, 'rsa_encrypt_aes_key.bin'), 'wb') as f:
            f.write(ciphertext)
        with open(os.path.join(self.output_folder, 'rsa_encrypt_aes_key.json'), 'w') as f:
            json.dump({'AES_KEY': ciphertext.hex()}, f)

        # 使用私钥解密数据
        if self.debug:
            decrypted_text = loaded_private_key.decrypt(
                ciphertext,
                padding.PKCS1v15()
            )
            logging.debug("AES iv + AES KEY: {0}".format(aes_iv+aes_key))
            logging.debug("RSA encrypted: {0}".format(ciphertext))
            logging.debug("RSA decrypted: {0}".format(decrypted_text))
            logging.debug([hex(byte).zfill(2) for byte in ciphertext])


def generate_solution_files(output_folder):
    if not os.path.isdir(output_folder):
        os.makedirs(output_folder, exist_ok=True)
    KeyGenerator.update_output_folder(output_folder)
    key_info = KeyGenerator.generate_all_key_info()
    kce = KeyContentEncrypt(output_folder, key_info, False)
    kce.encrypt_key_content()
    kce.rsa_encrypt_aes_key()


 