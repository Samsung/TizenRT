import unittest
from .v1_4.test_link_check_protocol import TestLinkCheckProtocol
from .v1_4.test_read_reg_protocol import TestReadRegProtocol
from .v1_4.test_write_reg_protocol import TestWriteRegProtocol
from .v1_4.test_stay_rom_protocol import TestStayRomProtocol
from .v1_4.test_set_baudrate_protocol import TestSetBaudrateProtocol
from .v1_4.test_check_crc_protocol import TestCheckCrcProtocol
from .v1_4.test_reboot_protocol import TestRebootProtocol
from .v1_4.test_flash_erase_all_protocol import TestFlashEraseAllProtocol
from .v1_4.test_flash_erase_4k_protocol import TestFlashErase4kProtocol
from .v1_4.test_flash_custom_erase_protocol import TestFlashCustomEraseProtocol
from .v1_4.test_flash_write_4k_protocol import TestFlashWrite4kProtocol
from .v1_4.test_flash_read_4k_protocol import TestFlashRead4kProtocol
from .v1_4.test_flash_write_sr_protocol import TestFlashWriteSRProtocol
from .v1_4.test_flash_read_sr_protocol import TestFlashReadSRProtocol
from .v1_4.test_flash_custom_read_protocol import TestFlashCustomReadProtocol
from .v1_4.test_flash_custom_write_protocol import TestFlashCustomWriteProtocol

from .test_bl2_link_check_protocol import TestBl2LinkCheckProtocol
from .test_check_crc_ext_protocol import TestCheckCrcExtProtocol
from .test_chip_enter_secure_debug_protocol import TestChipEnterSecureDebugProtocol
from .test_chip_request_secure_debug_protocol import TestChipRequestSecureDebugProtocol
from .test_flash_custom_read_ext_protocol import TestFlashCustomReadExtProtocol
from .test_flash_custom_write_ext_protocol import TestFlashCustomWriteExtProtocol
from .test_flash_erase_4k_ext_protocol import TestFlashErase4kExtProtocol
from .test_flash_erase_all_ext_protocol import TestFlashEraseAllExtProtocol
from .test_flash_read_4k_ext_protocol import TestFlashRead4kExtProtocol
from .test_flash_write_4k_ext_protocol import TestFlashWrite4kExtProtocol

if __name__ == '__main__':
    # 创建测试加载器
    loader = unittest.TestLoader()

    # 创建测试套件
    suite = unittest.TestSuite()

    # 加载测试用例
    suite.addTests(loader.loadTestsFromTestCase(TestLinkCheckProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestReadRegProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestWriteRegProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestStayRomProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestSetBaudrateProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestCheckCrcProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestRebootProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashEraseAllProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashErase4kProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashCustomEraseProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashWrite4kProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashRead4kProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashWriteSRProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashReadSRProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashCustomReadProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashCustomWriteProtocol))
    
    suite.addTests(loader.loadTestsFromTestCase(TestBl2LinkCheckProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestCheckCrcExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestChipEnterSecureDebugProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestChipRequestSecureDebugProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashCustomReadExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashCustomWriteExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashErase4kExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashEraseAllExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashRead4kExtProtocol))
    suite.addTests(loader.loadTestsFromTestCase(TestFlashWrite4kExtProtocol))
    

    # 运行测试套件
    runner = unittest.TextTestRunner()
    runner.run(suite)