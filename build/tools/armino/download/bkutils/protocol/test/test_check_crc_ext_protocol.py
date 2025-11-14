from bkutils.protocol.bootrom.v2_0 import CheckCrcExtProtocol
import unittest


class TestCheckCrcExtProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = CheckCrcExtProtocol()

    def test_cmd(self):
        sADD = 0x12345678
        eADD = 0x87654321
        cmd = self.protocol.cmd(sADD, eADD)
        cmd_byte = 0x13
        payload = bytes([sADD&0xff, (sADD>>8)&0xff, (sADD>>16)&0xff, (sADD>>24)&0xff, 
                         eADD&0xff, (eADD>>8)&0xff, (eADD>>16)&0xff, (eADD>>24)&0xff])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 11    # 7 + 4
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x13])
        CRC = 0x12345678
        payload = bytes([CRC&0xff, (CRC>>8)&0xff, (CRC>>16)&0xff, (CRC>>24)&0xff])
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

if __name__ == '__main__':
    unittest.main()