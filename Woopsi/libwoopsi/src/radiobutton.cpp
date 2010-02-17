#include "radiobutton.h"
#include "radiobuttongroup.h"
#include "graphicsport.h"

using namespace WoopsiUI;

RadioButton::RadioButton(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Button(x, y, width, height, GLYPH_SCREEN_DEPTH_UP, style) {
	_state = RADIO_BUTTON_STATE_OFF;
	_flags.borderless = true;

	// Use the glyph font as the primary font so that the alignment functions
	// produce correct results
	setFont(getGlyphFont());
}

void RadioButton::setState(RadioButton::RadioButtonState state) {
	if (_state != state) {
		_state = state;

		if (_state != RADIO_BUTTON_STATE_OFF) {
			((RadioButtonGroup*)_parent)->setSelectedGadget(this);
		}

		_gadgetEventHandlers->raiseValueChangeEvent();

		redraw();
	}
}

void RadioButton::drawContents(GraphicsPort* port) {

	// Clear the background
	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	// Work out which glyph to draw
	char glyph = GLYPH_RADIO_BUTTON_ON;

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

	// Draw button
	if (isEnabled()) {
		port->drawText(_textX, _textY, getFont(), glyph);
	} else {
		port->drawText(_textX, _textY, getFont(), glyph, 0, 1, getDarkColour());
	}
}

void RadioButton::onClick(s16 x, s16 y) {
	setState(RADIO_BUTTON_STATE_ON);
}
