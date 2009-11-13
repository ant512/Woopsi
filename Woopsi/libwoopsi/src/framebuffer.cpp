#include "framebuffer.h"
#include "graphics.h"
#include "woopsifuncs.h"

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

const u16* FrameBuffer::getData(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _bitmap + pos;
}

Graphics* FrameBuffer::newGraphics() {
	return new Graphics(this);
}

void FrameBuffer::blit(const s16 x, const s16 y, u16* data, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaCopy(data, pos, size);
}

void FrameBuffer::blitFill(const s16 x, const s16 y, u16 colour, const u32 size) {
	u16* pos = _bitmap + (y * _width) + x;
	woopsiDmaFill(colour, pos, size);
}
