# RPC Protocol for Embedded Systems

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Compatible-green.svg)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Supported-orange.svg)

A lightweight Remote Procedure Call (RPC) protocol implementation for microcontrollers using FreeRTOS and UART communication.

## 🚀 Features

- **Layered Architecture**: Physical, Data Link, Transport, Application layers
- **FreeRTOS Integration**: Full RTOS support with tasks and queues
- **Error Detection**: CRC8 checksum validation
- **Timeout Handling**: Configurable response timeouts
- **Modular Design**: Easy to port to different hardware
- **Thread Safe**: Mutex-protected shared resources

## 📁 Project Structure
