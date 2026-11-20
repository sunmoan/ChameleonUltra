#!/usr/bin/env python3
"""USB CDC control for a fixed-UUID, lab-only Chameleon Ultra BLE beacon.

Requires a firmware built with the lab beacon command extension, plus
python3 -m pip install pyserial on the Mac. No arbitrary UUID/raw BLE
packet injection is exposed by this interface.
"""
import argparse
import struct
import sys

SOF = 0x11
MAX_PAYLOAD = 4096
STATUS_SUCCESS = 0x68
CMD_GET_SLOT = 1018
CMD_SET_SLOT = 1003
CMD_LAB_BEACON_SET = 1044
CMD_LAB_BEACON_GET = 1045
CMD_LAB_BEACON_CLEAR = 1046
LAB_UUID = "7c9a8f42-3de6-4a91-bd75-2c61f4e89013"


def lrc(data: bytes) -> int:
    return (-sum(data)) & 0xFF


def pack_frame(command: int, payload: bytes = b"") -> bytes:
    if not 0 <= command <= 0xFFFF or len(payload) > MAX_PAYLOAD:
        raise ValueError("Invalid command or payload length")
    header = bytes((SOF, lrc(bytes((SOF,))))) + struct.pack(">HHH", command, 0, len(payload))
    return header + bytes((lrc(header),)) + payload + bytes((lrc(payload),))


def read_exactly(port, length: int) -> bytes:
    chunks = bytearray()
    while len(chunks) < length:
        chunk = port.read(length - len(chunks))
        if not chunk:
            raise TimeoutError("USB serial response timed out")
        chunks.extend(chunk)
    return bytes(chunks)


def read_frame(port):
    for _ in range(256):
        if read_exactly(port, 1) == bytes((SOF,)):
            break
    else:
        raise RuntimeError("Could not find Chameleon frame start byte")
    header = bytes((SOF,)) + read_exactly(port, 8)
    if lrc(header[:1]) != header[1] or lrc(header[:8]) != header[8]:
        raise ValueError("Invalid response header checksum")
    command, status, length = struct.unpack(">HHH", header[2:8])
    if length > MAX_PAYLOAD:
        raise ValueError(f"Response payload too large: {length}")
    data_and_lrc = read_exactly(port, length + 1)
    payload, checksum = data_and_lrc[:-1], data_and_lrc[-1]
    if lrc(payload) != checksum:
        raise ValueError("Invalid response payload checksum")
    return command, status, payload


def call(port, command: int, payload: bytes = b"") -> bytes:
    port.write(pack_frame(command, payload))
    port.flush()
    got_command, status, result = read_frame(port)
    if got_command != command:
        raise RuntimeError(f"Unexpected response command {got_command}, expected {command}")
    if status != STATUS_SUCCESS:
        raise RuntimeError(f"Device rejected command {command} (status=0x{status:04X})")
    return result


def get_slot(port) -> int:
    data = call(port, CMD_GET_SLOT)
    if len(data) != 1 or data[0] > 7:
        raise RuntimeError(f"Unexpected slot response: {data.hex()}")
    return data[0] + 1


def choose_slot(port, slot_number: int) -> None:
    call(port, CMD_SET_SLOT, bytes((slot_number - 1,)))


def list_ports() -> None:
    try:
        from serial.tools import list_ports as serial_ports
    except ImportError as exc:
        raise RuntimeError("Install pyserial: python3 -m pip install pyserial") from exc
    ports = list(serial_ports.comports())
    if not ports:
        print("No USB serial ports found")
    for port in ports:
        print(f"{port.device}  {port.description}")


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--port", help="e.g. /dev/cu.usbmodem1101 (use list-ports to discover)")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("list-ports", help="List serial ports on the Mac")
    sub.add_parser("status", help="Read BLE lab override state and active NFC slot")
    sub.add_parser("clear", help="Restore NFC-stored beacon configuration")
    p_set = sub.add_parser("set", help="Set a lab beacon with a firmware-fixed UUID")
    p_set.add_argument("--major", type=int, required=True)
    p_set.add_argument("--minor", type=int, required=True)
    p_set.add_argument("--power", type=int, default=-59, help="Measured RSSI at 1 m, dBm (default -59)")
    p_set.add_argument("--activate-slot8", action="store_true", help="Select NFC slot 8 to broadcast now")
    p_slot = sub.add_parser("slot", help="Read/select NFC slot (existing firmware USB command)")
    p_slot.add_argument("--select", type=int, choices=range(1, 9))
    args = parser.parse_args(argv)

    if args.command == "list-ports":
        list_ports()
        return
    if not args.port:
        parser.error("--port is required (use list-ports to locate your device)")
    try:
        import serial
    except ImportError as exc:
        raise RuntimeError("Install pyserial: python3 -m pip install pyserial") from exc

    with serial.Serial(args.port, baudrate=115200, timeout=3, write_timeout=3) as port:
        if args.command == "set":
            if not 0 <= args.major <= 65535 or not 0 <= args.minor <= 65535:
                parser.error("--major and --minor must each be 0..65535")
            if not -127 <= args.power <= 20:
                parser.error("--power must be -127..20 dBm")
            payload = struct.pack(">HHb", args.major, args.minor, args.power)
            call(port, CMD_LAB_BEACON_SET, payload)
            if args.activate_slot8:
                choose_slot(port, 8)
            slot = get_slot(port)
            print(f"Lab beacon configured: UUID={LAB_UUID}, major={args.major}, minor={args.minor}, power={args.power} dBm")
            print(f"Active NFC slot: {slot}; beacon transmission requires slot 8")
        elif args.command == "clear":
            call(port, CMD_LAB_BEACON_CLEAR)
            print("USB lab override disabled. Slot 8 returns to its stored NTAG213 beacon (if configured).")
        elif args.command == "status":
            data = call(port, CMD_LAB_BEACON_GET)
            if len(data) != 6:
                raise RuntimeError(f"Unexpected beacon response: {data.hex()}")
            enabled, major, minor, power = struct.unpack(">BHHb", data)
            slot = get_slot(port)
            print(f"Active NFC slot: {slot}")
            print(f"USB lab override: {'enabled' if enabled else 'disabled'}")
            if enabled:
                print(f"UUID={LAB_UUID}, Major={major}, Minor={minor}, Tx power={power} dBm")
            else:
                print("The firmware uses NTAG213 slot-8 data for iBeacon, if present.")
        elif args.command == "slot":
            if args.select is not None:
                choose_slot(port, args.select)
            print(f"Active NFC slot: {get_slot(port)}")


if __name__ == "__main__":
    try:
        main()
    except (OSError, TimeoutError, RuntimeError, ValueError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        sys.exit(1)
