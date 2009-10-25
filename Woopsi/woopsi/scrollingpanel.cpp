#include "scrollingpanel.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

ScrollingPanel::ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	
	Rect rect;
	getClientRect(rect);
	
	_canvasWidth = rect.width;
	_canvasHeight = rect.height;
	_canvasX = 0;
	_canvasY = 0;
	_flags.permeable = true;
}

void ScrollingPanel::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw border
	port->drawBevelledRect(0, 0, _width, _height);
	delete port;
}

void ScrollingPanel::jump(s32 x, s32 y) {
	// Calculate difference between jump value and current value and scroll
	scroll(x - _canvasX, y - _canvasY);
}

void ScrollingPanel::scroll(s32 dx, s32 dy) {

	Rect rect;
	getClientRect(rect);

	// Prevent scrolling outside boundaries
	if (_canvasX + dx < -(_canvasWidth - rect.width)) {
		dx = -(_canvasWidth - rect.width) - _canvasX;
	} else if (_canvasX + dx > 0) {
		dx = -_canvasX;
	}

	if (_canvasY + dy < -(_canvasHeight - rect.height)) {
		dy = -(_canvasHeight - rect.height) - _canvasY;
	} else if (_canvasY + dy > 0) {
		dy = -_canvasY;
	}

	// Perform scroll if necessary
	if ((dx != 0) || (dy != 0)) {

		// Perform scroll
		WoopsiArray<Gadget::Rect> revealedRects;
		Rect rect;
		getClientRect(rect);
		GraphicsPort* port = newGraphicsPort(true);
		port->scroll(0, 0, dx, dy, rect.width, rect.height, &revealedRects);
		delete port;

		// Adjust the scroll values
		_canvasY += dy;
		_canvasX += dx;

		// Draw revealed sections
		for (s32 i = 0; i < revealedRects.size(); ++i) {
			draw(revealedRects.at(i));
		}

		// Scroll all child gadgets
		scrollChildren(dx, dy);

		// Notify event handler
		raiseScrollEvent();
	}
}

// Reposition child gadgets without redrawing
void ScrollingPanel::scrollChildren(s32 dx, s32 dy) {
	s16 gadgetX = 0;
	s16 gadgetY = 0;
	for (s32 i = 0; i < _gadgets.size(); i++) {
		gadgetX = (_gadgets[i]->getX() - getX()) + dx;
		gadgetY = (_gadgets[i]->getY() - getY()) + dy;

		_gadgets[i]->moveTo(gadgetX, gadgetY);
	}
}

bool ScrollingPanel::drag(s16 x, s16 y, s16 vX, s16 vY) {

	if (isEnabled()) {
		if (_flags.dragging) {

			// Only run drag code if stylus has moved
			if ((vX != 0) || (vY != 0)) {

				// Perform scroll
				scroll(vX, vY);

				raiseDragEvent(x, y, vX, vY);

				return true;
			}
		}
	}

	return false;
}

// Ensure clipping rect falls within the client region of the gadget
bool ScrollingPanel::clipToClientRect(Rect& clipRect) {

	// Get parent-relative client region
	Rect clientRect;
	getClientRect(clientRect);

	// Adjust rect to screen-relative co-ordinates
	clientRect.x += getX();
	clientRect.y += getY();

	// Get end co-ords of clipping rect
	s32 clipX2 = clipRect.x + clipRect.width - 1;
	s32 clipY2 = clipRect.y + clipRect.height - 1;

	// Clip x to client region
	if (clipRect.x < clientRect.x) {
		clipRect.x = clientRect.x;
	}

	// Clip y to client region
	if (clipRect.y < clientRect.y) {
		clipRect.y = clientRect.y;
	}

	// Clip width to client region
	if (clipX2 > (clientRect.x + clientRect.width) - 1) {
		clipX2 = (clientRect.x + clientRect.width) - 1;
	}

	// Clip height to client region
	if (clipY2 > (clientRect.y + clientRect.height) - 1) {
		clipY2 = (clientRect.y + clientRect.height) - 1;
	}

	// Clip height to screen height
	if ((clipY2 >= SCREEN_HEIGHT) && (clipY2 < TOP_SCREEN_Y_OFFSET)) {
		clipY2 = SCREEN_HEIGHT - 1;
	} else if (clipY2 >= TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT) {
		clipY2 = TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT - 1;
	}

	// Do we have a valid region?
	if ((clipX2 <= clipRect.x) || (clipY2 <= clipRect.y)) {
		return false;
	}

	// Region OK - update rect
	clipRect.width = (clipX2 - clipRect.x) + 1;
	clipRect.height = (clipY2 - clipRect.y) + 1;

	// Adjust y value to match screen
	if (clipRect.y > TOP_SCREEN_Y_OFFSET) {
		clipRect.y -= TOP_SCREEN_Y_OFFSET;
	}

	return true;
}

void ScrollingPanel::raiseScrollEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);
	
		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleScrollEvent(e);
		}
	}
}
