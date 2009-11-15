#include "packedfont16.h"
#include "mutablebitmapbase.h"

using namespace WoopsiUI;

//
// pixeldata is an array of u16 values, each of which represents
// a single pixel.
//
void PackedFont16::renderChar(
		const u16* pixelData, u16 pixelsPerRow,
		MutableBitmapBase* bitmap,
		s16 x, s16 y,
		u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2)
{
	u16 colour = getColour();
	u16 bitmapWidth = bitmap->getWidth();

	// adjust clipY2 to be the last row+1 in the glyph
	// so we only write while (y<=clipY2)
	if (clipY2 > y + getHeight() - 1) {
		clipY2 = y + getHeight() - 1;
	}

	// skip over font data corresponding to pixels ABOVE
	// the clipping rectangle.
	if (y < clipY1) {
		u16 rowsToSkip = clipY1 - y;
		u16 pixelsToSkip = rowsToSkip * pixelsPerRow;
		y = clipY1;
		pixelData += pixelsToSkip;
	}

	// now output pixels till we reach the end
	while (y <= clipY2) {
		// get pointer to next row in output
		u16 rowCount = pixelsPerRow;
		u16 rowX = x;
		while (rowCount-- > 0) {
			// get next pixel
			u16 pixel = *pixelData++;
			// if we need to, write it to the bitmap
			if (
				pixel				// non-transparent pixel
				&&
				rowX >= clipX1 && rowX <= clipX2	// not clipped X-wise
			) {
				bitmap->setPixel(rowX, y, colour ? colour : pixel);
			}
			rowX++;
		}

		// move output pointer down one row
		y++;
	}
}
