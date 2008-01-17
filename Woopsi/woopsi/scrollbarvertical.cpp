#include "scrollbarvertical.h"

ScrollbarVertical::ScrollbarVertical(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_outline = OUTLINE_IN;

	// Create grip
	_gripY = 1;
	_grip = new ScrollbarVerticalGrip(1, _gripY, width - 2, 10);
	_grip->setEventHandler(this);
	addGadget(_grip);
}

ScrollbarVertical::~ScrollbarVertical() {
	delete _grip;
}

void ScrollbarVertical::draw() {
	Gadget::draw();
}

void ScrollbarVertical::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	port->drawFilledRect(0, 0, _width, _height, _darkColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool ScrollbarVertical::click(s16 x, s16 y) {
	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					break;
				}
			}

			// Did we click a gadget?
			if (_clickedGadget == NULL) {

				// Move the grip
				s16 newGripY;

				// Which way should the grip move?
				if (y > _grip->getY()) {
					// Move grip down
					newGripY = (_grip->getY() - getY()) + _grip->getHeight();
				} else {
					// Move grip up
					newGripY = (_grip->getY() - getY()) - _grip->getHeight();
				}

				// Get client rect for this gadget
				Rect rect;
				getClientRect(rect);

				// Adjust y value so that it does not exceed boundaries of gutter
				if (newGripY < rect.y) {
					newGripY = rect.y;
				} else if (newGripY + _grip->getHeight() > rect.height) {
					newGripY = (rect.height - _grip->getHeight()) + 1;
				}

				// Move the grip
				_grip->moveTo(0, newGripY);

				// Notify event handler of the scroll
				// Pass scroll distance as arguments
				raiseScrollEvent(0, newGripY - _gripY);

				// Update stored grip Y position
				_gripY = newGripY;

				// Handle click on gutter
				Gadget::click(x, y);
			}

			return true;
		}
	}

	return false;
}

bool ScrollbarVertical::release(s16 x, s16 y) {
	if (_clickedGadget != NULL) {

		// Release clicked gadget
		_clickedGadget->release(x, y);

		return true;
	} else if (_flags.clicked) {

		// Handle release on window
		Gadget::release(x, y);

		return true;
	}

	return false;
}

bool ScrollbarVertical::drag(s16 x, s16 y, s16 vX, s16 vY) {
	// Handle child dragging
	if (_clickedGadget != NULL) {
		return _clickedGadget->drag(x, y, vX, vY);
	}

	return false;
}

bool ScrollbarVertical::handleEvent(const EventArgs& e) {

	// Handle grip events
	if ((e.gadget == _grip) && (e.gadget != NULL)) {
		if (e.type == EVENT_DRAG) {

			// Grip being dragged - raise scroll event
			// Pass scroll distance as arguments
			s16 newGripY = e.gadget->getY();

			raiseScrollEvent(0, newGripY - _gripY);

			// Update stored grip Y position
			_gripY = newGripY;

			// TODO: Handle grip dragging here
			return true;
		}
	}

	return false;
}

void ScrollbarVertical::raiseScrollEvent(s16 x, s16 y) {
	if (_eventHandler != NULL) {

		EventArgs e;
		e.type = EVENT_SCROLL;
		e.eventX = x;
		e.eventY = y;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}
