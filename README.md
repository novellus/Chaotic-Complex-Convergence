Chaotic-Complex-Convergence
===========================

Generates a GUI of the set of points in the complex plane that converge under iteration of a function.

You'll want to run this on a linux command line with a frame buffer.
I made a bash script that compiles the program, turns off any blinking cursor, and executes the resulting executable.

In chaos.c, there is a section of user definables.
You'll want to set the xSize and ySize to match the size of your frame buffer. You can also set the complex function there.

You will know pretty much right away if your frame buffer is working (the screen will go white when the program starts). It will take a couple minutes before some of the pixels start converging and turn black.
Naturally, the longer it runs the more accurate it gets the pixel diagram.

PS: There is a hidden "feature", since the command line is still active behind the scenes, hitting space while the program is running will show the cursor, but it will disappear when the frame buffer updates again. Gives me an idea of how fast its going.
