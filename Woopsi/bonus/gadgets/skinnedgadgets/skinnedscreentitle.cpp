#include "skinnedscreentitle.h"
#include "graphicsport.h"
#include "screen.h"

using namespace WoopsiUI;

SkinnedScreenTitle::SkinnedScreenTitle(Screen* screen, const ScreenSkin* skin) : ScreenTitle(0, screen, NULL) {
	_flags.decoration = true;
	_skin = skin;

	// Parse skin information
	_flags.drawingEnabled = _skin->titleBar.visible;
	_width = SCREEN_WIDTH;
	_height = _skin->titleBar.bitmap.height;
	_font = _skin->screen.font.font;
	_backColour = _skin->screen.colours.back;
	_shineColour = _skin->screen.colours.shine;
	_highlightColour = _skin->screen.colours.highlight;
	_shadowColour = _skin->screen.colours.shadow;
	_fillColour = _skin->screen.colours.fill;
}

void SkinnedScreenTitle::draw(Rect clipRect) {

	// Get a new graphics port
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_flags.clicked) {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height, _skin->titleBar.bitmap.focusClick->getData(), 0, 0, _skin->titleBar.bitmap.width, _skin->titleBar.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->titleBar.bitmap.blurClick->getData(), 0, 0, _skin->titleBar.bitmap.width, _skin->titleBar.bitmap.height);
		}
	} else {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height, _skin->titleBar.bitmap.focus->getData(), 0, 0, _skin->titleBar.bitmap.width, _skin->titleBar.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->titleBar.bitmap.blur->getData(), 0, 0, _skin->titleBar.bitmap.width, _skin->titleBar.bitmap.height);
		}
	}

	if (_skin->screen.font.monochrome) {
		port->drawText(2, 1, _font, _screen->getTitle(), _skin->screen.font.colour);
	} else {
		port->drawText(2, 1, _font, _screen->getTitle());
	}
	
	delete port;
}
