# STM32 USB Macropad

A configurable USB macropad built using the STM32F446RE and TinyUSB.

The device operates as a composite USB HID + CDC device, allowing runtime key remapping through a Python desktop configurator while storing settings persistently in internal flash memory.

## Features

* 4 Programmable Buttons
* USB HID Keyboard Support
* USB Consumer Control Support

  * Volume Up
  * Volume Down
  * Mute
  * Media Playback Controls
* USB CDC Configuration Interface
* Runtime Key Remapping
* Persistent Flash Storage
* Multiple Key Combinations
* Python Desktop Configurator
* Automatic Device Detection Using USB Serial Number

## Hardware

### Controller

* STM32F446RE Nucleo Board

### Inputs

* 4 Tactile Push Buttons

### USB Interfaces

* HID Keyboard
* CDC Serial

## Firmware Features

* TinyUSB Composite Device
* HID Abstraction Layer
* Button Debouncing and Event Handling
* Flash-Based Configuration Storage
* Runtime Configuration Through CDC Commands
* Consumer Control Support
* Configuration Validation Using Magic Numbers and Versioning

## Supported Commands

### SET

Assign a keybind to a button.

```text
SET 0 CTRL SHIFT A
SET 1 VOLUP
```

### GET

Retrieve the configuration of a specific button.

```text
GET 0
```

### GETALL

Retrieve the complete configuration.

Example response:

```text
CONFIG_START
KEY:0:CTRL+A
KEY:1:VOLUP
KEY:2:ALT+TAB
KEY:3:MUTE
CONFIG_END
```

### SAVE

Store the current configuration in STM32 flash memory.

```text
SAVE
```

### PING

Verify communication with the device.

```text
PING
```

## Desktop Configurator

The Python configurator provides:

* Device Discovery
* USB Connection Management
* Key Assignment
* Configuration Retrieval
* Configuration Storage
* Automatic Device Detection

## Software Stack

### Firmware

* STM32CubeIDE
* STM32 HAL
* TinyUSB

### Desktop Application

* Python
* PySerial

## Project Structure

```text
Firmware/
├── Core/
├── Drivers/
├── USB/
├── cmd_parser.c
├── storage.c
├── hid_engine.c
├── cdc_handler.c
└── ...

Configurator/
├── main.py
├── services/
├── ui/
└── ...
```

## Example Usage

Assign:

```text
Button 0 -> CTRL + SHIFT + A
Button 1 -> Volume Up
Button 2 -> ALT + TAB
Button 3 -> Mute
```

Save configuration:

```text
SAVE
```

The configuration is stored in internal flash memory and persists after power cycling.

Current functionality:

* HID Keyboard Support
* Consumer Control Support
* Flash Persistence
* USB CDC Configuration
* Python Desktop Configurator
* Runtime Key Remapping

