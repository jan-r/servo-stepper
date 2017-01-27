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


License/Copyright
-----------------

Original code published 2014 by NilsRoe on Thingiverse
(http://www.thingiverse.com/thing:224969)

Extended by Jan Reucker 2017.

Licensed under Creative Commons - Attribution - Non-Commercial
(https://creativecommons.org/licenses/by-nc/3.0/legalcode)


