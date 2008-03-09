#include "sliderhorizontal.h"
#include <stdlib.h>

SliderHorizontal::SliderHorizontal(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_outline = OUTLINE_IN;
	_minimumValue = 0;
	_maximumValue = 0;
	_minimumGripWidth = 5;
	_pageSize = 1;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderHorizontalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->setEventHandler(this);
	addGadget(_grip);
}

SliderHorizontal::~SliderHorizontal() {
}

const s16 SliderHorizontal::getMinimumValue() const {
	return _minimumValue;
}

const s16 SliderHorizontal::getMaximumValue() const {
	return _maximumValue;
}

const s16 SliderHorizontal::getValue() const {
	// Calculate the current value represented by the left edge of the grip
	Rect rect;
	getClientRect(rect);
	
	// Is the grip smaller than the gutter?
	if (rect.width > _grip->getWidth()) {
	
		// Calculate ratio
		u32 ratio = (abs(_maximumValue - _minimumValue) << 8) / rect.width;
		
		// Calculate value
		u32 val ((_grip->getX() - getX()) * ratio);
		
		// Round up if fractional value is >= 128
		if (val & 128) {
			val += 0x100;
		}

		// Right shift to erase fractional part and return
		return val >> 8;
	} else {
		// Just return the minimum value
		return _minimumValue;
	}
}

const s16 SliderHorizontal::getPageSize() const {
	return _pageSize;
}

void SliderHorizontal::setMinimumValue(const s16 value) {
	_minimumValue = value;
}

void SliderHorizontal::setMaximumValue(const s16 value) {
	_maximumValue = value;
}

void SliderHorizontal::setValue(const s16 value) {

	// Convert the value to co-ordinates using fixed-point fractional values
	// for accuracy
	Rect rect;
	getClientRect(rect);
	
	// Can the grip move?
	if (rect.width > _grip->getWidth()) {
	
		// Calculate ratio (max fractional value of 255)
		u32 ratio = (abs(_maximumValue - _minimumValue) << 8) / rect.width;
		
		// Bitshift value up by 12 spaces so that the value returned is still
		// bitshifted 4 places up and thus has a fractional nibble
		s16 newGripX = (value << 12) / ratio;
		
		// Round up if fractional value is >= 8
		if (newGripX & 8) {
			newGripX += 0x10;
		}
		
		// Bitshift back down to eliminate fraction
		newGripX >>= 4;
		
		// Adjust new x so that it fits within gutter
		if (newGripX + _grip->getWidth() > rect.x + rect.width) {
			newGripX = (rect.x + rect.width) - _grip->getWidth();
		}

		// Move the grip
		_grip->moveTo(abs(newGripX), 0);
	}
}

void SliderHorizontal::setPageSize(s16 pageSize) {
	_pageSize = pageSize;
}

void SliderHorizontal::draw() {
	Gadget::draw();
}

void SliderHorizontal::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	port->drawFilledRect(0, 0, _width, _height, _darkColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool SliderHorizontal::click(s16 x, s16 y) {
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
				s16 newGripX;

				// Which way should the grip move?
				if (x > _grip->getX()) {
					// Move grip right
					newGripX = (_grip->getX() - getX()) + _grip->getWidth();
				} else {
					// Move grip left
					newGripX = (_grip->getX() - getX()) - _grip->getWidth();
				}

				// Get client rect for this gadget
				Rect rect;
				getClientRect(rect);

				// Adjust x value so that it does not exceed boundaries of gutter
				if (newGripX < rect.x) {
					newGripX = rect.x;
				} else if (newGripX + _grip->getWidth() > rect.x + rect.width) {
					newGripX = (rect.width - _grip->getWidth()) + 1;
				}

				// Handle click on gutter
				Gadget::click(x, y);

				// Move the grip
				_grip->moveTo(newGripX, 0);
			}

			return true;
		}
	}

	return false;
}

bool SliderHorizontal::release(s16 x, s16 y) {
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

bool SliderHorizontal::drag(s16 x, s16 y, s16 vX, s16 vY) {
	// Handle child dragging
	if (_clickedGadget != NULL) {
		return _clickedGadget->drag(x, y, vX, vY);
	}

	return false;
}

bool SliderHorizontal::handleEvent(const EventArgs& e) {

	// Handle grip events
	if ((e.gadget == _grip) && (e.gadget != NULL)) {
		if ((e.type == EVENT_DRAG) || (e.type == EVENT_MOVE)) {

			// Grip has moved
			raiseValueChangeEvent();

			return true;
		}
	}

	return false;
}

void SliderHorizontal::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);

	// Calculate new width
	s32 newWidth = rect.width;
	
	// Calculate the width of the content that has overflowed the viewport
	s32 overspill = ((abs(_maximumValue - _minimumValue) - _pageSize) << 12);
	
	// Is there any overflow?
	if (overspill > 0) {
	
		// Calculate the ratio of content to gutter
		u32 ratio = (abs(_maximumValue - _minimumValue) << 8) / rect.width;
		
		// New width is equivalent to the width of the gutter minus
		// the ratio-converted overflow width
		newWidth = (rect.width << 4) - (overspill / ratio);
		
		// Handle rounding
		if (newWidth & 8) {
			newWidth += 0x10;
		}
		
		// Bitshift to remove fraction
		newWidth >>= 4;

		// Ensure width is within acceptable boundaries
		if (newWidth < _minimumGripWidth) newWidth = _minimumGripWidth;
		if (newWidth > rect.width) newWidth = rect.width;
	}

	// Perform resize
	_grip->resize(newWidth, rect.height);
}
