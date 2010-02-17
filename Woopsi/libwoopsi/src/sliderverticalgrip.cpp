#include "sliderverticalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderVerticalGrip::SliderVerticalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

void SliderVerticalGrip::drawContents(GraphicsPort* port) {
	if (!_flags.clicked) {
		port->drawFilledRect(0, 0, _width, _height, getFillColour());
	} else {
		port->drawFilledRect(0, 0, _width, _height, getHighlightColour());
	}
}

void SliderVerticalGrip::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	if (isClicked()) {
		port->drawBevelledRect(0, 0, _width, _height, getShadowColour(), getShineColour());
	} else {
		port->drawBevelledRect(0, 0, _width, _height, getShineColour(), getShadowColour());
	}
}

void SliderVerticalGrip::onClick(s16 x, s16 y) {
	startDragging(x, y);
	redraw();
}

void SliderVerticalGrip::onRelease(s16 x, s16 y) {
	redraw();
}

void SliderVerticalGrip::onReleaseOutside(s16 x, s16 y) {
	redraw();
}

void SliderVerticalGrip::onDrag(s16 x, s16 y, s16 vX, s16 vY) {

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
}
