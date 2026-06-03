from PySide6.QtCore import Qt
from PySide6.QtWidgets import (
    QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QGridLayout, QLabel,
    QPushButton, QTextEdit, QFrame, QLineEdit, QMessageBox, QTableWidget,
    QTableWidgetItem
)

from app.core.constants import EXPECTED_SERIAL_NUMBER, MAX_BUTTONS
from app.services.config_service import ConfigService
from app.services.device_service import DeviceService
from app.services.serial_service import SerialService


class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("STM32 MacroPad Configurator")
        self.resize(1200, 800)

        self.device_service = DeviceService()
        self.serial_service = SerialService()
        self.config_service = ConfigService()
        self.current_port = ""

        self.binding_inputs: list[QLineEdit] = []
        self.get_buttons: list[QPushButton] = []
        self.set_buttons: list[QPushButton] = []

        self._build_ui()
        self._refresh_device_info()
        self._update_connection_state()

    def _build_ui(self) -> None:
        central = QWidget()
        self.setCentralWidget(central)
        root = QVBoxLayout(central)
        root.setContentsMargins(18, 18, 18, 18)
        root.setSpacing(14)

        header = QFrame()
        header.setObjectName("Panel")
        header_layout = QHBoxLayout(header)

        left = QVBoxLayout()
        title = QLabel("STM32 MacroPad Configurator")
        title.setObjectName("Title")
        subtitle = QLabel(f"Detect device using serial number: {EXPECTED_SERIAL_NUMBER}")
        subtitle.setObjectName("Muted")
        left.addWidget(title)
        left.addWidget(subtitle)

        right = QHBoxLayout()
        self.scan_button = QPushButton("Scan")
        self.connect_button = QPushButton("Connect")
        self.disconnect_button = QPushButton("Disconnect")
        self.get_all_button = QPushButton("Get All")
        self.save_button = QPushButton("Save to Flash")
        right.addWidget(self.scan_button)
        right.addWidget(self.connect_button)
        right.addWidget(self.disconnect_button)
        right.addWidget(self.get_all_button)
        right.addWidget(self.save_button)

        header_layout.addLayout(left)
        header_layout.addStretch()
        header_layout.addLayout(right)

        info_row = QHBoxLayout()
        self.status_card = self._make_card("Status", "Disconnected")
        self.port_card = self._make_card("Port", "Not connected")
        self.serial_card = self._make_card("Target Serial ID", EXPECTED_SERIAL_NUMBER)
        info_row.addWidget(self.status_card)
        info_row.addWidget(self.port_card)
        info_row.addWidget(self.serial_card)

        self.device_table = QTableWidget(0, 5)
        self.device_table.setHorizontalHeaderLabels(["Port", "Description", "Product", "Serial", "Match"])
        self.device_table.horizontalHeader().setStretchLastSection(True)
        self.device_table.verticalHeader().setVisible(False)
        self.device_table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.device_table.setSelectionBehavior(QTableWidget.SelectRows)

        binds_panel = QFrame()
        binds_panel.setObjectName("Panel")
        binds_layout = QVBoxLayout(binds_panel)
        binds_title = QLabel("Button Reprogramming")
        binds_title.setObjectName("Title")
        binds_help = QLabel("Examples: CTRL C, SHIFT ALT T, VOLUP, PLAY, F5")
        binds_help.setObjectName("Muted")
        binds_layout.addWidget(binds_title)
        binds_layout.addWidget(binds_help)

        grid = QGridLayout()
        grid.addWidget(QLabel("Button"), 0, 0)
        grid.addWidget(QLabel("Binding"), 0, 1)
        grid.addWidget(QLabel("Actions"), 0, 2)

        for button in range(MAX_BUTTONS):
            label = QLabel(f"Button {button}")
            line_edit = QLineEdit()
            line_edit.setPlaceholderText("Example: CTRL SHIFT ESC or VOLUP")
            get_button = QPushButton("Get")
            set_button = QPushButton("Set")

            get_button.clicked.connect(lambda checked=False, b=button: self._get_button_binding(b))
            set_button.clicked.connect(lambda checked=False, b=button: self._set_button_binding(b))

            self.binding_inputs.append(line_edit)
            self.get_buttons.append(get_button)
            self.set_buttons.append(set_button)

            action_row = QHBoxLayout()
            action_row.addWidget(get_button)
            action_row.addWidget(set_button)
            action_widget = QWidget()
            action_widget.setLayout(action_row)

            grid.addWidget(label, button + 1, 0)
            grid.addWidget(line_edit, button + 1, 1)
            grid.addWidget(action_widget, button + 1, 2)

        binds_layout.addLayout(grid)

        self.log_box = QTextEdit()
        self.log_box.setReadOnly(True)
        self.log_box.setPlaceholderText("Device log and command responses appear here...")

        root.addWidget(header)
        root.addLayout(info_row)
        root.addWidget(self.device_table, 2)
        root.addWidget(binds_panel, 3)
        root.addWidget(self.log_box, 2)

        self.scan_button.clicked.connect(self._refresh_device_info)
        self.connect_button.clicked.connect(self._connect_device)
        self.disconnect_button.clicked.connect(self._disconnect_device)
        self.get_all_button.clicked.connect(self._get_all_bindings)
        self.save_button.clicked.connect(self._save_config)

    def _make_card(self, title: str, value: str) -> QFrame:
        card = QFrame()
        card.setObjectName("Card")
        layout = QVBoxLayout(card)
        title_label = QLabel(title)
        title_label.setObjectName("Muted")
        value_label = QLabel(value)
        value_label.setStyleSheet("font-size: 18px; font-weight: 700;")
        layout.addWidget(title_label)
        layout.addWidget(value_label)
        layout.addStretch()
        card.value_label = value_label
        return card

    def _refresh_device_info(self) -> None:
        devices = self.device_service.scan_ports()
        self.device_table.setRowCount(len(devices))
        matched_port = ""

        for row, device in enumerate(devices):
            match = "YES" if device.serial_number == EXPECTED_SERIAL_NUMBER else "NO"
            if match == "YES":
                matched_port = device.port_name
            values = [device.port_name, device.description, device.product, device.serial_number, match]
            for col, value in enumerate(values):
                item = QTableWidgetItem(value)
                item.setTextAlignment(Qt.AlignCenter)
                self.device_table.setItem(row, col, item)

        self.current_port = matched_port
        if matched_port:
            self.status_card.value_label.setText("Device found")
            self.port_card.value_label.setText(matched_port)
            self._log(f"Detected target device on {matched_port} using serial number {EXPECTED_SERIAL_NUMBER}")
        else:
            if not self.serial_service.is_connected():
                self.status_card.value_label.setText("Device not found")
                self.port_card.value_label.setText("Not connected")
            self._log("Target serial number not found during scan.")

        self._update_connection_state()

    def _connect_device(self) -> None:
        if not self.current_port:
            QMessageBox.warning(self, "No Device", "No STM32 MacroPad with serial ID VOID001 was found.")
            return

        ok, response = self.device_service.verify_ping(self.current_port)
        self._log(f"PING response: {response}")
        if not ok:
            QMessageBox.warning(self, "Handshake Failed", f"Unexpected handshake response: {response}")
            return

        try:
            self.serial_service.connect(self.current_port)
            self.status_card.value_label.setText("Connected")
            self.port_card.value_label.setText(self.current_port)
            self._log(f"Connected to {self.current_port}")
        except Exception as exc:
            QMessageBox.critical(self, "Connection Error", str(exc))
            self._log(f"Connection error: {exc}")

        self._update_connection_state()

    def _disconnect_device(self) -> None:
        self.serial_service.disconnect()
        self.status_card.value_label.setText("Disconnected")
        self._log("Disconnected from device.")
        self._update_connection_state()

    def _get_button_binding(self, button: int) -> None:
        if not self.serial_service.is_connected():
            QMessageBox.warning(self, "Not Connected", "Connect to the device first.")
            return

        try:
            command = self.config_service.build_get_command(button)
            response = self.serial_service.send_and_readline(command)
            ok, parsed = self.config_service.parse_get_response(response)
            self._log(f"> {command.strip()}")
            self._log(f"< {response.strip()}")
            if ok:
                self.binding_inputs[button].setText(parsed)
            else:
                QMessageBox.warning(self, "GET Failed", parsed)
        except Exception as exc:
            QMessageBox.critical(self, "Error", str(exc))

    def _set_button_binding(self, button: int) -> None:
        if not self.serial_service.is_connected():
            QMessageBox.warning(self, "Not Connected", "Connect to the device first.")
            return

        raw_text = self.binding_inputs[button].text()
        valid, result = self.config_service.validate_binding(raw_text)
        if not valid:
            QMessageBox.warning(self, "Validation Error", result)
            return

        try:
            command = self.config_service.build_set_command(button, result)
            response = self.serial_service.send_and_readline(command)
            ok, parsed = self.config_service.parse_set_response(response)
            self._log(f"> {command.strip()}")
            self._log(f"< {response.strip()}")
            if ok:
                QMessageBox.information(self, "Success", f"Button {button} updated successfully.")
            else:
                QMessageBox.warning(self, "SET Failed", parsed)
        except Exception as exc:
            QMessageBox.critical(self, "Error", str(exc))

    def _save_config(self) -> None:
        if not self.serial_service.is_connected():
            QMessageBox.warning(self, "Not Connected", "Connect to the device first.")
            return

        try:
            command = self.config_service.build_save_command()
            response = self.serial_service.send_and_readline(command)
            ok, parsed = self.config_service.parse_save_response(response)
            self._log(f"> {command.strip()}")
            self._log(f"< {response.strip()}")
            if ok:
                QMessageBox.information(self, "Saved", "Configuration saved to flash.")
            else:
                QMessageBox.warning(self, "Save Failed", parsed)
        except Exception as exc:
            QMessageBox.critical(self, "Error", str(exc))

    def _get_all_bindings(self) -> None:
        if not self.serial_service.is_connected():
            QMessageBox.warning(self, "Not Connected", "Connect to the device first.")
            return

        try:
            command = self.config_service.build_getall_command()
            response = self.serial_service.send_and_read_config_block(command)
            self._log(f"> {command.strip()}")
            self._log(response.strip())
            lines = self.config_service.extract_getall_lines(response)
            for line in lines:
                if line.startswith("KEY:"):
                    parts = line.split(":", 2)
                    if len(parts) == 3 and parts[1].isdigit():
                        button = int(parts[1])
                        if 0 <= button < len(self.binding_inputs):
                            self.binding_inputs[button].setText(parts[2])
        except Exception as exc:
            QMessageBox.critical(self, "Error", str(exc))

    def _update_connection_state(self) -> None:
        connected = self.serial_service.is_connected()
        self.connect_button.setEnabled(bool(self.current_port) and not connected)
        self.disconnect_button.setEnabled(connected)
        self.get_all_button.setEnabled(connected)
        self.save_button.setEnabled(connected)
        for button in self.get_buttons:
            button.setEnabled(connected)
        for button in self.set_buttons:
            button.setEnabled(connected)

    def _log(self, text: str) -> None:
        self.log_box.append(text)
