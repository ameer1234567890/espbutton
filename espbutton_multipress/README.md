This is a multi-press button. There are two states of button press, short press and long press. Different HTTP(S) requests can be made upon short press or long press of the button. There is also a feedback added where the LED lights up when the long press interval has been reached, allowing the user to easily determine a long press.

This also has the possibility to add several levels of button presses such as 2 second press, 5 second press, etc... This can easily be achieved by adding a variable for each time interval and duplicating the long press routine with the new variable.

_Note: ESP8266's internal pullup resistors are often not strong enough for certain environments. You might need to consider using an external pullup resistor, for pulling up the button._
