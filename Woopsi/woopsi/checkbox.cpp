// TODO: Remove internal drawing and replace with graphicsport

#include "checkbox.h"
#include "graphicsport.h"

CheckBox::CheckBox(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Button(x, y, width, height, GLYPH_CHECK_BOX_ON, font) {
	_font = font;

	_state = CHECK_BOX_STATE_OFF;
	_flags.borderless = false;
	_outline = OUTLINE_OUT;
}

CheckBox::CheckBoxState CheckBox::getState() {
	return _state;
}

void CheckBox::setState(CheckBox::CheckBoxState state) {
	if (_state != state) {
		_state = state;

		raiseValueChangeEvent();

		draw();
	}
}

void CheckBox::draw() {
	Gadget::draw();
}

void CheckBox::draw(Rect clipRect) {

	clear(clipRect);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;

	// Choose appropriate glyph for state
	char glyph = GLYPH_CHECK_BOX_OFF;

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

	TextWriter::drawChar(_font, glyph, getX() + _textX, getY() + _textY, clipRect.x, clipRect.y, clipRect.x + clipRect.width - 1, clipRect.y + clipRect.height - 1);
}

bool CheckBox::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (_state == CHECK_BOX_STATE_ON) {
			setState(CHECK_BOX_STATE_OFF);
		} else {
			setState(CHECK_BOX_STATE_ON);
		}

		return true;
	}

	return false;
}
