// Main program for PacMan-ish
#include <stdio.h>				// For sprintf.

#include <coreinit/screen.h>	// For OSScreen.
#include <coreinit/thread.h>	// For Sleep
#include <vpad/input.h>			// For the game-pad inputs.
#include <whb/proc.h>			// For the main loop and for the home button to work correctly.
#include <whb/log.h>			// Using the console logging features sets up the screen output.
#include <whb/log_console.h>	// Found this is needed for the display to work properly.

#include "Draw.h"				// Headers for drawing via OSScreen.
#include "Images/Images.h"		// Header for the images to be drawn using Draw.h.
#include "Sounds.h"				// Headers for sounds via sndcore2.
#include "Game.h"				// Header for the PacMan-ish game.

#define MINDEL 17				// Minimum movement delay in ms (screen update of 60Hz).

#define XOFFSET 304				// Game screen X position (1280 - (21 blocks * 32 pixels)) / 21
#define YOFFSET  56				// Game screen Y position  (720 - (19 blocks * 32 pixels)) /  2

int playerMove = UP;			// Variable for move selected, initialised to UP so that pacman can't move at the start.

// Variable delay to increase speed during game.
void moveDelay()
{
	int delay;

	delay = (8000 - getScore()) / 100;			// Decrease delay as the score increases.
	if (delay <= MINDEL) delay = MINDEL;		// Limit to the minimum delay.
	OSSleepTicks(OSMillisecondsToTicks(delay));	// Allow some time back for the OS in ms.
}

// Update the game-pad text.
void displayGamepad()
{
	// Clear Game Pad screen to black.
	OSScreenClearBufferEx(SCREEN_DRC, 0x00000000u);

	char sscore[100] = "\0";	// Strings to display the current score and screen level.
	char slevel[100] = "\0";

	// Get the score and level for display.
	sprintf(sscore, "Score: % 6i ", getScore());
	sprintf(slevel, "Level: % 6i ", getScreen());

	// Display information about the game on the game pad screen.
	OSScreenPutFontEx(SCREEN_DRC, 3,  1, "PACMAN-ish");
	OSScreenPutFontEx(SCREEN_DRC, 3,  3, "A game based on the legendary PacMan.");
	OSScreenPutFontEx(SCREEN_DRC, 3,  5, "Use the left joycon or direction buttons to move PacMan.");
	OSScreenPutFontEx(SCREEN_DRC, 3,  6, "Collect the dots to score.");
	OSScreenPutFontEx(SCREEN_DRC, 3,  7, "The red and white balls send the baddies back to their pen.");
	OSScreenPutFontEx(SCREEN_DRC, 3,  9, "Press ZL to start the game.");
	OSScreenPutFontEx(SCREEN_DRC, 3, 12, sscore);
	OSScreenPutFontEx(SCREEN_DRC, 3, 13, slevel);

	// Flip buffers to display the updates.
	OSScreenFlipBuffersEx(SCREEN_DRC);
	return;
}

