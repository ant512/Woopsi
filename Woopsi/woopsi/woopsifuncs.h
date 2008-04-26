/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#ifndef _WOOPSIFUNCS_H_
#define _WOOPSIFUNCS_H_

#include <nds.h>

#ifdef USING_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SDL.h"

#define woopsiRGB(r, g, b) (((b) << 10) | ((g) << 5) | (r) | (1 << 15))

#define DMA_ON 0x80000000
#define DMA_NOW 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_16NOW (DMA_ON | DMA_NOW | DMA_16)
#define DMA_32NOW (DMA_ON | DMA_NOW | DMA_32)

void DMA_Copy(u16* source, u16* dest, u32 count, u32 mode);
void DMA_Force(u16 source, u16* dest, u32 count, u32 mode);
void DMA_Copy2(u16* source, u16* dest, u32 count, u32 mode);
void DMA_Force2(u16 source, u16* dest, u32 count, u32 mode);
bool DMA_Active();
bool DMA_Active2();

typedef struct PAD
{
	union
	{
		struct
		{
			u16 A:1;
			u16 B:1;
			u16 Select:1;
			u16 Start:1;
			u16 Right:1;
			u16 Left:1;
			u16 Up:1;
			u16 Down:1;
			u16 R:1;
			u16 L:1;
			u16 X:1;
			u16 Y:1;
			u16 Touch:1;
			u16 Lid:1;
		};
		u16 AllKeys;
	};
} _pad;

typedef struct PADS {
   _pad Held, Released, Newpress;
} _pads;

typedef struct STYLUS {
	u8 Held:1, Released:1, Newpress:1, unused:5;
	s16 X, Y, Vx, Vy, oldVx, oldVy, Downtime, Uptime, DblClick;
} _stylus;

extern _pads Pad;
extern _stylus Stylus;

void woopsiUpdateInput();
void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

// Generic functions
extern u16* DrawBg[2];

void initWoopsiGfxMode();
void woopsiWaitVBL();
bool woopsiLidClosed();

// Simulate some features of libnds
void DC_FlushRange(const void *base, u32 size);

#else

/**
 * Converts separate RGB component values into a single 16-bit value for use with the
 * DS' framebuffer.  All supplied values should be 5 bits wide (ie. between 0 and 31).
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return The 15-bit colour value with the top alpha bit set.
 */
#define woopsiRGB(r, g, b) (((b) << 10) | ((g) << 5) | (r) | (1 << 15))

// Extra DMA defines
#define REG_DMA2SRC *(volatile u32*)0x040000C8
#define REG_DMA2DST *(volatile u32*)0x040000CC
#define REG_DMA2CNT *(volatile u32*)0x040000D0

/**
 * Perform a DMA copy using the second DMA channel.  DMA copies use the DMA hardware
 * to rapidly copy a block of memory from one location to another.
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to copy.
 * @param mode The DMA mode to use.
 */
#define DMA_Copy2(source, dest, count, mode) {REG_DMA2SRC = (u32)source; REG_DMA2DST = (u32)dest; REG_DMA2CNT = (count) | (mode);}

/**
 * Perform a DMA force using the second DMA channel.  DMA force is similar to a copy but
 * it duplicates the same value repeatedly into the destination instead of copying a
 * range of values.
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to write.
 * @param mode The DMA mode to use.
 */
#define DMA_Force2(ulVal, dest, count, mode) {REG_DMA2SRC=(u32)&ulVal; REG_DMA2DST = (u32)dest; REG_DMA2CNT = (count) | (mode) | DMA_SRC_FIX;}

/**
 * Check if the first DMA channel is active.  DMA access is asynchronous so DMA operations 
 * can run whilst the rest of the program is running, so it is important to check that
 * a channel is inactive before using it.
 * @return True if the channel is still active.
 */
#define DMA_Active() (!(!(REG_DMA3CNT & DMA_ON)))

/**
 * Check if the second DMA channel is active.
 * @see DMA_Active()
 * @return True if the channel is still active.
 */
#define DMA_Active2() (!(!(REG_DMA2CNT & DMA_ON)))

#ifdef USING_PALIB
#include <PA9.h>
#else

#include <nds/memory.h>
#include <nds/bios.h>

#define REG_DMA3SRC *(volatile u32*)0x040000D4
#define REG_DMA3DST *(volatile u32*)0x040000D8
#define REG_DMA3CNT *(volatile u32*)0x040000DC
#define DMA_ON 0x80000000
#define DMA_NOW 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_16NOW (DMA_ON | DMA_NOW | DMA_16)
#define DMA_32NOW (DMA_ON | DMA_NOW | DMA_32)

/**
 * Perform a DMA copy using the first DMA channel.
 * @see DMA_Copy2()
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to copy.
 * @param mode The DMA mode to use.
 */
#define DMA_Copy(source, dest, count, mode) {REG_DMA3SRC = (u32)source; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}

/**
 * Wipe a region of memory using the DMA hardware.  This function is not used by Woopsi
 * and could be removed.
 * @param dest The destination memory address.
 * @param count The number of values to wipe.
 * @param mode The DMA mode to use.
 */
#define DMA_Clear(dest, count, mode) {REG_DMA3SRC = (u32)Blank; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}

/**
 * Perform a DMA force using the first DMA channel.
 * @see DMA_Force2()
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to write.
 * @param mode The DMA mode to use.
 */
#define DMA_Force(ulVal, dest, count, mode) {REG_DMA3SRC=(u32)&ulVal; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode) | DMA_SRC_FIX;}

/**
 * Unused by Woopsi; could be removed.
 */
#define DMA_CopyEx(type, source, dest, count, mode) {DMA_SRC(type) = (u32)source; DMA_DEST(type) = (u32)dest; DMA_CR(type) = (count) | (mode);}

/**
 * Structure to hold the status of the DS' control buttons.
 */
typedef struct PAD
{
	union
	{
		struct
		{
			u16 A : 1;
			u16 B : 1;
			u16 Select : 1;
			u16 Start : 1;
			u16 Right : 1;
			u16 Left : 1;
			u16 Up : 1;
			u16 Down : 1;
			u16 R : 1;
			u16 L : 1;
			u16 X : 1;
			u16 Y : 1;
			u16 Touch : 1;
			u16 Lid : 1;
		};
		u16 AllKeys;
	};
} _pad;

/**
 * Structure holding a pad struct for each possible button state.
 */
typedef struct PADS {
   _pad Held, Released, Newpress;
} _pads;

/**
 * Struture holding the status of the stylus.
 */
typedef struct STYLUS {
	u8 Held : 1, Released : 1, Newpress : 1, unused : 5;
	s16 X, Y, Vx, Vy, oldVx, oldVy, Downtime, Uptime, DblClick;
} _stylus;

/**
 * Instance of the _pads struct allows access to the pad state.
 */
extern _pads Pad;

/**
 * Instance of the _stylus struct allows access to the stylus state.
 */
extern _stylus Stylus;

/**
 * Update the pad and stylus structs with the latest physical status.  Called
 * every frame by the VBL function.
 * @see woopsiWaitVBL().
 */
void woopsiUpdateInput();

#endif

/**
 * Pointers to the DS' framebuffers.
 */
extern u16* DrawBg[2];

/**
 * Initialise the DS' screens into framebuffer mode.  Also sets up some other subsystems
 * and IRQs.
 */
void initWoopsiGfxMode();

/**
 * Wait for a VBL.  Switches into a wait state if the lid is closed.
 */
void woopsiWaitVBL();

/**
 * Check if the lid is closed or not.
 * @return True if the lid is closed.
 */
bool woopsiLidClosed();

#endif
#endif
