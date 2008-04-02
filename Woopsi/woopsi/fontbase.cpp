#include "fontbase.h"

FontBase::FontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour) {
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;
	_width = width;
	_height = height;
	_drawColour = 0;
	_isMonochrome = false;
}

const u16 FontBase::getBitmapWidth() const {
	return _bitmapWidth;
}

const u16 FontBase::getBitmapHeight() const {
	return _bitmapHeight;
}

const u8 FontBase::getWidth() const {
	return _width;
}

const u8 FontBase::getHeight() const {
	return _height;
}

const bool FontBase::isMonochrome() const {
	return _isMonochrome;
}

const u16 FontBase::getColour() const {
	return _drawColour;
}

// Return a pixel at a given location 
const u16 FontBase::getPixel(const u16 x, const u16 y) const {
	return getPixel(x + (y * _width));
}

const u16 FontBase::getTransparentColour() const {
	return _transparentColour;
}

void FontBase::setTransparentColour(const u16 colour) {
	_transparentColour = colour;
}

void FontBase::setColour(const u16 colour) {
	_drawColour = colour;
	_isMonochrome = true;
}

void FontBase::clearColour() {
	_isMonochrome = false;
}

// Does the supplied glyph contain any drawable data?
const bool FontBase::checkGlyphData(char glyph) const {
	return (_glyphMap[glyph >> 3] & (1 << (glyph % 8)));
}

// Initialise the glyph map array
void FontBase::initGlyphMap() {
	for (u8 i = 0; i < GLYPH_MAP_SIZE; i++) {
		_glyphMap[i] = 0;
	}
}

void FontBase::createGlyphMap() {

	initGlyphMap();

	// Calculate the number of rows in the bitmap
	u16 rows = _bitmapHeight / _height;
	
	// Calculate the number of columns in the bitmap
	u16 columns = _bitmapWidth / _width;
	
	// Loop through each row of glyphs
	for (u16 row = 0; row < rows; row++) {
	
		// Loop through each column of glyphs
		for (u16 column = 0; column < columns; column++) {
		
			// Get the co-ordinates of the glyph
			u16 glyphX = column * _width;
			u16 glyphY = row * _height;
			u8 glyph = column + (row * columns);
			
			// Scan the glyph
			if (scanGlyph(glyphX, glyphY)) {
			
				// Glyph contains valid data - set the relevant bit
				_glyphMap[glyph >> 3] |= (1 << (glyph % 8));
			}
		}
	}
}

// Calculate the width in pixels of a string when printed using this font
const u8 FontBase::getTextPixelWidth(const char* text) const {
	return strlen(text) * _width;
}
