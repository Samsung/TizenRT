#!/usr/bin/env python3

import logging
from .gen_ppc import *
from .gen_mpc import *
from .gen_security import *
from .gen_ota import *
from .gen_otp import *
from .bl1_sign import bl1_sign
from .bl2_sign import bl2_sign
from scripts.partition import *
from .compress import *
from .gen_ppc import *

def pack_all(config_dir, aes_key):
    if config_dir != None:
        logging.debug(f'cd {config_dir}')
        os.chdir(config_dir)

    gen_ppc_config_file('ppc.csv', 'gpio_dev.csv', '_ppc.h')

    o = OTA('ota.csv')
    s = Security('security.csv')

    if aes_key != None:
        flash_aes_key = aes_key
    else:
        flash_aes_key = s.flash_aes_key 

    gen_otp_efuse_config_file(s.flash_aes_type, flash_aes_key, s.bl2_root_pubkey, s.secureboot_en, o.get_boot_ota(),'otp_efuse_config.json')

    ota_type = o.get_strategy()

    boot_ota = o.get_boot_ota()

    p = Partitions('partitions.csv', ota_type, boot_ota, s.secureboot_en, s.crc_en)

    p.gen_bins_for_bl2_signing()

    pbl2 = p.find_partition_by_name('bl2')
    if pbl2 != None:
        bl1_sign('sign', s.bl1_root_key_type, s.bl1_root_privkey, s.bl1_root_pubkey, None, pbl2.bin_name, pbl2.load_addr, pbl2.static_addr, 'primary_manifest.bin')
        if (boot_ota == True):
            pbl2_B = p.find_partition_by_name('bl2_B')
            bl1_sign('sign', s.bl1_root_key_type, s.bl1_root_privkey, s.bl1_root_pubkey, None, pbl2_B.bin_name, pbl2_B.load_addr, pbl2_B.static_addr, 'secondary_manifest.bin')

        pall = p.find_partition_by_name('primary_all')
        bl2_sign('sign', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, None, 'primary_all_code.bin', pall.vir_sign_size, '0.0.1', o.get_app_security_counter(), 'primary_all_code_signed.bin', 'app_hash.json')

        if (ota_type == 'OVERWRITE'):
            compress_bin('primary_all_code_signed.bin', 'compress.bin')
            pota = p.find_partition_by_name('ota')
            bl2_sign('sign', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, None, 'compress.bin', pota.partition_size, '0.0.1', o.get_app_security_counter(), 'ota_signed.bin', 'ota_hash.json')
        elif (ota_type == 'XIP'):
            pota = p.find_partition_by_name('primary_all')
            app_version = o.get_version()
            bl2_sign('sign', s.bl2_root_key_type, s.bl2_root_privkey, s.bl2_root_pubkey, None, 'primary_all_code.bin', pall.vir_sign_size, app_version, o.get_app_security_counter(), 'ota_signed.bin', 'ota_hash.json')

    p.pack_bin('pack.json', s.flash_aes_type, flash_aes_key, o.get_app_security_counter(),o.get_encrypt(), o.get_boot_ota())
    p.install_bin()
