# STM32 MacroPad Configurator

Simple PySide6 desktop GUI for your STM32 macro pad.

## What it does
- Detects the device using USB serial number `VOID001`
- Verifies it with `PING`
- Connects and disconnects
- Sends `GET`, `SET`, `SAVE`, and `GETALL`
- Validates key binding text before sending
- Lets you reprogram 4 buttons quickly

## Important firmware notes
This GUI was built from the command parser you provided.

Expected command patterns:
- `PING\n`
- `GET 0\n`
- `SET 0 CTRL C\n`
- `SAVE\r\n`
- `GETALL\r\n`

## Run
```bash
pip install -r requirements.txt
python main.py
```
