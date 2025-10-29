from bkutils.protocol.bootrom.v2_0.v1_4 import FlashCustomEraseProtocol
import unittest


class TestFlashCustomEraseProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = FlashCustomEraseProtocol()

    def test_cmd(self):
        Size_cmd = 1
        ADDR = 0x12345678
        cmd = self.protocol.cmd(ADDR, Size_cmd)
        Scmd = 0x0f
        payload = bytearray([Size_cmd, ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, 0xff, 0xf4, (1 + len(payload))&0xff, ((1 + len(payload))>>8)&0xff, Scmd])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 16    # 11 + 5
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        operate = bytes([0x0f])
        Size_cmd = 1
        status = 0x0
        ADDR = 0x12345678
        payload = bytearray([Size_cmd, ADDR&0xff, (ADDR>>8)&0xff, (ADDR>>16)&0xff, (ADDR>>24)&0xff])
        response_content = bytearray([0x04, 0x0e, 0xff, 0x01, 0xe0, 0xfc, 0xf4, (2 + len(payload))&0xff, ((2 + len(payload))>>8)&0xff])
        response_content.extend(operate)
        response_content.append(status)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content, Size_cmd, ADDR), "response_check failed for a valid response")

        # 不合法的响应内容
        # invalid_response_content = bytearray([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
        # self.assertFalse(self.protocol.response_check(invalid_response_content), "response_check did not fail for an invalid response")

if __name__ == '__main__':
    unittest.main()