### Double Buffered Graphics Library

This project implements a graphics library for the TinyCore Linux VM run through
QEMU. No C standard library calls were used, other than for debugging purposes and
are commented out.


A test driver file "myDriver.c" allows the user to draw repeating 1px wide line rectangles
that alternate colors every 7 iterations. The rectangles continue to get smaller
until they reach the center of the screen which has been totally filled.

No C standard library calls were made in the driver program.

![Output Preview](/output.gif)
