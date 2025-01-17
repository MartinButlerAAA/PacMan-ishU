//
// Processing for the PacMan-ish game.
// 
// This is an example game program written in Java as a project between me and my son. My son designed the screen and characters;
// he also told me how he wanted the game to be played. I then used one of my earlier games to lift the graphics processing and main loop as a starting point.
//
// The program has now been converted to a Cpp Console program with text output and keyboard input. Which has then been used as the basis for this Wii U C version of the game.
//
#include <stdlib.h>				// For rand() random numbers.
#include <stdbool.h>			// To use boolean logic.

#include <coreinit/thread.h>	// for Sleep

#include "Sounds.h"				// A couple of the sounds are best processed directly in the game.
#include "Game.h"				// For constants used in the game.

// The PacMan-ish game board is a 2-dimensional array.
char scr[MAXY][MAXX] =
{ { BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK },
  { BLOCK, POINT, PACMN, POINT, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POWER, POINT, BLOCK },
  { BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK },
  { BLOCK, POINT, BLOCK, BLOCK, BLOCK, POINT, POINT, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POINT, BLOCK, BLOCK, BLOCK, POINT, BLOCK },
  { BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK },
  { BLOCK, BLOCK, BLOCK, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, BLOCK, BLOCK, BLOCK },
  { SPACE, SPACE, BLOCK, POINT, BLOCK, BLOCK, POINT, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, POINT, BLOCK, BLOCK, POINT, BLOCK, SPACE, SPACE },
  { SPACE, SPACE, BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK, SPACE, SPACE },
  { SPACE, SPACE, BLOCK, BLOCK, POINT, POINT, POINT, POINT, BLOCK, BLOCK, SPACE, BLOCK, BLOCK, POINT, POINT, POWER, POINT, BLOCK, BLOCK, SPACE, SPACE },
  { SPACE, SPACE, SPACE, BLOCK, POINT, BLOCK, BLOCK, BLOCK, BLOCK, SPACE, SPACE, SPACE, BLOCK, BLOCK, BLOCK, BLOCK, POINT, BLOCK, SPACE, SPACE, SPACE },
  { SPACE, SPACE, BLOCK, BLOCK, POINT, POINT, POINT, POINT, BLOCK, BLOCK, SPACE, BLOCK, BLOCK, POINT, POINT, POINT, POINT, BLOCK, BLOCK, SPACE, SPACE },
  { SPACE, SPACE, BLOCK, POINT, POINT, POINT, POWER, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK, SPACE, SPACE },
  { SPACE, SPACE, BLOCK, POINT, BLOCK, BLOCK, POINT, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, POINT, BLOCK, BLOCK, POINT, BLOCK, SPACE, SPACE },
  { BLOCK, BLOCK, BLOCK, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, BLOCK, BLOCK, BLOCK },
  { BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POWER, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK },
  { BLOCK, POINT, BLOCK, BLOCK, BLOCK, POINT, POINT, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POINT, BLOCK, BLOCK, BLOCK, POINT, BLOCK },
  { BLOCK, POINT, POWER, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, POINT, BLOCK },
  { BLOCK, POINT, POINT, POINT, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POINT, POINT, POINT, POINT, BLOCK, BLOCK, BLOCK, BLOCK, POINT, POINT, POINT, BLOCK },
  { BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK } };

// Globals for the game.
unsigned int	score;						// Score total.
unsigned int	screen;						// Current screen number.
unsigned int	Naliens;					// Number of aliens for the current screen.

gameObject_t	pacMan;						// Position of PacMan.

gameObject_t	aliens[30];					// Positions of Aliens.


// Array of characters used to represent game play for game progress and for display processing.
// Each character represents a block on the screen.
// Increased tables X and Y size by 1 to avoid the risk of overflow.
 char			store[MAXY+1][MAXX+1];

// Note that PACMAN PACMN appears on the gameboard and overwrites contents. The Aliens are sprites to avoid them changing the board.

//Put the aliens back to the starting pen.
static void aliensBack()
{
	// To be on the safe side the maximum number of aliens are always initialised, even if not used.
	for (int z = 0; z < 30; z++)
	{
		aliens[z].x = 10;
		aliens[z].y = 9;
		aliens[z].xold = 10;
		aliens[z].yold = 9;
		aliens[z].direction = RIGHT;
	}
}

