import time
from .bk7236 import BK7236
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import safe_data
from ...chip_controller import BaseController
from ...common.common import LINKCHECK

class BK7231N(BK7236):

    CHIP_ID = 0x7231C
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()


    def download_flash_cbus(self, wbuf:bytes, partition_start:int, partition_size:int, start_addr:int, retry=5, reset_type=None, link_type=None):
        raise Exception("not support operation.")
    

    def write_efuse_by_bit(self,reg_addr,bit,value):
        raise Exception("not support operation.")


    def write_efuse(self,reg_addr,value):
        raise Exception("not support operation.")



    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        raise Exception("not support operation.")
   


    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False,bigendian=False):
        raise Exception("not support operation.")


    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        raise Exception("not support operation.")


    def read_chip_uid(self):
        raise Exception("not support operation.")


    def read_all_efuse(self):
        raise Exception("not support operation.")


    def write_safe_json(self, safeinfo: safe_data):
        raise Exception("not support operation.")


    def read_bl2_version(self):
        raise Exception("not support operation.")


    def check_aes_key_enable(self):
        raise Exception("not support operation.")


    def check_aes_key_otp_data(self):
        raise Exception("not support operation.")


    def check_aes_key_otp_permission(self):
        raise Exception("not support operation.")


    def check_bkaesrdm_status(self):
        raise Exception("not support operation.")