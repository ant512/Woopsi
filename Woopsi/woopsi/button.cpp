#include "button.h"
#include "graphicsport.h"
#include "woopsi.h"

using namespace WoopsiUI;

Button::Button(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Label(x, y, width, height, text, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}

Button::Button(s16 x, s16 y, u16 width, u16 height, char letter, FontBase* font) : Label(x, y, width, height, letter, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}

void Button::draw(Rect clipRect) {
		
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (!isEnabled()) {

		// Draw disabled state
		port->drawFilledRect(0, 0, _width, _height, _backColour);

		port->drawText(_textX, _textY, _font, _text->getCharArray(), _darkColour);
	} else if (!isClicked()) {

		// Draw normal state
		port->drawFilledRect(0, 0, _width, _height, _backColour);

		port->drawText(_textX, _textY, _font, _text->getCharArray());
	} else {

		// Draw clicked state
		port->drawFilledRect(0, 0, _width, _height, _darkColour);

		port->drawText(_textX, _textY, _font, _text->getCharArray(), _shineColour);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool Button::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {
		draw();
		return true;
	}

	return false;
}

bool Button::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		if (woopsiApplication->getClickedGadget() == this) {
			woopsiApplication->setClickedGadget(NULL);
		}

		// Determine which release event to fire
		if (checkCollision(x, y)) {
			// Release occurred within gadget; raise release
			raiseReleaseEvent(x, y);

			// Also raise "action" event
			raiseActionEvent(x, y, 0, 0, KEY_CODE_NONE);
		} else {
			// Release occurred outside gadget; raise release
			raiseReleaseOutsideEvent(x, y);
		}

		draw();

		return true;
	}

	return false;
}
