#include "label.h"
#include "graphicsport.h"

using namespace WoopsiUI;

Label::Label(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : Gadget(x, y, width, height, 0, style) {
	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;

	setText(text);

	calculateTextPosition();
}

void Label::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	if (isEnabled()) {
		port->drawText(_textX, _textY, getFont(), _text);
	} else {
		port->drawText(_textX, _textY, getFont(), _text, 0, _text.getLength(), getDarkColour());
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

// Calculate values for centralised text
void Label::calculateTextPosition() {

	// Calculate vertical position
	switch (_vAlignment) {
		case TEXT_ALIGNMENT_VERT_CENTRE:
			_textY = (_height - getFont()->getHeight()) >> 1;
			break;
		case TEXT_ALIGNMENT_VERT_TOP:
			_textY = _padding;
			break;
		case TEXT_ALIGNMENT_VERT_BOTTOM:
			_textY = _height - getFont()->getHeight() - _padding;
			break;
	}

	// Calculate horizontal position
	switch (_hAlignment) {
		case TEXT_ALIGNMENT_HORIZ_CENTRE:
			_textX = (_width - getFont()->getStringWidth(_text)) >> 1;
			break;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			_textX = _padding;
			break;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			_textX = _width - getFont()->getStringWidth(_text) - _padding;
			break;
	}
}

void Label::setTextAlignmentHoriz(TextAlignmentHoriz alignment) {
	_hAlignment = alignment;
	calculateTextPosition();

	redraw();
}

void Label::setTextAlignmentVert(TextAlignmentVert alignment) {
	_vAlignment = alignment;
	calculateTextPosition();

	redraw();
}

void Label::setText(const WoopsiString& text) {
	_text = text;
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void Label::appendText(const WoopsiString& text) {
	_text.append(text);
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void Label::insertText(const WoopsiString& text, const u32 index) {
	_text.insert(text, index);
	calculateTextPosition();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
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
	redraw();
	
	return resized;
}

// Get the preferred dimensions of the gadget
void Label::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = ((!_flags.borderless + _padding) << 1) + getFont()->getStringWidth(_text);
	rect.height = ((!_flags.borderless + _padding) << 1) + getFont()->getHeight();
}

void Label::setFont(FontBase* font) {
	_style->font = font;

	// Need to recalculate the text position as the font may have changed size
	calculateTextPosition();

	redraw();
}
