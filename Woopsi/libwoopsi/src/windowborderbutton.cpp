#include "windowborderbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderButton::WindowBorderButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, GadgetStyle* style) : DecorationGlyphButton(x, y, width, height, normalGlyph, clickedGlyph, style) {

	// Use the glyph font as the primary font so that the alignment functions
	// produce correct results
	setFont(getGlyphFont());
}

void WindowBorderButton::drawContents(GraphicsPort* port) {

	// Choose a colour depending on parent's active state
	u16 colour = getFillColour();
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? getHighlightColour() : getFillColour();
	}

	// Clear the background
	port->drawFilledRect(0, 0, getWidth(), getHeight(), colour);

	// Draw the glyph
	if (_flags.clicked) {
		port->drawText(_textX, _textY, getFont(), _clickedGlyph);
	} else {
		port->drawText(_textX, _textY, getFont(), _normalGlyph);
	}
}

void WindowBorderButton::drawBorder(GraphicsPort* port) {
	drawOutline(port);
}