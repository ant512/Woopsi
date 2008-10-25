#include <string.h>
#include "windowbordertop.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderTop::WindowBorderTop(s16 x, u16 width, u16 height, const char* text, FontBase* font) : Gadget(x, 0, width, height, GADGET_BORDERLESS, font) {
	_flags.decoration = true;
	_flags.draggable = true;

	// Don't create a copy of the text, since we want to point at the title stored in 
	// the parent window
	_text = text;
}

void WindowBorderTop::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? _highlightColour : _fillColour;
	}

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawHorizLine(0, 0, _width, _shineColour);				// Top of window
	port->drawFilledRect(1, 1, _width - 2, _height - 1, colour);	// Background
	port->drawHorizLine(1, _height - 1, _width - 1, _shadowColour);	// Bottom
	port->drawVertLine(0, 0, _height, _shineColour);				// Left
	port->drawVertLine(_width - 1, 1, _height - 1, _shadowColour);	// Right
	port->drawText(2, 1, _font, _text);								// Title text
	delete port;
}

bool WindowBorderTop::focus() {
	return true;
}

bool WindowBorderTop::blur() {
	return true;
}
