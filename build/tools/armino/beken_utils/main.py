#!/usr/bin/env python3

import logging
import sys
import shutil
from argparse import _MutuallyExclusiveGroup
from genericpath import exists
import click
from click_option_group import optgroup,RequiredMutuallyExclusiveOptionGroup
import json

import struct
from scripts import partition
from scripts.partition import *
from scripts.gen_partition import *
from scripts.gen_ppc import *
from scripts.gen_mpc import *
from scripts.gen_security import *
from scripts.gen_ota import *
from scripts.gen_code import gen_code
from scripts.bl1_sign import *
from scripts.bl2_sign import *
from scripts.pk_hash import *
from scripts.pack import *
from scripts.compress import *
from scripts import crc
from scripts import encrypt
from scripts import xts_aes
from scripts import generator
from scripts.sign import *
from scripts.steps import *
from scripts.solution_generator import *
from scripts.copy_json_data_csv import *


def set_debug(debug):
    if debug:
        logging.basicConfig()
        logging.getLogger().setLevel(logging.DEBUG)
        stream_handler = logging.StreamHandler(sys.stdout)
        stream_handler.setLevel(logging.DEBUG)

@click.group()
@click.version_option(version='1.0.0.0')
def cli():
    """Beken security tools for generating code, signing, encrypting and packing"""
    pass

@cli.group("gen")
def gen():
    """Generate code from security config files"""

@cli.group("sign")
def sign():
    """BL1/BL2 signing commands"""

@cli.group("pack")
def pack():
    """Merge partitions, CRC/AES and packing"""

@cli.group("steps")
def steps():
    """Create downloadable bin step by step"""

@cli.group("pipeline")
def pipeline():
    """security solutions pipeline"""

