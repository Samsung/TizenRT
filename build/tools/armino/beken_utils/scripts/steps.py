#!/usr/bin/env python3

import logging
from .gen_ppc import *
from .gen_mpc import *
from .gen_security import *
from .gen_ota import *
from .gen_otp import *
from .bl1_sign import *
from .bl2_sign import *
from .partition import *
from .compress import *
from .gen_ppc import *
from .pk_hash import *

def check_exist(file_name):
    if not os.path.exists(file_name):
        logging.error(f'{file_name} NOT exists')
        exit(1)

def check_config():
    check_exist('bl2.bin')
    check_exist('tfm_s.bin')
    check_exist('cpu0_app.bin')
    check_exist('security.csv')
    check_exist('ota.csv')
    check_exist('partitions.csv')

def get_hash(file_name):
    with open(file_name, 'r') as f:
        d = json.load(f)
        return d['hash']

def get_app_sig(file_name):
    with open(file_name, 'r') as f:
        d = json.load(f)
        return d['signature']

#Step1 - get hash of app binary and hash of manifest
def get_app_bin_hash():
    check_config()

    #gen_ppc_config_file('ppc.csv', 'gpio_dev.csv', '_ppc.h')
    o = OTA('ota.csv')
    s = Security('security.csv')
    ota_type = o.get_strategy()
    p = Partitions('partitions.csv', ota_type, s.secureboot_en, s.crc_en)
    p.gen_bins_for_bl2_signing()

    pbl2 = p.find_partition_by_name('bl2')
    bl1_sign('hash', s.bl1_root_key_type, s.bl1_root_privkey, s.bl1_root_pubkey, None, pbl2.bin_name, pbl2.load_addr, pbl2.static_addr, 'primary_manifest.bin')

    pall = p.find_partition_by_name('primary_all')
    bl2_sign('hash', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, None, 'primary_all_code.bin', pall.vir_sign_size, '0.0.1', o.get_app_security_counter(), 'primary_all_code_signed.bin', 'app_hash.json')

#Step2 - generate signature from app/manifest hash, do it in server has private key
def sign_app_bin_hash():
    s = Security('security.csv')
    check_config()
    manifest_hash = get_hash('manifest_hash.json')
    app_hash = get_hash('app_hash.json')
    bl1_sign_hash(s.bl1_root_privkey, manifest_hash, 'manifest_sig.json')
    bl2_sign_hash(s.bl2_root_privkey, app_hash, 'app_sig.json')

#Step3 - generate signed bin from signature
def sign_from_app_sig():
    check_config()

    o = OTA('ota.csv')
    s = Security('security.csv')
    ota_type = o.get_strategy()
    p = Partitions('partitions.csv', ota_type, s.secureboot_en, s.crc_en)

    pbl2 = p.find_partition_by_name('bl2')
    bl1_sign('sign_from_sig', s.bl1_root_key_type, s.bl1_root_privkey, s.bl1_root_pubkey, None, pbl2.bin_name, pbl2.load_addr, pbl2.static_addr, 'primary_manifest.bin')

    app_sig = get_app_sig('app_sig.json')
    pall = p.find_partition_by_name('primary_all')
    bl2_sign('sign_from_sig', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, app_sig, 'primary_all_code.bin', pall.vir_sign_size, '0.0.1', o.get_app_security_counter(), 'primary_all_code_signed.bin', 'app_hash.json')

#Step4 - get hash of ota binary
def get_ota_bin_hash():
    check_config()

    o = OTA('ota.csv')
    s = Security('security.csv')
    ota_type = o.get_strategy()
    p = Partitions('partitions.csv', ota_type, s.secureboot_en, s.crc_en)

    if (ota_type == 'OVERWRITE'):
        compress_bin('primary_all_code_signed.bin', 'compress.bin')
        pota = p.find_partition_by_name('ota')
        bl2_sign('hash', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, None, 'compress.bin', pota.partition_size, '0.0.1', o.get_app_security_counter(), 'ota_signed.bin', 'ota_hash.json')

#Step5 - generate signature from ota bin hash, do it in server has private key
def sign_ota_bin_hash():
    s = Security('security.csv')
    check_config()
    manifest_hash = get_hash('manifest_hash.json')
    ota_hash = get_hash('ota_hash.json')
    bl2_sign_hash(s.bl2_root_privkey, ota_hash, 'ota_sig.json')

#Step6 - generate ota.bin from ota signature
def sign_from_ota_sig():
    check_config()

    o = OTA('ota.csv')
    s = Security('security.csv')
    ota_type = o.get_strategy()
    p = Partitions('partitions.csv', ota_type, s.secureboot_en, s.crc_en)

    ota_sig = get_app_sig('ota_sig.json')
    if (ota_type == 'OVERWRITE'):
        pota = p.find_partition_by_name('ota')
        bl2_sign('sign_from_sig', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, ota_sig, 'compress.bin', pota.partition_size, '0.0.1', o.get_app_security_counter(), 'ota_signed.bin', 'ota_hash.json')

#Step7 - pack download bin
def steps_pack():
    check_config()
    o = OTA('ota.csv')
    s = Security('security.csv')
    ota_type = o.get_strategy()
    boot_ota = o.get_boot_ota()
    p = Partitions('partitions.csv', ota_type, boot_ota, s.secureboot_en, s.crc_en)
    p.pack_bin('pack.json', s.flash_aes_type, s.flash_aes_key, o.get_app_security_counter(),o.get_encrypt(), o.get_boot_ota())
    insert_pk_hash('bootloader.bin', s.bl2_root_pubkey)
    p.install_bin()
