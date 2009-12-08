#include <string.h>
#include "windowbordertop.h"
#include "graphicsport.h"
#include "window.h"

using namespace WoopsiUI;

WindowBorderTop::WindowBorderTop(s16 x, u16 width, u16 height, Window* window, GadgetStyle* style) : Gadget(x, 0, width, height, GADGET_BORDERLESS, style) {
	_flags.decoration = true;
	_flags.draggable = true;
	_window = window;
}

void WindowBorderTop::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = getFillColour();
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? getHighlightColour() : getFillColour();
	}

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawHorizLine(0, 0, _width, getShineColour());				// Top of window
	port->drawFilledRect(1, 1, _width - 2, _height - 1, colour);		// Background
	port->drawHorizLine(1, _height - 1, _width - 1, getShadowColour());	// Bottom
	port->drawVertLine(0, 0, _height, getShineColour());				// Left
	port->drawVertLine(_width - 1, 1, _height - 1, getShadowColour());	// Right
	port->drawText(2, 1, getFont(), _window->getTitle());				// Title text
	delete port;
}

bool WindowBorderTop::focus() {
	return true;
}

bool WindowBorderTop::blur() {
	return true;
}
