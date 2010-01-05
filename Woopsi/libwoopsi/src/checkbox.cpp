#include "checkbox.h"
#include "graphicsport.h"

using namespace WoopsiUI;

CheckBox::CheckBox(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Button(x, y, width, height, GLYPH_CHECK_BOX_ON, style) {
	_state = CHECK_BOX_STATE_OFF;
	_flags.borderless = false;
	_outline = OUTLINE_OUT;
	setFont(_style->glyphFont);
}

void CheckBox::setState(CheckBox::CheckBoxState state) {
	if (_state != state) {
		_state = state;

		_gadgetEventHandlers->raiseValueChangeEvent();

		redraw();
	}
}

void CheckBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear the background
	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	// Work out which glyph to draw
	char glyph = GLYPH_CHECK_BOX_ON;

	switch (_state) {
		case CHECK_BOX_STATE_ON:
			glyph = GLYPH_CHECK_BOX_ON;
			break;
		case CHECK_BOX_STATE_OFF:
			glyph = GLYPH_CHECK_BOX_OFF;
			break;
		case CHECK_BOX_STATE_MU:
			glyph = GLYPH_CHECK_BOX_MU;
			break;
	}

	// Draw tick
	if (isEnabled()) {
		port->drawText(_textX, _textY, getFont(), glyph);
	} else {
		port->drawText(_textX, _textY, getFont(), glyph, 0, 1, getDarkColour());
	}

	delete port;
}

bool CheckBox::click(s16 x, s16 y) {
	if (Button::click(x, y)) {

		if (isEnabled()) {
			if (_state == CHECK_BOX_STATE_ON) {
				setState(CHECK_BOX_STATE_OFF);
			} else {
				setState(CHECK_BOX_STATE_ON);
			}
		}

		return true;
	}

	return false;
}
