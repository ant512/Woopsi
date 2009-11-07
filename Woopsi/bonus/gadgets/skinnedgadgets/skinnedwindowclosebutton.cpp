#include "skinnedwindowclosebutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedWindowCloseButton::SkinnedWindowCloseButton(s16 x, s16 y, const WindowSkin* skin) : DecorationGlyphButton(x, y, 0, 0, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, NULL) {

	_skin = skin;

	// Parse skin information
	_height = _skin->closeButton.bitmap.width;
	_width = _skin->closeButton.bitmap.height;
	_flags.borderless = _skin->closeButton.borderless;
	_flags.drawingEnabled = _skin->closeButton.visible;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;
}

void SkinnedWindowCloseButton::draw(Rect clipRect) {

	// Background
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_flags.clicked) {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height,
								_skin->closeButton.bitmap.focusClick->getData(),
								0,
								0,
								_skin->closeButton.bitmap.width, 
								_skin->closeButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height,
								_skin->closeButton.bitmap.blurClick->getData(),
								0,
								0,
								_skin->closeButton.bitmap.width,
								_skin->closeButton.bitmap.height);
		}
	} else {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height,
								_skin->closeButton.bitmap.focus->getData(),
								0,
								0,
								_skin->closeButton.bitmap.width,
								_skin->closeButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height,
								_skin->closeButton.bitmap.blur->getData(),
								0,
								0,
								_skin->closeButton.bitmap.width,
								_skin->closeButton.bitmap.height);
		}
	}

	delete port;
}
