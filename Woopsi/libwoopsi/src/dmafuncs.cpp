#include <nds.h>
#include "dmafuncs.h"

#ifdef USING_SDL

void woopsiDmaCopy(const u16* source, u16* dest, u32 count) {
	memcpy(dest, source, sizeof(u16) * count);
}

void woopsiDmaFill(u16 fill, u16* dest, u32 count) {
	for (u32 i = 0; i < count; i++) {
		*(dest + i) = fill;
	}
}

#else

void woopsiDmaCopy(const u16* source, u16* dest, u32 count) {

	// Get memory addresses of source and destination
	u32 srca = (u32)source;
	u32 dsta = (u32)dest;

	// Precalculate boundaries of framebuffer VRAM
	u32 bmp[2];
	bmp[0] = 0x06000000;
	bmp[1] = 0x06400000;

	// Use DMA hardware if both source and destination are within VRAM
    if ((dsta >= bmp[0]) && (dsta < bmp[1]) && (srca >= bmp[0]) && (srca < bmp[1])) {

		// libnds DMA functions work in bytes
		count *= 2;

		DC_FlushRange(source, count);

		// Choose fastest DMA copy mode
		if((srca|dsta|count) & 3)
			dmaCopyHalfWordsAsynch(3, source, dest, count);
		else
			dmaCopyWordsAsynch(3, source, dest, count);

		return;
	}

	// Cannot use DMA as not working exclusively with VRAM
	// Use for-loop instead
	for (u32 i = 0; i < count; i++) {
		*(dest + i) = *(source + i);
	}
}

void woopsiDmaFill(u16 fill, u16* dest, u32 count) {

	// Draw initial pixel
	*dest = fill;

	// Stop copying if there are no more pixels to draw
	if (count > 1) {

		u32 dsta = (u32)dest + 1;

		// Precalculate boundaries of framebuffer VRAM
		u32 bmp[2];
		bmp[0] = 0x06000000;
		bmp[1] = 0x06400000;

		// Use DMA hardware if destination is within VRAM
		if ((dsta >= bmp[0]) && (dsta < bmp[1])) {

			// libnds DMA functions work in bytes
			count *= 2;

			if((dsta|count) & 3)
				dmaFillHalfWords(fill, dest, count);
			else
				dmaFillWords(fill, dest, count);

			return;
		}
	}

	// Cannot use DMA as not working exclusively with VRAM
	// Use for-loop instead
	for (u32 i = 0; i < count; i++) {
		*(dest + i) = fill;
	}
}

#endif
