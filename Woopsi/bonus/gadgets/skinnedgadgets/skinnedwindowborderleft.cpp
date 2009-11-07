#include "skinnedwindowborderleft.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedWindowBorderLeft::SkinnedWindowBorderLeft(s16 x, s16 y, u16 height, const WindowSkin* skin) : WindowBorderSide(x, y, 0, height) {

	_skin = skin;

	// Parse skin information
	_width = _skin->leftBorder.bitmap.width;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;
}

void SkinnedWindowBorderLeft::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_parent->hasFocus()) {
		port->drawBitmap(0, 0, _skin->leftBorder.bitmap.width,
							   _height,
							   _skin->leftBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->leftBorder.bitmap.width,
							   _skin->leftBorder.bitmap.height);
	} else {
		port->drawBitmap(0, 0, _skin->leftBorder.bitmap.width,
							   _height,
							   _skin->leftBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->leftBorder.bitmap.width,
							   _skin->leftBorder.bitmap.height);
	}

	// Tidy up
	delete port;
}
