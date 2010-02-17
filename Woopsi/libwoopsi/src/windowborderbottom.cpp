#include "windowborderbottom.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderBottom::WindowBorderBottom(s16 x, s16 y, u16 width, u16 height, u8 sideBorderSize) : Gadget(x, y, width, height, GADGET_BORDERLESS) {

	_sideBorderSize = sideBorderSize;
	_flags.decoration = true;
	_flags.canReceiveFocus = false;
}

void WindowBorderBottom::drawContents(GraphicsPort* port) {

	// Choose a colour depending on parent's active state
	u16 colour = getFillColour();
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? getHighlightColour() : getFillColour();
	}

	// Background
	port->drawFilledRect(1, 0, _width - 2, _height - 1, colour);

	// Top
	port->drawHorizLine(_sideBorderSize - 1, 0, _width - (_sideBorderSize << 1) + 2, getShineColour());

	// Left
	port->drawVertLine(0, 0, _height, getShineColour());

	// Right
	port->drawVertLine(_width - 1, 0, _height, getShadowColour());

	// Bottom of window
	port->drawHorizLine(0, _height - 1, _width, getShadowColour());
}
