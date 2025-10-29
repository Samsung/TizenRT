import time
from datetime import datetime
import os
from .action_base import ActionBase
from ..chip_controller import BaseController
from ..chip_controller import get_chip_set_with_chip_id
from ..bk_log import BKLog
from ..common.common import ReadInfo, parse_readinfo_otp, parse_writeinfo_otp

class Dump(ActionBase):

    def __init__(self, port='/dev/ttyUSB0', baudrate=115200, link_check_type=None, reset_type=None, reset_baudrate=115200, getbus_times=100, reboot_after_donwload=True,recnt=5,chipid=None,debug='3',link_key=None) -> None:
        super().__init__(port,baudrate)       
        self.link_check_type = link_check_type
        self.reset_type = reset_type
        self.reset_baudrate = reset_baudrate
        self.reboot_after_donwload = reboot_after_donwload
        self.getbus_times = getbus_times
        self.retry = recnt
        self.chip_id = chipid
        self.debug = debug
        self.link_key = link_key

    def read_task(self,readinfo:ReadInfo):
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        self._init_log_level(self.ser.ser.name,self.debug)       
        try:
            # get bus
            tmp_res,linktype = base_controller.get_bus(reset_type=self.reset_type, linkcheck_obj=self.link_check_type, reset_baudrate=self.reset_baudrate,max_try_count=self.getbus_times,link_key=self.link_key)
            if not tmp_res:
                raise Exception("get bus fail.")
            # get chip id
            time.sleep(0.1) #fix read chip id fail sometimes
            sec_id = 0
            if self.chip_id is None:           
                self.chip_id = base_controller.get_chip_id_retry(self.retry)
                if self.chip_id is None:
                    raise Exception("get chip id fail.")
                if self.chip_id==0x7236 or self.chip_id >> 16==0x7236:
                    sec_id = base_controller.get_second_chip_id()
                    if sec_id is None:
                        raise Exception("get sec chip id fail.")                 
            BKLog.w("chip id is 0x{:x}".format(self.chip_id))     
            # init chip_set class
            chip_set = get_chip_set_with_chip_id(self.chip_id,sec_id)
            if chip_set is None:
                raise Exception("get chip set fail.")
            # update serial
            chip_set.update_serial_instance(self.ser)
            # parse flash
            BKLog.i("parse flash info...") 
            if not chip_set.parse_flash_info():
                raise Exception("parse flash info fail.")
            BKLog.i("set baudrate to {0}...".format(readinfo.work_baudrate)) 
            if not chip_set.set_baudrate(baudrate=readinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(readinfo.work_baudrate))

            if len(readinfo.vecbinInfo)>0:
                #dump file path               
                current_time = datetime.now()
                fmt_time = current_time.strftime('%Y%m%d%H%M%S')
                if readinfo.bAllFlash:
                    readinfo.vecbinInfo[0].iFileLength = int(chip_set._flash_cfg.flash_size/8)                   
                for rinfo in readinfo.vecbinInfo:
                    #add start address and length
                    rinfo.sFilePath = rinfo.sFilePath.replace(".bin",f"_0x{rinfo.iStartAddress:x}-0x{rinfo.iFileLength:x}.bin")
                    #add format time information
                    rinfo.sFilePath = rinfo.sFilePath.replace(".bin",f"_{fmt_time}.bin")
                    #add pre path
                    if readinfo.read_path is not None:
                        rinfo.sFilePath = readinfo.read_path + os.path.sep + rinfo.sFilePath
                #read whole flash    
                BKLog.i("start read flash ...")   
                if readinfo.bAllFlash:                   
                    flash_size = int(chip_set._flash_cfg.flash_size/8)
                    BKLog.i(f"read all flash path: {readinfo.vecbinInfo[0].sFilePath}")  
                    BKLog.i(f"start: 0x{readinfo.vecbinInfo[0].iStartAddress:x}") 
                    BKLog.i(f"length: 0x{flash_size:x}") 
                    if not chip_set.dump_flash(readinfo.vecbinInfo[0].iStartAddress,flash_size,readinfo.vecbinInfo[0].sFilePath,self.retry,readinfo.bConsole):
                        raise Exception("Read flash fail.")
                        
                else:
                    read_idx = 0
                    for binfile in readinfo.vecbinInfo:
                        BKLog.i(f"[{read_idx}] read flash path:{binfile.sFilePath}")  
                        BKLog.i(f"[{read_idx}] start: 0x{binfile.iStartAddress:x}") 
                        BKLog.i(f"[{read_idx}] length: 0x{binfile.iFileLength:x}")            
                        if not chip_set.dump_flash(binfile.iStartAddress,binfile.iFileLength,binfile.sFilePath,self.retry,readinfo.bConsole):
                            raise Exception("Read flash fail.")
                        read_idx+=1
                BKLog.i("Read flash done.") 
            # read uid
            if readinfo.read_uid:
                BKLog.i("start read chip UID ...")   
                ret,uid = chip_set.read_chip_uid()
                if not ret:
                    raise Exception("Read UID fail.")
                    
                bytesuid = bytes(uid[:5])
                BKLog.w("Read UID done.")
                BKLog.w("UID is: "+ bytes.hex(bytesuid))
                        
            # read efuse
            if readinfo.read_efuse:
                #parse read info
                efuseinfo = parse_readinfo_otp(readinfo.read_efuse)                 
                if efuseinfo is None:
                    raise Exception("Parse efuse read info fail.")
                for info in efuseinfo:
                    BKLog.i("start read efuse @0x{:x}, ".format(info[0])+ "length: {0}".format(info[1]))  
                    efuse,msg = chip_set.read_efuse(info[0], info[1])
                    if efuse is None:
                        BKLog.e(msg)
                        raise Exception("Read efuse fail.")
                    
                    BKLog.w("Read efuse done.")                         
                    BKLog.w("efuse stream: " + bytes.hex(bytes(efuse)))
                

            # read otp        
            if readinfo.read_otp:               
                #parse read info
                otpinfo = parse_readinfo_otp(readinfo.read_otp)                 
                if otpinfo is None:
                    raise Exception("Parse OTP read info fail.")
                
                for info in otpinfo:
                    BKLog.i("start read OTP @0x{:x}, ".format(info[0])+ "length: {0}".format(info[1]))  
                    otp,msg = chip_set.read_chip_otp(info[0], info[1],readinfo.reverse_content)
                    if otp is None:
                        BKLog.e(msg)
                        raise Exception("Read OTP fail.")
                    
                    BKLog.w("Read OTP done.")                    
                    bytesotp = bytes(otp)        
                    BKLog.w("OTP stream: " + bytes.hex(bytesotp))
            
            # read otp/efuse/uid       
            if readinfo.read_otp_efuse_uid:
                #efuse
                BKLog.i("start read efuse all ...")   
                ret,efuse = chip_set.read_efuse(0,0)
                if not ret:
                    raise Exception("Read efuse fail.")      
                   
                BKLog.w("Read efuse done.")
                cnt = len(efuse)
                for i in range(cnt):
                    BKLog.w(f"address: 0x{i:x}, read = 0x{efuse[i]:x}")
                
                #uid
                BKLog.i("start read chip UID ...")   
                ret,uid = chip_set.read_chip_uid()
                if not ret:
                    raise Exception("Read UID fail.")
                    
                bytesuid = bytes(uid[:5])
                BKLog.w("Read UID done.")
                BKLog.w("UID is: "+ bytes.hex(bytesuid))

                #otp           
                #parse read info
                otpinfo = parse_readinfo_otp(readinfo.read_otp_efuse_uid)                 
                if otpinfo is None:
                    raise Exception("Parse OTP write info fail.")
                
                for info in otpinfo:
                    BKLog.i("start read OTP @0x{:x}, ".format(info[0])+ "length: {0}".format(info[1]))  
                    otp,msg = chip_set.read_chip_otp(info[0], info[1],readinfo.reverse_content)
                    if otp is None:
                        BKLog.e(msg)
                        raise Exception("Read OTP fail.")
                    
                    BKLog.w("Read OTP done.")                    
                    bytesotp = bytes(otp)        
                    BKLog.w("OTP stream: " + bytes.hex(bytesotp))

            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            if self.reboot_after_donwload:
                BKLog.i("do reboot from bootrom.")
                base_controller.do_reboot()
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("Test complete, all pass.")
                BKLog.w("done.")
            else:
                BKLog.e("Test fail.")
            BKLog.i(f"Elapse time: {elapsed_time:.6f} seconds.\n")
            return result
            
    
        
            
    
        
        



