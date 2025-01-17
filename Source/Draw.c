// 
// Functions to draw on OSScreen for graphics.
// The header files used with drawImage were created with the ProcessBmp tool (see https://github.com/MartinButlerAAA/ProcessBmp).
#include <stdlib.h>				// for abs
#include <coreinit/screen.h>	// for OSScreen

#include "Draw.h"				// Header for graphics drawing
#include "Game.h"				// For directions

// Function to display an bitmap image that has been converted to an array of pixels (separate header file).
// direction allows the sprite to be rotated by 90 degrees, the default is to face right (0 or RIGHT).
// The directions have been taken from Game.h.
bool drawImage(unsigned int ImageP[SQY][SQX], unsigned int xpos, unsigned int ypos, unsigned int direction)
{
	// Exit if the positions passed in are off the screen.
	if (((xpos + SQX) >= 1280) || ((ypos + SQY) >= 720))
	{
		return false;
	}

	// Use the direction to allow the image to face left, right, up or down (default is right).
	// This primarily to allow PacMan to face in the direction of movement.
	// The pixels in the bit map file start at the bottom but screen y 0 starts at the top of the screen.
	if (direction == UP)
	{
		for (unsigned int y = 0; y < SQY; y++)
		{
			for (unsigned int x = 0; x < SQX; x++)
			{
				// Only display the pixel if it is not the background screen colour. This gives sprites a transparent background.
				// x and y are swapped here so that the image faces up.
				if (ImageP[x][y] != BKGNDCLR)
				{
					OSScreenPutPixelEx(SCREEN_TV, xpos + x, ypos + SQY - y - 1, ImageP[x][y]);
				}
			}
		}
	}
	else if (direction == DOWN)
	{
		for (unsigned int y = 0; y < SQY; y++)
		{
			for (unsigned int x = 0; x < SQX; x++)
			{
				// Only display the pixel if it is not the background screen colour. This gives sprites a transparent background.
				// x and y are swapped here, and the y count is reversed so that the image faces down.
				if (ImageP[x][SQY - y - 1] != BKGNDCLR)
				{
					// The pixels in the bit map file start at the bottom but screen y 0 starts at the top of the screen.
					OSScreenPutPixelEx(SCREEN_TV, xpos + x, ypos + SQY - y - 1, ImageP[x][SQY - y - 1]);
				}
			}
		}
	}
	else if ( direction == LEFT)
	{ 
		for (unsigned int y = 0; y < SQY; y++)
		{
			for (unsigned int x = 0; x < SQX; x++)
			{
				// Only display the pixel if it is not the background screen colour. This gives sprites a transparent background.
				// The x count is reversed so that the image faces left.
				if (ImageP[y][SQX -	x - 1] != BKGNDCLR)
				{
					// The pixels in the bit map file start at the bottom but screen y 0 starts at the top of the screen.
					OSScreenPutPixelEx(SCREEN_TV, xpos + x, ypos + SQY - y - 1, ImageP[y][SQX - x - 1]);
				}
			}
		}
	}
	// The default is to point right, so that if no direction is passed in (0 or any other value). The image is displayed as normal. 
	else
	{ 
		for (unsigned int y = 0; y < SQY; y++)
		{
			for (unsigned int x = 0; x < SQX; x++)
			{
				// Only display the pixel if it is not the background screen colour. This gives sprites a transparent background.
				if (ImageP[y][x] != BKGNDCLR)
				{
					// The pixels in the bit map file start at the bottom but screen y 0 starts at the top of the screen.
					OSScreenPutPixelEx(SCREEN_TV, xpos + x, ypos + SQY - y - 1, ImageP[y][x]);
				}
			}
		}
	}
	return true;
}

// Function to draw a straight line between start and end coordinates of the colour specified.
// Taken from an internet example, but modified to allow line to go in any direction.
bool drawLine(float x1, float y1, float x2, float y2, unsigned int colour)
{
	float dx;	// Difference on x axis.
	float dy;	// Difference on y axis.
	float len;	// Length of line.
	float x;	// Current x position.
	float y;	// Current y position.

	// Exit if the positions passed in are off the screen.
	if ((x1 >= 1280.0) || (y1 >= 720.0) || (x2 >= 1280.0) || (y2 >= 720.0))
	{
		return false;
	}
	if ((x1 < 0.0) || (y1 < 0.0) || (x2 < 0.0) || (y2 < 0.0))
	{
		return false;
	}

	// Calculate differences from start to end.
	dx = x2 - x1;
	dy = y2 - y1;

	// Set the length for the longer difference.
	if (abs(dx) >= abs(dy)) { len = abs(dx); }
	else					{ len = abs(dy); }

	// Divide by the length to give the step sizes for drawing. 
	dx = dx / len;
	dy = dy / len;

	// Set to the start of the line.
	x = x1;
	y = y1;

	// Count along the line length to draw the line.
	for (int i = 0; i <= len; i++)
	{
		OSScreenPutPixelEx(SCREEN_TV, x, y, colour);
		x = x + dx;
		y = y + dy;
	}
	return true;
}