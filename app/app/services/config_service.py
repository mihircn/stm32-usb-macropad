from app.core.constants import (
    GETALL_COMMAND,
    GET_COMMAND_TEMPLATE,
    MAX_BUTTONS,
    SAVE_COMMAND,
    SET_COMMAND_TEMPLATE,
    VALID_CONSUMER,
    VALID_KEYS,
    VALID_MODIFIERS,
)


class ConfigService:
    def validate_binding(self, text: str) -> tuple[bool, str]:
        cleaned = text.strip().upper()
        if not cleaned:
            return False, "Binding cannot be empty."

        tokens = cleaned.split()
        if len(tokens) > 7:
            return False, "Too many tokens. Max 7 is safe here."

        found_consumer = False
        found_keys = 0

        for token in tokens:
            if token in VALID_MODIFIERS:
                continue
            if token in VALID_CONSUMER:
                found_consumer = True
                continue
            if token in VALID_KEYS:
                found_keys += 1
                continue
            return False, f"Unknown token: {token}"

        if found_consumer and found_keys:
            return False, "Do not mix consumer and keyboard tokens in one binding."
        if found_keys > 6:
            return False, "Keyboard binding supports max 6 keys."

        return True, cleaned

    def build_set_command(self, button: int, binding: str) -> str:
        return SET_COMMAND_TEMPLATE.format(button=button, binding=binding)

    def build_get_command(self, button: int) -> str:
        return GET_COMMAND_TEMPLATE.format(button=button)

    def build_save_command(self) -> str:
        return SAVE_COMMAND

    def build_getall_command(self) -> str:
        return GETALL_COMMAND

    def parse_get_response(self, text: str) -> tuple[bool, str]:
        line = text.strip()
        if not line:
            return False, "Empty response"
        elif line in {"ERR", "BAD BUTTON", "UNKNOWN TOKEN"}:
            return False, line
        else:
            return True, line

    def parse_set_response(self, text: str) -> tuple[bool, str]:
        line = text.strip()
        return line == "OK", line

    def parse_save_response(self, text: str) -> tuple[bool, str]:
        line = text.strip()
        return line == "SAVED", line

    def extract_getall_lines(self, text: str) -> list[str]:
        lines = [line.strip() for line in text.splitlines() if line.strip()]
        if "CONFIG_START" in lines and "CONFIG_END" in lines:
            start = lines.index("CONFIG_START") + 1
            end = lines.index("CONFIG_END")
            return lines[start:end]
        return lines
