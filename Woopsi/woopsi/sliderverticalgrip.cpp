#include "sliderverticalgrip.h"

SliderVerticalGrip::SliderVerticalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

SliderVerticalGrip::~SliderVerticalGrip() {
}

void SliderVerticalGrip::draw() {
	Gadget::draw();
}

void SliderVerticalGrip::draw(Rect clipRect) {
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

bool SliderVerticalGrip::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		setDragging(x, y);

		draw();
		return true;
	}

	return false;
}

bool SliderVerticalGrip::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {

		_flags.dragging = false;

		draw();
		return true;
	}

	return false;
}

bool SliderVerticalGrip::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_flags.enabled) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destY = y - _grabPointY - _parent->getY();

			// Do we need to move?
			if (destY != y) {

				// Prevent grip from moving outside parent
				if (destY < 0) {
					destY = 0;
				} else {
					// Get parent rect
					Rect rect;
					_parent->getClientRect(rect);

					if (destY + _height > rect.height) {
						destY = rect.height - _height + 1;
					}
				}

				// Move to new location
				moveTo(0, destY);
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}
	
	return false;
}
