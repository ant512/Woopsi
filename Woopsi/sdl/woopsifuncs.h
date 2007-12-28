#ifndef _WOOPSIFUNCS_H_
#define _WOOPSIFUNCS_H_

#include <nds.h>
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

#endif
