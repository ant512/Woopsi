#include "graphics.h"
#include "textwriter.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
Graphics::Graphics(MutableBitmapBase* bitmap) : GraphicsUnclipped(bitmap) { }

// Draw a single pixel to the bitmap
void Graphics::drawPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	GraphicsUnclipped::drawPixel(x, y, colour);
}

// Get a single pixel from the bitmap
const u16 Graphics::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	return GraphicsUnclipped::getPixel(x, y);
}

// External filled rectangle function
void Graphics::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawFilledRect(x, y, width, height, colour);
	}
}

void Graphics::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	u16 height = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawHorizLine(x, y, width, colour);
	}
}

void Graphics::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {

	u16 width = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		Graphics::drawVertLine(x, y, height, colour);
	}
}

void Graphics::drawXORHorizLine(s16 x, s16 y, u16 width) {

	u16 height = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawXORHorizLine(x, y, width);
	}
}

void Graphics::drawXORVertLine(s16 x, s16 y, u16 height) {

	u16 width = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		Graphics::drawXORVertLine(x, y, height);
	}
}

// Scanline floodfill algorithm
void Graphics::floodFill(s16 x, s16 y, u16 newColour) {

	// Clip to bitmap
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	GraphicsUnclipped::floodFill(x, y, newColour);
}

//Draw bitmap to the internal bitmap
void Graphics::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY) {
	
	u16 bitmapWidth = bitmap->getWidth();
	u16 bitmapHeight = bitmap->getHeight();
	const u16* bitmapData = bitmap->getData();

	// Flush out the bitmap mem cache to ensure DMA can see correct data
	DC_FlushRange(bitmapData, bitmapWidth * bitmapHeight * sizeof(u16));

	// Ensure bitmap co-ordinates make sense
	if (bitmapX < 0) {
		bitmapX = 0;
	}

	if (bitmapY < 0) {
		bitmapY = 0;
	}

	// Ensure dimensions of bitmap being drawn do not exceed size of bitmap RAM
	if (x < 0) {
		bitmapX -= x;
		width += x;
		x = 0;
	}

	if (y < 0) {
		bitmapY -= y;
		height += y;
		y = 0;
	}

	if (x + width > _width) {
		width = _width - x;
	}

	if (y + height > _height) {
		height = _height - y;
	}

	// Ensure requested drawing dimensions do not exceed dimensions of bitmap
	if (width > bitmapWidth - bitmapX) {
		width = bitmapWidth - bitmapX;
	}

	if (height > bitmapHeight - bitmapY) {
		height = bitmapHeight - bitmapY;
	}

	if ((width > 0) && (height > 0)) {
		GraphicsUnclipped::drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY);
	}
}

// Clip co-ordinates
bool Graphics::clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height) {

	s32 x2 = *x + *width - 1;
	s32 y2 = *y + *height - 1;

	// Ensure values don't exceed bitmap dimensions
	if (*x < 0) *x = 0;
	if (*y < 0) *y = 0;
	if (x2 > _width - 1) x2 = _width - 1;
	if (y2 > _height - 1) y2 = _height - 1;

	// Return false if no box to draw
	if ((x2 < *x) || (y2 < *y)) return false;

	// Update pointer values
	*width = (x2 - *x) + 1;
	*height = (y2 - *y) + 1;

	// Return true as box can be drawn
	return true;
}