// Move the PacMan player using the move passed in, and update information to support graphics animation.
static void movePlayer()
{
	//Depending on the input value for direction, move the player and capture direction information for animation.
	// Update the scores if dots or power-ups detected.
	// Call the aliensBack function for power-ups.

	// Set the previous position to match the current position before moving.
	pacMan.xold = pacMan.x;
	pacMan.yold = pacMan.y;

	// If the move is for up and the position above isn't a block the do the move.
	if ((pacMan.direction == UP) && (store[pacMan.y - 1][pacMan.x] != BLOCK))
	{
		store[pacMan.y][pacMan.x] = SPACE;												// Clear the old PacMan position to a space.

		// If a power up has been eaten then increase score, play sound and put aliens back in their pen and sound.
		if (store[pacMan.y - 1][pacMan.x] == POWER) { score = score + 100; putsoundSel(POWERUP); aliensBack(); }
		if (store[pacMan.y - 1][pacMan.x] == POINT) { score++; putsoundSel(DOT); }		// If a dot is eaten increase the score and play the sound.
		pacMan.y--;																		// Move PacMan.
		store[pacMan.y][pacMan.x] = PACMN;
	}
	// Process down move.
	if ((pacMan.direction == DOWN) && (store[pacMan.y + 1][pacMan.x] != BLOCK))
	{
		store[pacMan.y][pacMan.x] = SPACE;
		if (store[pacMan.y + 1][pacMan.x] == POWER) { score = score + 100;  putsoundSel(POWERUP); aliensBack(); }
		if (store[pacMan.y + 1][pacMan.x] == POINT) { score++; putsoundSel(DOT); }
		pacMan.y++;
		store[pacMan.y][pacMan.x] = PACMN;
	}
	// Process left move.
	if ((pacMan.direction == LEFT) && (store[pacMan.y][pacMan.x - 1] != BLOCK))
	{
		store[pacMan.y][pacMan.x] = SPACE;
		if (store[pacMan.y][pacMan.x - 1] == POWER) { score = score + 100; putsoundSel(POWERUP); aliensBack(); }
		if (store[pacMan.y][pacMan.x - 1] == POINT) { score++;  putsoundSel(DOT); }
		pacMan.x--;
		store[pacMan.y][pacMan.x] = PACMN;
	}
	// Process right move.
	if ((pacMan.direction == RIGHT) && (store[pacMan.y][pacMan.x + 1] != BLOCK))
	{
		store[pacMan.y][pacMan.x] = SPACE;
		if (store[pacMan.y][pacMan.x + 1] == POWER) { score = score + 100; putsoundSel(POWERUP); aliensBack(); }
		if (store[pacMan.y][pacMan.x + 1] == POINT) { score++;  putsoundSel(DOT); }
		pacMan.x++;
		store[pacMan.y][pacMan.x] = PACMN;
	}
}

