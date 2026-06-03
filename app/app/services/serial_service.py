import serial

from app.core.constants import DEFAULT_BAUDRATE


class SerialService:
    def __init__(self) -> None:
        self.ser: serial.Serial | None = None
        self.port_name: str = ""

    def connect(self, port_name: str) -> None:
        self.disconnect()
        self.ser = serial.Serial(port_name, DEFAULT_BAUDRATE, timeout=1.0)
        self.port_name = port_name

    def disconnect(self) -> None:
        if self.ser and self.ser.is_open:
            self.ser.close()
        self.ser = None
        self.port_name = ""

    def is_connected(self) -> bool:
        return bool(self.ser and self.ser.is_open)

    def send_and_readline(self, command: str) -> str:
        if not self.ser or not self.ser.is_open:
            raise RuntimeError("Device not connected")
        self.ser.reset_input_buffer()
        self.ser.write(command.encode("utf-8"))
        self.ser.flush()
        return self.ser.readline().decode(errors="ignore")

    def send_and_read_config_block(self, command: str) -> str:
        if not self.ser or not self.ser.is_open:
            raise RuntimeError("Device not connected")
        self.ser.reset_input_buffer()
        self.ser.write(command.encode("utf-8"))
        self.ser.flush()

        lines = []
        for _ in range(32):
            line = self.ser.readline().decode(errors="ignore")
            if not line:
                break
            lines.append(line)
            if "CONFIG_END" in line:
                break
        return "".join(lines)
