ProductivityOpen P1AM [![Build Status](https://github.com/facts-engineering/P1AM/actions/workflows/main.yml/badge.svg)](https://github.com/facts-engineering/P1AM/actions)
============================

### Product Description
The ProductivityOpen P1AM is an automation platform compatible with Productivity 1000 Series I/O modules, P1AM Series shields, and Arduino MKR format shields.
 - P1AM-100: The board uses the SAMD21G18 Microcontroller like the Arduino MKRZERO and other similar boards.
 - P1AM-200: The board uses the SAMD51P20 Microcontroller like the Adafruit Grand Central and other similar boards.

### Hardware - P1000 I/O Modules

Productivity Series modules offer several types of industrial grade I/O 
 - Analog and Temperature Inputs 
 - Analog Outputs
 - Discrete Inputs
 - Discrete Outputs and Relays
 - Specialty Modules

### Hardware - P1AM Shields
ProductivityOpen offers 3 types of industrial shields and a prototyping kit.
 - P1AM-ETH
 - P1AM-SERIAL
 - P1AM-GPIO
 - P1AM-PROTO

### Base Controller
The P1AM Base Controller is the chip that directs communcations between the microcontroller and the P1000 Modules. A P1000 Series power supply or external 24V supply is required to power the Base Controller and modules. 

The **P1AM-100** communications with the SAMD21 are SPI based and use 5 total pins. Pins 8, 9 and 10 can be shared with other SPI devices. **A3 and A4 must not be used on any shield if using the base controller functionality of the P1AM-100**

| Pin | Function |
|:---:|:--------:|
|  8  |   MOSI   |
|  9  |   CLK    |
|  10 |   MISO   |
|  A3 |      CS  |
|  A4 |    ACK   |

The **P1AM-200** Base Controller pins are all internal. No header pins are used for IO module communications.

In depth information on the P1AM family hardware can be found on the reference page here: [ProductivityOpen Documentation](https://facts-engineering.github.io/)

Hardware can be purchased on the [Automation Direct Webstore](https://www.automationdirect.com/adc/shopping/catalog/programmable_controllers/productivity_open_(arduino-compatible))

## P1AM Library

### Description

The P1AM Library provides a simple interface for controlling P1000 Modules.
```C++
P1.writeDiscrete(HIGH, 1, 2);  //Turn slot 1 channel 2 on

float temperature = P1.readTemperature(2, 3);  //Return temperature read from slot 2 channel 3
```
### Installing the Library

>Install the Arduino IDE version 2.0.0 or later. We recommend the current version on the [Arduino website](https://www.arduino.cc/en/main/software).
 
- Use Arduino's Library Manager to install the library. `Tools > Manage Libraries`
- Type **P1AM** into the search box and click the install button

### Installing the Board
- Start the Arduino IDE and select File > Preferences
- Enter `https://raw.githubusercontent.com/facts-engineering/facts-engineering.github.io/master/package_productivity-P1AM-boardmanagermodule_index.json` into the Additional Board Manager URLs field. You can add multiple URLs, separating them with commas.
- Use Arduino's Boards Manager to install the board. `Tools > Board > Boards Manager`
- Type **P1AM** into the search box and click the install button

### Getting Started
Documentation can be found on the [ProductivityOpen Documentation](https://facts-engineering.github.io/). The website has information that includes the hardware documentation, library API, and code snippets.

The P1AM library also comes packed with examples that can be found in the Arduino IDE under `File > Examples > P1AM`.

## Additional Resources
[ProductivityBlocks Graphical Programming from Automation Direct](https://github.com/AutomationDirect/ProductivityBlocks)

[Automation Direct P1AM Customer Forum](http://go2adc.com/p1am)

[Automation Direct Webstore](https://www.automationdirect.com/adc/home/home)

[Arduino Getting Started](https://www.arduino.cc/en/Guide/HomePage)

[Arduino Forum](https://forum.arduino.cc/index.php)

[Arduino Support Discord](https://discord.com/invite/jQJFwW7)
