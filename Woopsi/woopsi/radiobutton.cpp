#include "radiobutton.h"
#include "radiobuttongroup.h"
#include "graphicsport.h"

RadioButton::RadioButton(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_SCREEN_DEPTH_UP, font) {
	_font = font;

	_state = RADIO_BUTTON_STATE_OFF;
	_flags.borderless = true;
}

void RadioButton::setState(RadioButton::RadioButtonState state) {
	if (_state != state) {
		_state = state;

		if (_state != RADIO_BUTTON_STATE_OFF) {
			((RadioButtonGroup*)_parent)->setSelectedGadget(this);
		}

		raiseValueChangeEvent();

		Gadget::draw();
	}
}

void RadioButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear the background
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	// Draw appropriate glyph for state
	switch (_state) {
		case RADIO_BUTTON_STATE_ON:
			port->drawText(_textX, _textY, _font, GLYPH_RADIO_BUTTON_ON);
			break;
		case RADIO_BUTTON_STATE_OFF:
			port->drawText(_textX, _textY, _font, GLYPH_RADIO_BUTTON_OFF);
			break;
		case RADIO_BUTTON_STATE_MU:
			port->drawText(_textX, _textY, _font, GLYPH_RADIO_BUTTON_MU);
			break;
	}

	delete port;
}

bool RadioButton::click(s16 x, s16 y) {
	if (Button::click(x, y)) {

		setState(RADIO_BUTTON_STATE_ON);

		return true;
	}

	return false;
}
