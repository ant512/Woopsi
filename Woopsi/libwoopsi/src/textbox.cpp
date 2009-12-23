#include "textbox.h"
#include "graphicsport.h"

using namespace WoopsiUI;

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char* text, GadgetStyle* style) : Label(x, y, width, height, text, style) {
	_cursorPos = 0;
	_showCursor = true;
	setOutlineType(OUTLINE_OUT_IN);
	moveCursorToPosition(_text->getLength());
}

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char letter, GadgetStyle* style) : Label(x, y, width, height, letter, style) {
	_cursorPos = 0;
	_showCursor = true;
	setOutlineType(OUTLINE_OUT_IN);
	moveCursorToPosition(_text->getLength());
}

void TextBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	if (isEnabled()) {
		port->drawText(_textX, _textY, getFont(), _text->getCharArray());
	} else {
		port->drawText(_textX, _textY, getFont(), _text->getCharArray(), getDarkColour());
	}

	// Draw cursor
	if (_showCursor) {
		port->drawFilledXORRect(getCursorXPos(), _textY, getFont()->getCharWidth(_text->getCharArray()[_cursorPos]), getFont()->getHeight());
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

const u16 TextBox::getCursorXPos() const {

	// Calculate position of cursor
	u16 cursorX = _textX;

	for (u16 i = 0; i < _cursorPos; i++) {
		cursorX += getFont()->getCharWidth(_text->getCharArray()[i]);
	}

	return cursorX;
}

void TextBox::setText(const char* text) {
	_text->setText(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::setText(const char text) {
	_text->setText(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::appendText(const char* text) {
	_text->append(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::appendText(const char text) {
	_text->append(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::insertText(const char* text, const u32 index) {
	// Get current text length - use this later to quickly get the length
	// of the inserted string to shift the cursor around
	u32 oldLen = _text->getLength();

	_text->insert(text, index);
	
	// Get the new string length and use it to calculate the length
	// of the inserted string
	u32 insertLen = _text->getLength() - oldLen;

	moveCursorToPosition(index + insertLen);
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::insertText(const char text, const u32 index) {
	_text->insert(text, index);

	// Cursor position just increases by one as we're inserting a single char
	moveCursorToPosition(getCursorPosition() + 1);
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::insertTextAtCursor(const char* text) {
	insertText(text, getCursorPosition());
}

void TextBox::insertTextAtCursor(const char text) {
	insertText(text, getCursorPosition());
}

void TextBox::moveCursorToPosition(const u32 position) {
	u32 len = _text->getLength();
	_cursorPos = len >= position ? position : len;

	redraw();
}

void TextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;
		redraw();
	}
}

void TextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;
		redraw();
	}
}

// Client rect is 1px smaller than usual as the border is 2 pixels thick
void TextBox::getClientRect(Rect& rect) const {
	if (!_flags.borderless) {
		rect.x = 2;
		rect.y = 2;
		rect.width = _width - 4;
		rect.height = _height - 4;
	} else {
		rect.x = 0;
		rect.y = 0;
		rect.width = _width;
		rect.height = _height;
	}
}

bool TextBox::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (isEnabled()) {
			// Work out where in the string the textbox was clicked and move the cursor to that
			// location

			if (_text->getLength() > 0) {
				s16 clickX = x - getX();
				s16 charX = _textX;
				u32 charIndex = 0;

				// Locate the first character that comes after the clicked character
				while ((charX < clickX) && (charIndex < _text->getLength())) {
					charX += getFont()->getCharWidth(_text->getCharArray()[charIndex]);
					++charIndex;
				}

				// Move back to the clicked character if we've moved past it
				if (charX > clickX) {
					charIndex--;
				}

				moveCursorToPosition(charIndex);
			}
		}

		return true;
	}

	return false;
}

bool TextBox::keyPress(KeyCode keyCode) {
	if (Gadget::keyPress(keyCode)) {
		if (keyCode == KEY_CODE_LEFT) {
			if (_cursorPos > 0) {
				moveCursorToPosition(_cursorPos - 1);
			}
		} else if (keyCode == KEY_CODE_RIGHT) {
			moveCursorToPosition(_cursorPos + 1);
		}

		return true;
	}

	return false;
}
