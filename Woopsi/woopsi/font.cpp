#include "font.h"

Font::Font(const u16* bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour) : FontBase(bitmapWidth, bitmapHeight, width, height, transparentColour) {
	_bitmap = bitmap;

	setTransparentColour(transparentColour);
	createGlyphMap();
}

// Return a pixel at a given location
const u16 Font::getPixel(const u32 position) const {
	if (!isMonochrome()) {
		return _bitmap[position];
	} else if (_bitmap[position] != getTransparentColour()) {
		return getColour();
	}

	return getTransparentColour();
}

// Scan a glyph for any data that can be drawn
const bool Font::scanGlyph(const u16 x, const u16 y) const {
	u16 maxX = x + getWidth();
	u16 maxY = y + getHeight();
	u16 bitmapWidth = getBitmapWidth();
	u16 bitmapHeight = getBitmapHeight();
	u32 bitmapSize = bitmapWidth * bitmapHeight;

	for (u16 x2 = x; x2 < maxX; x2++) {
		for (u16 y2 = y; y2 < maxY; y2++) {

		    u16 pixel = x2 + (y2 * bitmapWidth);

		    // Check pixel is within bitmap boundaries
		    if (pixel > bitmapSize) return false;

			if (_bitmap[pixel] != getTransparentColour()) {
				// Got some data
				return true;
			}
		}
	}

	// No data
	return false;
}
