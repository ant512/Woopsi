#include "decorationglyphbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

DecorationGlyphButton::DecorationGlyphButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, FontBase* font) : Button(x, y, width, height, normalGlyph, font) {
	_flags.decoration = true;

	_normalGlyph = normalGlyph;
	_clickedGlyph = clickedGlyph;
}

void DecorationGlyphButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear the background
	port->drawFilledRect(0, 0, _width, _height, _backColour);

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

bool DecorationGlyphButton::focus() {
	if (!_flags.hasFocus) {
		_flags.hasFocus = true;

		Gadget::draw();

		return true;
	}

	return false;
}
