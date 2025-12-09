import time
from ..bk_log import BKLog
from .serial_helper import SerialHelper
from ..protocol.app import AppEnterOutterFlashModeProtocol,AppEnterOutterFlashModeSuccessProtocol
from ..protocol.bootrom.v2_0 import ChipEnterCbusModeExtProtocol,ChipEnterCbusModeProtocol,\
    ChipExitCbusModeProtocol,ChipStartBL2UpgradeProtocol,ChipStopBL2UpgradeProtocol,ChipHighFreqEnableProtocol


class AdvancedController(object):
    # 部分芯片支持的控制操作
    def __init__(self, serial_instance: SerialHelper) -> None:
        self.serial_instance = serial_instance

    def enter_exit_cbus_mode(self,flash_size,enter_exit,part_addr=None,part_size=None,flash_addr=None,max_try_count=60):
        cnt = max_try_count
        while cnt>0:
            if enter_exit:
                cbus_protocol = ChipEnterCbusModeProtocol()
                if flash_size >= 256 * 1024 * 1024:
                    cbus_protocol = ChipEnterCbusModeExtProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(cbus_protocol.cmd([part_addr&0xff,(part_addr>>8)&0xff,(part_addr>>16)&0xff,(part_addr>>24)&0xff]
                                                                                    ,[part_size&0xff,(part_size>>8)&0xff,(part_size>>16)&0xff,(part_size>>24)&0xff]
                                                                                    ,[flash_addr&0xff,(flash_addr>>8)&0xff,(flash_addr>>16)&0xff,(flash_addr>>24)&0xff])
                                                                                    ,cbus_protocol.expect_length,5.0)
                if tmp_res and cbus_protocol.response_check(content):
                    return True
            else:
                cbus_protocol = ChipExitCbusModeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(cbus_protocol.cmd(),cbus_protocol.expect_length,1.0)
                if tmp_res and cbus_protocol.response_check(content):
                    return True
            time.sleep(0.2)
            cnt -= 1
        return False
         

    def start_stop_bl2_upgrade(self,start_stop,max_try_count = 5):
        cnt = max_try_count
        while cnt>0:
            if start_stop:
                bl2_upgrade_protocol = ChipStartBL2UpgradeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(bl2_upgrade_protocol.cmd(),bl2_upgrade_protocol.expect_length,10.0)
                if tmp_res and bl2_upgrade_protocol.response_check(content):
                    return bl2_upgrade_protocol.get_boot_flag(content)
            else:
                bl2_upgrade_protocol = ChipStopBL2UpgradeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(bl2_upgrade_protocol.cmd(),bl2_upgrade_protocol.expect_length,10.0)
                if tmp_res and bl2_upgrade_protocol.response_check(content):
                    return True
            cnt -= 1
        return False
    

    def enter_outter_flash_mode(self,max_retry_count=5):
        of_protocol = AppEnterOutterFlashModeProtocol()
        cnt = max_retry_count
        while cnt>0:      
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(of_protocol.cmd(),
                                                                of_protocol.expect_length,
                                                                2.0)
            if tmp_res and of_protocol.response_check(content):
                return True
            cnt-=1
        return False
    
    def enter_outter_flash_mode_success(self,max_retry_count=5):
        of_protocol = AppEnterOutterFlashModeSuccessProtocol()
        cnt = max_retry_count
        while cnt>0:      
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(of_protocol.cmd(),
                                                                of_protocol.expect_length,
                                                                2.0)
            if tmp_res and of_protocol.response_check(content):
                return True
            cnt-=1
        return False
    
    def high_freq_enable(self,mode,cpu_freq,flash_freq,max_retry_count=5):
        hf_protocol = ChipHighFreqEnableProtocol()
        cnt = max_retry_count
        while cnt>0:      
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(hf_protocol.cmd(mode,cpu_freq,flash_freq),
                                                                hf_protocol.expect_length,
                                                                2.0)
            if tmp_res and hf_protocol.response_check(content):
                return True
            cnt-=1
        return False
