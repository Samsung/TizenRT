import time
from typing import List
from .action_base import ActionBase
from ..chip_controller import BaseController
from ..chip_controller import FlashControllerTsingteng
from ..chip_controller import get_chip_set_with_chip_id
from ..chip_controller.chip_set.base_chip import BaseChip
from ..bk_log.log_utils import *
from ..common.common import FLASH_SIZE_DEF,RESET_TYPE,LINKCHECK, BinInfo, DownloadInfo, parse_safe_json_file, parse_writeinfo_otp, parse_writeinfo_efuse, bytes_reverse
from ..common.version import get_tool_version

class Download(ActionBase):

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

    
    def download_base_task(self, downloadinfo: DownloadInfo):
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        self._init_log_level(self.ser.ser.name,self.debug)
        try:
        # get bus
            tmp_res,linktype = base_controller.get_bus(reset_type=self.reset_type, linkcheck_obj=self.link_check_type,reset_baudrate=self.reset_baudrate, max_try_count=self.getbus_times,link_key=self.link_key)
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
            #check image file whether with crc32 per 32 bytes, BK7259 need remove it
            
            chip_set.pre_check_image_file(downloadinfo.vecbinInfo)
            # update serial
            chip_set.update_serial_instance(self.ser)

            if not chip_set.pre_set_efuse_nocrc():
                raise Exception("pre set efuse fail.")
            # parse flash
            BKLog.i("parse flash info...")
            if not chip_set.parse_flash_info():
                raise Exception("parse flash info fail.")
            BKLog.i("set baudrate to {0}...".format(downloadinfo.work_baudrate)) 
            th_flash_ctrl = FlashControllerTsingteng(serial_instance=self.ser,flash_cfg=chip_set._flash_cfg)

            if not chip_set.set_baudrate(baudrate=downloadinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(downloadinfo.work_baudrate))
            
            if chip_set._flash_mid==0x1370CD:
                if not th_flash_ctrl.tsingteng_flash_fix_mid_incorrect():
                    raise Exception("execute fix flash mid patch fail.")
                chip_set._flash_mid = 0x1360CD    
            if chip_set._flash_mid == 0x1560EB:               
                if not th_flash_ctrl.tsingteng_flash_detect_bad_product(downloadinfo.vecbinInfo):
                    raise Exception("execute detect bad flash patch fail.")
                if not th_flash_ctrl.tsingteng_flash_high_temperature_patch():
                    raise Exception("execute high temperature patch fail.")

            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                #unprotect flash
                BKLog.i("unprotect flash...") 
                if not chip_set.un_protect_flash(protect=False):
                    raise Exception("unprotect flash fail.")
                BKLog.i("unprotect flash OK.")

            if downloadinfo.bDownloadBin:
                #erase all flash if commandline contains '--erase-all'
                if downloadinfo.pre_erase==1:
                    if downloadinfo.bAllFlash:
                        BKLog.i("erase whole flash.")
                        if not chip_set.erase_flash(True, None, None, False, self.retry):                      
                            raise Exception("erase whole flash fail.")
                        BKLog.i("erase whole flash OK.")
                    else:
                        erase_idx = 0
                        for binfile in downloadinfo.vecbinInfo:
                            BKLog.i(f"[{erase_idx}] erase flash.")
                            BKLog.i(f"[{erase_idx}] start: 0x{binfile.iStartAddress:x}.")
                            BKLog.i(f"[{erase_idx}] length: 0x{binfile.iFileLength:x}.")
                            #erase flash by bin start address and size if commandline donot contain '--erase-all'
                            if not chip_set.erase_flash(False,binfile.iStartAddress,binfile.iStartAddress+binfile.iFileLength,False,self.retry):
                                raise Exception("erase flash fail.")
                            erase_idx+=1
                #write each bin to flash 
                write_idx = 0
                for binfile in downloadinfo.vecbinInfo:   
                    if binfile.iStartAddress & 0xfff:               
                        binfile.iStartAddress = int((binfile.iStartAddress+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
                    BKLog.i(f"[{write_idx}] write flash.")
                    BKLog.i(f"[{write_idx}] start: 0x{binfile.iStartAddress:x}.")
                    BKLog.i(f"[{write_idx}] length: 0x{binfile.iFileLength:x}.")
                    #write flash by bin start size and size    
                    if not chip_set.download_flash(binfile.pfile, binfile.iStartAddress, self.retry, self.reset_type==RESET_TYPE.DTR_RTS,self.reset_type, self.link_check_type):
                        raise Exception("Write flash fail.")
                    write_idx+=1
            BKLog.w("Write flash done.")
            
            #write cfg information to flash
            if downloadinfo.bDownloadDeviceName:
                BKLog.i(f"write device name: {bytes.decode(downloadinfo.device_name,'utf-8')}@0x{downloadinfo.device_name_Addr:x}.")
                if not chip_set.write_flash_info(downloadinfo.device_name_Addr,downloadinfo.device_name):
                    raise Exception("Write device name to flash fail.")
                BKLog.w("Write device name to flash done.")
                
            if downloadinfo.bDownloadDeviceID:
                BKLog.i(f"write device ID: {bytes.hex(downloadinfo.device_id)}@0x{downloadinfo.device_id_Addr:x}.")
                devid = downloadinfo.device_id
                if not downloadinfo.bEnableBigEndian:
                    devid = bytes_reverse(downloadinfo.device_id)
                if not chip_set.write_flash_info(downloadinfo.device_id_Addr,devid):
                    raise Exception("Write device ID to flash fail.")
                BKLog.w("Write device ID to flash done.")
                
            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                BKLog.i("protect flash...") 
                if not chip_set.un_protect_flash(protect=True):
                    raise Exception("protect flash fail.")
                BKLog.i("protect flash OK.")

            #write efuse and otp ...
            if downloadinfo.safeinfo.write_safe_config:
                BKLog.i("write safe json content.")                
                #set aes key with commandline input
                if downloadinfo.aes_key:
                    for sedata in downloadinfo.safeinfo.security_data_list:
                        if sedata.name.find('aes_key')!=-1:
                            sedata.data = downloadinfo.aes_key
                #BKLog.i("parse safe json ok.") 
                ret,msg = chip_set.write_safe_json(downloadinfo.safeinfo)
                if not ret:
                    BKLog.e(msg)
                    raise Exception("Write safe json fail.")
                BKLog.w("write safe json done.")

            if downloadinfo.efuse_info:
                BKLog.i("write chip efuse.")
                infolist = parse_writeinfo_efuse(downloadinfo.efuse_info)
                if infolist is None:
                    raise Exception("parse efuse info fail.")
                for info in infolist:
                    if len(info)==3:
                        ret,msg = chip_set.write_efuse_by_bit(info[1],info[2],info[0])
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                    elif len(info)==2: 
                        ret,msg = chip_set.write_efuse(info[0],info[1])     
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                BKLog.w("Write efuse done.")

            if downloadinfo.otp_info:
                BKLog.i("write chip OTP.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_info)
                if infolist is None:
                    raise Exception("parse OTP info fail.")
                for info in infolist:
                    ret,msg = chip_set.write_chip_otp(info[1], info[0],downloadinfo.reverse_content)                                 
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Write OTP fail.")
                BKLog.w("Write OTP done.")

            if downloadinfo.otp_pm:
                BKLog.i("set OTP permission.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_pm)
                if infolist is None:
                    raise Exception("parse OTP permission info fail.")
                for info in infolist:
                    ret,msg = chip_set.set_otp_permission_oplen(info[1], info[2], info[0])                               
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Set OTP permission fail.")
                BKLog.w("Set OTP permission done.")
            
            if chip_set._flash_mid==0x1360CD or chip_set._flash_mid==0x1460CD or chip_set._flash_mid==0x1560EB:
                if not th_flash_ctrl.tsingteng_flash_scratch_solution(chip_set._flash_mid):
                    raise Exception("Execute flash scratch solution fail.")

            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            #write tool version to flash OTP
            # if not chip_set.write_tool_version(get_tool_version()):
            #     BKLog.e("write tool version to flash OTP fail.")
                
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

    def download_with_outter_flash_task(self, downloadinfo: DownloadInfo):
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        self._init_log_level(self.ser.ser.name,self.debug)
        try:
        # get bus
            tmp_res,linktype = base_controller.get_bus(reset_type=self.reset_type, linkcheck_obj=self.link_check_type,reset_baudrate=self.reset_baudrate, max_try_count=self.getbus_times,link_key=self.link_key)
            if not tmp_res:
                raise Exception("get bus fail.")
            # get chip id
            time.sleep(0.1) #fix read chip id fail sometimes
            if self.chip_id is None:                                     
                self.chip_id = base_controller.get_chip_id_retry(self.retry)
                if self.chip_id is None:
                    raise Exception("get chip id fail.")               
            BKLog.w("chip id is 0x{:x}".format(self.chip_id))    
            # init chip_set class
            chip_set = get_chip_set_with_chip_id(self.chip_id)
            if chip_set is None:
                raise Exception("get chip set fail.")
            #check image file whether with crc32 per 32 bytes, BK7259 need remove it
            
            chip_set.pre_check_image_file(downloadinfo.vecbinInfo)
            # update serial
            chip_set.update_serial_instance(self.ser)

            if not chip_set.pre_set_efuse_nocrc():
                raise Exception("pre set efuse fail.")
            # parse flash
            BKLog.i("parse flash info...")
            if not chip_set.parse_flash_info():
                raise Exception("parse flash info fail.")
            
            ret = chip_set.read_and_check_sector(0x7ff000)
            if ret is None:
                raise Exception(f"read flash @0x7ff000 fail.")
            BKLog.w(f"Device MAC:{bytes.hex(bytes(ret[:6]))}")

            BKLog.i("set baudrate to {0}...".format(downloadinfo.work_baudrate)) 
            if not chip_set.set_baudrate(baudrate=downloadinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(downloadinfo.work_baudrate))
            
            

            th_flash_ctrl = FlashControllerTsingteng(serial_instance=self.ser,flash_cfg=chip_set._flash_cfg)
            if chip_set._flash_mid==0x1370CD:
                if not th_flash_ctrl.tsingteng_flash_fix_mid_incorrect():
                    raise Exception("execute fix flash mid patch fail.")
                chip_set._flash_mid = 0x1360CD    
            if chip_set._flash_mid == 0x1560EB:               
                if not th_flash_ctrl.tsingteng_flash_detect_bad_product(downloadinfo.vecbinInfo):
                    raise Exception("execute detect bad flash patch fail.")
                if not th_flash_ctrl.tsingteng_flash_high_temperature_patch():
                    raise Exception("execute high temperature patch fail.")
            
            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                #unprotect flash
                BKLog.i("unprotect flash...") 
                if not chip_set.un_protect_flash(protect=False):
                    raise Exception("unprotect flash fail.")
                BKLog.i("unprotect flash OK.")

            if downloadinfo.bDownloadBin:
                if downloadinfo.pre_erase==1:
                    #erase all flash if commandline contains '--erase-all'
                    if downloadinfo.bAllFlash:
                        BKLog.i("erase whole flash.")
                        if not chip_set.erase_flash(True, None, None, False, self.retry):                      
                            raise Exception("erase whole flash fail.")
                        BKLog.i("erase whole flash OK.")
                    else:
                        erase_idx = 0
                        for binfile in downloadinfo.vecbinInfo:
                            BKLog.w(f"[{erase_idx}] erase flash.")
                            BKLog.i(f"[{erase_idx}] start: 0x{binfile.iStartAddress:x}.")
                            BKLog.i(f"[{erase_idx}] length: 0x{binfile.iFileLength:x}.")
                            #erase flash by bin start address and size if commandline donot contain '--erase-all'
                            if not chip_set.erase_flash(False,binfile.iStartAddress,binfile.iStartAddress+binfile.iFileLength,False,self.retry):
                                raise Exception("erase flash fail.")
                            erase_idx+=1
                            #last bin need write to outter flash
                            if erase_idx== len(downloadinfo.vecbinInfo)-1:
                                break
                #write each bin to flash 
                time.sleep(0.001)
                write_idx = 0
                for binfile in downloadinfo.vecbinInfo:   
                    if binfile.iStartAddress & 0xfff:               
                        binfile.iStartAddress = int((binfile.iStartAddress+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
                    BKLog.w(f"[{write_idx}] write flash.")
                    BKLog.i(f"[{write_idx}] start: 0x{binfile.iStartAddress:x}.")
                    BKLog.i(f"[{write_idx}] length: 0x{binfile.iFileLength:x}.")
                    #write flash by bin start size and size    
                    if not chip_set.download_flash(binfile.pfile, binfile.iStartAddress, self.retry, self.reset_type==RESET_TYPE.DTR_RTS,self.reset_type, self.link_check_type):
                        raise Exception("Write flash fail.")
                    write_idx+=1
                    #last bin need write to outter flash
                    if write_idx== len(downloadinfo.vecbinInfo)-1:
                        break
            BKLog.w("Write flash done.")
            #write cfg information to flash
            if downloadinfo.bDownloadDeviceName:
                BKLog.i(f"write device name: {bytes.decode(downloadinfo.device_name,'utf-8')} @0x{downloadinfo.device_name_Addr:x}.")
                if not chip_set.write_flash_info(downloadinfo.device_name_Addr,downloadinfo.device_name):
                    raise Exception("Write device name to flash fail.")
                BKLog.w("Write device name to flash done.")
                
            if downloadinfo.bDownloadDeviceID:
                BKLog.i(f"write device ID: {bytes.hex(downloadinfo.device_id)} @0x{downloadinfo.device_id_Addr:x}.")
                devid = downloadinfo.device_id
                if not downloadinfo.bEnableBigEndian:
                    devid = bytes_reverse(downloadinfo.device_id)
                if not chip_set.write_flash_info(downloadinfo.device_id_Addr,devid):
                    raise Exception("Write device ID to flash fail.")
                BKLog.w("Write device ID to flash done.")
                
            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                BKLog.i("protect flash...") 
                if not chip_set.un_protect_flash(protect=True):
                    raise Exception("protect flash fail.")
                BKLog.i("protect flash OK.")

            #write efuse and otp ...
            if downloadinfo.safeinfo.write_safe_config:
                BKLog.i("write safe json content.")
                #set aes key with commandline input
                if downloadinfo.aes_key:
                    for sedata in downloadinfo.safeinfo.security_data_list:
                        if sedata.name.find('aes_key')!=-1:
                            sedata.data = downloadinfo.aes_key
                #BKLog.i("parse safe json ok.") 
                ret,msg = chip_set.write_safe_json(downloadinfo.safeinfo)
                if not ret:
                    BKLog.e(msg)
                    raise Exception("Write safe json fail.")
                BKLog.w("write safe json done.")

            if downloadinfo.efuse_info:
                BKLog.i("write chip efuse.")
                infolist = parse_writeinfo_efuse(downloadinfo.efuse_info)
                if infolist is None:
                    raise Exception("parse efuse info fail.")
                for info in infolist:
                    if len(info)==3:
                        ret,msg = chip_set.write_efuse_by_bit(info[1],info[2],info[0])
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                    elif len(info)==2: 
                        ret,msg = chip_set.write_efuse(info[0],info[1])     
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                BKLog.w("Write efuse done.")

            if downloadinfo.otp_info:
                BKLog.i("write chip OTP.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_info)
                if infolist is None:
                    raise Exception("parse OTP info fail.")
                for info in infolist:
                    ret,msg = chip_set.write_chip_otp(info[1], info[0],downloadinfo.reverse_content)                                 
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Write OTP fail.")
                BKLog.w("Write OTP done.")

            if downloadinfo.otp_pm:
                BKLog.i("set OTP permission.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_pm)
                if infolist is None:
                    raise Exception("parse OTP permission info fail.")
                for info in infolist:
                    ret,msg = chip_set.set_otp_permission_oplen(info[1], info[2], info[0])                               
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Set OTP permission fail.")
                BKLog.w("Set OTP permission done.")

            if chip_set._flash_mid==0x1360CD or chip_set._flash_mid==0x1460CD or chip_set._flash_mid==0x1560EB:
                if not th_flash_ctrl.tsingteng_flash_scratch_solution(chip_set._flash_mid):
                    raise Exception("Execute flash scratch solution fail.")
            #reboot and enter outer flash mode
            time.sleep(0.001)
            BKLog.progress_log('BK',"Writing Flash", int(3))
            BKLog.w("OUTTER FLASH OPERATION")
            BKLog.i("enter write outter flash mode...")         
            if not chip_set.reboot_and_enter_write_outter_flash_mode(self.reset_type):
                raise Exception("enter write outter flash mode fail.")
            # parse flash
            time.sleep(0.2)
            BKLog.i("parse outter flash info...")
            chip_set.parse_flash_info()

            BKLog.i("set baudrate to {0}...".format(downloadinfo.work_baudrate))            
            if not chip_set.set_baudrate(baudrate=downloadinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(downloadinfo.work_baudrate))
            
            if downloadinfo.bDownloadBin:
                #erase all flash if commandline contains '--erase-all'
                if downloadinfo.bAllFlash:
                    BKLog.i("erase whole outter flash.")
                    if not chip_set.erase_flash(True, None, None, False, self.retry):                      
                        raise Exception("erase whole outter flash fail.")
                    BKLog.i("erase whole outter flash OK.")
                else:                   
                    binfile = downloadinfo.vecbinInfo[len(downloadinfo.vecbinInfo)-1]
                    BKLog.w(f"[outter] erase flash.")
                    BKLog.i(f"[outter] start: 0x{binfile.iStartAddress:x}.")
                    BKLog.i(f"[outter] length: 0x{binfile.iFileLength:x}.")
                    #erase flash by bin start address and size if commandline donot contain '--erase-all'
                    if not chip_set.erase_flash(False,binfile.iStartAddress,binfile.iStartAddress+binfile.iFileLength,False,self.retry):
                        raise Exception("erase outter flash fail.")
                time.sleep(0.001)       
                #write each bin to flash                
                binfile = downloadinfo.vecbinInfo[len(downloadinfo.vecbinInfo)-1]                    
                BKLog.w(f"[outter] write flash.")
                BKLog.i(f"[outter] start: 0x{binfile.iStartAddress:x}.")
                BKLog.i(f"[outter] length: 0x{binfile.iFileLength:x}.")
                #write flash by bin start size and size    
                if not chip_set.download_flash(binfile.pfile, binfile.iStartAddress, self.retry, False, self.reset_type, self.link_check_type):
                    raise Exception("Write outter flash fail.")                   
            BKLog.w("Write outter flash done.")

            time.sleep(0.001)              
            if downloadinfo.custom_data:
                ret,bydata,addr,oplen = parse_device_mac_info(downloadinfo.custom_data)
                if not ret:
                    raise Exception("Input custom data format error.")
                self.ser.reset(115200)
                if not base_controller.reconnect_bus(reset_type=self.reset_type,linkcheck_obj=self.link_check_type,reset_baudrate=115200,max_retry_count=60,link_key=self.link_key):
                    raise Exception("reconnect bus fail.")
                BKLog.i("unprotect flash...") 
                if not chip_set.un_protect_flash(protect=False):
                    raise Exception("unprotect flash fail.")
                BKLog.i("unprotect flash OK.")
                BKLog.i(f"write custom data: {bytes.hex(bydata)} @0x{addr:x}.")
                
                if not chip_set.write_flash_info(addr,bydata):
                    raise Exception("Write custom data to flash fail.")
                BKLog.w("Write custom data to flash done.")
                BKLog.i("protect flash...") 
                if not chip_set.un_protect_flash(protect=True):
                    raise Exception("protect flash fail.")
                BKLog.i("protect flash OK.")

            result = True
        except Exception as e:
            result = False
            BKLog.e(str(e))

        finally:
            #write tool version to flash OTP
            # if not chip_set.write_tool_version(get_tool_version()):
            #     BKLog.e("write tool version to flash OTP fail.")
                
            if self.reboot_after_donwload:
                BKLog.i("do reboot from bootrom.")
                base_controller.do_reboot()
                #time.sleep(2)

            if result and downloadinfo.check_words != '':
                tmp = downloadinfo.check_words.split('@')               
                self.ser.reset(int(tmp[1]))
                result = False
                BKLog.w(f"start check key words {tmp[0]}.")
                retry_cnt = 5
                while retry_cnt>0:
                    start_t = time.time()
                    bcr = b''                  
                    while time.time()-start_t<3:
                        rbuf = self.ser.ser.read_all()
                        if rbuf:                            
                            #BKLog.w(rbuf)
                            rlist = rbuf.split(b'\r\n')
                            if len(rlist)==1:
                                bcr = bcr+rlist[0]
                            else:
                                for idx in range(len(rlist)):
                                    if idx==0:
                                        sub_rbuf = bcr+rlist[idx]
                                        bcr = b''
                                        # if sub_rbuf:
                                        #     BKLog.w(sub_rbuf)
                                    elif idx==len(rlist)-1:
                                        bcr = rlist[idx]
                                    else:
                                        sub_rbuf = rlist[idx]
                                        # if sub_rbuf:
                                        #     BKLog.w(sub_rbuf)
                                    
                                    if sub_rbuf.find(tmp[0].encode('utf-8'))!=-1:                      
                                        result = True
                                        break
                        if result:
                            break

                    if result:
                        break
                    base_controller.do_reset(reset_type=self.reset_type,baudrate=460800)
                    retry_cnt-=1
                if not result:
                    BKLog.e(f"check key words {tmp[0]} fail.")
               
            
            self.ser.disconnect()
            end_time = time.time()
            elapsed_time = end_time - start_time
            if result:
                BKLog.w("Test complete, all pass.")
                BKLog.w("done.")
            else:
                BKLog.e("Test fail.")
            BKLog.w(f"Elapse time: {elapsed_time:.6f} seconds.\n")
            return result  


    def download_secure_task(self, downloadinfo: DownloadInfo):
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        self._init_log_level(self.ser.ser.name,self.debug)
        try:
        # get bus
            tmp_res,linktype = base_controller.get_bus(reset_type=self.reset_type, linkcheck_obj=self.link_check_type,reset_baudrate=self.reset_baudrate, max_try_count=self.getbus_times,link_key=self.link_key)
            if not tmp_res:
                raise Exception("get bus fail.")
            # get chip id
            time.sleep(0.1) #fix read chip id fail sometimes
            if self.chip_id is None:                                     
                self.chip_id = base_controller.get_chip_id_retry(self.retry)
                if self.chip_id is None:
                    raise Exception("get chip id fail.")               
            BKLog.w("chip id is 0x{:x}".format(self.chip_id))     
            # init chip_set class
            chip_set = get_chip_set_with_chip_id(self.chip_id)
            if chip_set is None:
                raise Exception("get chip set fail.")
            # update serial
            chip_set.update_serial_instance(self.ser)
            # parse flash
            BKLog.i("parse flash info...") 
            if not chip_set.parse_flash_info():
                raise Exception("parse flash info fail.")
            BKLog.i("set baudrate to {0}...".format(downloadinfo.work_baudrate)) 

            if not chip_set.set_baudrate(baudrate=downloadinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(downloadinfo.work_baudrate))
            upgrade_pro = False
            if linktype==LINKCHECK.BOOTROM:
                BKLog.i("check aes key enable...")                             
                ret = chip_set.check_aes_key_enable()
                if ret is False:
                    raise Exception('check aes key enable fail.')
                BKLog.i("aes key is enable" if ret==1 else "aes key is disable")         
                if ret == 1:
                    BKLog.i("check keywords 'bkaesrdm' exist...")
                    ret,bl2_start,bl2_end = chip_set.check_bkaesrdm_status()
                    if ret is False:
                        raise Exception('check aesrdm keywords fail.')
                    BKLog.i("keywords is exist" if ret==0 else "keywords is not exist")         
                    if ret == 0:
                        BKLog.i("check bl2 version...")
                        cnt = 10
                        while cnt>0:
                            ret = self.bl2_version_upgrade(chip_set,downloadinfo.vecbinInfo[1].pfile,downloadinfo.vecbinInfo[1])
                            if ret is False:
                                raise Exception('upgrade bl2 fail.')
                            if ret is None:
                                break
                            cnt -= 1
                        downloadinfo.vecbinInfo[1].bCheck = False #dont download this bin later
                        upgrade_pro = True
            elif linktype==LINKCHECK.BL2:   #link to BL2
                cnt = 10
                while cnt>0:
                    ret = self.bl2_version_upgrade(chip_set,downloadinfo.vecbinInfo[1].pfile,downloadinfo.vecbinInfo[1])
                    if ret is False:
                        raise Exception('upgrade bl2 fail.')
                    if ret == 0:
                        break
                    cnt -= 1
                downloadinfo.vecbinInfo[1].bCheck = False #dont download this bin later
                upgrade_pro = True
            #write otp data
            if not upgrade_pro:
                for sub_bin in downloadinfo.vecbinInfo:
                    if sub_bin.bOtpData and sub_bin.bCheck:
                        ret,msg = chip_set.write_chip_otp(sub_bin.iStartAddress,bytes.hex(sub_bin.pfile),False)
                        if not ret:
                            raise Exception('write otp data fail.')
                        sub_bin.bCheck=False

                       
            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                #unprotect flash
                BKLog.i("unprotect flash...") 
                if not chip_set.un_protect_flash(protect=False):
                    raise Exception("unprotect flash fail.")
                BKLog.i("unprotect flash OK.")

            if downloadinfo.bDownloadBin:
                #erase all flash if commandline contains '--erase-all'
                if downloadinfo.bAllFlash:
                    BKLog.i("erase whole flash.")
                    if not chip_set.erase_flash(True, None, None, False, self.retry):                      
                        raise Exception("erase whole flash fail.")
                    BKLog.i("erase whole flash OK.")
                else:
                    erase_idx = 0
                    for binfile in downloadinfo.vecbinInfo:
                        BKLog.i(f"[{erase_idx}] erase flash.")
                        BKLog.i(f"[{erase_idx}] start: 0x{binfile.iStartAddress:x}.")
                        BKLog.i(f"[{erase_idx}] length: 0x{binfile.iFileLength:x}.")
                        #erase flash by bin start address and size if commandline donot contain '--erase-all'
                        if binfile.bCheck and binfile.bDataPort and not binfile.bOtpData:
                            if not chip_set.erase_flash(False,binfile.iStartAddress,binfile.iStartAddress+binfile.iFileLength,False,self.retry):
                                raise Exception("erase flash fail.")
                        erase_idx+=1
                #write each bin to flash 
                write_idx = 0
                for binfile in downloadinfo.vecbinInfo:
                    if binfile.bCheck and not binfile.bOtpData:   
                        if binfile.iStartAddress & 0xfff:               
                            binfile.iStartAddress = int((binfile.iStartAddress+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K
                        BKLog.i(f"[{write_idx}] write flash.")
                        BKLog.i(f"[{write_idx}] start: 0x{binfile.iStartAddress:x}.")
                        BKLog.i(f"[{write_idx}] length: 0x{binfile.iFileLength:x}.")
                        #write flash by bin start size and size
                        if binfile.bDataPort:    
                            if not chip_set.download_flash(binfile.pfile, binfile.iStartAddress, self.retry,True, self.reset_type, link_type=LINKCHECK.BL2):
                                raise Exception("Write flash fail.")
                        else:
                            if not chip_set.download_flash_cbus(binfile.pfile, binfile.iPartition_start, binfile.iPartition_size,binfile.iStartAddress, self.retry, self.reset_type, link_type=LINKCHECK.BL2):
                                raise Exception("Write flash fail.")
                    write_idx+=1
                BKLog.w("Write flash done.")
            
            #write cfg information to flash
            if downloadinfo.bDownloadDeviceName:
                BKLog.i(f"write device name: {downloadinfo.device_name} @0x{downloadinfo.device_name_Addr:x}.")
                if not chip_set.write_flash_info(downloadinfo.device_name_Addr,downloadinfo.device_name):
                    raise Exception("Write device name to flash fail.")
                BKLog.w("Write device name to flash done.")
                
            if downloadinfo.bDownloadDeviceID:
                BKLog.i(f"write device ID: {downloadinfo.device_id} @0x{downloadinfo.device_id_Addr:x}.")
                devid = downloadinfo.device_id
                if not downloadinfo.bEnableBigEndian:
                    devid = bytes_reverse(downloadinfo.device_id)
                if not chip_set.write_flash_info(downloadinfo.device_id_Addr,devid):
                    raise Exception("Write device ID to flash fail.")
                BKLog.w("Write device ID to flash done.")
                
            if downloadinfo.bDownloadBin or downloadinfo.bDownloadDeviceID or downloadinfo.bDownloadDeviceName:
                BKLog.i("protect flash...") 
                if not chip_set.un_protect_flash(protect=True):
                    raise Exception("protect flash fail.")
                BKLog.i("protect flash OK.")
            #write efuse and otp ...
            if downloadinfo.sEfuseKeyFile:
                BKLog.i("write safe json content.")
                safeinfo = parse_safe_json_file(downloadinfo.sEfuseKeyFile)
                #set aes key with commandline input
                if downloadinfo.aes_key:
                    for sedata in safeinfo.security_data_list:
                        if sedata.name.find('aes_key')!=-1:
                            sedata.data = downloadinfo.aes_key
                #BKLog.i("parse safe json ok.") 
                ret,msg = chip_set.write_safe_json(safeinfo)
                if not ret:
                    BKLog.e(msg)
                    raise Exception("Write safe json fail.")
                BKLog.w("write safe json done.")

            if downloadinfo.efuse_info:
                BKLog.i("write chip efuse.")
                infolist = parse_writeinfo_efuse(downloadinfo.efuse_info)
                if infolist is None:
                    raise Exception("parse efuse info fail.")
                for info in infolist:
                    if len(info)==3:
                        ret,msg = chip_set.write_efuse_by_bit(info[1],info[2],info[0])
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                    elif len(info)==2: 
                        ret,msg = chip_set.write_efuse(info[0],info[1])     
                        if not ret:
                            BKLog.e(msg)
                            raise Exception("Write efuse fail.")
                BKLog.w("Write efuse done.")

            if downloadinfo.otp_info:
                BKLog.i("write chip OTP.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_info)
                if infolist is None:
                    raise Exception("parse OTP info fail.")
                for info in infolist:
                    ret,msg = chip_set.write_chip_otp(info[1], info[0],downloadinfo.reverse_content,downloadinfo.bEnableBigEndian)                                 
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Write OTP fail.")
                BKLog.w("Write OTP done.")

            if downloadinfo.otp_pm:
                BKLog.i("set OTP permission.")
                infolist = parse_writeinfo_otp(downloadinfo.otp_pm)
                if infolist is None:
                    raise Exception("parse OTP permission info fail.")
                for info in infolist:
                    ret,msg = chip_set.set_otp_permission_oplen(info[1], info[2], info[0])                               
                    if not ret:
                        BKLog.e(msg)
                        raise Exception("Set OTP permission fail.")
                BKLog.w("Set OTP permission done.")
                
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

    def bl2_version_upgrade(self,chip_set:BaseChip,provision_bin,provisioninfo:BinInfo):
        new_ver = provision_bin[0x3004]+(provision_bin[0x3005]<<8)+(provision_bin[0x3006]<<16)+(provision_bin[0x3007]<<24)
        BKLog.i(f'new version is: 0x{new_ver:x}')
        bl2a_start = provision_bin[0x4c]+(provision_bin[0x4d]<<8)+(provision_bin[0x4e]<<16)+(provision_bin[0x4f]<<24)
        bl2a_end = provision_bin[0x50]+(provision_bin[0x51]<<8)+(provision_bin[0x52]<<16)+(provision_bin[0x53]<<24)
        bl2a_len = int(int((bl2a_end-bl2a_start-0xc0)/0x22)*0x20)
        bl2b_len = int(int((bl2a_end-bl2a_start)/0x22)*0x20)
        manifesta_start = 0x3000
        manifestb_start = 0x4000
        bl2b_start = 0x33000
        BKLog.i(f'bl2a_start is: 0x{bl2a_start:x}')
        BKLog.i(f'bl2a_end is: 0x{bl2a_end:x}')
        BKLog.i(f'bl2a_len is: 0x{bl2a_len:x}')
        BKLog.i(f'bl2b_len is: 0x{bl2b_len:x}')
        baudrate_backup = self.ser.baudrate
        basectrl = BaseController(serial_instance=self.ser)
        if not basectrl.reconnect_bus(reset_type=RESET_TYPE.DTR_RTS,linkcheck_obj=LINKCHECK.BL2,max_retry_count=30,link_key=self.link_key):
            BKLog.e("reconnect bus fail.")
            return False
        time.sleep(0.1)
        if not chip_set.set_baudrate(baudrate=baudrate_backup, delay_ms=20):
            BKLog.e("set baudrate {0} fail.".format(baudrate_backup))
            return False
        cur_ver = chip_set.read_bl2_version()
        if not cur_ver:
            BKLog.e("read bl2 current version fail.")
            return False
        BKLog.i(f'current version is: 0x{cur_ver:x}')
        if cur_ver!=0xffffffff and new_ver<=cur_ver:
            return None
        bootflag = basectrl.start_stop_bl2_upgrade(True)
        if bootflag is False:
            BKLog.e("start bl2 upgrade fail.")
            return False
        BKLog.i(f'bootflag is: {bootflag}')
        if not chip_set.un_protect_flash(protect=False):
            BKLog.e("unprotect flash fail.")
            return False
        if bootflag==2:
            BKLog.i("start write flash for manifest_a.")
            if not chip_set.erase_flash(full_erase=False,start_addr=manifesta_start,end_addr=manifesta_start+0x1000,align_addr=True):
                BKLog.e("erase flash fail.")
                return False
            time.sleep(0.1)
            if not chip_set.download_flash(wbuf=provision_bin[manifesta_start:manifesta_start+0x1000],start_addr=manifesta_start):
                BKLog.e("write flash fail.")
                return False
            BKLog.i("start write flash for bl2_a.")
            if not chip_set.download_flash_cbus(wbuf=provision_bin[bl2a_start+0xc0:bl2a_start+0xc0+bl2a_len],partition_start=provisioninfo.iPartition_start
                                                ,partition_size=provisioninfo.iPartition_size,start_addr=0
                                                ,reset_type=RESET_TYPE.DTR_RTS,link_type=LINKCHECK.BL2):
                BKLog.i("write flash for bl2_a fail.")
                return False
            
        elif bootflag==1:
            BKLog.i("start write flash for manifest_b.")
            if not chip_set.erase_flash(full_erase=False,start_addr=manifestb_start,end_addr=manifestb_start+0x1000,align_addr=True):
                BKLog.e("erase flash fail.")
                return False
            time.sleep(0.1)
            if not chip_set.download_flash(wbuf=provision_bin[manifestb_start:manifestb_start+0x1000],start_addr=manifestb_start):
                BKLog.e("write flash fail.")
                return False
            BKLog.i("start write flash for bl2_b.")
            if not chip_set.download_flash_cbus(wbuf=provision_bin[bl2b_start:bl2b_start+bl2b_len],partition_start=provisioninfo.iPartition_start
                                                ,partition_size=provisioninfo.iPartition_size,start_addr=0
                                                ,reset_type=RESET_TYPE.DTR_RTS,link_type=LINKCHECK.BL2):
                BKLog.i("write flash for bl2_b fail.")
                return False
            

        else:
            return False
        if not basectrl.start_stop_bl2_upgrade(start_stop=False):
            BKLog.e("stop bl2 upgrade fail.")
            return False
        time.sleep(0.01)
        basectrl.do_reboot()
        return True
                    
    def download_flash(self, work_br, binlist: List[BinInfo], all_flash_erase, chip_set: BaseChip, read_baudrate=500000):           
        msg = None
        # parse flash
        BKLog.i("parse flash info...") 
        if not chip_set.parse_flash_info():
            msg = "parse flash info fail."
            return False, msg
        
        BKLog.i("set baudrate to {0}...".format(work_br))
        if not chip_set.set_baudrate(baudrate=work_br, delay_ms=20):
            msg = ("set baudrate {0} fail.".format(work_br))
            return False, msg
                    
        #unprotect flash
        BKLog.i("unprotect flash...") 
        if not chip_set.un_protect_flash(protect=False):
            msg = ("unprotect flash fail.")
            return False, msg
        BKLog.i("unprotect flash OK.")
        
        if all_flash_erase:
            if not chip_set.erase_flash(True, None, None, False, self.retry):                      
                msg = ("erase whole flash fail.")
                return False, msg
            BKLog.i("erase whole flash OK.")
        else:
            for binfile in binlist:
                #erase flash by bin start address and size if commandline donot contain '--erase-all'
                if not chip_set.erase_flash(False,binfile.iStartAddress,binfile.iStartAddress+binfile.iFileLength,False,self.retry):
                    msg = ("erase flash fail.")
                    return False, msg

        #write each bin to flash                
        for binfile in binlist:   
            if binfile.iStartAddress & 0xfff:               
                binfile.iStartAddress = int((binfile.iStartAddress+FLASH_SIZE_DEF.SIZE_4K)/FLASH_SIZE_DEF.SIZE_4K)*FLASH_SIZE_DEF.SIZE_4K

            #write flash by bin start size and size    
            if not chip_set.download_flash(binfile.pfile, binfile.iStartAddress, self.retry, self.reset_type, self.link_check_type,read_baudrate=read_baudrate):
                msg = ("Write flash fail.")
                return False, msg
        BKLog.w("Write flash done.")

        BKLog.i("protect flash...") 
        if not chip_set.un_protect_flash(protect=True):
            msg = ("protect flash fail.")
            return False, msg
        BKLog.i("protect flash OK.")
        return True, None
        
           
    def download_safe_json(self, work_br, safe_json, aes_key, chip_set: BaseChip):
        msg = None     
        BKLog.i("set baudrate to {0}...".format(work_br))
        if not chip_set.set_baudrate(baudrate=work_br, delay_ms=20):
            msg = ("set baudrate {0} fail.".format(work_br))
            return False, msg
                           
        BKLog.i("write safe json content.")
        safeinfo = parse_safe_json_file(safe_json)
        #set aes key with commandline input
        if aes_key:
            for sedata in safeinfo.security_data_list:
                if sedata.name.find('aes_key')!=-1:
                    sedata.data = aes_key
        #BKLog.i("parse safe json ok.") 
        ret, msg = chip_set.write_safe_json(safeinfo)   
        if not ret:
            return False, msg
        BKLog.w("write safe json done.")                         
        return True, None

        
    