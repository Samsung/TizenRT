import time

from ..bk_log import BKLog
from .serial_helper import SerialHelper
from ..common.common import RESET_TYPE, LINKCHECK
from ..protocol.app import AppRebootProtocol,AppEnterOutterFlashModeProtocol,AppEnterOutterFlashModeSuccessProtocol,AppBootloaderOtaSuccessProtocol,AppBootloaderPartitionFlagProtocol
from ..protocol.bootrom.v2_0 import LinkCheckProtocol, Bl2LinkCheckProtocol, StayRomProtocol, \
    GetChipIdProtocol, RebootProtocol,ChipEnterCbusModeExtProtocol,ChipEnterCbusModeProtocol,\
    ChipExitCbusModeProtocol,ChipStartBL2UpgradeProtocol,ChipStopBL2UpgradeProtocol,LinkCheckExtProtocol


class BaseController(object):

    link_check_obj = None

    # 与芯片型号无关或者获取芯片型号之前的控制操作
    def __init__(self, serial_instance: SerialHelper) -> None:
        self.serial_instance = serial_instance

    def do_reset(self, reset_type, baudrate):
        if reset_type == RESET_TYPE.DTR_RTS:
            self.serial_instance.ser.dtr = 0
            self.serial_instance.ser.rts = 1
            time.sleep(0.2)
            self.serial_instance.ser.rts = 0
        elif reset_type == RESET_TYPE.CMD_ASCII:
            if baudrate != 115200:
                self.serial_instance.reset(baudrate=baudrate)
            self.serial_instance.write_cmd(AppRebootProtocol().cmd(reboot_type="at"))
            if baudrate != 115200:
                self.serial_instance.reset(baudrate=115200)
        elif reset_type == RESET_TYPE.CMD_HEX:
            if baudrate != 115200:
                self.serial_instance.reset(baudrate=baudrate)
            self.serial_instance.write_cmd(AppRebootProtocol().cmd(reboot_type="hex"))
            if baudrate != 115200:
                self.serial_instance.reset(baudrate=115200)
        else:
            raise Exception('Not support {0}'.format(reset_type))
    
    def setStaticLinkcheckobj(self, link_check_obj):
        BaseController.link_check_obj = link_check_obj

    def do_link_check(self, linkcheck_obj, max_try_count=60,link_key=None,use_mixed_detection=False):
        cnt = max_try_count
        while cnt > 0:
            if linkcheck_obj == LINKCHECK.BOOTROM:
                lcp = LinkCheckProtocol()
                res, content = (
                    self.serial_instance.write_cmd_and_wait_response_without_drain(
                        lcp.cmd(), lcp.expect_length, 0.0015
                    )
                )
                if res:
                    if use_mixed_detection:
                        res,isBl1 = lcp.response_check_mixed(content)
                        if res:
                            self.setStaticLinkcheckobj(LINKCHECK.BOOTROM if isBl1 else LINKCHECK.BL2)
                            return True
                    else:
                        if lcp.response_check(content):
                            self.setStaticLinkcheckobj(linkcheck_obj)
                            return True
            elif linkcheck_obj == LINKCHECK.BL2:
                bl2_lcp = Bl2LinkCheckProtocol()
                res, content = (
                    self.serial_instance.write_cmd_and_wait_response_without_drain(
                        bl2_lcp.cmd(), bl2_lcp.expect_length, 0.0015
                    )
                )
                if res:
                    if use_mixed_detection:
                        res,isBl2 = bl2_lcp.response_check_mixed(content)
                        if res:
                            self.setStaticLinkcheckobj(LINKCHECK.BL2 if isBl2 else LINKCHECK.BOOTROM)
                            return True
                    else:
                        if bl2_lcp.response_check(content):
                            self.setStaticLinkcheckobj(linkcheck_obj)
                            return True
            elif linkcheck_obj == LINKCHECK.BOOTROM_PRE:
                srp = StayRomProtocol()
                res, content = (
                    self.serial_instance.write_cmd_and_wait_response_without_drain(
                        srp.cmd(), srp.expect_length, 0.0015
                    )
                )
                if res and srp.response_check(content):
                    self.setStaticLinkcheckobj(linkcheck_obj)
                    return True
            elif linkcheck_obj == LINKCHECK.BOOTROM_EXT:
                lcp = LinkCheckExtProtocol()
                res, content = self.serial_instance.write_cmd_and_wait_response_without_drain(lcp.cmd(link_key,0x60), lcp.expect_length, 0.0015)
                if res and lcp.response_check(content):
                    self.setStaticLinkcheckobj(linkcheck_obj)
                    return True
            else:
                raise Exception("Not support {0}".format(linkcheck_obj))
            cnt -= 1

        return False

    def get_bus(self, reset_type=None, linkcheck_obj=None, reset_baudrate=115200, max_try_count=100,link_key=None):
        return self.get_bus_v3(reset_type, linkcheck_obj, reset_baudrate, max_try_count, link_key)
    
    def get_bus_v3(self, reset_type=None, linkcheck_obj=None, reset_baudrate=115200, max_try_count=100,link_key=None):
        BKLog.i('Getting bus v3...')
        self.do_reset(RESET_TYPE.DTR_RTS, baudrate=reset_baudrate)
        link_check_ret = False
        outter_cnt = 0
        max_recycle_count = 30
        max_retry_interval_first = 80
        max_retry_interval_other = 400
        max_retry_interval = max_retry_interval_first
        use_mixed_detection = True
        delay_time_before_reboot = 200
        delay_time_after_reboot = 10
        while outter_cnt < max_recycle_count:
            inner_cnt = 0
            while inner_cnt < max_retry_interval:
                link_check_ret = self.do_link_check(LINKCHECK.BOOTROM,link_key=link_key,max_try_count=1,use_mixed_detection=use_mixed_detection)
                if link_check_ret:
                    break
                link_check_ret = self.do_link_check(LINKCHECK.BL2,link_key=link_key,max_try_count=1,use_mixed_detection=use_mixed_detection)
                if link_check_ret:
                    break
                inner_cnt += 1
            if link_check_ret:
                break

            self.serial_instance.write_cmd(b'\n')
            time.sleep(delay_time_before_reboot/1000)
            self.do_reset(RESET_TYPE.CMD_ASCII, baudrate=reset_baudrate)
            time.sleep(delay_time_after_reboot/1000)
            max_retry_interval = max_retry_interval_other
            outter_cnt += 1
        
        if link_check_ret:
            BKLog.i('Get bus success, type: {0}'.format(BaseController.link_check_obj))
            return True, BaseController.link_check_obj
        else:
            BKLog.i('Get bus failed')
            return False,BaseController.link_check_obj 
    
    def reconnect_bus(self,reset_type=None, linkcheck_obj=None,reset_baudrate=115200,max_retry_count=60,link_key=None):
        self.serial_instance.reset(115200)
        ret,linktype = self.get_bus(reset_type,linkcheck_obj,reset_baudrate,max_retry_count,link_key)
        if not ret:
            return False
        return True

    def get_chip_id(self):
        chip_id_reg_list = [0x44010004, 0x800000, 0x34010004]
        gcip = GetChipIdProtocol()
        self.serial_instance.drain()
        for tmp_reg in chip_id_reg_list:
            res, content = self.serial_instance.write_cmd_and_wait_response(
                gcip.cmd(tmp_reg), gcip.expect_length, 0.5
            )
            if res and gcip.response_check(content, tmp_reg):
                tmp_chip_id = gcip.get_chip_id(content)
                if tmp_chip_id > 0 and tmp_chip_id != 0xFFFFFFFF:
                    return tmp_chip_id
            time.sleep(0.001)
        BKLog.w('Cant get chip id')
        return None
    
    
    def get_second_chip_id(self):
        # use pmu 0x7c id as second id
        reg_addr = 0x440001f0
        gcip = GetChipIdProtocol()
        self.serial_instance.drain()
        res, content = self.serial_instance.write_cmd_and_wait_response(gcip.cmd(reg_addr), gcip.expect_length, 0.5)
        if res and gcip.response_check(content, reg_addr):
            tmp_chip_id = gcip.get_chip_id(content)
            if tmp_chip_id > 0 and tmp_chip_id != 0xffffffff:
                return tmp_chip_id
        BKLog.w('Cant get second chip id')
        return None
    
    def get_chip_id_retry(self,max_retry_count=5,link_key=None):
        cnt = max_retry_count
        delay = 0.15
        while cnt > 0:
            chipid = self.get_chip_id()
            if chipid is not None:
                return chipid
            else:        
                if not self.reconnect_bus(RESET_TYPE.DTR_RTS,LINKCHECK.BOOTROM,115200,30,link_key):                                                   
                    return None
            time.sleep(delay)
            delay += 0.05
            cnt -= 1

    def do_reboot(self):
        rb_protocol = RebootProtocol()
        self.serial_instance.write_cmd(rb_protocol.cmd())

    def enter_exit_cbus_mode(
        self,
        flash_size,
        enter_exit,
        part_addr=None,
        part_size=None,
        flash_addr=None,
        max_try_count=60,
    ):
        cnt = max_try_count
        while cnt > 0:
            if enter_exit:
                cbus_protocol = ChipEnterCbusModeProtocol()
                if flash_size >= 256 * 1024 * 1024:
                    cbus_protocol = ChipEnterCbusModeExtProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                    cbus_protocol.cmd(
                        [
                            part_addr & 0xFF,
                            (part_addr >> 8) & 0xFF,
                            (part_addr >> 16) & 0xFF,
                            (part_addr >> 24) & 0xFF,
                        ],
                        [
                            part_size & 0xFF,
                            (part_size >> 8) & 0xFF,
                            (part_size >> 16) & 0xFF,
                            (part_size >> 24) & 0xFF,
                        ],
                        [
                            flash_addr & 0xFF,
                            (flash_addr >> 8) & 0xFF,
                            (flash_addr >> 16) & 0xFF,
                            (flash_addr >> 24) & 0xFF,
                        ],
                    ),
                    cbus_protocol.expect_length,
                    5.0,
                )
                if tmp_res and cbus_protocol.response_check(content):
                    return True
            else:
                cbus_protocol = ChipExitCbusModeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                    cbus_protocol.cmd(), cbus_protocol.expect_length, 1.0
                )
                if tmp_res and cbus_protocol.response_check(content):
                    return True
            time.sleep(0.2)
            cnt -= 1
        return False

    def start_stop_bl2_upgrade(self, start_stop, max_try_count=5):
        cnt = max_try_count
        while cnt > 0:
            if start_stop:
                bl2_upgrade_protocol = ChipStartBL2UpgradeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                    bl2_upgrade_protocol.cmd(), bl2_upgrade_protocol.expect_length, 10.0
                )
                if tmp_res and bl2_upgrade_protocol.response_check(content):
                    return bl2_upgrade_protocol.get_boot_flag(content)
            else:
                bl2_upgrade_protocol = ChipStopBL2UpgradeProtocol()
                tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                    bl2_upgrade_protocol.cmd(), bl2_upgrade_protocol.expect_length, 10.0
                )
                if tmp_res and bl2_upgrade_protocol.response_check(content):
                    return True
            cnt -= 1
        return False

    def enter_outter_flash_mode(self, max_retry_count=5):
        of_protocol = AppEnterOutterFlashModeProtocol()
        cnt = max_retry_count
        while cnt > 0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                of_protocol.cmd(), of_protocol.expect_length, 2.0
            )
            if tmp_res and of_protocol.response_check(content):
                return True
            cnt -= 1
        return False

    def enter_outter_flash_mode_success(self, max_retry_count=5):
        of_protocol = AppEnterOutterFlashModeSuccessProtocol()
        cnt = max_retry_count
        while cnt > 0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                of_protocol.cmd(), of_protocol.expect_length, 2.0
            )
            if tmp_res and of_protocol.response_check(content):
                return True
            cnt -= 1
        return False

    def read_bootloader_partition_flag(self, max_retry_count=5):
        partition_flag_protocol = AppBootloaderPartitionFlagProtocol()
        cnt = max_retry_count
        while cnt > 0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                partition_flag_protocol.cmd(), partition_flag_protocol.expect_length, 1.0
            )
            if tmp_res:
                ret,flag = partition_flag_protocol.response_check(content)
                if ret:
                    return flag
            cnt -= 1
        return None
    
    def write_bootloader_ota_success_flag(self, max_retry_count=5):
        ota_success_protocol = AppBootloaderOtaSuccessProtocol()
        cnt = max_retry_count
        while cnt > 0:
            tmp_res, content = self.serial_instance.write_cmd_and_wait_response(
                ota_success_protocol.cmd(), ota_success_protocol.expect_length, 1.0
            )
            if tmp_res and ota_success_protocol.response_check(content):
                return True
            cnt -= 1
        return False
