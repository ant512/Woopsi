#include "skinnedwindowborderright.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedWindowBorderRight::SkinnedWindowBorderRight(s16 x, s16 y, u16 height, const WindowSkin* skin) : WindowBorderSide(x, y, 0, height) {

	_skin = skin;

	// Parse skin information
	_width = _skin->rightBorder.bitmap.width;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;
}

void SkinnedWindowBorderRight::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_parent->hasFocus()) {
		port->drawBitmap(0, 0, _skin->rightBorder.bitmap.width,
							   _height,
							   _skin->rightBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->rightBorder.bitmap.width,
							   _skin->rightBorder.bitmap.height);
	} else {
		port->drawBitmap(0, 0, _skin->rightBorder.bitmap.width,
							   _height,
							   _skin->rightBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->rightBorder.bitmap.width,
							   _skin->rightBorder.bitmap.height);
	}

	// Tidy up
	delete port;
}
