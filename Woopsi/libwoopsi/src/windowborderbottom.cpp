#include "windowborderbottom.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderBottom::WindowBorderBottom(s16 x, s16 y, u16 width, u16 height, u8 sideBorderSize) : Gadget(x, y, width, height, GADGET_BORDERLESS) {

	_sideBorderSize = sideBorderSize;
	_flags.decoration = true;
}

void WindowBorderBottom::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? _highlightColour : _fillColour;
	}

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Background
	port->drawFilledRect(1, 0, _width - 2, _height - 1, colour);

	// Top
	port->drawHorizLine(_sideBorderSize - 1, 0, _width - (_sideBorderSize << 1) + 2, _shineColour);

	// Left
	port->drawVertLine(0, 0, _height, _shineColour);

	// Right
	port->drawVertLine(_width - 1, 0, _height, _shadowColour);

	// Bottom of window
	port->drawHorizLine(0, _height - 1, _width, _shadowColour);

	// Tidy up
	delete port;
}

bool WindowBorderBottom::focus() {
	return true;
}

bool WindowBorderBottom::blur() {
	return true;
}
