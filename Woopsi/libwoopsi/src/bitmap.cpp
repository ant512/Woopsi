#include "bitmap.h"
#include "woopsifuncs.h"
#include "graphics.h"
#include "rect.h"

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

// Draw a single pixel to the bitmap
void Bitmap::setPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	u32 pos = (y * _width) + x;
	_bitmap[pos] = colour;
}

const u16* Bitmap::getData(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap + pos;
}

Graphics* Bitmap::newGraphics() {
	Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = _width;
	rect.height = _height;
	return new Graphics(this, rect);
}

void Bitmap::blit(const s16 x, const s16 y, const u16* data, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(data, pos, size);
}

void Bitmap::blitFill(const s16 x, const s16 y, const u16 colour, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaFill(colour, pos, size);
}

void Bitmap::copy(s16 x, s16 y, u32 size, u16* dest) const {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(pos, dest, size);
}
