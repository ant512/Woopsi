#include "listboxitem.h"
#include "graphicsport.h"

ListBoxItem::ListBoxItem(s16 x, s16 y, u16 width, u16 height, const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour, FontBase* font) : Textbox(x, y, width, height, text, font) {
	setBorderless(true);
	setTextPositionVert(TEXT_POSITION_VERT_CENTRE);
	setTextPositionHoriz(TEXT_POSITION_HORIZ_LEFT);
	_value = value;
	_normalTextColour = normalTextColour;
	_normalBackColour = normalBackColour;
	_selectedTextColour = selectedTextColour;
	_selectedBackColour = selectedBackColour;
	_flags.doubleClickable = true;
}

void ListBoxItem::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (isSelected()) {
		port->drawFilledRect(0, 0, _width, _height, _selectedBackColour);
		port->drawText(_textX, _textY, _font, _text, _selectedTextColour);
	} else {
		port->drawFilledRect(0, 0, _width, _height, _normalBackColour);
		port->drawText(_textX, _textY, _font, _text, _normalTextColour);
	}

	delete port;
}

bool ListBoxItem::select() {
	if (!_selected) {
		_selected = true;
		draw();
		return true;
	}

	return false;
}

bool ListBoxItem::unselect() {
	if (_selected) {
		_selected = false;
		draw();
		return true;
	}

	return false;
}
