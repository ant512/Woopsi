#include "sliderverticalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderVerticalGrip::SliderVerticalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

void SliderVerticalGrip::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	if (!_flags.clicked) {
		port->drawFilledRect(0, 0, _width, _height, getFillColour());
	} else {
		port->drawFilledRect(0, 0, _width, _height, getHighlightColour());
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool SliderVerticalGrip::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (isEnabled()) {
			setDragging(x, y);
			redraw();
		}

		return true;
	}

	return false;
}

bool SliderVerticalGrip::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {

		_flags.dragging = false;

		redraw();
		return true;
	}

	return false;
}

bool SliderVerticalGrip::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (isEnabled()) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destY = y - _grabPointY - _parent->getY();

			// Do we need to move?
			if (destY != _y) {

				// Get parent rect
				Rect rect;
				_parent->getClientRect(rect);

				// Prevent grip from moving outside parent
				if (destY < rect.y) {
					destY = rect.y;
				} else {
					if (destY + _height > rect.height + rect.y) {
						destY = (rect.height + rect.y) - _height;
					}
				}

				// Move to new location
				moveTo(rect.x, destY);
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}
	
	return false;
}
