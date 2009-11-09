#include "bitmapwrapper.h"

using namespace WoopsiUI;

BitmapWrapper::BitmapWrapper(const u16* data, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
}

// Get a single pixel from the bitmap
const u16 BitmapWrapper::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap[pos];
}
