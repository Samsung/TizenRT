import time
from ..chip_controller import SerialHelper
from ..chip_controller import BaseController
from ..bk_log.log_utils import *


class ActionBase(object):

    def __init__(self, port, baudrate = 115200) -> None:
        self.ser = self.__init_serial(port,baudrate)
        
    def __init_serial(self, port, baudrate):
        tmp_ser = SerialHelper()
        tmp_ser.connect(port, baudrate, 0)
        return tmp_ser
    
    def _init_log_level(self,port,debug):
        if debug=='0':
            BKLog(logger_level=logging.CRITICAL, log_folder_path='./',threading_id=threading.current_thread().ident,COM_PORT=port) # 实例化log
        elif debug=='1':
            BKLog(logger_level=logging.ERROR, log_folder_path='./',threading_id=threading.current_thread().ident,COM_PORT=port) # 实例化log 
        elif debug=='2':
            BKLog(logger_level=logging.WARNING, log_folder_path='./',threading_id=threading.current_thread().ident,COM_PORT=port) # 实例化log
        elif debug=='3':
            BKLog(logger_level=logging.INFO, log_folder_path='./',threading_id=threading.current_thread().ident,COM_PORT=port) # 实例化log
        elif debug=='999':
            BKLog(logger_level=logging.DEBUG, log_folder_path='./',threading_id=threading.current_thread().ident,COM_PORT=port) # 实例化log

    
