#include "sliderhorizontal.h"
#include "sliderhorizontalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderHorizontal::SliderHorizontal(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_outline = OUTLINE_IN;
	_minimumValue = 0;
	_maximumValue = 0;
	_minimumGripWidth = 5;
	_pageSize = 1;
	_flags.permeable = true;

	_flags.borderless = false;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderHorizontalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->setEventHandler(this);
	addGadget(_grip);
}

const s16 SliderHorizontal::getValue() const {
	// Calculate the current value represented by the left edge of the grip
	Rect rect;
	getClientRect(rect);
	
	// Is the grip smaller than the gutter?
	if (rect.width > _grip->getWidth()) {
	
		// Calculate ratio
		u32 ratio = ((_maximumValue - _minimumValue) << 8) / rect.width;
		
		// Calculate value
		u32 val = ((_grip->getX() - getX()) - rect.x) * ratio;

		// Round up
		val += val & 128;

		// Right shift to erase fractional part and return
		return val >> 8;
	} else {
		// Just return the minimum value
		return _minimumValue;
	}
}

void SliderHorizontal::setValue(const s16 value) {

	// Convert the value to co-ordinates using fixed-point fractional values
	// for accuracy
	Rect rect;
	getClientRect(rect);

	s16 newValue = value;

	// Limit to max/min values
	if (newValue > _maximumValue) newValue = _maximumValue;
	if (newValue < _minimumValue) newValue = _minimumValue;
	
	// Can the grip move?
	if (rect.width > _grip->getWidth()) {
	
		// Calculate ratio (max fractional value of 255)
		u32 ratio = (rect.width << 8) / (u32)(_maximumValue - _minimumValue);

		// Convert value using ratio
		s32 newGripX = (newValue * ratio);

		// Round up
		newGripX += newGripX & 128;

		// Bitshift down
		newGripX >>= 8;

		// Add containing client co-ords
		newGripX += rect.x;

		// Adjust new x so that it fits within gutter
		if (newGripX + _grip->getWidth() > rect.x + rect.width) {
			newGripX = (rect.x + rect.width) - _grip->getWidth();
		}

		// Move the grip
		_grip->moveTo(newGripX, rect.y);
	}
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
	if (Gadget::click(x, y)) {

		// Did we click a gadget?
		if (_flags.clicked) {

			// Which way should the grip move?
			if (x > _grip->getX()) {
				// Move grip right
				setValue(getValue() + _pageSize);
				//jumpGrip(1);
			} else {
				// Move grip left
				setValue(getValue() - _pageSize);
				//jumpGrip(0);
			}
		}

		return true;
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
	s32 overspill = ((s32)(_maximumValue - _minimumValue)) - _pageSize;
	
	// Is there any overflow?
	if (overspill > 0) {
	
		// Calculate the ratio of content to gutter
		u32 ratio = (rect.width << 8) / (u32)(_maximumValue - _minimumValue);
		
		// New width is equivalent to the width of the gutter minus
		// the ratio-converted overflow width
		newWidth = (rect.width << 8) - (overspill * ratio);

		// Round up
		newWidth += newWidth & 128;

		// Bitshift to remove fraction
		newWidth >>= 8;

		// Ensure width is within acceptable boundaries
		if (newWidth < _minimumGripWidth) newWidth = _minimumGripWidth;
		if (newWidth > rect.width) newWidth = rect.width;
	}

	// Perform resize
	_grip->resize(newWidth, rect.height);
}

void SliderHorizontal::jumpGrip(u8 direction) {

	s32 newGripX;

	// Which way should the grip move?
	if (direction == 1) {
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

	// Move the grip
	_grip->moveTo(newGripX, rect.y);
}

bool SliderHorizontal::resize(u16 width, u16 height) {

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
