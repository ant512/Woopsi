#include "textbox.h"
#include "graphicsport.h"

using namespace WoopsiUI;

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Label(x, y, width, height, text, font) {
	_cursorPos = 0;
	_showCursor = true;
	setOutlineType(OUTLINE_OUT_IN);
	moveCursorToPosition(_text->getLength());
}

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font) : Label(x, y, width, height, letter, font) {
	_cursorPos = 0;
	_showCursor = true;
	setOutlineType(OUTLINE_OUT_IN);
	moveCursorToPosition(_text->getLength());
}

void TextBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	port->drawText(_textX, _textY, _font, _text->getCharArray());

	// Draw cursor
	if (_showCursor) {
		port->drawFilledXORRect(getCursorXPos(), _textY, _font->getCharWidth(_text->getCharArray()[_cursorPos]), _font->getHeight());
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

const u16 TextBox::getCursorXPos() const {

	// Calculate position of cursor
	u16 cursorX = _textX;

	for (u16 i = 0; i < _cursorPos; i++) {
		cursorX += _font->getCharWidth(_text->getCharArray()[i]);
	}

	return cursorX;
}

void TextBox::setText(const char* text) {
	_text->setText(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void TextBox::setText(const char text) {
	_text->setText(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void TextBox::appendText(const char* text) {
	_text->append(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void TextBox::appendText(const char text) {
	_text->append(text);
	moveCursorToPosition(_text->getLength());
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
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
	draw();
	raiseValueChangeEvent();
}

void TextBox::insertText(const char text, const u32 index) {
	_text->insert(text, index);

	// Cursor position just increases by one as we're inserting a single char
	moveCursorToPosition(getCursorPosition() + 1);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
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

	draw();
}

void TextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;
		draw();
	}
}

void TextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;
		draw();
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
		// Work out where in the string the textbox was clicked and move the cursor to that
		// location
		s16 clickX = x - getX();
		s16 charX = _textX;
		u32 charIndex = 0;
		u8 charWidth = _font->getCharWidth(_text->getCharArray()[charIndex]);

		while ((charX + charWidth < clickX) && (charIndex < _text->getLength())) {
			charX += charWidth;
			charWidth = _font->getCharWidth(_text->getCharArray()[charIndex]);
			++charIndex;
		}

		moveCursorToPosition(charIndex);

		return true;
	}

	return false;
}