@gen.command("partition")
@click.option("--partition_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='partitions.csv', help="partition CSV file.")
@click.option("--ota_type", type=click.Choice(['OVERWRITE', 'XIP']), default='OVERWRITE', required=True, help="The OTA type.")
@click.option("--out_hdr_file", type=str, required=False, default='partition_gen.h', help="Output file")
@click.option("--out_layout_file", type=str, required=False, default='partition_layout.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_partition_command(partition_csv, ota_type, out_hdr_file, out_layout_file, debug):
    """gen partition header and layout file."""
    set_debug(debug)
    p = Partitions(partition_csv, ota_type)
    gen_partitions_hdr_file(p, out_hdr_file)
    gen_partitions_layout_file(p, out_layout_file)

@gen.command("ppc")
@click.option("--ppc_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='ppc.csv', help="PPC CSV file.")
@click.option("--gpio_dev_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='gpio_dev.csv', help="GPIO map control config csv file.")
@click.option("--outfile", type=str, required=False, default='_ppc.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_ppc_command(ppc_csv, gpio_dev_csv, outfile, debug):
    """gen ppc.h from ppc.csv and gpio_dev.csv."""
    set_debug(debug)
    gen_ppc_config_file(ppc_csv, gpio_dev_csv, outfile)

@gen.command("mpc")
@click.option("--mpc_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='mpc.csv', help="MPC CSV file.")
@click.option("--outfile", type=str, required=False, default='_mpc.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_mpc_command(mpc_csv, outfile, debug):
    """gen mpc.h from mpc.csv."""
    set_debug(debug)
    gen_mpc_config_file(mpc_csv, outfile)

@gen.command("security")
@click.option("--security_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='security.csv', help="Security CSV file.")
@click.option("--outfile", type=str, required=False, default='security.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_security_command(security_csv, outfile, debug):
    """gen security.h from security.csv."""
    set_debug(debug)
    gen_security_config_file(security_csv, outfile)

@gen.command("ota")
@click.option("--ota_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='ota.csv', help="OTA CSV file.")
@click.option("--outfile", type=str, required=False, default='_ota.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_ota_command(ota_csv, outfile, debug):
    """gen ota.h from ota.csv."""
    set_debug(debug)
    gen_ota_config_file(ota_csv, outfile)

@gen.command("otp")
@click.option("--otp_csv", type=click.Path(exists=True, dir_okay=False), required=False, default='otp2.csv', help="OTP CSV file.")
@click.option("--outfile", type=str, required=False, default='_otp.h', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_ota_command(otp_csv, outfile, debug):
    """gen otp.h from otp.csv."""
    set_debug(debug)
    gen_otp_map_file()

@gen.command("otp_efuse")
@click.option("--flash_aes_type", type=click.Choice(['FIXED', 'RANDOM', 'NONE']), default='FIXED', required=True, help="Flash AES type.")
@click.option("--flash_aes_key", type=str, required=False, default=None, help="flash AES key.")
@click.option("--pubkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_pubkey.pem', help="PEM secure boot public key file.")
@click.option("--secure_boot", is_flag=True, help="Enable secure boot")
@click.option("--outfile", type=str, required=False, default='otp_efuse_config.json', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def gen_otp_efuse_command(flash_aes_type, flash_aes_key, pubkey_pem_file, secure_boot, outfile, debug):
    """gen otp_efuse_config.json from security csv files."""
    set_debug(debug)
    gen_otp_efuse_config_file(flash_aes_type, flash_aes_key, pubkey_pem_file, secure_boot, False, outfile)

@gen.command("all")
@click.option("--debug", is_flag=True, help="Enable debug")
def pack_command(debug):
    """generate all code from security csv files"""
    set_debug(debug)
    gen_code()

@pack.command("compress")
@click.option("--infile", type=click.Path(exists=True, dir_okay=False), required=False, default='primary_all_code_signed.bin', help="Binary to be compressed.")
@click.option("--outfile", type=str, required=False, default='otp_efuse_config.json', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def compress_command(infile, outfile, debug):
    """compress OTA binary (overwrite only)"""
    set_debug(debug)
    compress_bin(infile, outfile)

@pack.command("extract_pk_hash")
@click.option("--bin", type=click.Path(exists=True, dir_okay=False), required=True, default='bootloader.bin', help="Binaries that contains public key hash.")
@click.option("--outfile", type=str, required=False, default='pk_hash.json', help="Output public key hash json")
@click.option("--debug", is_flag=True, help="Enable debug")
def pk_hash_command(bin, outfile, debug):
    """extract public key hash from binary"""
    set_debug(debug)
    extract_pk_hash(bin, outfile)

@pack.command("insert_pk_hash")
@click.option("--bin", type=click.Path(exists=True, dir_okay=False), required=True, default='bootloader.bin', help="Binaries that contains public key hash.")
@click.option("--pubkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_pubkey.pem', help="PEM public key file.")
@click.option("--debug", is_flag=True, help="Enable debug")
def pk_hash_command(bin, pubkey_pem_file, debug):
    """insert public key hash from binary"""
    set_debug(debug)
    insert_pk_hash(bin, pubkey_pem_file)

@pack.command("get_pk_hash")
@click.option("--pubkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_pubkey.pem', help="PEM public key file.")
@click.option("--debug", is_flag=True, help="Enable debug")
def pk_hash_command(pubkey_pem_file, debug):
    """insert public key hash from binary"""
    set_debug(debug)
    get_pk_hash(pubkey_pem_file)

@steps.command("get_app_bin_hash")
@click.option("--debug", is_flag=True, help="Enable debug")
def get_bin_hash_command(debug):
    """Get binaries hash for signing"""
    set_debug(debug)
    get_app_bin_hash()

@steps.command("sign_app_bin_hash")
@click.option("--debug", is_flag=True, help="Enable debug")
def sign_hash_command(debug):
    """Signing app binaries hash"""
    set_debug(debug)
    sign_app_bin_hash()

@steps.command("sign_from_app_sig")
@click.option("--debug", is_flag=True, help="Enable debug")
def sign_from_sig_command(debug):
    """Create signed app bin based on signature"""
    set_debug(debug)
    sign_from_app_sig()

@steps.command("get_ota_bin_hash")
@click.option("--debug", is_flag=True, help="Enable debug")
def get_ota_bin_hash_command(debug):
    """Get OTA binary hash"""
    set_debug(debug)
    get_ota_bin_hash()

@steps.command("sign_ota_bin_hash")
@click.option("--debug", is_flag=True, help="Enable debug")
def sign_ota_bin_hash_command(debug):
    """Create signature of OTA bin hash"""
    set_debug(debug)
    sign_ota_bin_hash()

@steps.command("sign_from_ota_sig")
@click.option("--debug", is_flag=True, help="Enable debug")
def sign_from_ota_sig_command(debug):
    """Create signed OTA bin from signature"""
    set_debug(debug)
    sign_from_ota_sig()

@steps.command("pack")
@click.option("--debug", is_flag=True, help="Enable debug")
def pack_command(debug):
    """Pack download bin"""
    set_debug(debug)
    steps_pack()

@pack.command("all")
@click.option("--debug", is_flag=True, help="Enable debug")
@click.option("--config_dir", type=click.Path(exists=True, dir_okay=True), required=False, default=None, help="configuration files dir")
@click.option("--aes_key", type=str, required=False, default=None, help="configuration flash aes key")
def pack_command(debug, config_dir, aes_key):
    """Pack downloadable bin in a single command"""
    set_debug(debug)
    pack_all(config_dir, aes_key)

@sign.command("bl1_sign_hash")
@click.option("--privkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_privkey.pem', help="PEM private key file.")
@click.option("--hash", type=str, default=None, required=False, help="HASH value.")
@click.option("--outfile", type=str, required=False, default='manifest_signature.json', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def bl1_sign_hash_command(privkey_pem_file, hash, outfile, debug):
    """sign the hash via beken bootrom signing tool."""
    set_debug(debug)
    bl1_sign_hash(privkey_pem_file, hash, outfile)

@sign.command("bl1_sign")
@click.option("--action_type", type=click.Choice(['hash', 'sign', 'sign_from_sig']), default='sign', required=True, help="Sign action type.")
@click.option("--key_type", type=click.Choice(['ec256']), default='ec256', required=False, help="Sign algorithm, currently only support ec256.")
@click.option("--privkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_privkey.pem', help="PEM private key file.")
@click.option("--pubkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_pubkey.pem', help="PEM public key file.")
@click.option("--signature", type=str, required=False, default=None, help="Signature of manifest hash.")
@click.option("--bin_file", type=click.Path(exists=True, dir_okay=False), required=False, default='bl2.bin', help="Binary file to be signed.")
@click.option("--static_addr", type=str, required=True, default='0x0', help="BL2 static address.")
@click.option("--load_addr", type=str, required=True, default='0x28040000', help="BL2 load address.")
@click.option("--outfile", type=str, required=False, default='primary_manifest.bin', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def bl1_sign_command(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file, static_addr, load_addr, outfile, debug):
    """sign or calculate binary hash for bl1."""
    set_debug(debug)
    bl1_sign(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file, static_addr, load_addr, outfile)

@sign.command("bl2_sign_hash")
@click.option("-k", "--privkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_privkey.pem', help="PEM private key file.")
@click.option("-d", "--hash", type=str, default=None, required=False, help="HASH value.")
@click.option("-o", "--outfile", type=str, required=False, default='manifest_signature.json', help="Output file")
@click.option("--debug", is_flag=True, help="Enable debug")
def bl2_sign_hash_command(privkey_pem_file, hash, outfile, debug):
    """sign the hash via mcuboot imgtools."""
    set_debug(debug)
    bl2_sign_hash(privkey_pem_file, hash, outfile)

@sign.command("bl2_sign")
@click.option("--action_type", type=click.Choice(['hash', 'sign', 'sign_from_sig']), default='sign', required=True, help="Sign action type.")
@click.option("--key_type", type=click.Choice(['ec256']), default='ec256', required=False, help="Sign algorithm, currently only support ec256.")
@click.option("--privkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_privkey.pem', help="PEM private key file.")
@click.option("--pubkey_pem_file", type=click.Path(exists=True, dir_okay=False), required=False, default='root_ec256_pubkey.pem', help="PEM public key file.")
@click.option("--signature", type=str, required=False, default=None, help="Signature of manifest hash.")
@click.option("--bin_file", type=click.Path(exists=True, dir_okay=False), required=False, default='bl2.bin', help="Binary file to be signed.")
@click.option("--partition_size", type=int, required=True, default=0, help="Partition size.")
@click.option("--version", type=str, required=True, default='0.0.1', help="Version.")
@click.option("--security_counter", type=int, required=True, default=0, help="Version.")
@click.option("--sign_outfile", type=str, required=False, default='signed.bin', help="Output signed bin")
@click.option("--hash_outfile", type=str, required=False, default='bl2_hash.json', help="Output bl2 hash json file")
@click.option("--debug", is_flag=True, help="Enable debug")
def bl2_sign_command(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file,
partition_size, version, security_counter, sign_outfile, hash_outfile, debug):
    """sign or calculate binary hash for bl2."""
    set_debug(debug)
    bl2_sign(action_type, key_type, privkey_pem_file, pubkey_pem_file, signature, bin_file,
        partition_size, version, security_counter, sign_outfile, hash_outfile)

@pipeline.command("mock")
@click.option("--debug", is_flag=True, help="Enable debug")
@click.option("--outfolder", type=str, required=False, default='./key_output', help="Output folder path")
def mock_command(outfolder, debug):
    """generator mock data"""
    set_debug(debug)
    Mock(outfolder).generate_mock_data()

@pipeline.command("server_keys")
@click.option("--outfolder", type=str, required=False, default='./key_output', help="Output folder path")
@click.option("--debug", is_flag=True, help="Enable debug")
def server_keys_command(outfolder, debug):
    """generator a series keys and bin file"""
    set_debug(debug)
    generate_solution_files(outfolder)

@pipeline.command("process_keys_data")
@click.option("--srcfolder", type=str, required=False, default='./key_output', help="Output folder path")
@click.option("--dstfolder", type=str, required=False, default='./pack', help="Output folder path")
@click.option("--debug", is_flag=True, help="Enable debug")
def server_keys_command(srcfolder, dstfolder, debug):
    """generator a series keys and bin file"""
    set_debug(debug)
    keys_data_post_processing(srcfolder, dstfolder)

if __name__ == '__main__':
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)
    stream_handler = logging.StreamHandler(sys.stdout)
    stream_handler.setLevel(logging.DEBUG)
    cli()
