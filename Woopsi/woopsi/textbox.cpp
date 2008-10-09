#include "textbox.h"
#include "graphicsport.h"

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Label(x, y, width, height, text, font) {
	_cursorPos = 0;
	_showCursor = false;
	setBorderless(false);
}

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font) : Label(x, y, width, height, letter, font) {
	_cursorPos = 0;
	_showCursor = false;
	setBorderless(false);
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

void TextBox::insertTextAtCursor(const char* text) {
	_text->insert(text, _cursorPos);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void TextBox::insertTextAtCursor(const char text) {
	_text->insert(text, _cursorPos);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void TextBox::moveCursorToPosition(const u32 position) {
	u32 len = _text->length();
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
