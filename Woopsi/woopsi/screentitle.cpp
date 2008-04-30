#include "screentitle.h"
#include "graphicsport.h"
#include <string.h>

ScreenTitle::ScreenTitle(u16 height, char* text, FontBase* font) : Gadget(0, 0, SCREEN_WIDTH, height, GADGET_BORDERLESS, font) {
	_flags.decoration = true;

	// Create a copy of the text
	_text = new char[strlen(text) + 1];
	strcpy(_text, text);
}

void ScreenTitle::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(0, 0, _width, _height, _shineColour);	// Background
	port->drawHorizLine(0, _height - 1, _width, _shadowColour);	// Bottom
	port->drawText(2, 1, _font, _text);							// Title text
	delete port;
}

bool ScreenTitle::click(s16 x, s16 y) {

	if (Gadget::click(x, y)) {
		// Tell parent that it is being dragged
		if (_parent != NULL) {
			_parent->setDragging(x, y);
		}

		return true;
	}

	return false;
}
