#include "skinnedwindowdepthbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedWindowDepthButton::SkinnedWindowDepthButton(s16 x, s16 y, const WindowSkin* skin) : DecorationGlyphButton(x, y, 0, 0, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, NULL) {

	_skin = skin;

	// Parse skin information
	_height = _skin->depthButton.bitmap.width;
	_width = _skin->depthButton.bitmap.height;
	_flags.borderless = _skin->depthButton.borderless;
	_flags.drawingEnabled = _skin->depthButton.visible;
	_font = _skin->depthButton.font.font;
	_backColour = _skin->depthButton.colours.back;
	_shineColour = _skin->depthButton.colours.shine;
	_highlightColour = _skin->depthButton.colours.highlight;
	_shadowColour = _skin->depthButton.colours.shadow;
	_fillColour = _skin->depthButton.colours.fill;
}

void SkinnedWindowDepthButton::draw(Rect clipRect) {

	// Background
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (_flags.clicked) {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height,
								_skin->depthButton.bitmap.focusClick->getData(),
								0,
								0,
								_skin->depthButton.bitmap.width,
								_skin->depthButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height,
								_skin->depthButton.bitmap.blurClick->getData(),
								0,
								0,
								_skin->depthButton.bitmap.width,
								_skin->depthButton.bitmap.height);
		}
	} else {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height,
								_skin->depthButton.bitmap.focus->getData(),
								0,
								0,
								_skin->depthButton.bitmap.width,
								_skin->depthButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height,
								_skin->depthButton.bitmap.blur->getData(),
								0,
								0,
								_skin->depthButton.bitmap.width,
								_skin->depthButton.bitmap.height);
		}
	}

	delete port;
}
