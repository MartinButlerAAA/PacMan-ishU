#pragma once
// Header for PacMan-ish game

// The gameObject structure is used for game operation and to support graphic display.  The structure has the current position within the game board,
// the previous position and direction of travel. This information is used to support screen animation within the game.
typedef struct gameObject gameObject_t;

struct gameObject
{
	int x;
	int y;
	int xold;
	int yold;
	int direction;
};

#define MAXX  21			// Number of positions across the X axis within the game board.
#define MAXY  19			// Number of positions down   the Y axis within the game board.

#define UP     2			// Constants for direction of movement.
#define RIGHT  1			// These are for the player to move PacMan and for alien movement.
#define DOWN   3
#define LEFT   4

#define SPACE (char)' '		// The components used within the gameboard.
#define POINT (char)'.'
#define POWER (char)'o'
#define BLOCK (char)'#'
#define PACMN (char)'P'

extern void newScreen();									// Reset the screen and players ready for a new level.
extern char getGameScreen(unsigned int x, unsigned int y);	// Used to get individual blocks from the game board for display.
extern bool putMove(unsigned int  m);						// Pass in the player's move to the game.
extern bool screenEnd();									// Used to see if all dots collected, so end of a level. true is level complete.
extern bool moveAliens();									// Moves the aliens. Returns true if an alien has caught PacMan (i.e. game ended).
extern void newGame();										// Set up for the start of a new game.
extern gameObject_t getPacMan();							// Return current position of PacMan (inlcuding last position and direction to aid animation).
extern unsigned int getNumberAliens();						// Return the number of aliens in the level.
extern gameObject_t getAlien(unsigned int n);				// Return current position of alien n (including last position and direction to aid animation).
extern unsigned int getScore();								// Return the current score for display.
extern unsigned int getScreen();							// Return the current screen number for display.