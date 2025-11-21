from bkutils.protocol.bootrom.v2_0 import Bl2LinkCheckProtocol
import unittest


class TestBl2LinkCheckProtocol(unittest.TestCase):

    def setUp(self):
        self.protocol = Bl2LinkCheckProtocol()

    def test_cmd(self):
        cmd = self.protocol.cmd()
        cmd_byte = 0x02
        payload = bytes([])
        length = 1 + len(payload)
        expected_cmd = bytearray([0x01, 0xe0, 0xfc, length, cmd_byte])
        expected_cmd.extend(payload)
        self.assertEqual(cmd, expected_cmd, "cmd method did not generate the expected command")

    def test_expect_length(self):
        expected_length = 8    # 7 + 1
        self.assertEqual(self.protocol.expect_length, expected_length, "expect_length did not return the expected length")

    def test_response_check(self):
        # 合法的响应内容
        CMD = bytes([0x03])
        payload = bytes([0x00])
        response_content = bytearray([0x04, 0x0e, 3+len(CMD)+len(payload), 0x01, 0xe0, 0xfc])
        response_content.extend(CMD)
        response_content.extend(payload)
        self.assertTrue(self.protocol.response_check(response_content), "response_check failed for a valid response")

if __name__ == '__main__':
    unittest.main()