#include "screentitle.h"
#include "graphicsport.h"
#include "screen.h"
#include <string.h>

using namespace WoopsiUI;

ScreenTitle::ScreenTitle(u16 height, Screen* screen, GadgetStyle* style) : Gadget(0, 0, SCREEN_WIDTH, height, GADGET_BORDERLESS, style) {
	_flags.decoration = true;
	_flags.draggable = true;
	_flags.canReceiveFocus = false;

	_screen = screen;
}

void ScreenTitle::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, _width, _height, getShineColour());	// Background
	port->drawHorizLine(0, _height - 1, _width, getShadowColour());	// Bottom
	port->drawText(2, 1, getFont(), _screen->getTitle());			// Title text
}

void ScreenTitle::onClick(s16 x, s16 y) {
	startDragging(x, y);
}
