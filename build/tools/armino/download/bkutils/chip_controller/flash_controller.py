import time
from ..bk_log import BKLog
from .serial_helper import SerialHelper
from ..flash_adapter.flash_config import FlashConfig
from ..protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ..common.common import crc32_ver2


class FlashController(object):
    # 与Flash相关且与芯片型号无关控制操作
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL
    def __init__(self, serial_instance: SerialHelper, flash_cfg: FlashConfig) -> None:
        self.serial_instance = serial_instance
        self._flash_cfg = flash_cfg

    #read crc from bootrom ,then compare with computed crc
    def check_crc_ver2(self,buf:bytes,flash_addr:int,buf_len:int,timeout=1.8, recnt=5):
        crc_protocol = self.BOOTROM_PROTOCOL.CheckCrcProtocol()
        if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
            crc_protocol = self.BOOTROM_PROTOCOL.CheckCrcExtProtocol()
        crc_me = crc32_ver2(0xffffffff,buf)
        BKLog.d("buf crc is: 0x{:x}".format(crc_me))
        for _ in range(recnt):
            crc_res, crc_content = self.serial_instance.write_cmd_and_wait_response(
                    crc_protocol.cmd(flash_addr, flash_addr+buf_len-1), 
                    crc_protocol.expect_length,
                    timeout)
            if crc_res and crc_protocol.response_check(crc_content):
                break
        if not crc_res:
            return False
        crc_read = crc_protocol.get_crc_value(response_content=crc_content)
        BKLog.d("read crc is: 0x{:x}".format(crc_read))
        if crc_me != crc_read:
            return False
        
        return True
  
    def _write_flash_reg_enable(self,retry=5):
        wfe = self.BOOTROM_PROTOCOL.FlashWriteEnableProtocol()
        tmp_res = False
        for _ in range(retry):
            res, content = self.serial_instance.write_cmd_and_wait_response(wfe.cmd(), wfe.expect_length, 0.8)
            if res and wfe.response_check(content):
                tmp_res = True
                break
            else:
                BKLog.i('write flash register enable fail, retry')
                continue
        if tmp_res is False:
            raise Exception('write flash register enable fail')

    def _read_flash_status_reg_val(self,retry=5):
        frsp = self.BOOTROM_PROTOCOL.FlashReadSRProtocol()
        read_reg_code = self._flash_cfg.flash_status_reg_read_code
        sr_val = []
        for _ in range(len(read_reg_code)):
            tmp_reg = read_reg_code[_]
            tmp_val = None
            for _ in range(retry):
                res, content = self.serial_instance.write_cmd_and_wait_response(frsp.cmd(tmp_reg), frsp.expect_length, 0.8)
                if res and frsp.response_check(content, tmp_reg):
                    tmp_val = frsp.get_status_regist_val(content)
                    break
                else:
                    BKLog.i('read flash status register fail, retry')
                    continue
            if tmp_val is None:
                raise Exception('read flash status register fail')
            else:
                sr_val.append(tmp_val)
        return sr_val

    def _write_flash_status_reg_val(self,write_val,retry=5):
        fwsp = self.BOOTROM_PROTOCOL.FlashWriteSRProtocol()
        write_reg_code = self._flash_cfg.flash_status_reg_write_code
        if len(write_reg_code) == 1:
            tmp_res = False
            for _ in range(retry):
                res, content = self.serial_instance.write_cmd_and_wait_response(fwsp.cmd(write_reg_code[0], write_val), fwsp.expect_length(len(write_val)), 0.8)
                if res and fwsp.response_check(content, write_reg_code[0]): # fix me
                    tmp_res = True
                    break
                else:
                    BKLog.i('write flash status register fail, retry')
                    time.sleep(0.01)
                    continue
                
            if tmp_res is False:
                raise Exception('write flash status register fail')
        else:
            for idx in range(len(write_reg_code)):
                tmp_res = False
                for _ in range(retry):
                    res, content = self.serial_instance.write_cmd_and_wait_response(fwsp.cmd(write_reg_code[idx], [write_val[idx]]), fwsp.expect_length(1), 0.8)
                    if res and fwsp.response_check(content, write_reg_code[idx]):
                        tmp_res = True
                        break
                    else:
                        BKLog.i('write flash status register fail, retry')
                        time.sleep(0.01)
                        continue
                if tmp_res is False:
                    raise Exception('write flash status register fail')
                
                time.sleep(0.01)

              
    def unprotect_flash(self):
        unprotect_reg_val, mask = self._flash_cfg.unprotect_register_value
        reg_val = self._read_flash_status_reg_val()
        if self._flash_cfg.compare_register_value(reg_val, unprotect_reg_val, mask):
            return True
        else:
            #self._write_flash_reg_enable()
            #time.sleep(0.01)
            write_val = unprotect_reg_val
            for _ in range(len(write_val)):
                write_val[_] = write_val[_] | (reg_val[_] & (mask[_] ^ 0xff))
            self._write_flash_status_reg_val(write_val)
            reg_val = self._read_flash_status_reg_val()
            if self._flash_cfg.compare_register_value(reg_val, unprotect_reg_val, mask):
                return True
        return False


    def protect_flash(self):
        protect_reg_val, mask = self._flash_cfg.protect_register_value
        reg_val = self._read_flash_status_reg_val()
        if self._flash_cfg.compare_register_value(reg_val, protect_reg_val, mask):
            return True
        else:
            write_val = protect_reg_val
            for _ in range(len(write_val)):
                write_val[_] = write_val[_] | (reg_val[_] & (mask[_] ^ 0xff))
            self._write_flash_status_reg_val(write_val)
            reg_val = self._read_flash_status_reg_val()
            if self._flash_cfg.compare_register_value(reg_val, protect_reg_val, mask):
                return True
        return False
    

    def write_sector(self, flash_addr, buf):
        write_flash_protocol = self.BOOTROM_PROTOCOL.FlashWrite4kProtocol()       
        if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
            write_flash_protocol = self.BOOTROM_PROTOCOL.FlashWrite4kExtProtocol()
        
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                        write_flash_protocol.cmd(flash_addr, buf), 
                        write_flash_protocol.expect_length,
                        0.8)
        if tmp_res and write_flash_protocol.response_check(content,flash_addr=flash_addr):
            return True

        return False    


    def read_sector(self, flash_addr):
        read_flash_protocol = self.BOOTROM_PROTOCOL.FlashRead4kProtocol()       
        if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
            read_flash_protocol = self.BOOTROM_PROTOCOL.FlashRead4kExtProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(read_flash_protocol.cmd(flash_addr),
                                                                read_flash_protocol.expect_length,
                                                                1.0)
        if tmp_res and read_flash_protocol.response_check(content,flash_addr=flash_addr):
            return read_flash_protocol.get_read_content(content)
        
        return None
        

    def erase_sector(self, flash_addr):
        erase_flash_protocol = self.BOOTROM_PROTOCOL.FlashErase4kProtocol()
        if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
            erase_flash_protocol = self.BOOTROM_PROTOCOL.FlashErase4kExtProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(erase_flash_protocol.cmd(flash_addr),
                                                                erase_flash_protocol.expect_length,
                                                                1.0)
        if tmp_res and erase_flash_protocol.response_check(content,flash_addr=flash_addr):
            return True
        
        return False
    
    def erase_custom_size(self,flash_addr,cmd):
        '''
        cmd:
        normal,4k/32k/64k->0x20/0x52/0xd8
        ext,4k/32k/64k->0x21/0x5c/0xdc
        '''
        erase_flash_protocol = self.BOOTROM_PROTOCOL.FlashCustomEraseProtocol()
        
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(erase_flash_protocol.cmd(flash_addr,cmd),
                                                                erase_flash_protocol.expect_length,
                                                                1.0)
        if tmp_res and erase_flash_protocol.response_check(content,cmd,flash_addr=flash_addr):
            return True
        
        return False
    

    def read_flash_security(self, addr:int, oplen:int):
        fs_protocol = self.BOOTROM_PROTOCOL.FlashSecurityReadProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(fs_protocol.cmd(addr,oplen),
                                                                fs_protocol.expect_length(oplen),
                                                                1.0)
        if tmp_res and fs_protocol.response_check(content,addr):
            return fs_protocol.get_read_val(content,oplen)
        
        return None
        

    def write_flash_security(self,addr:int,buf):
        fs_protocol = self.BOOTROM_PROTOCOL.FlashSecurityWriteProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(fs_protocol.cmd(addr,buf),
                                                                fs_protocol.expect_length(len(buf)),
                                                                1.0)
        if tmp_res and fs_protocol.response_check(content,addr):
            return True
        
        return False
    
    def erase_flash_security(self,addr:int):
        '''
        erase 4k size
        '''
        fs_protocol = self.BOOTROM_PROTOCOL.FlashSecurityEraseProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(fs_protocol.cmd(addr),
                                                                fs_protocol.expect_length,
                                                                1.0)
        if tmp_res and fs_protocol.response_check(content,addr):
            return True
        
        return False
    
    def write_flash_size(self,addr:int,buf:bytes):
        '''
        write flash size<=256 
        '''
        if len(buf)>256:
            BKLog.e('error, buf size cannt be over 256')
            return False
        fs_protocol = self.BOOTROM_PROTOCOL.FlashCustomWriteProtocol()
        if self._flash_cfg.flash_size >= 256*1024*1024:
            fs_protocol = self.BOOTROM_PROTOCOL.FlashCustomWriteExtProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(fs_protocol.cmd(addr,buf),
                                                                            fs_protocol.expect_length,
                                                                            1.0)
        if tmp_res and fs_protocol.response_check(content,addr):
            return True
        
        return False
    
    def read_flash_size(self, addr:int, oplen:int):
        '''
        write flash size<=256 
        '''
        if oplen>256:
            BKLog.e('error, read size cannt be over 256')
            return None
        fcrp_protocol = self.BOOTROM_PROTOCOL.FlashCustomReadProtocol()
        if self._flash_cfg.flash_size >= 256*1024*1024:
            fcrp_protocol = self.BOOTROM_PROTOCOL.FlashCustomReadExtProtocol()
        tmp_res, content = self.serial_instance.write_cmd_and_wait_response(fcrp_protocol.cmd(addr=addr,oplen=oplen),
                                                                            fcrp_protocol.expect_length(oplen=oplen),
                                                                            1.0)
        if tmp_res and fcrp_protocol.response_check(content,addr):
            return fcrp_protocol.get_read_content(content)
        
        return None