#include "checkbox.h"
#include "graphicsport.h"

using namespace WoopsiUI;

CheckBox::CheckBox(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Button(x, y, width, height, GLYPH_CHECK_BOX_ON, style) {
	_state = CHECK_BOX_STATE_OFF;
	_flags.borderless = false;

	_borderSize.top = 1;
	_borderSize.right = 1;
	_borderSize.bottom = 1;
	_borderSize.left = 1;
	
	// Use the glyph font as the primary font so that the alignment functions
	// produce correct results
	setFont(getGlyphFont());
}

void CheckBox::setState(CheckBox::CheckBoxState state) {
	if (_state != state) {
		_state = state;

		_gadgetEventHandlers->raiseValueChangeEvent();

		markRectsDirty();
	}
}

void CheckBox::drawContents(GraphicsPort* port) {

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
}

void CheckBox::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShineColour(), getShadowColour());
}

void CheckBox::onClick(s16 x, s16 y) {
	if (_state == CHECK_BOX_STATE_ON) {
		setState(CHECK_BOX_STATE_OFF);
	} else {
		setState(CHECK_BOX_STATE_ON);
	}
}
