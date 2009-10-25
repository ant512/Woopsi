#include "windowborderbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WindowBorderButton::WindowBorderButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, FontBase* font) : DecorationGlyphButton(x, y, width, height, normalGlyph, clickedGlyph, font) {
}

void WindowBorderButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Choose a colour depending on parent's active state
	u16 colour = _fillColour;
	if (_parent != NULL) {
		colour = _parent->hasFocus() ? _highlightColour : _fillColour;
	}

	// Clear the background
	port->drawFilledRect(1, 1, _width - 2, _height - 2, colour);

	// Draw the glyph
	if (_flags.clicked) {
		port->drawText(_textX, _textY, _font, _clickedGlyph);
	} else {
		port->drawText(_textX, _textY, _font, _normalGlyph);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
}
