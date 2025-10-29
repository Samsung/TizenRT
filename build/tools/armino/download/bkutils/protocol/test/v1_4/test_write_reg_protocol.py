from bkutils.protocol.bootrom.v2_0.v1_4 import writeRegProtocol
import unittest


class TestWriteRegProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = writeRegProtocol()

    def test_cmd(self):
        reg_addr = 0x12345678  # 假设的reg_addr
        val = 0x87654321
        cmd = self.protocol.cmd(reg_addr, val)
        cmd_byte = 0x01
        payload = bytes([reg_addr&0xff, (reg_addr>>8)&0xff, (reg_addr>>16)&0xff, (reg_addr>>24)&0xff, 
                         val&0xff, (val>>8)&0xff, (val>>16)&0xff, (val>>24)&0xff])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 15    # 7 + 8
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x01])
        ADDR = 0x12345678
        DATA = 0x87654321
        payload = bytes([ADDR & 0xff, (ADDR >> 8) & 0xff, (ADDR >> 16) & 0xff, (ADDR >> 24) & 0xff,
                        DATA & 0xff, (DATA >> 8) & 0xff, (DATA >> 16) & 0xff, (DATA >> 24) & 0xff])
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content, ADDR), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

if __name__ == '__main__':
    unittest.main()