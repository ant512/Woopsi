#include "sliderhorizontalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderHorizontalGrip::SliderHorizontalGrip(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
}

void SliderHorizontalGrip::drawContents(GraphicsPort* port) {
	if (!_flags.clicked) {
		port->drawFilledRect(0, 0, getWidth(), getHeight(), getFillColour());
	} else {
		port->drawFilledRect(0, 0, getWidth(), getHeight(), getHighlightColour());
	}
}

void SliderHorizontalGrip::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	if (isClicked()) {
		port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShadowColour(), getShineColour());
	} else {
		port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShineColour(), getShadowColour());
	}
}

void SliderHorizontalGrip::onClick(s16 x, s16 y) {
	startDragging(x, y);
	redraw();
}

void SliderHorizontalGrip::onRelease(s16 x, s16 y) {
	redraw();
}

void SliderHorizontalGrip::onReleaseOutside(s16 x, s16 y) {
	redraw();
}

void SliderHorizontalGrip::onDrag(s16 x, s16 y, s16 vX, s16 vY) {

	// Work out where we're moving to
	s16 destX = x - _grabPointX - _parent->getX();

	// Do we need to move?
	if (destX != _rect.getX()) {

		// Get parent rect
		Rect rect;
		_parent->getClientRect(rect);

		// Prevent grip from moving outside parent
		if (destX < rect.x) {
			destX = rect.x;
		} else {
			if (destX + getWidth() > rect.width + rect.x) {
				destX = (rect.width + rect.x) - getWidth() ;
			}
		}

		// Move to new location
		moveTo(destX, rect.y);
	}
}
