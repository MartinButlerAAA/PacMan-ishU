#pragma once
// The function interface to play sounds.

//! A value from enum SOUNDSEL.
typedef unsigned int soundsel_t;

enum SOUNDSEL
{
	NOCHANGE  = 0,
	STOPBKGND = 1,
	STRTBKGND = 2,
	DOT       = 3,
	POWERUP   = 4,
	LEVEL     = 5,
	ENDGAME   = 6,
};

// Call this to set up sounds once, before attempting to call putsoundSel.
extern void setupSound();

// Call this with one of the above SOUNDSEL values for that sound action.
extern void putsoundSel(soundsel_t sndSel);

// Call this once before exiting to close down sound.
extern void QuitSound();
