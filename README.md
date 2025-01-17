# PacMan-ishU

My variation on PacMan translated to C to run on the Wii U.

This is a variation on PacMan that I wrote a number of years ago. My then 8 year old son designed the screen and the images and told me how he wanted the game to play. I then wrote the game in Java. As a programming exercise I've now converted it to C, to run on the Wii U under Homebrew (https://wiiubrew.org). It uses the OSScreen interfaces for the graphics and sndcore2 for the sound.

The images used by Draw.c have been converted from bitmap files into two dimensional arrays in .h files, so that they can easily be incorporated into the software. This is done using the tool https://github.com/MartinButlerAAA/ProcessBmp.

The Sounds.c code has been developed using https://github.com/ashquarky/LiveSynthesisU as an example. The code uses .wav sound files that have been converted to data arrays in .h files (I have borrowed the sounds I used from https://scratch.mit.edu/). The .wav files are converted to .h files using the tool https://github.com/MartinButlerAAA/ProcessWav.

The game is not very advanced, but it is my first game written for the Wii U.
