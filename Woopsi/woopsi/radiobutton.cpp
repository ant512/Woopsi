// TODO: Switch to GraphicsPort

#include "radiobutton.h"
#include "radiobuttongroup.h"

RadioButton::RadioButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_SCREEN_DEPTH_UP, font) {
	_font = font;

	_state = RADIO_BUTTON_STATE_OFF;
	_flags.borderless = true;
}

RadioButton::~RadioButton() {
}

RadioButton::RadioButtonState RadioButton::getState() {
	return _state;
}

void RadioButton::setState(RadioButton::RadioButtonState state) {
	if (_state != state) {
		_state = state;

		if (_state != RADIO_BUTTON_STATE_OFF) {
			((RadioButtonGroup*)_parent)->setSelectedGadget(this);
		}

		raiseValueChangeEvent();

		draw();
	}
}

void RadioButton::draw() {
	Gadget::draw();
}

void RadioButton::draw(Rect clipRect) {

	clear(clipRect);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;

	// Choose appropriate glyph for state
	char glyph = GLYPH_RADIO_BUTTON_OFF;

	switch (_state) {
		case RADIO_BUTTON_STATE_ON:
			glyph = GLYPH_RADIO_BUTTON_ON;
			break;
		case RADIO_BUTTON_STATE_OFF:
			glyph = GLYPH_RADIO_BUTTON_OFF;
			break;
		case RADIO_BUTTON_STATE_MU:
			glyph = GLYPH_RADIO_BUTTON_MU;
			break;
	}

	TextWriter::drawChar(_font, glyph, getX() + _textX, getY() + _textY, clipRect.x, clipRect.y, clipRect.x + clipRect.width - 1, clipRect.y + clipRect.height - 1);
}

bool RadioButton::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		setState(RADIO_BUTTON_STATE_ON);

		return true;
	}

	return false;
}
