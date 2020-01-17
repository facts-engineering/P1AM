ProductivityOpen P1AM-100 [![Build Status](https://travis-ci.org/facts-engineering/P1AM.svg?branch=master)](https://travis-ci.org/facts-engineering/P1AM)
============================

The ProductivityOpen P1AM-100 is an automation platform compatible with Productivity 1000 Series I/O modules, P1AM Series shields, and Arduino MKR format shields. It can be programmed using the Arduino IDE. The board uses the SAMD21G18 Microcontroller like the Arduino MKRZERO and other similar boards.

In depth information on the P1AM family and Productivity 1000 Modules can be found on the reference page here: 
[P1AM-100 Documentation](https://facts-engineering.github.io/)

Productivity Series modules offer several types of industrial grade I/O 
 - Analog and Temperature Inputs 
 - Analog Outputs
 - Discrete Inputs
 - Discrete Outputs and Relays
 - Specialty Modules

The P1AM Library provides a simple interface for controlling P1000 Modules
```C++
P1.writeDiscrete(HIGH,1,2);  //Turn slot 1 channel 2 on

float temperature = P1.readTemperature(2, 3);  //Return temperature read from slot 2 channel 3
```

The P1AM-100 can be purchased on the [Automation Direct Webstore](https://www.automationdirect.com/adc/shopping/catalog/programmable_controllers/open_source_controllers_(arduino-compatible)/productivityopen_(arduino-compatible)/controllers_-a-_shields/p1am-100)

Productivity 1000 Series Modules can be purchased on the [Automation Direct Webstore](https://www.automationdirect.com/adc/shopping/catalog/programmable_controllers/productivity_series_controllers/productivity1000_(stackable_micro_plc))

# P1AM-100 Installation #
>Install the Arduino IDE version 1.8.7 or later. We recommend the current version on the [Arduino website](https://www.arduino.cc/en/main/software).
After you have installed the Arduino IDE you will need to follow one of the library install methods and the boards manager install instructions to get started with the P1AM-100 

### P1AM-100 Library Install ###

**Install From Library Manager** 
- In the Arduino IDE go to `Sketch > Include Library > Manage Libraries`
- Type **P1AM** into the search box
- Click the install button in the P1AM library box

 ***OR***

**Install From Zip**
- Click the green button that says **Clone or download** on this repository page
- Select **Download ZIP**
- In the Arduino IDE go to `Sketch > Include Library > Add .ZIP Library`
- Navigate to the ZIP file that you downloaded in the window that opens

### Boards Manager Install ###
- Start Arduino and select `File > Preferences`
- Enter `https://raw.githubusercontent.com/facts-engineering/facts-engineering.github.io/master/package_productivity-P1AM-boardmanagermodule_index.json` into the *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open the Boards Manager from `Tools > Board > Boards Manager`
- Type **P1AM** into the search box and install the **P1AM-100** platform
- Select `Tools > Board > P1AM-100`
- Go to `Tools > Port` and select the COM Port your P1AM-100 is plugged into

Boards manager link: `https://raw.githubusercontent.com/facts-engineering/facts-engineering.github.io/master/package_productivity-P1AM-boardmanagermodule_index.json`

### Driver Installation ###
For Windows 7 and 8 it is required that you install a device driver for the P1AM-100
[Download driver installer here](https://github.com/facts-engineering/P1AMCore/raw/master/drivers.zip)


# Getting Started #
The P1AM-100 library comes packed with examples to help you get started.
The examples can be found in the Arduino IDE under `File > Examples > P1AM-100`

The **Basic** category contains the best examples to get started with the library and P1000 Modules.
The other categories contain examples that further explore the functionailty of the P1AM-100.

# Base Controller #
The P1AM Base Controller is the chip that directs communcations between the SAMD21 and the P1000 Modules. A P1000 Series power supply or external 24V supply is required to power the Base Controller and modules. The communications with the SAMD21 are SPI based and use 5 total pins. Pins 8, 9 and 10 can be shared with other SPI devices. **A3 and A4 must not be used on any shield if using the base controller functionality of the P1AM-100**

| Pin | Function |
|:---:|:--------:|
|  8  |   MOSI   |
|  9  |   CLK    |
|  10 |   MISO   |
|  A3 |      CS  |
|  A4 |    ACK   |


# Shields #

The P1AM-100 uses the Arduino MKR format header. The pins are 3.3V tolerant only, so do not apply any 5V signals. 

Industrial rated shields for the P1AM-100 can be purchased from [Automation Direct here](https://www.automationdirect.com/adc/shopping/catalog/programmable_controllers/open_source_controllers_(arduino-compatible)/productivityopen_(arduino-compatible)/controllers_-a-_shields/).

Please read through the Base Controller section above to determine if your shield pinout will work properly. For example, the Arduino MKR RGB Shield is not compatible with the P1AM-100 as it uses pins A3 and A4. 



# Additional Resources #
[ProductivityBlocks Graphical Programming from Automation Direct](https://github.com/AutomationDirect/ProductivityBlocks)

[Automation Direct P1AM Customer Forum](http://go2adc.com/p1am)

[Automation Direct Webstore](https://www.automationdirect.com/adc/home/home)

[Arduino Getting Started](https://www.arduino.cc/en/Guide/HomePage)

[Arduino Forum](https://forum.arduino.cc/index.php)


