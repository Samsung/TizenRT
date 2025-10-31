from bkutils.protocol.bootrom.v2_0.v1_4 import CheckCrcProtocol
import unittest


class TestCheckCrcProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = CheckCrcProtocol()

    def test_cmd(self):
        sADDR = 0x12345678  # 假设
        eADDR = 0x87654321
        cmd = self.protocol.cmd(sADDR, eADDR)
        cmd_byte = 0x10
        payload = bytes([sADDR&0xff, (sADDR>>8)&0xff, (sADDR>>16)&0xff, (sADDR>>24)&0xff, 
                         eADDR&0xff, (eADDR>>8)&0xff, (eADDR>>16)&0xff, (eADDR>>24)&0xff])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 11    # 7 + 4
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = 0x10
        CRC = 0x12345678  # 假设
        payload = bytes([CRC & 0xff, (CRC >> 8) & 0xff, (CRC >> 16) & 0xff, (CRC >> 24) & 0xff])
        response_content = bytearray([0x04, 0x0e, 3+1+len(payload), 0x01, 0xe0, 0xfc, CMD])
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")
        
    def test_get_crc_value(self):
        CMD = 0x10
        CRC = 0x12345678  # 假设
        payload = bytes([CRC & 0xff, (CRC >> 8) & 0xff, (CRC >> 16) & 0xff, (CRC >> 24) & 0xff])
        response_content = bytearray([0x04, 0x0e, 3+1+len(payload), 0x01, 0xe0, 0xfc, CMD])
        response_content.extend(payload)
        self.assertEqual(self.protocol.get_crc_value(response_content), CRC, "get_crc_value failed")
        

if __name__ == '__main__':
    unittest.main()