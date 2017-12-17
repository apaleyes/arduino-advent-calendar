# Simple advent calendar on Arduino

Idea of the calendar is: somehow find out the current day of month, assuming it is already December, and then light up as many leds  as there are days into the month. Note that this implementation counts down to New Year, so it uses 31 led.

Components used in the circuit:
1. Arduino Uno
2. 4 shift registers 74HC595
3. 31 3mm leds
4. 31 220 ohm resistors
5. Real time clock module DS1338
6. Lots of jumper wires

Credits:
* Circuit is pretty much taken from here: https://www.arduino.cc/en/Tutorial/ShiftOut, and then extended to 31 led.
* Code for handling shift registers is based on this article: http://www.instructables.com/id/Arduino-16-LEDs-using-two-74HC595-shift-registers-/.