// Display the game on the TV screen. A count of 0-3 is passed in to aid movement animation.
void displayScreen(int fineMove)
{
	int xmove;	// Used for movement animation.
	int ymove;

	// Game objects to get the current positions from the Game.
	gameObject_t pacman;
	// The initial version of the game had aliens for baddies (hence the name), these are now ghosts as per PacMan.
	gameObject_t alien;

	// Clear the TV to have a black background.
	OSScreenClearBufferEx(SCREEN_TV, 0x00000000u);

	// Draw the current state of the game board.
	for (int y = 0; y < MAXY; y++)
	{
		for (int x = 0; x < MAXX; x++)
		{
			// There are three main components of the game screen, blocks for the walls, dots and power-ups.
			if (getGameScreen(x, y) == BLOCK) { drawImage(BlockImage, XOFFSET + (x * SQX), YOFFSET + (y * SQY), 0); }
			if (getGameScreen(x, y) == POWER) { drawImage(BigDotImage, XOFFSET + (x * SQX), YOFFSET + (y * SQY), 0); }
			if (getGameScreen(x, y) == POINT) { drawImage(DotImage, XOFFSET + (x * SQX), YOFFSET + (y * SQY), 0); }
		}
	}

	// Put pacman on the screen taking into account the board offset and size of blocks.
	pacman = getPacMan();
	// The fine movement uses the fineMove value passed in to move the player in 1/4 block steps.
	xmove = ((int)pacman.x - (int)pacman.xold) * fineMove * SQX/4;
	ymove = ((int)pacman.y - (int)pacman.yold) * fineMove * SQY/4;
	// The fineMove count is used to select the image to animate mouth movement.
	// The direction is pased in to the draw function so that PacMan faces the way he is moving.
	if      (fineMove == 0) { drawImage(PacManCImage, XOFFSET + (pacman.xold * SQX) + xmove, YOFFSET + (pacman.yold * SQY) + ymove, pacman.direction); }
	else if (fineMove == 1) { drawImage(PacManBImage, XOFFSET + (pacman.xold * SQX) + xmove, YOFFSET + (pacman.yold * SQY) + ymove, pacman.direction); }
	else if (fineMove == 2) { drawImage(PacManAImage, XOFFSET + (pacman.xold * SQX) + xmove, YOFFSET + (pacman.yold * SQY) + ymove, pacman.direction); }
	else if (fineMove == 3) { drawImage(PacManBImage, XOFFSET + (pacman.xold * SQX) + xmove, YOFFSET + (pacman.yold * SQY) + ymove, pacman.direction); }

	// Put the aliens on the screen taking into account the board offset and size of blocks.
	for (unsigned int z = 0; z < getNumberAliens(); z++)
	{
		alien = getAlien(z);
		// The fine movement uses the fineMove value passed in to move the aliens in 1/4 block steps.
		xmove = ((int)alien.x - (int)alien.xold) * fineMove * SQX / 4;
		ymove = ((int)alien.y - (int)alien.yold) * fineMove * SQY / 4;
		// The direction passed in is set to 0 as the ghosts don't rotate like PacMan.
		// alien.direction is used to pick the image so that the eyes are pointing in the correct direction.
		// 
		if      (alien.direction == UP)    { drawImage(GhostUImage, XOFFSET + (alien.xold * SQX) + xmove, YOFFSET + (alien.yold * SQY) + ymove, 0); }
		else if (alien.direction == DOWN)  { drawImage(GhostDImage, XOFFSET + (alien.xold * SQX) + xmove, YOFFSET + (alien.yold * SQY) + ymove, 0); }
		else if (alien.direction == RIGHT) { drawImage(GhostRImage, XOFFSET + (alien.xold * SQX) + xmove, YOFFSET + (alien.yold * SQY) + ymove, 0); }
		else if (alien.direction == LEFT)  { drawImage(GhostLImage, XOFFSET + (alien.xold * SQX) + xmove, YOFFSET + (alien.yold * SQY) + ymove, 0); }
	}

	// Flip the screen buffer to show the new display.
    OSScreenFlipBuffersEx(SCREEN_TV);
	return;
}

// A small function to return the last button pressed by the player. 
// If no new button has been pressed the move value is left unchanged. 
void getVpad()
{
	VPADStatus status;	// Status returned for the game pad buttons.

	// Get the VPAD button last pressed.
	VPADRead(VPAD_CHAN_0, &status, 1, NULL);

	if ((status.hold & VPAD_BUTTON_UP)   || (status.hold & VPAD_STICK_L_EMULATION_UP))		{ playerMove = UP; }	// Player moves.
	if ((status.hold & VPAD_BUTTON_LEFT) || (status.hold & VPAD_STICK_L_EMULATION_LEFT))	{ playerMove = LEFT; }
	if ((status.hold & VPAD_BUTTON_RIGHT)|| (status.hold & VPAD_STICK_L_EMULATION_RIGHT))	{ playerMove = RIGHT; }
	if ((status.hold & VPAD_BUTTON_DOWN) || (status.hold & VPAD_STICK_L_EMULATION_DOWN))	{ playerMove = DOWN; }
	return;
}

// Fill the screen with blocks to show that the game has ended.
void gameEnded()
{
	// Clear both copies of the TV buffer to avoid strange effects.
	OSScreenClearBufferEx(SCREEN_TV, 0x00000000u);
	OSScreenFlipBuffersEx(SCREEN_TV);
	OSScreenClearBufferEx(SCREEN_TV, 0x00000000u);

	// Draw the current state of the game board.
	for (int y = 0; y < MAXY; y++)
	{
		for (int x = 0; x < MAXX; x++)
		{
			drawImage(BlockImage, XOFFSET + (x * SQX), YOFFSET + (y * SQY), 0);
		}

		// Flip the screen buffer to show the new display.
		OSScreenFlipBuffersEx(SCREEN_TV);

		// This is done twice so that both screen buffers are updated for display to avoid strange effects.
		for (int x = 0; x < MAXX; x++)
		{
			drawImage(BlockImage, XOFFSET + (x * SQX), YOFFSET + (y * SQY), 0);
		}

		// Flip the screen buffer to show the new display.
		OSScreenFlipBuffersEx(SCREEN_TV);

		OSSleepTicks(OSMillisecondsToTicks(50)); // Delay so that the changes can be seen.
	}
	return;
}

