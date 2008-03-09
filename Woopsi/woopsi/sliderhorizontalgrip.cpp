#include "sliderhorizontalgrip.h"

SliderHorizontalGrip::SliderHorizontalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

SliderHorizontalGrip::~SliderHorizontalGrip() {
}

void SliderHorizontalGrip::draw() {
	Gadget::draw();
}

void SliderHorizontalGrip::draw(Rect clipRect) {
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

bool SliderHorizontalGrip::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		setDragging(x, y);

		draw();
		return true;
	}

	return false;
}

bool SliderHorizontalGrip::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {

		_flags.dragging = false;

		draw();
		return true;
	}

	return false;
}

bool SliderHorizontalGrip::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_flags.enabled) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destX = x - _grabPointX - _parent->getX();

			// Do we need to move?
			if (destX != _x) {

				// Prevent grip from moving outside parent
				if (destX < 0) {
					destX = 0;
				} else {
					// Get parent rect
					Rect rect;
					_parent->getClientRect(rect);

					if (destX + _width > rect.width) {
						destX = rect.width - _width + 1;
					}
				}

				// Move to new location
				moveTo(destX, 0);
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}
	
	return false;
}
