import time
from serial import Timeout
from ...bk_log import BKLog
from ...common.common import user_operate, bytes_reverse, BIT, BFD, bytes_reverse_per4bytes
from ...common.encrypt_helper import decrypt_random, encrypt_random
from ..reg_controller import RegController


class BK7236Q_Security(RegController):
    def __init__(self, serial_instance):
        super().__init__(serial_instance)
        self.ser = serial_instance
        self.security_efuse_secure_ctrl = 0x4b108010
        self.security_efuse_op_base = 0x4b100008
        self.security_efuse_vddpos = 0x3
        self.security_efuse_margin_rd = 0x5
        self.security_efuse_start_addr = 0x4b108018
        self.security_efuse_end_addr = 0x4b1080FF#byte address
        self.security_efuse_length = 0x40
        
        

    def _set_efuse_write_enable(self,enable:bool):
        
        if enable:
            tmp=1
        else:
            tmp=0
        if not self._set_reg_bit_val(self.security_efuse_op_base,self.security_efuse_vddpos,1,tmp):
            return False
        return True
    
    def _set_efuse_margin_read(self,enable:bool):
        
        if enable:
            tmp=1
        else:
            tmp=0
        if not self._set_reg_bit_val(self.security_efuse_op_base,self.security_efuse_margin_rd,1,tmp):
            return False
        return True

    def _read_efuse_by_regaddr(self,reg_addr,length,big_endian=False):
        '''
        read bytes data from efuse
        'reg_addr' is the efuse reg address
        '''
        
        cnt = length
        idx = 0
        value = bytearray()
        while cnt>0:
            res = self.read_reg(reg_addr+4*idx)
            if res is None:
                return False,None
            if big_endian:
                value.append((res>>24)&0xff)
                value.append((res>>16)&0xff)
                value.append((res>>8)&0xff)      
                value.append(res&0xff)              
            else:                                   
                value.append(res&0xff)
                value.append((res>>8)&0xff)
                value.append((res>>16)&0xff)
                value.append((res>>24)&0xff)            
            cnt -= 4
            idx += 1
        
        return True, value
        
  
    
    def _write_efuse_by_regaddr(self,reg_addr,value,big_endian=False):
        '''
        write bytes data to efuse
        'reg_addr' is the efuse reg address
        '''

        cnt = len(value)
        idx = 0
        while cnt>0:
            if big_endian:               
                wrv = (value[4*idx+3])+(value[4*idx+2]<<8)+(value[4*idx+1]<<16)+(value[4*idx]<<24)
            else:
                wrv = (value[4*idx])+(value[4*idx+1]<<8)+(value[4*idx+2]<<16)+(value[4*idx+3]<<24)
            if not self.write_reg(reg_addr+4*idx, wrv):
                BKLog.e(f"Write reg fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            res = self.read_reg(reg_addr+4*idx)
            if res is None:
                BKLog.e(f"Read reg fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            if res!=wrv:
                BKLog.e(f"Compare reg read and write fail, address: 0x{reg_addr+4*idx:x}.")
                return False
            cnt -= 4
            idx += 1

        return True
    
    def _write_efuse(self,efuse_addr,val,reverse=False,big_endian=False):
        ss = bytes.fromhex(val)
        if reverse:
            ss = bytes_reverse_per4bytes(ss)
        length = len(ss)
        msg = None
        if efuse_addr<=self.security_efuse_end_addr and efuse_addr>=self.security_efuse_start_addr:
            if efuse_addr+length>self.security_efuse_end_addr:
                msg = "Error, efuse address(0x{:x}) + length(0x{:x}) over the efuse end.".format(efuse_addr,length)
                return False,msg
        else:
            msg = "Error, invalid efuse address: 0x{:x}.".format(efuse_addr)
            return False,msg
        
        content,msg = self._read_efuse(efuse_addr,len(ss))
        if content is None:
            return False,msg
        if content==ss:
            BKLog.w("Write efuse ignore, already written same data.")           
            return True,None
        else:
            for idx in range(length):               
                if content[idx]!=0:
                    msg = "Write efuse ignore, already written different data."
                    return False,msg
        if not self._set_efuse_write_enable(enable=True):
            return False,'_set_efuse_write_enable fail'
        if not self._write_efuse_by_regaddr(efuse_addr,ss,big_endian):
            return False,'_write_efuse_by_regaddr fail'
        if not self._set_efuse_write_enable(enable=False):
            return False,'_set_efuse_write_enable fail'
        return True,None
    
    def _read_efuse(self,efuse_addr,oplen,big_endian=False):
        if efuse_addr<=self.security_efuse_end_addr and efuse_addr>=self.security_efuse_start_addr:
            if efuse_addr+oplen>self.security_efuse_end_addr:
                msg = "Error, efuse address(0x{:x}) + length(0x{:x}) over the efuse end.".format(efuse_addr,oplen)
                return False,msg
        else:
            msg = "Error, invalid efuse address: 0x{:x}.".format(efuse_addr)
            return False,msg
        if not self._set_efuse_margin_read(enable=True):
            return None,'_set_efuse_margin_read fail'
        ret,content = self._read_efuse_by_regaddr(efuse_addr,oplen,big_endian)
        if not ret:
            return None,'_read_efuse_by_regaddr fail'
        if not self._set_efuse_margin_read(enable=False):
            return None,'_set_efuse_margin_read fail'
        return content, None
    
    def _read_efuse_security_ctrl(self,efuse_idx):
        if not self._set_efuse_margin_read(enable=True):
            return None,None,'_set_efuse_margin_read fail'
        if efuse_idx>=4:
            ret,content = self._read_efuse_by_regaddr(self.security_efuse_secure_ctrl+1*4,1,False)
        else:
            ret,content = self._read_efuse_by_regaddr(self.security_efuse_secure_ctrl,1,False)
        if not ret:
            return None,None,'_read_efuse_by_regaddr fail'
        if not self._set_efuse_margin_read(enable=False):
            return None,None,'_set_efuse_margin_read fail'
        if efuse_idx>=4:
            return content,content[efuse_idx-4],None
        else:
            return content,content[efuse_idx],None
        
    
    def _write_efuse_security_ctrl(self,efuse_idx,val):
        '''
        efuse_idx: efuse byte address
        val: byte value
        '''
        fullv,rv,msg = self._read_efuse_security_ctrl(efuse_idx)
        if rv is None:
            return False,'_read_efuse_security_ctrl fail'
        BKLog.i(f"Read efuse original value: 0x{rv:x}, address: {efuse_idx}.")
        for bit in range(8):
            #1->0, return fail
            if val&BIT(bit)==0 and rv&BIT(bit)==BIT(bit):
                BKLog.i(f"address: {efuse_idx}, bit {bit}.")
                #BKLog.e("Write efuse ignore, already written to 1.")   
                msg = BKLog.e("Write efuse ignore, already written to 1.")            
                return False,msg
        if rv== val:
            BKLog.i(f"address: {efuse_idx}, bit {bit}.")
            BKLog.w("Write efuse ignore, already written same data.")
            return True,None
        if efuse_idx>=4:           
            fullv[efuse_idx-4] = val
        else:         
            fullv[efuse_idx] = val
        if not self._set_efuse_write_enable(enable=True):
            return False,'_set_efuse_write_enable fail'
        if efuse_idx>=4:
            if not self._write_efuse_by_regaddr(self.security_efuse_secure_ctrl+1*4,fullv,False):
                return False,'_write_efuse_by_regaddr fail'
        else:
            if not self._write_efuse_by_regaddr(self.security_efuse_secure_ctrl,fullv,False):
                return False,'_write_efuse_by_regaddr fail'
        if not self._set_efuse_write_enable(enable=False):
            return False,'_set_efuse_write_enable fail'
        return True,None


    def _write_efuse_security_ctrl_bit(self,efuse_idx,bit,val):
        fullv,rv,msg = self._read_efuse_security_ctrl(efuse_idx)
        if rv is None:
            return False,'_read_efuse_security_ctrl fail'
        if efuse_idx>=4:
            rv = fullv[efuse_idx-4]&BIT(bit)
        else:
            rv = fullv[efuse_idx]&BIT(bit)
        BKLog.i(f"Read efuse original value: 0x{rv:x}, address: {efuse_idx}, bit {bit}.")
        if rv==BIT(bit):
            if val==0:
                BKLog.i(f"address: {efuse_idx}, bit {bit}.")
                msg = "Write efuse ignore, already written to 1."
                return False,msg
            else:
                BKLog.i(f"address: {efuse_idx}, bit {bit}.")
                BKLog.e("Write efuse ignore, already written same data.")
                return True,None
        else:
            if val==0:
                BKLog.i(f"address: {efuse_idx}, bit {bit}.")
                BKLog.e("Write efuse ignore, dont need write.")
                return True,None

        if efuse_idx>=4:
            tmp = fullv[efuse_idx-4]|BIT(bit)
            fullv[efuse_idx-4] = tmp
        else:
            tmp = fullv[efuse_idx]|BIT(bit)
            fullv[efuse_idx] = tmp
        if not self._set_efuse_write_enable(enable=True):
            return False,'_set_efuse_write_enable fail'
        if efuse_idx>=4:
            if not self._write_efuse_by_regaddr(self.security_efuse_secure_ctrl+1*4,fullv,False):
                return False,'_write_efuse_by_regaddr fail'
        else:
            if not self._write_efuse_by_regaddr(self.security_efuse_secure_ctrl,fullv,False):
                return False,'_write_efuse_by_regaddr fail'
        if not self._set_efuse_write_enable(enable=False):
            return False,'_set_efuse_write_enable fail'
        return True,None
    
    def _read_efuse_info(self,usr_info:user_operate):
        '''
        read efuse by the 'user_operate' information
        '''  
        address = int(usr_info.start_addr,16)
        length = int(usr_info.byte_len,16)
        ret,msg = self._read_efuse(address,length,usr_info.big_endian)

        if ret is not None:
            ss = ret
            if usr_info.reverse:
                ss = bytes_reverse_per4bytes(ret)
            sstr = bytes.hex(bytes(ss))
            if usr_info.data_type=="ascii":
                sstr = encrypt_random(sstr)
            return sstr,msg
        else:
            return None,msg


    def _write_efuse_info(self,usr_info:user_operate):
        '''
        write otp by the 'user_operate' information
        '''
        ss = usr_info.data
        if usr_info.data_type=="hex":
            ss = usr_info.data
        elif usr_info.data_type=="ascii":
            ss = decrypt_random(usr_info.data)
        else:
            msg = "Error, data type not support: {0}.".format(usr_info.data_type)
            return False,msg
        msg = None
        address = int(usr_info.start_addr,16)
        length = int(usr_info.byte_len,16)
        if length!=len(ss)/2:
            msg = "Error, data length(0x{:x}) is not 0x{:x}.".format(len(usr_info.data),length)
            return False,msg       
        
        return self._write_efuse(address,ss,usr_info.reverse,usr_info.big_endian)