# RandeChistli 🌡️ 🔥

A temperature monitoring system for forest cabins and off-grid living environments. RandeChistli warns you when it gets too cold and you need to add wood to the fire!

<p align="center">
  <img src="https://raw.githubusercontent.com/dusjagr/RandeChistli/main/images/randechistli_logo.png" alt="RandeChistli Logo" width="300"/>
</p>

## Overview

RandeChistli (Swiss German for "Forest Temperature Monitor") is a specialized temperature monitoring system developed at [RandeLab](http://www.randelab.ch/) - a forest-geeking and residency place located on Hagen in Switzerland. 

This project provides both visual and audio alerts when temperatures drop below configurable thresholds, making it perfect for:
- Forest cabins with wood heating
- Off-grid living environments
- Workshop temperature monitoring
- Greenhouse monitoring
- Any environment where temperature drops need to be detected

## Features

- **Visual Feedback**: Uses 8 NeoPixel RGB LEDs to display temperature and status
- **Audio Alerts**: Plays warning tones when temperature falls below threshold
- **Configurable Thresholds**: Set your own temperature warnings
- **Low Power**: Built on ATtiny85 for minimal power consumption
- **Startup Animation**: Engaging rainbow animation and melody on startup
- **Temperature-Based Color Mapping**: 
  - Cold temperatures: Blue shades
  - Medium temperatures: Green shades
  - High temperatures: Red/Yellow shades

## Hardware Requirements

- ATtiny85 microcontroller
- DS18B20/DS18S20 OneWire temperature sensor
- 8 NeoPixel/WS2812B RGB LEDs
- Small speaker or piezo buzzer
- 4.7K resistor (pull-up for OneWire)
- Power supply (3-5V)
- Breadboard or custom PCB

## Wiring Diagram

```
ATtiny85     Components
--------     ----------
PB4    ------| DS18B20 Temperature Sensor (with 4.7K pull-up to VCC)
Pin 0  ------| NeoPixel Data In
Pin 1  ------| Speaker/Buzzer
VCC    ------| Power (3-5V)
GND    ------| Ground
```

## Software Dependencies

- [PlatformIO](https://platformio.org/) environment
- [OneWire](https://github.com/PaulStoffregen/OneWire) library
- [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) library

## Installation

### Using PlatformIO

1. Clone this repository:
   ```
   git clone https://github.com/dusjagr/RandeChistli.git
   ```

2. Open the project in PlatformIO:
   ```
   cd RandeChistli
   platformio open
   ```

3. Build and upload to your ATtiny85:
   ```
   platformio run --target upload
   ```

### Manual Installation

1. Install the required libraries in the Arduino IDE
2. Copy the source files to your Arduino project
3. Configure your board settings for ATtiny85
4. Upload to your device

## Configuration

The main parameters can be adjusted in the code:

```cpp
#define ONEWIREPIN   PB4     // Temperature sensor pin
#define NEOPIXELPIN    0     // NeoPixel data pin
#define NUMPIXELS      8     // Number of NeoPixels
#define SPEAKERPIN     1     // Speaker pin

#define hell          25     // Brightness level
#define lowTemp        4     // Low temperature threshold (°C)
#define maxTemp       30     // Maximum temperature range (°C)
#define warnTemp      5      // Warning temperature threshold (°C)
#define warnAlarm     2      // How often it beeps when below warnTemp
```

## About RandeLab

RandeLab is a creative forest-geeking space and residency located in Switzerland. Known as the "Center for Alternative Coconut Research", it provides a natural environment for technology experimentation and creative projects. 

Visit [www.randelab.ch](http://www.randelab.ch/) to learn more about their projects and initiatives.

## License

Open source hardware project - free to use, modify and distribute with attribution.

## Credits

Developed by Marc Dusseiller (dusjagr) at RandeLab.
