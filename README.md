Super Quadagon
==========

A simple game for the Arduino + 8x8 LED display. I used it to get familiar with how to make those work together.

## Installation
Copy the contents of the 'libraries' folder into your Arduino installation's 'libraries' folder. This will be something like 'arduino-1.0.5/libraries'.
Then just open the .ino file in your Arduino IDE!

## Electronics
Check out the schematics at: https://upverter.com/KingstonMakerspace/e3012575a3353ac9/Super-Quadagon/ . I have not built the PCB yet, it's all on a breadboard right now.

## Use and Playing the Game
There are three buttons, which you should set up so they can be easily pressed. One button, labeled 'Left' int he schematics, moves a single pixel counter-clockwise around a ring near the center. The 'Right' button goes clockwise. 
As the wall closes inon you from outside, make sure your dot doesn't get hit, by keeping it in the unlit 'hole' of the wall.
Each level is faster than the one before it. 
If you mess up, game over, and your score (number of walls avoided) will display in flashing dots around the outside of the display.
Press the 'Reset' button to try again.
