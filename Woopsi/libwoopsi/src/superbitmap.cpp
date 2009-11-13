#include "superbitmap.h"
#include "bitmap.h"
#include "bitmapbase.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
SuperBitmap::SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, FontBase* font) : Gadget(x, y, width, height, GADGET_BORDERLESS, font) {
	
	_bitmap = new Bitmap(bitmapWidth, bitmapHeight);
	_graphics = _bitmap->newGraphics();

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
	delete _graphics;
	delete _bitmap;
}

void SuperBitmap::clearBitmap() {
	_graphics->drawFilledRect(0, 0, _bitmap->getWidth(), _bitmap->getHeight(), _backColour);
}

void SuperBitmap::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	port->drawBitmap(x, y, _width, _height, _bitmap, _bitmapX, _bitmapY);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
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

Bitmap* SuperBitmap::getBitmap() {
	return _bitmap;
}

Graphics* SuperBitmap::getGraphics() {
	return _graphics;
}
