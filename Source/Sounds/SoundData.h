#pragma once
// I've based the sound processing on LiveSynthesisU. This example of using sndcore2 was extremely usedful.
// There seem to be errors in the sndcore2/voice.h header file. I could not get sound going using the voice.h
// data structures and function calls. I have taken the structures and function definitions from LiveSynthesisU.
// These need to be used along with the exact values in Sound.c for the sound interfaces to operate.

// List of the header files for each sound, so that only one header needs to be included elsewhere in the program.
#include "Sounds/BoomCloud.wav.h"
#include "Sounds/Bite.wav.h"
#include "Sounds/Cheer.wav.h"
#include "Sounds/Creak1.wav.h"
#include "Sounds/DanceAround.wav.h"

// The following has been taken from main.h from LiveSynthesisU. These are not the same as the wut sndcore2.
// These are necessary for sound to work correctly. It took me several haerd days of trial and error to determine
// that I could not used the voice.h header file.
#include <wut.h>

typedef struct AXVoiceLink AXVoiceLink;
typedef struct AXVoiceOffsets AXVoiceOffsets;
typedef struct AXVoice AXVoice;

// The structure needs to be here, to be part of the AXVoice structure, it is not used in the sound code.
struct AXVoiceLink
{
	void* next;
	void* prev;
};

struct AXVoiceOffsets
{
	uint16_t dataType;			// Must be 25 for 8-bit data to match the header files above.
	uint16_t loopingEnabled;
	uint32_t loopOffset;		// set to 0 for start of file
	uint32_t endOffset;			// set to size of data  -1.
	uint32_t currentOffset;		// set to 0.
	const unsigned char* data;	// Important that this is unsigned char pointer for software to work.
};

struct AXVoice
{
	//! The index of this voice out of the total voices (not used by sound.c code)
	uint32_t index;

	//! Current play state of this voice
	uint32_t state;

	//! Current volume of this voice
	uint32_t volume;

	//! The renderer to use for this voice (not used by sound.c code)
	uint32_t renderer;

	//! this is a link used in the stack, we do this in host-memory currently (not used by sound.c code)
	AXVoiceLink link;

	//! A link to the next callback to invoke (not used by sound.c code)
	void* cbNext;

	//! The priority of this voice used for force-acquiring a voice
	uint32_t priority;

	//! The callback to call if this is force-free'd by another acquire (not used by sound.c code)
	void* callback;

	//! The user context to send to the callbacks (not used by sound.c code)
	void* userContext;

	//! A bitfield representing different things needing to be synced.
	uint32_t syncBits;

	WUT_UNKNOWN_BYTES(0x8);

	//! The current offset data!
	AXVoiceOffsets offsets;

	//! An extended version of the callback above (not used by sound.c code)
	void* callbackEx;

	//! The reason for the callback being invoked (not used by sound.c code)
	uint32_t callbackReason;

	// (not used by sound.c code)
	float unk0;
	float unk1;
};

// Again these are different from voice.h, these must be used for sound to operate correctly.
extern void AXQuit(void);
extern int32_t AXVoiceBegin(void* v);
extern int32_t AXVoiceEnd(void* v);
extern void AXSetVoiceType(void* v, uint16_t type);
extern void AXSetVoiceOffsets(void* v, const void* buf);
extern void AXSetVoiceSrcType(void* v, uint32_t type);
extern void AXSetVoiceVe(void* v, const void* vol);
extern int32_t AXSetVoiceDeviceMix(void* v, int32_t device, uint32_t id, void* mix);
extern void AXSetVoiceState(void* v, uint16_t state);
extern void AXSetVoiceSrc(void* v, const void* src);
extern void* AXAcquireVoice(uint32_t prio, void* callback, uint32_t arg);
extern void AXFreeVoice(void* v);
