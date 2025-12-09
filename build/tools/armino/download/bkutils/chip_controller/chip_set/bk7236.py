import time
from serial import Timeout
from .base_chip import BaseChip
from .bk7236_security import BK7236_Security
from .bk7236_high_baudrate import BK7236_HighBaudrate
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import FLASH_SIZE_DEF, LINKCHECK,RESET_TYPE,FLASH_ERASE_CMD_EXT, FLASH_ERASE_CMD, safe_data, BinInfo,is_buf_all_0xff, BIT, BFD, crc32_ver2, bytes_reverse,bytes_reverse_per4bytes
from ...chip_controller import BaseController,FlashController,RegController
from ...common.encrypt_helper import generate_random_8bytes,aes128_ecb_encrypt

class BK7236(BaseChip):

    CHIP_ID = 0x7236
    SECOND_ID = 0x20340b10
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
        
    
    def set_high_baudrate(self, baudrate):
        '''
        use apll and change uart to high baudrate
        '''
        try:
            high_br = BK7236_HighBaudrate(serial_instance=self.ser)
            if not high_br.use_apll_set_baudrate(baudrate=baudrate):
                raise Exception(f"use apll set high baudrate {baudrate} fail.")
            base_ctrl = BaseController(serial_instance=self.ser)
            if base_ctrl.do_link_check(linkcheck_obj=LINKCHECK.BOOTROM,max_try_count=10):
                return True
            return False
        except Exception as e:
            BKLog.e(f"set_high_baudrate fail: {e}")
            return False

    #align the address on the start or end
    def align_sector_address_for_erase(self,addr:int,start_or_end:bool,reconnect_retry=False,reset_type=None,link_type=None,read_baudrate=115200):
        flash_control = FlashController(self.ser,self._flash_cfg)
        erase_addr = int(addr/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
        baudrate_backup = self.ser.baudrate
        if not self.set_baudrate(read_baudrate):
            BKLog.w(f"Set baudrate: {read_baudrate} fail.")
            return False
        time.sleep(0.1)
        cnt = 5
        while cnt>0:
            ret = flash_control.read_sector(erase_addr)
            if ret is not None:
                break
            cnt-=1  
        if ret is None:
            BKLog.e(f"read_sector fail @0x{erase_addr:x}.")         
            return False
        cnt = 5
        while cnt>0:
            if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                res = flash_control.erase_custom_size(erase_addr,FLASH_ERASE_CMD_EXT.SIZE_4K)
            else:
                res = flash_control.erase_custom_size(erase_addr,FLASH_ERASE_CMD.SIZE_4K)
            if res:
                break
            cnt-=1
        if not res:
            BKLog.e(f"erase_custom_size fail @0x{erase_addr:x}.")
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
            if not self.retry_write_sector(erase_addr,ret,reconnect_bus=reconnect_retry,reset_type=reset_type,link_type=link_type):
                BKLog.e("Write and check sector @0x{:x} fail.".format(erase_addr))
                return False
        return True
       
    #align the address on the start or end
    def align_sector_address_for_write(self, addr: int, start_or_end: bool, content: bytes, reconnect_retry=False,reset_type=None, link_type=None,read_baudrate=115200):
        #BKLog.i(f'input content length is: 0x{len(content):x}')
        #BKLog.i(f'address is: 0x{addr:x}')
        flash_control = FlashController(self.ser,self._flash_cfg)
        erase_addr = int(addr/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
        baudrate_backup = self.ser.baudrate
        if not self.set_baudrate(read_baudrate):
            BKLog.e(f"Set baudrate: {read_baudrate} fail.")
            return False
        time.sleep(0.1)
        cnt = 5
        while cnt>0:
            ret = flash_control.read_sector(erase_addr)
            if ret is not None:
                break
            cnt-=1  
        if ret is None:
            BKLog.e(f"read_sector fail @0x{erase_addr:x}.")         
            return False
        cnt = 5
        while cnt>0:
            if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                res = flash_control.erase_custom_size(erase_addr,FLASH_ERASE_CMD_EXT.SIZE_4K)
            else:
                res = flash_control.erase_custom_size(erase_addr,FLASH_ERASE_CMD.SIZE_4K)
            if res:
                break
            cnt-=1

        if not res:
            BKLog.e(f"erase_custom_size fail @0x{erase_addr:x}.")
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
        time.sleep(0.1)
        if not self.write_and_check_sector(ret,erase_addr):
            BKLog.i("write sector @0x{:x} fail, retry".format(erase_addr))
            if not self.retry_write_sector(erase_addr,ret,reconnect_bus=reconnect_retry,reset_type=reset_type,link_type=link_type):
                BKLog.e("Write and check sector @0x{:x} fail.".format(erase_addr))
                return False
        return True

    def retry_write_sector(self, flash_addr: int, buf: bytes, recnt=3, reconnect_bus=False, reset_type=None, link_type=None,link_key=None):
        flash_control = FlashController(self.ser,self._flash_cfg)
        base_controller = BaseController(serial_instance=self.ser)

        if flash_control.erase_sector(flash_addr):               
            if self.write_and_check_sector(buf, flash_addr):
                return True
        if reconnect_bus:
            baudrate_backup = self.ser.baudrate
            cnt = recnt
            while cnt>0:            
                ret = base_controller.reconnect_bus(reset_type=reset_type, linkcheck_obj=link_type, reset_baudrate=115200,max_retry_count=30,link_key=link_key)                           
                if ret:                
                    time.sleep(0.01)
                    if self.set_baudrate(baudrate_backup):               
                        if flash_control.erase_sector(flash_addr):               
                            if self.write_and_check_sector(buf, flash_addr):
                                return True
                cnt -= 1
        return False
    
    def retry_write_sector_cbus(self, flash_addr: int, buf: bytes, recnt=5, reset_type=None, link_type=None, partition_start=None, partition_size=None):
        base_controller = BaseController(serial_instance=self.ser)
        baudrate_backup = self.ser.baudrate
        cnt = recnt
        while cnt>0:
            self.ser.reset(baudrate=115200)
            ret,linktype = base_controller.get_bus(reset_type=reset_type, linkcheck_obj=link_type, max_try_count=30)                 
            if ret:                
                time.sleep(0.01)
                if self.set_baudrate(baudrate_backup):
                    if base_controller.enter_exit_cbus_mode(True,partition_start,partition_size,flash_addr,60):                                                
                        if self.write_and_check_sector(buf, flash_addr):
                            return True
            cnt -= 1
        return False
    
    def read_chip_flash_security(self,addr:int,oplen:int,file,console:bool=False):
        flash_control = FlashController(self.ser,self._flash_cfg)
        file_buf = b''
        i = 0
        BKLog.i("Start read flash security area.")
        while i< oplen:            
            rxbuf = flash_control.read_flash_security(addr+i,oplen)
            if rxbuf is None:
                return False
            file_buf+=rxbuf
            i += FLASH_SIZE_DEF.SIZE_4K
        BKLog.i("End read flash security.")
        if console:
            i=0
            while i<oplen:
                if oplen-i>0x100:
                    sublen=0x100
                else:
                    sublen=oplen-i
                BKLog.w("out stream: "+hex(file_buf[i:i+sublen]))
                i += 0x100
        else:
            with open(file,'wb') as f:
                f.write(file_buf[:oplen])
                f.close()
        return True

    def write_chip_flash_security(self,addr:int,buf):
        flash_control = FlashController(self.ser,self._flash_cfg)
        oplen = len(buf)
        i = 0
        BKLog.i("Start write flash security area.")
        while i< oplen:            
            if not flash_control.write_flash_security(addr+i,buf):
                return False          
            i += FLASH_SIZE_DEF.SIZE_4K
        BKLog.i("End write flash security.")
        return True
    
    def erase_chip_flash_security(self,addr:int,oplen):
        flash_control = FlashController(self.ser,self._flash_cfg)
        i = 0
        BKLog.i("Start erase flash security area.")
        while i< oplen:            
            if not flash_control.erase_flash_security(addr+i):
                return False          
            i += FLASH_SIZE_DEF.SIZE_4K
        BKLog.i("End erase flash security.")
        return True

###################################################################################################
#for chipset interface function
###################################################################################################
    def update_flash_mid(self, mid):
        self._flash_mid = mid
        self._flash_cfg.parse_flash_info(self._flash_mid)
    
    def parse_flash_info(self):
        tmp_mid = None
        fmp = self.BOOTROM_PROTOCOL.GetFlashMidProtocol()
        res, content = self.ser.write_cmd_and_wait_response(fmp.cmd(0x9f), fmp.expect_length, 0.1)
        if res and fmp.response_check(content):
            tmp_mid = fmp.get_mid(content)
            BKLog.i(f'flash mid is 0x{tmp_mid:x}')
            self._flash_mid = tmp_mid
            #self._flash_mid =0x1560EB
            self._flash_cfg.parse_flash_info(self._flash_mid)
            return True
            
        BKLog.e(f'read flash mid fail.')
        return False

    def write_tool_version(self, ver: str):
        '''
        write version info to flash OTP (addr: 0x000100)
        '''
        flash_ctrl = FlashController(self.ser,self._flash_cfg)
        by_ver = ver.encode('utf-8')
        addr = 0x0001000
        BKLog.i(f'version info: {by_ver}')
        flash_ctrl._write_flash_reg_enable()
        time.sleep(0.05)   
        if not flash_ctrl.erase_flash_security(addr):
            BKLog.e(f'erase flash OTP @0x{addr:x} fail.')
            return False
        time.sleep(0.05)
        flash_ctrl._write_flash_reg_enable()
        time.sleep(0.05)  
        if not flash_ctrl.write_flash_security(addr,by_ver):
            BKLog.e(f'write flash OTP @0x{addr:x} fail.')
            return False
        time.sleep(0.1)
        read_ver = flash_ctrl.read_flash_security(addr,len(by_ver))
        if read_ver is None:
            BKLog.e(f'read flash OTP @0x{addr:x} fail.')
            return False
        
        if read_ver!=by_ver:
            BKLog.e(f'data compare fail, read: {read_ver}, write: {by_ver}')
            return False
        
        return True


    def pre_check_image_file(self, bin_list:list):
        return True

    
    def pre_set_efuse_nocrc(self):
        return True

    def set_baudrate(self, baudrate, delay_ms=20,max_retry_cnt=5,isBl1:bool=True):
        if not isBl1:
            if baudrate != self.ser.baudrate:           
                cnt = max_retry_cnt
                while cnt>0:
                    self.ser.drain()
                    sbp = self.BOOTROM_PROTOCOL.SetBaudrateProtocol()
                    self.ser.write_cmd(sbp.cmd(baudrate, delay_ms))
                    time.sleep(delay_ms/2000)
                    self.ser.switch_baudrate(baudrate)
                    ret_content = self.ser.wait_for_cmd_response(sbp.expect_length, delay_ms/1000 + 0.5)
                    if len(ret_content) == sbp.expect_length:
                        if sbp.response_check(ret_content, baudrate):
                            return True
                    base_ctrl = BaseController(serial_instance=self.ser)
                    if base_ctrl.do_link_check(linkcheck_obj= BaseController.link_check_obj if BaseController.link_check_obj else LINKCHECK.BOOTROM):
                        return True
                    cnt-=1
                    BKLog.e('reconnect bus to retry')
                    if not base_ctrl.reconnect_bus(reset_type=RESET_TYPE.MULTI,linkcheck_obj=BaseController.link_check_obj if BaseController.link_check_obj else LINKCHECK.BOOTROM,max_retry_count=20):
                        BKLog.e('reconnect bus fail')
                        BKLog.e('baudrate switch fail')
                        return False                   
                BKLog.e('baudrate switch fail')
                return False
            else:
                BKLog.i('ignore baudrate switch')
                return True
        else:
            if baudrate != self.ser.baudrate:  
                if baudrate<=2600000:         
                    cnt = max_retry_cnt
                    while cnt>0:
                        self.ser.drain()
                        sbp = self.BOOTROM_PROTOCOL.SetBaudrateProtocol()
                        self.ser.write_cmd(sbp.cmd(baudrate, delay_ms))
                        time.sleep(delay_ms/2000)
                        self.ser.switch_baudrate(baudrate)
                        ret_content = self.ser.wait_for_cmd_response(sbp.expect_length, delay_ms/1000 + 0.5)
                        if len(ret_content) == sbp.expect_length:
                            if sbp.response_check(ret_content, baudrate):
                                return True
                        base_ctrl = BaseController(serial_instance=self.ser)
                        if base_ctrl.do_link_check(linkcheck_obj=LINKCHECK.BOOTROM,max_try_count=10):
                            return True
                        
                        BKLog.w('reconnect bus to retry')
                        if not base_ctrl.reconnect_bus(reset_type=RESET_TYPE.DTR_RTS,linkcheck_obj=LINKCHECK.BOOTROM,max_retry_count=20):
                            BKLog.e('reconnect bus fail')
                            BKLog.e('baudrate switch fail')
                            return False
                        cnt-=1
                elif baudrate==3000000:
                    BKLog.w(f'use apll to set high baudrate.')
                    if self.set_high_baudrate(baudrate):
                        return True
                    BKLog.w(f'use apll to set high baudrate:{baudrate} fail.')
                else:
                    BKLog.e(f'not support baudrate: {baudrate}')           
                BKLog.e('baudrate switch fail')
                return False
            else:
                BKLog.i('ignore baudrate switch')
                return True

    def un_protect_flash(self, protect: bool):
        flash_control = FlashController(self.ser,self._flash_cfg)
        if protect:
            return flash_control.protect_flash()
        else:
            return flash_control.unprotect_flash()
     

    #read 4k data from flash and check by crc
    def read_and_check_sector(self,addr:int,recnt=5):
        flash_control = FlashController(self.ser,self._flash_cfg)      
        ret = None
        cnt = recnt
        while cnt>0:
            ret=flash_control.read_sector(addr)
            
            if ret and flash_control.check_crc_ver2(ret,addr,FLASH_SIZE_DEF.SIZE_4K, recnt=recnt):
                return ret 
            cnt-=1
                  
        BKLog.e("Read and check sector @0x{:x} fail.".format(addr))
        return None
    
    #write 4k data to flash and check by crc
    def write_and_check_sector(self,buf_sec:bytes,addr:int):
        flash_control = FlashController(self.ser,self._flash_cfg)
        length = len(buf_sec)
        if not flash_control.write_sector(addr,buf_sec):
            BKLog.e(f"Write sector @0x{addr:x} fail, buf length: 0x{length:x}")
            return False
        if not flash_control.check_crc_ver2(buf_sec,addr,length):  
            BKLog.e("Check CRC32 @0x{:x}, ".format(addr)+"len: 0x{:x} fail.".format(length))
            return False   
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
                end_addr = int(end_addr/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K

            erase_size = end_addr-start_addr
            i=0

            while i<erase_size:
                current_progress = 100 * i / erase_size
                BKLog.progress_log('BK',"Erasing Flash", int(current_progress))
                if erase_size-i>int(FLASH_SIZE_DEF.SIZE_64K):
                    if (start_addr+i)&0xffff:
                        cnt = retry
                        ret = False
                        while cnt>0 and not ret:
                            BKLog.d("Erase sector @0x{:x} .".format(start_addr+i))
                            if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                                ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD_EXT.SIZE_4K)
                            else:
                                ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD.SIZE_4K)
                            cnt-=1                       
                        if not ret:
                            BKLog.e("Erase sector @0x{:x} fail.".format(start_addr+i))
                            return False
                        i+=FLASH_SIZE_DEF.SIZE_4K
                    else:
                        cnt = retry
                        ret = False                    
                        while cnt>0 and not ret:
                            BKLog.d("Erase block(64K) @0x{:x} .".format(start_addr+i))
                            if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                                ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD_EXT.SIZE_64K)
                            else:
                                ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD.SIZE_64K)
                            cnt-=1
                        if not ret:
                            BKLog.e("Erase block(64K) @0x{:x} fail.".format(start_addr+i))
                            return False
                        i+=FLASH_SIZE_DEF.SIZE_64K                                                              
                else: 
                    cnt = retry
                    ret = False
                    while cnt>0 and not ret:
                        BKLog.d("Erase sector @0x{:x} .".format(start_addr+i))
                        if self._flash_cfg.flash_size >= 256 * 1024 * 1024:
                            ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD_EXT.SIZE_4K)
                        else:
                            ret = flash_control.erase_custom_size(start_addr+i,FLASH_ERASE_CMD.SIZE_4K)
                        cnt-=1                       
                    if not ret:
                        BKLog.e("Erase sector @0x{:x} fail.".format(start_addr+i))
                        return False
                    i+=FLASH_SIZE_DEF.SIZE_4K                      
                        
            BKLog.i("end erase.")    
        return True
      
    #download bytes to flash
    def download_flash(self, wbuf: bytes, start_addr: int, retry=5, reconnect_retry=False,reset_type=None, link_type=None, read_baudrate=115200):
        file_len = len(wbuf)
        #align 0x100 bytes
        if file_len % 0x100:
            wbuf += b'\xff' * (0x100 - file_len % 0x100)
            file_len = len(wbuf)
        
        end_addr = start_addr + file_len
        BKLog.i("write flash start @0x{:x}, ".format(start_addr)+"length: 0x{:x}".format(file_len))
        if start_addr & 0xfff:
            if not self.align_sector_address_for_write(start_addr,True,wbuf,reconnect_retry,reset_type=reset_type,link_type=link_type,read_baudrate=read_baudrate):
                BKLog.e("Align start address @0x{:x} fail.".format(start_addr))
                return False
            wbuf = wbuf[(FLASH_SIZE_DEF.SIZE_4K-start_addr & 0xfff):]
            start_addr = int((start_addr+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
            file_len = len(wbuf)
        
        if end_addr & 0xfff:
            if not self.align_sector_address_for_write(end_addr,False,wbuf,reconnect_retry,reset_type=reset_type,link_type=link_type,read_baudrate=read_baudrate):
                BKLog.e("Align end address @0x{:x} fail.".format(end_addr))
                return False
            wbuf = wbuf[:len(wbuf)-(end_addr & 0xfff)]         
            end_addr = int(end_addr /FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
            file_len = len(wbuf)
        i=0
        while i<file_len:
            current_progress = 100 * i / file_len
            BKLog.progress_log('BK',"Writing Flash", int(current_progress))
            if not is_buf_all_0xff(wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K]):
                BKLog.d("write sector @0x{:x} .".format(i+start_addr))               
                if not self.write_and_check_sector(wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K],i+start_addr):
                    BKLog.i("write sector @0x{:x} fail, retry".format(i+start_addr))
                    if not self.retry_write_sector(i+start_addr, wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K], retry,reconnect_retry, reset_type, link_type):
                        BKLog.e("write sector @0x{:x} by retry fail.".format(i+start_addr))
                        return False                       
            i+=FLASH_SIZE_DEF.SIZE_4K
        return True
    
    #download flash and check crc at the end of writting bin
    def download_flash_checkCRC_at_end(self, wbuf: bytes, start_addr: int, retry=5,reconnect_retry=False, reset_type=None, link_type=None, read_baudrate=115200):
        file_len = len(wbuf)
        #align 0x100 bytes
        if file_len % 0x100:
            wbuf += b'\xff' * (0x100 - file_len % 0x100)
            file_len = len(wbuf)
        
        end_addr = start_addr + file_len
        BKLog.i("write flash start @0x{:x}, ".format(start_addr)+"length: 0x{:x}".format(file_len))
        buf_backup = wbuf
        start_backup = start_addr
        len_backup = file_len
        flash_ctrl = FlashController(serial_instance=self.ser,flash_cfg=self._flash_cfg)
        if start_addr & 0xfff:
            if not self.align_sector_address_for_write(start_addr,True,wbuf,reconnect_retry,reset_type=reset_type,link_type=link_type,read_baudrate=read_baudrate):
                BKLog.e("Align start address @0x{:x} fail.".format(start_addr))
                return False
            wbuf = wbuf[(FLASH_SIZE_DEF.SIZE_4K-start_addr & 0xfff):]
            start_addr = int((start_addr+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
            file_len = len(wbuf)
        
        if end_addr & 0xfff:
            if not self.align_sector_address_for_write(end_addr,False,wbuf,reconnect_retry,reset_type=reset_type,link_type=link_type,read_baudrate=read_baudrate):
                BKLog.e("Align end address @0x{:x} fail.".format(end_addr))
                return False
            wbuf = wbuf[:len(wbuf)-(end_addr & 0xfff)]
            end_addr = int(end_addr /FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
            file_len = len(wbuf)
        i=0
        while i<file_len:
            current_progress = 100 * i / file_len
            BKLog.progress_log('BK',"Writing Flash", int(current_progress))
            if not is_buf_all_0xff(wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K]):
                BKLog.d("write sector @0x{:x} .".format(i+start_addr))
                if not flash_ctrl.write_sector(i+start_addr,wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K]):
                    return False
                                     
            i+=FLASH_SIZE_DEF.SIZE_4K
        if not flash_ctrl.check_crc_ver2(buf_backup,start_backup,len_backup):
            BKLog.w("Check CRC for address @0x{:x} fail.".format(end_addr))
            return False
        return True


    #read flash and save in file if console set false
    def dump_flash(self,start,oplen,file,recnt=5,console=False):
        file_buf = b''
        cnt = recnt
        i = 0
        while i< oplen:
            current_progress = 100 * i / oplen
            BKLog.progress_log('BK',"Reading Flash", int(current_progress))
            ret = self.read_and_check_sector(start+i,cnt)
            if ret==None:
                BKLog.e("Read flash fail.")
                return False
            file_buf+=ret
            i += FLASH_SIZE_DEF.SIZE_4K
        if console:
            BKLog.w("out stream: "+bytes.hex(file_buf[:oplen]))
            
        else:
            with open(file,'wb') as f:
                f.write(file_buf[:oplen])
                f.close()
        return True
    

    def write_flash_info(self,flash_addr,buf):
        '''
        write device name or mac to flash by flash address
        '''
        flash_control = FlashController(self.ser,self._flash_cfg)
        align_addr = flash_addr&0xfffff000

        baudrate_backup = self.ser.baudrate
        if not self.set_baudrate(115200):
            BKLog.e("Set baudrate: 115200 fail.")
            return False
        time.sleep(0.01)             
        #read the sector first
        rbytes = flash_control.read_sector(align_addr)
        if rbytes is None:
            BKLog.e(f"Read sector fail @0x{align_addr:x}")
            return False
        #replace the buf to sector bytearray
        wbytes = bytearray(rbytes)
        tmp = int(flash_addr&0xfff)
        oplen = len(buf)
        
        wbytes[tmp:(tmp+oplen)] = buf
        
        #erase the sector
        if self._flash_cfg.flash_size>= 256*1024*1024:
            erase_cmd = FLASH_ERASE_CMD_EXT.SIZE_4K
        else:
            erase_cmd = FLASH_ERASE_CMD.SIZE_4K
        if not flash_control.erase_custom_size(align_addr,erase_cmd):
            BKLog.e(f"Erase sector fail @0x{align_addr:x}")
            return False
        if not self.set_baudrate(baudrate_backup):
            BKLog.e(f"Set baudrate: {baudrate_backup} fail.")
            return False
        #write the sector bytearray again
        if not self.write_and_check_sector(wbytes,align_addr):
            BKLog.e(f"Write sector fail @0x{align_addr:x}")
            return False
        return True
    
    def download_flash_cbus(self, wbuf:bytes, partition_start:int, partition_size:int, start_addr:int, retry=5, reset_type=None, link_type=None):
        file_len = len(wbuf)
        BKLog.i(f'original buf len: 0x{file_len:x}')
        page_write = False
        #align 0x100 bytes
        if file_len % 0x1000:
            wbuf += b'\xff' * (0x1000 - file_len % 0x1000)
            file_len = len(wbuf)
               
        BKLog.i("write flash start @0x{:x}, ".format(start_addr)+"length: 0x{:x}".format(file_len))
        basectrl = BaseController(serial_instance=self.ser)
        BKLog.i("enter cbus mode...")
        if not basectrl.enter_exit_cbus_mode(self._flash_cfg.flash_size,True,partition_start,partition_size,start_addr,50):
            BKLog.e('enter cbus mode fail.')
            return False
        BKLog.i("enter cbus mode done.")
        # if file_len%0x1000:           
        #     page_write_size = int(file_len%0x1000)                      
        #     file_len = file_len-page_write_size
        #     page_write= True
        
        i=0
        while i<file_len:
            current_progress = 100 * i / file_len
            BKLog.progress_log(self.ser.ser.name,"Writing Flash", int(current_progress))
            if not is_buf_all_0xff(wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K]):
                if not self.write_and_check_sector(wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K],i+start_addr):
                    if not self.retry_write_sector(i+start_addr, wbuf[i:i+FLASH_SIZE_DEF.SIZE_4K], retry, True,reset_type, link_type):
                        return False                       
            i+=FLASH_SIZE_DEF.SIZE_4K

        # if page_write:
        #     fc = FlashController(serial_instance=self.ser,flash_cfg=self._flash_cfg)           
        #     sub_start = start_addr+file_len
        #     sz = 0
        #     while sz<page_write_size:
        #         if not fc.write_flash_size(sub_start+sz,wbuf[file_len+sz:file_len+sz+int(FLASH_SIZE_DEF.SIZE_PAGE/4)]):
        #             return False
        #         sz+=int(FLASH_SIZE_DEF.SIZE_PAGE/4)
        #     if not fc.check_crc_ver2(wbuf[file_len:],sub_start,page_write_size):
        #         return False
            
        BKLog.i("exit cbus mode...")               
        if not basectrl.enter_exit_cbus_mode(self._flash_cfg.flash_size,False,partition_start,partition_size,start_addr):
            BKLog.e('exit cbus mode fail.')
            return False
        return True
    

    def check_aes_key_enable(self):
        security = BK7236_Security(serial_instance=self.ser)        
        ret,val = security._read_efuse(3,1)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False
        if val[0]&BIT(5)!=BIT(5):
            return 0      
        return 1
    
    def check_aes_key_otp_data(self):
        security = BK7236_Security(serial_instance=self.ser)
        ret,msg = security._read_chip_otp(0x4b100460,0x20)
        if ret is None:
            BKLog.e(f"Read otp fail.")
            return False
        for idx in range(0x20/4):
            if ret[idx*4:idx*4+4]!=[0,0,0,0]:
                return True
        return False
    
    def check_aes_key_otp_permission(self):
        '''
        return False for execute fail, 0 for all permission is 0, 1 for permission is 0xf
        '''
        security = BK7236_Security(serial_instance=self.ser)
        pm_na = False
        for rg in range(0x20/4):
            pm = security._read_ot_permission_value(0x4b100460+rg*4)
            if pm is None:
                BKLog.e(f"Read otp permission fail.")
                return False
            if pm == 0xf:
                pm_na = True
        if pm_na:
            ret,msg = security._set_otp_permission_oplen(0x4b100460,0x20,'na')
            if not ret:
                BKLog.e(f"set otp permission fail.")
                return False
            else:
                return 1
            
        else:
            return 0
            

    def check_bkaesrdm_status(self):
        '''
        return False for execute fail, 0 for bkaesrdm keywords exist, 1 for not
        '''
        flashctrl = FlashController(serial_instance=self.ser,flash_cfg=self._flash_cfg)
        readout = flashctrl.read_flash_size(addr=0x44,oplen=16)
        if readout is None:
            return False,None,None
        keywords = 'BKAESRDM'
        if readout[:8] == bytes(keywords,'utf-8'):
            bl2_start_addr = readout[8]+(readout[9]<<8)+(readout[10]<<16)+(readout[11]<<24)
            bl2_end_addr = readout[12]+(readout[13]<<8)+(readout[14]<<16)+(readout[15]<<24)
            return 0,bl2_start_addr,bl2_end_addr
        return 1,None,None
    
    def read_bl2_version(self):
        flashctrl = FlashController(serial_instance=self.ser,flash_cfg=self._flash_cfg)
        readout = flashctrl.read_flash_size(0x3004,4)
        if readout is None:
            return False
        version = readout[0]+(readout[1]<<8)+(readout[2]<<16)+(readout[3]<<24)
        return version
        
    def reboot_and_enter_write_outter_flash_mode(self,reset_type=RESET_TYPE.DTR_RTS,max_retry_count=10):
        basectrl = BaseController(serial_instance=self.ser)
        BKLog.i("do reboot...")
        if reset_type==RESET_TYPE.DTR_RTS:       
            basectrl.do_reset(reset_type=RESET_TYPE.DTR_RTS,baudrate=115200)
        else:
            cnt = 3
            while cnt>0:
                basectrl.do_reboot()
                cnt-=1
                time.sleep(0.01)
        
        self.ser.reset(115200)        
        time.sleep(2)
        cnt = max_retry_count
        while cnt>0:
            if basectrl.enter_outter_flash_mode(max_retry_count=1):
                break
            if reset_type==RESET_TYPE.DTR_RTS:       
                basectrl.do_reset(reset_type=RESET_TYPE.DTR_RTS,baudrate=115200)
            else:
                cnt = 3
                while cnt>0:
                    basectrl.do_reboot()
                    cnt-=1
                    time.sleep(0.01)
            time.sleep(2)
            cnt-=1
        if cnt<=0:
            BKLog.w("enter outter flash mode fail.")
            return False
        BKLog.i("enter outter flash mode ok.")
        if not basectrl.enter_outter_flash_mode_success(max_retry_count=3):
            BKLog.w("check enter outter flash mode success fail.")
            return False
        BKLog.i("enter outter flash mode success.")
        time.sleep(2)

        BKLog.i("outter flash do link check.")
        if not basectrl.do_link_check(linkcheck_obj=LINKCHECK.BOOTROM):
            BKLog.w("outter flash do link check fail.")
            return False
        BKLog.i("outter flash do link check ok.")
        return True

    def cal_and_write_aes128_data(self,key):
        '''
        生成8 bytes随机数，+读出来的8 bytes device id，使用aes128加密后写入otp170
        :param key: AES128密钥
        :returns:
            写入成功返回True，失败返回False
        '''
        #generate 8 bytes random number
        rdm_content = generate_random_8bytes()
        #read 8 bytes device id
        security = BK7236_Security(serial_instance=self.ser)
        if not security._set_otp_write_enable():
            BKLog.e(f"Set OTP write enable fail.")
            return False
        time.sleep(0.1)
        ret, val = security._read_otp_by_regaddr(security.security_otp_apb_start_addr+0x3c0,8)
        if not ret:
            BKLog.e(f"Read OTP fail, address: 0x{security.security_otp_enable_addr:x}.")
            return False
        src_data = rdm_content+val
        enc_data = aes128_ecb_encrypt(src_data,key)
        if not enc_data:
            BKLog.e(f"aes128 encrypt fail.")
            return False
        ret,msg = security._write_chip_otp(security.security_otp_apb_start_addr+0x168,rdm_content.hex())
        if not ret:
            BKLog.e(f"_write_chip_otp fail, address: 0x{security.security_otp_apb_start_addr+0x168:x}.")
            BKLog.e(msg)
            return False
        ret,msg = security._write_chip_otp(security.security_otp_apb_start_addr+0x170,enc_data.hex())
        if not ret:
            BKLog.e(f"_write_chip_otp fail, address: 0x{security.security_otp_apb_start_addr+0x170:x}.")
            BKLog.e(msg)
            return False,msg
        return True,msg
