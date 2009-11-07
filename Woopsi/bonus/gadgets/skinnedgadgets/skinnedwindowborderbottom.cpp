#include "skinnedwindowborderbottom.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedWindowBorderBottom::SkinnedWindowBorderBottom(s16 x, s16 y, u16 width, const WindowSkin* skin) : WindowBorderBottom(x, y, width, 0, 0) {

	_skin = skin;

	// Parse skin information
	_height = _skin->bottomCentreBorder.bitmap.height;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;
	_sideBorderSize = _skin->leftBorder.bitmap.width;
}

void SkinnedWindowBorderBottom::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_parent->hasFocus()) {
		port->drawBitmap(0, 0, _skin->bottomLeftBorder.bitmap.width,
							   _skin->bottomLeftBorder.bitmap.height,
							   _skin->bottomLeftBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->bottomLeftBorder.bitmap.width,
							   _skin->bottomLeftBorder.bitmap.height);

		port->drawBitmap(_skin->bottomLeftBorder.bitmap.width, 0,
							   _width - _skin->bottomLeftBorder.bitmap.width - _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomCentreBorder.bitmap.height,
							   _skin->bottomCentreBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->bottomCentreBorder.bitmap.width,
							   _skin->bottomCentreBorder.bitmap.height);

		port->drawBitmap(_width - _skin->bottomRightBorder.bitmap.width,
							   0,
							   _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomRightBorder.bitmap.height,
							   _skin->bottomRightBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomRightBorder.bitmap.height);
	} else {
		port->drawBitmap(0, 0, _skin->bottomLeftBorder.bitmap.width,
							   _skin->bottomLeftBorder.bitmap.height,
							   _skin->bottomLeftBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->bottomLeftBorder.bitmap.width,
							   _skin->bottomLeftBorder.bitmap.height);

		port->drawBitmap(_skin->bottomLeftBorder.bitmap.width, 0,
							   _width - _skin->bottomLeftBorder.bitmap.width - _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomCentreBorder.bitmap.height,
							   _skin->bottomCentreBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->bottomCentreBorder.bitmap.width,
							   _skin->bottomCentreBorder.bitmap.height);

		port->drawBitmap(_width - _skin->bottomRightBorder.bitmap.width,
							   0,
							   _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomRightBorder.bitmap.height,
							   _skin->bottomRightBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->bottomRightBorder.bitmap.width,
							   _skin->bottomRightBorder.bitmap.height);
	}

	// Tidy up
	delete port;
}
