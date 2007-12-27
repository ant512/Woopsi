#include "windowbordertop.h"

WindowBorderTop::WindowBorderTop(s16 x, u16 width, u16 height, char* text, FontBase* font) : Gadget(x, 0, width, height, GADGET_BORDERLESS, font) {
	_flags.decoration = true;

	// Create a copy of the text
	_text = new char[strlen(text) + 1];
	strcpy(_text, text);
}

WindowBorderTop::~WindowBorderTop() {
	if (_text != NULL) {
		delete [] _text;
	}
}

void WindowBorderTop::draw() {
	Gadget::draw();
}

void WindowBorderTop::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->isActive() ? _highlightColour : _fillColour;
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

bool WindowBorderTop::click(s16 x, s16 y) {

	if (Gadget::click(x, y)) {
		// Tell parent that it is being dragged
		if (_parent != NULL) {
			_parent->setDragging(x, y);
		}

		return true;
	}

	return false;
}
