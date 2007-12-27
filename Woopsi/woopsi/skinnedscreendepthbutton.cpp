#include "skinnedscreendepthbutton.h"

SkinnedScreenDepthButton::SkinnedScreenDepthButton(s16 x, s16 y, const ScreenSkin* skin)
 : ScreenDepthButton(x, y, 0, 0, NULL) {
	_flags.decoration = true;
	_skin = skin;

	// Parse skin information
	_flags.borderless = _skin->depthButton.borderless;
	_flags.visible = _skin->depthButton.visible;
	_width = _skin->depthButton.bitmap.width;
	_height = _skin->depthButton.bitmap.height;
	_font = _skin->screen.font.font;
	_backColour = _skin->screen.colours.back;
	_shineColour = _skin->screen.colours.shine;
	_highlightColour = _skin->screen.colours.highlight;
	_shadowColour = _skin->screen.colours.shadow;
	_fillColour = _skin->screen.colours.fill;
}

SkinnedScreenDepthButton::~SkinnedScreenDepthButton() {
}

void SkinnedScreenDepthButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	if (_flags.clicked) {
		if (_parent->isActive()) {
			port->drawBitmap(0, 0, _width, _height, _skin->depthButton.bitmap.focusClick, 0, 0, _skin->depthButton.bitmap.width, _skin->depthButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->depthButton.bitmap.blurClick, 0, 0, _skin->depthButton.bitmap.width, _skin->depthButton.bitmap.height);
		}
	} else {
		if (_parent->isActive()) {
			port->drawBitmap(0, 0, _width, _height, _skin->depthButton.bitmap.focus, 0, 0, _skin->depthButton.bitmap.width, _skin->depthButton.bitmap.height);
		} else {
			port->drawBitmap(0, 0, _width, _height, _skin->depthButton.bitmap.blur, 0, 0, _skin->depthButton.bitmap.width, _skin->depthButton.bitmap.height);
		}
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
}
