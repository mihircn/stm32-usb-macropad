import time
import serial
from serial.tools import list_ports

from app.core.constants import DEFAULT_BAUDRATE, EXPECTED_SERIAL_NUMBER, HANDSHAKE_TIMEOUT_S, PING_COMMAND, PING_RESPONSE
from app.models.device_info import DeviceInfo


class DeviceService:
    def scan_ports(self) -> list[DeviceInfo]:
        devices: list[DeviceInfo] = []
        for port in list_ports.comports():
            devices.append(
                DeviceInfo(
                    port_name=port.device,
                    description=port.description or "",
                    manufacturer=getattr(port, "manufacturer", "") or "",
                    product=getattr(port, "product", "") or "",
                    serial_number=getattr(port, "serial_number", "") or "",
                    vid=getattr(port, "vid", None),
                    pid=getattr(port, "pid", None),
                )
            )
        return devices

    def find_device(self) -> DeviceInfo | None:
        for device in self.scan_ports():
            if device.serial_number == EXPECTED_SERIAL_NUMBER:
                return device
        return None

    def verify_ping(self, port_name: str) -> tuple[bool, str]:
        try:
            with serial.Serial(port_name, DEFAULT_BAUDRATE, timeout=HANDSHAKE_TIMEOUT_S) as ser:
                time.sleep(0.2)
                ser.reset_input_buffer()
                ser.write(PING_COMMAND.encode("utf-8"))
                response = ser.readline().decode(errors="ignore").strip()
                return response == PING_RESPONSE, response
        except Exception as exc:
            return False, str(exc)
