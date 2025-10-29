import time
from .flash_controller import FlashController
from ..bk_log import BKLog
from .serial_helper import SerialHelper
from ..flash_adapter.flash_config import FlashConfig
from ..protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from .flash_tsingteng_test_data import g_hex_data


class FlashControllerTsingteng(FlashController):
    '''
    about tsingteng flash fix solution
    '''
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self, serial_instance: SerialHelper, flash_cfg: FlashConfig):
        super().__init__(serial_instance,flash_cfg)
        
    
    def tsingteng_flash_enter_factory_mode(self):
        fac_cmd_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashFactoryCmdProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(fac_cmd_ctrl.cmd(0x33),fac_cmd_ctrl.expect_length,0.8)
        if not res or not fac_cmd_ctrl.response_check(content):
            return False
        res, content = self.serial_instance.write_cmd_and_wait_response(fac_cmd_ctrl.cmd(0xCC),fac_cmd_ctrl.expect_length,0.8)
        if not res or not fac_cmd_ctrl.response_check(content):
            return False
        res, content = self.serial_instance.write_cmd_and_wait_response(fac_cmd_ctrl.cmd(0xAA),fac_cmd_ctrl.expect_length,0.8)
        if not res or not fac_cmd_ctrl.response_check(content):
            return False
        return True

    def tsingteng_flash_exit_factory_mode(self):
        fac_cmd_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashFactoryCmdProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(fac_cmd_ctrl.cmd(0x55),fac_cmd_ctrl.expect_length,0.8)
        if not res or not fac_cmd_ctrl.response_check(content):
            return False
        res, content = self.serial_instance.write_cmd_and_wait_response(fac_cmd_ctrl.cmd(0x88),fac_cmd_ctrl.expect_length,0.8)
        if not res or not fac_cmd_ctrl.response_check(content):
            return False
        return True

    def tsingteng_flash_nvm_erase(self, addr:int):

        self._write_flash_reg_enable()
        th_nvm_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashNVMEraseProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(th_nvm_ctrl.cmd(addr),th_nvm_ctrl.expect_length,0.8)
        if res and th_nvm_ctrl.response_check(content):
            return True
        
        return False

    def tsingteng_flash_nvm_read(self, addr:int, oplen:int):
        th_nvm_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashNVMReadProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(th_nvm_ctrl.cmd(addr,oplen),th_nvm_ctrl.expect_length(oplen),0.8)
        if res and th_nvm_ctrl.response_check(content):
            return th_nvm_ctrl.get_read_val(content,oplen)
        
        return None

    def tsingteng_flash_nvm_write(self, addr:int, data:bytes):

        self._write_flash_reg_enable()
        th_nvm_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashNVMWriteProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(th_nvm_ctrl.cmd(addr,data),th_nvm_ctrl.expect_length(len(data)),0.8)
        if res and th_nvm_ctrl.response_check(content):
            return True
        
        return False

    def tsingteng_flash_reg_write(self, addr:int, data:bytes):
        th_reg_ctrl = self.BOOTROM_PROTOCOL.TsingtengFlashRegWriteProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(th_reg_ctrl.cmd(addr,data),th_reg_ctrl.expect_length(len(data)),0.8)
        if res and th_reg_ctrl.response_check(content):
            return True
        
        return False


    def tsingteng_flash_nvm_erase_and_check(self, flash_mid:int, addr:int, reg_addr:int=0x000203, reg_v=None):
        
        for _ in range(2):
            if not self.tsingteng_flash_nvm_erase(addr):
                return False
        if reg_v is not None:
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(reg_addr,reg_v):
                    return False
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=0xFF:
                    return False
            rbuf = self.tsingteng_flash_nvm_read(addr+0x100,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=0xFF:
                    return False
            if flash_mid==0x1460CD or flash_mid==0x1560EB:
                rbuf = self.tsingteng_flash_nvm_read(addr+0x200,0x100)
                if rbuf is None:
                    return False
                for idx in range(0x100):
                    if rbuf[idx]!=0xFF:
                        return False
                rbuf = self.tsingteng_flash_nvm_read(addr+0x300,0x100)
                if rbuf is None:
                    return False
                for idx in range(0x100):
                    if rbuf[idx]!=0xFF:
                        return False
                    
    def tsingteng_flash_nvm_copy_and_check(self, flash_mid:int, addr_from:int, addr_to:int, addr_to_b:int, reg_addr:int=0x000203, reg_v=None):

        rbuf = self.tsingteng_flash_nvm_read(addr_from,0x100)
        if rbuf is None:
            return False
        user_data = rbuf
        if not self.tsingteng_flash_nvm_write(addr_to,user_data[0:0x40]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0x400000,user_data[0x40:0x80]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0x800000,user_data[0x80:0xC0]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0xC00000,user_data[0xC0:0x100]):
            return False
        
        if not self.tsingteng_flash_nvm_write(addr_to_b,user_data[0:0x40]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0x400000,user_data[0x40:0x80]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0x800000,user_data[0x80:0xC0]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0xC00000,user_data[0xC0:0x100]):
            return False
        
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_to,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=user_data[idx]:
                    return False
                
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_to_b,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=user_data[idx]:
                    return False

    def tsingteng_flash_nvm_copy_and_check_flag(self, addr_from:int, addr_to:int, addr_to_b:int, transfer:bool=False,transfer_9byte=0x71, reg_addr:int=0x000203, reg_v=None):
        user_data = []
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_from,0x100)
            if rbuf is None:
                return False
            user_data.append(rbuf)
        for idx in range(0x100):
            if user_data[0][idx]!=user_data[1][idx] or user_data[1][idx]!=user_data[2][idx]:
                return False

        if transfer:
            user_data[0][9]=transfer_9byte
            user_data[0][13] |= (1<<6)
            if user_data[0][13]>>7 == 0:
                user_data[0][13] |= (1<<7)
            else:
                user_data[0][13] &= 0x7f

        if not self.tsingteng_flash_nvm_write(addr_to,user_data[0][0:0x40]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0x400000,user_data[0][0x40:0x80]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0x800000,user_data[0][0x80:0xC0]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to+0xC00000,user_data[0][0xC0:0x100]):
            return False
        
        if not self.tsingteng_flash_nvm_write(addr_to_b,user_data[0][0:0x40]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0x400000,user_data[0][0x40:0x80]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0x800000,user_data[0][0x80:0xC0]):
            return False
        if not self.tsingteng_flash_nvm_write(addr_to_b+0xC00000,user_data[0][0xC0:0x100]):
            return False
        

        if reg_v is not None:
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(reg_addr,reg_v):
                    return False

        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_to,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=user_data[0][idx]:
                    return False
        
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_to_b,0x100)
            if rbuf is None:
                return False
            for idx in range(0x100):
                if rbuf[idx]!=user_data[0][idx]:
                    return False
                
        
    def tsingteng_flash_check_13_14_value(self, addr_13:int, addr_14:int):
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_13,1)
            if rbuf is None:
                return False           
            if rbuf[0]!=0x13:
                return False
            
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_14,1)
            if rbuf is None:
                return False           
            if rbuf[0]!=0x14:
                return False
            
    def tsingteng_flash_write_flag_and_check(self, addr_w:int, addr_w_b:int,addr_r:int=0,reg_addr:int=0x000203, reg_v=None):
        if addr_r!=0:
            for _ in range(3):
                rbuf = self.tsingteng_flash_nvm_read(addr_r,2)
                if rbuf is None:
                    return False
                if rbuf[0]!=0 or rbuf[1]!=0xff:
                    return False
        wbuf = bytes([0x55])
        if not self.tsingteng_flash_nvm_write(addr_w,wbuf):
            return False
        
        if not self.tsingteng_flash_nvm_write(addr_w_b,wbuf):
            return False
        
        if reg_v is not None:
            if not self.tsingteng_flash_reg_write(reg_addr,reg_v):
                return False

        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_w,1)
            if rbuf is None:
                return False
            if rbuf[0]!=0x55:
                return False
            
        for _ in range(3):
            rbuf = self.tsingteng_flash_nvm_read(addr_w_b,1)
            if rbuf is None:
                return False
            if rbuf[0]!=0x55:
                return False


    def tsingteng_flash_scratch_40HB(self):
        bcontinue = False
        flash_mid = 0x1360CD
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        #set nvm read correct
        buf_w = bytes([0x03])
        if not self.tsingteng_flash_reg_write(0x0d2001,buf_w):
            return False
        buf_w = bytes([0x02])
        if not self.tsingteng_flash_reg_write(0x010001,buf_w):
            return False
        rbuf = self.tsingteng_flash_nvm_read(0xed0000,3)
        rec_flag = (rbuf[0]<<16) + (rbuf[1]<<8) + rbuf[2]
        if rec_flag==0xffffff:            
            res = self.tsingteng_flash_nvm_read(0x0d6000,1)
            if res[0]&0x7f==0x7c:
                rec_flag = 0x555555
            elif res[0]&0x7f==0x3C:
                if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x003000):
                    return False
                if not self.tsingteng_flash_nvm_copy_and_check(flash_mid,0x006000,0x003000,0x003100):
                    return False               
                if not self.tsingteng_flash_write_flag_and_check(0xed0000,0xed0100,0x403000):
                    return False
                bcontinue=True
            else:
                return False
        if rec_flag==0x55ffff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x006000,0x000203,bytes([0xcf,0xfd,0x00,0x00])):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xff,0xfd,0x00,0x00])):
                    return False
           
            if not self.tsingteng_flash_nvm_copy_and_check_flag(0x003000,0x006000,0x006100,True,0x71,0x000203,bytes([0xed,0xfd,0x00,0x00])):
                return False
            
            if not self.tsingteng_flash_write_flag_and_check(0xee0000,0xee0100,0x406000):
                return False
            bcontinue=True
        
        if rec_flag==0x5555ff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x003000,0x000203,bytes([0xcf,0xfd,0x00,0x00])):
                return False
            if not self.tsingteng_flash_write_flag_and_check(0xef0000,0xef0100,0,0x000203,bytes([0xed,0xfd,0x00,0x00])):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xff,0xfd,0x00,0x00])):
                    return False
            bcontinue = True
        if rec_flag==0x555555 or bcontinue:
            if not self.tsingteng_flash_exit_factory_mode():
                return False
        return True


    def tsingteng_flash_scratch_80HB(self):
        bcontinue = False
        flash_mid = 0x1460CD
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        #set nvm read correct
        buf_w = bytes([0x03])
        if not self.tsingteng_flash_reg_write(0x0d2001,buf_w):
            return False
        buf_w = bytes([0x02])
        if not self.tsingteng_flash_reg_write(0x010001,buf_w):
            return False
        rbuf = self.tsingteng_flash_nvm_read(0xed0100,3)
        rec_flag = (rbuf[0]<<16) + (rbuf[1]<<8) + rbuf[2]
        if rec_flag==0xffffff:            
            res = self.tsingteng_flash_nvm_read(0x0d6000,1)
            if res[0]&0x7f==0x7c:
                rec_flag = 0x555555
            elif res[0]&0x7f==0x3C:
                if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x003000):
                    return False
                if not self.tsingteng_flash_nvm_copy_and_check(flash_mid,0x006000,0x003000,0x003200):
                    return False               
                if not self.tsingteng_flash_write_flag_and_check(0xed0100,0xed0300,0x403000):
                    return False
                bcontinue=True
            else:
                return False
        if rec_flag==0x55ffff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x006000,0x000203,bytes([0xcf,0xfd,0x00,0x00])):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xff,0xfd,0x00,0x00])):
                    return False
           
            if not self.tsingteng_flash_nvm_copy_and_check_flag(0x003000,0x006000,0x006200,True,0x71,0x000203,bytes([0xed,0xfd,0x00,0x00])):
                return False
            
            if not self.tsingteng_flash_write_flag_and_check(0xee0100,0xee0300,0x406000):
                return False
            bcontinue=True
        
        if rec_flag==0x5555ff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x003000,0x000203,bytes([0xcf,0xfd,0x00,0x00])):
                return False
            if not self.tsingteng_flash_write_flag_and_check(0xef0100,0xef0300,0,0x000203,bytes([0xed,0xfd,0x00,0x00])):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xff,0xfd,0x00,0x00])):
                    return False
            bcontinue = True
        if rec_flag==0x555555 or bcontinue:
            if not self.tsingteng_flash_exit_factory_mode():
                return False
        return True


    def tsingteng_flash_scratch_16HB(self):
        bcontinue = False
        flash_mid = 0x1560EB
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        #set nvm read correct
        buf_w = bytes([0x03])
        if not self.tsingteng_flash_reg_write(0x0d2001,buf_w):
            return False
        buf_w = bytes([0x02])
        if not self.tsingteng_flash_reg_write(0x010001,buf_w):
            return False
        rbuf = self.tsingteng_flash_nvm_read(0xed0b00,3)
        rec_flag = (rbuf[0]<<16) + (rbuf[1]<<8) + rbuf[2]
        if rec_flag==0xffffff:            
            res = self.tsingteng_flash_nvm_read(0x0d0500,1)
            if res[0]&0x7f==0x7c:
                rec_flag = 0x555555
            elif res[0]&0x7f==0x3C:
                if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x000000):
                    return False
                if not self.tsingteng_flash_nvm_copy_and_check(flash_mid,0x000400,0x000000,0x000200):
                    return False
                if not self.tsingteng_flash_nvm_copy_and_check_flag(0x000500,0x000100,0x000300):
                    return False
                if not self.tsingteng_flash_check_13_14_value(0x000000,0x230000):
                    return False
                if not self.tsingteng_flash_write_flag_and_check(0xed0900,0xed0b00,0x400100):
                    return False
                bcontinue=True
            else:
                return False
        if rec_flag==0x55ffff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x000400,0x000203,bytes([0xc7,0xfd,0x00,0x00])):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xf7,0xfd,0x00,0x00])):
                    return False
            if not self.tsingteng_flash_nvm_copy_and_check_flag(0x000000,0x000400,0x000600):
                return False
            for _ in range(2):
                if not self.tsingteng_flash_reg_write(0x000203,bytes([0xf7,0xfd,0x00,0x00])):
                    return False
            if not self.tsingteng_flash_nvm_copy_and_check_flag(0x000100,0x000500,0x000700,True,0x79,0x000203,bytes([0xe5,0xfd,0x00,0x00])):
                return False
            if not self.tsingteng_flash_check_13_14_value(0x000400,0x230400):
                return False
            if not self.tsingteng_flash_write_flag_and_check(0xee0900,0xee0b00,0x400500):
                return False
            bcontinue=True
        
        if rec_flag==0x5555ff or bcontinue:
            if not self.tsingteng_flash_nvm_erase_and_check(flash_mid,0x000000,0x000203,bytes([0xc7,0xfd,0x00,0x00])):
                return False
            if not self.tsingteng_flash_write_flag_and_check(0xef0900,0xef0b00,0,0x000203,bytes([0xe5,0xfd,0x00,0x00])):
                return False
            bcontinue = True
        if rec_flag==0x555555 or bcontinue:
            if not self.tsingteng_flash_exit_factory_mode():
                return False
        return True

    def tsingteng_flash_scratch_solution(self, flash_mid:int):
        '''
        fix tsingteng flash bit reverse problem issue
        '''
        if flash_mid == 0x1560EB:
            if not self.tsingteng_flash_scratch_16HB():
                return False
        elif flash_mid == 0x1460CD:
            if not self.tsingteng_flash_scratch_80HB():
                return False
        elif flash_mid == 0x1360CD:
            if not self.tsingteng_flash_scratch_40HB():
                return False
            
        return True



    def tsingteng_flash_fix_mid_incorrect(self):
        '''
        change TH25Q40HB mid which is incorrect 0x1370CD to 0x1360CD 
        '''
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        if not self.tsingteng_flash_reg_write(0x0d2001,bytes([0x03])):
            return False
        cnt = 3
        while cnt>0:
            for _ in range(2):
                if not self.tsingteng_flash_nvm_write(0x010100,bytes([0xef])):
                    return False
            rbuf = self.tsingteng_flash_nvm_read(0x010100,1)
            if rbuf[0]==0x60:
                break
            cnt-=1
        if rbuf[0]!=0x60:
            return False
        if not self.tsingteng_flash_exit_factory_mode():
            return False
        
        return True
    
    def tsingteng_flash_read_mid_by_factory_mode(self):
        chac = None
        rsfdm_ctrl = self.BOOTROM_PROTOCOL.FlashReadSFDPProtocol()
        res, content = self.serial_instance.write_cmd_and_wait_response(rsfdm_ctrl.cmd(0x100000,1),rsfdm_ctrl.expect_length(1),0.8)
        if res and rsfdm_ctrl.response_check(content):
            chac = rsfdm_ctrl.get_read_val(content,1)
        else:
            return None
        
        if chac[0]!=0xEB and chac[0]!=0xCD:
            return None
        
        if not self.tsingteng_flash_enter_factory_mode():
            return None
        
        if not self.tsingteng_flash_reg_write(0x0d2001,bytes([0x03])):
            return None
        
        if not self.tsingteng_flash_reg_write(0x010001,bytes([0x02])):
            return None
        
        addr = 0x000900
        if chac[0]==0xCD:
            addr = 0x000100
        
        rbuf = self.tsingteng_flash_nvm_read(addr,3)
        mid = (rbuf[2]<<16)+(rbuf[1]<<8)+rbuf[0]
        if not self.tsingteng_flash_exit_factory_mode():
            return None
        return mid
    
    def tsingteng_flash_high_temperature_patch(self):
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        
        if not self.tsingteng_flash_reg_write(0x0d2001,bytes([0x03])):
            return None
        
        if not self.tsingteng_flash_reg_write(0x010001,bytes([0x02])):
            return None
        
        rbuf = self.tsingteng_flash_nvm_read(0xE30500,1)
        if rbuf[0]==0xff or rbuf[0]==0:
            return False
        if rbuf[0]>=0x16:
            self.tsingteng_flash_exit_factory_mode()
            return True
        cnt = 10
        while cnt>0:
            if not self.tsingteng_flash_nvm_write(0x070500,bytes([0x8f])):
                return False           
            time.sleep(0.001)
            rbuf = self.tsingteng_flash_nvm_read(0x070500,1)
            if rbuf[0]&0x70==0:
                break
            cnt-=1
        if rbuf[0]&0x70!=0:
            return False
        if not self.tsingteng_flash_exit_factory_mode():
            return False
        return True


    def tsingteng_flash_write_and_verify(self, start:int, oplen:int):
        '''
        start address: 0x1fd000
        length: 12*1024=0x3000 (12k)
        '''
        op_len = 0x3000
        if oplen<0x3000:
            op_len = oplen       
        loop = int(op_len/0x1000)
        for idx in range(loop):
            BKLog.i("erase sector @0x{:x} .".format(idx*0x1000+start))
            if not self.erase_custom_size(idx*0x1000+start,0x20):               
                BKLog.e("write sector @0x{:x} fail.".format(idx*0x1000+start))
                return False
        for idx in range(loop):
            BKLog.i("write sector @0x{:x} .".format(idx*0x1000+start))
            if not self.write_sector(idx*0x1000+start,g_hex_data[idx*0x1000:idx*0x1000+0x1000]):
                BKLog.i("write sector @0x{:x} fail.".format(idx*0x1000+start))               
                return False
            if not self.check_crc_ver2(g_hex_data[idx*0x1000:idx*0x1000+0x1000],idx*0x1000+start,0x1000):  
                BKLog.e("Check CRC32 @0x{:x}, ".format(idx*0x1000+start)+"len: 0x{:x} fail.".format(0x1000))
                return False      
        return True

    def tsingteng_flash_detect_bad_product(self, binlist):
        '''
        write and check 12k data, to detect bad flash
        '''
        
        if not self.tsingteng_flash_enter_factory_mode():
            return False
        if not self.tsingteng_flash_reg_write(0x000203,bytes([0xc7,0xfd,0x00,0x00])):
            return False
        if not self.tsingteng_flash_reg_write(0x000203,bytes([0xf3,0xfd,0x00,0x00])):
            return False
        if not self.tsingteng_flash_exit_factory_mode():
            return False
        time.sleep(0.005)
        if not self.unprotect_flash():
            return False
        for bin in binlist:
            if not self.tsingteng_flash_write_and_verify(bin.iStartAddress,bin.iFileLength):
                return False
        if not self.protect_flash():
            return False
        
        return True

