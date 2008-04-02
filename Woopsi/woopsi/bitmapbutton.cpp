#include "bitmapbutton.h"
#include "graphicsport.h"

BitmapButton::BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const u16* bitmapNormal, const u16* bitmapClicked) : Textbox(x, y, width, height, "", NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;
	_bitmapX = bitmapX;
	_bitmapY = bitmapY;
	_bitmapClicked = bitmapClicked;
	_bitmapNormal = bitmapNormal;
}

void BitmapButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	if (_flags.clicked) {
		port->drawBitmap(x, y, _width, _height, _bitmapClicked, _bitmapX, _bitmapY, _bitmapWidth, _bitmapHeight);
	} else {
		port->drawBitmap(x, y, _width, _height, _bitmapNormal, _bitmapX, _bitmapY, _bitmapWidth, _bitmapHeight);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}
