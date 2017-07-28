# adafruit-Trellis_Push-and-Hold
functions for the adafruit trellis converting buttons to push and hold = three options per button

This ino file gives examples of creating push and hold buttons for the adafruit trellis button pad. With the functions you can set the time to create three options for when a button is pushed (or more if you extend the code). You can split the button pad between having some buttons as momentary push buttons and others on the same trellis as bting latching On/Off. 

The momentary buttons will act on a short press (<400ms), medium press (>400 <1500ms) and long press (>1500ms). Each of the press types will give access to a function that is associates with the button. That means three functions per momentary button. The led lights up as long as the button is held.

The ino also include a function to use pairs of buttons to create increases or decreases in a stored value. One button increases the value, the adjacent button decreases it.

The ino also provides simple array tables the stores that variable values for the momentary and latching buttons, meaning that there are no hidden variables. this provides easy access for manipulation.