// Main program with setup, close down and the main game loop.
int main(int argc, char** argv)
{
	VPADStatus status;	// Status returned for the game pad buttons to get ZL for game start.
	unsigned int gameState = 0;	// Game state waiting = 0, starting = 1, running = 2, ending = 3.
	bool doNewScreen = false;	// Flag to add delay at end of screen.

	// This is the main process and must be in the program at the start for the home button to operate correctly.
	WHBProcInit();
	WHBLogConsoleInit();	// Console Init gets the display to operate correctly so keep in the build.

	setupSound();			// Call separate function from sound.h to do all of the sound setup.

	newScreen();			// Set up the screen and game the first time for the game to be displayed while waiting to start.
	newGame();

	// There must be a main loop on WHBProc running, for the program to correctly operate with the home button.
	// Home pauses this loop and continues if resume is selected. There must therefore be one main loop of processing in the main program.
	while (WHBProcIsRunning())
	{
		if (gameState == 0)			// waiting;
		{
			VPADRead(VPAD_CHAN_0, &status, 1, NULL);					// Get the VPAD button last pressed.
			if  (status.trigger & VPAD_BUTTON_ZL) { gameState = 1; }	// change to starting.
			displayGamepad();											// Show game instructions on the game-pad.
			displayScreen(0);											// Show game screen while waiting.
			moveDelay();
		}
		else if (gameState == 1)	// starting
		{							// Set ready for a new game.
			newScreen();			// Set up the screen.
			newGame();				// New game done after newScreen, as newScreen increments aliens.
			gameState = 2;			// set to running.
			playerMove = UP;		// Set the move to be upwards so that PacMan does not move once new screen starts.
		}
		else if (gameState == 2)	// running
		{
			putMove(playerMove);	// Pass the player move (aquired during animiation) into the game.

			if (moveAliens() == true)	// If move aliens shows that pacman has been caught, break the loop to end the game.
			{
				gameState = 3;			// set to ending
			}
			if (screenEnd() == true)	// If all dots eaten, go up to the next level.
			{
				doNewScreen = true;
			}
			displayGamepad();	// Update scores on game-pad.

			// Display is done 4 times to support animation.
			// The Vpad input is obtained each time to support faster response to the controls.
			// The number passed in to displayScreen is used to support finer movement animation.
			displayScreen(0);
			moveDelay();		// Give time back to the OS (delay reduces to make game go faster).
			getVpad();

			displayScreen(1);
			moveDelay();
			getVpad();

			displayScreen(2);
			moveDelay();
			getVpad();

			displayScreen(3);
			moveDelay();
			getVpad();

			// Do the new screen after PacMan has moved, so the player sees the last point eaten.
			if (doNewScreen == true)
			{
				putsoundSel(LEVEL);								// Play the level-up sound.
				OSSleepTicks(OSMillisecondsToTicks(1000));		// Allow time to see the board at end of the screen.
				newScreen();
				playerMove = UP;		// Set the move to be upwards, so that PacMan does not move once new screen starts.
				doNewScreen = false;	// Only do the new screen once when commanded.
			}
		}
		else if (gameState == 3)	// ending
		{
			displayGamepad();							// Show instructions and scores on game-pad.
			putsoundSel(ENDGAME);						// Play the end of game sound.
			OSSleepTicks(OSMillisecondsToTicks(1000));	// Allow time to see the board at end of the game.
			gameEnded();
			OSSleepTicks(OSMillisecondsToTicks(2000));	// Allow time to see end screen.
			newScreen();								// Set up the screen to be displayed while waiting to start.
			gameState = 0;								// Back to waiting.
		}
	}

	QuitSound();	// Close sound stuff.

	// If we get out of the program clean up and exit.
    WHBLogConsoleFree();
    WHBProcShutdown();
    return 0;
}
