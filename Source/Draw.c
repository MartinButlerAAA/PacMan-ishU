// 
// Functions to draw on OSScreen for graphics.
// The header files used with drawImage were created with the ProcessBmp tool (see https://github.com/MartinButlerAAA/ProcessBmp).
#include <stdlib.h>				// for abs
#include <coreinit/screen.h>	// for OSScreen

#include "Draw.h"				// Header for graphics drawing
#include "Game.h"				// For directions

/**
 * 8x8 monochrome bitmap fonts for rendering
 * Author: Daniel Hepper <daniel@hepper.net>
 *
 * License: Public Domain
 *
 * Based on:
 * // Summary: font8x8.h
 * // 8x8 monochrome bitmap fonts for rendering
 * //
 * // Author:
 * //     Marcel Sondaar
 * //     International Business Machines (public domain VGA fonts)
 * //
 * // License:
 * //     Public Domain
 **/

 // Contains an 8x8 font map for unicode points U+0020 - U+007F (basic latin)
static char font8x8_basic[96][8] = {
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0020 (space)
{ 0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00},   // U+0021 (!)
{ 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0022 (")
{ 0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00},   // U+0023 (#)
{ 0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00},   // U+0024 ($)
{ 0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00},   // U+0025 (%)
{ 0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00},   // U+0026 (&)
{ 0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0027 (')
{ 0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00},   // U+0028 (()
{ 0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00},   // U+0029 ())
{ 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},   // U+002A (*)
{ 0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00},   // U+002B (+)
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+002C (,)
{ 0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00},   // U+002D (-)
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+002E (.)
{ 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00},   // U+002F (/)
{ 0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00},   // U+0030 (0)
{ 0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00},   // U+0031 (1)
{ 0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00},   // U+0032 (2)
{ 0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00},   // U+0033 (3)
{ 0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00},   // U+0034 (4)
{ 0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00},   // U+0035 (5)
{ 0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00},   // U+0036 (6)
{ 0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00},   // U+0037 (7)
{ 0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00},   // U+0038 (8)
{ 0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00},   // U+0039 (9)
{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00},   // U+003A (:)
{ 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06},   // U+003B (;)
{ 0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00},   // U+003C (<)
{ 0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00},   // U+003D (=)
{ 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00},   // U+003E (>)
{ 0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00},   // U+003F (?)
{ 0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00},   // U+0040 (@)
{ 0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00},   // U+0041 (A)
{ 0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00},   // U+0042 (B)
{ 0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00},   // U+0043 (C)
{ 0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00},   // U+0044 (D)
{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00},   // U+0045 (E)
{ 0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00},   // U+0046 (F)
{ 0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00},   // U+0047 (G)
{ 0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00},   // U+0048 (H)
{ 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0049 (I)
{ 0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00},   // U+004A (J)
{ 0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00},   // U+004B (K)
{ 0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00},   // U+004C (L)
{ 0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00},   // U+004D (M)
{ 0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00},   // U+004E (N)
{ 0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00},   // U+004F (O)
{ 0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00},   // U+0050 (P)
{ 0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00},   // U+0051 (Q)
{ 0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00},   // U+0052 (R)
{ 0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00},   // U+0053 (S)
{ 0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0054 (T)
{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00},   // U+0055 (U)
{ 0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0056 (V)
{ 0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},   // U+0057 (W)
{ 0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00},   // U+0058 (X)
{ 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00},   // U+0059 (Y)
{ 0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00},   // U+005A (Z)
{ 0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00},   // U+005B ([)
{ 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00},   // U+005C (\)
{ 0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00},   // U+005D (])
{ 0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00},   // U+005E (^)
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},   // U+005F (_)
{ 0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+0060 (`)
{ 0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00},   // U+0061 (a)
{ 0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00},   // U+0062 (b)
{ 0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00},   // U+0063 (c)
{ 0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00},   // U+0064 (d)
{ 0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00},   // U+0065 (e)
{ 0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00},   // U+0066 (f)
{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0067 (g)
{ 0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00},   // U+0068 (h)
{ 0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+0069 (i)
{ 0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E},   // U+006A (j)
{ 0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00},   // U+006B (k)
{ 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00},   // U+006C (l)
{ 0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00},   // U+006D (m)
{ 0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00},   // U+006E (n)
{ 0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00},   // U+006F (o)
{ 0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F},   // U+0070 (p)
{ 0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78},   // U+0071 (q)
{ 0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00},   // U+0072 (r)
{ 0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00},   // U+0073 (s)
{ 0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00},   // U+0074 (t)
{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00},   // U+0075 (u)
{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00},   // U+0076 (v)
{ 0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00},   // U+0077 (w)
{ 0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00},   // U+0078 (x)
{ 0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F},   // U+0079 (y)
{ 0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00},   // U+007A (z)
{ 0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00},   // U+007B ({)
{ 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},   // U+007C (|)
{ 0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00},   // U+007D (})
{ 0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // U+007E (~)
{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

// The function abs is available from stdlib.h on the Wii U, but not on the PC. 
// A local version of abs is used here, so that the following trig functions could be developed and tested on a PC, before use on the Wii U.
static float myAbs(float a)
{
	if (a >= 0)
	{
		return a;
	}
	else
	{
		return -a;
	}
}

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

// Draw a line of the colour specified.
// Taken from an internet example, but modified to allow line to go in any direction.
bool drawTextLine(float x1, float y1, float x2, float y2, unsigned int colour, unsigned int display)
{
	float dx;	// different on x axis
	float dy;	// difference on y axis
	float len;	// length of line
	float x;	// current x position
	float y;	// current y position.

	// Exit if the positions passed in are off the screen.
	if ((x1 >= 1240.0) || (y1 >= 720.0) || (x2 >= 1240.0) || (y2 >= 720.0))
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
	if (myAbs(dx) >= myAbs(dy)) { len = myAbs(dx); }
	else { len = myAbs(dy); }

	// Divide by the length to give the step sizes for drawing. 
	dx = dx / len;
	dy = dy / len;

	// Set to the start of the line.
	x = x1;
	y = y1;

	// Count alog the line length to draw the line.
	for (int i = 0; i <= len; i++)
	{
		OSScreenPutPixelEx(display, x, y, colour);
		x = x + dx;
		y = y + dy;
	}
	return true;
}


// Draw text at the position specified.
// In this case x and y position are not set to be within XDISPMAX and YDISPMAX, to allow text outside of the game play area.
bool drawText(char* dispString, unsigned int colour, unsigned int scale, unsigned int xpos, unsigned int ypos, unsigned int display)
{
	unsigned int mask;
	int b;
	char e;

	// Exit if the positions passed in, are off the screen.
	// Unsigned so can't go negative.
	if ((xpos >= 1240) || (ypos >= 720))
	{
		return false;
	}
	// Exit if text size not sensible.
	if ((scale < 1) || (scale > 6))
	{
		return false;
	}
	// Exit if screen selected does not exist.
	if ((display != SCREEN_TV) && (display != SCREEN_DRC))
	{
		return false;
	}

	// Go throught the characters limit at a sensible length.
	for (int a = 0; a < 100; a++) {
		// If end of string is detected then exit.
		if (dispString[a] == 0x00) {
			break;
		}
		// If the character is outside the visible character set, then display a hash symbol.
		if ((dispString[a] < 0x20) || (dispString[a] > 127)) {
			b = '#';
		}
		else {
			b = dispString[a];
		}
		// The first 32 characters are not visible.
		b = (char)(b - 32);
		// Count through the 8 bytes of data that set the 8x8 bit settings for the character.
		for (int y = 0; y < 8; y++) {
			// Extract the byte for the current 8 pixels.
			e = font8x8_basic[b][y];
			mask = 1;	// Set the mask to the start of the byte.
			// Count through the 8 pixels to display them.
			for (int z = 0; z < 8; z++) {
				// Loop round for the number of actual pixels, per character pixel.
				for (int w = 0; w < scale; w++) {
					// Draw the lines to make up the current pixel.
					// The drawLine function will deal with any lines that are off the edge of the screen.
					if ((e & mask) != 0) {										// xpos is the x position within the entire screen.
						drawTextLine((a * 8 * scale) + (z * scale) + xpos,		// 8 pixels across chracter, multiplied by scale which is number of actual pixels per character pixel.
							(y * scale) + ypos + w,								// w is to move down by the scale number of pixels per character pixel.
							(a * 8 * scale) + (z * scale) + xpos + (scale - 1),	// z is the pixel position across the current row.
							(y * scale) + ypos + w,								// ypos is the y position within the entire screen.
							colour,												// colour is the 24bit colour in Wii U format.
							display);											// Gamepad DRC or TV screen.
					}
				}
				mask = mask << 1; // Move the mask to the next pixel.
			}
		}
	}
	return true;
}

bool drawTextCentred(char* dispString, unsigned int colour, unsigned int scale, unsigned int xpos, unsigned int ypos, unsigned int display)
{
	unsigned int mask;				// mask used for bit processing.
	unsigned int numChar = 0;		// Number of characters in the string to be printed.
	unsigned int xposNew, yposNew;	// x and y positions modified so that the centre of the text is on xpos and ypos.
	int b;							// working variables.
	char e;

	// Exit if the positions passed in, are off the screen.
	// Unsigned so can't go negative.
	if ((xpos >= 1240) || (ypos >= 720))
	{
		return false;
	}
	// Exit if text size not sensible.
	if ((scale < 1) || (scale > 6))
	{
		return false;
	}
	// Exit if screen selected does not exist.
	if ((display != SCREEN_TV) && (display != SCREEN_DRC))
	{
		return false;
	}

	// Find how long the string. Strlen not used to avoid any differences between Wii U and PC.
	for (int a = 0; a < 100; a++) {
		// If end of string is detected then exit.
		if (dispString[a] == 0x00) {
			numChar = a;
			break;
		}
	}
	// Do calculations to centre the text on the x and y positions within the game area of the screen.
	// The xpos is adjusted for half the length of the string multiplied by the scaled pixels per character. 
	xposNew = xpos - ((numChar / 2) * 8 * scale);
	// The ypos is adjusted for the YOFFSET to the start of the game screen, then to half the scaled height of the text pixels.
	yposNew = ypos - (4 * scale);

	// Go throught the characters limit at a sensible length.
	for (int a = 0; a < 100; a++) {
		// If end of string is detected then exit.
		if (dispString[a] == 0x00) {
			break;
		}
		// If the character is outside the visible character set, then display a hash symbol.
		if ((dispString[a] < 0x20) || (dispString[a] > 127)) {
			b = '#';
		}
		else {
			b = dispString[a];
		}
		// The first 32 characters are not visible.
		b = (char)(b - 32);
		// Count through the 8 bytes of data that set the 8x8 bit settings for the character.
		for (int y = 0; y < 8; y++) {
			// Extract the byte for the current 8 pixels.
			e = font8x8_basic[b][y];
			mask = 1;	// Set the mask to the start of the byte.
			// Count through the 8 pixels to display them.
			for (int z = 0; z < 8; z++) {
				// Loop round for the number of actual pixels, per character pixel.
				for (int w = 0; w < scale; w++) {
					// Draw the lines to make up the current pixel.
					// The drawLine function will deal with any lines that are off the edge of the screen.
					if ((e & mask) != 0) {											// xposNew is the x centred position within the game screen.
						drawTextLine((a * 8 * scale) + (z * scale) + xposNew,		// 8 pixels across chracter, multiplied by scale which is number of actual pixels per character pixel.
							(y * scale) + yposNew + w,								// w is to move down by the scale number of pixels per character pixel.
							(a * 8 * scale) + (z * scale) + xposNew + (scale - 1),	// z is the pixel position across the current row.
							(y * scale) + yposNew + w,								// ypos is the y centred position within the game screen.
							colour,													// colour is the 24bit colour in Wii U format.
							display);												// Gamepad DRC or TV screen.
					}
				}
				mask = mask << 1; // Move the mask to the next pixel.
			}
		}
	}
	return true;
}
