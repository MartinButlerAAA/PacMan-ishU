// Header for drawing functions.
#pragma once

// For this to work well for a project all icons, sprites need to be the same size. C does not like passing in arrays without known sizes.
// For many games screens built from blocks of the same size works well. The following define the size of the sprites and therefore image arrays.
#define SQX 32
#define SQY 32

// The background colour is defined. Pixels in images that match the background colour are not processed in effect giving sprites a transparent background.
#define BKGNDCLR 0x00000000

// Function to display an bitmap image that has been converted to an array of pixels (separate header file).
// direction allows the sprite to be rotated by 90 degrees, the default is to face right (0 or RIGHT).
// The directions have been taken from Game.h.
bool drawImage(unsigned int ImageP[SQY][SQX], unsigned int xpos, unsigned int ypos, unsigned int direction);

// Function to draw a straight line between start and end coordinates of the colour specified.
bool drawLine(float x1, float y1, float x2, float y2, unsigned int colour);
