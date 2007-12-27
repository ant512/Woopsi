#include "skinnedscreenflipbutton.h"

SkinnedScreenFlipButton::SkinnedScreenFlipButton(s16 x, s16 y, const ScreenSkin* skin)
: ScreenFlipButton(x, y, 0, 0, NULL) {
	_flags.decoration = true;
	_skin = skin;

	// Parse skin information
	_flags.borderless = _skin->flipButton.borderless;
	_flags.visible = _skin->flipButton.visible;
	_width = _skin->flipButton.bitmap.width;
	_height = _skin->flipButton.bitmap.height;
	_font = _skin->screen.font.font;
	_backColour = _skin->screen.colours.back;
	_shineColour = _skin->screen.colours.shine;
	_highlightColour = _skin->screen.colours.highlight;
	_shadowColour = _skin->screen.colours.shadow;
	_fillColour = _skin->screen.colours.fill;
}

SkinnedScreenFlipButton::~SkinnedScreenFlipButton() {
}

void SkinnedScreenFlipButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	if (_flags.clicked) {
		if (_parent->isActive()) {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.focusClick, 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.blurClick, 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		}
	} else {
		if (_parent->isActive()) {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.focus, 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->flipButton.bitmap.blur, 0, 0, _skin->flipButton.bitmap.width, _skin->flipButton.bitmap.height);
		}
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}