// Move the aliens on the game board.
bool moveAliens()
{
	unsigned int z;		// Working index.
	bool gend = false;	// Return flag.

	// Move all of the aliens being used for this game level.
	for (z = 0; z < Naliens; z++)
	{
		// Set the previous position to match the current position before moving.
		aliens[z].xold = aliens[z].x;
		aliens[z].yold = aliens[z].y;

		// The following logic keeps moving in the same direction until the alien hits a block.
		// If the alien can no longer move due to a block, its direction is cancelled.
		if (aliens[z].direction == UP)
		{
			if (store[aliens[z].y - 1][aliens[z].x] != BLOCK)
			{
				aliens[z].y--;
			}
			else
			{
				aliens[z].direction = 0;
			}
		}

		if (aliens[z].direction == DOWN)
		{
			if (store[aliens[z].y + 1][aliens[z].x] != BLOCK)
			{
				aliens[z].y++;
			}
			else
			{
				aliens[z].direction = 0;
			}
		}
		if (aliens[z].direction == LEFT)
		{
			if (store[aliens[z].y][aliens[z].x - 1] != BLOCK)
			{
				// This is additional logic to use available up directions.
				// This is necessary so that aliens can go into some side paths.
				if ((store[aliens[z].y - 1][aliens[z].x] != BLOCK) && (rand()%6 > 4))
				{
					aliens[z].direction = UP;
				}
				else
				{
					aliens[z].x--;
				}
			}
			else
			{
				aliens[z].direction = 0;
			}
		}
		if (aliens[z].direction == RIGHT)
		{
			if (store[aliens[z].y][aliens[z].x + 1] != BLOCK)
			{
				// This is additional logic to use available down directions.
				if ((store[aliens[z].y + 1][aliens[z].x] != BLOCK) && (rand() % 6 > 4))
				{
					aliens[z].direction = DOWN;
				}
				else
				{
					aliens[z].x++;
				}
			}
			else
			{
				aliens[z].direction = 0;
			}
		}

		// If the alien is stopped (i.e. it has hit a wall, pick a new random direction).
		if (aliens[z].direction == 0)
		{
			aliens[z].direction = rand() % 4 + 1;
		}

		// If an alien has run into PacMan, then the game is over so return true.
		if ((store[aliens[z].y][aliens[z].x] == PACMN) || (store[aliens[z].yold][aliens[z].xold] == PACMN))
		{
			gend = true;
		}
	}
	return gend;
}

// Accessor function that gives the content of the game board at a selected coordinate.
char getGameScreen(unsigned int x, unsigned int y)
{ 
	// If the coordinate requested is outside of the game board return a blank space.
	if ((x >= MAXX) || (y >= MAXY))
	{ 
		return (SPACE);
	}
	// Otherwise return the content of the game board.
	else
	{
		// Do not report the position of PacMan. This is reported via a separate access function to support animation.
		if (store[y][x] != PACMN)
		{
			return store[y][x];
		}
		else
		{
			return(SPACE);
		}
	}
}

// If the move passed in to the game is valid, pass it into the PacMan move processing.
bool putMove(unsigned int m)
{
	if ((m == UP) || (m == DOWN) || (m == LEFT) || (m == RIGHT))
	{
		pacMan.direction = m;
		movePlayer();
		return true;
	}
	else
	{
		return false;
	}
}

// Set the screen back to the start ready for another level.
void newScreen()
{
	screen++;				// Game level is increased.
	score = score + 1000;	// Add to the score.
	Naliens++;				// Add an alien to make it harder.

	// Put PacMan back to the starting position.
	pacMan.x = 2;
	pacMan.y = 1;
	pacMan.direction = UP;		// PacMan to not move at start of game unless controlled by player.
	pacMan.xold = 2;
	pacMan.yold = 1;

	// Put aliens back into their starting pen.
	aliensBack();

	// Copy the starting screen back into the working copy of the game board.
	for (int y = 0; y < MAXY; y++)
	{
		for (int x = 0; x < MAXX; x++)
		{
			store[y][x] = scr[y][x];
		}
	}

	// Put PacMan back on the game board.
	store[pacMan.y][pacMan.x] = PACMN;
}

// Check to see if all of the dots have been eaten, if so it is the end of the screen.
bool screenEnd()
{
	int x, y, z;	// Working indices.
	z = 0;			// Set dot count to 0.

	for (y = 0; y < MAXY; y++)
	{
		for (x = 0; x < MAXX; x++)
		{
			if ((store[y][x] == POWER) || (store[y][x] == POINT))
			{
				z++;
			}
		}
	}
	// If there are no dots left then return true that the screen has ended.
	if (z == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Set up for the start of a new game.
extern void newGame()
{
	score = 0;
	screen = 1;
	Naliens = 3;
}


// Returns current position of PacMan (inlcuding last position and direction to aid animation).
gameObject_t getPacMan()
{
	return pacMan;
}

// Return the number of Aliens in the level.
unsigned int getNumberAliens()
{
	return Naliens;
}

// Returns current position of alien n (including last position and direction to aid animation).
gameObject_t getAlien(unsigned int n)
{
	return aliens[n];
}

// Return the current score for display.
extern unsigned int getScore()
{
	return score;
}

// Return the current screen number for display.
extern unsigned int getScreen()
{
	return screen;
}
