#include "fontbase.h"

FontBase::FontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 height, const u16 transparentColour) {
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;
	_height = height;
	_drawColour = 0;
	_isMonochrome = false;
}

void FontBase::setColour(const u16 colour) {
	_drawColour = colour;
	_isMonochrome = true;
}

// Initialise the glyph map array
void FontBase::initGlyphMap() {
	for (u8 i = 0; i < GLYPH_MAP_SIZE; i++) {
		_glyphMap[i] = 0;
	}
}
