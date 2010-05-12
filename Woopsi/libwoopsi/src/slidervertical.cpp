#include "slidervertical.h"
#include "sliderverticalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderVertical::SliderVertical(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;
	_minimumGripHeight = 10;
	_pageSize = 1;

	_flags.permeable = true;
	_flags.borderless = false;
	_flags.doubleClickable = false;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderVerticalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->addGadgetEventHandler(this);
	addGadget(_grip);

	_gutterHeight = rect.height;
}

const s16 SliderVertical::getGripValue() const {

	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);
	
	// Is the grip smaller than the gutter?
	if (rect.height > _grip->getHeight()) {
	
		// Calculate ratio
		u32 ratio = ((_maximumValue - _minimumValue) << 16) / _gutterHeight;
		
		// Calculate value
		u32 val = ((_grip->getY() - getY()) - rect.y) * ratio;

		// Round up
		val += val & 0x8000;

		// Right shift to erase fractional part and return
		return val >> 16;
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
	if ((rect.height > _grip->getHeight()) && (_maximumValue != _minimumValue)) {
	
		// Calculate ratio
		u32 ratio = (_gutterHeight << 16) / (u32)(_maximumValue - _minimumValue);
		
		// Convert value using ratio
		s32 newGripY = (newValue * ratio);

		// Round up
		newGripY += newGripY & 0x8000;

		// Bitshift down
		newGripY >>= 16;

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

void SliderVertical::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, _width, _height, getDarkColour());
}

void SliderVertical::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, _width, _height, getShadowColour(), getShineColour());
}

void SliderVertical::onClick(s16 x, s16 y) {

	// Which way should the grip move?
	if (y > _grip->getY()) {
		// Move grip down
		setValue(getValue() + _pageSize);
	} else {
		// Move grip up
		setValue(getValue() - _pageSize);
	}
}

void SliderVertical::handleDragEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s16 newValue = getGripValue();

		// Grip has moved
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderVertical::handleMoveEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s16 newValue = getGripValue();

		// Grip has moved
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
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
		u32 ratio = (rect.height << 16) / (u32)(_maximumValue - _minimumValue);
		
		// New height is equivalent to the height of the gutter minus
		// the ratio-converted overflow height
		newHeight = (rect.height << 16) - (overspill * ratio);

		// Round up
		newHeight += newHeight & 0x8000;

		// Bitshift to remove fraction
		newHeight >>= 16;

		// Calculate height of the gutter for use in ratio calculations
		_gutterHeight = rect.height;

		// Ensure height is within acceptable boundaries
		if (newHeight < _minimumGripHeight) {

			// Adjust calculated height of gutter to take into account
			// adjusted grip height.  Since we limit the height of the grip
			// to ensure it doesn't get too small, we must treat the gutter
			// as if it shrinks by the amount that the grip has been enlarged
			_gutterHeight -= _minimumGripHeight - newHeight;

			// Limit height of grip
			newHeight = _minimumGripHeight;
		}

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

void SliderVertical::onResize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool events = raisesEvents();

	// Disable event raising
	setRaisesEvents(false);

	resizeGrip();

	// Set back to current value
	setValue(value);

	// Reset event raising
	setRaisesEvents(events);
}

s16 SliderVertical::getValuesPerPixel() const {
	return ((_maximumValue - _minimumValue) / _gutterHeight) + 1;
}
