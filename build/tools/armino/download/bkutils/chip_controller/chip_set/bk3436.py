import time
from .bk7231N import BK7231N
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import safe_data
from ...common.common import FLASH_SIZE_DEF, LINKCHECK,FLASH_ERASE_CMD_EXT, FLASH_ERASE_CMD, safe_data, BinInfo,is_buf_all_0xff, BIT, BFD, crc32_ver2, bytes_reverse,bytes_reverse_per4bytes
from ...chip_controller import BaseController,FlashController,RegController


class BK3436(BK7231N):

    CHIP_ID = 0x3436
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()

    #align the address on the start or end
    def align_sector_address_for_erase(self,addr:int,start_or_end:bool,reset_type=None,link_type=None,read_baudrate=500000):
        flash_control = FlashController(self.ser,self._flash_cfg)
        erase_addr = int(addr/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
        baudrate_backup = self.ser.baudrate
        if not self.set_baudrate(read_baudrate):
            BKLog.w(f"Set baudrate: {read_baudrate} fail.")
            return False
        time.sleep(0.1)
        ret = flash_control.read_sector(erase_addr)
        if ret is None:           
            return False
        
        res = flash_control.erase_sector(erase_addr)
        if not res:
            return False
        if not self.set_baudrate(baudrate_backup):
            BKLog.e(f"Set baudrate: {baudrate_backup} fail.")
            return False
        if start_or_end:
            ret = ret[:addr&0xfff]+b'\xff'*(FLASH_SIZE_DEF.SIZE_4K-addr&0xfff)
        else:          
            ret = b'\xff'*(addr&0xfff)+ret[addr&0xfff:]           
        
        if not self.write_and_check_sector(ret,erase_addr):
            BKLog.i("write sector @0x{:x} fail, retry".format(erase_addr))
            if not self.retry_write_sector(erase_addr,ret,reconnect_bus=False,reset_type=reset_type,link_type=link_type):
                BKLog.e("Write and check sector @0x{:x} fail.".format(erase_addr))
                return False
        return True
       
    #align the address on the start or end
    def align_sector_address_for_write(self, addr: int, start_or_end: bool, content: bytes, reset_type=None, link_type=None,read_baudrate=500000):
        BKLog.i(f'input content length is: 0x{len(content):x}')
        BKLog.i(f'address is: 0x{addr:x}')
        flash_control = FlashController(self.ser,self._flash_cfg)
        erase_addr = int(addr/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
        baudrate_backup = self.ser.baudrate
        if not self.set_baudrate(read_baudrate):
            BKLog.e(f"Set baudrate: {read_baudrate} fail.")
            return False
        time.sleep(0.1)
        
        ret = flash_control.read_sector(erase_addr)         
        if ret is None:           
            return False
        
        res = flash_control.erase_sector(erase_addr)
        if not res:
            return False
        if not self.set_baudrate(baudrate_backup):
            BKLog.e(f"Set baudrate: {baudrate_backup} fail.")
            return False
        if start_or_end:          
            ret = ret[:addr&0xfff]+content[:(FLASH_SIZE_DEF.SIZE_4K-addr&0xfff)]
        else:                                
            ret = content[-(addr&0xfff):] + ret[addr&0xfff:]  
            #print(binascii.b2a_hex(ret))
            #print(len(ret))
        
        if not self.write_and_check_sector(ret,erase_addr):
            BKLog.i("write sector @0x{:x} fail, retry".format(erase_addr))
            if not self.retry_write_sector(erase_addr,ret,reconnect_bus=False,reset_type=reset_type,link_type=link_type):
                BKLog.e("Write and check sector @0x{:x} fail.".format(erase_addr))
                return False
        return True

    def parse_flash_info(self):
        BKLog.i("ignore read flash mid.")
        self._flash_mid = 0x424b01        
        self._flash_cfg.parse_flash_info(self._flash_mid)
        return True


    def un_protect_flash(self, protect: bool):
        BKLog.i("ignore unprotect/protect flash.")
        return True


    def erase_flash(self, full_erase: bool, start_addr=None, end_addr=None, align_addr=False, retry=5):
        flash_control = FlashController(self.ser,self._flash_cfg)
        if full_erase:
            # erase all
            BKLog.i("erase all chip.")
            erase_res = False
            erase_all_protocol = self.BOOTROM_PROTOCOL.FlashEraseAllProtocol()
            if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                erase_all_protocol = self.BOOTROM_PROTOCOL.FlashEraseAllExtProtocol()
            for _ in range(retry):
                tmp_res, content = self.ser.write_cmd_and_wait_response(erase_all_protocol.cmd(20), erase_all_protocol.expect_length, timeout_sec=20)
                if tmp_res and erase_all_protocol.response_check(content):
                    erase_res = True
                    break
                else:
                    BKLog.i("chip erase fail, retry")                  
            if not erase_res:
                raise Exception('chip erase fail.')
        else:
            BKLog.i("erase start address @0x{:x}, ".format(start_addr)+"end address @0x{:x}.".format(end_addr))
            if start_addr is None or end_addr is None:
                BKLog.e("Error, address parameters is None.")
                return False
            if start_addr>end_addr:
                BKLog.e("Error, start address is over end.")
                return False
            if start_addr & 0xfff:
                if align_addr:
                    if not self.align_sector_address_for_erase(start_addr,1):
                        BKLog.e("Align start address @0x{:x} fail.".format(start_addr))
                        return False
                start_addr = int((start_addr+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K

            if end_addr & 0xfff:
                if align_addr:
                    if not self.align_sector_address_for_erase(end_addr,0):
                        BKLog.e("Align end address @0x{:x} fail.".format(end_addr))
                        return False
                end_addr = int(end_addr /FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K

            erase_size = end_addr-start_addr
            i=0

            while i<erase_size:
                current_progress = 100 * i / erase_size
                BKLog.progress_log("BK","Erasing Flash", int(current_progress))                
                cnt = retry
                ret = False
                while cnt>0 and not ret:
                    BKLog.i("Erase sector @0x{:x} .".format(start_addr+i))                   
                    ret = flash_control.erase_sector(start_addr+i)                    
                    cnt-=1                       
                if not ret:
                    BKLog.e("Erase sector @0x{:x} fail.".format(start_addr+i))
                    return False
                i+=FLASH_SIZE_DEF.SIZE_4K                      
                        
            BKLog.i("end erase.")    
        return True