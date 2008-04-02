#include "monofont.h"

MonoFont::MonoFont(const u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, u8 width, u8 height, u16 drawColour) : FontBase(bitmapWidth, bitmapHeight, width, height) {
	_bitmap = bitmap;

	setTransparentColour(0);
	setColour(drawColour);
	createGlyphMap();
}

// Return a pixel at a given location 
const u16 MonoFont::getPixel(const u32 position) const {
	if (_bitmap[position >> 4] & (1 << (16 - (position % 16)))) {
		// Got some data
		return getColour();
	}

	return getTransparentColour();
}

// Scan a glyph for any data that can be drawn
const bool MonoFont::scanGlyph(const u16 x, const u16 y) const {
	u8 maxX = x + getWidth();
	u8 maxY = y + getHeight();
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
