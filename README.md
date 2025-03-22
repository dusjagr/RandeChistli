# RandeChistli 🌡️ 🔥

A temperature monitoring system for forest cabins and off-grid living environments. RandeChistli warns you when it gets too cold and you need to add wood to the fire!

<p align="center">
  <img src="https://raw.githubusercontent.com/dusjagr/RandeChistli/main/images/randechistli_logo.png" alt="RandeChistli Logo" width="300"/>
</p>

## Overview

RandeChistli (Swiss German for "Forest Temperature Monitor") is a specialized temperature monitoring system developed at [RandeLab](http://www.randelab.ch/) - a forest-geeking and residency place located on Hagen in Switzerland. 

The project continues the heritage of the [Gär Lämpli (發酵小燈)](https://www.hackteria.org/wiki/G%C3%A4r_L%C3%A4mpli) - an open hardware temperature monitoring device originally designed for fermentation projects and creative circuit workshops.

This project provides both visual and audio alerts when temperatures drop below configurable thresholds, making it perfect for:
- Forest cabins with wood heating
- Off-grid living environments
- Workshop temperature monitoring
- Fermentation monitoring
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
- **Audio Bootloader Compatible**: Code can be uploaded using audio files (see Gär Lämpli wiki)

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

### Audio Bootloader Method

If your device is configured with the [NEO Coconut Audio Bootloader](https://github.com/8BitMixtape/8Bit-Mixtape-NEO/wiki), you can upload code using audio files. This method was used in the original Gär Lämpli project.

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

## Project History and Related Works

### Gär Lämpli - 發酵小燈

The RandeChistli builds upon the [Gär Lämpli](https://www.hackteria.org/wiki/G%C3%A4r_L%C3%A4mpli) (Fermentation Lamp) project, which was originally designed for monitoring fermentation temperatures. The project has several international editions including:

- **Hamamatsu Edition**: Created during workshops at [Take-Space / Fabalab Hamamatsu](http://www.take-space.com/) in Japan
- **Taipei Edition (起酵燈仔)**: Developed for workshops in Taiwan
- **Unagi Version**: A specialized version with custom PCB design

### Hardware Design

The original Gär Lämpli hardware designs include:

- PCB designs created in DIY-CAD workshops
- Gerber files and BOM available at [Kitspace.org](https://kitspace.org/boards/github.com/genericlab/unagi_gar-lampli/)
- Custom enclosures designed in OpenSCAD:
  - [OpenSCAD file](https://github.com/8BitMixtape/8Bit-Mixtape-NEO/blob/master/boards/DIMI-8/Unagi_chistli.scad)
  - [SVG files for laser cutting](https://github.com/8BitMixtape/8Bit-Mixtape-NEO/blob/master/boards/DIMI-8/Unagi_chistli_final.svg)

### NEO Coconut Platform

The project is powered by the [NEO Coconut platform](https://github.com/8BitMixtape/8Bit-Mixtape-NEO/wiki), which provides:

- Audio bootloader for programming via sound
- Common hardware framework for creative electronic projects
- Support for various sensors and outputs

## About RandeLab

RandeLab is a creative forest-geeking space and residency located in Switzerland. Known as the "Center for Alternative Coconut Research", it provides a natural environment for technology experimentation and creative projects. 

Visit [www.randelab.ch](http://www.randelab.ch/) to learn more about their projects and initiatives.

## Community & Resources

- [Hackteria Wiki](https://www.hackteria.org/): Community platform for open source biological art projects
- [GOSH Forum](https://forum.openhardware.science/t/diy-cad-creative-circuit-board-design-tool-for-gar-lampli/322/2): Discussions on the Open Source Hardware aspects of the project
- [8BitMixtape NEO Wiki](https://github.com/8BitMixtape/8Bit-Mixtape-NEO/wiki): Documentation for the underlying platform

## License

Open source hardware project - free to use, modify and distribute with attribution.

## Credits

Developed by Marc Dusseiller (dusjagr) at RandeLab, building upon the Gär Lämpli project and NEO Coconut platform created by the global open hardware community.
