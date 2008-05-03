#include "button.h"
#include "graphicsport.h"

Button::Button(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Textbox(x, y, width, height, text, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}

Button::Button(s16 x, s16 y, u16 width, u16 height, char letter, FontBase* font) : Textbox(x, y, width, height, letter, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}

void Button::draw(Rect clipRect) {
		
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (!_flags.clicked) {

		// Draw normal state
		port->drawFilledRect(0, 0, _width, _height, _backColour);

		port->drawText(_textX, _textY, _font, _text);
	} else {

		// Draw clicked state
		port->drawFilledRect(0, 0, _width, _height, _darkColour);

		port->drawText(_textX, _textY, _font, _text, _shineColour);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}
