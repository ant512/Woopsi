#include "monofont.h"
#include "mutablebitmapbase.h"

using namespace WoopsiUI;

MonoFont::MonoFont(const u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, u8 width, u8 height, u16 drawColour) : FixedWidthFontBase(bitmapWidth, bitmapHeight, width, height) {
	_bitmap = bitmap;

	setTransparentColour(0);
	setColour(drawColour);
	createGlyphMap();
}

// Return a pixel at a given location 
const u16 MonoFont::getPixel(const s16 x, const s16 y) const {

	u32 position = (y * getBitmapWidth()) + x;

	if (_bitmap[position >> 4] & (1 << (16 - (position % 16)))) {
		// Got some data
		return getColour();
	}

	return getTransparentColour();
}

// Scan a glyph for any data that can be drawn
const bool MonoFont::scanGlyph(const u16 x, const u16 y) const {
	u16 maxX = x + getWidth();
	u16 maxY = y + getHeight();
	u16 bitmapWidth = getBitmapWidth();
	u32 pos = 0;

	for (u16 x2 = x; x2 < maxX; x2++) {
		for (u16 y2 = y; y2 < maxY; y2++) {
			
			pos = x2 + (y2 * bitmapWidth);

			if (_bitmap[pos >> 4] & (1 << (16 - (pos % 16)))) {
				// Got some data
				return true;
			}
		}
	}
	
	// No data
	return false;
}

s16 MonoFont::drawChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {

	u8 fontWidth = getWidth();
	u8 fontHeight = getHeight();
	s16 output = x + fontWidth;

	// Early exit if the char will be printed outside the clipping region
	if (y > clipY2) return output;
	if (y + fontHeight < clipY1) return output;
	if (x > clipX2) return output;
	if (x + fontWidth < clipX1) return output;

	// Check that the font has data for the glyph
	if (!isCharBlank(letter)) {

		// Extract data from font object for extra speed
		u16 transparentColour = getTransparentColour();
		u16 fontBitmapWidth = getBitmapWidth();
	
		// Calculate the number of columns in the bitmap
		u16 columns = fontBitmapWidth / fontWidth;

		// Calculate the position of the letter in the font bitmap
		u16 letterY = (letter / columns) * fontHeight;
		u16 letterX = (letter % columns) * fontWidth;

		// Calculate values for clipping
		s16 startX = x > clipX1 ? x : clipX1;
		s16 endX = x + fontWidth - 1 < clipX2 ? x + fontWidth - 1: clipX2;
		s16 startY = y > clipY1 ? y : clipY1;
		s16 endY = y + fontHeight - 1 < clipY2 ? y + fontHeight - 1: clipY2;

		// Calculate clipping starting offsets - tells us the pixel offset
		// from the top-left of the character that we start drawing from
		u16 offsetStartX = x > clipX1 ? 0 : clipX1 - x;
		u16 offsetStartY = y > clipY1 ? 0 : clipY1 - y;

		// Calculate dimensions of region to copy
		u16 clipWidth = (endX - startX) + 1;
		u16 clipHeight = (endY - startY) + 1;

		// Ensure region to be drawn does not exceed the size of the character
		if (clipWidth > fontWidth - offsetStartX) clipWidth = fontWidth - offsetStartX;
		if (clipHeight > fontHeight - offsetStartY) clipHeight = fontHeight - offsetStartY;

		// Abort if there is no copying to be done
		if ((clipWidth == 0) || (clipHeight == 0)) return x + fontWidth;

		// Copy the pixel data
		u16 sourceColour;

		for (u16 pY = 0; pY < clipHeight; ++pY) {
			for (u16 pX = 0; pX < clipWidth; ++pX) {

				sourceColour = getPixel(letterX + offsetStartX + pX, letterY + offsetStartY + pY);

				if (sourceColour != transparentColour) {
					bitmap->setPixel(x + offsetStartX + pX, y + offsetStartY + pY, sourceColour);
				}
			}
		}
	}

	return x + getWidth();
}
