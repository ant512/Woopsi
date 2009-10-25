#include "packedfont1.h"

using namespace WoopsiUI;

void PackedFont1::renderChar(
		const u16* pixelData, u16 pixelsPerRow,
		u16* bitmap, u16 bitmapWidth,
		s16 x, s16 y,
		u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2)
{
	u16 colour = getColour();
	u16 curr;
	u16 mask;

	// adjust clipY2 to be the last row in the glyph
	// so we only write while (y=<clipY2)
	if (clipY2 > y + getHeight() - 1) {
		clipY2 = y + getHeight() - 1;
	}

	// setting mask to 0 forces read of next word. setting curr
	// avoids a compiler warning.
	mask = 0;
	curr = 0;

	// skip over font data corresponding to pixels ABOVE
	// the clipping rectangle.
	if (y < clipY1) {
		u16 rowsToSkip = clipY1-y;
		u16 pixelsToSkip = rowsToSkip * pixelsPerRow;
		u16 bitsToSkip = pixelsToSkip & 15;
		y = clipY1;
		bitmap += bitmapWidth * rowsToSkip;
		pixelData += pixelsToSkip / 16;		// skip over a bunch of u16s
		if (bitsToSkip) {
			curr = *pixelData++;
			mask = 0x8000 >> bitsToSkip;
		}
	}

	while (y <= clipY2) {
		// get pointer to next row
		u16 *row = bitmap;
		bitmap += bitmapWidth;

		u16 rowCount = pixelsPerRow;
		u16 rowX = x;
		while (rowCount-- > 0) {
			// if we have runout, get next chunk
			if (!mask) {
				mask = 0x8000;
				curr = *pixelData++;
			}

			// unpack next pixel
			if (curr & mask) {
				// if we need to, write it to the bitmap
				if (
					rowX >= clipX1 && rowX <= clipX2	// not clipped X-wise
				) {
					*row = colour;
				}
			}
			mask >>= 1;
			rowX++;
			row++;
		}
		y++;
	}
}
