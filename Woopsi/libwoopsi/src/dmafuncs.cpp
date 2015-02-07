#include <nds.h>
#include "dmafuncs.h"

#ifndef USING_SDL

static const u32 MEM_VRAM_START = 0x06000000;
static const u32 MEM_VRAM_END = 0x06400000;

#endif

void woopsiDmaCopy(const u16* source, u16* dest, u32 count) {

#ifdef USING_SDL

    memcpy(dest, source, sizeof(u16) * count);

#else

    // Get memory addresses of source and destination
    u32 srca = (u32)source;
    u32 dsta = (u32)dest;

    // Use DMA hardware if both source and destination are within VRAM
    if (0 && (dsta >= MEM_VRAM_START) && (dsta < MEM_VRAM_END) && (srca >= MEM_VRAM_START) && (srca < MEM_VRAM_END)) {

        // libnds DMA functions work in bytes
        count *= 2;

        DC_FlushRange(source, count);

        while (dmaBusy(3)) {}

        // Choose fastest DMA copy mode
        if ((srca | dsta | count) & 3) {
            dmaCopyHalfWordsAsynch(3, source, dest, count);
        } else {
            dmaCopyWordsAsynch(3, source, dest, count);
        }
    } else {
        // Cannot use DMA as not working exclusively with VRAM
		for (u32 i = 0; i < count; i++) {
			*(dest + i) = *(source + i);
		}
    }

#endif

}

void woopsiDmaFill(u16 fill, u16* dest, u32 count) {

#ifdef USING_SDL

    for (u32 i = 0; i < count; i++) {
        *(dest + i) = fill;
    }

#else

    // Draw initial pixel
    *dest = fill;

    // Stop copying if there are no more pixels to draw
    if (count > 1) {

        u32 dsta = (u32)dest + 1;

        // Use DMA hardware if destination is within VRAM
        if ((dsta >= MEM_VRAM_START) && (dsta < MEM_VRAM_END)) {

            // libnds DMA functions work in bytes
            count *= 2;

            while (dmaBusy(3)) {}

            if ((dsta | count) & 3) {
                dmaFillHalfWords(fill, dest, count);
            } else {
                dmaFillWords(fill, dest, count);
            }

            return;
        }
    }

    // Cannot use DMA as not working exclusively with VRAM
    for (u32 i = 0; i < count; i++) {
        *(dest + i) = fill;
    }

#endif

}