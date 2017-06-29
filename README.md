Z-AXIS STEPPER-TO-SERVO ADAPTER
===============================

Description
-----------

Simulates an axis, drives RC Servo.

Want to use a servo in GRBL for the Z axis, but don't want to change
your firmware?

This quick hack is designed as a plug-in replacement for a stepper
driver such as the easydriver or Pololu driver. It reads the DIR and
STEP signals and converts an internal step counter into PWM for an
RC servo.

Great for use in existing hardware. No fundamental changes in firmware
are needed.


See it in action
----------------

* [YouTube - Micro CNC plotter made from old DVD drives (Arduino/grbl)](https://youtu.be/HlBhCY8luEI)
* [YouTube - Micro CNC plotter made from old DVD drives plots a floral ornament](https://youtu.be/K-D9JYRs5zM)



Pins/Schematic
--------------

```
          ______
   RESET-|      |-+5V
  HOME_Z-|ATTiny|-STEP
        -|25/85 |-DIR
     GND-|______|-Servo signal
```

Connect the parts to the respective pins. Set step direction to
inverted in your Machine.


Fuses
-----

The fuses of your ATTiny should be programmed to the following values:

* lfuse: 0xe2
* hfuse: 0xdf

Using avrdude, you can set the fuses with the following command line (assuming
an ATTiny25 connected via an Atmel AN910 compatible programmer):

```
avrdude -c avr910 -P /dev/ttyUSB0 -b115200 -pt25 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m
```

(adjust -c [programmer type] -P [COM port] -b [baud rate] to your setup)


License/Copyright
-----------------

Original code published 2014 by NilsRoe on Thingiverse
(http://www.thingiverse.com/thing:224969)

Extended by Jan Reucker 2017.

Licensed under Creative Commons - Attribution - Non-Commercial
(https://creativecommons.org/licenses/by-nc/3.0/legalcode)


