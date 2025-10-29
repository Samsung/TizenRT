#!/usr/bin/env python3

import logging
from .gen_ppc import *
from .gen_mpc import *
from .gen_security import *
from .gen_ota import *
from .gen_otp_map import *
from .gen_partition import *
from .partition import *

def gen_code():
    gen_ppc_config_file('ppc.csv', 'gpio_dev.csv', '_ppc.h')
    gen_mpc_config_file('mpc.csv', '_mpc.h')
    gen_security_config_file('security.csv', 'security.h')
    gen_ota_config_file('ota.csv', '_ota.h')
    gen_otp_map_file()

    s = Security('security.csv')

    o = OTA('ota.csv')

    p = Partitions('partitions.csv', o.get_strategy(), o.get_boot_ota(), s.secureboot_en, s.crc_en)
    gen_partitions_hdr_file(p, 'partitions_partition.h')
    gen_partitions_layout_file(p, 'partition_layout.txt')
    p.create_partition_partition(s.flash_aes_type, s.flash_aes_key)
