import os
import sys
import logging
import threading
from logging.handlers import RotatingFileHandler

class BKLog(object):
    LOGGER = logging.getLogger('BKLog')
    LOGGER_MAP = {
    }
    ifclose = False
    # pre_percent = 0
    def __init__(self, logger_level=logging.INFO, log_folder_path=None, threading_id=None, COM_PORT=''):
        if BKLog.LOGGER_MAP.get(threading_id, None) is not None:
            raise Exception('logger only init once in one thread')
  
        BKLog.LOGGER_MAP[threading_id] = {
            'pre_percent': 0,
            'logger': logging.getLogger(f'BKLog_{COM_PORT}')
        }

        BKLog.LOGGER_MAP[threading_id]['logger'].setLevel(logger_level)
        formatter = logging.Formatter(f'%(asctime)s %(levelname)-7s {COM_PORT} # %(message)s')
        
        stdout_handler = logging.StreamHandler(sys.stdout)     
        BKLog.LOGGER_MAP[threading_id]['logger'].addHandler(stdout_handler)
        stdout_handler.setLevel(logger_level)
        stdout_handler.setFormatter(formatter)
        

        if log_folder_path is not None and logger_level < logging.WARNING:
            if not os.path.exists(log_folder_path):
                os.makedirs(log_folder_path)
            log_file_name = (f'bk_flash{COM_PORT}.log' if COM_PORT.startswith('/') else f'bk_flash_{COM_PORT}.log').replace('/','_') 
            log_file_path = os.path.join(log_folder_path, log_file_name)
            file_handler = RotatingFileHandler(log_file_path, maxBytes=10*1024*1024, backupCount=10)
            BKLog.LOGGER_MAP[threading_id]['logger'].addHandler(file_handler)
            file_handler.setLevel(logger_level)
            file_handler.setFormatter(formatter)

    @staticmethod
    def __logger():
        threading_id = threading.current_thread().ident
        if BKLog.LOGGER_MAP.get(threading_id, None) is not None:
            return BKLog.LOGGER_MAP[threading_id]['logger']
        else:
            return BKLog.LOGGER_MAP[None]['logger']

    #print test data such as serial port in/out data
    @staticmethod
    def d(msg,  *args, **kwargs):
        if not BKLog.ifclose:
            BKLog.__logger()._log(logging.DEBUG, '{0}'.format(msg), args, **kwargs)

    #print test normal info
    @staticmethod
    def i(msg, *args, **kwargs):
        if not BKLog.ifclose:
            BKLog.__logger()._log(logging.INFO, '{0}'.format(msg), args, **kwargs)


    #print test warning
    @staticmethod
    def w(msg, *args, **kwargs):
        if not BKLog.ifclose:
            BKLog.__logger()._log(logging.WARNING, '{0}'.format(msg), args, **kwargs)

    #print tool fail info
    @staticmethod
    def e(msg, *args, **kwargs):
        if not BKLog.ifclose:
            BKLog.__logger()._log(logging.ERROR, '{0}'.format(msg), args, **kwargs)

    #print download/erase/read progress
    @staticmethod
    def progress_log(device: str, status: str, progress: int):
        format_id_key = threading.current_thread().ident
        if BKLog.LOGGER_MAP.get(format_id_key, None) is None:
            format_id_key = None
        if not BKLog.ifclose and progress!=BKLog.LOGGER_MAP[format_id_key]['pre_percent']:
            BKLog.w('DEVICE: {0}, Status: {1}, Progress: {2}'.format(device, status, progress))
        BKLog.LOGGER_MAP[format_id_key]['pre_percent'] = progress
    
    @staticmethod
    def log_with_code(device: str, status: str, code: int, msg: str):
        if not BKLog.ifclose:
            BKLog.i(logging.INFO, 'DEVICE: {0}, Status: {1}, Code: {2}, Message: {2}'.format(device, status, code, msg))


if __name__ == '__main__':

    # 自定义封装示例
    def program_progress_log(device, progress):
        BKLog.progress_log(device, 'Programming', progress)
    
    def test_thread(com_num):
        BKLog(logger_level=logging.INFO, log_folder_path='./', threading_id=threading.current_thread().ident, COM_PORT=com_num)
        BKLog.d(f'{com_num} debug level log test')
        BKLog.i(f'{com_num} info level log test')
        BKLog.w(f'{com_num} warning level log test')
        BKLog.e(f'{com_num} errot level log test')
        for _ in range(0, 101, 10):
            BKLog.progress_log(com_num, 'Erasing', _)
        
        for _ in range(0, 101, 10):
            program_progress_log(com_num, _)

    # 通用log示例
    BKLog(logger_level=logging.INFO, log_folder_path='./') # 实例化log
    BKLog.i('debug level log test')
    t_group = []
    for _ in range(5):
        t_group.append(threading.Thread(target=test_thread, args=(f'COM{_}',)))
    for tmp_t in t_group:
        tmp_t.start()
    for tmp_t in t_group:
        tmp_t.join()

    BKLog.i('finish test')