############################################################################################################
# security operation
############################################################################################################

    
    def write_chip_otp(self, otp_addr:int, content:str, reverse_content=False,bigendian=False):
        '''
        write otp by address information
        '''
        security = BK7236_Security(serial_instance=self.ser)
        return security._write_chip_otp(otp_addr,content,reverse_content,bigendian)
               
    def set_otp_permission_oplen(self, reg_addr, oplen, permission):
        '''
        reg_addr: start OTP address
        oplen: reg count
        permission: wr/ro/na
        '''
        security = BK7236_Security(serial_instance=self.ser)
        return security._set_otp_permission_oplen(reg_addr,oplen,permission)
           

    def write_efuse_by_bit(self,reg_addr,bit,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7236_Security(serial_instance=self.ser)
        return security._write_efuse_by_bit(reg_addr,bit,value)
    
    def write_efuse(self,reg_addr,value):
        '''
        write one byte value, 8 bits
        'addr' is the pos of efuse
        '''
        security = BK7236_Security(serial_instance=self.ser)
        return security._write_efuse(reg_addr,value)

    
    def read_chip_otp(self,otp_addr, oplen, reverse_content=False,bigendian=False):
        security = BK7236_Security(serial_instance=self.ser)
        ret,msg = security._read_chip_otp(otp_addr,oplen,bigendian)
        if ret is not None:
            if reverse_content:
                return bytes_reverse_per4bytes(ret),msg
            else:
                return ret,msg
        else:
            return None,msg

    def read_chip_uid(self):
        '''
        read chip UID from OTP, length: 5 bytes
        '''
        security = BK7236_Security(serial_instance=self.ser)
        if not security._set_otp_write_enable():
            BKLog.e(f"Set OTP write enable fail.")
            return False, None
        time.sleep(0.1)
        ret, val = security._read_otp_by_regaddr(0x4b1007c0,5)
        if not ret:
            BKLog.e(f"Read OTP fail, address: 0x{security.security_otp_enable_addr:x}.")
            return False, None
        
        return True,val
    
    def read_efuse(self,addr,oplen):
        '''
        read chip efuse info
        'addr' is start efuse address
        if 'oplen' set to 0, read all efuse value
        '''
        
        security = BK7236_Security(serial_instance=self.ser)

        if oplen==0:
            byteslen = int(security.security_efuse_length/8)
        
        ret,val = security._read_efuse(addr,byteslen)
        if not ret:
            BKLog.e(f"Read efuse fail.")
            return False,None
        return True,val

    def write_safe_json(self, safeinfo: safe_data):
        '''
        write safe info set by json file
        '''
        security = BK7236_Security(serial_instance=self.ser)
        if safeinfo.user_operate_enable:
            for usrop in safeinfo.user_operate_list:
                if usrop.status=="true":
                    if usrop.mode=="write":
                        #write otp info
                        if usrop.name.find("otp")!=-1:
                            ret,msg = security._write_otp_info(usrop)
                            if not ret:
                                return False,msg
                        #write flash info
                        elif usrop.name.find("flash")!=-1:
                            pass
                    else:
                        #read otp
                        if usrop.name.find("otp")!=-1:
                            ret,msg = security._read_otp_info(usrop)
                            if not ret:
                                return False,msg
                            BKLog.w(f'read {usrop.name}: {ret}')
                        #write flash info
                        elif usrop.name.find("flash")!=-1:
                            pass

            BKLog.i("write otp ok.")
        if safeinfo.security_data_enable:
            for secdata in safeinfo.security_data_list:                
                if secdata.status=="true":
                    if secdata.mode=="write":
                        ret,msg = security._write_otp_info(secdata)
                        if not ret:
                            return False,msg
                    else:
                        #read otp                        
                        ret,msg = security._read_otp_info(secdata)
                        if not ret:
                            return False,msg
                        BKLog.w(f'read {secdata.name}: {ret}')                        
            BKLog.i("write otp ok.")
        if safeinfo.efuse_enable:
            for efu in safeinfo.efuse_info_list:
                if efu.status=="true":
                    if efu.mode=="write":
                        addr = int(efu.byte_addr,16)
                        value = int(efu.data,16)
                        ret,msg = security._write_efuse(addr,value)
                        if not ret:
                            return False,msg
                    else:
                        addr = int(efu.byte_addr,16)
                        length = int(efu.length,16)
                        ret,efuse_value = security._read_efuse(addr,length)
                        if not ret:
                            return False,None
                        BKLog.w(f'read {efu.name}: {bytes.hex(bytes(efuse_value))}')  
            BKLog.i("write efuse ok.")                
        if safeinfo.security_ctrl_enable:
            for sectrl in safeinfo.security_ctrl:
                slst = sectrl.split(',')
                if len(slst)==3:
                    if slst[2]=="1":
                        byte_idx = int(slst[0])
                        bit_idx = int(slst[1])
                        ret,msg = security._write_efuse_by_bit(byte_idx,bit_idx,1)
                        if not ret:
                            return False,msg
            BKLog.i("write efuse ok.")            
        return True,None

    
