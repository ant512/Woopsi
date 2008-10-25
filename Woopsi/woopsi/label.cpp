#include "label.h"
#include "graphicsport.h"

using namespace WoopsiUI;

Label::Label(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;

	_text = new WoopsiString();
	setText(text);

	calculateTextPosition();
}

Label::Label(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;

	_text = new WoopsiString();
	setText(letter);

	calculateTextPosition();
}

void Label::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	port->drawText(_textX, _textY, _font, _text->getCharArray());

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

// Calculate values for centralised text
void Label::calculateTextPosition() {

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
			_textX = (_width - _font->getStringWidth(_text->getCharArray())) >> 1;
			break;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			_textX = _padding;
			break;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			_textX = _width - _font->getStringWidth(_text->getCharArray()) - _padding;
			break;
	}
}

void Label::setTextAlignmentHoriz(TextAlignmentHoriz alignment) {
	_hAlignment = alignment;
	calculateTextPosition();

	draw();
}

void Label::setTextAlignmentVert(TextAlignmentVert alignment) {
	_vAlignment = alignment;
	calculateTextPosition();

	draw();
}

void Label::setText(const char* text) {
	_text->setText(text);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void Label::setText(const char text) {
	_text->setText(text);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void Label::appendText(const char* text) {
	_text->append(text);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void Label::appendText(const char text) {
	_text->append(text);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void Label::insertText(const char* text, const u32 index) {
	_text->insert(text, index);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

void Label::insertText(const char text, const u32 index) {
	_text->insert(text, index);
	calculateTextPosition();
	draw();
	raiseValueChangeEvent();
}

bool Label::resize(u16 width, u16 height) {

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
void Label::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = ((!_flags.borderless + _padding) << 1) + _font->getStringWidth(_text->getCharArray());
	rect.height = ((!_flags.borderless + _padding) << 1) + _font->getHeight();
}
