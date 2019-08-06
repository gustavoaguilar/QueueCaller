 
# QueueCaller

## Introduction

This project consists in a queue caller, it’s a display that call the client's order numbers, used in restaurants. The MCU receives the number via serial and display
it, with the functionality of beeping and blinking. You can see a demonstration in this [video](https://youtu.be/PtiIMsICWHU).

## Materials

The materials used are:

* Arduino Uno Rev 3
* 7Seg Display with 4 digits - SH...
* MAX7219 LED Driver
* Buzzer
* 22K Resistor
* Jumpers
* Breadboard

## Circuit

Don't forget to consult the datasheet of your components to get the right pinout and intructions.

Schematic:
![alt text](https://github.com/gustavoaguilar/QueueCaller/blob/master/doc/schematic.png)

My circuit:

![alt text](https://github.com/gustavoaguilar/QueueCaller/blob/master/doc/circuit.jpg)
Note: I didn’t had a 22K Ohm resistor, so I used 10K, 5.2K and 5.2K in series.

## Code

### Dependencies

Needed libraries:

* SPI

### Some Highlights
In this project I used the default SPI pins from the Arduino Uno, they are configured by the SPI library:

* MOSI - pin 11
* CLK - pin 13

This first section is where you may change the pins used according to your circuit:

```C
#define DISPLAY_CS 10
#define BUZZER_PIN 8
```

## Use
The MCU expects 5 bytes in the serial port, the first 4 are the digits from the number and the fifth is the ‘b’ "flag", when this exist(***order[4] == ‘b’***) the system will Blink and Beep the new number, else it will only update the number in the display.

You can configure your serial port with miniterm and then use this command to send data:

* echo 0000b > /dev/ttyACM0

Note: ‘0000’ can be replaced with any 4 digit number, and ‘b’ may exist or not.
