#include "decorationglyphbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

DecorationGlyphButton::DecorationGlyphButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, GadgetStyle* style) : Button(x, y, width, height, normalGlyph, style) {
	_flags.decoration = true;
	_flags.canReceiveFocus = false;

	_borderSize.top = 1;
	_borderSize.right = 1;
	_borderSize.bottom = 1;
	_borderSize.left = 1;
	
	_normalGlyph = normalGlyph;
	_clickedGlyph = clickedGlyph;
	
	// Use the glyph font as the primary font so that the alignment functions
	// produce correct results
	setFont(getGlyphFont());
}

void DecorationGlyphButton::drawContents(GraphicsPort* port) {
	if (_flags.clicked) {
		port->drawText(_textX, _textY, getFont(), _clickedGlyph);
	} else {
		port->drawText(_textX, _textY, getFont(), _normalGlyph);
	}
}

void DecorationGlyphButton::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());

	drawOutline(port);
}

void DecorationGlyphButton::drawOutline(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	if (isClicked()) {
		// Bevelled into the screen
		col1 = getShadowColour();
		col2 = getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = getShineColour();
		col2 = getShadowColour();
	}
	
	port->drawBevelledRect(0, 0, getWidth(), getHeight(), col1, col2);
}

void DecorationGlyphButton::onFocus() {
	markRectsDirty();
}
