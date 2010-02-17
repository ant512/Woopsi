#include "superbitmap.h"
#include "bitmap.h"
#include "bitmapbase.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
SuperBitmap::SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, GadgetStyle* style) : Gadget(x, y, width, height, GADGET_BORDERLESS, style) {
	
	_bitmap = new Bitmap(bitmapWidth, bitmapHeight);
	_graphics = _bitmap->newGraphics();

	_bitmapX = 0;
	_bitmapY = 0;

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
	delete _graphics;
	delete _bitmap;
}

void SuperBitmap::clearBitmap() {
	_graphics->drawFilledRect(0, 0, _bitmap->getWidth(), _bitmap->getHeight(), getBackColour());
}

void SuperBitmap::drawContents(GraphicsPort* port) {

	Rect rect;
	getClientRect(rect);

	port->drawBitmap(0, 0, rect.width, rect.height, _bitmap, _bitmapX, _bitmapY);
}

void SuperBitmap::drawBorder(GraphicsPort* port) {
	port->drawRect(0, 0, _width, _height, getShadowColour());
}

void SuperBitmap::onClick(s16 x, s16 y) {
	startDragging(x, y);
}

void SuperBitmap::onDrag(s16 x, s16 y, s16 vX, s16 vY) {

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
}

Bitmap* SuperBitmap::getBitmap() {
	return _bitmap;
}

Graphics* SuperBitmap::getGraphics() {
	return _graphics;
}
