/* Contributed by Steven
 * 20071213
 * - Minor modifications and SDL code by Ant
 */

#ifndef _WOOPSIFUNCS_H_
#define _WOOPSIFUNCS_H_

#include <nds.h>
#include "framebuffer.h"
#include "font.h"
#include "monofont.h"
#include "bitmapwrapper.h"

// DMA handling bitmasks
#define DMA_ON 0x80000000
#define DMA_NOW 0x00000000
#define DMA_16 0x00000000
#define DMA_32 0x04000000
#define DMA_16NOW (DMA_ON | DMA_NOW | DMA_16)
#define DMA_32NOW (DMA_ON | DMA_NOW | DMA_32)

/**
 * Converts separate RGB component values into a single 16-bit value for use
 * with the DS' framebuffer.  All supplied values should be 5 bits wide (ie.
 * between 0 and 31).
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return The 15-bit colour value with the top alpha bit set.
 */
#define woopsiRGB(r, g, b) (((b) << 10) | ((g) << 5) | (r) | (1 << 15))

/**
 * Structure to hold the status of the DS' control buttons.
 */
typedef struct PAD
{
	union
	{
		struct
		{
			u16 A:1;			/**< A button */
			u16 B:1;			/**< B button */
			u16 Select:1;		/**< Select button */
			u16 Start:1;		/**< Start button */
			u16 Right:1;		/**< Right d-pad */
			u16 Left:1;			/**< Left d-pad */
			u16 Up:1;			/**< Up d-pad */
			u16 Down:1;			/**< Down d-pad */
			u16 R:1;			/**< R button */
			u16 L:1;			/**< L button */
			u16 X:1;			/**< X button */
			u16 Y:1;			/**< Y button */
			u16 Touch:1;		/**< Stylus touch */
			u16 Lid:1;			/**< Lid state */
		};
		u16 AllKeys;			/**< Bitmask representing entire control */
	};
} _pad;

/**
 * Structure holding a pad struct for each possible button state.
 */
typedef struct PADS {
   _pad Held;					/**< Each value set to 1 represents a held button */
   _pad Released;				/**< Inverse of held */
   _pad Newpress;				/**< Each value set to 1 represents a newly pressed button */
} _pads;

/**
 * Struture holding the status of the stylus.
 */
typedef struct STYLUS {
	u8 Held:1;					/**< Stylus is held down */
	u8 Released:1;				/**< Inverse of held */
	u8 Newpress:1;				/**< Stylus has been newly pressed */
	u8 unused:5;				/**< Padding bits */
	s16 X;						/**< X co-ord of the stylus */
	s16 Y;						/**< Y co-ord of the stylus */
	s16 Vx;						/**< X distance the stylus has moved in the last vblank */
	s16 Vy;						/**< Y distance the stylus has moved in the last vblank */
	s16 oldVx;
	s16 oldVy;
	s16 Downtime;				/**< Number of vblanks stylus has been held */
	s16 Uptime;					/**< Number of vblanks stylus has been released */
	s16 DblClick;				/**< Indicates a double click */
} _stylus;

/**
 * Instance of the _pads struct allows access to the pad state.
 */
extern _pads Pad;

/**
 * Instance of the _stylus struct allows access to the stylus state.
 */
extern _stylus Stylus;

#ifdef USING_SDL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void DMA_Copy(u16* source, u16* dest, u32 count, u32 mode);
void DMA_Force(u16 source, u16* dest, u32 count, u32 mode);
bool DMA_Active();

void putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

// Simulate some features of libnds
void DC_FlushRange(const void *base, u32 size);

#else

#include <nds/memory.h>
#include <nds/bios.h>

// DMA registers
#define REG_DMA3SRC *(volatile u32*)0x040000D4
#define REG_DMA3DST *(volatile u32*)0x040000D8
#define REG_DMA3CNT *(volatile u32*)0x040000DC

/**
 * Check if the first DMA channel is active.  DMA access is asynchronous so DMA
 * operations can run whilst the rest of the program is running, so it is
 * important to check that a channel is inactive before using it.
 * @return True if the channel is still active.
 */
#define DMA_Active() (!(!(REG_DMA3CNT & DMA_ON)))

/**
 * Perform a DMA copy using the first DMA channel.
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to copy.
 * @param mode The DMA mode to use.
 */
#define DMA_Copy(source, dest, count, mode) {REG_DMA3SRC = (u32)source; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}

/**
 * Perform a DMA force using the first DMA channel.
 * @param source The source memory address.
 * @param dest The destination memory address.
 * @param count The number of values to write.
 * @param mode The DMA mode to use.
 */
#define DMA_Force(ulVal, dest, count, mode) {REG_DMA3SRC=(u32)&ulVal; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode) | DMA_SRC_FIX;}

#endif

/**
 * Pointers to the DS' framebuffers.
 */
extern WoopsiUI::FrameBuffer* frameBuffer[2];

/**
 * Pointers to Woopsi system fonts.
 */
extern WoopsiUI::BitmapWrapper* systemFontBitmap;
extern WoopsiUI::Font* systemFont;
extern WoopsiUI::MonoFont* tinyFont;

/**
 * Initialise the DS' screens into framebuffer mode.  Also sets up some other subsystems
 * and IRQs.
 */
void initWoopsiGfxMode();

/**
 * Initialise the Woopsi system fonts.
 */
void woopsiInitFonts();

/**
 * Delete the Woopsi system fonts.
 */
void woopsiFreeFonts();

/**
 * Wait for a VBL.  Switches into a wait state if the lid is closed.
 */
void woopsiWaitVBL();

/**
 * Check if the lid is closed or not.
 * @return True if the lid is closed.
 */
bool woopsiLidClosed();

/**
 * Update the pad and stylus structs with the latest physical status.  Called
 * every frame by the VBL function.
 * @see woopsiWaitVBL().
 */
void woopsiUpdateInput();

void woopsiDmaCopy(const u16* source, u16* dest, u32 count);

void woopsiDmaFill(u16 fill, u16* dest, u32 count);

#endif
