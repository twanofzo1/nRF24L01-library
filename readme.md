# NRF24L01+ Wireless Transceiver Library

A C++ library for the NRF24L01+ 2.4GHz wireless transceiver, written for the Arduino IDE and PlatformIO ecosystem.

This library was developed as part of a school project where multiple ICs could be chosen.  
I deliberately selected the NRF24L01+ as it was considered the most challenging option and not recommended due to its complexity and timing-sensitive behavior.

The goal of this project was to gain a deep understanding of SPI communication, RF configuration, and low-level driver development on microcontrollers.

---

## Features

- Low-level NRF24L01+ driver written from scratch
- SPI-based communication (no dependency on existing NRF libraries)
- Configurable:
  - Data rate
  - RF channel
  - TX/RX addresses
  - Payload size
- Transmit and receive support
- Blocking and non-blocking operation
- Designed for clarity and correctness rather than abstraction-heavy APIs

---

## Supported Platforms

- Arduino-compatible boards (AVR, ESP32, etc.)
- PlatformIO
- Any MCU with SPI support (with minimal adaptation)

---

## Motivation

Most available NRF24L01 libraries abstract away the internal workings of the chip.
For this project, the focus was on:

- Understanding the NRF24L01+ register map
- Correct SPI transaction timing
- Handling TX/RX states and FIFOs
- Writing a reusable driver instead of application-specific code

This resulted in a clean and readable implementation that prioritizes learning and correctness.

---

