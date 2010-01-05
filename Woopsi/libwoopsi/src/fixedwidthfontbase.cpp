#include "fixedwidthfontbase.h"
#include "woopsistring.h"

using namespace WoopsiUI;

FixedWidthFontBase::FixedWidthFontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour) : 
	FontBase(height, transparentColour) {
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;
	_width = width;
}

u16 FixedWidthFontBase::getStringWidth(const WoopsiString& text) const {
	return text.getLength() * getWidth();
}

void FixedWidthFontBase::createGlyphMap() {

	initGlyphMap();

	// Calculate the number of rows in the bitmap
	u16 rows = getBitmapHeight() / getHeight();
	
	// Calculate the number of columns in the bitmap
	u16 columns = getBitmapWidth() / getWidth();
	
	// Loop through each row of glyphs
	for (u16 row = 0; row < rows; row++) {
	
		// Loop through each column of glyphs
		for (u16 column = 0; column < columns; column++) {
		
			// Get the co-ordinates of the glyph
			u16 glyphX = column * getWidth();
			u16 glyphY = row * getHeight();
			u8 glyph = column + (row * columns);
			
			// Scan the glyph
			if (scanGlyph(glyphX, glyphY)) {
			
				// Glyph contains valid data - set the relevant bit
				_glyphMap[glyph >> 3] |= (1 << (glyph % 8));
			}
		}
	}
}

// Initialise the glyph map array
void FixedWidthFontBase::initGlyphMap() {
	for (u8 i = 0; i < GLYPH_MAP_SIZE; i++) {
		_glyphMap[i] = 0;
	}
}
