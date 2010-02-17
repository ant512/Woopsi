#include "windowborderside.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderSide::WindowBorderSide(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_BORDERLESS) {

	_flags.hasFocus = true;
	_flags.decoration = true;
	_flags.canReceiveFocus = false;
}

void WindowBorderSide::drawContents(GraphicsPort* port) {

	// Choose a colour depending on parent's active state
	u16 colour = getFillColour();
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? getHighlightColour() : getFillColour();
	}

	// Left bevel background
	port->drawFilledRect(1, 0, _width - 1, _height, colour);

	// Left
	port->drawVertLine(0, 0, _height, getShineColour());

	// Right
	port->drawVertLine(_width - 1, 0, _height, getShadowColour());
}
