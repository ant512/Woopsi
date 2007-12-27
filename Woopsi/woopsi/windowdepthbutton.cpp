#include "windowdepthbutton.h"
#include "amigawindow.h"

WindowDepthButton::WindowDepthButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_WINDOW_DEPTH_UP, font) {
	_font = font;
	_flags.decoration = true;
}

WindowDepthButton::~WindowDepthButton() {
}

void WindowDepthButton::draw() {
	Gadget::draw();
}

void WindowDepthButton::draw(Rect clipRect) {

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->isActive() ? _highlightColour : _fillColour;
	}

	// Background
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(1, 1, _width - 2, _height - 1, colour);

	// Choose appropriate glyph for state
	char glyph = GLYPH_WINDOW_DEPTH_UP;

	if (_flags.clicked) {
		glyph = GLYPH_WINDOW_DEPTH_DOWN;
	}

	port->drawText(_textX, _textY, _font, glyph);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool WindowDepthButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;

		// Was the stylus released over this control?
		if (checkCollision(x, y)) {
			// Swap window depth
			_parent->swapDepth();
		}

		Gadget::draw();

		_parent->setClickedGadget(NULL);

		return true;
	}

	return false;
}

bool WindowDepthButton::focus() {
	if (!_flags.active) {
		_flags.active = true;

		Gadget::draw();

		return true;
	}

	return false;
}

bool WindowDepthButton::blur() {

	_flags.active = false;

	Gadget::draw();

	return true;
}
