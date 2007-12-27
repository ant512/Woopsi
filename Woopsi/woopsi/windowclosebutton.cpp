#include "windowclosebutton.h"

WindowCloseButton::WindowCloseButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_WINDOW_CLOSE, font) {
	_font = font;
	_flags.decoration = true;
}

WindowCloseButton::~WindowCloseButton() {
}

void WindowCloseButton::draw() {
	Gadget::draw();
}

void WindowCloseButton::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->isActive() ? _highlightColour : _fillColour;
	}

	// Background
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(1, 1, _width - 2, _height - 1, colour);
	port->drawText(_textX, _textY, _font, _text);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool WindowCloseButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;

		// Was the stylus released over this control?
		if (checkCollision(x, y)) {

			// Work out what to do with the parent window
			if (_parent->getCloseType() == CLOSE_TYPE_CLOSE) {

				// Close the parent window
				_parent->close();
			} else {

				// Hide the parent window
				_parent->hide();
			}
		} else {
			Gadget::draw();
		}
		
		return true;
	}
	
	return false;
}

bool WindowCloseButton::focus() {
	if (!_flags.active) {
		_flags.active = true;

		Gadget::draw();

		return true;
	}

	return false;
}

bool WindowCloseButton::blur() {

	_flags.active = false;

	Gadget::draw();

	return true;
}
