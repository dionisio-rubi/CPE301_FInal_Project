# Swamp Cooler using Arduino

Group Name: Team PEARL


Team: Liam Prior, Lyssie Brown, Rubi Dionisio, and Alejandro Mozqueda

CPE 301 Spring 2023 at University of Nevada, Reno

## Overview of Design

With the Arduino 2560, the team built a working cooler with sensors provided in the Arduino kit. The project does the following: monitors the water levels in a cup and prints an alert when the water level is low, monitors and displays the current air temperature in Celsius and humidity on the LCD screen, starts and stops a fan motor as needed when the temperature falls out of a certain range, allows a person to adjust the angle of an output vent from the system, allows a person to enable or disable the system using a push button, and finally, records the time and date every time the motor is turned on  or off.

The following allowed libraries were used to create the program for the swamp cooler: uRTCLib.h, LiquidCrystal.h, dht.h. No other libraries were used, however, to aid the program, code used in the following previous labs for the CPE class were used: Timers, UARTs, GPIO, ADC. Certain states are represented in the following ways: DISABLED by a yellow LED in which it represents the circuit not in use, IDLE by a green light represents circuit working, ERROR by a red LED, RUNNING by a blue LED.

## Constraints
The circuit only works when the Arduino is plugged into a computer set up with the Arduino IDE. Additionally, two of the breadboards need to be connected to one external power source while the other also needs to be plugged into its own power source.