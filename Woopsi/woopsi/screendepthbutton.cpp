// TODO: Switch to GraphicsPort

#include "screendepthbutton.h"

ScreenDepthButton::ScreenDepthButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_SCREEN_DEPTH_UP, font) {
	_flags.decoration = true;
}

ScreenDepthButton::~ScreenDepthButton() {
}

void ScreenDepthButton::draw(Rect clipRect) {
	clear(clipRect);

	// Choose appropriate glyph for state
	char glyph = GLYPH_SCREEN_DEPTH_UP;

	if (_flags.clicked) {
		glyph = GLYPH_SCREEN_DEPTH_DOWN;
	}

	TextWriter::drawChar(_font, glyph, getX() + _textX, getY() + _textY, clipRect.x, clipRect.y, clipRect.x + clipRect.width - 1, clipRect.y + clipRect.height - 1);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

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
			// Swap screens
			_parent->lowerToBottom();

			// Lose focus
			_parent->blur();
		}

		Gadget::draw();

		return true;
	}

	return false;
}
