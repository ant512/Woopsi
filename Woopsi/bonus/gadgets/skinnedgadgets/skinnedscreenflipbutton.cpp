#include "skinnedscreenflipbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SkinnedScreenFlipButton::SkinnedScreenFlipButton(s16 x, s16 y, const ScreenSkin* skin)
: DecorationGlyphButton(x, y, 0, 0, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, NULL) {
	_flags.decoration = true;
	_skin = skin;

	// Parse skin information
	_flags.borderless = _skin->flipButton.borderless;
	_flags.drawingEnabled = _skin->flipButton.visible;
	_width = _skin->flipButton.bitmap.width;
	_height = _skin->flipButton.bitmap.height;
	_font = _skin->screen.font.font;
	_backColour = _skin->screen.colours.back;
	_shineColour = _skin->screen.colours.shine;
	_highlightColour = _skin->screen.colours.highlight;
	_shadowColour = _skin->screen.colours.shadow;
	_fillColour = _skin->screen.colours.fill;
}

void SkinnedScreenFlipButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	if (_flags.clicked) {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.focusClick->getData(), 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.blurClick->getData(), 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		}
	} else {
		if (_parent->hasFocus()) {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.focus->getData(), 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.blur->getData(), 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		}
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}
