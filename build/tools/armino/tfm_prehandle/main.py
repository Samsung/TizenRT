#!/usr/bin/env python3

import logging
import sys
import click

from scripts.gen_code import gen_code

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


@gen.command("all")
@click.option("--debug", is_flag=True, help="Enable debug")
def pack_command(debug):
    """generate all code from security csv files"""
    set_debug(debug)
    gen_code()

if __name__ == '__main__':
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)
    stream_handler = logging.StreamHandler(sys.stdout)
    stream_handler.setLevel(logging.DEBUG)
    cli()
