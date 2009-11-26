#include "screentitle.h"
#include "graphicsport.h"
#include "screen.h"
#include <string.h>

using namespace WoopsiUI;

ScreenTitle::ScreenTitle(u16 height, Screen* screen, FontBase* font) : Gadget(0, 0, SCREEN_WIDTH, height, GADGET_BORDERLESS, font) {
	_flags.decoration = true;
	_flags.draggable = true;

	_screen = screen;
}

void ScreenTitle::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(0, 0, _width, _height, _colours.shine);	// Background
	port->drawHorizLine(0, _height - 1, _width, _colours.shadow);	// Bottom
	port->drawText(2, 1, _font, _screen->getTitle());			// Title text
	delete port;
}

bool ScreenTitle::focus() {
	return true;
}

bool ScreenTitle::blur() {
	return true;
}
