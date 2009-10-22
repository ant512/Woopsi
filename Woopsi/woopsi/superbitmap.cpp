#include "superbitmap.h"
#include "bitmap.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
SuperBitmap::SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, FontBase* font) : Gadget(x, y, width, height, GADGET_BORDERLESS, font) {
	
	_bitmap = new Bitmap(bitmapWidth, bitmapHeight);

	_bitmapX = 0;
	_bitmapY = 0;

	_outline = OUTLINE_OUT;

	_flags.decoration = isDecoration;

	if (isDecoration) {
		_flags.borderless = true;
		_flags.draggable = false;
	} else {
		_flags.draggable = true;
	}

	clearBitmap();
}

SuperBitmap::~SuperBitmap() {
	delete _bitmap;
}

void SuperBitmap::clearBitmap() {
	drawFilledRect(0, 0, _bitmap->getWidth(), _bitmap->getHeight(), _backColour);
}

void SuperBitmap::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	// Flush out the bitmap mem cache to ensure DMA can see correct data
	DC_FlushRange(_bitmap->getBitmap(), _bitmap->getWidth() * _bitmap->getHeight() * sizeof(u16));

	port->drawBitmap(x, y, _width, _height, _bitmap->getBitmap(), _bitmapX, _bitmapY, _bitmap->getWidth(), _bitmap->getHeight());

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
}

// Draw a single pixel to the bitmap
void SuperBitmap::drawPixel(s16 x, s16 y, u16 colour) {
	_bitmap->drawPixel(x, y, colour);
}

// Get a single pixel from the bitmap
const u16 SuperBitmap::getPixel(s16 x, s16 y) const {
	return _bitmap->getPixel(x, y);
}

// External filled rectangle function
void SuperBitmap::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	_bitmap->drawFilledRect(x, y, width, height, colour);
}

void SuperBitmap::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {
	_bitmap->drawHorizLine(x, y, width, colour);
}

void SuperBitmap::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {
	_bitmap->drawVertLine(x, y, height, colour);
}

void SuperBitmap::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	_bitmap->drawRect(x, y, width, height, colour);
}

void SuperBitmap::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
	_bitmap->drawLine(x1, y1, x2, y2, colour);
}

void SuperBitmap::drawCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	_bitmap->drawCircle(x0, y0, radius, colour);
}

void SuperBitmap::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	_bitmap->drawFilledCircle(x0, y0, radius, colour);
}

void SuperBitmap::drawText(s16 x, s16 y, FontBase* font, const char* string) {
	_bitmap->drawText(x, y, font, string);
}

// Print a string in a specific colour
void SuperBitmap::drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour) {
	_bitmap->drawText(x, y, font, string, colour);
}

bool SuperBitmap::drag(s16 x, s16 y, s16 vX, s16 vY) {

	if ((isEnabled()) && (_flags.dragging)) {
		_bitmapX -= vX;
		_bitmapY -= vY;

		// Prevent scrolling outside boundaries of bitmap
		if (_bitmapX < 0) {
			_bitmapX = 0;
		} else if (_bitmapX > _bitmap->getWidth() - _width) {
			_bitmapX = _bitmap->getWidth() - _width;
		}

		if (_bitmapY < 0) {
			_bitmapY = 0;
		} else if (_bitmapY > _bitmap->getHeight() - _height) {
			_bitmapY = _bitmap->getHeight() - _height;
		}

		// Redraw the gadget
		redraw();

		raiseDragEvent(x, y, vX, vY);

		return true;
	}

	return false;
}

// Scanline floodfill algorithm
void SuperBitmap::floodFill(s16 x, s16 y, u16 newColour) {
	_bitmap->floodFill(x, y, newColour);
}

//Draw bitmap to the internal bitmap
void SuperBitmap::drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight) {
	_bitmap->drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, bitmapWidth, bitmapHeight);
}

void SuperBitmap::drawBitmap(s16 x, s16 y, u16 width, u16 height, const Bitmap* bitmap, s16 bitmapX, s16 bitmapY) {
	_bitmap->drawBitmap(x, y, width, height, bitmap->getBitmap(), bitmapX, bitmapY, bitmap->getWidth(), bitmap->getHeight());
}

void SuperBitmap::drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {
	_bitmap->drawEllipse(xCentre, yCentre, horizRadius, vertRadius, colour);
}


void SuperBitmap::drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {
	_bitmap->drawFilledEllipse(xCentre, yCentre, horizRadius, vertRadius, colour);
}

const u16* SuperBitmap::getBitmap() const {
	return _bitmap->getBitmap();
}
