#include "screendepthbutton.h"

ScreenDepthButton::ScreenDepthButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_SCREEN_DEPTH_UP, font) {
	_flags.decoration = true;
}

ScreenDepthButton::~ScreenDepthButton() {
}

void ScreenDepthButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear the background
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw the glyph
	if (_flags.clicked) {
		port->drawText(_textX, _textY, _font, GLYPH_SCREEN_DEPTH_DOWN);
	} else {
		port->drawText(_textX, _textY, _font, GLYPH_SCREEN_DEPTH_UP);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
}

bool ScreenDepthButton::click(s16 x, s16 y) {
	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_flags.clicked = true;

			// Tell parent that the clicked gadget has changed
			if (_parent != NULL) {
				_parent->setClickedGadget(this);
			}

			Gadget::draw();

			return true;
		}
	}

	return false;
}

bool ScreenDepthButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;

		// Was the stylus released over this control?
		if (checkCollision(x, y)) {
			raiseReleaseEvent(x, y);
		}

		Gadget::draw();

		return true;
	}

	return false;
}
