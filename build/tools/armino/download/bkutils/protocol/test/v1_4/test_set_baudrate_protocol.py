from bkutils.protocol.bootrom.v2_0.v1_4 import SetBaudrateProtocol
import unittest


class TestSetBaudrateProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = SetBaudrateProtocol()

    def test_cmd(self):
        baudrate = 0x12345678  # 假设
        delay_ms = 0x12
        cmd = self.protocol.cmd(baudrate, delay_ms)
        cmd_byte = 0x0F
        payload = bytes([baudrate&0xff, (baudrate>>8)&0xff, (baudrate>>16)&0xff, (baudrate>>24)&0xff, 
                         delay_ms&0xff])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 12    # 7 + 5
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x0F])
        DATA = 0x12345678  # 假设
        delay_ms = 0x12
        payload = bytes([DATA & 0xff, (DATA >> 8) & 0xff, (DATA >> 16) & 0xff, (DATA >> 24) & 0xff, delay_ms])
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content, DATA), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

if __name__ == '__main__':
    unittest.main()