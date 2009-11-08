#include "bitmap.h"
#include "textwriter.h"
#include "woopsifuncs.h"
#include "graphics.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
Bitmap::Bitmap(u16 width, u16 height) {
	_width = width;
	_height = height;

	// Allocate memory for bitmap
	_bitmap = new u16[_width * _height];
}

// Get a single pixel from the bitmap
const u16 Bitmap::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap[pos];
}

Graphics* Bitmap::newGraphics() {
	return new Graphics(this);
}
