#include "skinnedwindowbordertop.h"
#include "graphicsport.h"
#include "skinnedwindow.h"
#include "window.h"

using namespace WoopsiUI;

SkinnedWindowBorderTop::SkinnedWindowBorderTop(s16 x, u16 width, Window* window, const WindowSkin* skin) : WindowBorderTop(x, width, 0, window) {

	_skin = skin;

	// Parse skin information
	_height = _skin->topCentreBorder.bitmap.height;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;
}

void SkinnedWindowBorderTop::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_parent->hasFocus()) {
		port->drawBitmap(0, 0, _skin->topLeftBorder.bitmap.width,
							   _skin->topLeftBorder.bitmap.height,
							   _skin->topLeftBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->topLeftBorder.bitmap.width,
							   _skin->topLeftBorder.bitmap.height);

		port->drawBitmap(_skin->topLeftBorder.bitmap.width, 0,
							   _width - _skin->topLeftBorder.bitmap.width - _skin->topRightBorder.bitmap.width,
							   _skin->topCentreBorder.bitmap.height,
							   _skin->topCentreBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->topCentreBorder.bitmap.width,
							   _skin->topCentreBorder.bitmap.height);

		port->drawBitmap(_width - _skin->topRightBorder.bitmap.width,
							   0,
							   _skin->topRightBorder.bitmap.width,
							   _skin->topRightBorder.bitmap.height,
							   _skin->topRightBorder.bitmap.focus->getData(),
							   0,
							   0,
							   _skin->topRightBorder.bitmap.width,
							   _skin->topRightBorder.bitmap.height);
	} else {
		port->drawBitmap(0, 0, _skin->topLeftBorder.bitmap.width,
							   _skin->topLeftBorder.bitmap.height,
							   _skin->topLeftBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->topLeftBorder.bitmap.width,
							   _skin->topLeftBorder.bitmap.height);

		port->drawBitmap(_skin->topLeftBorder.bitmap.width, 0,
							   _width - _skin->topLeftBorder.bitmap.width - _skin->topRightBorder.bitmap.width,
							   _skin->topCentreBorder.bitmap.height,
							   _skin->topCentreBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->topCentreBorder.bitmap.width,
							   _skin->topCentreBorder.bitmap.height);

		port->drawBitmap(_width - _skin->topRightBorder.bitmap.width,
							   0,
							   _skin->topRightBorder.bitmap.width,
							   _skin->topRightBorder.bitmap.height,
							   _skin->topRightBorder.bitmap.blur->getData(),
							   0,
							   0,
							   _skin->topRightBorder.bitmap.width,
							   _skin->topRightBorder.bitmap.height);
	}

	// Calculate text position
	u8 textY = (_skin->topCentreBorder.bitmap.height / 2) - (_font->getHeight() / 2);
	u8 textX = 4;

	if (((SkinnedWindow*)_parent)->hasCloseButton()) {
		textX += _skin->closeButton.bitmap.width + _skin->closeButton.offsetX;
	}

	if (_skin->window.font.monochrome) {
		port->drawText(textX, textY, _font, _window->getTitle(), _skin->window.font.colour);	
	} else {
		port->drawText(textX, textY, _font, _window->getTitle());
	}

	delete port;
}
