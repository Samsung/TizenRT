import time
from .action_base import ActionBase
from ..chip_controller import BaseController
from ..chip_controller import get_chip_set_with_chip_id
from ..bk_log import BKLog
from ..common.common import AuthorizeInfo


class Authorize(ActionBase):

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

    def authorize_task(self, authinfo: AuthorizeInfo):
        result = False
        start_time = time.time()
        base_controller = BaseController(serial_instance=self.ser)
        self._init_log_level(self.ser.ser.name,self.debug)
        try:
        # get bus
            tmp_res,linktype = base_controller.get_bus(reset_type=self.reset_type, linkcheck_obj=self.link_check_type, reset_baudrate=self.reset_baudrate, max_try_count=self.getbus_times,link_key=self.link_key)
            if not tmp_res:
                raise Exception("get bus fail.")
            # get chip id
            sec_id = 0
            time.sleep(0.1) #fix read chip id fail sometimes
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
            
            BKLog.i("set baudrate to {0}...".format(authinfo.work_baudrate)) 
            if not chip_set.set_baudrate(baudrate=authinfo.work_baudrate, delay_ms=20):
                raise Exception("set baudrate {0} fail.".format(authinfo.work_baudrate))
            
            BKLog.i("authorization...") 
            if not chip_set.cal_and_write_aes128_data(bytes.fromhex(authinfo.aes_key)):
                raise Exception("authorization fail.")
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

