# MotorSkarsta
Modify an Ikea Skarsta to be an automatic standing desk.

## Introduction
I bought an Ikea Skarsta table and saw that the internal manual mechanism could be easily converted to a motorized standing desk with a powerful enough motor. The basic operating principle is that the desk moves up and down controlled by two buttons.

## Features
* Moves up and down by control of two buttons
* Motor shuts off when bounds (min/max) of desk are reached
* Save current height by pressing up button 5 times
* Move to saved height by pushing both up and down

## Parts
* Arduino Microcontroller (UNO/Pro Micro/Whatever Works)
* BTS7960 H-Bridge
* 2 x Momentary Push Button
* Dupont Connectors
* 12V 5A Power Supply (One with barrel to terminal adapter made for leds to make life easier)
* LM2596 Step-Down Converter **(Optional)**
* HC-SR04 Ultrasonic Sensor **(Optional)**
* Spare Wire **(Optional)**
* Some kind of random project box to hide electronics
* Some kind of mount for buttons
