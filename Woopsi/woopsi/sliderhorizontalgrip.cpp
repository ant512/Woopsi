#include "sliderhorizontalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderHorizontalGrip::SliderHorizontalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
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
	if (isEnabled()) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destX = x - _grabPointX - _parent->getX();

			// Do we need to move?
			if (destX != _x) {

				// Get parent rect
				Rect rect;
				_parent->getClientRect(rect);

				// Prevent grip from moving outside parent
				if (destX < rect.x) {
					destX = rect.x;
				} else {
					if (destX + _width > rect.width + rect.x) {
						destX = (rect.width + rect.x) - _width ;
					}
				}

				// Move to new location
				moveTo(destX, rect.y);
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}
	
	return false;
}
