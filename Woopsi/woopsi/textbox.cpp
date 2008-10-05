#include "textbox.h"
#include "graphicsport.h"

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_text = NULL;

	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;
	_cursorPos = 0;

	setText(text);
	calculateTextPosition();
}

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_text = NULL;

	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;

	char text[2];
	text[0] = letter;
	text[1] = '\0';

	setText(text);
	calculateTextPosition();
}

void TextBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	port->drawText(_textX, _textY, _font, _text);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

// Calculate values for centralised text
void TextBox::calculateTextPosition() {

	// Calculate vertical position
	switch (_vAlignment) {
		case TEXT_ALIGNMENT_VERT_CENTRE:
			_textY = (_height - _font->getHeight()) >> 1;
			break;
		case TEXT_ALIGNMENT_VERT_TOP:
			_textY = _padding;
			break;
		case TEXT_ALIGNMENT_VERT_BOTTOM:
			_textY = _height - _font->getHeight() - _padding;
			break;
	}

	// Calculate horizontal position
	switch (_hAlignment) {
		case TEXT_ALIGNMENT_HORIZ_CENTRE:
			_textX = (_width - _font->getStringWidth(_text)) >> 1;
			break;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			_textX = _padding;
			break;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			_textX = _width - _font->getStringWidth(_text) - _padding;
			break;
	}
}

void TextBox::setTextAlignmentHoriz(TextAlignmentHoriz alignment) {
	_hAlignment = alignment;
	calculateTextPosition();

	draw();
}

void TextBox::setTextAlignmentVert(TextAlignmentVert alignment) {
	_vAlignment = alignment;
	calculateTextPosition();

	draw();
}

void TextBox::setText(const char* text) {
	
	// Have we already created a block of memory that we need to free?
	if (_text != NULL) {
		// Free the memory
		delete[] _text;
	}

	// Move cursor to end of new text
	_cursorPos = strlen(text);

	// Create new memory for string
	_text = new char[_cursorPos + 1];

	// Copy text
	strcpy(_text, text);

	calculateTextPosition();

	draw();

	raiseValueChangeEvent();
}

void TextBox::setText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	setText(newText);

	delete[] newText;
}

void TextBox::appendText(const char* text) {

	if (_text != NULL) {

		// Move cursor to end of new string
		_cursorPos = strlen(_text) + strlen(text);

		// Reserve memory for concatenated string
		char* newText = new char[_cursorPos + 1];

		// Copy old text into new text
		strcpy(newText, _text);

		// Concatenate strings
		strcat(newText, text);

		// Free existing memory
		delete[] _text;

		// Update pointer
		_text = newText;

		draw();

		raiseValueChangeEvent();
	} else {
		// No text, so set it for the first time
		setText(text);
	}

	calculateTextPosition();
}

void TextBox::appendText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	appendText(newText);

	delete[] newText;
}

void TextBox::insertTextAtCursor(const char* text) {

	if (_text != NULL) {

		u32 oldLen = strlen(_text);
		u32 insertLen = strlen(text);

		// Reserve memory for new string
		char* newText = new char[oldLen + insertLen + 1];

		// Copy start of existing string into new string
		strncpy(newText, _text, _cursorPos);

		// Copy insert into new string
		strncpy(newText + _cursorPos, text, insertLen);

		// Copy end of existing string into new string
		strcpy(newText + _cursorPos + insertLen, _text + _cursorPos);

		// Move cursor to end of inserted text
		_cursorPos += insertLen;

		// Delete existing string
		delete[] _text;

		// Swap pointers
		_text = newText;

		draw();

		raiseValueChangeEvent();
	} else {
		// No text, so set it for the first time
		setText(text);
	}
}

void TextBox::insertTextAtCursor(const char text) {
	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	insertTextAtCursor(newText);

	delete[] newText;
}

bool TextBox::resize(u16 width, u16 height) {

	// Remember current values
	bool resized = false;
	bool drawing = _flags.drawingEnabled;

	// Hide and disable drawing
	erase();
	_flags.drawingEnabled = false;

	// Attempt to resize
	if (Gadget::resize(width, height)) {
		calculateTextPosition();

		resized = true;
	}

	// Show and reset drawing
	_flags.drawingEnabled = drawing;
	draw();
	
	return resized;
}

// Get the preferred dimensions of the gadget
void TextBox::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = ((!_flags.borderless + _padding) << 1) + _font->getStringWidth(_text);
	rect.height = ((!_flags.borderless + _padding) << 1) + _font->getHeight();
}

void TextBox::moveCursorToPosition(const u32 position) {
	u32 len = strlen(_text);
	_cursorPos = len >= position ? position : len;
}
