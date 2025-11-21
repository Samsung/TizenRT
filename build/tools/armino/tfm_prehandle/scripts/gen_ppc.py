#!/usr/bin/env python3

import logging

from .ppc import *
from .gen_license import get_license
from .common import *

ppc_keys = [
    'Device',
    'Secure',
    'Privilege'
]

gpio_keys = [
    'GPIO',
    'Device'
]

def gen_ppc_const(ppc, f):
    for dict in ppc.csv.dic_list:
        if dict["Secure"] == "TRUE":
            line = f'#define GEN_SECURITY_DEV_%s_IS_SECURE    %s' %(dict["Device"], "1\r\n")
        else:
            line = f'#define GEN_SECURITY_DEV_%s_IS_SECURE    %s' %(dict["Device"], "0\r\n")
        f.write(line)
        if dict["Privilege"] == "TRUE":
            line = f'#define GEN_SECURITY_DEV_%s_IS_PRIVILEGE    %s' %(dict["Device"], "1\r\n")
        else:
            line = f'#define GEN_SECURITY_DEV_%s_IS_PRIVILEGE    %s' %(dict["Device"], "0\r\n")
        f.write(line)

    empty_line(f)
    line = f'#define DEV_IS_SECURE(dev)    GEN_SECURITY_DEV_##dev##_IS_SECURE\r\n'
    f.write(line)
    line = f'#define DEV_ID_IS_SECURE(dev,id)    GEN_SECURITY_DEV_##dev##id##_IS_SECURE\r\n'
    f.write(line)
    line = f'#define DEV_IS_PRIVILEGE(dev)    GEN_SECURITY_DEV_##dev##_IS_PRIVILEGE\r\n'
    f.write(line)
    line = f'#define DEV_ID_IS_PRIVILEGE(dev, id)    GEN_SECURITY_DEV_##dev##id##_IS_PRIVILEGE\r\n'
    f.write(line)

def gen_ppc_init_file(ppc):
    ppc_init_file_name = f'tfm_hal_ppc.h'
    f = open(ppc_init_file_name, 'w')

    logging.debug(f'Create {ppc_init_file_name}')
    f.write(get_license())

    line = f'static inline void tfm_hal_ppc_init(void)\r\n'
    f.write(line)
    line = f'{{\r\n'
    f.write(line)

    for dict in ppc.csv.dic_list:
        if dict["Secure"] == "TRUE":
            line = f'    bk_prro_set_secure(PRRO_DEV_%s, PRRO_SECURE);\r\n' %(dict["Device"])
            f.write(line)

        if dict["Privilege"] == "FALSE":
            line = f'    bk_prro_set_privilege(PRRO_DEV_%s, PRRO_NON_PRIVILEGED);\r\n' %(dict["Device"])
            f.write(line)

    line = f'}}'
    f.write(line)

def gen_ppc_config_file(ppc_csv='ppc.csv', gpio_dev_csv='gpio_dev.csv', outfile='_ppc.h'):
    if not os.path.exists(ppc_csv):
        logging.debug(f'ppc.csv not exists, stip it')
        return

    ppc = PPC(ppc_csv, gpio_dev_csv)
    f = open(outfile, 'w+')

    logging.debug(f'Create {outfile}')
    f.write(get_license())

    ppc.check_gpio_security()
    ppc.gen_ppc_bin()
    gen_ppc_const(ppc, f)
    gen_ppc_init_file(ppc)
