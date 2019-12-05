ProductivityOpen P1AM-100 [![Build Status](https://travis-ci.org/facts-engineering/P1AM.svg?branch=master)](https://travis-ci.org/facts-engineering/P1AM)
============================

The ProductivityOpen P1AM-100 is an automation platform compatible with Productivity 1000 Series I/O modules, P1AM Series shields, and Arduino MKR format shields. It can be programmed using the Arduino IDE.

Productivity Series modules offer several types of industrial grade I/O 
 - Analog Inputs 
 - Analog Outputs
 - Temperature Inputs
 - Discrete Inputs
 - Discrete Outputs and Relays
 - Specialty Modules

P1000 Series Modules can be found here on the [Automation Direct Webstore](https://www.automationdirect.com/adc/shopping/catalog/programmable_controllers/productivity_series_controllers/productivity1000_(stackable_micro_plc))

Information on using P1000 Series Modules with the P1AM-100 can be found on the reference page here: 
[P1AM-100 Wiki](https://github.com/facts-engineering/P1AM/wiki)

# P1AM-100 Installation #
*Install the Arduino IDE version 1.8.7 or later. We recommend the current version on the [Arduino website](https://www.arduino.cc/en/main/software).
After you have installed the Arduino IDE,You will need to follow one of the library install methods and the boards manager install instructions to get started with the P1AM-100* 

## P1AM-100 Library Install ##

**Install From Library Manager** 
- In the Arduino IDE go to Sketch -> Include Library -> Manage Libraries
- Type P1AM into the search box
- Click Install

**Install From Zip**
- Click the green button that says "Clone or download"
- Select "Download ZIP"
- In the Arduino IDE go to Sketch -> Include Library -> Add .ZIP Library
- Navigate to the ZIP file that you downloaded in the window that opens

## Boards Manager Install ##
- Start Arduino and open the Preferences window.
- Enter ```CREATE LINK HERE``` into the *Additional Board Manager URLs* field. You can add multiple URLs, separating them with commas.
- Open Boards Manager from Tools > Board menu and install the *P1AM* platform 
- Select Tools -> Board -> P1AM-100

Boards manager link: `SAME LINK AS ABOVE`

## Driver Installation ##
For Windows 7 and 8 it is required that you install a device driver for the P1AM-100
[Download driver installer here:](LINK TO DRIVER DOWNLOAD FROM ?raw=true)

___________________________________________________________________________
# Getting Started #
The P1AM-100 library comes packed with examples to help you get started.
The examples can be found in the Arduino IDE under:
File -> Examples -> P1AM-100 

We recommend going through each example that works with your modules to get a better understanding of the API


# Useful Links #
[ProBlocks Graphical Programming from ADC](LINK HERE)

[Automation Direct Webstore](https://www.automationdirect.com/adc/home/home)

[Arduino Getting Started](https://www.arduino.cc/en/Guide/HomePage)

[Arduino Programming](https://www.arduino.cc/en/Guide/HomePage)

