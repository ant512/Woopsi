#include "monofont.h"

using namespace WoopsiUI;

MonoFont::MonoFont(const u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, u8 width, u8 height, u16 drawColour) : FixedWidthFontBase(bitmapWidth, bitmapHeight, width, height) {
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

s16 MonoFont::drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {

	// Check that the font has data for the glyph
	if (!isCharBlank(letter)) {
	
		u16 letterStart = letter * getWidth();
		u8 letterStartY = 0;
		u16 sourcePixel;

		// Extract data from font object for extra speed
		u16 transparentColour = getTransparentColour();
		u8 fontWidth = getWidth();
		u8 fontHeight = getHeight();
		u16 fontBitmapWidth = getBitmapWidth();

		// Calculate the start position of the letter in the font bitmap
		letterStartY = letterStart / fontBitmapWidth;
		letterStart %= fontBitmapWidth;

		letterStart += fontHeight * (letterStartY * fontBitmapWidth);

		// TODO: Finish per-row/per-column clipping here
		// Calculate values for clipping
		s16 startX = x > clipX1 ? x : clipX1;
		s16 endX = x + fontWidth - 1 < clipX2 ? x + fontWidth - 1: clipX2;
		s16 startY = y > clipY1 ? y : clipY1;
		s16 endY = y + fontHeight - 1 < clipY2 ? y + fontHeight - 1: clipY2;

		// Draw letter to the screen in rows

		u16 sourceColour = 0;

		for (s16 destPixelY = startY; destPixelY <= endY; destPixelY++) {

			for (s16 destPixelX = startX; destPixelX <= endX; destPixelX++) {

				// Get the array index of the pixel to copy
				sourcePixel = letterStart + (destPixelX - x) + ((destPixelY - y) * fontBitmapWidth);

				sourceColour = getPixel(sourcePixel);

				// Is the pixel part of the font, not the background?
				if (sourceColour != transparentColour) {

					// Draw text pixel
					bitmap[destPixelX + (destPixelY * bitmapWidth)] = sourceColour;
				}
			}
		}
	}

	return x + getWidth();
}
