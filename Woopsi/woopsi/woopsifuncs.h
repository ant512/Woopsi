/* Contributed by Steven
 * 20071213
 * - Minor modifications by Ant
 */

#ifndef _WOOPSIFUNCS_H_
#define _WOOPSIFUNCS_H_

#define woopsiRGB(r, g, b) (((b) << 10) | ((g) << 5) | (r) | (1 << 15))

#ifdef USING_PALIB
#include <PA9.h>
#else

#include <nds.h>
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
#define DMA_Copy(source, dest, count, mode) {REG_DMA3SRC = (u32)source; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}
#define DMA_Clear(dest, count, mode) {REG_DMA3SRC = (u32)Blank; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode);}
#define DMA_Force(ulVal, dest, count, mode) {REG_DMA3SRC=(u32)&ulVal; REG_DMA3DST = (u32)dest; REG_DMA3CNT = (count) | (mode) | DMA_SRC_FIX;}
#define DMA_CopyEx(type, source, dest, count, mode) {DMA_SRC(type) = (u32)source; DMA_DEST(type) = (u32)dest; DMA_CR(type) = (count) | (mode);}

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

#endif

extern u16* DrawBg[2];

void initWoopsiGfxMode();
void woopsiWaitVBL();
bool woopsiLidClosed();

#endif
