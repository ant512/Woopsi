#include "slidervertical.h"
#include "sliderverticalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderVertical::SliderVertical(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_outline = OUTLINE_IN;
	_minimumValue = 0;
	_maximumValue = 0;
	_minimumGripHeight = 5;
	_pageSize = 1;
	_flags.permeable = true;

	_flags.borderless = false;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderVerticalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->setEventHandler(this);
	addGadget(_grip);
}

const s16 SliderVertical::getValue() const {
	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);
	
	// Is the grip smaller than the gutter?
	if (rect.height > _grip->getHeight()) {
	
		// Calculate ratio
		u32 ratio = ((_maximumValue - _minimumValue) << 8) / rect.height;
		
		// Calculate value
		u32 val = ((_grip->getY() - getY()) - rect.y) * ratio;

		// Round up
		val += val & 128;

		// Right shift to erase fractional part and return
		return val >> 8;
	} else {
		// Just return the minimum value
		return _minimumValue;
	}
}

void SliderVertical::setValue(const s16 value) {

	// Convert the value to co-ordinates using fixed-point fractional values
	// for accuracy
	Rect rect;
	getClientRect(rect);

	s16 newValue = value;

	// Limit to max/min values
	if (newValue > _maximumValue) newValue = _maximumValue;
	if (newValue < _minimumValue) newValue = _minimumValue;
	
	// Can the grip move?
	if (rect.height > _grip->getHeight()) {
	
		// Calculate ratio (max fractional value of 255)
		u32 ratio = (rect.height << 8) / (u32)(_maximumValue - _minimumValue);
		
		// Convert value using ratio
		s32 newGripY = (newValue * ratio);

		// Round up
		newGripY += newGripY & 128;

		// Bitshift down
		newGripY >>= 8;

		// Add containing client co-ords
		newGripY += rect.y;
		
		// Adjust new y so that it fits within gutter
		if (newGripY + _grip->getHeight() > rect.y + rect.height) {
			newGripY = (rect.y + rect.height) - _grip->getHeight();
		}

		// Move the grip
		_grip->moveTo(rect.x, newGripY);
	}
}

void SliderVertical::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw background
	port->drawFilledRect(0, 0, _width, _height, _darkColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

bool SliderVertical::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		// Did we click a gadget?
		if (_flags.clicked) {

			// Which way should the grip move?
			if (y > _grip->getY()) {
				// Move grip down
				setValue(getValue() + _pageSize);
				//jumpGrip(1);
			} else {
				// Move grip up
				setValue(getValue() - _pageSize);
				//jumpGrip(0);
			}
		}

		return true;
	}

	return false;
}

bool SliderVertical::handleEvent(const EventArgs& e) {

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

void SliderVertical::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);

	// Calculate new height
	s32 newHeight = rect.height;
	
	// Calculate the height of the content that has overflowed the viewport
	s32 overspill = ((s32)(_maximumValue - _minimumValue)) - _pageSize;
	
	// Is there any overflow?
	if (overspill > 0) {
	
		// Calculate the ratio of content to gutter
		u32 ratio = (rect.height << 8) / (u32)(_maximumValue - _minimumValue);
		
		// New height is equivalent to the height of the gutter minus
		// the ratio-converted overflow height
		newHeight = (rect.height << 8) - (overspill * ratio);

		// Round up
		newHeight += newHeight & 128;

		// Bitshift to remove fraction
		newHeight >>= 8;

		// Ensure height is within acceptable boundaries
		if (newHeight < _minimumGripHeight) newHeight = _minimumGripHeight;
		if (newHeight > rect.height) newHeight = rect.height;
	}

	// Perform resize
	_grip->resize(rect.width, newHeight);
}

void SliderVertical::jumpGrip(u8 direction) {

	s32 newGripY;

	// Which way should the grip move?
	if (direction == 1) {
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
	} else if (newGripY + _grip->getHeight() > rect.y + rect.height) {
		newGripY = (rect.height - _grip->getHeight()) + 1;
	}

	// Move the grip
	_grip->moveTo(rect.x, newGripY);
}

bool SliderVertical::resize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool resized = false;
	bool events = raisesEvents();
	bool drawing = _flags.drawingEnabled;

	// Disable event raising
	setRaisesEvents(false);

	// Hide and disable drawing
	erase();
	_flags.drawingEnabled = false;

	if (Gadget::resize(width, height)) {
		resizeGrip();

		// Set back to current value
		setValue(value);

		resized = true;
	}

	// Show and reset drawing
	_flags.drawingEnabled = drawing;
	draw();

	// Reset event raising
	setRaisesEvents(events);

	return resized;
}
