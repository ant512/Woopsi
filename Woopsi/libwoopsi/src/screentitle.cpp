#include "screentitle.h"
#include "graphicsport.h"
#include "screen.h"
#include <string.h>

using namespace WoopsiUI;

ScreenTitle::ScreenTitle(u16 height, Screen* screen, GadgetStyle* style) : Gadget(0, 0, SCREEN_WIDTH, height, GADGET_BORDERLESS, style) {
	_flags.decoration = true;
	_flags.draggable = true;

	_screen = screen;
}

void ScreenTitle::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(0, 0, _width, _height, getShineColour());	// Background
	port->drawHorizLine(0, _height - 1, _width, getShadowColour());	// Bottom
	port->drawText(2, 1, getFont(), _screen->getTitle());			// Title text
	delete port;
}

bool ScreenTitle::focus() {
	return true;
}

bool ScreenTitle::blur() {
	return true;
}
