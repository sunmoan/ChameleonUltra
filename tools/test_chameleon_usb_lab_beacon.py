"""Offline protocol framing tests; no device or pyserial required."""
import importlib.util
import pathlib
import unittest

p = pathlib.Path(__file__).with_name("chameleon_usb_lab_beacon.py")
spec = importlib.util.spec_from_file_location("chameleon_usb_lab_beacon", p)
m = importlib.util.module_from_spec(spec)
spec.loader.exec_module(m)


class FakePort:
    def __init__(self, response=b""):
        self.response = bytearray(response)
        self.written = bytearray()

    def write(self, data):
        self.written.extend(data)
        return len(data)

    def read(self, n):
        b = self.response[:max(1, min(n, 3))]
        del self.response[:len(b)]
        return bytes(b)

    def flush(self):
        pass


class FramingTests(unittest.TestCase):
    def test_empty_request_frame(self):
        self.assertEqual(m.pack_frame(m.CMD_LAB_BEACON_GET).hex(), "11ef041500000000e700")

    def test_nonempty_frame(self):
        request = m.pack_frame(m.CMD_LAB_BEACON_SET, bytes.fromhex("00010002c5"))
        self.assertEqual(len(request), 15)
        self.assertEqual(sum(request[:2]) & 255, 0)
        self.assertEqual(sum(request[:9]) & 255, 0)
        self.assertEqual(sum(request[9:]) & 255, 0)

    def test_parse_response(self):
        status = 0x68
        payload = bytes.fromhex("0100010002c5")
        header = bytes((m.SOF, 0xEF)) + __import__("struct").pack(">HHH", m.CMD_LAB_BEACON_GET, status, len(payload))
        response = header + bytes((m.lrc(header),)) + payload + bytes((m.lrc(payload),))
        device = FakePort(response)
        self.assertEqual(m.call(device, m.CMD_LAB_BEACON_GET), payload)
        self.assertEqual(device.written, m.pack_frame(m.CMD_LAB_BEACON_GET))

    def test_reject_bad_lrc(self):
        broken = bytearray(m.pack_frame(1045))
        broken[8] = 0
        with self.assertRaises(ValueError):
            m.read_frame(FakePort(broken))


if __name__ == "__main__":
    unittest.main()
