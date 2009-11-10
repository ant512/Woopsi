#include "framebuffer.h"
#include "graphics.h"

using namespace WoopsiUI;

FrameBuffer::FrameBuffer(u16* data, u16 width, u16 height) {
	_width = width;
	_height = height;
	_bitmap = data;
}

// Get a single pixel from the bitmap
const u16 FrameBuffer::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap[pos];
}

// Draw a single pixel to the bitmap
void FrameBuffer::setPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	u32 pos = (y * _width) + x;
	_bitmap[pos] = colour;
}

Graphics* FrameBuffer::newGraphics() {
	return new Graphics(this);
}
