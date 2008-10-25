#include "screentitle.h"
#include "graphicsport.h"
#include <string.h>

using namespace WoopsiUI;

ScreenTitle::ScreenTitle(u16 height, const char* text, FontBase* font) : Gadget(0, 0, SCREEN_WIDTH, height, GADGET_BORDERLESS, font) {
	_flags.decoration = true;
	_flags.draggable = true;

	// Don't create a copy of the title text, as we want to point to the title stored in
	// the parent screen
	_text = text;
}

void ScreenTitle::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(0, 0, _width, _height, _shineColour);	// Background
	port->drawHorizLine(0, _height - 1, _width, _shadowColour);	// Bottom
	port->drawText(2, 1, _font, _text);							// Title text
	delete port;
}

bool ScreenTitle::focus() {
	return true;
}

bool ScreenTitle::blur() {
	return true;
}
