#include "textbox.h"
#include "graphicsport.h"

Textbox::Textbox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_text = NULL;

	_outline = OUTLINE_IN;

	_hPos = TEXT_POSITION_HORIZ_CENTRE;
	_vPos = TEXT_POSITION_VERT_CENTRE;
	_padding = 2;

	setText(text);
	calculateTextPosition();
}

Textbox::Textbox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_text = NULL;

	_outline = OUTLINE_IN;

	_hPos = TEXT_POSITION_HORIZ_CENTRE;
	_vPos = TEXT_POSITION_VERT_CENTRE;
	_padding = 2;

	char text[2];
	text[0] = letter;
	text[1] = '\0';

	setText(text);
	calculateTextPosition();
}

void Textbox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	port->drawText(_textX, _textY, _font, _text);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

// Calculate values for centralised text
void Textbox::calculateTextPosition() {

	// Calculate vertical position
	switch (_vPos) {
		case TEXT_POSITION_VERT_CENTRE:
			_textY = (_height >> 1) - (_font->getHeight() >> 1);
			break;
		case TEXT_POSITION_VERT_TOP:
			_textY = _padding;
			break;
		case TEXT_POSITION_VERT_BOTTOM:
			_textY = _height - _font->getHeight() - _padding;
			break;
	}

	// Calculate horizontal position
	switch (_hPos) {
		case TEXT_POSITION_HORIZ_CENTRE:
			_textX = (_width >> 1) - ((_font->getWidth() * strlen(_text)) >> 1);
			break;
		case TEXT_POSITION_HORIZ_LEFT:
			_textX = _padding;
			break;
		case TEXT_POSITION_HORIZ_RIGHT:
			_textX = _width - (_font->getWidth() * strlen(_text)) - _padding;
			break;
	}
}

void Textbox::setTextPositionHoriz(TextPositionHoriz position) {
	_hPos = position;
	calculateTextPosition();

	draw();
}

void Textbox::setTextPositionVert(TextPositionVert position) {
	_vPos = position;
	calculateTextPosition();

	draw();
}

void Textbox::setText(const char* text) {
	
	// Have we already created a block of memory that we need to free?
	if (_text != NULL) {
		// Free the memory
		delete[] _text;
	}

	// Create new memory for string
	_text = new char[strlen(text) + 1];

	// Copy text
	strcpy(_text, text);

	calculateTextPosition();

	draw();

	raiseValueChangeEvent();
}

void Textbox::addText(const char* text) {

	if (_text != NULL) {
		// Reserve memory for concatenated string
		char* newText = new char[strlen(_text) + strlen(text) + 2];

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

bool Textbox::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {
		draw();
		return true;
	}

	return false;
}

bool Textbox::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {
		draw();
		return true;
	}

	return false;
}

bool Textbox::resize(u16 width, u16 height) {

	// Remember current values
	bool resized = false;
	bool visible = isVisible();

	// Hide and disable drawing
	erase();
	setVisible(false);

	// Attempt to resize
	if (Gadget::resize(width, height)) {
		calculateTextPosition();

		resized = true;
	}

	// Show and reset drawing
	setVisible(visible);
	draw();

	return resized;
}

// Get the preferred dimensions of the gadget
void Textbox::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = (_padding << 1) + (_font->getWidth() * strlen(_text));
	rect.height = (_padding << 1) + _font->getHeight();
}
