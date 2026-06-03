from dataclasses import dataclass


@dataclass
class DeviceInfo:
    port_name: str
    description: str = ""
    manufacturer: str = ""
    product: str = ""
    serial_number: str = ""
    vid: int | None = None
    pid: int | None = None

    @property
    def display_name(self) -> str:
        name = self.product or self.description or self.port_name
        return f"{name} ({self.port_name})"
