# Rotary Numeric Spinner

![License: MIT](https://img.shields.io/badge/Code-MIT-green.svg)
![Docs: CC BY 4.0](https://img.shields.io/badge/Docs-CC%20BY%204.0-blue.svg)
![Arduino](https://img.shields.io/badge/Arduino-Nano-00979D.svg)
![Wokwi](https://img.shields.io/badge/Simulator-Wokwi-purple.svg)

A Wokwi/Arduino rotary encoder numeric spinner project using an Arduino Nano, a 20x4 I2C LCD, and a KY-040 rotary encoder.

## Wokwi Simulation

Original Wokwi project:

https://wokwi.com/projects/441483281452613633

## Included Files

```text
rotary-numeric-spinner/
├── README.md
├── sketch.ino
├── diagram.json
├── libraries.txt
├── RotaryKeyboard.cpp
├── RotaryKeyboard.h
├── RotaryNumberSpinner.cpp
├── RotaryNumberSpinner.h
├── wokwi-project.txt
├── LICENSE-CODE.md
└── LICENSE-DOCS.md
```

## Features

- Rotary encoder numeric spinner control
- Range-based numeric selection
- List-based numeric selection
- 20x4 I2C LCD display
- Shared rotary keyboard component
- Wokwi simulation support

## Required Libraries

Install these from the Arduino Library Manager or include them in Wokwi `libraries.txt`:

```text
Encoder
LiquidCrystal I2C
EncoderButton
```

## Hardware

| Part | Quantity |
| --- | ---: |
| Arduino Nano | 1 |
| 20x4 I2C LCD | 1 |
| KY-040 Rotary Encoder | 1 |

## Current Pinout

| Arduino Nano Pin | Connected Device |
| --- | --- |
| A0 | Rotary encoder CLK |
| A1 | Rotary encoder DT |
| A2 | Rotary encoder SW |
| A4 | LCD SDA |
| A5 | LCD SCL |
| 5V | LCD/encoder VCC |
| GND | LCD/encoder GND |

## License

- Code license: MIT. See [LICENSE-CODE.md](LICENSE-CODE.md).
- Documentation and images: CC BY 4.0. See [LICENSE-DOCS.md](LICENSE-DOCS.md).

Copyright (c) 2026 Larry Brendel.
