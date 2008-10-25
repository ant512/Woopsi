#include "contextmenuitem.h"
#include "graphicsport.h"

using namespace WoopsiUI;

ContextMenuItem::ContextMenuItem(s16 x, s16 y, u16 width, u16 height, const char* text, u32 value, FontBase* font) : Label(x, y, width, height, text, font) {
	setBorderless(true);
	setTextAlignmentVert(TEXT_ALIGNMENT_VERT_CENTRE);
	setTextAlignmentHoriz(TEXT_ALIGNMENT_HORIZ_LEFT);
	_value = value;
}

void ContextMenuItem::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	if (!isEnabled()) {
		// Draw disabled item
		port->drawFilledRect(0, 0, _width, _height, _shineColour);
		port->drawText(_textX, _textY, _font, _text->getCharArray(), _darkColour);
	} else if (isClicked()) {
		// Draw clicked item
		port->drawFilledRect(0, 0, _width, _height, _shadowColour);
		port->drawText(_textX, _textY, _font, _text->getCharArray(), _shineColour);
	} else {
		// Draw standard item
		port->drawFilledRect(0, 0, _width, _height, _shineColour);
		port->drawText(_textX, _textY, _font, _text->getCharArray());
	}

	delete port;
}

bool ContextMenuItem::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {
		draw();
		return true;
	}

	return false;
}

bool ContextMenuItem::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {
		draw();
		return true;
	}

	return false;
}
