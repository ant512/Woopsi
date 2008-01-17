#include "scrollbarverticalgrip.h"

ScrollbarVerticalGrip::ScrollbarVerticalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

ScrollbarVerticalGrip::~ScrollbarVerticalGrip() {
}

void ScrollbarVerticalGrip::draw() {
	Gadget::draw();
}

void ScrollbarVerticalGrip::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	if (!_flags.clicked) {
		port->drawFilledRect(0, 0, _width, _height, _fillColour);
	} else {
		port->drawFilledRect(0, 0, _width, _height, _highlightColour);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool ScrollbarVerticalGrip::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		setDragging(x, y);

		draw();
		return true;
	}

	return false;
}

bool ScrollbarVerticalGrip::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {

		_flags.dragging = false;

		draw();
		return true;
	}

	return false;
}

bool ScrollbarVerticalGrip::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_flags.enabled) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destX = x - _grabPointX - _parent->getX();
			s16 destY = y - _grabPointY - _parent->getY();

			// Do we need to move?
			if ((destX != x) && (destY != y)) {

				// Prevent grip from moving outside parent
				if (destX < 0) {
					destX = 0;
				} else if (destX + _width > _parent->getWidth()) {
					destX = _parent->getWidth() - _width;
				}

				if (destY < 0) {
					destY = 0;
				} else if (destY + _height > _parent->getY() + _parent->getHeight()) {
					destY = _parent->getHeight() - _height;
				}

				// Move to new location
				moveTo(destX, destY);
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}
	
	return false;
}
